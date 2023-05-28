#include <MD_AD9833.h>
#include <MD_AD9833_lib.h>

// Pins for SPI comm with the AD9833 IC
#define FSYNC D8
#define SDATA D7
#define SCLK  D5
//SDATA 13
//SCLK  14

// Pins to select channel on 74HC4051
#define CHAN1 D1
#define CHAN2 D2
#define CHAN4 D3

#define TONELENGTH  100

MD_AD9833	AD(SDATA, SCLK, FSYNC);  // Hardware SPI

#define BASEFREQ 250.0
int chan = 0;

void setMode(int mode) {
	switch(mode) {
		case 0: AD.setMode(MD_AD9833::MODE_OFF); break;
		case 1: AD.setMode(MD_AD9833::MODE_SINE); break;
		case 2: AD.setMode(MD_AD9833::MODE_SQUARE1); break;
		case 3: AD.setMode(MD_AD9833::MODE_SQUARE2); break;
		case 4: AD.setMode(MD_AD9833::MODE_TRIANGLE); break;
	}
}

void setChannel(int c) {
  digitalWrite(CHAN1, (c&1)?HIGH:LOW);
  digitalWrite(CHAN2, (c&2)?HIGH:LOW);
  digitalWrite(CHAN4, (c&4)?HIGH:LOW);
}

void doTone(int chan) {
  setChannel(chan);
  Serial.printf("Mux channel set to %i\n", chan);
  setMode(1);
  delay(TONELENGTH);
  setMode(0);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Hello world");
  AD.begin();
  setMode(1);
  pinMode(CHAN1, OUTPUT);
  pinMode(CHAN2, OUTPUT);
  pinMode(CHAN4, OUTPUT);
  Serial.printf("AD9833 set frequency to %i : ", BASEFREQ);
  if(AD.setFrequency(MD_AD9833::CHAN_0, BASEFREQ)) {
    Serial.println("OK");
  } else {
    Serial.println("FAIL");
  }

//  pinMode(BUZZER, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  // Serial.println("Hello from the loop");
  delay(1000);
  if(++chan > 3) chan = 0;
  doTone(chan);
  delay(66);
}
