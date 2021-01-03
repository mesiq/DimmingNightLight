const int pirPin = 8;  //PIR sensor pin (digital)
const int ledPin = 9; //LED pin (digital PWM)
const int photorPin = A0; //photoresistor pin (analogue)

const int lightDuration = 45; //the number of seconds the LED stays on if motion is detected
const int offDelay = 3; //the number of seconds to ignore motion after turning them off due to detection of light
const int onDelay = 0; //the number of seconds to ignore motion delay after first detection; if light will be detected after this time, LED won't be turned on
const int darkTreshold = 400; //light treshold to accept motion detection
const int fadeOnDelay = 10;// milliseconds between fading values to make fading smoother
const int fadeOffDelay = 30; // milliseconds between fading values to make fading smoother
const int fadeOffAddDelay = 50; // additional delay in millisecs added to fadeOffDelay at the end of fading (last 20 fades of 255) to make final fadeoff slower

unsigned long currentMillis = 0;  //used to store ms since system was turned on
unsigned long prevMillis = 0;  //used to count elapsed time

bool ledState = false;  //stores state of the LED: on - true; off - false
bool pirSensSig = HIGH; // motion is detected when pir sensor returns HIGH/LOW state

void setup() //one-time run before loop function
{
  //Serial.begin(9600); //uncomment if you want/need to use serial debugger via Arduino IDE
  pinMode(ledPin, OUTPUT);
  pinMode(pirPin, INPUT);
  pinMode(photorPin, INPUT);
  //Serial.println("Motion detection initiated");
}

void loop() // built-in infinitive loop function
{
  currentMillis = millis(); //stores current processor run time (not a real time)
  //Serial.println(analogRead(photorPin)); //debugging photoresistor input
  //Serial.println(digitalRead(pirPin)); //debugging PIR Sensor input

  if (dark()) //if it's dark
  {
    if (motion()) //if motion was detected
    {
      //Serial.println("Motion detected");
      prevMillis = currentMillis; //reset timeout timer
      if (!ledState) //if LED is off
      {
        delay(onDelay * 1000UL); //wait for defined time
        if (dark())  //if it is still dark after defined delay
        {
          ledState = fadeLED(true); //fade LED on
        }
      } else //if motion is detected, but LED is already on
      {
        delay(200); //add 0.2sec delay - it's not really necessary to make motion detection dozen times a second
      }
    } else if ((currentMillis - prevMillis) >= (lightDuration * 1000UL) && ledState)  //if defined timeout runs out (since motion has been detected) and LED are still on, let's start fading them off
    {
      //Serial.println("Motion not detected for defined time, turning LED off");
      ledState = fadeLED(false); // initiate LED fade off and store the final state of LED as during fade off there might be motion detected and we will fade LED back on
      if (ledState) //if motion was detected during fade off and result is LED is on
      {
        //Serial.println("Motion detected during fade off, reseting timer");
        prevMillis = currentMillis;
      }
    } else //if it's dark AND motion was detected AND (LED is off OR timeout didn't run out)
    {
      delay(200);
    }
  } else if (ledState) //if it is not dark and LED is on
  {
    //Serial.println("Enough light detected, turning off LED");
    ledState = false;
    analogWrite(ledPin, 0);
    delay(offDelay * 1000UL);
  } else //if it is not dark and LED is off
  {
    delay(200);
  }
}

/*function to read photoresistor state*/
bool dark()
{
  return (analogRead(photorPin) < darkTreshold); //reads photoresistor value and compares it to treshold to evaluate light conditions
}

bool motion ()
{
  return(digitalRead(pirPin) == pirSensSig); //reads motion sensor value and compares it to decalred on-state (depending on sensor it can be high or low)
}

/*function to read LED state and fade on or off accordingly*/
bool fadeLED(bool ledState)
{
  bool returnState;
  int ledVal, incVal, fadeDelay;
  if (ledState)  //LED is supposed to fade on
  {
    ledVal = 0; //initial LED value (off)
    incVal = 1; //used to increase brightness
    returnState = true; //set logical value of LED to on
    fadeDelay = fadeOnDelay;
    //Serial.println("Initiating fading on");
  } else //LED are on and we will initiate fade off
  {
    ledVal = 255; //initial LED value (full brightness)
    incVal = -1;  //used to decrease brightness
    returnState = false; //set logical value of LED to off
    fadeDelay = fadeOffDelay;
    //Serial.println("Initiating fading off");
  }
  
  for(int i = 0; i <= 255; i++) //Start of fading on/off
  {
    if (dark()) // stop fading and turn LED off if there's enough light (regural light was turned on)
    {
      //Serial.println("Enough light, turning LED off, exiting fading loop");
      analogWrite(ledPin, 0);
      returnState = false;
      break;
    }
    
    analogWrite(ledPin, ledVal);
    ledVal += incVal;
    
    if((motion()) && (!returnState)) //if motion is detected while fading off, start fadin on
    {
      //Serial.println("Motion Detected during fade off, starting fade on");
      incVal = 1;
      returnState = true;
      i = ledVal;
      fadeDelay = fadeOnDelay;
    }
    
    delay(fadeDelay);
    if ((!returnState) && (i >= 235))  //add additinal fading smoothness for last 20 cycles
    {
      delay(fadeOffAddDelay);
      //Serial.println("Slowing down final fading");
    }
  }
  //Serial.println("Fading finished");
  return returnState;  //returns whether lights are on or off
}
