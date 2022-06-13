#include <Bounce2.h>
#include <Adafruit_Sensor.h>
#include "DHT.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#define DHT11_PIN 2 //온습도 센서
#define DHTTYPE DHT11

int debounce_time= 10; // 5 ~20ms
boolean ledOn=false;
Bounce bouncer = Bounce();

DHT dht(DHT11_PIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);
const int sensorPin= 0; // sensor Pin => A0
int liquid_level;// 수위값 저장

void setup()
{
  pinMode(9, OUTPUT);//초록
  pinMode(8, OUTPUT);//빨강
  bouncer.attach(11, INPUT_PULLUP);//동작버튼
  bouncer.interval(debounce_time);
  pinMode(4, OUTPUT);//가습기 조절

  lcd.begin();
  lcd.noBacklight();

  Serial.begin(9600);
  dht.begin();
}
void loop()
{
  bouncer.update() ;
  if ( bouncer.fell()) // 눌러진 경우 (상승 에지 감지)
  {
    ledOn = !ledOn; // LED값을 Toggle
    if(!ledOn){
      digitalWrite(9,LOW);
      lcd.noBacklight();
    }else{
      lcd.backlight();
      digitalWrite(9,HIGH);
    }
  }

  if(ledOn){
  float humidity = dht.readHumidity(); // 상대 습도 읽기
  float temperature = dht.readTemperature(); // 온도 읽기
  liquid_level = analogRead(sensorPin); // 물수위 읽기

  Serial.print("Liquid=");
  Serial.println(liquid_level);// 물수위 전달
  Serial.print("Temperature=");
  Serial.print(temperature);
  Serial.print(" Humidity=");
  Serial.println(humidity);

  lcd.clear();
  lcd.home();
  //lcd에 정보 출력
  lcd.print("L= ");
  lcd.print(liquid_level);
  lcd.setCursor(0,1);
  lcd.print("H=");
  lcd.print(humidity);
  lcd.print(" T=");
  lcd.print(temperature);
  delay(1000);
  }
  
  if (Serial.available())//서버에서 값 받아오기
  {
    String value = Serial.readString();

    //가습기 동작하도록 추가해야함
    digitalWrite(4,HIGH);//가습기ON(HIGH를 주면 ON,LOW를 주면 OFF)
    
  }
}
