#include <Adafruit_GFX.h> 
#include <Adafruit_SSD1306.h>
#include <Adafruit_BMP280.h>   

#define BMP_SCK  (13)
#define BMP_MISO (12)
#define BMP_MOSI (11)
#define BMP_CS   (10)

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET    -1 // Reset pin # (or -1 if sharing Arduino reset pin)

Adafruit_BMP280 bmp; // I2C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); //Declaring the display name (display)

void setup() 
{
  // Start serial communication for debugging purposes
  Serial.begin(9600);

  /* START BMP SENSOR */
  if (!bmp.begin(0x76)) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
    while (1);
  }

  /* OLED SETUP */
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); //Start the OLED display
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(2); 
  display.print("Starting");  
  display.display();    
  delay(1000);
}

void loop() 
{
  display.clearDisplay();     // clear OLED screen
  
  float t = bmp.readTemperature();

  /* Printing the current Temperature */
  display.setCursor(0,0);                   
  display.setTextSize(2); 
  display.print("TEMP");
  
  display.setCursor(0,18);
  display.print(t,1);
  display.setCursor(50,17);
  display.setTextSize(1);
  display.print("C");

  /* UPDATE DISPLAY */
  display.display();
  delay(1000);
}
