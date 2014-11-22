#include <SoftwareSerial.h>
#include <Adafruit_GPS.h>
#include <GPRSInterface.h>
#include <WaveRP.h>
#include <SdFat.h>
#include <SdFatUtil.h>
#include <aJSON.h>
#define PIN_TX 1
#define PIN_RX 0

SoftwareSerial softSerial(3, 2); // RX, TX
Adafruit_GPS GPS(&softSerial);
Sd2Card card;
SdFile root;
SdFile file;
WaveRP wave;
String json_example;
aJsonObject* json = aJson.parse(json_example);

void setup() {
  GPRSInterface gprs(PIN_TX,PIN_RX,19200,"cmnet",NULL,NULL);
  gprs.init();
  
  GPS.begin(9600);
}

void loop() {
}

