const int ledPin = 9; //PWM pin for LED fading
const int pirPin1 = 2;  //digital input1
const unsigned long motionDelay = 30; //the number of seconds the LEDs stay on if motion is detected
const int fadeOnDelay = 40;// millisecs delay to make fading smoother
const int fadeOffDelay = 50; // millisecs delay to make fading smoother
const int fadeOffAddDelay = 30; // additional delay in millisecs added to fadeOffDelay at the end of fading (last 20 fades of 255)
unsigned long currentMillis = 0;  //used to store ms since system was turned on
unsigned long prevMillis = 0;  //used to count elapsed time
bool ledState = false;  //stores whether the LEDs are on or off
bool pirSensSig = HIGH; // motion is detected when pir sensor returns HIGH state

void setup() 
{
  //Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(pirPin1, INPUT);
//  pinMode(pirPin2, INPUT);
}

void loop() 
{
  //Serial.println("Starting motion detect");
  delay(500);  //wait half a second after turning off LEDs before starting motion detection
  motionRead(); //starts function for monitoring motion from the PIR sensor
}


/*function to read LED state and fade on or off accordingly*/
bool fadeLED(bool posInc)
{
  bool ledState;
  int ledVal, incVal, fadeDelay;
  if(posInc)
  {
    ledVal = 0; //LEDs starts off and will fade on in for loop
    incVal = 1; //used to increase brightness
    ledState = true;
    fadeDelay = fadeOnDelay;
    //Serial.println("Fading on");
  }
  else
  {
    ledVal = 255; //LEDs starts on and will fade off in for loop
    incVal = -1;  //used to decrease brightness
    ledState = false;
    fadeDelay = fadeOffDelay;
    //Serial.println("Fading off");
  }
  for(int i = 0; i <= 255; i++)  //loop to fade LEDs on and off
  {
    analogWrite(ledPin, ledVal);
    ledVal += incVal;
    if((digitalRead(pirPin1) == pirSensSig) && (ledState == false)) //if motion is detected while fading off, start fadin on
    {
      incVal = 1;
      ledState = true;
      i = ledVal;
      fadeDelay = fadeOnDelay;
    }
    if ((ledState == false ) && (i >= 235))
    {
      delay(fadeOffAddDelay);
      //Serial.println("slow");
    }
    delay(fadeDelay);
  }
  return ledState;  //returns whether lights are on or off
}

/*function to read motion sensor and turn on lights accordingly*/
void motionRead()
{
  while(true) //loop
  {
    currentMillis = millis(); //stores current time in ms
  //Serial.println(digitalRead(pirPin1));
    if(digitalRead(pirPin1) == pirSensSig) //if motion is detected
    {
      prevMillis = currentMillis; //counter is reset each time motion is detected regardless of whether LEDs are on or off
      //Serial.println("Detecting motion");
      if(!ledState) //turns on LEDs only if motion was just detected and LEDs are already off
      {
        //Serial.println("Detecting motion and turning on lights");
        ledState = fadeLED(HIGH); //increments the LEDs on and stores state of LEDs
      }
    }
    delay(200);
    if(((currentMillis - prevMillis) >= (motionDelay*1000)) && ledState)  //if it's been the set number of seconds since motion has been detected and LEDs are on
    {
      ledState = fadeLED(LOW); //fades LEDs off and stores state of LEDs
      if (ledState == true) // if motion was detected meanwhle
      {
        prevMillis = currentMillis;
      }
      //Serial.println("No more motion");
    }
    //Serial.println(currentMillis - prevMillis);
  }
}
