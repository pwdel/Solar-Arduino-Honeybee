/*
Measuring Current Using ACS712, Adapted for non-regulated input voltage
*/
const int analogIn = A0;
int mVperAmp = 185; // use 185 for 5A Module, 100 for 20A Module, 66 for 30A Module
int RawValue= 0;

/*
The nominal ACSoffset is 2.5V.  The derived value is VCC/2
However, it is suggested to calibrate this value by connecting the ACS712 current inputs to 0
When the ACS712 current = 0, the measured output value will equal the ACSoffset.
VCC can be measured by plugging the Arduino 5V to A0.
*/
int ACSoffset = 2260; // find this value by disconnecting 
double Voltage = 0;
double Amps = 0;

void setup(){ 
 Serial.begin(9600);
}

void loop(){
 
 RawValue = analogRead(analogIn);
 Voltage = (RawValue / 1023.0) * 4720; // Gets you mV
 Amps = ((Voltage - ACSoffset) / mVperAmp);
 
 
 Serial.print("Raw Value = " ); // shows pre-scaled value 
 Serial.print(RawValue); 
 Serial.print("\t mV = "); // shows the voltage measured 
 Serial.print(Voltage,3); // the '3' after voltage allows you to display 3 digits after decimal point
 Serial.print("\t Amps = "); // shows the voltage measured 
 Serial.println(Amps,3); // the '3' after voltage allows you to display 3 digits after decimal point
 delay(2500); 
 
}
