// This section is to see if the master recorded every pulse send by a magnet
#define ARRAY_SIZE 10
#define BUTTON_PIN 5

// Primary Pins
#define PRIMARY_PIN 2
#define P_CLOCK 12
#define P_BUS 11

// Primary Variables
byte pCounter = 0, pTimeArray[10];
int pInitialTime, pCurrentTime;

// Secondary Pins
#define SECONDARY_PIN 3
#define S_BUS 10
#define S_CLOCK 9

// Secondary Variables
byte sCounter = 0;
uint16_t sTimeArray[10];
int sInitialTime, sCurrentTime;

bool flag = HIGH;

void setup()  {
  Serial.begin(115200);
  for (int i=0; i < ARRAY_SIZE; i++)
  {
    pTimeArray[i] = 0;
    sTimeArray[i] = 0;
  }
  // Pins for the primary sensor
  pinMode(PRIMARY_PIN, INPUT);
  pinMode(P_CLOCK, OUTPUT);
  pinMode(P_BUS, OUTPUT);
  pinMode(P_CLOCK, HIGH);

  // Pins for the secondary sensor
  pinMode(SECONDARY_PIN, INPUT);
  pinMode(S_CLOCK, OUTPUT);
  pinMode(S_BUS, OUTPUT);
  pinMode(S_CLOCK, HIGH);

  pinMode(BUTTON_PIN, INPUT);
  Serial.println("begin:");
  EIFR |= (1 << INTF1); // Don't worry about it
  
  attachInterrupt(digitalPinToInterrupt(PRIMARY_PIN), primaryRev, FALLING);
  attachInterrupt(digitalPinToInterrupt(SECONDARY_PIN), secondaryRev, FALLING);
  pInitialTime = millis();
  sInitialTime = pInitialTime;
}

void primaryRev() {
  pCurrentTime = millis();
  pTimeArray[pCounter] = pCurrentTime - pInitialTime;
  //Serial.println(pTimeArray[pCounter]);
  pInitialTime = pCurrentTime;
  pCounter++;
}

void secondaryRev() {
  sCurrentTime = millis();
  sTimeArray[sCounter++] = sCurrentTime - sInitialTime;
  sInitialTime = sCurrentTime;
}

void loop() {
  if(digitalRead(BUTTON_PIN) && flag)
  {
    flag = LOW;
    Serial.println("p");
    for(int i=0; i < pCounter; i++)
    {
      byte revTime2 = pTimeArray[i];
      for(int j=0; j < 8; j++)
      {
        bool msb = revTime2 & 0x80;
        Serial.print(msb);
        digitalWrite(P_BUS, msb);
        revTime2 <<= 1;
        digitalWrite(P_CLOCK, HIGH);
        digitalWrite(P_CLOCK, LOW);
      }
      Serial.println();
    }
    Serial.println("s");
    for(int i=0; i < sCounter; i++)
    {
      uint16_t revTime2 = sTimeArray[i];
      for(int j=0; j < 16; j++)
      {
        bool msb = revTime2 & 0x8000;
        Serial.print(msb);
        digitalWrite(S_BUS, msb);
        revTime2 <<= 1;
        digitalWrite(S_CLOCK, HIGH);
        digitalWrite(S_CLOCK, LOW);
      }
      Serial.println();
    }
    pCounter = 0;
    sCounter = 0;
  }
  // Sending primary data to slave
  if(pCounter >= ARRAY_SIZE)
  {
    pCounter = 0;
    Serial.println("p");
    for(int i=0; i < ARRAY_SIZE; i++)
    {
      byte revTime = pTimeArray[i];
      for(int j=0; j < 8; j++)
      {
        bool msb = revTime & 0x80;
        Serial.print(msb);
        digitalWrite(P_BUS, msb);
        revTime <<= 1;
        digitalWrite(P_CLOCK, HIGH);
        digitalWrite(P_CLOCK, LOW);
      }
      Serial.println();
    }
  }
  // Sending secondary data to slave
  if(sCounter >= ARRAY_SIZE)
  {
    sCounter = 0;
    Serial.println("s");
    for(int i=0; i < ARRAY_SIZE; i++)
    {
      uint16_t revTime1 = sTimeArray[i];
      //Serial.println(revTime1);
      for(int j=0; j < 16; j++)
      {
        bool msb1 = revTime1 & 0x8000;
        Serial.print(msb1);
        digitalWrite(S_BUS, msb1);
        revTime1 <<= 1;
        //Serial.println(revTime1);
        digitalWrite(S_CLOCK, HIGH);
        digitalWrite(S_CLOCK, LOW);
      }
      Serial.println();
    }
  }
}
/*
// This section is to see if the slave can write all data to file
#define ARRAY_SIZE 10
#define REV_TIME 0x0505
#define BUS 11
#define CLK 12
void setup() {
  Serial.begin(115200);
  pinMode(BUS, OUTPUT);
  pinMode(CLK, OUTPUT);
  // Primary 8 bit
  for(int l=0; l < 106; l++)
  {
    //int start = micros();
    for(int i=0; i < ARRAY_SIZE; i++)
    {
      byte revTime = REV_TIME;
      for(int j=0; j < 8; j++)
      {
        bool msb = revTime & 0x80;
        Serial.print(msb);
        digitalWrite(BUS, msb);
        revTime <<= 1;
        digitalWrite(CLK, HIGH);
        digitalWrite(CLK, LOW);
      }
      Serial.println();
    }
    delay(REV_TIME*ARRAY_SIZE);
  }

  // Secondary 16 bit
  for(int l=0; l < 16; l++)
  {
    //int start = micros();
    for(int i=0; i < ARRAY_SIZE; i++)
    {
      uint16_t revTime = REV_TIME;
      for(int j=0; j < 16; j++)
      {
        bool msb = revTime & 0x8000;
        Serial.print(msb);
        digitalWrite(BUS, msb);
        revTime <<= 1;
        digitalWrite(CLK, HIGH);
        digitalWrite(CLK, LOW);
      }
      Serial.println();
    }
    delay(REV_TIME*ARRAY_SIZE);
  }
}

void loop() {
}*/
