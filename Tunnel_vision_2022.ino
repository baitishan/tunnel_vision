#include <Adafruit_NeoPixel.h>

#define PIN_Long_1 5
#define PIN_Long_2 6
#define PIN_Long_NUMPIXELS 21
#define PIN_short_1 7
#define PIN_short_2 8
#define PIN_short_NUMPIXELS 9

Adafruit_NeoPixel pixels_long1(PIN_Long_NUMPIXELS, PIN_Long_1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels_long2(PIN_Long_NUMPIXELS, PIN_Long_2, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels_short1(PIN_Long_NUMPIXELS, PIN_short_1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels_short2(PIN_Long_NUMPIXELS, PIN_short_2, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 80

//Max7219 8x8
int ANIMDELAY = 100;  // animation delay, deafault value is 100
int INTENSITYMIN = 0; // minimum brightness, valid range [0,15]
int INTENSITYMAX = 8; // maximum brightness, valid range [0,15]

int DIN_PIN = 12;      // data in pin
int CS_PIN = 11;       // load (CS) pin
int CLK_PIN = 10;      // clock pin

// MAX7219 registers
byte MAXREG_DECODEMODE = 0x09;
byte MAXREG_INTENSITY  = 0x0a;
byte MAXREG_SCANLIMIT  = 0x0b;
byte MAXREG_SHUTDOWN   = 0x0c;
byte MAXREG_DISPTEST   = 0x0f;

const unsigned char heart[] =
{
  B01100110,
  B11111111,
  B11111111,
  B11111111,
  B01111110,
  B00111100,
  B00011000,
  B00000000
};

int sensor = 2;              // Infrarotsensor HC-SR501
int state = LOW;             // by default, no motion detected
int val = 0;                 // variable to store the sensor status (value)

void setup() {
 

  pixels_long1.begin();
  pixels_long2.begin();
  pixels_short1.begin();
  pixels_short2.begin();

  pinMode(DIN_PIN, OUTPUT);
  pinMode(CLK_PIN, OUTPUT);
  pinMode(CS_PIN, OUTPUT);

  // initialization of the MAX7219
  setRegistry(MAXREG_SCANLIMIT, 0x07);
  setRegistry(MAXREG_DECODEMODE, 0x00);  // using an led matrix (not digits)
  setRegistry(MAXREG_SHUTDOWN, 0x01);    // not in shutdown mode
  setRegistry(MAXREG_DISPTEST, 0x00);    // no display test
  setRegistry(MAXREG_INTENSITY, 0x0f & INTENSITYMIN);

  // draw heart
  setRegistry(1, heart[0]);
  setRegistry(2, heart[1]);
  setRegistry(3, heart[2]);
  setRegistry(4, heart[3]);
  setRegistry(5, heart[4]);
  setRegistry(6, heart[5]);
  setRegistry(7, heart[6]);
  setRegistry(8, heart[7]);

  pinMode(sensor, INPUT);    // initialize IR sensor as an input
  Serial.begin(9600);        // initialize serial
      
}

void loop() {
  pixels_long1.clear();
  pixels_long2.clear();
  pixels_short1.clear();
  pixels_short2.clear();

  val = digitalRead(sensor);   // read sensor value

//  if (val == HIGH) {           // check if the sensor is HIGH
//    Serial.println("Motion detected!");

    // draw heart
    setRegistry(1, heart[0]);
    setRegistry(2, heart[1]);
    setRegistry(3, heart[2]);
    setRegistry(4, heart[3]);
    setRegistry(5, heart[4]);
    setRegistry(6, heart[5]);
    setRegistry(7, heart[6]);
    setRegistry(8, heart[7]);

    for(int i=PIN_Long_NUMPIXELS; i>=0; i--) {
  
     
      pixels_long1.setPixelColor(i, pixels_long1.Color(100, 100, 255));
      pixels_long2.setPixelColor(i, pixels_long1.Color(100, 100, 255));
      pixels_short1.setPixelColor(i, pixels_long1.Color(100, 100, 255));
      pixels_short2.setPixelColor(i, pixels_long1.Color(100, 100, 255));
      
      pixels_long1.show();
      pixels_long2.show();
      pixels_short1.show();
      pixels_short2.show();
      delay(DELAYVAL);
    }
    
      // second beat
      setRegistry(MAXREG_INTENSITY, 0x0f & INTENSITYMAX);
      delay(ANIMDELAY);
      
      // switch off
      setRegistry(MAXREG_INTENSITY, 0x0f & INTENSITYMIN);
      delay(ANIMDELAY);
      
      // second beat
      setRegistry(MAXREG_INTENSITY, 0x0f & INTENSITYMAX);
      delay(ANIMDELAY);
      
      // switch off
      setRegistry(MAXREG_INTENSITY, 0x0f & INTENSITYMIN);
      delay(ANIMDELAY*6);
//   }  else {
//     Serial.println("Motion ended!");
     pixels_long1.clear();
     pixels_long2.clear();
     pixels_short1.clear();
     pixels_short2.clear();


//     // draw heart
//     setRegistry(1, heart[7]);
//     setRegistry(2, heart[7]);
//     setRegistry(3, heart[7]);
//     setRegistry(4, heart[7]);
//     setRegistry(5, heart[7]);
//     setRegistry(6, heart[7]);
//     setRegistry(7, heart[7]);
//     setRegistry(8, heart[7]);

//  }
}


void setRegistry(byte reg, byte value)
{
  digitalWrite(CS_PIN, LOW);

  putByte(reg);   // specify register
  putByte(value); // send data

  digitalWrite(CS_PIN, LOW);
  digitalWrite(CS_PIN, HIGH);
}

void putByte(byte data)
{
  byte i = 8;
  byte mask;
  while (i > 0)
  {
    mask = 0x01 << (i - 1);        // get bitmask
    digitalWrite( CLK_PIN, LOW);   // tick
    if (data & mask)               // choose bit
      digitalWrite(DIN_PIN, HIGH); // send 1
    else
      digitalWrite(DIN_PIN, LOW);  // send 0
    digitalWrite(CLK_PIN, HIGH);   // tock
    --i;                           // move to lesser bit
  }
}
