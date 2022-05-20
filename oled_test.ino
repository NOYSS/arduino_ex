#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "DHT.h"

#define DHTPIN 2 
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///<  0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

DHT dht(DHTPIN, DHTTYPE);

float tempCheck = 0;
float humidityCheck = 0;
void setup() {
  Serial.begin(9600);
  dht.begin();
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
}

void loop() {
  // Wait a few seconds between measurements.
  delay(5000);

  // Reading temperature or humidity takes about 250 milliseconds!
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

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.print(F("°F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(hif);
  Serial.println(F("°F"));

  temperatureText(t,h);
}

void temperatureText(float t, float h) {
  if(tempCheck != t || humidityCheck != h){
   
  display.clearDisplay();

  tempCheck = t;
  display.setTextSize(3); 
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 4);
  display.println("T");
  display.setTextSize(3); 
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(40, 4);
  display.println(String(t, 0));
  display.setTextSize(3); 
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(94, 4);
  display.println("C");
  display.setTextSize(2); 
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(80, 0);
  display.println("o");

  humidityCheck = h;
  display.setTextSize(3); 
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 36);
  display.println("H");
  display.setTextSize(3); 
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(40, 36);
  display.println(String(h, 0));
  display.setTextSize(3); 
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(80, 36);
  display.println("%");
  
  display.display();    
  delay(100);
  }
}

