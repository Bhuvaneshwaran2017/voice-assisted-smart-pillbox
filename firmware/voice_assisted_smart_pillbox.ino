#include <Wire.h>
#include <RTClib.h>
#include <DFRobotDFPlayerMini.h>

RTC_DS3231 rtc;
HardwareSerial DFSerial(2);
DFRobotDFPlayerMini dfPlayer;

#define DF_RX 16
#define DF_TX 17
#define GREEN_LED 26
#define YELLOW_LED 27
#define RED_LED 32
#define BUZZER 25
#define MORNING_BTN 13
#define AFTERNOON_BTN 14
#define NIGHT_BTN 18

bool dfPlayerReady = false;
bool audioFinished = false;
int activeReminder = 0;
int lastMorningDay = -1;
int lastAfternoonDay = -1;
int lastNightDay = -1;
unsigned long lastBeepTime = 0;
bool buzzerState = false;

void setup() {
  Serial.begin(115200);
  delay(1000);

  Wire.begin(21, 22);
  if (!rtc.begin()) {
    Serial.println("ERROR: RTC NOT FOUND!");
    while (1) delay(100);
  }

  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(MORNING_BTN, INPUT_PULLUP);
  pinMode(AFTERNOON_BTN, INPUT_PULLUP);
  pinMode(NIGHT_BTN, INPUT_PULLUP);
  allOutputsOff();

  DFSerial.begin(9600, SERIAL_8N1, DF_RX, DF_TX);
  delay(1500);

  if (dfPlayer.begin(DFSerial)) {
    dfPlayerReady = true;
    dfPlayer.outputDevice(DFPLAYER_DEVICE_SD);
    delay(500);
    dfPlayer.volume(28);
    Serial.println("DFPLAYER: OK");
  } else {
    Serial.println("DFPLAYER: NOT FOUND!");
  }

  Serial.println("VOICE-ASSISTED SMART PILLBOX READY");
  Serial.println("08:00 Morning | 13:00 Afternoon | 18:30 Night");
  showTime();
}

void loop() {
  DateTime now = rtc.now();
  checkDFPlayer();

  int hour = now.hour();
  int minute = now.minute();
  int day = now.day();

  if (Serial.available()) {
    char command = Serial.read();
    if (command == 'M' || command == 'm') startReminder(1);
    if (command == 'A' || command == 'a') startReminder(2);
    if (command == 'N' || command == 'n') startReminder(3);
    if (command == 'X' || command == 'x') stopReminder();
    if (command == 'S' || command == 's') showTime();
    if (command == '1') playVoice(1);
    if (command == '2') playVoice(2);
    if (command == '3') playVoice(3);
  }

  if (hour == 8 && minute == 0 && activeReminder == 0 && lastMorningDay != day) {
    startReminder(1);
    lastMorningDay = day;
  }

  if (hour == 13 && minute == 0 && activeReminder == 0 && lastAfternoonDay != day) {
    startReminder(2);
    lastAfternoonDay = day;
  }

  if (hour == 18 && minute == 30 && activeReminder == 0 && lastNightDay != day) {
    startReminder(3);
    lastNightDay = day;
  }

  if (activeReminder == 1 && digitalRead(MORNING_BTN) == LOW) {
    stopReminder();
    delay(300);
  }

  if (activeReminder == 2 && digitalRead(AFTERNOON_BTN) == LOW) {
    stopReminder();
    delay(300);
  }

  if (activeReminder == 3 && digitalRead(NIGHT_BTN) == LOW) {
    stopReminder();
    delay(300);
  }

  if (activeReminder != 0 && audioFinished) {
    if (millis() - lastBeepTime >= 500) {
      lastBeepTime = millis();
      buzzerState = !buzzerState;
      digitalWrite(BUZZER, buzzerState);
    }
  } else {
    buzzerState = false;
    digitalWrite(BUZZER, LOW);
  }

  delay(50);
}

void startReminder(int reminder) {
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(RED_LED, LOW);

  activeReminder = reminder;
  audioFinished = false;
  buzzerState = false;
  lastBeepTime = millis();

  if (reminder == 1) {
    digitalWrite(GREEN_LED, HIGH);
    playVoice(1);
  } else if (reminder == 2) {
    digitalWrite(YELLOW_LED, HIGH);
    playVoice(2);
  } else if (reminder == 3) {
    digitalWrite(RED_LED, HIGH);
    playVoice(3);
  }
}

void playVoice(int voiceNumber) {
  if (!dfPlayerReady) return;

  if (voiceNumber == 1) dfPlayer.playMp3Folder(1);
  if (voiceNumber == 2) dfPlayer.playMp3Folder(2);
  if (voiceNumber == 3) dfPlayer.playMp3Folder(3);
}

void checkDFPlayer() {
  if (!dfPlayerReady) return;

  if (dfPlayer.available()) {
    uint8_t type = dfPlayer.readType();
    int value = dfPlayer.read();

    if (type == DFPlayerPlayFinished) {
      audioFinished = true;
    }

    (void)value;
  }
}

void stopReminder() {
  activeReminder = 0;
  audioFinished = false;
  allOutputsOff();

  if (dfPlayerReady) dfPlayer.stop();
}

void allOutputsOff() {
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(RED_LED, LOW);
  digitalWrite(BUZZER, LOW);
  buzzerState = false;
}

void showTime() {
  DateTime now = rtc.now();

  Serial.print("RTC: ");
  if (now.day() < 10) Serial.print("0");
  Serial.print(now.day());
  Serial.print("/");

  if (now.month() < 10) Serial.print("0");
  Serial.print(now.month());
  Serial.print("/");
  Serial.print(now.year());
  Serial.print(" ");

  if (now.hour() < 10) Serial.print("0");
  Serial.print(now.hour());
  Serial.print(":");

  if (now.minute() < 10) Serial.print("0");
  Serial.print(now.minute());
  Serial.print(":");

  if (now.second() < 10) Serial.print("0");
  Serial.println(now.second());
}
