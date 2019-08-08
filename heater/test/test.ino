/*
nRF24L01 Arduino Receiver接收端

Ansifa
2015/3/7

引脚接法：
nRF24L01   Arduino UNO
VCC <-> 3.3V
GND <-> GND
CE  <-> D9
CSN <-> D10
MOSI<-> D11
MISO<-> D12
SCK <-> D13
IRQ <-> 不接
*/
#include <Wire.h>
#include "RTClib.h"

RTC_DS3231 rtc;
#include <Servo.h>   
#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>

    
    unsigned int adata = 0, total = 0;
    int minutes = 0;
    int pos = 0; 
  
    Servo myservo;
void setup()
{
  #ifndef ESP8266
  while (!Serial); // for Leonardo/Micro/Zero
  #endif

    myservo.attach(3);
    Serial.begin(9600);

     delay(3000); // wait for console opening

      if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
    if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }

    //---------初始化部分，不可随时修改---------
    Mirf.cePin = 9;     //设置CE引脚为D9
    Mirf.csnPin = 10;   //设置CE引脚为D10
    Mirf.spi = &MirfHardwareSpi;
    Mirf.init();  //初始化nRF24L01

    //---------配置部分，可以随时修改---------
    //设置接收标识符"Rev01"
    Mirf.setRADDR((byte *)"Rec01");
    //设置一次收发的字节数，这里发一个整数，
    //写sizeof(unsigned int)，实际等于2字节
    Mirf.payload = sizeof(unsigned int);
    //发送通道，可以填0~128，收发必须一致。
    Mirf.channel = 3;
    Mirf.config();

    //注意一个Arduino写Sender.ino，另一个写Receiver.ino。
    //这里用来辨别写入了Receiver.ino程序
    Serial.println("I'm Receiver...");
}

void loop()
{
work();

}
void work()
{
     for(pos = 0; pos < 180; pos += 1)  // 从0度到180度运动 
  {                                                     // 每次步进一度
    myservo.write(pos);        // 指定舵机转向的角度
    delay(15);                       // 等待15ms让舵机到达指定位置
  } 
  for(pos = 180; pos>=1; pos-=1)   //从180度到0度运动  
  {                                
    myservo.write(pos);         // 指定舵机转向的角度 
    delay(15);                        // 等待15ms让舵机到达指定位置 
  } 
          DateTime now = rtc.now();
             Serial.print("adata=");
            Serial.println(adata);
            minutes = now.minute();
            Serial.print("minutes=");
          Serial.println(minutes);
        while(total <= 15)
        {
          Serial.print("total=");
          Serial.println(total);

          Serial.print("now.minute()=");
           Serial.println(now.minute());
            DateTime now = rtc.now();
          if ( minutes != now.minute())
          {
            total = total +1;
            Serial.println(total);
            if( minutes == 59)
            {
              minutes = 0;
            }
            else
            {
              minutes = minutes + 1;
            }
            delay(300);
          }
        }
       if (total == 16 )
       total = 0;
 }
        

       
  

