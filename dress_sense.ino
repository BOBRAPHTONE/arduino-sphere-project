#include <SoftwareSerial.h>
#include <Adafruit_GPS.h>
#include <WaveHC.h>
#include <WaveUtil.h>
#include <FatReader.h>
#include <SdReader.h>
#include <avr/pgmspace.h>
#include "types.h"


// DEFINES
#define PIN_TX 1
#define PIN_RX 0
#define GPSECHO false
//#define DEBUG

// GLOBAL VARS
SoftwareSerial softSerial(9, 8); // RX, TX
Adafruit_GPS GPS(&softSerial);
Location location;

Location route[] = {
  // Outside Queens
  {51.4570, -2.6019, "0.WAV"},
  // Road, no crossing, go right
  {51.4582, -2.6036, "1.WAV"},
  // Straight down main road to zebra crossing
  // then cross whilst looking right and left
  {51.4586, -2.6037, "2.WAV"},
  // zebra crossing
  {51.4589, -2.6038, "3.WAV"},
  // cross over road, then you've reached the terrace bat for a coffee
  // with friends
  {51.4585, -2.6058, "4.WAV"},
  // Come out of terrace bar and turn right
  {51.4582, -2.6069, "5.WAV"},
  // Cross the drop kerb and turn left
  {51.45772, -2.60824, "6.WAV"},
  // Cross road
  {51.45662, -2.60702, "7.WAV"},
  // Barclays banks
  {51.45628, -2.60624, "8.WAV"},
  // Cross over road
  {51.45611, -2.60522, "9.WAV"},
  // Walk past the Bristol museum
  {51.45611, -2.60522, "10.WAV"},
  // Traffic lights on your right, keep going past them
  {51.45587, -2.60428, "11.WAV"},
  // You should spot a post box on the corner, turn left
  {51.45538, -2.60181, "12.WAV"}
};

SdReader card;    // This object holds the information for the card
FatVolume vol;    // This holds the information for the partition on the card
FatReader root;   // This holds the information for the filesystem on the card
FatReader f;      // This holds the information for the file we're play

WaveHC wave;      // This is the only wave (audio) object, since we will only play one at a time


Location gpsToLocation(Adafruit_GPS gps) {
  Location l; 
  l.lat = gps.latitudeDegrees;
  l.lon = gps.longitudeDegrees;
  return l;
}

boolean nearLocation(Location one, Location two) {
  // Set delta using the potentiometer
  float delta = 0.0000005 * analogRead(A0);
#ifdef DEBUG
  serial.println(delta, 6);
#endif
  float latDiff = abs(one.lat - two.lat);
  float lonDiff = abs(one.lon - two.lon);
  double diff = sqrt(pow(latDiff, 2) + pow(lonDiff, 2));
#ifdef DEBUG
  Serial.println(diff, 6);
#endif
  return (diff <= delta);
}



void setup() {
  setupInterrupts();
  Serial.begin(9600);
  GPS.begin(9600);
  // Enable recommended minimum and fix data
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  // set update rate
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);

  // Set the output pins for the DAC control. This pins are defined in the library
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
 
  // pin13 LED
  pinMode(13, OUTPUT);

  if (!card.init()) {         //play with 8 MHz spi (default faster!)  
    Serial.println("Card init. failed!");  // Something went wrong, lets print out why
    sdErrorCheck();
    while(1);                            // then 'halt' - do nothing!
  }
  card.partialBlockRead(true);
// Now we will look for a FAT partition!
  uint8_t part;
  for (part = 0; part < 5; part++) {     // we have up to 5 slots to look in
    if (vol.init(card, part)) 
      break;                             // we found one, lets bail
  }
  if (part == 5) {                       // if we ended up not finding one  :(
    Serial.println("No valid FAT partition!");
    sdErrorCheck();      // Something went wrong, lets print out why
    while(1);                            // then 'halt' - do nothing!
  }
  
  // Lets tell the user about what we found
  Serial.print("Using partition ");
  Serial.print(part, DEC);
  Serial.print(", type is FAT");
  Serial.println(vol.fatType(),DEC);     // FAT16 or FAT32?
  
  // Try to open the root directory
  if (!root.openRoot(vol)) {
    Serial.println("Can't open root dir!"); // Something went wrong,
    while(1);                             // then 'halt' - do nothing!
  }
  
  // Whew! We got past the tough parts.
  Serial.println("Ready!");
  delay(1000);
}

// This is run every time Timer2 interrupts
void check_trigger() {
  boolean near = nearLocation(location, route[0]);
  if (near) {
    playfile("0.WAV");
  }
}

uint32_t timer = millis();
void loop() {
  if (GPS.newNMEAreceived()) {
    // we can fail to parse a sentence in which case we should just wait for another if (!GPS.parse(GPS.lastNMEA())) return;
  }

  if (timer > millis()) timer = millis(); 
  location = gpsToLocation(GPS); 
  Serial.print("Location: "); 
  Serial.print(location.lat); 
  Serial.print(","); 
  Serial.println(location.lon);
  boolean near = nearLocation(location, route[0]);
  Serial.print("Is close to first point? ");
  Serial.println(near);
  delay(1000);
}
