#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <Servo.h>

#define FIREBASE_HOST "carpark-cb140-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "zF8jnuNaxVICYzlTxLYyYbMPXySSgSQEJJD6M8pB"
#define WIFI_SSID "Carpark"
#define WIFI_PASSWORD "12344567"
#define block_dis 7 // cm
#define min_dis 15 // cm
#define car_dis 7 // cm
#define TRIGGER 12 // d6
#define ECHO 14 // d5
#define TRIGGER2 5 // d1
#define ECHO2 4 // d2
#define TRIGGER3 0 // d3
#define ECHO3 13 // d7
#define pin 15 //d8

//output d4 trigger - 2
//input rx eco - 3

Servo servo;

void setup() {
  
  
  Serial.begin(9600);
  pinMode(TRIGGER, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(TRIGGER2, OUTPUT);
  pinMode(ECHO2, INPUT);
  pinMode(TRIGGER3, OUTPUT);
  pinMode(ECHO3, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  servo.attach(pin);


  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

}


void loop() {

  bool sen_1_blocked = false;
  bool car_1_parked = false;
  bool sen_2_blocked = false;
  bool car_2_parked = false;
  bool incoming_car = false;
/************************************************/
  long duration, distance;
  digitalWrite(TRIGGER, LOW);
  delayMicroseconds(2); 
  
  digitalWrite(TRIGGER, HIGH);
  delayMicroseconds(10); 
  
  digitalWrite(TRIGGER, LOW);
  duration = pulseIn(ECHO, HIGH);
  
  distance = (duration/2) / 29.1; 
/************************************************/
   long duration2, distance2;
  digitalWrite(TRIGGER2, LOW);
  delayMicroseconds(2); 
  
  digitalWrite(TRIGGER2, HIGH);
  delayMicroseconds(10); 
  
  digitalWrite(TRIGGER2, LOW);
  duration2 = pulseIn(ECHO2, HIGH);
  
  distance2 = (duration2/2) / 29.1; 

 /************************************************/
  long duration3, distance3;
  digitalWrite(TRIGGER3, LOW);
  delayMicroseconds(2); 
  
  digitalWrite(TRIGGER3, HIGH);
  delayMicroseconds(10); 
  
  digitalWrite(TRIGGER3, LOW);
  duration3 = pulseIn(ECHO3, HIGH);
  
  distance3 = (duration3/2) / 29.1; 
  
  Serial.print("debug - distance1 :");
  Serial.println(distance);
  Serial.print("debug - distance2 :");
  Serial.println(distance2);
  Serial.print("debug - distance3 :");
  Serial.println(distance3);
  Serial.println("...........");
  Serial.println("...........");
  Serial.println("...........");
  Serial.println("...........");

  /*******************logic*********************/

    if( distance < block_dis )
        sen_1_blocked = true;
    else
        sen_1_blocked = false;

    if( distance2 < block_dis )
        sen_1_blocked = true;
    else
        sen_1_blocked = false;



    if( distance < min_dis )
        car_1_parked = true;
    else
        car_1_parked = false;

    if( distance2 < min_dis )
        car_2_parked = true;
    else
        car_2_parked = false;

    if( distance3 < car_dis )
        incoming_car = true;

    else
        incoming_car = false;

    /*******************sending logic****************/

    if( incoming_car == true){
      Firebase.setBool("incoming_car", true);
      if( car_1_parked == false && car_2_parked == false )
        servo.write(90);
    }
    else{
      Firebase.setBool("incoming_car", false);
      servo.write(0);
    }
      
    if( car_1_parked == true )
      Firebase.setBool("Car_parked_spot1", true);

    else
      Firebase.setBool("Car_parked_spot1", false);

    if( car_2_parked == true )
      Firebase.setBool("Car_parked_spot2", true);

    else
      Firebase.setBool("Car_parked_spot2", false);

  Firebase.setFloat("Distance", distance);
  Firebase.setFloat("Distance2", distance2);
  Firebase.setFloat("Distance3", distance3);

  // handle error
  if (Firebase.failed()) {
      Serial.print("debug- sending failed");
      Serial.println(Firebase.error());  
      return;
  }
  
 /*digitalWrite(LED_BUILTIN, HIGH);
 delay(500);
 digitalWrite(LED_BUILTIN, LOW);*/

 
 
/*
  // get value 
  Serial.print("number: ");
  Serial.println(Firebase.getFloat("number"));
  delay(1000);

  // remove value
  Firebase.remove("number");
  delay(1000);

  // set string value
  Firebase.setString("message", "hello world");
  // handle error
  if (Firebase.failed()) {
      Serial.print("setting /message failed:");
      Serial.println(Firebase.error());  
      return;
  }
  delay(1000);
  
  // set bool value
  Firebase.setBool("truth", false);
  // handle error
  if (Firebase.failed()) {
      Serial.print("setting /truth failed:");
      Serial.println(Firebase.error());  
      return;
  }
  delay(1000);

  // append a new value to /logs
  String name = Firebase.pushInt("logs", n++);
  // handle error
  if (Firebase.failed()) {
      Serial.print("pushing /logs failed:");
      Serial.println(Firebase.error());  
      return;
  }
  Serial.print("pushed: /logs/");
  Serial.println(name);
  delay(1000);
/*
  long duration, distance;
 
  
  digitalWrite(TRIGGER, LOW);  
  delayMicroseconds(2); 
  
  digitalWrite(TRIGGER, HIGH);
  delayMicroseconds(10); 
  
  digitalWrite(TRIGGER, LOW);
  duration = pulseIn(ECHO, HIGH);
  distance = (duration/2) / 29.1; // final distance in cm
  
  Serial.print(distance);
  Serial.println("Centimeter:");
  delay(1000);
  */

  
}
