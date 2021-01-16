#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

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

/* WIFI SETTINGS */
const char *ssid     = "SSID";
const char *password = "PASSOWRD";
const char* host     = "script.google.com"; 
String url;

/* Allows the OLED to be updated faster than google */
int delay_counter = 60;

void setup(void)
{
  // Start serial communication for debugging purposes
  Serial.begin(9600);
  
  // Start up the library
  sensors.begin();

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

  /* SENDING DATA TO GOOGLE SCRIPT EVERY 60 SECONDS*/
  if (delay_counter == 60)
  {
    SendData();
  }
    
  /* SERIAL OUTPUT */
  Serial.println(sensors.getTempCByIndex(0));

  /* UPDATE DISPLAY */
  delay_counter += 1;
  display.display();
  delay(1000);
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
    Serial.println("connection failed");
    return;
  }
  
  url = "/macros/s/--GOOGLE_SCRIPT_URL--/exec?";
  url += "ID=";   
  url += String("DS18B20"); // Name of google "Sheet" for the data to be sent to, must match     
  url += "&TMP=";  
  url += String(sensors.getTempCByIndex(0));     
                          
  Serial.print("Requesting URL: ");
  Serial.println(url);

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  delay(500);
}
