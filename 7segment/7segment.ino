// 7세그먼트 4자리 표시기 (소수점 없는 버전)
// by Perplexity

// 핀 설정
const int DigitPins[4]   = {2, 3, 4, 5};        // Digit 제어 핀 (D1~D4)
const int SegmentPins[8] = {6, 7, 8, 9, 10, 11, 12, 13}; // a, b, c, d, e, f, g, dp

// 논리값 정의 (공통 애노드 기준)
const bool DigitOn  = LOW;
const bool DigitOff = HIGH;
const bool SegOn    = HIGH;
const bool SegOff   = LOW;

// 숫자 배열 (dp 제외)
const int BLANK[8] = {LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW};
const int N0[8]    = {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, LOW, LOW};
const int N1[8]    = {LOW, HIGH, HIGH, LOW, LOW, LOW, LOW, LOW};
const int N2[8]    = {HIGH, HIGH, LOW, HIGH, HIGH, LOW, HIGH, LOW};
const int N3[8]    = {HIGH, HIGH, HIGH, HIGH, LOW, LOW, HIGH, LOW};
const int N4[8]    = {LOW, HIGH, HIGH, LOW, LOW, HIGH, HIGH, LOW};
const int N5[8]    = {HIGH, LOW, HIGH, HIGH, LOW, HIGH, HIGH, LOW};
const int N6[8]    = {HIGH, LOW, HIGH, HIGH, HIGH, HIGH, HIGH, LOW};
const int N7[8]    = {HIGH, HIGH, HIGH, LOW, LOW, LOW, LOW, LOW};
const int N8[8]    = {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, LOW};
const int N9[8]    = {HIGH, HIGH, HIGH, HIGH, LOW, HIGH, HIGH, LOW};
const int MIN[8]   = {LOW, LOW, LOW, LOW, LOW, LOW, HIGH, LOW}; // 마이너스

// 표시할 값 저장용 배열
const int* lights[4];

// 시리얼 입력 버퍼
char incoming[9] = {};

void setup() {
  Serial.begin(9600);

  // 핀모드 설정
  for (byte i = 0; i < 4; i++) {
    pinMode(DigitPins[i], OUTPUT);
  }
  for (byte i = 0; i < 8; i++) {
    pinMode(SegmentPins[i], OUTPUT);
  }

  // 초기 표시값 (예: 1234)
  lights[0] = N1;
  lights[1] = N2;
  lights[2] = N3;
  lights[3] = N4;
}

void loop() {
  if (Serial.available() > 0) {
    readSerialInput();
    parseInput();
    printDebug();
  }
  displaySegments();
}

// 시리얼 데이터 읽기 (소수점 필터링)
void readSerialInput() {
  memset(incoming, 0, sizeof(incoming));
  int i = 0;
  while (Serial.available() > 0 && i < sizeof(incoming) - 1) {
    char c = Serial.read();
    if (c != '.' && c != ',') { // 소수점 문자 무시
      incoming[i++] = c;
    }
    delay(3);
  }
}

// 입력값 파싱 (소수점 로직 제거)
void parseInput() {
  int counter = -1;
  for (int i = 0; i < 9 && counter < 3; i++) {
    counter++;
    switch (incoming[i]) {
      case '0': lights[counter] = N0; break;
      case '1': lights[counter] = N1; break;
      case '2': lights[counter] = N2; break;
      case '3': lights[counter] = N3; break;
      case '4': lights[counter] = N4; break;
      case '5': lights[counter] = N5; break;
      case '6': lights[counter] = N6; break;
      case '7': lights[counter] = N7; break;
      case '8': lights[counter] = N8; break;
      case '9': lights[counter] = N9; break;
      case '-': lights[counter] = MIN; break;
      default:  lights[counter] = BLANK; break;
    }
  }
}

// 7세그먼트 표시 (dp 세그먼트 항상 OFF)
void displaySegments() {
  for (byte seg = 0; seg < 8; seg++) {
    digitalWrite(SegmentPins[seg], SegOn);
    for (byte digit = 0; digit < 4; digit++) {
      digitalWrite(DigitPins[digit], (lights[digit][seg] == 1) ? DigitOn : DigitOff);
    }
    delayMicroseconds(3000); // 밝기 조절
    digitalWrite(SegmentPins[seg], SegOff);
    for (byte digit = 0; digit < 4; digit++) {
      digitalWrite(DigitPins[digit], DigitOff);
    }
  }
}

// 디버그 출력 (옵션)
void printDebug() {
  for (int y = 0; y < 4; y++) {
    Serial.print("Digit ");
    Serial.print(y);
    Serial.print(": ");
    for (int z = 0; z < 8; z++) {
      Serial.print(lights[y][z]);
    }
    Serial.println();
  }
}
