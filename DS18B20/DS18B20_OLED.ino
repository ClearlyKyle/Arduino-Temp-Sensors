#include <DallasTemperature.h>

#include <Adafruit_GFX.h>      
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET    -1 // Reset pin # (or -1 if sharing Arduino reset pin)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); //Declaring the display name (display)

// Data wire is conntec to the Arduino digital pin 4
#define ONE_WIRE_BUS 14

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);

void setup(void)
{
  // Start serial communication for debugging purposes
  Serial.begin(9600);
  
  // Start up the library
  sensors.begin();
  
  /* OLED SETUP */
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); //Start the OLED display
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(2); 
  display.print("Starting");  
  display.display();    
  delay(1000);
}

void loop(void){ 
  // Call sensors.requestTemperatures() to issue a global temperature and Requests to all devices on the bus
  sensors.requestTemperatures(); 
  
  display.clearDisplay();
  
  /* Printing the current Temperature to OLED */
  display.setCursor(0,0);                   
  display.setTextSize(2); 
  display.print("DSP");
  
  display.setCursor(0,18);
  display.print(sensors.getTempCByIndex(0),1);
  display.setCursor(50,17);
  display.setTextSize(1);
  display.print("C");

  /* UPDATE DISPLAY */
  display.display();
  delay(1000);
}
