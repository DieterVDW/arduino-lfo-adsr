#define TRIGGER_INPUT_PIN 0
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
#define ADSR_OFFSET_PARAMETER_VALUE 11 // 181 // Unused for now
#define ADSR_WIDTH_PARAMETER_VALUE 13 // 217 // Unused for now
#define ADSR_INVERT_PARAMETER_VALUE 15 // 249
#define ADSR_REPEAT_PARAMETER_VALUE 16 // 265

#define UPDATE_INTERVAL 100 // microseconds

#define PARAM_READ_INTERVAL 10 // ms
unsigned long lastParamTime;

// LFO parameters
#define LFO_PERIOD_MIN 1000 // micros
#define LFO_PERIOD_MAX 1000000 // micros
#define LFO_PERIOD_SLOPE 1.0
int lfo_shape = 512;
int lfo_offset = 0;
int lfo_width = 255;
boolean lfo_sync;
unsigned long lfoStart = 0;
unsigned long lfoPeriodLength = 500000;

// ADSR parameters
#define ADSR_TIME_MIN 1000 // micros
#define ADSR_TIME_MAX 10000000 // micros
#define ADSR_SLOPE 1.0
#define ADSR_TRIGGER_UPDATE_INTERVAL 1000 // micros
int adsr_offset = 0;
int adsr_width = 255;
boolean adsr_invert = false;
boolean adsr_repeat = false;
unsigned long adsrAttackTime = 500000;
unsigned long adsrDecayTime = 500000;
unsigned long adsrSustainLevel = 255;
unsigned long adsrReleaseTime = 500000;
int adsrStartValue;
int adsrValue;

// Trigger
unsigned long lastTriggerUpdateTime = 0;
unsigned long lastTriggerTime = 0;
boolean trigger;

void setup() {
  pinMode(TRIGGER_INPUT_PIN, INPUT);
  analogWrite(ADSR_OUTPUT_PIN, 0);
}

void loop() {
  unsigned long curTime = micros();

  // Read parameter values every PARAM_READ_INTERVAL ms
  if (curTime / 1000 > lastParamTime + PARAM_READ_INTERVAL) {
    readParameter();
    lastParamTime = curTime / 1000;
  }

  // Check trigger
  if (curTime > lastTriggerUpdateTime + ADSR_TRIGGER_UPDATE_INTERVAL) {
    boolean triggerVal = digitalRead(TRIGGER_INPUT_PIN);
    if (triggerVal != trigger) {
      // Trigger changed!
      trigger = triggerVal;
      lastTriggerTime = curTime;
    }

    lastTriggerUpdateTime = curTime;
  }

  // Set ADSR voltage
  adjustADSR(curTime);

  // Set LFO voltage
  adjustLFO(curTime);

  delayMicroseconds(UPDATE_INTERVAL - (curTime % UPDATE_INTERVAL));
}

void adjustLFO(long curTime) {
  long position = (lfo_sync ? (curTime - lastTriggerTime) : curTime) % lfoPeriodLength;

  int value;
  if (lfo_shape == 0) {
    // Sine wave
    value = (sin(2*PI*position/lfoPeriodLength)+1)*255.0/2.0;
  } else {
    // Triangular wave, shape modifiable with shape parameter
    long peakPosition = map(lfo_shape, 0, 1024, 0, lfoPeriodLength);
    if (position < peakPosition) {
       // Ramping up
      value = map(position, 0, peakPosition, 0, 256);
    } else {
      // Ramping down
      value = map(position, peakPosition, lfoPeriodLength, 256, 0);
    }
  }

  value = map(value, 0, 255, 0, lfo_width); // Scale for width
  value = map(value, 0, 255, lfo_offset, 256); // Adjust for offset

  analogWrite(LFO_OUTPUT_PIN, value);
}

void adjustADSR(long curTime) {
  long position = curTime - lastTriggerTime;

  if (position == 0) {
    adsrStartValue = adsrValue;
  }
  if (adsr_repeat && trigger) {
    position %= (adsrAttackTime + adsrDecayTime + adsrReleaseTime);
  }

  if (trigger) {
    // Trigger on
    if (position < adsrAttackTime) {
      adsrValue = map(position, 0, adsrAttackTime, adsrStartValue, 256);
    } else if (position < (adsrAttackTime + adsrDecayTime)) {
      adsrValue = map(position - adsrAttackTime, 0, adsrDecayTime, 255, adsrSustainLevel);
    } else if (adsr_repeat && position < (adsrAttackTime + adsrDecayTime + adsrReleaseTime))  {
      adsrValue = map(position - adsrAttackTime - adsrDecayTime, 0, adsrReleaseTime, adsrSustainLevel, 0); // Release
    } else {
      adsrValue = adsrSustainLevel;
    }
  } else {
    // Trigger off
    if (position < adsrReleaseTime) {
      adsrValue = map(position, 0, adsrReleaseTime, adsrStartValue, 0);
    } else {
      adsrValue = 0;
    }
  }

  int value = map(adsrValue, 0, 255, 0, adsr_width); // Scale for width
  value = map(value, 0, 255, adsr_offset, 255); // Adjust for offset
  if (adsr_invert) {
    value = 255 - value;
  }
  analogWrite(ADSR_OUTPUT_PIN, value);
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
      lfoPeriodLength = slopeMap(paramValue, LFO_PERIOD_MIN, LFO_PERIOD_MAX, LFO_PERIOD_SLOPE);
      break;
    case LFO_SHAPE_PARAMETER_VALUE:
      lfo_shape = paramValue;
      break;
    case LFO_OFFSET_PARAMETER_VALUE:
      lfo_offset = map(paramValue, 0, 1024, 0, 256);
      break;
    case LFO_WIDTH_PARAMETER_VALUE:
      lfo_width = map(paramValue, 0, 1024, 0, 256);
      break;
    case LFO_SYNC_PARAMETER_VALUE:
      lfo_sync = mapBoolean(paramValue);
      break;
    case ADSR_ATTACK_PARAMETER_VALUE:
      adsrAttackTime = slopeMap(paramValue, ADSR_TIME_MIN, ADSR_TIME_MAX, ADSR_SLOPE);
      break;
    case ADSR_DECAY_PARAMETER_VALUE:
      adsrDecayTime = slopeMap(paramValue, ADSR_TIME_MIN, ADSR_TIME_MAX, ADSR_SLOPE);
      break;
    case ADSR_SUSTAIN_PARAMETER_VALUE:
      adsrSustainLevel = map(paramValue, 0, 1024, 0, 256);
      break;
    case ADSR_RELEASE_PARAMETER_VALUE:
      adsrReleaseTime = slopeMap(paramValue, ADSR_TIME_MIN, ADSR_TIME_MAX, ADSR_SLOPE);
      break;
    case ADSR_OFFSET_PARAMETER_VALUE:
      adsr_offset = map(paramValue, 0, 1024, 0, 256);
      break;
    case ADSR_WIDTH_PARAMETER_VALUE:
      adsr_width = map(paramValue, 0, 1024, 0, 256);
      break;
    case ADSR_INVERT_PARAMETER_VALUE:
      adsr_invert = mapBoolean(paramValue);
      break;
    case ADSR_REPEAT_PARAMETER_VALUE:
      adsr_repeat = mapBoolean(paramValue);
      break;
    default:
      Serial.print("Mapping parameter failed!: ");
      Serial.println(quantChoice);
  }

  Serial.print("LFO: P:");
  Serial.print(lfoPeriodLength / 1000); Serial.print("ms S:");
  Serial.print(lfo_shape); Serial.print(" O:");
  Serial.print(lfo_offset); Serial.print(" W:");
  Serial.print(lfo_width); Serial.print(" S:");
  Serial.println(lfo_sync);

  Serial.print("ADSR: A:");
  Serial.print(adsrAttackTime / 1000); Serial.print("ms D:");
  Serial.print(adsrDecayTime / 1000); Serial.print("ms S:");
  Serial.print(adsrSustainLevel); Serial.print(" R:");
  Serial.print(adsrReleaseTime / 1000); Serial.print("ms O:");
  Serial.print(adsr_offset); Serial.print(" W:");
  Serial.print(adsr_width); Serial.print(" I:");
  Serial.print(adsr_invert); Serial.print(" R:");
  Serial.println(adsr_repeat);
}

boolean mapBoolean(int paramValue) {
  return map(paramValue, 0, 1024, 0, 2) == 1;
}

/*
 * Own map() implementation that supports big long's and slope.
 */
long slopeMap(long paramValue, long min, long max, float slope) {
  float index = (float)paramValue/1024;
  paramValue -= (1 - pow(index, 2)) * 1024 * slope; // Compensate for slope
  return (max / 1024) * paramValue;
}

