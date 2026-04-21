//
// Software for anemometer unit
// Version 2
// Frequency: 916 MHz
// v1  --  07/03/2019  Original file
// v2  --  08/09/2019  Added support for MPL3115A2 barometer
// v3  --  10/03/2019  Added ability for multiple anemometers to connect to one control station
// NEW VERSION CREATED FROM Anemometer and MiniGNI
// All code updated by KLA, 2023/12/28

#define USE_WIRELESS // Talk over the RFM95 LoRa Radio (adafruit id# 3178)
#define USE_SD
#define USE_BARO
#define USE_RH

#include <FIR.h>
#include <Wire.h>
#include <SPI.h>
#include <Servo.h> //KLA added 

//KLA anemometer defintions 


#define CPU_HZ 48000000
#define TIMER_PRESCALER_DIV 1024
#define SEC_WIND_AVERAGE 5 // Number of second wind average reported

//KLA miniGNI definitons
#define SERVO_PIN 9
#define SD_CHIP_SELECT 10
#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 3
//#define DHTPIN 5 //KLA added this from miniGNI code
// DHT PIN WAS CHANGED BECUASE IT WAS THE SAME PIN AS THE ANEMOMETER 

//***** KLA code from miniGNI that's replaced with other functions potentially

//radio 
#ifdef USE_WIRELESS
  #include <RH_RF95.h>
  #define RF95_FREQ 912.0 //KLA change this depending on miniGNI number
  RH_RF95 rf95(RFM95_CS, RFM95_INT);
  int16_t packetnum = 0;  // packet counter, we increment per xmission
#endif
// #ifdef 
//   #include <RH_RF95.h>
//   #define RF95_FREQ 915.0 //change this depending on what mini-GNI + control station pair
//   RH_RF95 rf95(RFM95_CS, RFM95_INT);
//   int16_t packetnum = 0;  // packet counter, we increment per xmission
// #endif

//temperature humidity sensor 
// #ifdef USE_DHT
//   #include "DHT.h"
//   #define DHTTYPE DHT22
//   DHT dht(DHTPIN, DHTTYPE);
// #endif
// KLA original code from miniGNI
#ifdef USE_RH
  #include <Adafruit_AHTX0.h> //KLA added 25-06-09
  Adafruit_AHTX0 aht;
#endif

// barometer
//Adafruit_MPL3115A2 baro = Adafruit_MPL3115A2();
//KLA miniGNI code below, coded out
#ifdef USE_BARO
 #include <Adafruit_MPL3115A2.h>
 Adafruit_MPL3115A2 baro = Adafruit_MPL3115A2();
#endif

// SD CARD
#ifdef USE_SD
  #include <SD.h>
  #include "RTClib.h"
  RTC_PCF8523 rtc;
  File logfile;
#endif

//// servo motor position
//Servo myservo;
//int pos = 0;            // variable to store the servo position
//bool door_status = 0;   // 0:closed, 1:open

Servo myservo;
int pos = 0;            // variable to store the servo position
bool door_status = 0;   // 0:closed, 1:open


// Calibration constants for the anemometer
const float anemometer_calibration_coeff_a = 0;
const float anemometer_calibration_coeff_b = 0.02468637;
const float anemometer_calibration_coeff_c = 1.818727;

// Function prototypes
void TC5_Handler(void);
bool tcIsSyncing();
void tcStartCounter();
void tcReset();
void tcDisable();
void tcConfigure(int);
void isr_pin_rising();

// Make an instance of the FIR filter. In this case it will be
// an N point moving average
FIR<float, SEC_WIND_AVERAGE> fir;

char logfile_name[13];
uint32_t rising_counter = 0; // Counts the number of rising pulses
int filtered_frequency; // FIR filtered anemometer frequency



void send_windspeed_on_lora(int windspeed, int altitude)
{
  // Select LoRa
  digitalWrite(SD_CHIP_SELECT, HIGH);
  digitalWrite(RFM95_CS, LOW);
  
  // Send the scaled windspeed (windspeed * 10) via the LoRA radio
  char buffer[12] = "";
  sprintf(buffer, "%06d,%04d\r\n", altitude, windspeed);
  Serial.println(buffer);
  rf95.send((uint8_t *)buffer, 12);
  delay(10);
  rf95.waitPacketSent();

  // De-select LoRa
  digitalWrite(RFM95_CS, HIGH);
  digitalWrite(SD_CHIP_SELECT, LOW);
}

void get_log_file_name(char *filename)
{
  /*
  Format is YYMMDDHH.DAT
  YY  - Year
  MM - Month
  DD - Day
  HH  - Hour
  */

  DateTime now = rtc.now();

  strcpy(filename, "00000000.TXT");

  // Set the Year
  filename[0] = '0' + (now.year() / 10) % 10;
  filename[1] = '0' + now.year() % 10;

  // Set the Month
  filename[2] = '0' + (now.month() / 10) % 10;
  filename[3] = '0' + now.month() % 10;

  // Set the Day
  filename[4] = '0' + (now.day() / 10) % 10;
  filename[5] = '0' + now.day() % 10;

  // Set the hour
  filename[6] = '0' + (now.hour() / 10) % 10;
  filename[7] = '0' + now.hour() % 10;
}


bool check_and_initialize_file(char *fname)
{
  /*
   * Check to see if the filename already exists on the SD card. If not, create
   * it and write the header.
   */
   if (!SD.exists(fname)){
     logfile = SD.open(fname, FILE_WRITE);
     if (!logfile)
     {
       Serial.println("Cannot open/create log file.");
     }

     logfile.write("mini-GNI Anemometer Log\r\n");
     logfile.write("Anemometer calibration coefficients: ");
     char buffer[100] = "";
     sprintf(buffer, "a=%f\tb=%f\tc=%f\r\n");
     logfile.write(buffer);
     logfile.close();
   }
   return 0;
}




float frequency_to_windspeed(float a, float b, float c, uint32_t frequency)
{
  /*
   * This function turns the frequency into the windspeed using the
   * calibration coefficients for the given sensor. Currently supports
   * up to quadratic calibration curves.
   */
   if (frequency == 0)
   {
    return 0;
   }
  return a * frequency * frequency + b * frequency + c;
}



void setup()
{
  // Setup serial
  Serial.begin(115200);
  String setup_error = "NONE";
  pinMode(SERVO_PIN, OUTPUT);

  // Serial.begin(115200);
  // delay(1000);
  // Serial.println("Mini-GNIv2 + Anemometer");
  #ifdef USE_RH
    if (!aht.begin()){
      while (1) delay(10);}
  #endif  

  #ifdef USE_WIRELESS
    pinMode(RFM95_RST, OUTPUT);
    digitalWrite(RFM95_RST, HIGH);
    digitalWrite(RFM95_RST, LOW);
    delay(10);
    digitalWrite(RFM95_RST, HIGH);
    delay(10);
    if(!rf95.init()) {
      setup_error = "RF INIT";
    }
    if (!rf95.setFrequency(RF95_FREQ)) {
      setup_error = "RF FREQ";
    }
  #endif  

  #ifdef USE_BARO
    if (! baro.begin()) {
      setup_error = "BARO INIT";
    }
  #endif
  

  // Setup FIR Filter
  float fir_coef[SEC_WIND_AVERAGE];
  memset(fir_coef, 1, sizeof(fir_coef));
  fir.setFilterCoeffs(fir_coef);

  // Setup the RTC and set the datetime if it wasnt initializaed yet
  if (! rtc.begin())
  {
    Serial.println("Cannot Start RTC");
    while (1);
  }

  if (! rtc.initialized())
  {
    Serial.println("RTC not initialized - setting to compile date/time.");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  // Setup interrupts and timer for anemometer measurement
  attachInterrupt(digitalPinToInterrupt(6), isr_pin_rising, RISING);
  tcConfigure(1024); //configure the timer to run at 1 Hertz (1024 counts)
  tcStartCounter(); //starts the timer

  #ifdef USE_SD
    if ( !rtc.initialized()) {
      Serial.println("RTC is NOT running! Setting now");
      rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); //sets the RTC to the date & time this sketch was compiled
    }
    if (!SD.begin(SD_CHIP_SELECT)) {
      Serial.println("Card init. failed!");
    }
   
    get_log_file_name(logfile_name);
    logfile = SD.open(logfile_name, FILE_WRITE);
    logfile.print("Datetime,Frequency,Windspeed,Altitude,TemperatureC,Pressure,RelativeHum,TempDH,DoorStatus\r\n");
    //logfile.print("Timestamp\tHz\tm/s\r\n");
    logfile.close();
  #endif  

  myservo.attach(SERVO_PIN);
  myservo.write(0); //start closed!
  delay(2000); //wait for the movement
  myservo.detach();

  // Open up the logfile for this datetime and write header
 
}




// int lenMicroSecondsOfPeriod = 20 * 1000; // 20 milliseconds (ms)
// int closedPosition = 1.0 * 1000; // 1.0 ms is 0 degrees
// int openPosition = 2.0 * 1000; // 2.0 ms is 180 degrees





void loop()
{
  delay(1000);
  // Main loop:
  // * Get the measured filtered anemometer frequency.
  // * Calibrate frequency to physical units.
  // * Send the data over serial and log to file.
  // * Every 30 packets, send one via LoRA.

  //KLA from miniGNI - establishing open and close commands 
  static int radio_message_counter = 30;

  char open_command[20];
  char close_command[20];

  sprintf(open_command, "OPEN %02d",1);
  sprintf(close_command, "CLOSE %02d",1);


  // Get the measured filtered anemometer frequency.
  int anemometer_frequency = filtered_frequency;
  DateTime now = rtc.now();

  // Calibrate frequency to physical units.
  float anemometer_windspeed = frequency_to_windspeed(anemometer_calibration_coeff_a,
                                                     anemometer_calibration_coeff_b,
                                                     anemometer_calibration_coeff_c,
                                                     filtered_frequency);
  int windspeed_int = 0;
  windspeed_int = round(100*anemometer_windspeed);
  
  // read in altitude, temperature, pressure from barometer
  int altm_int = 0;
  int tempC_int = 0;
  int presP_int = 0;
  int rhDH_int = 0; //KLA
  int tempDH_int = 0; //KLA
  int wind_send = 0;

  #ifdef USE_BARO
    float altm  = baro.getAltitude();
    float tempC = baro.getTemperature();
    float presP = baro.getPressure();
    altm_int  = round(100*altm);
    tempC_int = round(100*tempC);
    presP_int = round(100*presP);
  #endif
  
  #ifdef USE_RH
    sensors_event_t humidity, temp;
    aht.getEvent(&humidity, &temp); // Read both humidity and temperature from AHT

    float rhDH = humidity.relative_humidity;
    float tempDH = temp.temperature;
    rhDH_int = round(100*rhDH);
    tempDH_int = round(100*tempDH);
  #endif

  Serial.print("Temp: ");
  Serial.println(temp.temperature);
  Serial.print("RH: ");
  Serial.println(humidity.relative_humidity);
  
  // Send data over serial and log to file.
  char message[68];
  sprintf(message, "%04d%02d%02dT%02d%02d%02d,%d,%f,%06d,%04d,%06d,%04d,%04d,%1d", now.month(), now.day(), now.year(),
          now.hour(), now.minute(), now.second(), filtered_frequency, anemometer_windspeed, altm_int, tempC_int, presP_int, rhDH_int,tempDH_int,door_status);
  
 // Write message to file
   #ifdef USE_SD
    digitalWrite(8,HIGH);
    delay(100);
    File logfile = SD.open(logfile_name, FILE_WRITE);
    logfile.println(message);
    logfile.close();
    digitalWrite(8,LOW);
    Serial.println("Transmitting..."); // Send a message to rf95_server  
    Serial.print("Sending "); 
    Serial.println(message);
    Serial.println("Sending...");
    delay(10);
    //digitalWrite(RFM95_CS, LOW); 
  #endif
  // Echo to serial terminal (optional)

    

 // Now to base station screen 
  //message_send[18] = 0;
  if (3 <= radio_message_counter)
  {
    radio_message_counter = 0;
    digitalWrite(SD_CHIP_SELECT, HIGH);
    digitalWrite(RFM95_CS, LOW);
    char message_send[25] = "";
    sprintf(message_send, "%06d,%04d,%04d,%04d,%1d",altm_int,rhDH_int,tempDH_int,windspeed_int,door_status);
    #ifdef USE_WIRELESS
      message_send[24] = 0;
      rf95.send((uint8_t *)message_send, 25);
      delay(10);
      rf95.waitPacketSent();
      uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
      uint8_t len = sizeof(buf);
      if (rf95.waitAvailableTimeout(1000))
      { 
            // Should be a reply message for us now   
       if (rf95.recv(buf,&len))
       {
          Serial.print("Got reply: ");
          Serial.println((char*)buf);
          Serial.print("RSSI: ");
          Serial.println(rf95.lastRssi(), DEC);
          if( strcmp( (char*)buf ,close_command) == 0){ //KLA
            myservo.attach(9);
            for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
              myservo.write(pos);                 // tell servo to go to position in variable 'pos'
              delay(15);                          // waits 15ms for the servo to reach the position
            }
            myservo.detach();
            door_status = 0;
          }
          if( strcmp( (char*)buf ,open_command) == 0){ // KLA
            myservo.attach(9);
            for (pos = 0; pos <= 180; pos += 1) { 
              myservo.write(pos);              
              delay(15);                       
            }
            door_status = 1;
            delay(1000);
            myservo.detach();
          }
        }
        else
        {
          Serial.println("Receive failed");
        }
      }
      else
      {
      }
    #endif
  }
 // Send message via radio every 30th message
  //   if (9 <= radio_message_counter)
  //   {
  //    // Time to send the packet and reset
  //     radio_message_counter = 0;
  //     Serial.println("Sending via LORA.");
  //     send_windspeed_on_lora(windspeed_int, altm_int);
  //   }
  radio_message_counter ++;

}




/*
 * Timer/Interrupt helpers, ISRs - There should be no need to modify any of this
 */

void isr_pin_rising(){
  /*
   * ISR for counting rising events
   */
  rising_counter++;
}


void TC5_Handler (void) {
  //Serial.println(millis());  // Want to check that this really runs at 1 Hz?
  filtered_frequency = fir.processReading(rising_counter);
  rising_counter = 0;
  TC5->COUNT16.INTFLAG.bit.MC0 = 1;
}


/* 
 *  TIMER SPECIFIC FUNCTIONS FOLLOW
 *  you shouldn't change these unless you know what you're doing
 */

//Configures the TC to generate output events at the sample frequency.
//Configures the TC in Frequency Generation mode, with an event output once
//each time the audio sample frequency period expires.
 void tcConfigure(int sampleRate)
{
 // Enable GCLK for TCC2 and TC5 (timer counter input clock)
 GCLK->CLKCTRL.reg = (uint16_t) (GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN_GCLK0 | GCLK_CLKCTRL_ID(GCM_TC4_TC5)) ;
 while (GCLK->STATUS.bit.SYNCBUSY);

 tcReset(); //reset TC5

 // Set Timer counter Mode to 16 bits
 TC5->COUNT16.CTRLA.reg |= TC_CTRLA_MODE_COUNT16;
 // Set TC5 mode as match frequency
 TC5->COUNT16.CTRLA.reg |= TC_CTRLA_WAVEGEN_MFRQ;
 //set prescaler and enable TC5
 TC5->COUNT16.CTRLA.reg |= TC_CTRLA_PRESCALER_DIV1024 | TC_CTRLA_ENABLE;
 //set TC5 timer counter based off of the system clock and the user defined sample rate or waveform
 TC5->COUNT16.CC[0].reg = (uint16_t) (SystemCoreClock / sampleRate - 1);
 while (tcIsSyncing());
 
 // Configure interrupt request
 NVIC_DisableIRQ(TC5_IRQn);
 NVIC_ClearPendingIRQ(TC5_IRQn);
 NVIC_SetPriority(TC5_IRQn, 0);
 NVIC_EnableIRQ(TC5_IRQn);

 // Enable the TC5 interrupt request
 TC5->COUNT16.INTENSET.bit.MC0 = 1;
 while (tcIsSyncing()); //wait until TC5 is done syncing 
} 

//Function that is used to check if TC5 is done syncing
//returns true when it is done syncing
bool tcIsSyncing()
{
  return TC5->COUNT16.STATUS.reg & TC_STATUS_SYNCBUSY;
}

//This function enables TC5 and waits for it to be ready
void tcStartCounter()
{
  TC5->COUNT16.CTRLA.reg |= TC_CTRLA_ENABLE; //set the CTRLA register
  while (tcIsSyncing()); //wait until snyc'd
}

//Reset TC5 
void tcReset()
{
  TC5->COUNT16.CTRLA.reg = TC_CTRLA_SWRST;
  while (tcIsSyncing());
  while (TC5->COUNT16.CTRLA.bit.SWRST);
}

//disable TC5
void tcDisable()
{
  TC5->COUNT16.CTRLA.reg &= ~TC_CTRLA_ENABLE;
  while (tcIsSyncing());
}
