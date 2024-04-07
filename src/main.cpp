#include <Arduino.h>
#include "ESP32Wiimote.h"

#define PWMA_PIN 16
#define AIN1_PIN 14
#define AIN2_PIN 12
#define PWMB_PIN 27
#define BIN1_PIN 17
#define BIN2_PIN 13

#define motorFreq       15000
#define motorResolution 8
 
#define motorAChannel   3
#define motorBChannel   4

ESP32Wiimote wiimote;

int motorAPWM = 255;
int motorBPWM = 255;
int wheel = 0;

void setup()
{
    Serial.begin(115200);
    Serial.println("ESP32 Wiimote Car");

    pinMode(PWMA_PIN, OUTPUT);
    pinMode(PWMB_PIN, OUTPUT);
    pinMode(AIN1_PIN, OUTPUT);
    pinMode(AIN2_PIN, OUTPUT);
    pinMode(BIN1_PIN, OUTPUT);
    pinMode(BIN2_PIN, OUTPUT);

    ledcSetup(motorAChannel, motorFreq, motorResolution);
    ledcSetup(motorBChannel, motorFreq, motorResolution);
 
    ledcAttachPin(PWMA_PIN, motorAChannel);
    ledcAttachPin(PWMB_PIN, motorBChannel);
    
    wiimote.init();
    Serial.println("Started");
}

void loop() {
    wiimote.task();

    if (wiimote.available() > 0) 
    {
        ButtonState  button  = wiimote.getButtonState();
        AccelState   accel   = wiimote.getAccelState();

        motorAPWM = 255;
        motorBPWM = 255;
        wheel = 0;
        
        if(!(button & BUTTON_ONE) && !(button & BUTTON_TWO))
        {
            if(button & BUTTON_UP)
            {
                digitalWrite(AIN1_PIN, LOW);
                digitalWrite(AIN2_PIN, HIGH);
                digitalWrite(BIN1_PIN, HIGH);
                digitalWrite(BIN2_PIN, LOW);
            }

            if(button & BUTTON_DOWN)
            {
                digitalWrite(AIN1_PIN, HIGH);
                digitalWrite(AIN2_PIN, LOW);
                digitalWrite(BIN1_PIN, LOW);
                digitalWrite(BIN2_PIN, HIGH);
            }

            if(button & BUTTON_LEFT)
            {
                digitalWrite(AIN1_PIN, HIGH);
                digitalWrite(AIN2_PIN, LOW);
                digitalWrite(BIN1_PIN, HIGH);
                digitalWrite(BIN2_PIN, LOW);
            }

            if(button & BUTTON_RIGHT)
            {
                digitalWrite(AIN1_PIN, LOW);
                digitalWrite(AIN2_PIN, HIGH);
                digitalWrite(BIN1_PIN, LOW);
                digitalWrite(BIN2_PIN, HIGH);
            }

            if (!(button & BUTTON_UP) && !(button & BUTTON_DOWN) && !(button & BUTTON_LEFT) && !(button & BUTTON_RIGHT))
            {
                digitalWrite(AIN1_PIN, LOW);
                digitalWrite(AIN2_PIN, LOW);
                digitalWrite(BIN1_PIN, LOW);
                digitalWrite(BIN2_PIN, LOW);
            }
        }else 
        {
            if((button & BUTTON_ONE) && (button & BUTTON_TWO))
            {
                digitalWrite(AIN1_PIN, LOW);
                digitalWrite(AIN2_PIN, LOW);
                digitalWrite(BIN1_PIN, LOW);
                digitalWrite(BIN2_PIN, LOW);
            } else 
            {
                wheel = map(accel.yAxis, 101, 152, -255, 255);
                if(wheel > 10)
                {
                    motorAPWM = 255;
                    motorBPWM = 255 - abs(wheel);
                }else if(wheel < 0)
                {
                    motorAPWM = 255 - abs(wheel);
                    motorBPWM = 255;
                }else 
                {
                    motorAPWM = 255;
                    motorBPWM = 255;
                }

                if(button & BUTTON_ONE)
                {
                digitalWrite(AIN1_PIN, LOW);
                digitalWrite(AIN2_PIN, HIGH);
                digitalWrite(BIN1_PIN, LOW);
                digitalWrite(BIN2_PIN, HIGH);                        
                }

                if(button & BUTTON_TWO)
                {
                digitalWrite(AIN1_PIN, HIGH);
                digitalWrite(AIN2_PIN, LOW);
                digitalWrite(BIN1_PIN, HIGH);
                digitalWrite(BIN2_PIN, LOW);
                }
            }
        }

        ledcWrite(motorAChannel, motorAPWM);
        ledcWrite(motorBChannel, motorBPWM);

        Serial.print(wheel);
        Serial.print(" ");
        Serial.print(motorAPWM);
        Serial.print(" ");
        Serial.println(motorBPWM);
    }
    delay(10);
}