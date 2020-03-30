
#include <SPI.h>
#include <WiFi101.h>
#include <WiFiUdp.h>
#include <RTCZero.h>
#include <ArduinoJson.h>
#include <LiquidCrystal.h>
#include "arduino_secrets.h"

// Your network SSID (name)
char ssid[] = SECRET_SSID;
// Your network password (use for WPA, or use as key for WEP)
char pass[] = SECRET_PASS;
// OpenWeatherApi endpoint plus token 
char endpoint[] = ENDPOINT;
char token[] = TOKEN;
char host[] = HOST;
int keyIndex = 0;

// rs: the number of the Arduino pin that is connected to the RS pin on the LCD
// enable: the number of the Arduino pin that is connected to the enable pin on the LCD/
// d0, d1, d2, d3, d4, d5, d6, d7: the numbers of the Arduino pins that are connected to the corresponding data pins on the LCD
const int rs = 13, en = 12, d9 = 9, d8 = 8, d7 = 7, d6 = 6;
int status = WL_IDLE_STATUS;
// GMT+2 Madrid
const int GMT = 2;

WiFiClient client;
LiquidCrystal lcd(rs, en, d9, d8, d7, d6); 
RTCZero rtc;

void setup() {
  Serial.begin(115200);

  // check if the WiFi module works
  if (WiFi.status() == WL_NO_SHIELD) {
    lcd.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

  // attempt to connect to WiFi network:
  while ( status != WL_CONNECTED) {
    lcd.print("Attempting to connect to SSID: ");
    lcd.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(2000);
  }

  // you're connected now, so print out the status:
  printWiFiStatus();

  rtc.begin();

  unsigned long epoch;
  int numberOfTries = 0, maxTries = 6;
  do {
    epoch = WiFi.getTime();
    numberOfTries++;
  }
  while ((epoch == 0) && (numberOfTries < maxTries));

  if (numberOfTries == maxTries) {
    lcd.print("NTP unreachable!!");
    while (1);
  }
  else {
    lcd.print("Epoch received: ");
    lcd.print(epoch);
    rtc.setEpoch(epoch);

    lcd.println();
  }

  lcd.begin(16, 2);
}

void loop() {
  lcd.clear();
  printDateOnLCD();
  printWeatherOnLCD();
  delay(60000);
}

void printDateOnLCD()
{
  lcd.print(rtc.getDay());
  lcd.print("/");
  lcd.print(rtc.getMonth());
  lcd.print("/");
  lcd.print(rtc.getYear());
  lcd.print(" ");
  lcd.print(rtc.getHours() + GMT);
  lcd.print(":");
  lcd.print(rtc.getMinutes());
}

void printWeatherOnLCD() {
  requestOpenWeatherApi();
}
void requestOpenWeatherApi() {
 if (client.connect(HOST, 80)) {
    Serial.println("connected to server");
    // Make a HTTP request:
    client.println((String)"GET " + endpoint + token + " HTTP/1.1");
    client.println((String)"Host: " + host);
    client.println();
    getResponse();
    client.stop();
  }
}

void getResponse() {
  // Wait for a response from the server indicating availability,
  // and then collect the response and build it into a string.
  String response;
  String jsonResponse;
  long startTime = millis();
  delay( 200 );
  while ( client.available() < 1 && (( millis() - startTime ) < 5000 ) ){
        delay( 5 );
  }
  
  if( client.available() > 0 ){ // Get response from server
     char charIn;
     do {
         // Read a char from the buffer.
         charIn = client.read();
         // Append the char to the string response.
         response += charIn;     
        } while ( client.available() > 0 );
    }
  printJSON(getJSONString(response, "{"));
}

void printJSON(String json) {
  const size_t capacity = JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(1) + 2*JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(4) + JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(6) + JSON_OBJECT_SIZE(13) + 290;
  // JsonDocument that allocates its memory pool in the heap.
  DynamicJsonDocument doc(capacity);
  // Parses a JSON input and puts the result in a JsonDocument.
  deserializeJson(doc, json);
  JsonObject main = doc["main"];
  double mainTemp = main["temp"];
  double tempMin = main["temp_min"];// 283.59
  double tempMax = main["temp_max"];// 283.59
  //Position the LCD cursor.
  lcd.setCursor(0, 1);
  lcd.print(String(tempMin) + "-" + String(mainTemp) + "-" + String(tempMax)+ "");
}

String getJSONString(String str, String start) {
  int letterStart = str.indexOf(start);
  if (letterStart==-1) return "";
  letterStart += start.length() -1;
  return str.substring(letterStart, str.length() - 1);
}

void printWiFiStatus() {
  // print the SSID of the network you're attached to:
  lcd.print("SSID: ");
  lcd.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  lcd.print("IP Address: ");
  lcd.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  lcd.print("signal strength (RSSI):");
  lcd.print(rssi);
  lcd.println(" dBm");
}

void print2digits(int number) {
  if (number < 10) {
    lcd.print("0");
  }
  lcd.print(number);
}
