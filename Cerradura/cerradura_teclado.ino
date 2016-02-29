#include <Stepper.h>
#include <LiquidCrystal.h>
#include <Keypad.h>

#define STEPS 200
#define ENABLE  1
#define DISABLE 0

const byte rows = 4;
const byte columns = 3;
const int defaulPass = 1;
const byte pinGreen = 3;
const byte bridgeH = 2;
byte pinsRows[rows] = {15, 14, 13, 12};
byte pinsColumns[columns] = {18, 17, 16};
char keys[rows][columns] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

char data[16];
byte data_count = 0;
byte active;
int pushTime = 0;

Keypad numpad = Keypad(makeKeymap(keys), pinsRows, pinsColumns, rows, columns);
LiquidCrystal lcd(11, 10, 9, 8, 7, 6);

Stepper stepper(STEPS, 5, 4);

void openDoor() {
  digitalWrite(bridgeH, HIGH);
  digitalWrite(pinGreen, LOW);
  stepper.step(250);
  Serial.println("Abriendo Puerta");
  digitalWrite(bridgeH, LOW);
}

void closeDoor() {
  digitalWrite(bridgeH, HIGH);
  digitalWrite(pinGreen, HIGH);
  stepper.step(-250);  
  Serial.println("Cerrando Puerta");
  digitalWrite(bridgeH, LOW);
}

boolean isCorrect() {
  int numPass = 0;
    numPass += data[0] - 48;
  Serial.println(data);
  Serial.println(numPass);
  return (numPass == defaulPass);
}

void setup() {
  lcd.begin(16,2);
  active = ENABLE;
  stepper.setSpeed(100);
  pinMode(pinGreen, OUTPUT);
  pinMode(bridgeH, OUTPUT);
  digitalWrite(pinGreen, HIGH);
  digitalWrite(bridgeH, LOW);
  Serial.begin(9600);
}

void loop() {
  char key = numpad.getKey();
  if (key) {
    if (active == ENABLE) {
      lcd.setCursor(0,0);
      lcd.print("Contrasena");
      switch(key) {
        case '*':
          data_count--;
          lcd.setCursor(data_count,1);
          lcd.print(" ");
          data[data_count] = '\0';
          break;
        case '#':
          if (isCorrect()) {
            active = DISABLE;
            lcd.setCursor(0,0);
            lcd.print("Acceso          ");
            lcd.setCursor(0,1);
            lcd.print("Concedido       ");
            openDoor();
          }
          else {
            lcd.setCursor(0,0);
            lcd.print("Acceso          ");
            lcd.setCursor(0,1);
            lcd.print("Denegado        ");
          }
          memset(data,0,sizeof(data));
          delay(3000);
          break;
        default:
          data[data_count] = key;
          lcd.setCursor(data_count,1);
          lcd.print("*");
          data_count++;
          break;
      }
    }
    else {
      if (key == '#') {
        lcd.setCursor(0,0);
        lcd.print("Cerrando        ");
        lcd.setCursor(0,1);
        lcd.print("Puerta          ");
        closeDoor();
        active = ENABLE;
        delay(3000);
      }
    }
  }
}
