
#include <Servo.h>
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>


#define Firebase_Host "https://iot-feeder-1d737-default-rtdb.asia-southeast1.firebasedatabase.app/" //host link from the fire base
#define Firebase_Auth "E3ZIcq4fdZaPuihuSBX0yBLxNlpnLW5BNGwfZBYf" //databse secret. Path projrct setting (gear icon near project overview)->sevice account->Data secret-> show secret


const char* ssid = "kirthik"; // SSID (name) of your wifi or mobile hotspot
const char* password = "987654321"; //password for the wifi
const int trigPin = D6;
const int echoPin = D7;
long duration;
int distance=0;
const int sensorPin = A0;
int ledPin = 4;


// Variables for storing the sensor data
int waterLevel = 0;
FirebaseData firebaseData; //object for the firebasedata

FirebaseJson json; //object for firebase json

Servo servo; // object for servo

WiFiServer server(80);


void setup()
{
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(sensorPin, INPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
servo.attach(5); //D1
servo.write(0);
delay(2000);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());
  
  Firebase.begin(Firebase_Host, Firebase_Auth);
  Firebase.reconnectWiFi(true);
}
void loop()
{
  //Below code for calculate distance using US sensor
  delay(1000);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // measure the duration of the echo pulse in microseconds
  duration = pulseIn(echoPin, HIGH);

  // calculate the distance in centimeters
  distance = duration*0.034/2; //Final variable for the distance

  if(1){
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm     ");
  Serial.println("Food is very LOW");
  
}

// Below code is for calculate water level using WL sensor
waterLevel = analogRead(sensorPin)-5; //Final variable for water level

  if(waterLevel<100){
  Serial.print("Water Level: ");
  Serial.println(waterLevel);
  }

  delay(1000);

  //Upload the value of distance and water level to the firebase
  Firebase.setString(firebaseData, "ultra value/us", distance); //First create a Variale ultra value in the firebase project
  Firebase.setString(firebaseData, "ultra value/lvl", waterLevel);// "us" and "lvl" are the variables in firebase (automatically they will create)

  // code to control servo motor
  if(Firebase.getString(firebaseData, "/ultra value/user_name")) //check the value available or not
  {
    String servostatus = firebaseData.stringData(); 
    if(servostatus.toInt() == 0){     //check the input is '0'. if '0' the motor will run
      servo.write(180);

delay(2000);

servo.write(0);
delay(1000);

    }

    //code to control water pump using relay
    if(Firebase.getString(firebaseData, "/ultra value/Water_pump"))//check the value available or not
  {
    String waterstatus = firebaseData.stringData();
    if(waterstatus.toInt() == 0){   //check the input is '0'. if '0' the motor will run
      digitalWrite(ledPin, LOW);

    }
    else{
      digitalWrite(ledPin, HIGH);
    }

  }
  }
}
