#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_ADS1X15.h>
#include <Adafruit_MCP4725.h>
Adafruit_MCP4725 dac;
Adafruit_ADS1115 ads;
LiquidCrystal_I2C lcd(0x27, 16, 2);
#define clk 2
#define dt 3
#define sw 4
char screen = 0;
char arrowpos = 0;
float power = 0;
float current = 0;
float curcurrent = 0;
float curpower = 0;
float curvoltage = 0;
int counter = 0;
uint32_t dac_value;
volatile boolean currentmode = false;
volatile boolean powermode = false;
volatile boolean TurnDetected = false;
volatile boolean up = false;
volatile boolean button = false;

byte customChar1[8] = {
  0b10000,
  0b11000,
  0b11100,
  0b11110,
  0b11110,
  0b11100,
  0b11000,
  0b10000
};

byte customChar2[8] = {
  0b00100,
  0b01110,
  0b11111,
  0b00000,
  0b00000,
  0b11111,
  0b01110,
  0b00100,
};

ISR(PCINT2_vect) {
  if (digitalRead(sw) == LOW) {
    button = true;
  }
}

void isr0 ()  {
  TurnDetected = true;
  up = (digitalRead(clk) == digitalRead(dt));
}

void setup() {
  lcd.init();
  lcd.backlight();
  ads.begin();
  dac.begin(0x60);
  dac_value = 0;
  dac.setVoltage(dac_value, false);
  pinMode(sw, INPUT_PULLUP);
  pinMode(clk, INPUT);
  pinMode(dt, INPUT);
  PCICR |= 0b00000100;
  PCMSK2 |= 0b00010000;   // turn o PCINT20(D4)
  attachInterrupt(0, isr0, RISING);
  ICR1 = 2047;
  lcd.createChar(0, customChar1);
  lcd.createChar(1, customChar2);
  lcd.clear();
  lcd.print("digitaleclab.com");
  delay(3000);
  screen0();
  lcd.setCursor(0, 0);
  lcd.write((uint8_t)0);
}

void loop() {

  if (currentmode) {
    curcurrent = ads.readADC_Differential_0_1() * 0.1875 / 1000.00 / 0.0975;
    if (counter == 100) {
      lcd.setCursor(4, 1);
      lcd.print(curcurrent);
      lcd.print("A ");
      counter = 0;
    }
    if (curcurrent < current) {
      if (dac_value < 4095) {
        dac_value++;
        dac.setVoltage(dac_value, false);
      }
      else {
        dac.setVoltage(dac_value, false);
      }
    }
    else {
      if (dac_value > 0) {
        dac_value = dac_value - 1;
        dac.setVoltage(dac_value, false);
      }
      else {
        dac.setVoltage(dac_value, false);
      }
    }
    counter++;
    delayMicroseconds(100);
  }

  if (powermode) {
    curcurrent = ads.readADC_Differential_0_1() * 0.1875 / 1000.00 / 0.0975;
    curvoltage = ads.readADC_SingleEnded(2) * 0.1875 * 11.13 / 1000.00;
    curpower = curvoltage * curcurrent;
    //Serial.println(curpower);
    if (counter == 100) {
      lcd.setCursor(4, 1);
      lcd.print(curpower);
      lcd.print("W ");
      counter = 0;
    }
    if (curpower < power) {
      if (dac_value < 4095) {
        dac_value++;
        dac.setVoltage(dac_value, false);
      }
      else {
        dac.setVoltage(dac_value, false);
      }
    }
    else {
      if (dac_value > 0) {
        dac_value = dac_value - 1;
        dac.setVoltage(dac_value, false);
      }
      else {
        dac.setVoltage(dac_value, false);
      }
    }
    counter++;
    delayMicroseconds(100);
  }

  if (TurnDetected) {
    delay(200);
    switch (screen) {
      case 0:
        switch (arrowpos) {
          case 0:
            if (!up) {
              screen0();
              lcd.setCursor(0, 1);
              lcd.write((uint8_t)0);
              arrowpos = 1;
            }
            break;
          case 1:
            if (up) {
              screen0();
              lcd.setCursor(0, 0);
              lcd.write((uint8_t)0);
              arrowpos = 0;
            }
            break;
        }
        break;
      case 1:
        switch (arrowpos) {
          case 0:
            if (!up) {
              screen1();
              lcd.setCursor(0, 1);
              lcd.write((uint8_t)0);
              arrowpos = 1;
            }
            break;
          case 1:
            if (up) {
              screen1();
              lcd.setCursor(0, 0);
              lcd.write((uint8_t)0);
              arrowpos = 0;
            }
            else {
              screen1();
              lcd.setCursor(7, 1);
              lcd.write((uint8_t)0);
              arrowpos = 2;
            }
            break;
          case 2:
            if (up) {
              screen1();
              lcd.setCursor(0, 1);
              lcd.write((uint8_t)0);
              arrowpos = 1;
            }
            break;
        }
        break;
      case 2:
        if (up) {
          power = power + 0.1;
          lcd.setCursor(7, 0);
          lcd.print(power);
          lcd.print("W");
          lcd.write((uint8_t)1);
          lcd.print("  ");
        }
        else {
          power = power - 0.1;
          if (power < 0) {
            power = 0;
          }
          lcd.setCursor(7, 0);
          lcd.print(power);
          lcd.print("W");
          lcd.write((uint8_t)1);
          lcd.print("  ");
        }
        break;
      case 4:
        switch (arrowpos) {
          case 0:
            if (!up) {
              screen4();
              lcd.setCursor(0, 1);
              lcd.write((uint8_t)0);
              arrowpos = 1;
            }
            break;
          case 1:
            if (up) {
              screen4();
              lcd.setCursor(0, 0);
              lcd.write((uint8_t)0);
              arrowpos = 0;
            }
            else {
              screen4();
              lcd.setCursor(7, 1);
              lcd.write((uint8_t)0);
              arrowpos = 2;
            }
            break;
          case 2:
            if (up) {
              screen4();
              lcd.setCursor(0, 1);
              lcd.write((uint8_t)0);
              arrowpos = 1;
            }
            break;
        }
        break;
      case 5:
        if (up) {
          current = current + 0.1;
          lcd.setCursor(9, 0);
          lcd.print(current);
          lcd.print("A");
          lcd.write((uint8_t)1);
          lcd.print(" ");
        }
        else {
          current = current - 0.1;
          if (current < 0) {
            current = 0;
          }
          lcd.setCursor(9, 0);
          lcd.print(current);
          lcd.print("A");
          lcd.write((uint8_t)1);
          lcd.print(" ");
        }
        break;
    }
    TurnDetected = false;
  }

  if (button) {
    delay(200);
    switch (screen) {
      case 0:
        if (arrowpos == 0) {
          screen = 1;
          screen1();
          lcd.setCursor(0, 0);
          lcd.write((uint8_t)0);
        }
        else {
          screen = 4;
          screen4();
          lcd.setCursor(0, 0);
          lcd.write((uint8_t)0);
        }
        break;
      case 1:
        switch (arrowpos) {
          case 0:
            screen = 2;
            screen2();
            break;
          case 1:
            powermode = true;
            screen = 3;
            screen3();
            break;
          case 2:
            screen = 0;
            screen0();
            lcd.setCursor(0, 0);
            lcd.write((uint8_t)0);
            break;
        }
        break;
      case 2:
        screen = 1;
        screen1();
        lcd.setCursor(0, 0);
        lcd.write((uint8_t)0);
        break;
      case 3:
        powermode = false;
        dac.setVoltage(0, false);
        dac_value = 0;
        counter = 0;
        screen = 1;
        screen1();
        lcd.setCursor(0, 0);
        lcd.write((uint8_t)0);
        break;
      case 4:
        switch (arrowpos) {
          case 0:
            screen = 5;
            screen5();
            break;
          case 1:
            screen = 6;
            screen6();
            currentmode = true;
            counter = 0;
            break;
          case 2:
            screen = 0;
            screen0();
            lcd.setCursor(0, 0);
            lcd.write((uint8_t)0);
            break;
        }
        break;
      case 5:
        screen = 4;
        screen4();
        lcd.setCursor(0, 0);
        lcd.write((uint8_t)0);
        break;
      case 6:
        screen = 4;
        screen4();
        lcd.setCursor(0, 0);
        lcd.write((uint8_t)0);
        currentmode = false;
        dac.setVoltage(0, false);
        dac_value = 0;
        break;
    }
    arrowpos = 0;
    button = false;
  }
}

void screen0() {
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Const. Power");
  lcd.setCursor(1, 1);
  lcd.print("Const. Current");
}

void screen1() {
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Power:");
  lcd.print(power);
  lcd.print("W");
  lcd.setCursor(1, 1);
  lcd.print("Start");
  lcd.setCursor(8, 1);
  lcd.print("Back");
}

void screen2() {
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Power:");
  lcd.print(power);
  lcd.print("W");
  lcd.write((uint8_t)1);
}

void screen3() {
  lcd.clear();
  lcd.print("Set:");
  lcd.print(power);
  lcd.print("W");
  lcd.setCursor(0, 1);
  lcd.print("Cur:");
  lcd.print(curpower);
  lcd.print("W");
  lcd.setCursor(11, 1);
  lcd.write((uint8_t)0);
  lcd.print("STOP");
}

void screen4() {
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Current:");
  lcd.print(current);
  lcd.print("A");
  lcd.setCursor(1, 1);
  lcd.print("Start");
  lcd.setCursor(8, 1);
  lcd.print("Back");
}

void screen5() {
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Current:");
  lcd.print(current);
  lcd.print("A");
  lcd.write((uint8_t)1);
}

void screen6() {
  lcd.clear();
  lcd.print("Set:");
  lcd.print(current);
  lcd.print("A");
  lcd.setCursor(0, 1);
  lcd.print("Cur:");
  lcd.print(curcurrent);
  lcd.print("A");
  lcd.setCursor(11, 1);
  lcd.write((uint8_t)0);
  lcd.print("STOP");
}
