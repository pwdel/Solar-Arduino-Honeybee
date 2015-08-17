
/*
MSR3 Datalogger Adjusted

 The circuit:
 * analog sensors on analog pins 0, 1 and 2
 * SD card attached to SD card slot of the Arduino Yún
 * Assuming a 4.72VDD (e.g. not 5.0V Regulated)
 * Measuring Current Using ACS712, Adapted for non-regulated input voltage
 * Measuring voltage through a voltage divider using a nominal 1000ohm and 220ohm resistor

Run YunDiskExpander function to get the SD Card partitioned.

Log into your Linolo version of Arduino and ensure that the path "/mnt/sda1" path.
Your path may be different - figure out your path and put it in the code.

 You can remove the SD card while the Linux and the 
 sketch are running but be careful not to remove it while
 the system is writing to it.

 created  16 Aug 2015
 modified 
 by Patrick Delaney

 */

// Variables for current sensor
int mVperAmp = 185; // use 185 for 5A Module, 100 for 20A Module, 66 for 30A Module

/*
The nominal ACSoffset is 2.5V.  The derived value is VCC/2
However, it is suggested to calibrate this value by connecting the ACS712 current inputs to 0
When the ACS712 current = 0, the measured output value will equal the ACSoffset.
VCC can be measured by plugging the Arduino 5V to A0.
*/
int ACSoffset = 2260; // find this value by connecting output to VDD, and measure RawValues
int VDD = 4720; // VDD in mV

#include <FileIO.h>

void setup() {
  // Initialize the Bridge and the Serial
  Bridge.begin();
  Serial.begin(9600);
  FileSystem.begin();

  while(!Serial);  // wait for Serial port to connect.
  Serial.println("Filesystem datalogger\n");
}

void loop() {
  // make a string that start with a timestamp for assembling the data to log:
  String dataString;
  dataString += getTimeStamp();
  dataString += " = ";

    
  // read 3 sensors and append to dataString
  // Declare variables for purposes listed - A0: "Vbatt", A1: "Io", A2: "Ii"
   for (int analogPin = 0; analogPin < 3; analogPin++) {
    int sensor = analogRead(analogPin);

      // Delcare string within scope
      int outputvalue;
      String output;


      // Calculate voltage by voltage divider function, Volt1
    if (analogPin == 0){
      outputvalue = (220 / 1220 ) * ((sensor / 1023.0) * VDD); // (sensor/1023)*VDD gets you mV of output, assuming VDD = 4720
      output = String(outputvalue);
    }

      // Calculate output amps by scaling function, Amps1
    if (analogPin == 1){
      // Calculate voltage by scaling function
      outputvalue = ((((sensor / 1023.0) * VDD) - ACSoffset) / mVperAmp); // gets you mV/mVperAmp
      output = String(outputvalue);
    }

      // Calculate input amps by scaling function, Amps2
    if (analogPin == 2){
      outputvalue = ((((sensor / 1023.0) * VDD) - ACSoffset) / mVperAmp); // gets you mV/mVperAmp
      output = String(outputvalue);
    }

    dataString += String(output);

    if (analogPin < 2) {
      dataString += ",";  // separate the values with a comma
    }
  }
  
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  // The FileSystem card is mounted at the following "/mnt/FileSystema1"
  File dataFile = FileSystem.open("/mnt/sda1/datalog.txt", FILE_APPEND);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
    Serial.println(dataString);
  }  
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  } 

  delay(15000);  
    
}

// This function return a string with the time stamp
String getTimeStamp() {
  String result;
  Process time;
  // date is a command line utility to get the date and the time 
  // in different formats depending on the additional parameter 
  time.begin("date");
  time.addParameter("+%D-%T");  // parameters: D for the complete date mm/dd/yy
                                //             T for the time hh:mm:ss    
  time.run();  // run the command

  // read the output of the command
  while(time.available()>0) {
    char c = time.read();
    if(c != '\n')
      result += c;
  }

  return result;
}
