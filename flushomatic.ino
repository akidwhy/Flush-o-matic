// Flush-o-matic
// This arduino code uses a 28BYJ-48 stepper motor.
// A button and a driver for the stepper motor is needed.
////////////////////////////////////////////////


#define trigPin 3
#define echoPin 2

const int buttonPin = 4; //push button attached to this pin

int buttonState = LOW; //this variable tracks the state of the button, low if not pressed, high if pressed

long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 50;    // the debounce time; increase if the output flickers

long duration, distance;

bool timer_result = false;
int time_count = 0;

//declare variables for the motor pins
int motorPin1 = 8;    // Blue   - 28BYJ48 pin 1
int motorPin2 = 9;    // Pink   - 28BYJ48 pin 2
int motorPin3 = 10;    // Yellow - 28BYJ48 pin 3
int motorPin4 = 11;    // Orange - 28BYJ48 pin 4
                        // Red    - 28BYJ48 pin 5 (VCC)

int motorSpeed = 1200;  //variable to set stepper speed
int count = 0;          // count of steps made
int countsperrev = 356; // number of steps per full revolution. 512 is full rotation
int lookup[8] = {B01000, B01100, B00100, B00110, B00010, B00011, B00001, B01001};

//////////////////////////////////////////////////////////////////////////////
void setup() {
  //declare the motor pins as outputs
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);
  
  pinMode(buttonPin, INPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  Serial.begin(9600);
}

//////////////////////////////////////////////////////////////////////////////
void loop(){
  //sample the state of the button - is it pressed or not?
  buttonState = digitalRead(buttonPin);

  if ( (millis() - lastDebounceTime) > debounceDelay) {
    if (buttonState == HIGH) {
      rotate();
      lastDebounceTime = millis(); //set the current time
    }
  }
  digitalWrite(trigPin, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); // Added this line
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;
  Serial.print(distance);
  Serial.println(" cm 1");

  if(distance >= 13 && distance <= 40){
    timer_result = timer();
  }
  if(distance > 40 && timer_result == true){
    rotate();
    digitalWrite(motorPin1, LOW);
    digitalWrite(motorPin4, LOW);
    timer_result = false;
    delay(30000);
  }
  
  
}

bool timer(){
  while(distance >= 13 && distance <= 40){
    digitalWrite(trigPin, LOW);  // Added this line
    delay(1000); // Added this line
    digitalWrite(trigPin, HIGH);
    delay(1000); // Added this line
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    distance = (duration/2) / 29.1;
    Serial.print(distance);
    Serial.print(" cm 2");
    time_count++;
    Serial.print(" Count:");
    Serial.println(time_count);
    if(distance > 40){
      int count_whileLoop = 0;
      while (count_whileLoop < 5){
        digitalWrite(trigPin, LOW);  // Added this line
        delay(1000); // Added this line
        digitalWrite(trigPin, HIGH);
        delay(1000); // Added this line
        digitalWrite(trigPin, LOW);
        duration = pulseIn(echoPin, HIGH);
        distance = (duration/2) / 29.1;
        Serial.print(distance);
        Serial.print(" cm 3");
        if(distance < 40){
          time_count++;
        }
        count_whileLoop++;
        Serial.print(" Count:");
        Serial.println(count_whileLoop);
      }
    }
    if(time_count >= 6 && distance > 40){
      time_count = 0;
      return true;
    }
  }
  
  time_count = 0;
  return false;
}

///////////////////////////////////////////////////////////////////////////////
void rotate(){
  while(count < countsperrev ){
        //Serial.print("1Count: ");
        //Serial.println(count);
        clockwise();
        count++;
      }
  delay(3500);
  count = 0;
  while(count < countsperrev ){
    //Serial.print("2Count: ");
    //Serial.println(count);
    anticlockwise();
    count++;
  }
  count = 0;
}

//////////////////////////////////////////////////////////////////////////////
//set pins to ULN2003 high in sequence from 1 to 4
//delay "motorSpeed" between each pin setting (to determine speed)
void anticlockwise()
{
  for(int i = 0; i < 8; i++)
  {
    setOutput(i);
    delayMicroseconds(motorSpeed);
  }
}

void clockwise()
{
  for(int i = 7; i >= 0; i--)
  {
    setOutput(i);
    delayMicroseconds(motorSpeed);
  }
}

void setOutput(int out)
{
  digitalWrite(motorPin1, bitRead(lookup[out], 0));
  digitalWrite(motorPin2, bitRead(lookup[out], 1));
  digitalWrite(motorPin3, bitRead(lookup[out], 2));
  digitalWrite(motorPin4, bitRead(lookup[out], 3));
}