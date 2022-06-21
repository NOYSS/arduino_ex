/* Dev by Thanakorn 2022/06/21 */
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "DHT.h"
#include "RTClib.h"

// OLED
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///<  0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// DHT
#define DHTPIN 2
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
DHT dht(DHTPIN, DHTTYPE);

RTC_DS1307 RTC;     // Setup an instance of DS1307 naming it RTC

#define BUZZER_PIN 8

void setup () {
  Serial.begin(57600); // Set serial port speed
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, HIGH);  //ปิดเสียงเตือน
  delay(100);
  digitalWrite(BUZZER_PIN, LOW);   //เปิดเสียงเตือน
  delay(200);
  digitalWrite(BUZZER_PIN, HIGH);  //ปิดเสียงเตือน
 
  Wire.begin(); // Start the I2C
  RTC.begin();  // Init RTC
  RTC.adjust(DateTime(__DATE__, __TIME__));  // Time and date is expanded to date and time on your computer at compiletime
  Serial.print('Time and date set');

  // OLED SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }

  // DHT
  dht.begin();
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }

}

void loop () {
  delay(1000);
  display.clearDisplay();
  dateTimeModule();
  dhtModule();
  display.display();
  delay(100);
}

void dateTimeModule() {
  DateTime now = RTC.now();
  String dateShow = String(now.day()) + "/" + String(now.month()) + "/" + String(now.year()) + "  " + String(now.hour()) + ":" + String(now.minute(), DEC);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(dateShow);
}

void dhtModule() {
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  String temperatureShow = "Temperature : " + String(t, 0) + " *C";
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 12);
  display.println(temperatureShow);

  String humidityShow = "Humidity: " + String(h, 0) + " %";
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 24);
  display.println(humidityShow);
}

void piezoModule() {

}
