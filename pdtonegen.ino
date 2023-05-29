#include <MD_AD9833.h>
#include <MD_AD9833_lib.h>

// Pins for SPI communication with the AD9833 IC
#define FSYNC D8
#define SDATA D7
#define SCLK  D5

// Pins to select channel on 74HC4051 demultiplexer
#define CHAN1 D1
#define CHAN2 D2
#define CHAN4 D3

// Initialise AD9833 device data
MD_AD9833	AD(SDATA, SCLK, FSYNC);  // Hardware SPI


/*
 * Configuration parameters - Tass algorithm, fixed basic values
 */
#define CARRIER 250.0   // Base tone frequency, 250Hz
#define BARFREQ 1.5     // Frequency base, 1.5Hz
#define TONELEN 100     // Length of tone, 0.1s = 100ms
#define TONEBARS  3     // bars of tones in a phrase
#define RESTBARS  2     // bars of silence between phrases
#define NUMFINGERS 4    // number of fingers = number of channels in output file

/*
 * Derived values
 */
float jitterPct = 0.0;                              // 0..0.235
static const float tBarLength = 1.0/BARFREQ;        // bar length in seconds
static const float tNoteLength = tBarLength/4.0;    // tick length in seconds - not sure what happens if nFingers != 4

static const float jMax = jitterPct * tNoteLength;
                        // maximum jitter, difference between earliest and latest

// 24 possible sequences of 4 fingers
const uint8_t sequences[24][4] = {
    {1,2,3,4},{1,2,4,3},{1,3,2,4},{1,3,4,2},
    {1,4,2,3},{1,4,3,2},{2,1,3,4},{2,1,4,3},
    {2,3,1,4},{2,3,4,1},{2,4,1,3},{2,4,3,1},
    {3,1,2,4},{3,1,4,2},{3,2,1,4},{3,2,4,1},
    {3,4,1,2},{3,4,2,1},{4,1,2,3},{4,1,3,2},
    {4,2,1,3},{4,2,3,1},{4,3,1,2},{4,3,2,1}};

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
  AD.setMode(MD_AD9833::MODE_SINE);
  delay(TONELEN);
  AD.setMode(MD_AD9833::MODE_OFF);
}

// play a phrase consisting of 3 bars of tones followed by 2 bars of silence
void doPhrase() {
  uint8_t seq[TONEBARS*NUMFINGERS];
  int jit_ms[TONEBARS*NUMFINGERS];
  int s=0;
  int lastRandom = -1;

  // collect our random sequences for the bars in this phrase
  for(int i=0; i<TONEBARS; i++) {
    int jRandom;
    // random() can repeat values on successive calls if done too quickly
    for(;;) {
      jRandom = random(24);
      if(jRandom != lastRandom)
        break;
    }
    lastRandom = jRandom;

    Serial.printf("Bar %i using pattern %i\n", i, jRandom);
    const uint8_t *barseq; barseq = sequences[jRandom];
    for(int f=0; f<NUMFINGERS; f++) {
      seq[s] = barseq[f];
      jit_ms[s] = random(-20, 20);
      s++;
    }

    // phrase start
    long pStart = millis();
    long baseTime = 0;
    for(int i=0; i<TONEBARS*NUMFINGERS; i++) {
      long now = millis();
// next note starts at now+baseTime+jitter (can be negative)
      long nextStart = now + baseTime + jit_ms[i];

      baseTime += 166;
    }
  }


  delay(1000);
  if(++chan > 3) chan = 0;
  doTone(chan);
  delay(66);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Hello world");
  // initialise AD9833 tone generator
  AD.begin();
  setMode(1);

  // initialise 74HC4051 demultiplexer
  pinMode(CHAN1, OUTPUT);
  pinMode(CHAN2, OUTPUT);
  pinMode(CHAN4, OUTPUT);

  Serial.printf("AD9833 set frequency to %i : ", CARRIER);
  if(AD.setFrequency(MD_AD9833::CHAN_0, CARRIER)) {
    Serial.println("OK");
  } else {
    Serial.println("FAIL");
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  // Serial.println("Hello from the loop");
  doPhrase();
}
