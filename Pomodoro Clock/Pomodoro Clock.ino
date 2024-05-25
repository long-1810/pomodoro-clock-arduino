#include <pitches.h>
#include <TimeLib.h>
#include <LiquidCrystal.h>

const int RS = 12;
const int ENABLE = 11;
const int D4 = 5;
const int D5 = 4;
const int D6 = 3;
const int D7 = 2;
const int BUZZER_PIN = 8;

// LCD
LiquidCrystal lcd(RS, ENABLE, D4, D5, D6, D7);

// POMODORO
const int STUDY_DURATION = 15; // 1500s = 25 minutes
const int SHORT_BREAK_DURATION = 3; // 300s = 5 mins
const int LONG_BREAK_DURATION = 9; // 900s = 15 mins
const int LONG_BREAK_CYCLE = 2; // After 3 short breaks is 1 long break

int studySessionsPassed = 1;
int initSessionTime; // in time_t
int sessionTime; // counting from initSessionTime
enum SESSION_NAMES {
  study, short_break, long_break
};
SESSION_NAMES currentSession = study;

// ALARM
const int PITCH = NOTE_A4;
const int DURATION = 2000;


void setup() {
  lcd.begin(16, 2);
  initSessionTime = now();
  currentSession = study;

  pinMode(BUZZER_PIN, OUTPUT); 
}

void loop() {
  pomodoro();
  displaySessionTime();
  delay(1000);
}

void displaySessionTime() {
  int sessionHours = sessionTime / 3600;
  int sessionMinutes = (sessionTime % 3600) / 60;
  int sessionSeconds = (sessionTime % 3600) % 60;

  char formattedDate[1000];
  sprintf(formattedDate, "%02d:%02d:%02d", sessionHours, sessionMinutes, sessionSeconds);

  lcd.clear();
  switch (currentSession) {
    case study:
      lcd.print("Study (25m)");
      break;
    case short_break:
      lcd.print("Break (5m)");
      break;
    case long_break:
      lcd.print("Break (15m)");
      break;
  }
  lcd.setCursor(0,1);
  lcd.print(formattedDate);
}

void pomodoro() {
  sessionTime = now() - initSessionTime;
  if (currentSession == study && sessionTime >= STUDY_DURATION) {
    if (studySessionsPassed % (LONG_BREAK_CYCLE + 1) == 0) {
      studySessionsPassed++;
      currentSession = long_break;
      initSessionTime = now();
      playAlarmSound();
    }
    else {
      studySessionsPassed++;
      currentSession = short_break;
      initSessionTime = now();
      playAlarmSound();
    }
  } else if (currentSession == long_break && sessionTime >= LONG_BREAK_DURATION) {
      currentSession = study;
      initSessionTime = now();
      playAlarmSound();
  } else if (currentSession == short_break && sessionTime >= SHORT_BREAK_DURATION) {
      currentSession = study;
      initSessionTime = now();
      playAlarmSound();
  }
}

void playAlarmSound() {
  tone(BUZZER_PIN, PITCH, DURATION);
}
