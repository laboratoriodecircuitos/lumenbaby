// Projeto LumenBABY — Luminária Nuvem Inteligente
// Marca: LumenSense
// Plataforma alvo: Arduino Pro Mini 5V / 16 MHz
//
// Este arquivo é a base inicial segura e didática do firmware,
// respeitando o documento:
// ../../docs/00_DOCUMENTO_INICIAL_LUMENBABY.md

// ---------------------------------------------------------------------------
// Configurações principais
// ---------------------------------------------------------------------------

const bool DEBUG_SERIAL_ENABLED = true;
const unsigned long SERIAL_BAUD_RATE = 9600;
const unsigned long DEBUG_REPORT_INTERVAL_MS = 1000;

const byte LDR_PIN = A0;
const byte MODE_BUTTON_PIN = 2;
const byte BRIGHTNESS_BUTTON_PIN = 3;
const byte LED_PWM_PIN = 9;

const byte MANUAL_BRIGHTNESS_LOW = 70;
const byte MANUAL_BRIGHTNESS_MEDIUM = 140;
const byte MANUAL_BRIGHTNESS_HIGH = 220;

const unsigned long DEBOUNCE_TIME_MS = 40;

// Limites iniciais para teste. O valor real deve ser ajustado em bancada,
// porque depende da posição do LDR no divisor de tensão.
const int LDR_DARK_THRESHOLD = 450;
const byte AUTOMATIC_BRIGHTNESS = 180;

// Parâmetros simples do modo respiração.
const byte BREATHING_MIN_BRIGHTNESS = 20;
const byte BREATHING_MAX_BRIGHTNESS = 180;
const unsigned long BREATHING_STEP_TIME_MS = 20;

enum OperationMode {
  MODE_AUTO,
  MODE_BREATHING,
  MODE_MANUAL
};

struct DebouncedButton {
  byte pin;
  bool stableState;
  bool lastReading;
  unsigned long lastChangeTime;
  bool pressedEvent;
};

OperationMode currentMode = MODE_AUTO;

DebouncedButton modeButton = {
  MODE_BUTTON_PIN,
  HIGH,
  HIGH,
  0,
  false
};

DebouncedButton brightnessButton = {
  BRIGHTNESS_BUTTON_PIN,
  HIGH,
  HIGH,
  0,
  false
};

const byte manualBrightnessLevels[] = {
  MANUAL_BRIGHTNESS_LOW,
  MANUAL_BRIGHTNESS_MEDIUM,
  MANUAL_BRIGHTNESS_HIGH
};

const byte MANUAL_BRIGHTNESS_LEVEL_COUNT =
  sizeof(manualBrightnessLevels) / sizeof(manualBrightnessLevels[0]);

byte manualBrightnessIndex = 1;
byte currentLedBrightness = 0;

byte breathingBrightness = BREATHING_MIN_BRIGHTNESS;
int breathingDirection = 1;
unsigned long lastBreathingStepTime = 0;
unsigned long lastDebugReportTime = 0;

void setupPins();
void setupDebugSerial();
int readLdr();
void updateButtons();
void updateButton(DebouncedButton &button);
void handleModeButtonPressed();
void handleBrightnessButtonPressed();
void applyLedBrightness(byte brightness);
void updateCurrentMode();
void updateAutomaticMode();
void updateBreathingMode();
void updateManualMode();
void updateDebugSerial();
void printDebugStatus();
const __FlashStringHelper *currentModeName();

void setup() {
  setupPins();
  setupDebugSerial();

  if (DEBUG_SERIAL_ENABLED) {
    Serial.println(F("LumenBABY iniciado em modo automatico."));
  }
}

void loop() {
  updateButtons();
  updateCurrentMode();
  updateDebugSerial();
}

void setupPins() {
  // Estado seguro: a saída do MOSFET é configurada e desligada primeiro.
  pinMode(LED_PWM_PIN, OUTPUT);
  analogWrite(LED_PWM_PIN, 0);
  currentLedBrightness = 0;

  pinMode(MODE_BUTTON_PIN, INPUT_PULLUP);
  pinMode(BRIGHTNESS_BUTTON_PIN, INPUT_PULLUP);
}

void setupDebugSerial() {
  if (DEBUG_SERIAL_ENABLED) {
    Serial.begin(SERIAL_BAUD_RATE);
    Serial.println(F("LumenBABY diagnostico Serial ativo."));
    Serial.println(F("Etapa 03: Arduino sem carga de potencia."));
    Serial.println(F("Nao conectar MOSFET, fita LED, MT3608, TP4056 ou baterias."));
    Serial.println(F("Botoes: INPUT_PULLUP, pressionado = LOW."));
  }
}

int readLdr() {
  return analogRead(LDR_PIN);
}

void updateButtons() {
  updateButton(modeButton);
  updateButton(brightnessButton);

  if (modeButton.pressedEvent) {
    modeButton.pressedEvent = false;
    handleModeButtonPressed();
  }

  if (brightnessButton.pressedEvent) {
    brightnessButton.pressedEvent = false;
    handleBrightnessButtonPressed();
  }
}

void updateButton(DebouncedButton &button) {
  bool reading = digitalRead(button.pin);
  unsigned long now = millis();

  if (reading != button.lastReading) {
    button.lastChangeTime = now;
    button.lastReading = reading;
  }

  if ((now - button.lastChangeTime) >= DEBOUNCE_TIME_MS) {
    if (reading != button.stableState) {
      button.stableState = reading;

      // Com INPUT_PULLUP, o botão pressionado é lido como LOW.
      if (button.stableState == LOW) {
        button.pressedEvent = true;
      }
    }
  }
}

void handleModeButtonPressed() {
  if (currentMode == MODE_AUTO) {
    currentMode = MODE_BREATHING;
  } else if (currentMode == MODE_BREATHING) {
    currentMode = MODE_MANUAL;
  } else {
    currentMode = MODE_AUTO;
  }

  if (DEBUG_SERIAL_ENABLED) {
    Serial.print(F("Modo alterado para: "));
    Serial.println(currentModeName());
  }
}

void handleBrightnessButtonPressed() {
  if (currentMode != MODE_MANUAL) {
    return;
  }

  manualBrightnessIndex++;

  if (manualBrightnessIndex >= MANUAL_BRIGHTNESS_LEVEL_COUNT) {
    manualBrightnessIndex = 0;
  }

  if (DEBUG_SERIAL_ENABLED) {
    Serial.print(F("Nivel manual de brilho: "));
    Serial.println(manualBrightnessIndex);
  }
}

void applyLedBrightness(byte brightness) {
  currentLedBrightness = brightness;
  analogWrite(LED_PWM_PIN, currentLedBrightness);
}

void updateCurrentMode() {
  if (currentMode == MODE_AUTO) {
    updateAutomaticMode();
  } else if (currentMode == MODE_BREATHING) {
    updateBreathingMode();
  } else {
    updateManualMode();
  }
}

void updateAutomaticMode() {
  int ldrValue = readLdr();

  // Base inicial: se a leitura indicar ambiente escuro, acende em brilho fixo.
  // A orientação do divisor pode inverter este comportamento e deve ser testada.
  if (ldrValue < LDR_DARK_THRESHOLD) {
    applyLedBrightness(AUTOMATIC_BRIGHTNESS);
  } else {
    applyLedBrightness(0);
  }
}

void updateBreathingMode() {
  unsigned long now = millis();

  if ((now - lastBreathingStepTime) < BREATHING_STEP_TIME_MS) {
    return;
  }

  lastBreathingStepTime = now;

  int nextBrightness = breathingBrightness + breathingDirection;

  if (nextBrightness >= BREATHING_MAX_BRIGHTNESS) {
    nextBrightness = BREATHING_MAX_BRIGHTNESS;
    breathingDirection = -1;
  } else if (nextBrightness <= BREATHING_MIN_BRIGHTNESS) {
    nextBrightness = BREATHING_MIN_BRIGHTNESS;
    breathingDirection = 1;
  }

  breathingBrightness = nextBrightness;
  applyLedBrightness(breathingBrightness);
}

void updateManualMode() {
  applyLedBrightness(manualBrightnessLevels[manualBrightnessIndex]);
}

void updateDebugSerial() {
  if (!DEBUG_SERIAL_ENABLED) {
    return;
  }

  unsigned long now = millis();

  if ((now - lastDebugReportTime) < DEBUG_REPORT_INTERVAL_MS) {
    return;
  }

  lastDebugReportTime = now;
  printDebugStatus();
}

void printDebugStatus() {
  int ldrValue = readLdr();

  Serial.print(F("Modo: "));
  Serial.print(currentModeName());
  Serial.print(F(" | LDR A0: "));
  Serial.print(ldrValue);
  Serial.print(F(" | PWM D9: "));
  Serial.print(currentLedBrightness);
  Serial.print(F(" | Brilho manual indice: "));
  Serial.print(manualBrightnessIndex);
  Serial.println(F(" | Botoes INPUT_PULLUP: pressionado = LOW"));
}

const __FlashStringHelper *currentModeName() {
  if (currentMode == MODE_AUTO) {
    return F("automatico");
  }

  if (currentMode == MODE_BREATHING) {
    return F("respiracao");
  }

  return F("manual");
}
