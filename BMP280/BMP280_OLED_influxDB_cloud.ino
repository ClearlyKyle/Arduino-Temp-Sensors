
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_BMP280.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET    -1 // Reset pin # (or -1 if sharing Arduino reset pin)

Adafruit_BMP280 bmp; // I2C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); //Declaring the display name (display)

/* WIFI SETTINGS */
const char *ssid     = "SSID";
const char *password = "PASSOWRD";

/* INFLUXDB SETTINGS */
String INFLUXDB_URL = "eu-central-1-1.aws.cloud2.influxdata.com";
String TOKEN = "TOKEN";
String BUCKET = "BUCKET NAME";
String ORG = "your_email@mail.co.uk";

String Link =  "/api/v2/write?org=" + ORG + "&bucket=" + BUCKET;

const int httpsPort = 443; //HTTPS= 443 and HTTP = 80
const char *host = "eu-central-1-1.aws.cloud2.influxdata.com/";

int delay_counter = 60;

void setup() {
  // Start serial communication for debugging purposes
  Serial.begin(9600);
  
  /* Setting up WIFI */
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD); 
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

  /* BMP SETUP */
  if (!bmp.begin(0x76)) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
    while (1);
  }
  
  /* OLED SETUP */
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); //Start the OLED display
  display.setRotation(2);
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(2); 
  display.print("Starting");  
  display.display();    
  delay(1000);;
}

void loop()
{
  display.clearDisplay();
  
  float T = bmp.readTemperature();              //Temperature in °C
  // float P = bmp.readPressure();              //Pressure in Pa
  // float A = bmp.readAltitude(1010);          //Calculating the Altitude, the "1010" is the pressure in (hPa) at sea level
                                              
  /* display.setCursor(X, Y); */
  /* Printing the current Temperature */
  display.setCursor(0,0);                   
  display.setTextSize(2); 
  display.print("TEMP");
  
  display.setCursor(0,18);
  display.print(T,1);
  display.setCursor(50,17);
  display.setTextSize(1);
  display.print("C");

  /* SENDING DATA TO INFLUXDB SCRIPT EVERY 20 SECONDS*/
  if (delay_counter == 60)
  {
    SendData();
  }

  /* UPDATE DISPLAY */
  delay_counter += 1;
  display.display();
  delay(1000);        // update OLED every 1 second
}

void SendData()
{
  delay_counter = 0;
  
  Serial.print("Connecting to: ");
  Serial.println(INFLUXDB_URL);
  
  WiFiClientSecure client;
  client.setInsecure();
  
  const int httpsPort = 443;
  if (!client.connect(INFLUXDB_URL, httpsPort)) 
  {
    Serial.println("Connection failed");
    return;
  }

  String data = "ESP2-BMP-OLED temperature=" + String(bmp.readTemperature());
  
  Serial.print("Requesting URL: ");
  Serial.println(data);
  
  client.print(String("POST ") + Link + " HTTP/1.1\r\n" +
               "Host: eu-central-1-1.aws.cloud2.influxdata.com" + "\r\n" +
               "Authorization: Token " + TOKEN + "\r\n" +
               "Connection: close\r\n" +
               "Content-Type: text/plain\r\n" + 
               "Content-Length: " + data.length() + "\r\n" +
               "\r\n" +
               data + "\r\n");
               
  Serial.println();
  delay(500);
}
