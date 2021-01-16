# Arduino-RPM-Write-to-File
The master and slave sketches used to save rpm data using digital hall effect sensors

  The master Arduino is the one that is connected to the Hall Effect sensors and measures the time per revolution of the sensor in milliseconds. It saves each value in an array of size ARRAY_SIZE and when this array is full it sends the data to the slave Arduino. This data is send using a basic data transmission protocol and NR2 encoding. The data for each sensor is stored in it's own array and send through it own bus using it's own clock (i.e. for N number of sensors, N number of clocks and busses must be used).

  The slave Arduino is the one that receives the data and write to an a file on an SD card using an SD shield. The shield currently used is an OSEPP-MSDS-01 for an Arduino Uno. If the shield is changed the required pins must be changed. This Arduino receives the data from the master and stores it in an array of size ARRAY_SIZE (Note: These two arrays can be, and it is recommended they be, different sizes. Currently in the master sketch SIZE_ARRAY = 10 and in the slave sketch SIZE_ARRAY = 100. This is done to avoid opening the file as often since this is the time consuming part). Once the array is full it appends the file with all the values.
  
  The validation sketches were used to test the sketches.

# Curent Variables and Macros Used for the Master
The global names and variable are as follows:
* ARRAY_SIZE:                   Number of data points saved and transmitted to slave.
* BUTTON_PIN:                   This pin is tied to a button so that when pressed data remaining in the arrays at the end of a run is flushed out and saved.
* PRIMARY_PIN, SECONDARY_PIN:   The pins reading the Hall Effect sensors.   MUST ENSURE THESE PINS ARE CAPABLE OF HANDLING INTERRUPTS.
* P_CLOCK, S_CLOCK:             The pins used as the clock for data transmission.
* P_BUS, S_BUS:                 The pins used to transfer data to the slave.
* pCounter, sCounter:           Keeps track of the number of data points currently stored in the array.
* pTimeArray, sTimeArray:       Arrays where the data is stored.
* pInitialTime, sInitialTime:   Variables that store the time in which the last interrupt happened.
* pCurrentTime, sCurrentTime:   Variables that store the time when the current interrupt is called.
* flag:                         A one time variable used to prevent bouncing when the button is pushed at the end of a run. Set to HIGH.

# Curent Variables and Macros Used for the Slave
* chipSelect:           Pin needed for the shield and specified by the shiel.
* ARRAY_SIZE:           Number of data points stored and saved to file
* BUTTON_PIN:           This pin is tied to a button so that when pressed data remaining in the arrays at the end of a run is flushed out and saved.
* P_CLOCK, S_CLOCK:     Pins used as the clock for the data transmission.   MUST ENSURE THESE PINS ARE CAPABLE OF HANDLING INTERRUPTS.
* P_BUS, S_BUS:         The pins used to recieve data from the master.
* pData, sData:         Arrays where the data is stored.
* pCounter, sCounter:   Keeps track of the number of data points currently stored in the array.
* pBase, sBase:         Is used to help convert bits into base 10. Starts out as 128 for 8 bit and 32768 for 16 bit numbers.
* pNmuber, sNumber:     Stores the received data value in base 10.
* flag:                 A one time variable used to prevent bouncing when the button is pushed at the end of a run. Set to HIGH.

# Transmission Algorithm
  The transmission algorithm is as follows:
   *  Counter is set to 0. In the very improbable case that a revolution happens before the array data is done transmitting. Prevents array overflow or data loss.
   *  for loop that goes through the array starts
      * A temp variable revTime is made and stores the current element in the array.
      * A for loop to go through every bit of revTime (right now it's 8 bits for the primary and 16 bits for the secondary) starts
        * The most significant bit from revTime is stored and send to the bus
        * revTime is shifted left one bit
        * The clock in then pulsed HIGH and then LOW
        
# Receiving Algorithm
  The receiving algorithm is as follows.
  * A temp variable msb stores the bit currently on the bus
  * msb is multiplied by base and added to number
  * base is divided by 2 using interger division
  * once base reaches 0 number is added to the data array and the index is updated.
    * number is set back to 0
    * base is set back to 128 or 32768
    
# Basic Overview of the Master Sketch
  In setup(), the arrays are initialized with 0's. The sensor pin, referred in this file as SENSOR, is set to INPUT. The clock pin (CLOCK) and the bus pin (BUS) are set to OUTPUT and CLOCK is set to HIGH. SENSOR is attached to an interrupt and the sensors used are 1 except when it senses the magnet and then goes to 0. The InitialTime variables are set to the current time and setup() ends.
  In loop(), the sketch is constantly checking if the button was pressed or if either array is full. If not nothing happens. When the interrupts are called (i.e. a revolution happens), currentTime variable is updated, the difference between InitialTime and CurrentTime is stored in the appropriate place in the TimeArray according to the counter variable and counter is updated. This happens until the counter variable is equal to ARRAY_SIZE and the if statement in loop() becomes true. The transmission algorithm then begins.
  When the button is pushed flag is then set to low to prevent bouncing and then transmission agorithm starts. THE BUTTON SHOULD BE PUSHED ONLY AT THE END OF THE RUN WITH THE CAR OFF. ARDUINO SHOULD BE RESET BEFORE ANOTHER RUN BEGINS.

# Basic Overview of the Slave Sketch
  In setup(), the arrays are initialized with 0's. The clock pin (CLOCK) and bus pin (BUS) are set to INPUT. The button pin (BUTTON) is also set to INPUT. The base variables are set to 2^(n-1) where n is the size of the receiving data, in bits. CLOCK is attached to an interrupt and set to trigger on the falling edge. SD card is initialized. Setup() ends.
  In loop(), the sketch is constantly checking if the data array is full. When an iterrupt is triggered the receiving algorithm begins. When the data array is full one of the if statements becomes true. The sketch then converts the data stored in the array into a tab delimitered string and saves it into the corresponding file in the SD card.
