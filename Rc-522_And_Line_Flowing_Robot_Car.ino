//Prateek
//www.prateeks.in

#include <SPI.h>
#include <MFRC522.h>

//Ir Sensor Interfacing
#define LINESENSOR1 6
#define LINESENSOR2 7
#define LINESENSOR3 8

//Buzzer Interfacing
#define BUZZER A0

//Motor Driver Interfacing
#define MOTOR1A 2
#define MOTOR1B 3
#define MOTOR2A 4
#define MOTOR2B 5

//Rc-522 Rfid Interfacing
#define RFID_SDA 10
#define RFID_SCK 13
#define RFID_MOSI 11
#define RFID_MISO 12
#define RFID_RST 9

//constant values definition
#define STOP_TIME 3000
#define BEEP_LENGTH 200
#define MOTOR_SPEED 110
#define MINIMUM_TIME_BETWEEN_CARDS 2000

MFRC522 mfrc522(10, 9);
long int last_card_read;
#define COMMANDS_LENGTH 4
char* Type[4] = {"Stop", "Beep&stop", "Beep&ignore", "Stop"};
int Value[4] = {3000, 1000, 0, 5000};

//Here You put Your Rfid Card Number
char* CardID[4] = {"xx xx xx xx", "xx xx xx xx", "xx xx xx xx", "xx xx xx xx"};

void setup() {

  SPI.begin();
  mfrc522.PCD_Init();
  pinMode(LINESENSOR1, INPUT);
  pinMode(LINESENSOR2, INPUT);
  pinMode(LINESENSOR3, INPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(MOTOR1A, OUTPUT);
  pinMode(MOTOR1B, OUTPUT);
  pinMode(MOTOR2A, OUTPUT);
  pinMode(MOTOR2B, OUTPUT);
  pinMode(DISTANCESENSOR, INPUT);

}

void loop() {
  if (digitalRead(LINESENSOR1) == LOW && digitalRead(LINESENSOR2) == HIGH && digitalRead(LINESENSOR3) == LOW) {
    Forward();
  }
  else if (digitalRead(LINESENSOR1) == HIGH && digitalRead(LINESENSOR2) == LOW && digitalRead(LINESENSOR3) == LOW) {
    Left();
    delay(20);
  }
  else if (digitalRead(LINESENSOR1) == LOW && digitalRead(LINESENSOR2) == LOW && digitalRead(LINESENSOR3) == HIGH) {
    Right();
    delay(20);
  }

  if (millis() - last_card_read >= MINIMUM_TIME_BETWEEN_CARDS) {
    if ( ! mfrc522.PICC_IsNewCardPresent()) {
      return;
    }
    if ( ! mfrc522.PICC_ReadCardSerial()) {
      return;
    }
    String content = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
      Serial.print(mfrc522.uid.uidByte[i], HEX);
      content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
      content.concat(String(mfrc522.uid.uidByte[i], HEX));
    }
    content.toUpperCase();
    content = content.substring(1);
    for (int a = 0; a < COMMANDS_LENGTH; a++) {
      last_card_read = millis();
      if (content == CardID[a]) {
        if (Type[a] == "Stop") {
          Stop();
          delay(Value[a]);
        } else if (Type[a] == "Beep&stop") {
          digitalWrite(BUZZER, HIGH);
          Stop();
          delay(Value[a]);
          digitalWrite(BUZZER, LOW);
        } else if (Type[a] == "Beep&ignore") {
          Stop();
          digitalWrite(BUZZER, HIGH);
          delay(500);
          digitalWrite(BUZZER, LOW);
        } else if (Type[a] == "Ignore") {

        }
      }
    }
  }
}

//turn on the motor to go forward
void Forward() {
  digitalWrite(MOTOR1A, HIGH);
  analogWrite(MOTOR1B, 255 - MOTOR_SPEED);

  digitalWrite(MOTOR2A, HIGH);
  analogWrite(MOTOR2B, 255 - MOTOR_SPEED);
}

//turn on the motor to go backwards
void Backward() {
  digitalWrite(MOTOR1A, LOW);
  analogWrite(MOTOR1B, MOTOR_SPEED);

  digitalWrite(MOTOR2A, LOW);
  analogWrite(MOTOR2B, MOTOR_SPEED);
}

//turn on the motor to turn left
void Left() {
  digitalWrite(MOTOR1A, LOW);
  analogWrite(MOTOR1B, MOTOR_SPEED);

  digitalWrite(MOTOR2A, HIGH);
  analogWrite(MOTOR2B, 255 - MOTOR_SPEED);
}

//turn on the motor to turn right
void Right() {
  digitalWrite(MOTOR1A, HIGH);
  analogWrite(MOTOR1B, 255 - MOTOR_SPEED);

  digitalWrite(MOTOR2A, LOW);
  analogWrite(MOTOR2B, MOTOR_SPEED);
}

//tstop motors
void Stop() {
  digitalWrite(MOTOR1A, LOW);
  digitalWrite(MOTOR1B, LOW);

  digitalWrite(MOTOR2A, LOW);
  digitalWrite(MOTOR2B, LOW);
}
