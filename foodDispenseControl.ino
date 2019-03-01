#include <Servo.h>
#include <DS3231.h>

Servo myservo;
DS3231  rtc(SDA, SCL); // Init the DS3231 using the hardware interface
Time t;

int feedingHours[] = {6, 00, 00}; //using 24hr format
int feedingMins[] =  {14, 00, 00}; 
int feedingSec[] =   {22, 00, 00};

bool rotateForward = false;

int serving = 0;
int angle = 0;
 
void setup() {
  Serial.begin(9600);  // Setup Serial connection, must be 9600  
  rtc.begin(); // Initialize the rtc object  
  
  myservo.attach(9); // attaches the servo on pin 9 to the servo object
  myservo.write(0); //set servo to origin  
    
//  Uncommented to set the date and time
//  rtc.setDOW(THURSDAY);        // Set Day-of-Week
//  rtc.setTime(10, 24, 00);     // Set the time to 12:00:00 (24hr format)
//  rtc.setDate(1, 17, 2019);    // Set the date to January 1st, 2014
}

void loop() {
  Serial.print("Current time: ");
  Serial.println(rtc.getTimeStr());  
  delay(1000);

  Serial.println("Next serving: " + getNextAlarm(serving));
    
  if(checkAlarm(serving)){
    if(!rotateForward){
        angle+=90;
        serving++;
                        
        myservo.write(angle);
        Serial.println("Serving #" + String(serving));
        Serial.println("rotate servo to angle " + String(angle));
      
        rotateForward = !rotateForward;
      }
    
    else{
        angle-=90;
        serving++;
        
        myservo.write(angle);
        Serial.println("Serving #" + String(serving));
        Serial.println("rotate to angle " + String(angle));        
        
        rotateForward = !rotateForward;
      }

    //reset serving count    
    if(serving == 3){serving = 0;}  
  }      
}

bool checkAlarm(int x){
  t = rtc.getTime();
  return t.hour == feedingHours[x] && t.min == feedingMins[x] && t.sec == feedingSec[x];
}

String getNextAlarm(int x){
    return String(feedingHours[x]) + ":" + String(feedingMins[x]) + ":" + String(feedingSec[x]);
  }
