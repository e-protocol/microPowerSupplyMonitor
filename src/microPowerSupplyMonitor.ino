/* Device: VoltAmp Meter 
 * Board: Arduino Nano
 * Modules: Ina219, 
 * OLED 0.96 Display SSD1306 128x64
 * Details: 
 * 0x3C - I2C address Display
 * 0x40 - I2C address Ina219
 * Created by E-Protocol
 */

//define display 0.96 128x64 SSD1306
#include "U8glib.h"
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE | U8G_I2C_OPT_DEV_0);

//define INA219 volt-amp meter
#include <Wire.h>
#include <Adafruit_INA219.h>
Adafruit_INA219 meterVA;

//battery
#define ARRAY_SIZE 5
float maxBatVolt = 4.2; 
float minBatVolt = 2.4; 
float maxAnalogValue, minAnalogValue;
float batBuff[ARRAY_SIZE];
int curIndex = 0;
float shiftCoef = 1.175; //analog data shift, income values greater than real

void setup() 
{ 
  meterVA.begin();
  u8g.setColorIndex(1);
  //meterVA.setCalibration_16V_400mA();
  meterVA.setCalibration_32V_1A();
  pinMode(A2,INPUT);
  minAnalogValue = minBatVolt / 5.0 * 1023.0 * shiftCoef;
  maxAnalogValue = maxBatVolt / 5.0 * 1023.0 * shiftCoef;

  //init battery buffer
  for(int k = 0; k < ARRAY_SIZE; k++)
    batBuff[k] = 0.0;
}

//Battery position in pixels
#define CHARGE_AREA_START_X     95
#define CHARGE_AREA_START_Y     5
#define CHARGE_AREA_WIDTH       20
#define CHARGE_AREA_HEIGHT      12

void showBatteryLevel(float percent)
{
    if (percent > 100.0)
      percent = 100.0;
    else if( percent < 1.0)
      percent = 1.0;

    float widthCoef = percent / 100.0;

    //better display bars
    if(widthCoef >= 0.95)
        widthCoef = 1.0;

    //draw battery
    u8g.drawFrame(CHARGE_AREA_START_X, CHARGE_AREA_START_Y, 
                  CHARGE_AREA_WIDTH + 4, CHARGE_AREA_HEIGHT);
    u8g.drawBox(CHARGE_AREA_START_X + CHARGE_AREA_WIDTH + 4, 
                CHARGE_AREA_START_Y + (int)(CHARGE_AREA_HEIGHT / 4), 
                3, (int)(CHARGE_AREA_HEIGHT / 2));

    //fill battery
    u8g.drawBox(CHARGE_AREA_START_X + 2, CHARGE_AREA_START_Y + 2,
                (int)(CHARGE_AREA_WIDTH * widthCoef), 
                CHARGE_AREA_HEIGHT - 4);

    //display battery level
    u8g.setFont(u8g_font_9x18);
    u8g.setPrintPos(CHARGE_AREA_START_X - 2, 
                    CHARGE_AREA_HEIGHT * 2 + 7);
    u8g.print(String(percent,0) + "%");            
}

float getMapVal(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

float getBattery()
{
  float chargeLevel = getMapVal((float)analogRead(2), minAnalogValue, 
        maxAnalogValue, 0.0, 100.0);

  batBuff[curIndex] = chargeLevel;
  float battery = 0.0;
  curIndex++;

  if(curIndex >= ARRAY_SIZE)
    curIndex = 0;

  for(int k = 0; k < ARRAY_SIZE; k++)
    battery += batBuff[k];

  battery /= (float)ARRAY_SIZE;

  if(battery < 0.0)
    battery = 0.0;
  else if(battery >= 100.0)
    battery = 100.0;

  return battery;
}

void loop() 
{   
  //ina219 data
  float busvoltage = meterVA.getBusVoltage_V();
  float shuntvoltage = meterVA.getShuntVoltage_mV();
  float volt = busvoltage + (shuntvoltage / 1000.0);
  float amp = meterVA.getCurrent_mA() / 1000.0;
  float power = amp * volt; //meterVA.getPower_mW() / 1000.0;

  if(isinf(amp) || isnan(amp))
    amp = 0.0;

  //get battery charge
  float battery = getBattery();

  //print data on screen
  u8g.firstPage();
  
  do 
  {
    u8g.setFont(u8g_font_9x15);
    uint8_t yPos = 15;
    uint8_t xPos = 10;
    uint8_t ySpace = 18;
  
    u8g.setPrintPos(10,yPos);
    u8g.print("V " + String(volt,2));
    yPos += ySpace;
    
    u8g.setPrintPos(10,yPos);
    u8g.print("A " + String(amp,2));
    yPos += ySpace;
    
    u8g.setPrintPos(10,yPos);
    u8g.print("W " + String(power,2));
    showBatteryLevel(battery);
  } while(u8g.nextPage());
  
  delay(2000);
}
