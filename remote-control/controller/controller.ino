#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <can.h>
#include <mcp2515.h>
#include <Servo.h>
#include <SPI.h>
#define maxdeger 1940 //max 2000 oluyor escler 1000-2000 arası calisir
#define mindeger 1060
#define sabitleme_toleransi 30,
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
struct can_frame canSend;
struct can_frame canRcv;

//CANBUS
MCP2515 mcp2515(8);

float hizBoleni = 1.0;
int button = 2;
bool button_deger = false;
bool clear_state = true;

//LCD için değerler
int dis_sicaklik;
int dis_nem;
int basinc_deger;
int sicak_deger;
bool lcd_durum = false;
#define IUDSZ_BMPWIDTH  128
#define IUDSZ_BMPHEIGHT  60
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
const unsigned char bitmap_iudsz[] PROGMEM = {
  B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11110000,
  B11000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00110000,
  B11000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00110000,
  B11000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00110000,
  B11000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00110000,
  B11000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00110000,
  B11000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00110000,
  B11000000,B00000011,B10000000,B00111000,B00000001,B11001111,B11111111,B00000001,B11100000,B00011100,B00001111,B00000000,B11110000,B00111000,B00000000,B00110000,
  B11000000,B00000011,B11000000,B00111100,B00000001,B11001111,B11111111,B00000001,B11100000,B00011100,B00000111,B10000001,B11100000,B00111100,B00000000,B00110000,
  B11000000,B00000111,B11000000,B00111110,B00000001,B11001111,B11111111,B00000011,B11110000,B00011100,B00000011,B10000001,B11000000,B01111100,B00000000,B00110000,
  B11000000,B00000111,B11100000,B00111110,B00000001,B11000000,B01110000,B00000011,B11110000,B00011100,B00000011,B11000011,B11000000,B01111110,B00000000,B00110000,
  B11000000,B00000111,B11100000,B00111111,B00000001,B11000000,B01110000,B00000111,B11110000,B00011100,B00000001,B11100011,B10000000,B01111110,B00000000,B00110000,
  B11000000,B00001110,B01100000,B00111111,B10000001,B11000000,B01110000,B00000111,B00111000,B00011100,B00000000,B11100111,B00000000,B11101110,B00000000,B00110000,
  B11000000,B00001110,B01100000,B00111011,B10000001,B11000000,B01110000,B00000111,B00111000,B00011100,B00000000,B11111111,B00000000,B11100111,B00000000,B00110000,
  B11000000,B00011100,B01110000,B00111001,B11000001,B11000000,B01110000,B00001110,B00011000,B00011100,B00000000,B01111110,B00000000,B11000111,B00000000,B00110000,
  B11000000,B00011100,B01110000,B00111001,B11100001,B11000000,B01110000,B00001110,B00011000,B00011100,B00000000,B00111100,B00000000,B11000111,B00000000,B00110000,
  B11000000,B00011100,B00110000,B00111000,B11110001,B11000000,B01110000,B00011110,B00011100,B00011100,B00000000,B00011100,B00000001,B11000011,B10000000,B00110000,
  B11000000,B00111000,B00111000,B00111000,B01111001,B11000000,B01110000,B00011100,B00011100,B00011100,B00000000,B00011100,B00000001,B11000011,B10000000,B00110000,
  B11000000,B00111100,B01111000,B00111000,B00111101,B11000000,B01110000,B00011100,B00011110,B00011100,B00000000,B00011000,B00000011,B11000011,B11000000,B00110000,
  B11000000,B01111111,B11111000,B00111000,B00011101,B11000000,B01110000,B00111111,B11111110,B00011100,B00000000,B00011000,B00000011,B11111111,B11000000,B00110000,
  B11000000,B01111111,B11111100,B00111000,B00011111,B11000000,B01110000,B00111111,B11111110,B00011100,B00000000,B00011000,B00000011,B11111111,B11000000,B00110000,
  B11000000,B01110000,B00011100,B00111000,B00001111,B11000000,B01110000,B00111000,B00000111,B00011100,B00000000,B00011100,B00000111,B00000000,B11100000,B00110000,
  B11000000,B11110000,B00001110,B00111000,B00000111,B11000000,B01110000,B01110000,B00000111,B00011111,B11110000,B00011100,B00000111,B00000000,B11100000,B00110000,
  B11000000,B11100000,B00001110,B00111000,B00000011,B11000000,B01110000,B01110000,B00000111,B00011111,B11110000,B00011000,B00000111,B00000000,B11100000,B00110000,
  B11000000,B11100000,B00001111,B00111000,B00000001,B11000000,B01110000,B11110000,B00000011,B10011111,B11110000,B00011100,B00001110,B00000000,B01110000,B00110000,
  B11000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00110000,
  B11000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00110000,
  B11011100,B00000110,B00000111,B00000111,B00000001,B10000000,B11100001,B11100000,B01110000,B00111000,B00110000,B00011000,B00001110,B00001110,B00000111,B00110000,
  B11111110,B00011111,B00001111,B10001111,B10000111,B11000011,B11100011,B11100001,B11111000,B11111100,B11111000,B01111100,B00111111,B00111110,B00011111,B10110000,
  B11111000,B00111100,B00011110,B00011110,B00001111,B00000111,B10000111,B10000011,B11000001,B11100001,B11100000,B11110000,B01111100,B01111000,B00111100,B00110000,
  B11111000,B01111100,B00111100,B00111110,B00011111,B00001111,B00001111,B00000111,B11000001,B11100001,B11000001,B11110000,B01111000,B01111000,B00111100,B00110000,
  B11111000,B11111100,B01111110,B00111110,B00111111,B00001111,B10001111,B10001111,B11000011,B11100011,B11100011,B11110000,B11111100,B11111100,B11111100,B01110000,
  B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11110000,
  B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11110000,
  B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11110000,
  B11111111,B11111111,B11111111,B11111111,B11111110,B00111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11110000,
  B11111111,B11111111,B11111111,B11111111,B11111110,B00111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11110000,
  B11111111,B11111111,B11111111,B11111111,B11111110,B00111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11110000,
  B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11110000,
  B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11110000,
  B11111111,B11111111,B11111111,B11111111,B11111110,B00111111,B00000111,B11111111,B00011111,B11111100,B00011111,B11111111,B11111111,B11111111,B11111111,B11110000,
  B11111111,B11111111,B11111111,B11111111,B11111110,B00111100,B00000001,B11111110,B00001111,B11111000,B00000111,B11111111,B11111111,B11111111,B11111111,B11110000,
  B11111111,B11111111,B11111111,B11111111,B11111110,B00111100,B00000001,B11111110,B00001111,B11110000,B00000111,B11111111,B11111111,B11111111,B11111111,B11110000,
  B11111111,B11111111,B11111111,B11111111,B11111110,B00111000,B11111011,B11111110,B00000111,B11100001,B11001111,B11111111,B11111111,B11111111,B11111111,B11110000,
  B11111111,B11111111,B11111111,B11111111,B11111110,B00111000,B11111111,B11111100,B00000111,B11100011,B11111111,B11111111,B11111111,B11111111,B11111111,B11110000,
  B11111111,B11111111,B11111111,B11111111,B11111110,B00111000,B11111111,B11111100,B01100111,B11100001,B11111111,B11111111,B11111111,B11111111,B11111111,B11110000,
  B11111111,B11111111,B11111111,B11111111,B11111110,B00111000,B00111111,B11111000,B01100011,B11110000,B11111111,B11111111,B11111111,B11111111,B11111111,B11110000,
  B11111111,B11111111,B11111111,B11111111,B11111110,B00111100,B00001111,B11111000,B11100011,B11110000,B00111111,B11111111,B11111111,B11111111,B11111111,B11110000,
  B11111111,B11111111,B11111111,B11111111,B11111110,B00111110,B00000011,B11110000,B11110001,B11111000,B00001111,B11111111,B11111111,B11111111,B11111111,B11110000,
  B11111111,B11111111,B11111111,B11111111,B11111110,B00111111,B10000001,B11110000,B11110001,B11111110,B00000111,B11111111,B11111111,B11111111,B11111111,B11110000,
  B11111111,B11111111,B11111111,B11111111,B11111110,B00111111,B11100000,B11110001,B11110001,B11111111,B10000011,B11111111,B11111111,B11111111,B11111111,B11110000,
  B11111111,B11111111,B11111111,B11111111,B11111110,B00111111,B11111000,B11100001,B11110000,B11111111,B11000011,B11111111,B11111111,B11111111,B11111111,B11110000,
  B11111111,B11111111,B11111111,B11111111,B11111110,B00111111,B11111000,B11100000,B00000000,B11111111,B11100001,B11111111,B11111111,B11111111,B11111111,B11110000,
  B11111111,B11111111,B11111111,B11111111,B11111110,B00111101,B11111100,B11100000,B00000000,B11110111,B11100001,B11111111,B11111111,B11111111,B11111111,B11110000,
  B11111111,B11111111,B11111111,B11111111,B11111110,B00111000,B11111000,B11000011,B11111000,B01100011,B11100011,B11111111,B11111111,B11111111,B11111111,B11110000,
  B11111111,B11111111,B11111111,B11111111,B11111110,B00111000,B00000000,B11000111,B11111100,B01100000,B00000011,B11111111,B11111111,B11111111,B11111111,B11110000,
  B11111111,B11111111,B11111111,B11111111,B11111110,B00111100,B00000001,B10000111,B11111100,B01110000,B00000111,B11111111,B11111111,B11111111,B11111111,B11110000,
  B11111111,B11111111,B11111111,B11111111,B11111110,B00111110,B00000011,B10000111,B11111100,B00111100,B00001111,B11111111,B11111111,B11111111,B11111111,B11110000,
  B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11110000,
  B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11110000,
  B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11110000,
  B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11110000
};

//Joystick değerleri - 2 koordinat
int pinJoyStick_X_1 = 1;
int pinJoyStick_Y_1 = 0;
int pinJoyStick_X_2 = 2;
int pinJoyStick_Y_2 = 3;

int valueJoyStick_X_1 = 0;
int valueJoyStick_Y_1 = 0;
int valueJoyStick_X_2 = 0;
int valueJoyStick_Y_2 = 0;

union ArrayToInteger {
  byte array[2];
  int integer;
} converter;
union ArrayToDouble {
  byte array[4];
  double number;
} doubler;


void setup()
{
  Serial.begin(9600);
  SPI.begin();
  
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize I2C addr to 0x3C ( for 128x64 Display )
  display.clearDisplay(); // clear the display before starting the program to avoid adafruit splashscreen ( *we can also skip it by modifing header file )
  display.drawBitmap(0, 0, bitmap_iudsz, 126, 60, WHITE);
  display.display();
  delay(5000);
  
  pinMode(button , INPUT);
  attachInterrupt(digitalPinToInterrupt(button), lcd_state, CHANGE);
  
  mcp2515.reset();
  mcp2515.setBitrate(CAN_125KBPS, MCP_8MHZ);
  mcp2515.setNormalMode();
}

void loop()
{
  //  currentTime = millis();
  if (Serial.available())
  {
    int incomingByte = Serial.read();
    hizBoleni = incomingByte / 10.0;
  }

  // Araçtan gelen değerler bölünerek USB Üzerine yazılıyor.
  // Eğer araçtan bir bilgi yukarı gelecek ise bunu Can_ID'si üzerinden belirtmeniz gerekir.
  if (mcp2515.readMessage(&canRcv) == MCP2515::ERROR_OK)
  {
    if (canRcv.can_id == 0x03)
    {
      doubler.array[0] = canRcv.data[0];
      doubler.array[1] = canRcv.data[1];
      doubler.array[2] = canRcv.data[2];
      doubler.array[3] = canRcv.data[3];
      Serial.print('b'); // basinc belirteci
      Serial.println(doubler.number);
      basinc_deger = doubler.number;
      doubler.array[0] = canRcv.data[4];
      doubler.array[1] = canRcv.data[5];
      doubler.array[2] = canRcv.data[6];
      doubler.array[3] = canRcv.data[7];
      Serial.print('s'); // sicaklik belirteci
      Serial.println(doubler.number);
      sicak_deger = doubler.number;
    }
  }
  else {
    //Serial.println("hata");
  }

  // Analogread 0-1023 arasında okuma yapar, burada esc değerleri olan 1000-2000 arasına eşitleniyor.
  // Herhangi bir joystick değerini 3000'den çıkarmak, joystick eksenini ters çevirme anlamına gelir.
  valueJoyStick_X_1 = analogRead(pinJoyStick_X_1) + 1000;
  valueJoyStick_Y_1 = (analogRead(pinJoyStick_Y_1) + 1000); //Buradaki 3000'i sildik. Eklenebilir. (3000 - ....)
  valueJoyStick_X_2 = (analogRead(pinJoyStick_X_2) + 1000); //Buradaki 3000'i sildik. Eklenebilir.
  valueJoyStick_Y_2 = 3000 - (analogRead(pinJoyStick_Y_2) + 1000);
  
  // Joystick değerlerini merkezi değiştirmeden bölme işlemleri
  valueJoyStick_X_1 = 1500 + (valueJoyStick_X_1 - 1500) / hizBoleni;
  valueJoyStick_Y_1 = 1500 + (valueJoyStick_Y_1 - 1500) / hizBoleni;
  valueJoyStick_X_2 = 1500 + (valueJoyStick_X_2 - 1500) / hizBoleni;
  valueJoyStick_Y_2 = 1500 + (valueJoyStick_Y_2 - 1500) / hizBoleni;

  if (valueJoyStick_X_1 > maxdeger) valueJoyStick_X_1 = maxdeger;
  if (valueJoyStick_Y_1 > maxdeger) valueJoyStick_Y_1 = maxdeger;
  if (valueJoyStick_X_2 > maxdeger) valueJoyStick_X_2 = maxdeger;
  if (valueJoyStick_Y_2 > maxdeger) valueJoyStick_Y_2 = maxdeger;

  if (valueJoyStick_X_1 < mindeger) valueJoyStick_X_1 = mindeger;
  if (valueJoyStick_Y_1 < mindeger)valueJoyStick_Y_1 = mindeger;
  if (valueJoyStick_X_2 < mindeger) valueJoyStick_X_2 = mindeger;
  if (valueJoyStick_Y_2 < mindeger)valueJoyStick_Y_2 = mindeger;

  // joystick'ler belli bi toleransla ortadayken 1500'e sabitliyoruz
  if (valueJoyStick_X_1 < 1500 + sabitleme_toleransi / hizBoleni && valueJoyStick_X_1 > 1500 - sabitleme_toleransi / hizBoleni)
    valueJoyStick_X_1 = 1500;
  if (valueJoyStick_Y_1 < 1500 + sabitleme_toleransi / hizBoleni && valueJoyStick_Y_1 > 1500 - sabitleme_toleransi / hizBoleni)
    valueJoyStick_Y_1 = 1500;
  if (valueJoyStick_X_2 < 1500 + sabitleme_toleransi / hizBoleni && valueJoyStick_X_2 > 1500 - sabitleme_toleransi / hizBoleni)
    valueJoyStick_X_2 = 1500;
  if (valueJoyStick_Y_2 < 1500 + sabitleme_toleransi / hizBoleni && valueJoyStick_Y_2 > 1500 - sabitleme_toleransi / hizBoleni)
    valueJoyStick_Y_2 = 1500;
    
  // Joystick degerleri 8 byte'a siralanip yollaniyor
  canSend.can_id = 0x02;
  canSend.can_dlc = 8;
  canSend.data[0] = highByte(valueJoyStick_X_1);
  canSend.data[1] = lowByte(valueJoyStick_X_1);
  canSend.data[2] = highByte(valueJoyStick_Y_1);
  canSend.data[3] = lowByte(valueJoyStick_Y_1);
  canSend.data[4] = highByte(valueJoyStick_X_2);
  canSend.data[5] = lowByte(valueJoyStick_X_2);
  canSend.data[6] = highByte(valueJoyStick_Y_2);
  canSend.data[7] = lowByte(valueJoyStick_Y_2);

  // LCD'ye yazı yazdırma
  if (!lcd_durum)
  {
    if (clear_state == true) lcd.clear();
    clear_state = false;
    display.setCursor(0,0);
    display.print("Basinc:");
    display.setCursor(8, 0);
    .print(basinc_deger);
    display.setCursor(0, 1);
    display.print("Sicaklik:");
    display.setCursor(10, 1);
    display.print(sicak_deger);
  }
  if (lcd_durum)
  {
    if (clear_state == true) lcd.clear();
    clear_state = false;
    display.setCursor(0,0);
    display.print("x1:");
    display.setCursor(3, 0);
    display.print(valueJoyStick_X_1);
    display.setCursor(8, 0);
    display.print("x2:");
    display.setCursor(11, 0);
    display.print(valueJoyStick_X_2);
    display.setCursor(0, 1);
    display.print("y2:");
    display.setCursor(3, 1);
    display.print(valueJoyStick_Y_2);
    display.setCursor(8, 1);
    display.print("%");
    if (dis_nem < 10)
    {
      display.setCursor(9, 1);
      display.print("0");
      display.setCursor(10, 1);
      display.print(dis_nem);
    }
    else
    {
      display.setCursor(9, 1);
      display.print(dis_nem);
    }
    if (dis_sicaklik < 10)
    {
      display.setCursor(12, 1);
      display.print("0");
      display.setCursor(13, 1);
      display.print(dis_sicaklik);
    }
    else
    {
      display.setCursor(12, 1);
      display.print(dis_sicaklik);
    }
    display.setCursor(14, 1);
    display.print("C");
  }
  mcp2515.sendMessage(&canSend);
  delay(20);
}

// Bu fonksiyonu düzenlilik için alta aldım, umarım sorun olmaz
void lcd_state()
{
  if (lcd_durum == false) lcd_durum = true;
  else lcd_durum = false;
  clear_state = true;
  Serial.println(lcd_durum);
}