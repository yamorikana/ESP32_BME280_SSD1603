/*-------------------------------------------
 *  OLEDと、環境センサBME280
 * 
 *  mcu : esp32-wroom-32d/32e / ESP8266
 *  
*/
extern void HotStart();
extern void print280Values();
extern void setup280();
extern void bme280_PROC();

#include <Adafruit_BME280.h>     // bme280
#include<Wire.h>                 // i^2c
#include <SSD1306.h>
 
// 定数宣言
#define SEALEVELPRESSURE_HPA (1013.25)    // bme280用定数

// class宣言
Adafruit_BME280 bme; // I2C
SSD1306  display(0x3c, 21, 22); 

// 変数宣言     "0123456789ABCDEF"
unsigned long delayTime;
unsigned status;
static int value;     //analog値を代入する変数を定義
static float Volts;
static float FloatValue;
unsigned static int Val_ad;

//◆◆ 初期化 ◆◆◆◆◆◆◆◆◆◆
void setup() {
    Serial.begin(115200);
    // ＬＣＤ設定
    Wire.begin(21,22); // 端子の定義Wire.begin(SDA,SCL)
    display.init();  
    display.setFont(ArialMT_Plain_16);  
    display.drawString(0, 0, "ESP32 and OLED"); 
    display.display(); 
    setup280();        // BME280初期化
}

//◆◆ メインループ ◆◆◆◆◆◆◆◆◆◆
void loop(){
    bme280_PROC();  // 環境センサメイン処理部
    delay(500);
}

// BME280の取得＆出力
void bme280_PROC(){
    display.clear(); 
    print280Values();
}

//◆◆ BME280の状況を出力 ◆◆◆◆◆◆◆◆◆◆
void print280Values() {
    // 温度の計測＆OLED表示
    display.drawString(0, 0, "Temp = "); 
    FloatValue =  bme.readTemperature() ;  // 取得
    display.drawString( 60 , 0,  String(FloatValue,8)); 
    // 気圧の計測＆OLED表示
    display.drawString(0, 16, "Press = "); 
    FloatValue =  bme.readPressure() / 100.0F ;  // 取得
    display.drawString(60, 16, String (FloatValue,8)); 
     // 湿度の計測＆OLED表示
    display.drawString(0, 32, "Humi = "); 
    FloatValue =  bme.readHumidity() ;  // 取得
    display.drawString(60, 32, String (FloatValue,8)); 
     // 高度の計測＆OLED表示
    display.drawString(0, 48, "Altitu = "); 
    FloatValue =  bme.readAltitude(SEALEVELPRESSURE_HPA) ; 
    display.drawString(60, 48, String (FloatValue,8)); 
    display.display(); 
}

//-------------------------------------------------------------
// BME280の接続確認
void setup280(){
      status = bme.begin();  
    if (!status) {
        display.drawString(0, 0,  "BME280 sensor"); 
        display.drawString(16,16, "Connection"); 
        display.drawString(32,32, "Error....!"); 
        display.display(); 
        while (1) delay(10);
    }
    display.drawString(8,8, "BME280 Ready!"); 
    delay(500);
    delayTime = 500;
}
