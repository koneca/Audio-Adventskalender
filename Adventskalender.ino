/*
 Example: Control a WTV020-SD-16P module to play voices from an Arduino board.
 Created by ELECTRONOOBS, oct 14, 2016.
 */
 
#include <Wtv020sd16p.h>
#include <Wire.h>
#include "RTClib.h"

int resetPin = 4;  // The pin number of the reset pin.
int clockPin = 5;  // The pin number of the clock pin.
int dataPin = 6;  // The pin number of the data pin.
int busyPin = 7;  // The pin number of the busy pin.

/*
Create an instance of the Wtv020sd16p class.
 1st parameter: Reset pin number.
 2nd parameter: Clock pin number.
 3rd parameter: Data pin number.
 4th parameter: Busy pin number.
 */
Wtv020sd16p Player(resetPin,clockPin,dataPin,busyPin);
RTC_DS1307 RTC;
int Played;

void setup() {
  //Initializes the module.
  Player.reset();
  RTC.begin();
  Serial.begin(9600);
  
  Serial.println("Starting Adventskalender");
  Player.setVolume(4);
  Played = 0;
  //Player.mute();  

  /*if (! RTC.isrunning())
  {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }*/
}

void PlayTrack(unsigned int Track)
{
  if(! Player.isPlaying() && ! Played)
  {
    //Plays synchronously an audio file. Busy pin is used for this method.
    Serial.println("playing!");
    Player.asyncPlayVoice(Track);
    Player.setVolume(4);
    Played = 1;
  }
  else
  {
    Serial.println("Player is busy");
  }
}

void loop() {
  DateTime now = RTC.now();
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(' ');
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();

  if (now.day() < 25 && now.month() == 12)
  {
    PlayTrack(now.day());
  }
  else
  {
    Serial.println("Start playing default track");
    PlayTrack(25);
  }
  delay(2000);
}