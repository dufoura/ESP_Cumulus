#include <FS.h>
/****Maj vers serveur****/
//#include <Time.h>
long sendInterval= 3600000; //in millis
long lastInterval= 0;
long actual_time=0;
boolean first_co=false;
/**************************/
// Date and time functions using a DS1307 RTC connected via I2C and Wire lib
#include <Wire.h>
#include "RTClib.h"
RTC_DS1307 rtc;
DateTime now;

int hour_on=99;
int min_on=99;
int hour_off=99;
int min_off=99;
/******OTA************/

/**********EEPROM************/
#include <EEPROM.h>
int addr = 0; 
int size_buf=30;
int i=0;
/*****************************/

/***************Wifi configuration*****************/
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

char IP_rasp[13]="192.168.1.47";
String ssid;
String password;

String espName		= "esp01";
String rel1Name="capteur1";
ESP8266WebServer	server(80);

const char* APssid = "ESPap";
const char* APpassword = "123456789";

int init_esp=0;

const int	httpPort		= 80;

String host		= "192.168.1.47";

String ClientIP;
// send data
WiFiClient client;
char inByte;
/************************/

/***specifique switch***/
int id=0;
String id_str="000";
boolean auth_rel1=false; 
boolean auth_rel2=false;	
int pin_rel1=14;//pin cumulus
int pin_rel2=12;

boolean flag_data=false;
/************************/
void setup(void)
{
  EEPROM.begin(512);
  /*******specifique switch********/
  //Récupération des paramères dans l'EEPROM en cas de coupure. 
  auth_rel1=(boolean) EEPROM.read(1);
  hour_on=(boolean) EEPROM.read(175);
  min_on=(boolean) EEPROM.read(176);
  hour_off=(boolean) EEPROM.read(177);
  min_off=(boolean) EEPROM.read(178);
  
  pinMode(pin_rel1, OUTPUT);
  digitalWrite(pin_rel1, auth_rel1);
  /*****************************/
  Serial.begin(115200);
  //Permet d'eviter des erreurs : 
  ssid.reserve(size_buf);
  password.reserve(size_buf);
  espName.reserve(size_buf);

  id=EEPROM.read(0);//en fonction de la valeur ID, on se connecte en point local wifi, ou sur le réseau
  id_str=String(id);
  Serial.print("satellite ID : ");
  Serial.println(id);
  if(id==0 || id==255) init_esp=0;
  else init_esp=1;

  if(init_esp==0)//Demarrage en Access point
  {
    WiFi.softAP(APssid,APpassword);
    delay(100);
    Serial.print("IP softAP address: ");
    Serial.println(WiFi.softAPIP());
  }
  else //demarrage appairage station
  {
     first_co=true;
    ssid=read_StringEE(3);
    password=read_StringEE(35);
    espName=read_StringEE(70);
    rel1Name=read_StringEE(105);
    Serial.print("local network : ");
    Serial.println(ssid);
    WiFi.begin(ssid.c_str(), password.c_str());
    i=0;
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
      i++;
      if(i>40)
      {
      Serial.println("no wifi available...");
      //send_notification("info","Connection issue : please move your sat closer");
      //ESP.restart();
      break;
      }
    } 
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

	/*******Connexion avec la raspberry pour maj donn�e*******/
	// This will send the request to the server
	//send_data_html(false);

	}
    if (!SPIFFS.begin())
  {
    Serial.println("Failed to mount file system");
    return;
  }

  server.on("/config_local_network", config_local_network);

  server.on("/action", handle_action);

  server.on("/edit", send_data_handle);

  server.on("/data", update_data);
  server.on("/data_sat.json", send_data_json);
  
  server.begin();
  Serial.println("HTTP server started");
/***************************************************/ 
//Initialisation module RTC : 
   if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  //A décommenter la première fois pour initialiser le module RTC.
  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////           MAIN 
void loop(void)
{
server.handleClient();     
/******specifique switch*******/
//Récupération de l'heure : 
 now = rtc.now();


//Si correspond à l'heure d'allumage : 
if(hour_on==now.hour() && min_on==now.minute())
{
  auth_rel1=true;
  Serial.println("Allumage rel1");   
}
//Si correspond à l'heure d'extinction : 
if(hour_off==now.hour() && min_off==now.minute())
{
  auth_rel1=false;   
  Serial.println("Extinction rel1");
}

if(first_co)
{
  first_co=false;
  send_data_html(false);
}
  
if(auth_rel1) digitalWrite(pin_rel1, HIGH);
else digitalWrite(pin_rel1, LOW);

 delay(100);
/***************************/
//Caractère à envoyer sur le port série permettant de réinitialiser l'ESP.
 if (Serial.available() > 0) {
    // get incoming byte:
    inByte = Serial.read();
    if(inByte=='R') //Reinitialisation de l'esp en cas de caract�re R envoye
    {    
    Serial.println("reinitialisation appairage");
    EEPROM.write(0, 0);//Flag pour connexion directe au wifi local
    EEPROM.commit();
    }
    if(inByte=='I') //Reinitialisation de l'esp en cas de caract�re R envoye
    {
      EEPROM.write(71, 0);    
    EEPROM.write(70, 0);
      EEPROM.commit();
    }
    if(inByte=='T') //Reinitialisation de l'esp en cas de caract�re R envoye
    {
    Serial.print("time now : ");
    Serial.print(now.hour(), DEC);
    Serial.print('h');
    Serial.print(now.minute(), DEC);
    Serial.print('m');
    Serial.println(now.second(), DEC);
    }
 }
} 








