#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

#include <Adafruit_GFX.h>    
#include <Adafruit_SSD1306.h>
#include <Adafruit_BME280.h>

#define BMP_SCK  (13)
#define BMP_MISO (12)
#define BMP_MOSI (11)
#define BMP_CS   (10)

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET    -1 // Reset pin # (or -1 if sharing Arduino reset pin)

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme; // I2C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); //Declaring the display name (display)

/* WIFI SETTINGS */
const char *ssid     = "SSID";
const char *password = "PASSOWRD";
const char* host     = "script.google.com"; 
String url;

/* Allows the OLED to be updated faster than google */
int delay_counter = 60;

void setup() 
{
  // Start serial communication for debugging purposes
  Serial.begin(9600);

  /* Setting up WIFI */
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password); 
  WiFi.mode(WIFI_STA);        //This line hides the viewing of ESP as wifi hotspot
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  // Serial.println(WiFi.localIP());

  /* START BME SENSOR */
  if (!bme.begin(0x76)) {
    Serial.println(F("Could not find a valid BME280 sensor, check wiring!"));
    while (1);
  }
  bme.setSampling(Adafruit_BME280::MODE_FORCED, 
                    Adafruit_BME280::SAMPLING_X1, // temperature 
                    Adafruit_BME280::SAMPLING_X1, // pressure 
                    Adafruit_BME280::SAMPLING_X1, // humidity 
                    Adafruit_BME280::FILTER_OFF   ); 
  
  /* OLED SETUP */
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); //Start the OLED display
  display.setRotation(2);
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(2); 
  display.print("Starting");  
  display.display();    
  delay(1000);
}

void loop() 
{
  bme.takeForcedMeasurement(); // has no effect in normal mode
  display.clearDisplay();     // clear OLED screen
  
  float t = bme.readTemperature();

  /* Printing the current Temperature */
  display.setCursor(0,0);                   
  display.setTextSize(2); 
  display.print("TEMP");
  
  display.setCursor(0,18);
  display.print(t,1);
  display.setCursor(50,17);
  display.setTextSize(1);
  display.print("C");

  /* SENDING DATA TO GOOGLE SCRIPT EVERY 60 SECONDS*/
  if (delay_counter == 60)
  {
    SendData();
  }

  /* UPDATE DISPLAY */
  delay_counter += 1;
  display.display();
  delay(1000);        // update OLED every 1 second
}

// Send data to a google script using a GET request
// which then updates a Google Sheet
void SendData()
{
  delay_counter = 0;
  
  Serial.print("connecting to: ");
  Serial.println(host);
  
  WiFiClientSecure client;
  client.setInsecure();
  
  const int httpsPort = 443;
  if (!client.connect(host, httpsPort)) 
  {
    Serial.println("Connection failed");
    return;
  }
  
  url = "/macros/s/--GOOGLE_SCRIPT_URL--/exec?";
  url += "ID=";   
  url += String("BME280"); // Name of google "Sheet" for the data to be sent to, must match     
  url += "&TMP=";  
  url += String(bme.readTemperature());     
                          
  Serial.print("Requesting URL: ");
  Serial.println(url);

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  delay(500);
}
