#include <Adafruit_NeoPixel.h>   
#include <Servo.h>
#include <cstdlib>  
#include <ctime>  

#define TRIG_PIN1 3
#define ECHO_PIN1 4
#define TRIG_PIN2 5
#define ECHO_PIN2 7
#define TRIG_PIN3 9
#define ECHO_PIN3 8
#define FOR_PIN 10
#define BACK_PIN 11
#define SPEAKER_PIN 13
#define ANALOG_PIN A0
#define LED_PIN 6 
#define NUM_LEDS 8 

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);
Servo myServo;

int duration1=0;
int duration2=0;
int duration3=0;
int distance1;
int distance2;
int distance3;
int currentSpeed=0;
bool closeDis = false;
bool reversing = false;



void setup()
{
  Serial.begin(9600);

  pinMode(TRIG_PIN1, OUTPUT);
  pinMode(ECHO_PIN1, INPUT);
  pinMode(TRIG_PIN2, OUTPUT);
  pinMode(ECHO_PIN2, INPUT);
  pinMode(TRIG_PIN3, OUTPUT);
  pinMode(ECHO_PIN3, INPUT);
  pinMode(SPEAKER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(FOR_PIN, OUTPUT);
  pinMode(BACK_PIN, OUTPUT);

  myServo.attach(12);  
  myServo.write(84);
  strip.begin();
  starter1();
  strip.show();
}



void loop()
{
   sendSignalUltra();
   servoLogic();
   delay(5);
}


void sendSignalUltra()
{
  // Trigger the first ultrasonic sensor 1
  digitalWrite(TRIG_PIN1, LOW);
  delayMicroseconds(3);
  digitalWrite(TRIG_PIN1, HIGH);
  delayMicroseconds(8);
  digitalWrite(TRIG_PIN1, LOW);

  // Measure the pulse width on the echo pin for the first sensor
  duration1 = pulseIn(ECHO_PIN1, HIGH);
  
   delay(10); // Brief delay to avoid conflict with the second sensor

  // Trigger the second ultrasonic sensor 2
  digitalWrite(TRIG_PIN2, LOW);
  delayMicroseconds(3);
  digitalWrite(TRIG_PIN2, HIGH);
  delayMicroseconds(8);
  digitalWrite(TRIG_PIN2, LOW);

  // Measure the pulse width on the echo pin for the second sensor
  duration2 = pulseIn(ECHO_PIN2, HIGH);

   delay(30); // Brief delay to avoid conflict with the second sensor

  // Trigger the second ultrasonic sensor 3
  digitalWrite(TRIG_PIN3, LOW);
  delayMicroseconds(3);
  digitalWrite(TRIG_PIN3, HIGH);
  delayMicroseconds(8);
  digitalWrite(TRIG_PIN3, LOW);

  // Measure the pulse width on the echo pin for the second sensor
  duration3 = pulseIn(ECHO_PIN3, HIGH);
  
}


void servoLogic()
{
  int dayOrnight = analogRead(ANALOG_PIN);
  int brightness = map(dayOrnight,0,404,10,200);
  int headlight = 200 - brightness;
  strip.setBrightness(headlight); 

  //Check and print the first sensor's distance
  if (duration1 == 0)
    {
     Serial.println("No echo s1");
    } 
  else
    {
     float distance1 = (duration1 * 0.034) / 2;
     Serial.print("S1 Dis: ");
     Serial.print(distance1);
     Serial.println("cm");
    }
  distance1 = (duration1 * 0.034) / 2; 

  // Check and print the second sensor's distance
  if (duration2 == 0) 
    {
    Serial.println("No echo s2");
    } 
  else   
    {
     float distance2 = (duration2 * 0.034) / 2;
     Serial.print("S2 Dis: ");
     Serial.print(distance2);
     Serial.println("cm");
    }
  distance2 = (duration2 * 0.034) / 2;

 // Check and print the third sensor's distance
  if (duration3 == 0) 
    {
     Serial.println("No echo detected for Sensor 3");
    } 
  else 
    {
     float distance3 = (duration3 * 0.034) / 2;
     Serial.print("S3 Dis: ");
     Serial.print(distance3);
     Serial.println("cm");
    }
  distance3 = (duration3 * 0.034) / 2;

  frontBlock();
  strip.show();
 }                   




void frontBlock()
{
    if(distance3 <= 42)
     {
        //All path are blocked so there is not way to go. Stop the car. No movement.//working perfectly
        if(distance3 <=24 && distance1 <=15 && distance2 <=15 )
        {
          //Red light stop no movement
          for (int i = 0; i < NUM_LEDS ; i++) 
           {
           strip.setPixelColor(i, strip.Color(0,0,0));
           } 
          for (int i = 0; i < NUM_LEDS ; i++) 
           {
           strip.setPixelColor(i, strip.Color(255,0,0));
           } 
          tone(SPEAKER_PIN , 1500, 1000); 
          Serial.print("No path !!!!!!!!!!!!!"); 
          Serial.print("\n"); 
          analogWrite(FOR_PIN,0);
          analogWrite(BACK_PIN,0);
          currentSpeed=0;
          closeDis = true;
          reversing = false;
        }
        //Front is only block car can move left and right. So reversign the car and moving left and right.
        else if(distance3 <= 28 && (distance1 >= 15 || distance2 >= 15) )
        {
          //stop the car for 2 second
          //delay(3000);
          //Green light while reversing
          for (int i = 0; i < NUM_LEDS ; i++) 
           {
           strip.setPixelColor(i, strip.Color(0,0,0));
           } 
          for (int i = 0; i < NUM_LEDS ; i++) 
           {
            strip.setPixelColor(i, strip.Color(0,255,0));
           } 
         
          if(distance1 > distance2)
           {
           //steer left
           myServo.write(120);
           }
          else if(distance2 > distance1 )
           {
           //steer right
           myServo.write(50);
           }
          else
           {
            myServo.write(84);
           }
           
           Serial.print("Reverse operation"); 
           Serial.print("\n"); 
           Serial.print("Backstarter");
           Serial.print("\n");
           analogWrite(FOR_PIN,0);
           analogWrite(BACK_PIN,130);
 
           currentSpeed=0;
           closeDis = true;
           reversing = true;
           tone(SPEAKER_PIN , 1000, 500); 
        }
        //Front is block by object with distance less than 40 cm and more than 30 cm
        else if(distance3 <= 40 && (distance1 >= 15 || distance2 >= 15))
        {
          //stop the car and take decision
       
           Serial.print("stopped distance between 48 and 28"); 
           Serial.print("\n"); 
          currentSpeed=0;
          closeDis = true;
          reversing = false;
          tone(SPEAKER_PIN , 1500, 1000); 
          //decided turn left or right
         if(distance1 > distance2)
          {
          
           for (int i = 3; i < 8  ; i++) 
              {
               strip.setPixelColor(i, strip.Color(128, 128, 128));
              }
           for (int i = 0; i < 3 ; i++) 
              {
               strip.setPixelColor(i, strip.Color(0, 0, 0));
              }
             
             Serial.print("steer left it is empty and front block"); 
             Serial.print("\n"); 
             myServo.write(50);
             analogWrite(FOR_PIN,110);
             analogWrite(BACK_PIN,0);
         
             currentSpeed=0;
        
          }
         else if (distance1 < distance2)
          {
      
            for (int i = 0; i < 5; i++) 
             {
             strip.setPixelColor(i, strip.Color(128, 128, 128));
             }
            for (int i = 5; i < 8 ; i++) 
             {
             strip.setPixelColor(i, strip.Color(0, 0, 0));
             }
             Serial.print("steer right it is empty and front block"); 
             Serial.print("\n"); 
             myServo.write(120);
             analogWrite(FOR_PIN,110);
             analogWrite(BACK_PIN,0);
             currentSpeed=0;
           
          }
         else
         {
           srand(time(0));
           int randomNumber = rand() % 2;
           if(randomNumber == 1)
           {
         
             for (int i = 0; i < 2; i++) 
              {
               strip.setPixelColor(i, strip.Color(255, 0, 0));
              }
              for (int i = 2; i < 5 ; i++) 
              {
               strip.setPixelColor(i, strip.Color(0, 255, 0));
              }
              for (int i = 5; i < 8 ; i++) 
              {
               strip.setPixelColor(i, strip.Color(0, 0, 255));
              }
     
             Serial.print("steer left it is empty and front block"); 
             Serial.print("\n"); 
             myServo.write(50);
             analogWrite(FOR_PIN,110);
             analogWrite(BACK_PIN,0);
             currentSpeed=0;
           }
            else
            { 
           
              for (int i = 0; i < 2; i++) 
              {
               strip.setPixelColor(i, strip.Color(255, 0, 0));
              }
              for (int i = 2; i < 5 ; i++) 
              {
               strip.setPixelColor(i, strip.Color(0, 255, 0));
              }
              for (int i = 5; i < 8 ; i++) 
              {
               strip.setPixelColor(i, strip.Color(0, 0, 255));
              }
              Serial.print("steer right it is empty and front block"); 
              Serial.print("\n"); 
              myServo.write(120);
              analogWrite(FOR_PIN,110);
              analogWrite(BACK_PIN,0);
              currentSpeed=0;
            }
          }  
        }
      }
    else
     {
        closeDis = false;
        frontOpen();
     } 

}



void frontOpen()
{
  if (distance3 > 42)
     {
      //If the oject is at close distance
      if( distance3 < 90)
        {
          Serial.print(currentSpeed);
          Serial.print("\n");
          if(distance1 < 50)
            {
              if(currentSpeed==0)
               {
                 analogWrite(FOR_PIN,120);
                 analogWrite(BACK_PIN,0);
               }

             for (int i = 0; i < 5; i++) 
               {
                strip.setPixelColor(i, strip.Color(255, 255, 0));
               }
             for (int i = 6; i < 8 ; i++) 
               {
                strip.setPixelColor(i, strip.Color(0, 0, 0));
               }
              Serial.print("Steer right obj close");
              Serial.print("\n"); 
              int leftRadius = 50 - distance1;
              int movingAngle = map(leftRadius,0,50,100,120);
              myServo.write(movingAngle);
              analogWrite(FOR_PIN,110);
              analogWrite(BACK_PIN,0);
          
              currentSpeed = 90;
            }
           else if(distance2 < 50 )
            {
              if(currentSpeed==0)
                {
                 analogWrite(FOR_PIN,120);
                 analogWrite(BACK_PIN,0);
                }
     
              for (int i = 3; i < 8  ; i++) 
                {
                 strip.setPixelColor(i, strip.Color(255, 255, 0));
                }
              for (int i = 0; i < 2 ; i++) 
                {
                 strip.setPixelColor(i, strip.Color(0, 0, 0));
                }
              Serial.print("steer left obj close"); 
              Serial.print("\n"); 
              int rightRadius = 50 - distance2;
              int movingAngle = map(rightRadius,0,50,70,50);
              myServo.write(movingAngle);
              analogWrite(FOR_PIN,110);
              analogWrite(BACK_PIN,0);

              currentSpeed = 85;
             }
           else //(distance1 > 50 && distance2 > 50 )
            {
              if(currentSpeed==0)
              {
                 analogWrite(FOR_PIN,120);
                 analogWrite(BACK_PIN,0);
              }
               for (int i = 0; i < 2 ; i++) 
                {
                strip.setPixelColor(i, strip.Color(128, 0, 128));
                }
               for (int i = 2; i < 6 ; i++) 
                {
                strip.setPixelColor(i, strip.Color(128, 0, 128));
                }
               for (int i = 6; i < 8 ; i++) 
                {
               strip.setPixelColor(i, strip.Color(128, 0, 128));
                }
              Serial.print("slow speed"); 
              Serial.print("\n"); 
              myServo.write(84);
              
              analogWrite(FOR_PIN,72);
              analogWrite(BACK_PIN,0);
              currentSpeed=72;
              closeDis = true;
   
            }
         }
        //There is no obj at all
        else
        {
          if(currentSpeed==0)
          {
           starter1();
          }

         if(distance1 < 50 )
          {    
            
           for (int i = 0; i < 3; i++) 
            {
             strip.setPixelColor(i, strip.Color(0, 0, 255));
            }
           for (int i = 5; i < 8 ; i++) 
            {
             strip.setPixelColor(i, strip.Color(0, 0, 0));
            }
            Serial.print("Steer right no obj");
            Serial.print("\n"); 
            int leftRadius = 50 - distance1;
            int movingAngle = map(leftRadius,0,50,100,120);
            myServo.write(movingAngle);
            delay(50);
            analogWrite(FOR_PIN,78);
            analogWrite(BACK_PIN,0);
            currentSpeed = 78;
         }
         else if(distance2 < 50 )
            {
                   
                for (int i = 5; i < 8  ; i++) 
                  {
                  strip.setPixelColor(i, strip.Color(0, 0, 255));
                  }
                for (int i = 0; i < 3 ; i++) 
                  {
                    strip.setPixelColor(i, strip.Color(0, 0, 0));
                  }
              Serial.print("steer left no obj "); 
              Serial.print("\n"); 
              int rightRadius = 50 - distance2;
              int movingAngle = map(rightRadius,0,50,70,50);
              myServo.write(movingAngle);
              delay(50);
              analogWrite(FOR_PIN,78);
              analogWrite(BACK_PIN,0);
              currentSpeed = 78;
             }
          else //(distance1 > 60 && distance2 > 60 )
            {
              if(currentSpeed==0)
              {
               starter1();
              }
              
             for (int i = 0; i < 3 ; i++) 
               {
                strip.setPixelColor(i, strip.Color(255, 255, 255));
               }
             for (int i = 3; i < 5 ; i++) 
               {
                strip.setPixelColor(i, strip.Color(255, 255 , 255));
               }
             for (int i = 5; i < 8 ; i++) 
               {
                strip.setPixelColor(i, strip.Color(255, 255 , 255));
               }
              myServo.write(84);
              Serial.print("Maxspeed");
              Serial.print("\n");
              analogWrite(FOR_PIN,87);
              analogWrite(BACK_PIN,0);
              currentSpeed = 87;
       
           }
        }
    }
}



//Starters
void starter1()
{
  Serial.print("Starter 1");
  Serial.print("\n");
  int accerlation = 80;
  analogWrite(FOR_PIN,150);
  analogWrite(BACK_PIN,0);
  delay(200);

  for(int i=0;i<10;i++)
   {
    accerlation = accerlation - i;  
    analogWrite(FOR_PIN,accerlation);
    analogWrite(BACK_PIN,0);
   }
}


