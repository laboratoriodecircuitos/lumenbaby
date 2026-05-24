/*
  ==============================================================================
  LumenBABY — Luminária Nuvem Inteligente
  ==============================================================================

  Firmware principal da LumenBABY, luminária decorativa infantil em formato de
  nuvem desenvolvida pela LumenSense.

  O sistema controla uma fita LED por PWM através de MOSFET, utilizando leitura
  de luminosidade ambiente por LDR e interface local com dois botões físicos.

  Funcionalidades principais:
  - controle automático de iluminação com LDR;
  - transições suaves de acendimento e apagamento;
  - modo manual com múltiplos níveis de brilho;
  - modo respiração com variação suave de intensidade;
  - persistência de ajustes de brilho na EEPROM;
  - inicialização em estado seguro com saída PWM desligada.

  Hardware-alvo:
  - Arduino Pro Mini 5V / 16 MHz;
  - LDR no pino A0;
  - botão de modo no pino D2;
  - botão de brilho no pino D3;
  - saída PWM para MOSFET no pino D9.

  Aviso de uso:
  Este firmware faz parte de um projeto educacional, documental e em validação.
  A LumenBABY não deve ser apresentada como brinquedo, produto médico, monitor
  de bebê ou dispositivo de segurança crítica.

  Autor:
  Rafael Albuquerque — Laboratório de Circuitos

  Licença:
  LumenBABY Source Available License.
  Consulte o arquivo LICENSE.md para permissões e restrições de uso.

  Repositório:
  https://github.com/laboratoriodecircuitos/lumenbaby
*/

#include <EEPROM.h>

// ---------------------------------------------------------------------------
// Configurações principais
// ---------------------------------------------------------------------------

const bool DEBUG_SERIAL_ENABLED = true;
const unsigned long SERIAL_BAUD_RATE = 9600;
const unsigned long DEBUG_REPORT_INTERVAL_MS = 1000;

// false = comportamento normal esperado para MOSFET N-channel low-side bem
// montado, onde PWM maior aumenta brilho.
// true = comportamento invertido/ativo-baixo, onde PWM físico menor aumenta
// brilho. Revisar esta configuração após a validação elétrica final.
const bool LED_PWM_OUTPUT_INVERTED = false;

const byte LDR_PIN = A0;
const byte MODE_BUTTON_PIN = 2;
const byte BRIGHTNESS_BUTTON_PIN = 3;
const byte LED_PWM_PIN = 9;

const unsigned long DEBOUNCE_TIME_MS = 40;

// Limiares iniciais do LDR. A calibração fina será feita em etapa futura.
// A histerese evita alternância quando a leitura fica perto do limite.
const int LDR_DARK_THRESHOLD = 450;
const int LDR_LIGHT_THRESHOLD = 600;
const unsigned long AUTOMATIC_RESPONSE_DELAY_MS = 1000;
const unsigned long AUTOMATIC_FADE_DURATION_MS = 1000;

const byte BRIGHTNESS_LEVELS[] = {
  25, 50, 75, 100, 125, 150, 175, 200, 225, 255
};
const byte BRIGHTNESS_LEVEL_COUNT =
  sizeof(BRIGHTNESS_LEVELS) / sizeof(BRIGHTNESS_LEVELS[0]);
const unsigned long MANUAL_LEVEL_FADE_DURATION_MS = 350;
const unsigned long MANUAL_SHORT_PRESS_MAX_MS = 600;
const unsigned long MANUAL_HOLD_START_MS = 600;
const unsigned long MANUAL_HOLD_STEP_TIME_MS = 25;
const byte MANUAL_HOLD_STEP_AMOUNT = 2;
const byte DEFAULT_MANUAL_BRIGHTNESS_INDEX = 5;
const byte DEFAULT_AUTOMATIC_BRIGHTNESS_INDEX = 6;

const byte EEPROM_MAGIC = 0x4C;  // "L" de LumenBABY/LumenSense.
const byte EEPROM_DATA_VERSION = 1;
const int EEPROM_MAGIC_ADDRESS = 0;
const int EEPROM_VERSION_ADDRESS = 1;
const int EEPROM_AUTOMATIC_BRIGHTNESS_INDEX_ADDRESS = 2;
const int EEPROM_MANUAL_BRIGHTNESS_INDEX_ADDRESS = 3;
const unsigned long EEPROM_SAVE_DELAY_MS = 2000;

// Efeito visual inspirado no ritmo 4-7-8: apenas referência visual.
const byte BREATHING_MIN_BRIGHTNESS = 10;
const byte BREATHING_MAX_BRIGHTNESS = 170;
const unsigned long BREATHING_INHALE_MS = 4000;
const unsigned long BREATHING_HOLD_MS = 7000;
const unsigned long BREATHING_EXHALE_MS = 8000;
const unsigned long BREATHING_UPDATE_INTERVAL_MS = 20;

enum OperationMode {
  MODE_AUTO,
  MODE_BREATHING,
  MODE_MANUAL
};

enum BreathingPhase {
  BREATHING_PHASE_INHALE,
  BREATHING_PHASE_HOLD,
  BREATHING_PHASE_EXHALE
};

struct DebouncedButton {
  byte pin;
  bool stableState;
  bool lastReading;
  bool isPressed;
  unsigned long lastChangeTime;
  unsigned long pressedStartTime;
  unsigned long lastPressDuration;
  bool pressedEvent;
  bool releasedEvent;
};

OperationMode currentMode = MODE_AUTO;
bool debugSerialReady = false;

DebouncedButton modeButton = {
  MODE_BUTTON_PIN,
  HIGH,
  HIGH,
  false,
  0,
  0,
  0,
  false,
  false
};

DebouncedButton brightnessButton = {
  BRIGHTNESS_BUTTON_PIN,
  HIGH,
  HIGH,
  false,
  0,
  0,
  0,
  false,
  false
};

byte manualBrightnessIndex = DEFAULT_MANUAL_BRIGHTNESS_INDEX;
byte automaticBrightnessIndex = DEFAULT_AUTOMATIC_BRIGHTNESS_INDEX;
bool brightnessHoldActive = false;
bool brightnessHoldWasActiveOnThisPress = false;
int brightnessHoldDirection = 1;
byte brightnessHoldValue = 0;
byte brightnessHoldStartIndex = 0;
unsigned long lastBrightnessHoldStepTime = 0;

bool brightnessSettingsSavePending = false;
unsigned long brightnessSettingsSaveRequestTime = 0;

byte currentLedBrightness = 0;  // Brilho lógico: 0 sempre significa apagado.
byte currentPhysicalPwmOutput = 0;
byte ledFadeStartBrightness = 0;
byte ledFadeTargetBrightness = 0;
unsigned long ledFadeStartTime = 0;
unsigned long ledFadeDuration = 0;
bool ledFadeActive = false;

bool automaticStableEnvironmentDark = false;
bool automaticPendingTransition = false;
bool automaticPendingEnvironmentDark = false;
unsigned long automaticPendingStartTime = 0;
int lastAutomaticLdrValue = 0;
bool lastAutomaticEnvironmentDark = false;
byte lastAutomaticTargetBrightness = 0;

unsigned long breathingCycleStartTime = 0;
unsigned long lastBreathingUpdateTime = 0;
BreathingPhase currentBreathingPhase = BREATHING_PHASE_INHALE;

unsigned long lastDebugReportTime = 0;

void setupPins();
void setupDebugSerial();
void loadBrightnessSettingsFromEeprom();
void scheduleBrightnessSettingsSave();
void updateBrightnessSettingsSave();
void saveBrightnessSettingsToEeprom();
bool isValidBrightnessIndex(byte index);
byte sanitizeBrightnessIndex(byte index, byte fallbackIndex);
byte sanitizeLogicalBrightness(int logicalBrightness);
void forceLedOff();
void resetAutomaticPendingState();
int getEffectiveLdrLightThreshold();
int readLdr();
void updateButtons();
void updateButton(DebouncedButton &button);
void clearButtonEvents(DebouncedButton &button);
void handleModeButtonPressed();
void handleBrightnessButtonReleased();
void handleBrightnessButtonShortPress();
void handleManualBrightnessShortPress();
void handleAutomaticBrightnessShortPress();
void updateBrightnessButtonHold();
void resetBrightnessHoldState();
bool isBrightnessControlMode();
byte getAutomaticTargetBrightness();
byte getBrightnessHoldStartValue();
void applyBrightnessHoldValue(byte brightness);
void syncActiveBrightnessIndex(byte brightness);
byte findNearestBrightnessIndex(byte brightness);
void requestLedBrightness(byte targetBrightness, unsigned long durationMs);
void updateLedFade();
byte easeInOutByte(unsigned long elapsedMs, unsigned long durationMs);
void cancelLedFade();
void applyLedBrightness(int brightness);
byte logicalBrightnessToPhysicalPwm(byte logicalBrightness);
void updateCurrentMode();
void handleModeChanged();
void updateAutomaticMode();
void updateAutomaticTarget();
bool isEnvironmentDark(int ldrValue);
bool determineAutomaticEnvironmentDark(int ldrValue);
byte calculateAutomaticBrightness(int ldrValue);
void resetAutomaticModeState();
void updateBreathing478Mode();
void updateManualMode();
void updateDebugSerial();
void printDebugStatus();
const __FlashStringHelper *currentModeName();
const __FlashStringHelper *breathingPhaseName();
const __FlashStringHelper *automaticStateName();

void setup() {
  setupPins();
  setupDebugSerial();
  loadBrightnessSettingsFromEeprom();
  resetAutomaticModeState();

  if (debugSerialReady) {
    Serial.println(F("LumenBABY iniciado em modo automatico."));
  }
}

void loop() {
  updateButtons();
  updateCurrentMode();
  updateLedFade();
  updateBrightnessSettingsSave();
  updateDebugSerial();
}

void setupPins() {
  // Estado seguro: a saída do MOSFET é configurada e recebe brilho lógico 0.
  pinMode(LED_PWM_PIN, OUTPUT);
  forceLedOff();

  pinMode(MODE_BUTTON_PIN, INPUT_PULLUP);
  pinMode(BRIGHTNESS_BUTTON_PIN, INPUT_PULLUP);
}

void setupDebugSerial() {
  if (DEBUG_SERIAL_ENABLED) {
    Serial.begin(SERIAL_BAUD_RATE);
    debugSerialReady = true;
    Serial.println(F("LumenBABY diagnostico Serial ativo."));
    Serial.println(F("Etapa 07: reforcos defensivos de seguranca no firmware."));
    Serial.println(F("Sem fita LED, MT3608, TP4056, baterias ou fonte externa."));
    Serial.println(F("Botoes: INPUT_PULLUP, pressionado = LOW."));
    Serial.println(F("Saida PWM D9: normal."));

    if (LDR_LIGHT_THRESHOLD <= LDR_DARK_THRESHOLD) {
      Serial.println(F("Aviso: limiares LDR ajustados para comportamento previsivel."));
    }
  }
}

void loadBrightnessSettingsFromEeprom() {
  byte storedMagic = EEPROM.read(EEPROM_MAGIC_ADDRESS);
  byte storedVersion = EEPROM.read(EEPROM_VERSION_ADDRESS);
  byte storedAutomaticBrightnessIndex =
    EEPROM.read(EEPROM_AUTOMATIC_BRIGHTNESS_INDEX_ADDRESS);
  byte storedManualBrightnessIndex =
    EEPROM.read(EEPROM_MANUAL_BRIGHTNESS_INDEX_ADDRESS);

  bool storedSettingsValid =
    storedMagic == EEPROM_MAGIC &&
    storedVersion == EEPROM_DATA_VERSION &&
    isValidBrightnessIndex(storedAutomaticBrightnessIndex) &&
    isValidBrightnessIndex(storedManualBrightnessIndex);

  if (storedSettingsValid) {
    automaticBrightnessIndex = sanitizeBrightnessIndex(
      storedAutomaticBrightnessIndex,
      DEFAULT_AUTOMATIC_BRIGHTNESS_INDEX
    );
    manualBrightnessIndex = sanitizeBrightnessIndex(
      storedManualBrightnessIndex,
      DEFAULT_MANUAL_BRIGHTNESS_INDEX
    );

    if (debugSerialReady) {
      Serial.print(F("EEPROM carregada. Auto indice: "));
      Serial.print(automaticBrightnessIndex);
      Serial.print(F(" | Manual indice: "));
      Serial.println(manualBrightnessIndex);
    }

    return;
  }

  automaticBrightnessIndex = sanitizeBrightnessIndex(
    DEFAULT_AUTOMATIC_BRIGHTNESS_INDEX,
    0
  );
  manualBrightnessIndex = sanitizeBrightnessIndex(
    DEFAULT_MANUAL_BRIGHTNESS_INDEX,
    0
  );
  scheduleBrightnessSettingsSave();

  if (debugSerialReady) {
    Serial.print(F("EEPROM invalida ou vazia. Usando padroes. Auto indice: "));
    Serial.print(automaticBrightnessIndex);
    Serial.print(F(" | Manual indice: "));
    Serial.println(manualBrightnessIndex);
  }
}

void scheduleBrightnessSettingsSave() {
  brightnessSettingsSavePending = true;
  brightnessSettingsSaveRequestTime = millis();
}

void updateBrightnessSettingsSave() {
  if (!brightnessSettingsSavePending) {
    return;
  }

  unsigned long now = millis();

  if (brightnessButton.isPressed || brightnessHoldActive) {
    brightnessSettingsSaveRequestTime = now;
    return;
  }

  if (ledFadeActive || automaticPendingTransition) {
    brightnessSettingsSaveRequestTime = now;
    return;
  }

  if ((now - brightnessSettingsSaveRequestTime) < EEPROM_SAVE_DELAY_MS) {
    return;
  }

  saveBrightnessSettingsToEeprom();
}

void saveBrightnessSettingsToEeprom() {
  automaticBrightnessIndex = sanitizeBrightnessIndex(
    automaticBrightnessIndex,
    DEFAULT_AUTOMATIC_BRIGHTNESS_INDEX
  );
  manualBrightnessIndex = sanitizeBrightnessIndex(
    manualBrightnessIndex,
    DEFAULT_MANUAL_BRIGHTNESS_INDEX
  );

  EEPROM.update(EEPROM_MAGIC_ADDRESS, EEPROM_MAGIC);
  EEPROM.update(EEPROM_VERSION_ADDRESS, EEPROM_DATA_VERSION);
  EEPROM.update(
    EEPROM_AUTOMATIC_BRIGHTNESS_INDEX_ADDRESS,
    automaticBrightnessIndex
  );
  EEPROM.update(EEPROM_MANUAL_BRIGHTNESS_INDEX_ADDRESS, manualBrightnessIndex);

  brightnessSettingsSavePending = false;

  if (debugSerialReady) {
    Serial.print(F("EEPROM salva. Auto indice: "));
    Serial.print(automaticBrightnessIndex);
    Serial.print(F(" | Manual indice: "));
    Serial.println(manualBrightnessIndex);
  }
}

bool isValidBrightnessIndex(byte index) {
  return index < BRIGHTNESS_LEVEL_COUNT;
}

byte sanitizeBrightnessIndex(byte index, byte fallbackIndex) {
  if (isValidBrightnessIndex(index)) {
    return index;
  }

  if (isValidBrightnessIndex(fallbackIndex)) {
    return fallbackIndex;
  }

  return 0;
}

byte sanitizeLogicalBrightness(int logicalBrightness) {
  // Mantem a intencao explicita: brilho logico valido fica sempre em 0-255.
  if (logicalBrightness < 0) {
    return 0;
  }

  if (logicalBrightness > 255) {
    return 255;
  }

  return (byte)logicalBrightness;
}

void forceLedOff() {
  ledFadeActive = false;
  ledFadeStartBrightness = 0;
  ledFadeTargetBrightness = 0;
  currentLedBrightness = 0;
  currentPhysicalPwmOutput = logicalBrightnessToPhysicalPwm(0);
  analogWrite(LED_PWM_PIN, currentPhysicalPwmOutput);
}

void resetAutomaticPendingState() {
  automaticPendingTransition = false;
  automaticPendingEnvironmentDark = false;
  automaticPendingStartTime = 0;
}

int getEffectiveLdrLightThreshold() {
  if (LDR_LIGHT_THRESHOLD > LDR_DARK_THRESHOLD) {
    return LDR_LIGHT_THRESHOLD;
  }

  return LDR_DARK_THRESHOLD;
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

  if (brightnessButton.releasedEvent) {
    brightnessButton.releasedEvent = false;
    handleBrightnessButtonReleased();
  }

  brightnessButton.pressedEvent = false;
  modeButton.releasedEvent = false;
}

void updateButton(DebouncedButton &button) {
  bool reading = digitalRead(button.pin);
  unsigned long now = millis();

  if (reading != button.lastReading) {
    button.lastChangeTime = now;
    button.lastReading = reading;
  }

  if ((now - button.lastChangeTime) < DEBOUNCE_TIME_MS) {
    return;
  }

  if (reading == button.stableState) {
    return;
  }

  button.stableState = reading;

  if (button.stableState == LOW) {
    button.isPressed = true;
    button.pressedStartTime = now;
    button.pressedEvent = true;
  } else {
    if (button.isPressed) {
      button.lastPressDuration = now - button.pressedStartTime;
      button.releasedEvent = true;
    }

    button.isPressed = false;
  }
}

void clearButtonEvents(DebouncedButton &button) {
  button.pressedEvent = false;
  button.releasedEvent = false;
}

void handleModeButtonPressed() {
  if (currentMode == MODE_AUTO) {
    currentMode = MODE_BREATHING;
  } else if (currentMode == MODE_BREATHING) {
    currentMode = MODE_MANUAL;
  } else {
    currentMode = MODE_AUTO;
  }

  handleModeChanged();

  if (debugSerialReady) {
    Serial.print(F("Modo alterado para: "));
    Serial.println(currentModeName());
  }
}

void handleBrightnessButtonReleased() {
  if (!isBrightnessControlMode()) {
    resetBrightnessHoldState();
    return;
  }

  if (brightnessHoldWasActiveOnThisPress) {
    brightnessHoldDirection = -brightnessHoldDirection;
    syncActiveBrightnessIndex(brightnessHoldValue);

    byte finalBrightnessIndex = currentMode == MODE_AUTO
      ? automaticBrightnessIndex
      : manualBrightnessIndex;

    if (finalBrightnessIndex != brightnessHoldStartIndex) {
      scheduleBrightnessSettingsSave();
    }

    resetBrightnessHoldState();
    return;
  }

  if (brightnessButton.lastPressDuration < MANUAL_SHORT_PRESS_MAX_MS) {
    handleBrightnessButtonShortPress();
  }
}

void handleBrightnessButtonShortPress() {
  if (currentMode == MODE_AUTO) {
    handleAutomaticBrightnessShortPress();
  } else if (currentMode == MODE_MANUAL) {
    handleManualBrightnessShortPress();
  }
}

void handleManualBrightnessShortPress() {
  manualBrightnessIndex = sanitizeBrightnessIndex(
    manualBrightnessIndex,
    DEFAULT_MANUAL_BRIGHTNESS_INDEX
  );
  manualBrightnessIndex++;

  if (manualBrightnessIndex >= BRIGHTNESS_LEVEL_COUNT) {
    manualBrightnessIndex = 0;
  }

  requestLedBrightness(
    BRIGHTNESS_LEVELS[manualBrightnessIndex],
    MANUAL_LEVEL_FADE_DURATION_MS
  );
  scheduleBrightnessSettingsSave();

  if (debugSerialReady) {
    Serial.print(F("Nivel manual: "));
    Serial.println(manualBrightnessIndex);
  }
}

void handleAutomaticBrightnessShortPress() {
  automaticBrightnessIndex = sanitizeBrightnessIndex(
    automaticBrightnessIndex,
    DEFAULT_AUTOMATIC_BRIGHTNESS_INDEX
  );
  automaticBrightnessIndex++;

  if (automaticBrightnessIndex >= BRIGHTNESS_LEVEL_COUNT) {
    automaticBrightnessIndex = 0;
  }

  byte targetBrightness = getAutomaticTargetBrightness();
  lastAutomaticTargetBrightness = targetBrightness;

  if (automaticStableEnvironmentDark) {
    requestLedBrightness(targetBrightness, MANUAL_LEVEL_FADE_DURATION_MS);
  }
  scheduleBrightnessSettingsSave();

  if (debugSerialReady) {
    Serial.print(F("Nivel automatico: "));
    Serial.print(automaticBrightnessIndex);
    Serial.print(F(" | Alvo automatico: "));
    Serial.println(targetBrightness);
  }
}

void updateBrightnessButtonHold() {
  if (!brightnessButton.isPressed) {
    return;
  }

  unsigned long now = millis();
  unsigned long heldTime = now - brightnessButton.pressedStartTime;

  if (heldTime < MANUAL_HOLD_START_MS) {
    return;
  }

  if (!brightnessHoldActive) {
    brightnessHoldActive = true;
    brightnessHoldWasActiveOnThisPress = true;
    brightnessHoldValue = getBrightnessHoldStartValue();
    brightnessHoldStartIndex = currentMode == MODE_AUTO
      ? automaticBrightnessIndex
      : manualBrightnessIndex;
    lastBrightnessHoldStepTime = 0;

    if (currentMode == MODE_MANUAL || automaticStableEnvironmentDark) {
      cancelLedFade();
    }
  }

  if ((now - lastBrightnessHoldStepTime) < MANUAL_HOLD_STEP_TIME_MS) {
    return;
  }

  lastBrightnessHoldStepTime = now;

  int nextBrightness =
    brightnessHoldValue + (brightnessHoldDirection * MANUAL_HOLD_STEP_AMOUNT);

  if (nextBrightness > BRIGHTNESS_LEVELS[BRIGHTNESS_LEVEL_COUNT - 1]) {
    nextBrightness = BRIGHTNESS_LEVELS[BRIGHTNESS_LEVEL_COUNT - 1];
  }

  if (nextBrightness < BRIGHTNESS_LEVELS[0]) {
    nextBrightness = BRIGHTNESS_LEVELS[0];
  }

  brightnessHoldValue = nextBrightness;
  applyBrightnessHoldValue(brightnessHoldValue);
}

void resetBrightnessHoldState() {
  brightnessHoldActive = false;
  brightnessHoldWasActiveOnThisPress = false;
  brightnessHoldStartIndex = 0;
  clearButtonEvents(brightnessButton);
}

bool isBrightnessControlMode() {
  return currentMode == MODE_AUTO || currentMode == MODE_MANUAL;
}

byte getAutomaticTargetBrightness() {
  automaticBrightnessIndex = sanitizeBrightnessIndex(
    automaticBrightnessIndex,
    DEFAULT_AUTOMATIC_BRIGHTNESS_INDEX
  );
  return BRIGHTNESS_LEVELS[automaticBrightnessIndex];
}

byte getBrightnessHoldStartValue() {
  if (currentMode == MODE_AUTO && !automaticStableEnvironmentDark) {
    return getAutomaticTargetBrightness();
  }

  return currentLedBrightness;
}

void applyBrightnessHoldValue(byte brightness) {
  if (currentMode == MODE_AUTO) {
    automaticBrightnessIndex = sanitizeBrightnessIndex(
      findNearestBrightnessIndex(brightness),
      DEFAULT_AUTOMATIC_BRIGHTNESS_INDEX
    );
    lastAutomaticTargetBrightness = getAutomaticTargetBrightness();

    if (automaticStableEnvironmentDark) {
      applyLedBrightness(brightness);
    }

    return;
  }

  applyLedBrightness(brightness);
}

void syncActiveBrightnessIndex(byte brightness) {
  if (currentMode == MODE_AUTO) {
    automaticBrightnessIndex = sanitizeBrightnessIndex(
      findNearestBrightnessIndex(brightness),
      DEFAULT_AUTOMATIC_BRIGHTNESS_INDEX
    );
    lastAutomaticTargetBrightness = getAutomaticTargetBrightness();
    return;
  }

  manualBrightnessIndex = sanitizeBrightnessIndex(
    findNearestBrightnessIndex(brightness),
    DEFAULT_MANUAL_BRIGHTNESS_INDEX
  );
}

byte findNearestBrightnessIndex(byte brightness) {
  byte nearestIndex = 0;
  int nearestDistance = 300;

  for (byte i = 0; i < BRIGHTNESS_LEVEL_COUNT; i++) {
    int distance = brightness > BRIGHTNESS_LEVELS[i]
      ? brightness - BRIGHTNESS_LEVELS[i]
      : BRIGHTNESS_LEVELS[i] - brightness;

    if (distance < nearestDistance) {
      nearestDistance = distance;
      nearestIndex = i;
    }
  }

  return nearestIndex;
}

void requestLedBrightness(byte targetBrightness, unsigned long durationMs) {
  targetBrightness = sanitizeLogicalBrightness(targetBrightness);
  ledFadeTargetBrightness = targetBrightness;

  if (durationMs == 0 || currentLedBrightness == targetBrightness) {
    ledFadeActive = false;
    applyLedBrightness(targetBrightness);
    return;
  }

  ledFadeStartBrightness = currentLedBrightness;
  ledFadeStartTime = millis();
  ledFadeDuration = durationMs;
  ledFadeActive = true;
}

void updateLedFade() {
  if (!ledFadeActive) {
    return;
  }

  unsigned long elapsed = millis() - ledFadeStartTime;

  if (elapsed >= ledFadeDuration) {
    ledFadeActive = false;
    applyLedBrightness(ledFadeTargetBrightness);
    return;
  }

  byte easedProgress = easeInOutByte(elapsed, ledFadeDuration);
  long brightnessRange = (long)ledFadeTargetBrightness - ledFadeStartBrightness;
  int nextBrightness =
    ledFadeStartBrightness + ((brightnessRange * easedProgress) / 255);

  applyLedBrightness(nextBrightness);
}

byte easeInOutByte(unsigned long elapsedMs, unsigned long durationMs) {
  if (durationMs == 0 || elapsedMs >= durationMs) {
    return 255;
  }

  unsigned long progress = (elapsedMs * 255UL) / durationMs;
  unsigned long eased =
    (progress * progress * (3UL * 255UL - 2UL * progress)) / (255UL * 255UL);

  return eased;
}

void cancelLedFade() {
  ledFadeActive = false;
  ledFadeTargetBrightness = currentLedBrightness;
}

void applyLedBrightness(int brightness) {
  byte safeBrightness = sanitizeLogicalBrightness(brightness);
  byte nextPhysicalPwmOutput = logicalBrightnessToPhysicalPwm(safeBrightness);

  if (!ledFadeActive) {
    ledFadeTargetBrightness = safeBrightness;
  }

  currentLedBrightness = safeBrightness;

  if (nextPhysicalPwmOutput == currentPhysicalPwmOutput) {
    return;
  }

  currentPhysicalPwmOutput = nextPhysicalPwmOutput;
  analogWrite(LED_PWM_PIN, currentPhysicalPwmOutput);
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
    updateBreathing478Mode();
  } else {
    updateManualMode();
  }
}

void handleModeChanged() {
  resetBrightnessHoldState();
  resetAutomaticPendingState();

  if (currentMode == MODE_AUTO) {
    resetAutomaticModeState();
  } else if (currentMode == MODE_BREATHING) {
    cancelLedFade();
    breathingCycleStartTime = millis();
    lastBreathingUpdateTime = 0;
    currentBreathingPhase = BREATHING_PHASE_INHALE;
  } else {
    manualBrightnessIndex = sanitizeBrightnessIndex(
      manualBrightnessIndex,
      DEFAULT_MANUAL_BRIGHTNESS_INDEX
    );
    requestLedBrightness(
      BRIGHTNESS_LEVELS[manualBrightnessIndex],
      MANUAL_LEVEL_FADE_DURATION_MS
    );
  }
}

void updateAutomaticMode() {
  updateBrightnessButtonHold();
  updateAutomaticTarget();
}

void updateAutomaticTarget() {
  int ldrValue = readLdr();
  bool environmentDark = determineAutomaticEnvironmentDark(ldrValue);
  unsigned long now = millis();

  lastAutomaticLdrValue = ldrValue;
  lastAutomaticEnvironmentDark = environmentDark;

  if (environmentDark == automaticStableEnvironmentDark) {
    resetAutomaticPendingState();
    return;
  }

  if (!automaticPendingTransition ||
      automaticPendingEnvironmentDark != environmentDark) {
    automaticPendingTransition = true;
    automaticPendingEnvironmentDark = environmentDark;
    automaticPendingStartTime = now;
    return;
  }

  if ((now - automaticPendingStartTime) < AUTOMATIC_RESPONSE_DELAY_MS) {
    return;
  }

  automaticStableEnvironmentDark = environmentDark;
  automaticPendingTransition = false;
  lastAutomaticTargetBrightness = automaticStableEnvironmentDark
    ? getAutomaticTargetBrightness()
    : 0;
  requestLedBrightness(lastAutomaticTargetBrightness, AUTOMATIC_FADE_DURATION_MS);
}

bool isEnvironmentDark(int ldrValue) {
  return ldrValue < LDR_DARK_THRESHOLD;
}

bool determineAutomaticEnvironmentDark(int ldrValue) {
  int effectiveLightThreshold = getEffectiveLdrLightThreshold();

  if (automaticStableEnvironmentDark) {
    return ldrValue <= effectiveLightThreshold;
  }

  return ldrValue < LDR_DARK_THRESHOLD;
}

byte calculateAutomaticBrightness(int ldrValue) {
  if (isEnvironmentDark(ldrValue)) {
    return getAutomaticTargetBrightness();
  }

  return 0;
}

void resetAutomaticModeState() {
  int ldrValue = readLdr();
  bool environmentDark = isEnvironmentDark(ldrValue);
  byte targetBrightness = calculateAutomaticBrightness(ldrValue);

  // Só força uma transição inicial se o brilho atual ainda não combina com o LDR.
  if (currentLedBrightness == targetBrightness) {
    automaticStableEnvironmentDark = environmentDark;
  } else {
    automaticStableEnvironmentDark = !environmentDark;
  }

  resetAutomaticPendingState();
  lastAutomaticLdrValue = ldrValue;
  lastAutomaticEnvironmentDark = environmentDark;
  lastAutomaticTargetBrightness = targetBrightness;
}

void updateBreathing478Mode() {
  unsigned long now = millis();

  if ((now - lastBreathingUpdateTime) < BREATHING_UPDATE_INTERVAL_MS) {
    return;
  }

  lastBreathingUpdateTime = now;

  unsigned long cycleDuration =
    BREATHING_INHALE_MS + BREATHING_HOLD_MS + BREATHING_EXHALE_MS;
  unsigned long cyclePosition = (now - breathingCycleStartTime) % cycleDuration;
  byte brightness = BREATHING_MIN_BRIGHTNESS;

  if (cyclePosition < BREATHING_INHALE_MS) {
    currentBreathingPhase = BREATHING_PHASE_INHALE;
    byte progress = easeInOutByte(cyclePosition, BREATHING_INHALE_MS);
    brightness = BREATHING_MIN_BRIGHTNESS +
      ((((unsigned long)BREATHING_MAX_BRIGHTNESS - BREATHING_MIN_BRIGHTNESS) *
        progress) / 255);
  } else if (cyclePosition < (BREATHING_INHALE_MS + BREATHING_HOLD_MS)) {
    currentBreathingPhase = BREATHING_PHASE_HOLD;
    brightness = BREATHING_MAX_BRIGHTNESS;
  } else {
    currentBreathingPhase = BREATHING_PHASE_EXHALE;
    unsigned long exhalePosition =
      cyclePosition - BREATHING_INHALE_MS - BREATHING_HOLD_MS;
    byte progress = easeInOutByte(exhalePosition, BREATHING_EXHALE_MS);
    brightness = BREATHING_MAX_BRIGHTNESS -
      ((((unsigned long)BREATHING_MAX_BRIGHTNESS - BREATHING_MIN_BRIGHTNESS) *
        progress) / 255);
  }

  applyLedBrightness(brightness);
}

void updateManualMode() {
  updateBrightnessButtonHold();
}

void updateDebugSerial() {
  if (!debugSerialReady) {
    return;
  }

  if (ledFadeActive) {
    return;
  }

  if (automaticPendingTransition) {
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
  bool displayedEnvironmentDark = currentMode == MODE_AUTO
    ? determineAutomaticEnvironmentDark(ldrValue)
    : isEnvironmentDark(ldrValue);

  Serial.print(F("Modo: "));
  Serial.print(currentModeName());
  Serial.print(F(" | LDR A0: "));
  Serial.print(ldrValue);
  Serial.print(F(" | Limiar LDR: "));
  Serial.print(LDR_DARK_THRESHOLD);
  Serial.print(F("/"));
  Serial.print(getEffectiveLdrLightThreshold());
  Serial.print(F(" | Ambiente: "));

  if (displayedEnvironmentDark) {
    Serial.print(F("escuro"));
  } else {
    Serial.print(F("claro"));
  }

  Serial.print(F(" | Brilho logico: "));
  Serial.print(currentLedBrightness);
  Serial.print(F(" | Alvo: "));
  Serial.print(ledFadeTargetBrightness);
  Serial.print(F(" | PWM fisico D9: "));
  Serial.print(currentPhysicalPwmOutput);
  Serial.print(F(" | Saida PWM: "));

  if (LED_PWM_OUTPUT_INVERTED) {
    Serial.print(F("invertida"));
  } else {
    Serial.print(F("normal"));
  }

  Serial.print(F(" | Manual indice: "));
  Serial.print(manualBrightnessIndex);
  Serial.print(F(" | Auto indice: "));
  Serial.print(automaticBrightnessIndex);

  if (currentMode == MODE_AUTO) {
    Serial.print(F(" | Auto: "));
    Serial.print(automaticStateName());
    Serial.print(F(" | Auto alvo: "));
    Serial.print(getAutomaticTargetBrightness());
  }

  if (currentMode == MODE_BREATHING) {
    Serial.print(F(" | Respiracao 4-7-8: "));
    Serial.print(breathingPhaseName());
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

const __FlashStringHelper *breathingPhaseName() {
  if (currentBreathingPhase == BREATHING_PHASE_INHALE) {
    return F("inspiracao");
  }

  if (currentBreathingPhase == BREATHING_PHASE_HOLD) {
    return F("retencao");
  }

  return F("expiracao");
}

const __FlashStringHelper *automaticStateName() {
  if (automaticPendingTransition) {
    return F("aguardando atraso");
  }

  if (ledFadeActive) {
    return F("fade ativo");
  }

  return F("estavel");
}
