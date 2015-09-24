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


void setup() {
  pinMode(LFO_OUTPUT_PIN, OUTPUT);
  pinMode(ADSR_OUTPUT_PIN, OUTPUT);
  pinMode(VALUE_INPUT_PIN, INPUT);
  pinMode(PARAMETER_INPUT_PIN, INPUT);
}

void loop() {
  int parameterChoice = analogRead(PARAMETER_INPUT_PIN);
  if (parameterChoice != 0) {
    handleParameter(parameterChoice);
  }
  delay(100);
}

void handleParameter(int paramChoice) {
  Serial.print("Parameter input detected: ");
  Serial.print(paramChoice);

  // Quantize paramChoice
  int quantChoice = map(paramChoice, 0, 1024, 0, 64);
  Serial.print(" = ");
  Serial.println(quantChoice);

  switch (quantChoice) {
    case LFO_RATE_PARAMETER_VALUE:
      Serial.println("LFO_RATE_PARAMETER_VALUE");
      break;
    case LFO_SHAPE_PARAMETER_VALUE:
      Serial.println("LFO_SHAPE_PARAMETER_VALUE");
      break;
    case LFO_OFFSET_PARAMETER_VALUE:
      Serial.println("LFO_OFFSET_PARAMETER_VALUE");
      break;
    case LFO_WIDTH_PARAMETER_VALUE:
      Serial.println("LFO_WIDTH_PARAMETER_VALUE");
      break;
    case LFO_SYNC_PARAMETER_VALUE:
      Serial.println("LFO_SYNC_PARAMETER_VALUE");
      break;
    case ADSR_ATTACK_PARAMETER_VALUE:
      Serial.println("ADSR_ATTACK_PARAMETER_VALUE");
      break;
    case ADSR_DECAY_PARAMETER_VALUE:
      Serial.println("ADSR_DECAY_PARAMETER_VALUE");
      break;
    case ADSR_SUSTAIN_PARAMETER_VALUE:
      Serial.println("ADSR_SUSTAIN_PARAMETER_VALUE");
      break;
    case ADSR_RELEASE_PARAMETER_VALUE:
      Serial.println("ADSR_RELEASE_PARAMETER_VALUE");
      break;
    case ADSR_U1_PARAMETER_VALUE:
      Serial.println("ADSR_U1_PARAMETER_VALUE");
      break;
    case ADSR_U2_PARAMETER_VALUE:
      Serial.println("ADSR_U2_PARAMETER_VALUE");
      break;
    case ADSR_REPEAT_PARAMETER_VALUE:
      Serial.println("ADSR_REPEAT_PARAMETER_VALUE");
      break;
    case ADSR_SYNC_PARAMETER_VALUE:
      Serial.println("ADSR_SYNC_PARAMETER_VALUE");
      break;
    default:
      Serial.print("Mapping parameter failed!: ");
      Serial.println(quantChoice);
  }
}

