/*
  WS2812B ArtNet Firmware for ESP32
  © Riley Ellis, ELLISNET 2025
  https://artnet.ellisnet.me/
*/

#include <ArtnetWifi.h>
#include <Arduino.h>
#include <FastLED.h>

#define LED_TYPE WS2812B;

#define DEBUG_LOG true;

// Wifi settings
const char* ssid = "artnet";
const char* password = "qycc4242";

// LED settings
const int numLeds = 300; 
const int numberOfChannels = numLeds * 3; 
const byte dataPin = 12;
CRGB leds[numLeds];


ArtnetWifi artnet;
const int startUniverse = 0; 

const int maxUniverses = numberOfChannels / 510 + ((numberOfChannels % 510) ? 1 : 0);
bool universesReceived[maxUniverses];
bool sendFrame = 1;

bool ConnectWifi(void)
{
  bool state = true;
  int i = 0;

  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.println("Connecting to WiFi");

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
    if (i > 20)
    {
      state = false;
      break;
    }
    i++;
  }
  if (state)
  {
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
  else
  {
    Serial.println("");
    Serial.println("Connection failed.");
    Serial.println(Wifi.status());
  }

  return state;
}

void initTest()
{
  for (int i = 0 ; i < 36 ; i++)
  {
    leds[i] = CRGB(127, 0, 0);
  }
  FastLED.show();
  delay(500);
  for (int i = 0 ; i < 36 ; i++)
  {
    leds[i] = CRGB(0, 127, 0);
  }
  FastLED.show();
  delay(500);
  for (int i = 0 ; i < 36 ; i++)
  {
    leds[i] = CRGB(0, 0, 127);
  }
  FastLED.show();
  delay(500);
  for (int i = 0 ; i < numLeds ; i++)
  {
    leds[i] = CRGB(0, 0, 0);
  }
  FastLED.show();
}

void onDmxFrame(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t* data)
{
  sendFrame = 1;
  if (universe == 15)
  {
    FastLED.setBrightness(data[0]);
    FastLED.show();
  }

  // range check
  if (universe < startUniverse)
  {
    return;
  }
  uint8_t index = universe - startUniverse;
  if (index >= maxUniverses)
  {
    return;
  }

  universesReceived[index] = true;

  for (int i = 0 ; i < maxUniverses ; i++)
  {
    if (!universesReceived[i])
    {
      sendFrame = 0;
      break;
    }
  }

  for (int i = 0; i < length / 3; i++)
  {
    int led = i + (index * 170);
    if (led < numLeds)
    {
      for (int x=0; x<8; x++) {
        for (int g=38*x; g<38*(x+1); g++) {
          leds[g] = CRGB(data[i*3], data[i*3+1], data[i*3+2]);
        }
      }// I knew there was an easier way
      
    }
    Serial.println(i);
  }
  
  FastLED.show();
  if (DEBUG_LOG) {
    bool tail = false;
    Serial.print("DMX: Univ: ");
    Serial.print(universe, DEC);
    Serial.print(", Seq: ");
    Serial.print(sequence, DEC);
    Serial.print(", Data (");
    Serial.print(length, DEC);
    Serial.print("): ");
    
    if (length > 16) {
      length = 16;
      tail = true;
    }
    // send out the buffer
    for (uint16_t i = 0; i < length; i++)
    {
      Serial.print(data[i], HEX);
      Serial.print(" ");
    }
    if (tail) {
      Serial.print("...");
    }
    Serial.println();
  }
  if (sendFrame)
  {
    memset(universesReceived, 0, maxUniverses);
  }
}

void setup()
{
  Serial.begin(115200);
  delay(2000);
  ConnectWifi();
  artnet.begin();
  FastLED.addLeds<WS2812B, dataPin, GRB>(leds, numLeds);
  initTest();

  memset(universesReceived, 0, maxUniverses);
  artnet.setArtDmxCallback(onDmxFrame);
}

void loop()
{
  artnet.read();
}