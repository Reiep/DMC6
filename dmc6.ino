// MIT Licence, feel free to use it!
#include <Bounce2.h>

#define BTN1 2
#define BTN2 3
#define BTN3 4
#define BTN4 5
#define BTN5 6
#define BTN6 7
#define INTERVAL 25

#define LED1 9
#define LED2 14
#define LED3 15
#define LED4 16
#define LED5 17
#define LED6G 11
#define LED6R 12
#define LED6B 13

#define BANK1 0
#define BANK2 1
#define BANK3 2
#define BANK1ALT 3
#define BANK2ALT 4
#define BANK3ALT 5

#define ON 127
#define OFF 0

#define ALTPROGRAMSHIFT 18

Bounce2::Button bounce1 = Bounce2::Button();
Bounce2::Button bounce2 = Bounce2::Button();
Bounce2::Button bounce3 = Bounce2::Button();
Bounce2::Button bounce4 = Bounce2::Button();
Bounce2::Button bounce5 = Bounce2::Button();
Bounce2::Button bounce6 = Bounce2::Button();

int progCounter = 0;
int currentProg = 0;
int currentBank = 0;
bool isAltMode = false;
bool isCCMode = false;
bool isMuteMode = false;

int ledArray[] = {LED1, LED2, LED3, LED4, LED5, LED6R, LED6G, LED6B};
int buttonArray[] = {BTN1, BTN2, BTN3, BTN4, BTN5, BTN6};
Bounce2::Button bounceArray[] = {bounce1, bounce2, bounce3, bounce4, bounce5, bounce6};
byte currentCCValue[] = {OFF, OFF, OFF, OFF, OFF};
byte ccParam[] = {60, 61, 0, 62, 63};


void midiCC(byte cont, byte value) {
  // Serial.print("Sending CC: ");
  // Serial.print(cont);
  // Serial.print(" / ");
  // Serial.println(value);
  Serial.write(0xB0);
  Serial.write(cont);
  Serial.write(value);
}

void midiPC(int prog) {
  if (isAltMode && !isMuteMode) {
    prog += 10;
  }
  // Serial.print("Sending PC: ");
  // Serial.println(prog);
  Serial.write(0xC0);
  Serial.write(prog);
}


void flashLED(byte LED) {
  for (int i=0; i<10; i++) {
    digitalWrite(ledArray[LED-1], !digitalRead(ledArray[LED-1]) );
    delay(100);
  }
}

void clearLED() {
  for (int i = 0; i <5; i++) {
       digitalWrite(ledArray[i], LOW);
  }
}

void sendTap() {
  midiCC(64, 127);
}

void bootSequenceLights(bool withDelay) {
  unsigned long ledDelay = 100;

  digitalWrite(LED1, HIGH);
  delay(ledDelay);
  digitalWrite(LED2, HIGH);
  delay(ledDelay);
  digitalWrite(LED3, HIGH);
  delay(ledDelay);
  digitalWrite(LED4, HIGH);
  delay(ledDelay);
  digitalWrite(LED5, HIGH);
  delay(ledDelay);
  digitalWrite(LED6B, HIGH);
  digitalWrite(LED6R, HIGH);
  digitalWrite(LED6G, LOW);
  if (withDelay) {
    delay(10*ledDelay);
  }
  digitalWrite(LED1, LOW);
  delay(0.25*ledDelay);
  digitalWrite(LED2, LOW);
  delay(0.25*ledDelay);
  digitalWrite(LED3, LOW);
  delay(0.25*ledDelay);
  digitalWrite(LED4, LOW);
  delay(0.25*ledDelay);
  digitalWrite(LED5, LOW);
}




void setup() {
 
  Serial.begin(31250);  // init MIDI output
  // Serial.begin(9600);
  
  for (int i = 0; i < 6; i++) {
    bounceArray[i].attach(buttonArray[i], INPUT);
    bounceArray[i].interval(INTERVAL);
    bounceArray[i].setPressedState(LOW);
    digitalWrite(buttonArray[i], HIGH);
  }
  for (int i = 0; i < 8; i++) {
    pinMode(ledArray[i], OUTPUT);
  }

//flash LEDs to show we're alive
  bootSequenceLights(true);
}

void loop() {

  for (int i = 0; i < 6; i++) {
    bounceArray[i].update();
    if (bounceArray[i].rose()) {
      if (i < 5) {
        if (currentBank == 2) {
          if (i != 2) { // bottom right button
            if (currentCCValue[i]==OFF) {
              currentCCValue[i]=ON;
              digitalWrite(ledArray[i], HIGH);
            } else {
              currentCCValue[i]=OFF;
              digitalWrite(ledArray[i], LOW);
            }
            midiCC(ccParam[i], currentCCValue[i]);
          } else {
            if (!isMuteMode) {
              isMuteMode = true;
              midiPC(99);
              digitalWrite(ledArray[i], HIGH);
            } else {
              isMuteMode = false;
              midiPC(currentProg);
              digitalWrite(ledArray[i], LOW);
            }
          }
                    
        } else {
          // Serial.print("Button pressed: ");
          // Serial.println(i);
          clearLED();
          digitalWrite(ledArray[i], HIGH);
          currentProg = currentBank * 5 + i + 1;
          midiPC(currentProg);
        }
      } else {
        if (bounceArray[i].previousDuration() > 250) {
          // Serial.println("Button hold: 6");
          clearLED();
          bootSequenceLights(false);
          isAltMode = !isAltMode;
          if (currentBank == 1) {
            digitalWrite(LED6G, HIGH);
            isAltMode?digitalWrite(LED6B, LOW):digitalWrite(LED6B, HIGH);
            digitalWrite(LED6R, LOW);
            isCCMode = false;
          } else if (currentBank == 2) {
            digitalWrite(LED6B, HIGH);
            digitalWrite(LED6R, HIGH);
            digitalWrite(LED6G, HIGH);
            isCCMode = true;
          } else if (currentBank == 0) {
            digitalWrite(LED6R, HIGH);
            digitalWrite(LED6G, LOW);
            isAltMode?digitalWrite(LED6B, LOW):digitalWrite(LED6B, HIGH);
            isCCMode = false;
          }
        } else {
          // Serial.println("Button pressed: 6");
          // Serial.print("Current Bank: ");
          // Serial.println(currentBank);
          clearLED();
          if (currentBank == 0) {
            currentBank = 1;
            digitalWrite(LED6G, HIGH);
            isAltMode?digitalWrite(LED6B, LOW):digitalWrite(LED6B, HIGH);
            digitalWrite(LED6R, LOW);
            isCCMode = false;
          } else if (currentBank == 1) {
            currentBank = 2;
            digitalWrite(LED6B, HIGH);
            digitalWrite(LED6R, HIGH);
            digitalWrite(LED6G, HIGH);
            isCCMode = true;
          } else if (currentBank == 2) {
            currentBank = 0;
            digitalWrite(LED6R, HIGH);
            digitalWrite(LED6G, LOW);
            isAltMode?digitalWrite(LED6B, LOW):digitalWrite(LED6B, HIGH);
            isCCMode = false;
          }
          // Serial.print("New Bank: ");
          // Serial.println(currentBank);
        }
      }
    }
  }
}
