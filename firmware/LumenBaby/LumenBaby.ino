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

// false = comportamento normal esperado para MOSFET N-channel low-side bem
// montado, onde PWM maior aumenta brilho.
// true = comportamento invertido/ativo-baixo observado nesta validação, onde
// PWM físico menor aumenta brilho.
// Revisar esta configuração após a validação elétrica final.
const bool LED_PWM_OUTPUT_INVERTED = false;

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
bool debugSerialReady = false;

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
byte currentLedBrightness = 0;  // Brilho lógico: 0 sempre significa apagado.
byte currentPhysicalPwmOutput = 0;

byte breathingBrightness = BREATHING_MIN_BRIGHTNESS;
int breathingDirection = 1;
unsigned long lastBreathingStepTime = 0;
unsigned long lastDebugReportTime = 0;

int lastAutomaticLdrValue = 0;
bool lastAutomaticEnvironmentDark = false;
byte lastAutomaticTargetBrightness = 0;

void setupPins();
void setupDebugSerial();
int readLdr();
void updateButtons();
void updateButton(DebouncedButton &button);
void handleModeButtonPressed();
void handleBrightnessButtonPressed();
void applyLedBrightness(byte brightness);
byte logicalBrightnessToPhysicalPwm(byte logicalBrightness);
void updateCurrentMode();
void updateAutomaticMode();
bool isEnvironmentDark(int ldrValue);
byte calculateAutomaticBrightness(int ldrValue);
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
  // Estado seguro: a saída do MOSFET é configurada e recebe brilho lógico 0.
  pinMode(LED_PWM_PIN, OUTPUT);
  applyLedBrightness(0);

  pinMode(MODE_BUTTON_PIN, INPUT_PULLUP);
  pinMode(BRIGHTNESS_BUTTON_PIN, INPUT_PULLUP);
}

void setupDebugSerial() {
  if (DEBUG_SERIAL_ENABLED) {
    Serial.begin(SERIAL_BAUD_RATE);
    debugSerialReady = true;
    Serial.println(F("LumenBABY diagnostico Serial ativo."));
    Serial.println(F("Etapa 04: validacao de baixa potencia."));
    Serial.println(F("Usar somente MOSFET com LED simples e resistor."));
    Serial.println(F("Sem fita LED, MT3608, TP4056, baterias ou fonte externa."));
    Serial.println(F("Botoes: INPUT_PULLUP, pressionado = LOW."));
    Serial.println(F("Observe LDR claro/escuro para calibrar o limiar depois."));
    Serial.print(F("Saida PWM D9: "));

    if (LED_PWM_OUTPUT_INVERTED) {
      Serial.println(F("invertida"));
    } else {
      Serial.println(F("normal"));
    }
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
  byte previousLedBrightness = currentLedBrightness;
  byte previousPhysicalPwmOutput = currentPhysicalPwmOutput;

  currentLedBrightness = brightness;
  currentPhysicalPwmOutput = logicalBrightnessToPhysicalPwm(currentLedBrightness);
  analogWrite(LED_PWM_PIN, currentPhysicalPwmOutput);

  if (debugSerialReady &&
      (previousLedBrightness != currentLedBrightness ||
       previousPhysicalPwmOutput != currentPhysicalPwmOutput) &&
      currentMode != MODE_BREATHING) {
    Serial.print(F("PWM D9 aplicado | brilho logico: "));
    Serial.print(currentLedBrightness);
    Serial.print(F(" | PWM fisico: "));
    Serial.println(currentPhysicalPwmOutput);
  }
}

byte logicalBrightnessToPhysicalPwm(byte logicalBrightness) {
  if (LED_PWM_OUTPUT_INVERTED) {
    return 255 - logicalBrightness;
  }

  return logicalBrightness;
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
  byte targetBrightness = calculateAutomaticBrightness(ldrValue);

  lastAutomaticLdrValue = ldrValue;
  lastAutomaticEnvironmentDark = isEnvironmentDark(ldrValue);
  lastAutomaticTargetBrightness = targetBrightness;

  applyLedBrightness(targetBrightness);
}

bool isEnvironmentDark(int ldrValue) {
  return ldrValue < LDR_DARK_THRESHOLD;
}

byte calculateAutomaticBrightness(int ldrValue) {
  if (isEnvironmentDark(ldrValue)) {
    return AUTOMATIC_BRIGHTNESS;
  }

  return 0;
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
  if (!debugSerialReady) {
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
  Serial.print(F(" | Limiar LDR: "));
  Serial.print(LDR_DARK_THRESHOLD);
  Serial.print(F(" | Ambiente: "));

  if (isEnvironmentDark(ldrValue)) {
    Serial.print(F("escuro"));
  } else {
    Serial.print(F("claro"));
  }

  Serial.print(F(" | Brilho logico D9: "));
  Serial.print(currentLedBrightness);
  Serial.print(F(" | PWM fisico D9: "));
  Serial.print(currentPhysicalPwmOutput);
  Serial.print(F(" | Saida PWM: "));

  if (LED_PWM_OUTPUT_INVERTED) {
    Serial.print(F("invertida"));
  } else {
    Serial.print(F("normal"));
  }

  Serial.print(F(" | Brilho manual indice: "));
  Serial.print(manualBrightnessIndex);

  if (currentMode == MODE_AUTO) {
    Serial.print(F(" | LDR usado no automatico: "));
    Serial.print(lastAutomaticLdrValue);
    Serial.print(F(" | Ambiente automatico: "));

    if (lastAutomaticEnvironmentDark) {
      Serial.print(F("escuro"));
    } else {
      Serial.print(F("claro"));
    }

    Serial.print(F(" | Brilho alvo automatico: "));
    Serial.print(lastAutomaticTargetBrightness);
    Serial.print(F(" | PWM fisico aplicado: "));
    Serial.print(currentPhysicalPwmOutput);
  }

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
