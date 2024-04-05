#include <Arduino.h>
#include "ESP32Wiimote.h"

ESP32Wiimote wiimote;

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
 
int motorAPWM = 255;
int motorBPWM = 255;
int wheel = 0;

void setup()
{
    Serial.begin(115200);
    Serial.println("ESP32Wiimote Car");

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

void loop()
{
    wiimote.task();

    if (wiimote.available() > 0) 
    {
        ButtonState  button  = wiimote.getButtonState();
        AccelState   accel   = wiimote.getAccelState();
        NunchukState nunchuk = wiimote.getNunchukState();

        char ca     = (button & BUTTON_A)     ? 'A' : '.';
        char cb     = (button & BUTTON_B)     ? 'B' : '.';
        char cc     = (button & BUTTON_C)     ? 'C' : '.';
        char cz     = (button & BUTTON_Z)     ? 'Z' : '.';
        char c1     = (button & BUTTON_ONE)   ? '1' : '.';
        char c2     = (button & BUTTON_TWO)   ? '2' : '.';
        char cminus = (button & BUTTON_MINUS) ? '-' : '.';
        char cplus  = (button & BUTTON_PLUS)  ? '+' : '.';
        char chome  = (button & BUTTON_HOME)  ? 'H' : '.';
        char cleft  = (button & BUTTON_LEFT)  ? '<' : '.';
        char cright = (button & BUTTON_RIGHT) ? '>' : '.';
        char cup    = (button & BUTTON_UP)    ? '^' : '.';
        char cdown  = (button & BUTTON_DOWN)  ? 'v' : '.';
    
        motorAPWM = 255;
        motorBPWM = 255;
        wheel = 0;
        
        if((c1 != '1') && (c2 != '2')){
            if(cup == '^'){
                digitalWrite(AIN1_PIN, LOW);
                digitalWrite(AIN2_PIN, HIGH);
                digitalWrite(BIN1_PIN, HIGH);
                digitalWrite(BIN2_PIN, LOW);
            }

            if(cdown == 'v'){
                digitalWrite(AIN1_PIN, HIGH);
                digitalWrite(AIN2_PIN, LOW);
                digitalWrite(BIN1_PIN, LOW);
                digitalWrite(BIN2_PIN, HIGH);
            }

            if(cleft == '<'){
                digitalWrite(AIN1_PIN, HIGH);
                digitalWrite(AIN2_PIN, LOW);
                digitalWrite(BIN1_PIN, HIGH);
                digitalWrite(BIN2_PIN, LOW);
            }

            if(cright == '>'){
                digitalWrite(AIN1_PIN, LOW);
                digitalWrite(AIN2_PIN, HIGH);
                digitalWrite(BIN1_PIN, LOW);
                digitalWrite(BIN2_PIN, HIGH);
            }

            if ((cup == '.') && (cdown == '.') && (cleft == '.') && (cright == '.')){
                digitalWrite(AIN1_PIN, LOW);
                digitalWrite(AIN2_PIN, LOW);
                digitalWrite(BIN1_PIN, LOW);
                digitalWrite(BIN2_PIN, LOW);
            }
        }else {
            if((c1 == '1') && (c2 == '2')){
                digitalWrite(AIN1_PIN, LOW);
                digitalWrite(AIN2_PIN, LOW);
                digitalWrite(BIN1_PIN, LOW);
                digitalWrite(BIN2_PIN, LOW);
            } else {
                wheel = map(accel.yAxis, 101, 152, -255, 255);
                if(wheel > 10){
                    motorAPWM = 255;
                    motorBPWM = 255 - abs(wheel);
                }else if(wheel < 0){
                    motorAPWM = 255 - abs(wheel);
                    motorBPWM = 255;
                }else {
                    motorAPWM = 255;
                    motorBPWM = 255;
                }

                if(c1=='1'){
                digitalWrite(AIN1_PIN, LOW);
                digitalWrite(AIN2_PIN, HIGH);
                digitalWrite(BIN1_PIN, LOW);
                digitalWrite(BIN2_PIN, HIGH);                        
                }
                if(c2 == '2'){
                digitalWrite(AIN1_PIN, HIGH);
                digitalWrite(AIN2_PIN, LOW);
                digitalWrite(BIN1_PIN, HIGH);
                digitalWrite(BIN2_PIN, LOW);
                }
            }
        }

        ledcWrite(motorAChannel, motorAPWM);
        ledcWrite(motorBChannel, motorBPWM);

        Serial.print(c1);
        Serial.print(c2);
        Serial.print(cleft);
        Serial.print(cright);
        Serial.print(cup);
        Serial.print(cdown);
        Serial.print(" ");
        Serial.print(accel.yAxis);
        Serial.print(" ");
        Serial.print(wheel);
        Serial.print(" ");
        Serial.print(motorAPWM);
        Serial.print(" ");
        Serial.println(motorBPWM);
    }
    
    delay(10);
}