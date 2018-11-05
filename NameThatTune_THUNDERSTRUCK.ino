//Name that Tune
//Written by Jordan Rejaud for FUZE

#include <FastLED.h>
#include <Bounce2.h>
#include "melodies.h"

//******************************************************************************
// User modifiable values
//******************************************************************************
//How many different tunes are programmed on the board
#define NUMBER_OF_TUNES 6

//The delay time between each notes in the song
//Delay between notes: Note Duration + 30%
//If a note lasts for 1s, then there will be a 0.30 second delay after the note ends before the next note starts
#define DELAY_BETWEEN_NOTES 0.30

//The color of the tune selection menu
const int TUNE_MENU_SELECTION_COLOR[3] = {29, 37, 86};

//Note Colors

//These are the colors that the LEDs will flash when a particular note is being played
const int A_NOTE_COLOR[3] = {80,0,0}; //Red
const int B_NOTE_COLOR[3] = {255,40,0}; //Orange
const int C_NOTE_COLOR[3] = {130, 80, 0}; //Yellow
const int D_NOTE_COLOR[3] = {0,80,0}; //Green
const int E_NOTE_COLOR[3] = {0,0,80}; //Blue
const int F_NOTE_COLOR[3] = {40, 0, 100}; //Indigo
const int G_NOTE_COLOR[3] = {80,0,120}; //Violet
//******************************************************************************

//LED Initialization (Don't change any of this!)
//https://github.com/FastLED/FastLED/wiki/Basic-usage for more details
#define NUMBER_OF_LEDS 6 //Number of linear LEDs on the board (don't change this)
#define DATA_PIN 16 //Serial Data output (don't change this)
#define CLOCK_PIN 15 //Serial Clock output (don't change this)
CRGB leds[NUMBER_OF_LEDS]; //LED Array (don't change this)

//Buttons, Don't change these
//This sets which pins in your Fuze board are connected to each button
//Since these are constants (they can't be changed), they are written in UPPER CASE
#define BUTTON_ONE_PIN 7
#define BUTTON_TWO_PIN 5
#define BUTTON_THREE_PIN 9

#define BUTTON_TRIANGLE_PIN 10
#define BUTTON_CIRCLE_PIN 2
#define BUTTON_SQUARE_PIN 8

//Buzzer, don't change this
//This sets which pin on your Fuze board is connected to the buzzer
//Since this is a constant (they can't be changed), it's written in UPPER CASE
#define BUZZER_PIN 3

//Photocell, don't change this
//This sets which pin on your Fuze board is connect to the photocell
//Since this is a constant (it can't be changed), it's written in UPPER CASE
#define PHOTO_CELL_PIN A3

//These are the highest (and lowest) analog value that photocell can read
#define MAX_PHOTO_CELL_VALUE 1024
#define MIN_PHOTO_CELL_VALUE 0

//This enables "debouncing" on the buttons
//https://github.com/thomasfredericks/Bounce2
Bounce triangleDebouncer = Bounce();
Bounce circleDebouncer = Bounce();
Bounce squareDebouncer = Bounce();

int selectedTuneIndex;

void setup() {
  // put your setup code here, to run once:
  //Initialize LEDs, don't change this part of the code
  FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN, BGR>(leds, NUMBER_OF_LEDS);
  clearAllLEDs();

  //This tells the Fuze board that these pins are buttons, don't change this part of the code
  pinMode(BUTTON_TRIANGLE_PIN, INPUT_PULLUP);
  pinMode(BUTTON_CIRCLE_PIN, INPUT_PULLUP);
  pinMode(BUTTON_SQUARE_PIN, INPUT_PULLUP);

  //Initialize the button debounce, don't change this part of the code
  #define DEBOUNCE_INTERVAL 40
  triangleDebouncer.attach(BUTTON_TRIANGLE_PIN);
  triangleDebouncer.interval(DEBOUNCE_INTERVAL);
  squareDebouncer.attach(BUTTON_SQUARE_PIN);
  squareDebouncer.interval(DEBOUNCE_INTERVAL);
  circleDebouncer.attach(BUTTON_CIRCLE_PIN);
  circleDebouncer.interval(DEBOUNCE_INTERVAL);

  resetNameThatTune();
}

void loop() {
  // put your main code here, to run repeatedly:

  //Read the latest button states
  triangleDebouncer.update();
  squareDebouncer.update();
  circleDebouncer.update();

  //Check if the user is pushing a button
  if (triangleDebouncer.fell()) {
    //Increment the selected tune index
    selectedTuneIndex = incrementIndex(selectedTuneIndex, NUMBER_OF_TUNES - 1);
    updateLEDMenuBasedOnSelectedTuneIndex(selectedTuneIndex);
  } else if (squareDebouncer.fell()) {
    //Decrement the selected tune index
    selectedTuneIndex = decrementIndex(selectedTuneIndex, NUMBER_OF_TUNES - 1);
    updateLEDMenuBasedOnSelectedTuneIndex(selectedTuneIndex);
  } else if (circleDebouncer.fell()) {
    //Start the selected tune
    startTune(selectedTuneIndex);
  }
}

void resetNameThatTune() {
  noTone(BUZZER_PIN);
  selectedTuneIndex = 0;
  updateLEDMenuBasedOnSelectedTuneIndex(selectedTuneIndex);
}

void updateLEDMenuBasedOnSelectedTuneIndex(int tuneIndex) {
  clearAllLEDs();
  for (int i=0;i<=tuneIndex;i++) {
      leds[i].setRGB(TUNE_MENU_SELECTION_COLOR[0], TUNE_MENU_SELECTION_COLOR[1], TUNE_MENU_SELECTION_COLOR[2]);
  }
  FastLED.show();
}

void startTune(int tuneIndex) {
  if (tuneIndex == 0) {
    //Fur Elise
    startPlayingTune(FUR_ELISE_BPM, FUR_ELISE_NOTE_COUNT, FUR_ELISE_MELODY, FUR_ELISE_NOTE_DURATION);
  } else if (tuneIndex == 1) {
    //Twinkle Twinkle little star
    startPlayingTune(TWINKLE_TWINKLE_LITTLE_STAR_BPM, TWINKLE_TWINKLE_LITTLE_STAR_NOTE_COUNT, TWINKLE_TWINKLE_LITTLE_STAR_MELODY, TWINKLE_TWINKLE_LITTLE_STAR_NOTE_DURATION);
  } else if (tuneIndex == 2) {
    //Twinkle Twinkle little star (Kristy's variation)
    startPlayingTune(TWINKLE_TWINKLE_LITTLE_STAR_ZUBI_STYLE_BPM, TWINKLE_TWINKLE_LITTLE_STAR_ZUBI_STYLE_NOTE_COUNT, TWINKLE_TWINKLE_LITTLE_STAR_ZUBI_STYLE_MELODY, TWINKLE_TWINKLE_LITTLE_STAR_ZUBI_STYLE_NOTE_DURATION);
  } else if (tuneIndex == 3) {
    //Flight of the Bumble-bee
    startPlayingTune(FLIGHT_OF_THE_BUMBLE_BEE_BPM, FLIGHT_OF_THE_BUMBLE_BEE_NOTE_COUNT, FLIGHT_OF_THE_BUMBLE_BEE_MELODY, FLIGHT_OF_THE_BUMBLE_BEE_NOTE_DURATION);
  } else if (tuneIndex == 4) {
    //If you're happy
    startPlayingTune(IF_YOUR_HAPPY_BPM, IF_YOUR_HAPPY_NOTE_COUNT, IF_YOUR_HAPPY_MELODY, IF_YOUR_HAPPY_NOTE_DURATION);
  } else if (tuneIndex == 5) {
    //ZubiFlyer Startup Sound
    startPlayingTune(STARTUP_BPM, STARTUP_NOTE_COUNT, STARTUP_MELODY, STARTUP_NOTE_DURATION);
  }
}

void startPlayingTune(int songBPM, int noteCount, const int melody[], const int notesDuration[]) {
  long delayCompletedTimeMillis; //Used to calculate the delay

  //For each note
  for (int noteIndex = 0; noteIndex < noteCount; noteIndex++) {
    //Calculate note duration
    int noteDuration = calculateNoteDurationMS(pgm_read_word_near(notesDuration + noteIndex), songBPM);
    //Play Note
    int note = pgm_read_word_near(melody + noteIndex);
    tone(BUZZER_PIN, note, noteDuration);
    //Turn on LEDs
    int redColor = getPitchRedValue(note);
    int greenColor = getPitchGreenValue(note);
    int blueColor = getPitchBlueValue(note);
    setAllLEDsColor(redColor, greenColor, blueColor);

    delayCompletedTimeMillis = millis() + noteDuration;

    //Wait until the note is done playing
    while(millis()<delayCompletedTimeMillis) {

    }

    //Done playing note
    noTone(BUZZER_PIN);
    clearAllLEDs();

    //Wait for a small time before starting to play the next note
    delayCompletedTimeMillis = millis() + noteDuration*DELAY_BETWEEN_NOTES;

    while(millis()<delayCompletedTimeMillis) {

    }
  }
  updateLEDMenuBasedOnSelectedTuneIndex(selectedTuneIndex);
}


//Calculate how long a note lasts (in Milliseconds) based on quantity/BPM and song BPM
int calculateNoteDurationMS(int quantityPerBPM, int songBPM) {
  return ((float)quantityPerBPM/(float)songBPM * 1000);
}

int getPitchRedValue(int pitch) {
  //B
  if (pitch == NOTE_B0 || pitch == NOTE_B1 || pitch == NOTE_B2 || pitch == NOTE_B3 || pitch == NOTE_B4 || pitch == NOTE_B5 || pitch == NOTE_B6 || pitch == NOTE_B7) {
    //Orange
    return B_NOTE_COLOR[0];
  //C
  } else if (pitch == NOTE_C1 || pitch == NOTE_CS1 || pitch == NOTE_C2 || pitch == NOTE_CS2 || pitch == NOTE_C3 || pitch == NOTE_CS3 || pitch == NOTE_C4 || pitch == NOTE_CS4 || pitch == NOTE_C5 || pitch == NOTE_CS5 || pitch == NOTE_C6 || pitch == NOTE_CS6 || pitch == NOTE_C7 || pitch == NOTE_CS7 || pitch == NOTE_C8 || pitch == NOTE_CS8) {
    //Yellow
    return C_NOTE_COLOR[0];
  //D
  } else if (pitch == NOTE_D1 || pitch == NOTE_DS1 || pitch == NOTE_D2 || pitch == NOTE_DS2 || pitch == NOTE_D3 || pitch == NOTE_DS3 || pitch == NOTE_D4 || pitch == NOTE_DS4 || pitch == NOTE_D5 || pitch == NOTE_DS5 || pitch == NOTE_D6 || pitch == NOTE_DS6 || pitch == NOTE_D7 || pitch == NOTE_DS7 || pitch == NOTE_D8 || pitch == NOTE_DS8) {
    //Green
    return D_NOTE_COLOR[0];
  //E
  } else if (pitch == NOTE_E1 || pitch == NOTE_E2 || pitch == NOTE_E3 || pitch == NOTE_E4 || pitch == NOTE_E5 || pitch == NOTE_E6 || pitch == NOTE_E7) {
    //Blue
    return E_NOTE_COLOR[0];
  //F
  } else if (pitch == NOTE_F1 || pitch == NOTE_FS1 || pitch == NOTE_F2 || pitch == NOTE_FS2 || pitch == NOTE_F3 || pitch == NOTE_FS3 || pitch == NOTE_F4 || pitch == NOTE_FS4 || pitch == NOTE_F5 || pitch == NOTE_FS5 || pitch == NOTE_F6 || pitch == NOTE_FS6 || pitch == NOTE_F7 || pitch == NOTE_FS7) {
    //Indigo
    return F_NOTE_COLOR[0];
  //G
  } else if (pitch == NOTE_G1 || pitch == NOTE_GS1 || pitch == NOTE_G2 || pitch == NOTE_GS2 || pitch == NOTE_G3 || pitch == NOTE_GS3 || pitch == NOTE_G4 || pitch == NOTE_GS4 || pitch == NOTE_G5 || pitch == NOTE_GS5 || pitch == NOTE_G6 || pitch == NOTE_GS6 || pitch == NOTE_G7 || pitch == NOTE_GS7) {
    //Violet
    return G_NOTE_COLOR[0];
  //A
  } else if (pitch == NOTE_A1 || pitch == NOTE_AS1 || pitch == NOTE_A2 || pitch == NOTE_AS2 || pitch == NOTE_A3 || pitch == NOTE_AS3 || pitch == NOTE_A4 || pitch == NOTE_AS4 || pitch == NOTE_A5 || pitch == NOTE_AS5 || pitch == NOTE_A6 || pitch == NOTE_AS6 || pitch == NOTE_A7 || pitch == NOTE_AS7) {
    //Red
    return A_NOTE_COLOR[0];
  }

  //Default color (Red)
  return A_NOTE_COLOR[0];
}

int getPitchGreenValue(int pitch) {
  //B
  if (pitch == NOTE_B0 || pitch == NOTE_B1 || pitch == NOTE_B2 || pitch == NOTE_B3 || pitch == NOTE_B4 || pitch == NOTE_B5 || pitch == NOTE_B6 || pitch == NOTE_B7) {
    //Orange
    return B_NOTE_COLOR[1];
  //C
  } else if (pitch == NOTE_C1 || pitch == NOTE_CS1 || pitch == NOTE_C2 || pitch == NOTE_CS2 || pitch == NOTE_C3 || pitch == NOTE_CS3 || pitch == NOTE_C4 || pitch == NOTE_CS4 || pitch == NOTE_C5 || pitch == NOTE_CS5 || pitch == NOTE_C6 || pitch == NOTE_CS6 || pitch == NOTE_C7 || pitch == NOTE_CS7 || pitch == NOTE_C8 || pitch == NOTE_CS8) {
    //Yellow
    return C_NOTE_COLOR[1];
  //D
  } else if (pitch == NOTE_D1 || pitch == NOTE_DS1 || pitch == NOTE_D2 || pitch == NOTE_DS2 || pitch == NOTE_D3 || pitch == NOTE_DS3 || pitch == NOTE_D4 || pitch == NOTE_DS4 || pitch == NOTE_D5 || pitch == NOTE_DS5 || pitch == NOTE_D6 || pitch == NOTE_DS6 || pitch == NOTE_D7 || pitch == NOTE_DS7 || pitch == NOTE_D8 || pitch == NOTE_DS8) {
    //Green
    return D_NOTE_COLOR[1];
  //E
  } else if (pitch == NOTE_E1 || pitch == NOTE_E2 || pitch == NOTE_E3 || pitch == NOTE_E4 || pitch == NOTE_E5 || pitch == NOTE_E6 || pitch == NOTE_E7) {
    //Blue
    return E_NOTE_COLOR[1];
  //F
  } else if (pitch == NOTE_F1 || pitch == NOTE_FS1 || pitch == NOTE_F2 || pitch == NOTE_FS2 || pitch == NOTE_F3 || pitch == NOTE_FS3 || pitch == NOTE_F4 || pitch == NOTE_FS4 || pitch == NOTE_F5 || pitch == NOTE_FS5 || pitch == NOTE_F6 || pitch == NOTE_FS6 || pitch == NOTE_F7 || pitch == NOTE_FS7) {
    //Indigo
    return F_NOTE_COLOR[1];
  //G
  } else if (pitch == NOTE_G1 || pitch == NOTE_GS1 || pitch == NOTE_G2 || pitch == NOTE_GS2 || pitch == NOTE_G3 || pitch == NOTE_GS3 || pitch == NOTE_G4 || pitch == NOTE_GS4 || pitch == NOTE_G5 || pitch == NOTE_GS5 || pitch == NOTE_G6 || pitch == NOTE_GS6 || pitch == NOTE_G7 || pitch == NOTE_GS7) {
    //Violet
    return G_NOTE_COLOR[1];
  //A
  } else if (pitch == NOTE_A1 || pitch == NOTE_AS1 || pitch == NOTE_A2 || pitch == NOTE_AS2 || pitch == NOTE_A3 || pitch == NOTE_AS3 || pitch == NOTE_A4 || pitch == NOTE_AS4 || pitch == NOTE_A5 || pitch == NOTE_AS5 || pitch == NOTE_A6 || pitch == NOTE_AS6 || pitch == NOTE_A7 || pitch == NOTE_AS7) {
    //Red
    return A_NOTE_COLOR[1];
  }

  //Default color (Red)
  return A_NOTE_COLOR[1];
}

int getPitchBlueValue(int pitch) {
  //B
  if (pitch == NOTE_B0 || pitch == NOTE_B1 || pitch == NOTE_B2 || pitch == NOTE_B3 || pitch == NOTE_B4 || pitch == NOTE_B5 || pitch == NOTE_B6 || pitch == NOTE_B7) {
    //Orange
    return B_NOTE_COLOR[2];
  //C
  } else if (pitch == NOTE_C1 || pitch == NOTE_CS1 || pitch == NOTE_C2 || pitch == NOTE_CS2 || pitch == NOTE_C3 || pitch == NOTE_CS3 || pitch == NOTE_C4 || pitch == NOTE_CS4 || pitch == NOTE_C5 || pitch == NOTE_CS5 || pitch == NOTE_C6 || pitch == NOTE_CS6 || pitch == NOTE_C7 || pitch == NOTE_CS7 || pitch == NOTE_C8 || pitch == NOTE_CS8) {
    //Yellow
    return C_NOTE_COLOR[2];
  //D
  } else if (pitch == NOTE_D1 || pitch == NOTE_DS1 || pitch == NOTE_D2 || pitch == NOTE_DS2 || pitch == NOTE_D3 || pitch == NOTE_DS3 || pitch == NOTE_D4 || pitch == NOTE_DS4 || pitch == NOTE_D5 || pitch == NOTE_DS5 || pitch == NOTE_D6 || pitch == NOTE_DS6 || pitch == NOTE_D7 || pitch == NOTE_DS7 || pitch == NOTE_D8 || pitch == NOTE_DS8) {
    //Green
    return D_NOTE_COLOR[2];
  //E
  } else if (pitch == NOTE_E1 || pitch == NOTE_E2 || pitch == NOTE_E3 || pitch == NOTE_E4 || pitch == NOTE_E5 || pitch == NOTE_E6 || pitch == NOTE_E7) {
    //Blue
    return E_NOTE_COLOR[2];
  //F
  } else if (pitch == NOTE_F1 || pitch == NOTE_FS1 || pitch == NOTE_F2 || pitch == NOTE_FS2 || pitch == NOTE_F3 || pitch == NOTE_FS3 || pitch == NOTE_F4 || pitch == NOTE_FS4 || pitch == NOTE_F5 || pitch == NOTE_FS5 || pitch == NOTE_F6 || pitch == NOTE_FS6 || pitch == NOTE_F7 || pitch == NOTE_FS7) {
    //Indigo
    return F_NOTE_COLOR[2];
  //G
  } else if (pitch == NOTE_G1 || pitch == NOTE_GS1 || pitch == NOTE_G2 || pitch == NOTE_GS2 || pitch == NOTE_G3 || pitch == NOTE_GS3 || pitch == NOTE_G4 || pitch == NOTE_GS4 || pitch == NOTE_G5 || pitch == NOTE_GS5 || pitch == NOTE_G6 || pitch == NOTE_GS6 || pitch == NOTE_G7 || pitch == NOTE_GS7) {
    //Violet
    return G_NOTE_COLOR[2];
  //A
  } else if (pitch == NOTE_A1 || pitch == NOTE_AS1 || pitch == NOTE_A2 || pitch == NOTE_AS2 || pitch == NOTE_A3 || pitch == NOTE_AS3 || pitch == NOTE_A4 || pitch == NOTE_AS4 || pitch == NOTE_A5 || pitch == NOTE_AS5 || pitch == NOTE_A6 || pitch == NOTE_AS6 || pitch == NOTE_A7 || pitch == NOTE_AS7) {
    //Red
    return A_NOTE_COLOR[2];
  }

  //Default color (Red)
  return A_NOTE_COLOR[2];
}

//Common Methods
//******************************************************************************
//Increment the index (and set to 0 if it goes past max)
int incrementIndex(int currentIndex, int maxIndex) {
  currentIndex = currentIndex + 1;
  if (currentIndex > maxIndex) {
    return 0;
  }
  return currentIndex;
}

//Decrement the index (and set to max if it goes below 0)
int decrementIndex(int currentIndex, int maxIndex) {
  currentIndex = currentIndex - 1;
  if (currentIndex < 0) {
    return maxIndex;
  }
  return currentIndex;
}

void setAllLEDsColor(int redColorValue, int greenColorValue, int blueColorValue) {
  for (int i=0;i<NUMBER_OF_LEDS;i++) {
     leds[i].setRGB(redColorValue, greenColorValue, blueColorValue);
  }
  FastLED.show();
}

//Used to clear all the LEDs after a light sequence is complete
void clearAllLEDs() {
  for (int i=0;i<NUMBER_OF_LEDS;i++) {
     leds[i] = CRGB::Black;
  }
  FastLED.show();
}
//******************************************************************************
