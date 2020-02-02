#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
// change these to hotspot creds
const char *host = "https://webhook.site/e9c3b85b-e4b0-4d38-adf8-d49d6402844e";
//const char host = "http://52.116.36.178:800";
const char *ssid = "iPotato 11 Pro Max";
const char *password = "nice try";
void sendPulse();
void sendSteps();
const unsigned int millis_per_min = 30 * 1000;
unsigned long last_time = 0;
unsigned int counter = 0;

#include <Adafruit_NeoPixel.h>
// Which pin on the Arduino is connected to the NeoPixels?
#define PIN        14
// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 12 // Popular NeoPixel ring size
// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 50 // Time (in milliseconds) to pause between pixels
#define MAX_BRIGHTNESS 20
int brightness = 0;

void blinkRingRed() {
  pixels.clear();
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(MAX_BRIGHTNESS, 0, 0));
  }
  pixels.show();
  delay(100);
  pixels.clear();
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 0, 0));
  }
  pixels.show();
  delay(100);
  pixels.clear();
}



void pulseRing() {
  pixels.clear();
  while (brightness < MAX_BRIGHTNESS) {
    for (int i = 0; i < NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(0, brightness, 0));
    }
    delay(5);
    pixels.show();
    brightness++;
  }
  while (brightness >= 0) {
    for (int i = 0; i < NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(0, brightness, 0));
    }
    delay(5);
    pixels.show();
    brightness--;
  }
  brightness = 0;
  pixels.clear();
  delay(10);
}

void setup() {
  pixels.begin();
  WiFi.begin(ssid, password);   //WiFi connection
  Serial.begin(115200);
  Serial.println(WiFi.macAddress());
  while (WiFi.status() != WL_CONNECTED) {  //Wait for the WiFI connection completion
    delay(300);
    Serial.println("Waiting for connection");
    Serial.println(WiFi.macAddress());
    blinkRingRed();
  }
  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
  pixels.clear();
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 0, MAX_BRIGHTNESS));
  }
  pixels.show();
  delay(100);
  last_time = millis();
}
void loop() {
  unsigned long elapsed = millis() - last_time;
  if (elapsed >= millis_per_min && counter % 2 == 0) {
    last_time = millis();
    sendPulse();
    pulseRing();
    counter++;
  }
  else if (elapsed >= millis_per_min && counter % 2 == 1) {
    last_time = millis();
    sendSteps();
    counter++;
    pulseRing();
  }

}
void sendPulse()
{
  Serial.println("Sending pulse rate...");
  HTTPClient http;    //Declare object of class HTTPClient
  http.begin("http://52.116.36.178:8001/postHeartrate");
  http.addHeader("Content-Type", "application/json");
  int httpCode = http.POST("{\"user\":\"woodle\",\"hrate\":150}");
  String payload = http.getString();                  //Get the response payload
  Serial.println(httpCode);   //Print HTTP return code
  Serial.println(payload);    //Print request response payload
  http.end();  //Close connection
}
void sendSteps()
{
  Serial.println("Sending step count...");
  HTTPClient http;    //Declare object of class HTTPClient
  http.begin("http://52.116.36.178:8001/postSteps");
  http.addHeader("Content-Type", "application/json");
  int httpCode = http.POST("{\"user\":\"woodle\",\"steps\":11}");
  String payload = http.getString();                  //Get the response payload
  Serial.println(httpCode);   //Print HTTP return code
  Serial.println(payload);    //Print request response payload
  http.end();  //Close connection
}
