#include "Wire.h"
#include <Adafruit_PWMServoDriver.h>

#define LED_WIFI 1
#define LED_ACT 2
#define LED_ERR 3

#define PCA9685_I2C_ADDR 64

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

void setLed(int id, float ratio) {
    int val = floor(ratio*4096);
    pwm.setPWM(id,
               4096-val,
               val);
}

int idxLedIndicator(int channel) {
    if ((channel < 1) || (channel > 5))
        return 0;

    static int idx[5] = {14, 12, 10, 4, 6};

    return idx[channel-1];
}

int idxLedPwm(int channel) {
    if ((channel < 1) || (channel > 5))
        return 0;

    static int idx[5] = {13, 11, 9, 5, 7};

    return idx[channel-1];
}

void setup() {
  Wire.begin(5, 4); // SDA, SCL
  Serial.begin(115000);

  pinMode(12, OUTPUT);
  digitalWrite(12, LOW);

  pwm.setPWMFreq(1600);
  pwm.begin();

  // switch off all LEDs
  for (int i = 0; i < 16; i++)
      setLed(i, false);
  for (int i = 1; i < 6; i++)
    setLed(idxLedPwm(i), false);

  Serial.write("\nSetup finished.\n");
}


void loop() {
    setLed(LED_ACT, true);

    for (int i = 0; i < 6; i++) {
        setLed(idxLedIndicator(i), 0);
        setLed(idxLedPwm(i), 0);
        delay(500);
        setLed(idxLedIndicator(i), 1);
        setLed(idxLedPwm(i), 1);
    }

    delay(500);

    for (int v = 4096; v >= 0; v--) {
        float r = 4096/v;
        for (int i = 0; i < 6; i++) {
            setLed(idxLedIndicator(i), 1);
            setLed(idxLedPwm(i), r);
        }
        delay(1);
    }

    Serial.write("Loop end.\n");
}


