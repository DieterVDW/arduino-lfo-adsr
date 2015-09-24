#define VALUE_INPUT_PIN A0
#define PARAMETER_INPUT_PIN A1
#define LFO_OUTPUT_PIN 5
#define ADSR_OUTPUT_PIN 9

#define PARAMETER_INPUT_ALLOWANCE 5

#define LFO_RATE_PARAMETER_VALUE 5 // 81
#define LFO_SHAPE_PARAMETER_VALUE 7 // 114
#define LFO_OFFSET_PARAMETER_VALUE 10 // 166
#define LFO_WIDTH_PARAMETER_VALUE 12 // 199
#define LFO_SYNC_PARAMETER_VALUE 14 // 234

#define ADSR_ATTACK_PARAMETER_VALUE 4 // 64
#define ADSR_DECAY_PARAMETER_VALUE 6// 99
#define ADSR_SUSTAIN_PARAMETER_VALUE 8 // 133
#define ADSR_RELEASE_PARAMETER_VALUE 9 // 148
#define ADSR_U1_PARAMETER_VALUE 11 // 181 // Unused for now
#define ADSR_U2_PARAMETER_VALUE 13 // 217 // Unused for now
#define ADSR_REPEAT_PARAMETER_VALUE 15 // 249
#define ADSR_SYNC_PARAMETER_VALUE 16 // 265

#define UPDATE_INTERVAL 10 // ms

#define PARAM_READ_INTERVAL 500 // ms
long lastParamTime;

// LFO parameters
int lfo_rate;
int lfo_shape;
int lfo_offset;
int lfo_width;
boolean lfo_sync;

// Derived values
#define LFO_FREQ_LOW 10 // ms
#define LFO_FREQ_HIGH 10000 // ms
long lfoStart = 0;
int lfoPeriodLength;

// ADSR parameters
int adsr_attack;
int adsr_decay;
int adsr_sustain;
int adsr_release;
boolean adsr_repeat;
boolean adsr_sync;

void setup() {
  pinMode(LFO_OUTPUT_PIN, OUTPUT);
  pinMode(ADSR_OUTPUT_PIN, OUTPUT);
  pinMode(VALUE_INPUT_PIN, INPUT);
  pinMode(PARAMETER_INPUT_PIN, INPUT);
}

void loop() {
  long curTime = millis();

  // Read parameter values every PARAM_READ_INTERVAL ms
  if (curTime > lastParamTime + PARAM_READ_INTERVAL) {
    readParameter();
    lastParamTime = curTime;
  }

  // Set LFO voltage
  adjustLFO(curTime);

  // Set ADSR voltage
  adjustADSR(curTime);
  
  delay(UPDATE_INTERVAL - (curTime % UPDATE_INTERVAL));
}

void adjustLFO(long curTime) {
  int position = (curTime - lfoStart) % lfoPeriodLength;

  // Only saw wave for now
  int value = map(position, 0, lfoPeriodLength, 0, 256);
  
  analogWrite(LFO_OUTPUT_PIN, value);
}

void adjustADSR(long curTime) {
  
}

void readParameter() {
  int parameterChoice = analogRead(PARAMETER_INPUT_PIN);
  if (parameterChoice != 0) {
    handleParameter(parameterChoice);
  }
}

void handleParameter(int paramChoice) {
  // Quantize paramChoice
  int quantChoice = map(paramChoice, 0, 1024, 0, 64);
  int paramValue = analogRead(VALUE_INPUT_PIN);

  switch (quantChoice) {
    case LFO_RATE_PARAMETER_VALUE:
      lfo_rate = paramValue;
      lfoPeriodLength = map(lfo_rate, 0, 1024, LFO_FREQ_LOW, LFO_FREQ_HIGH); 
      break;
    case LFO_SHAPE_PARAMETER_VALUE:
      lfo_shape = paramValue;
      break;
    case LFO_OFFSET_PARAMETER_VALUE:
      lfo_offset = paramValue;
      break;
    case LFO_WIDTH_PARAMETER_VALUE:
      lfo_width = paramValue;
      break;
    case LFO_SYNC_PARAMETER_VALUE:
      lfo_sync = mapBoolean(paramValue);
      break;
    case ADSR_ATTACK_PARAMETER_VALUE:
      adsr_attack = paramValue;
      break;
    case ADSR_DECAY_PARAMETER_VALUE:
      adsr_decay = paramValue;
      break;
    case ADSR_SUSTAIN_PARAMETER_VALUE:
      adsr_sustain = paramValue;
      break;
    case ADSR_RELEASE_PARAMETER_VALUE:
      adsr_release = paramValue;
      break;
    case ADSR_U1_PARAMETER_VALUE:
    case ADSR_U2_PARAMETER_VALUE:
      // Unused for now
      break;
    case ADSR_REPEAT_PARAMETER_VALUE:
      adsr_repeat = mapBoolean(paramValue);
      break;
    case ADSR_SYNC_PARAMETER_VALUE:
      adsr_sync = mapBoolean(paramValue);
      break;
    default:
      Serial.print("Mapping parameter failed!: ");
      Serial.println(quantChoice);
  }

  Serial.print("LFO: ");
  Serial.print(lfo_rate); Serial.print(" ");
  Serial.print(lfo_shape); Serial.print(" ");
  Serial.print(lfo_offset); Serial.print(" ");
  Serial.print(lfo_width); Serial.print(" ");
  Serial.println(lfo_sync);
  
  Serial.print("ADSR: ");
  Serial.print(adsr_attack); Serial.print(" ");
  Serial.print(adsr_decay); Serial.print(" ");
  Serial.print(adsr_sustain); Serial.print(" ");
  Serial.print(adsr_release); Serial.print(" ");
  Serial.print(adsr_repeat); Serial.print(" ");
  Serial.println(adsr_sync);
}

boolean mapBoolean(int paramValue) {
  return map(paramValue, 0, 1024, 0, 2) == 1;
}

