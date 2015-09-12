/*
MSR4 Datalogger Adjusted
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
 by Patrick Delaney
 modified 4 Sept 2015
 by Russ Terrel
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
float VDD = 4720; // VDD in MilliVolts
float DMR = 0.181; // Voltage Divider Value found by R2/(R2+R1)

#include <FileIO.h>

// Added 09-02-2015 RFT
int Loop_CNT = 0;

void setup() {
  // Initialize the Bridge and the Console
  Bridge.begin();
  Console.begin();
  FileSystem.begin();

  // Modified 09-07-2015 RFT (Remove comment for debug)
  // while (!Console); // wait for Console port to connect.
  Console.println("Filesystem datalogger\n");

  // Add 09-3-2015 RFT
  newFile();
}

void loop() {
  // Added 09-02-2015 RFT
  if (Loop_CNT == 0) {
    writeHeader();
  }
  Loop_CNT += 1;
  // make a string that starts with a timestamp for assembling the data to log:
  String dataString;
  dataString += getTimeStamp();
  dataString += " = ";


  // read 3 sensors and append to dataString
  // Declare variables for purposes listed - A0: "Vbatt", A1: "Io", A2: "Ii"
  for (int analogPin = 0; analogPin < 3; analogPin++) {
    int sensor = analogRead(analogPin);

    // Delcare string within scope
    // Outputvalue must be a float because it is a fraction.
    float outputvalue;
    String output;


    // Calculate voltage by voltage divider function, Volt1
    if (analogPin == 0) {
      outputvalue = (1 / DMR ) * ((sensor / 1023.0) * VDD); // (sensor/1023)*VDD gets you mV of output, assuming VDD = 4720
      output = String(outputvalue);
    }

    // Calculate output amps by scaling function, Amps1
    if (analogPin == 1) {
      // Calculate voltage by scaling function
      outputvalue = ((((sensor / 1023.0) * VDD) - ACSoffset) / mVperAmp); // gets you mV/mVperAmp
      output = String(outputvalue);
    }

    // Calculate input amps by scaling function, Amps2
    if (analogPin == 2) {
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
    // print to the Console port too:
    Console.println(dataString);
  }
  // if the file isn't open, pop up an error:
  else {
    Console.println("error opening datalog.txt");
  }

  // Added 09-03-2015 RFT
  if (Loop_CNT == 120) {
    newFile();
    Loop_CNT = 0;
  }

  delay(15000);

}

// This function return a string with the time stamp
String getTimeStamp() {
  String result;
  Process time;
  time.begin("date");
  time.addParameter("+%D-%T");  // parameters: D for the complete date mm/dd/yy
  //             T for the time hh:mm:ss
  time.run();  // run the command

  // read the output of the command
  while (time.available() > 0) {
    char c = time.read();
    if (c != '\n')
      result += c;
  }

  return result;
}

//Added 09-02-2015 RFT
void writeHeader() {
  Process time;
  time.begin("date");
  time.addParameter("+%D,%T,%Z");
  time.run();
  String timeString = time.readString();
  File dataFile = FileSystem.open("/mnt/sda1/datalog.txt", FILE_APPEND);
  if (dataFile) {
    dataFile.print(timeString);
    dataFile.close();
    Console.println(timeString);
  }
  else {
    Console.println("error opening datalog.txt");
  }
}

//Added 09-03-2015 RFT
void newFile() {
  Process p;
  // Rename file. Move datalog.txt to datalog1.txt
  p.runShellCommand("/usr/bin/pretty-wifi-info.lua | mv /mnt/sda1/datalog.txt /mnt/sda1/datalog1.txt");
  while (p.running());
}
