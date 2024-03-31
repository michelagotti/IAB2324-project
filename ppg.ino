//#include <LiquidCrystal.h>

// Initialize the LCD
const int rs = 3, en = 4, d4 = 5, d5 = 6, d6 = 7, d7 = 8;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Initialize the global variables to measure time, beats and bpm
int count=0;
int beats=0;
int bpm=0;

void setup()
{ 
  // Serial connection to PC at 115200 bits per second
  Serial.begin(115200);
 
  // Set signal labels in the Serial Plotter
  Serial.println();
  Serial.println("GND:");
  Serial.println("max:");
  Serial.println("smooth:");
  Serial.println("raw:");
  Serial.println("flat:");
  Serial.println("beat:");
  // Set the LCD display to have in the first row the "BPM:" indication and in the second
  // row, the actual number 
  lcd.begin(16, 2);

  lcd.setCursor(0, 0);
  lcd.print("BPM:");
  lcd.setCursor(0,1);
  lcd.print(bpm);
}

void loop()
{
  // Declare variables
  int raw;                 // variable to store measured voltage
  static float smooth = 0; // variable to store smoothed signal
  static float flat = 0;   // variable to store flattened voltage
  int beat;                // indicates when a beat is detected
  int scale=7;             // magnitude factor for better visualization
  String number_show;      // variable to store the bpm value, to show

  // Record raw PPG signal from analog input pin A7
  raw = analogRead(7)*scale;
   
  // Generate two filtered versions of PPG signal
  int previous_smooth = smooth;  // remember previous signal value
  smooth = 0.9*smooth + 0.1*raw;
  flat= 0.999* flat + 0.001*raw;
  
  // Detect the beat when smooth signal exceeds flat signal
  if (smooth >= flat && previous_smooth < flat) 
  {
    beat = 600; //value established only for a matter of visualization
    beats = beats+1;
  } 
  else beat = 500;
 
  // Print current values of all signals on a single line of text
  Serial.print("0,650,"); // print minimum and maximum values
  Serial.print(smooth);    // print current signal level of the smoothed one 
  Serial.print(",");       
  Serial.print(raw);       // print current signal level of the raw one
  Serial.print(",");      
  Serial.print(flat);      // print current signal level of the flaw one
  Serial.print(",");       
  Serial.print(beat);      // display the beat graph
  Serial.print(",");
  Serial.println(bpm);     // display the beats per minute
  
  
  // Discharge capacitor if signal is stuck close to minimum
  if (smooth < 30)
  {
    digitalWrite(2, LOW);      // set capacitor discharge pin to 0V
    pinMode(2, OUTPUT);        // enable capacitor discharge pin
    while(analogRead(7) < 30); // discharge until signal back in range
    pinMode(2, INPUT);         // disable capacitor discharge pin
  }
   
  delay(10);                   // 10 ms delay limits sampling rate
  count= count+1;
  if(count==500) //every 10 seconds
  {
    bpm= beats*12;
    beats=0;
    count=0;
  }
  lcd.setCursor(0,1);
  lcd.print(".");
  number_show= String(bpm);
  lcd.setCursor(0,1);
  lcd.print(disp);
}
