#include <SPI.h>
#include <SD.h>
const int chipSelect = 8;

#define ARRAY_SIZE 100

#define BUTTON_PIN 5
// Primary Pin, Non-negotiable
#define P_CLOCK 3
#define P_BUS 9

// Primary Variables
byte pData[ARRAY_SIZE];
byte pCounter = 0;
byte pBase;
byte pNumber = 0;

bool flag = HIGH;

// Secondary Pin, non-negotiable
#define S_CLOCK 2
#define S_BUS 4

// Secondary Variables
uint16_t sData[ARRAY_SIZE];
byte sCounter = 0;
uint16_t sBase;
uint16_t sNumber = 0;

void setup()  {
  
  // Works
  Serial.begin(115200);
  pinMode(P_CLOCK, INPUT);
  pinMode(S_CLOCK, INPUT);
  pinMode(P_BUS, INPUT);
  pinMode(S_BUS, INPUT);
  pinMode(BUTTON_PIN, INPUT);
  for(int i = 0; i < ARRAY_SIZE; i++)
  {
    pData[i] = 0;
    sData[i] = 0;
  }
  pBase = 128; sBase = 32768;
  pNumber = 0; sNumber = 0;
  
  attachInterrupt(digitalPinToInterrupt(P_CLOCK), readPrimaryData, FALLING);
  attachInterrupt(digitalPinToInterrupt(S_CLOCK), readSecondaryData, FALLING);
  /*
  // SD card initialization
  if(!SD.begin(chipSelect))
  {
    Serial.println("Card failed, or not present");
    while(1);
  }
  */
  Serial.println("begin:");
}
//  Works

void readPrimaryData() {
  bool msb1 = digitalRead(P_BUS);
  //Serial.print(msb1);
  pNumber += (pBase * msb1);
  pBase = pBase/2;
  if (pBase == 0)
  {
    //Serial.println();
    pData[pCounter++] = pNumber;
    pNumber = 0;
    pBase = 128;
  }
}

void readSecondaryData() {
  bool msb2 = digitalRead(S_BUS);
  //Serial.print(msb2);
  sNumber += (sBase * msb2);
  sBase = sBase/2;
  if (sBase == 0)
  {
    //Serial.println();
    sData[sCounter++] = sNumber;
    sNumber = 0;
    sBase = 32768;
  }
}

void loop() {
  if(digitalRead(BUTTON_PIN) && flag)
  {
    flag = LOW;
    // Writing leftover primary data to file
    String leftOverString = "";
    for(int i=0; i < pCounter; i++)
    {
      leftOverString += String(pData[i]) + "\t";
    }
    Serial.println(leftOverString);
    /*File dataFile1 = SD.open("pdata.txt", FILE_WRITE);
    if(dataFile1)
    {
      dataFile1.println(leftOverString);
      dataFile1.close();
      Serial.println(leftOverString);
    }
    else
    {
      Serial.println("Error Opening File");
    }*/
    // Writing leftover secondary data to file
    leftOverString = "";
    for(int i = 0; i < sCounter; i++)
    {
      leftOverString += String(sData[i]) + "\t";
    }
    Serial.println(leftOverString);
    /*dataFile1 = SD.open("sdata.txt", FILE_WRITE);
    if(dataFile1)
    {
      dataFile1.println(leftOverString);
      dataFile1.close();
      Serial.println(leftOverString);
    }
    else
    {
      Serial.println("Error Opening File");
    }*/
  }
  // Writing primary data to file
  if(!(pCounter < ARRAY_SIZE))
  {
    pCounter = 0;
    //int start = millis();
    String dataString1 = "";
    for (int i=0; i < ARRAY_SIZE; i++)
    {
      dataString1 += String(pData[i]) + "\t";
    }
    Serial.println(dataString1);
    /*File dataFile1 = SD.open("pdata.txt", FILE_WRITE);
    if(dataFile1)
    {
      dataFile1.println(dataString1);
      dataFile1.close();
      Serial.println(dataString1);
    }
    else
    {
      Serial.println("Error Opening File");
    }*/
  }
  // Writing secondary data to file
  if(!(sCounter < ARRAY_SIZE))
  {
    sCounter = 0;
    //int start = millis();
    String dataString2 = "";
    for (int i=0; i < ARRAY_SIZE; i++)
    {
      dataString2 += String(sData[i]) + "\t";
    }
    Serial.println(dataString2);
    /*File dataFile2 = SD.open("sdata.txt", FILE_WRITE);
    if(dataFile2)
    {
      dataFile2.println(dataString2);
      dataFile2.close();
      Serial.println(dataString2);
    }
    else
    {
      Serial.println("Error Opening File");
    }*/
  }
}
