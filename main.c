#include <WiFi.h>
#include <WiFiUdp.h>
#include <ArtnetWifi.h>
#include <FastLED.h>

const char* ssid = "toob-";
const char* password = "";

const int numLeds = 120; 
const int numberOfChannels = numLeds * 3;
#define DATA_PIN 12 
CRGB leds[numLeds];

ArtnetWifi artnet;
const int startUniverse = 0;

bool sendFrame = 1;
int previousDataLength = 0;

boolean ConnectWifi(void)
{
  boolean state = true;
  int i = 0;

  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.println("Connecting to WiFi");

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (i > 50){
      state = false;
      break;
    }
    i++;
  }
  if (state){
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    leds[1] = CRGB(0,255,0);
    FastLED.show();
  } else {
    Serial.println("");
    Serial.println("Connection failed.");
    leds[1] = CRGB(255,0,0);
    FastLED.show();
  }

  return state;
}

void onDmxFrame(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t* data)
{
  sendFrame = 1;

  if (universe == 15)
  {
    FastLED.setBrightness(data[0]);
  }

  for (int i = 0; i < length / 3; i++)
  {
    int led = i + (universe - startUniverse) * (previousDataLength / 3);
    if (led < numLeds)
    {
      leds[led] = CRGB(data[i * 3], data[i * 3 + 1], data[i * 3 + 2]);
    }
  }
  previousDataLength = length;     
  FastLED.show();
}

void setup()
{
  Serial.begin(115200);
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, numLeds);
  Serial.println("ELLISNET ARTNET NODE V0.0");
  leds[0] = CRGB(128,128,128);
  FastLED.show();
  ConnectWifi();
  artnet.begin();
  //FastLED.setBrightness(128);
  leds[0] = CRGB(128,128,128);
  FastLED.show();
  artnet.setArtDmxCallback(onDmxFrame);
}

void loop()
{
  artnet.read();
}