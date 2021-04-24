/************************************************************************************
    
    <hot water tank control>
    Copyright (C) <2019>  <Fabrice BAUDIN>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

    info@innoreentech.fr

****************************************************************************************/


/*******************************
 *    utilities                *
 *******************************/

#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <wchar.h> 
#include <WString.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <ESP8266Ping.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266HTTPUpdateServer.h> // Becarefull it is necessary to add this timer1_disable(); in ccp file
    
#include <ArduinoJson.h>

#include <stdlib.h>
    
#include <EEPROM.h> 

#define MEM_NAME_MODULE     1   //Name of module
#define MEM_ADRESS_SERVER   22  // adress of server php
#define MEM_PORT_SERVER     43  // Port of server
#define MEM_PERIOD          50  // Period to send datas in seconds
#define MEM_SSID            70
#define MEM_PASSWORD        120 
#define MEM_POWER_HEATING   170
    
/* Configuration de l'écran */

#include <SPI.h>
#include <Wire.h>
#include <gpio.h>

/* oled screen setting */

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET   // GPIO0
Adafruit_SSD1306 display(-1);

#if (SSD1306_LCDHEIGHT != 48)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

int           time_screen_on;
byte          screen_on;

#define DELAY_SCREEN 30000        // x Sharpness, use pwm interupt

/* Pwm setting */

#define PERIOD       1020   // ms   
#define SHARPNESS    1   // in ms
#define PWM_PIN      12     //D6

int           i=0;
int           start_period;
int           end_signal;
int           heat_water;      
byte          out;     
byte          forced=0;
long          power_heating=3000;  // Ths is the power of the heat ballon

/* Pin setting */

#define PUSH_BUTTOM      D4      // push buttom
#define ENABLE_HEATING   D8 
  
#define PWM_PIN          D6      //Output pwm for ssr relay
#define LED              D0      //D0 output control

/* OneWire setting */

#include <OneWire.h>
#include <DallasTemperature.h>


#define ONE_WIRE_BUS          D7               
#define TEMPERATURE_PRECISION 10          

OneWire one_wire(ONE_WIRE_BUS);           //declaration of bus
DallasTemperature sensor(&one_wire);      //Localisation du bus 
DeviceAddress water_thermometer;

double   temperature_water;
double   temperature_setting;
byte     alarm=0;

/* Message linky traitement */

byte   statut_message=0;
#define MESSAGE_IN_READ 0
#define MESSAGE_READY   1
#define MESSAGE_ERROR   2
#define DATA_READY      3


String message="";
String etiquette="";
String donnee="";
byte   checksum=0;

/* Enedis status */

String            ptec;           // période tarifaire en cours
String            demain;         //couleur du lendemain
unsigned int      i_inst;         //intensité instantanée
int               index_i;
unsigned int      total_i;
double            p_app;          //puissance apparente
int               index_app;
unsigned int      total_app;
unsigned int      kwh;            //kilowatts heures calculer
String            h_hp_hc;        //Horaire heures pleines heures creuses
unsigned long     base;           //consommation pour un tarif de base
    
char              mode_linky[6];  //Etat du du compteur
unsigned int      i_sousc;        //Intensité souscrite
char              adresse[12];    //Adresse du compteur
unsigned int      op_tarif;

String            hour_statut="pleine";


/* push buttom filter */

int             push_buttom_start;
#define         PUSH_BUTTOM_DELAY   1    // number of cycle before to valid the buttom

/* Period Send  data  wifi information */

int             period;                   // period to sent datas at server
unsigned long   delay_sent;               // delay to send a new message in milliseconds
unsigned long   last_message;

char            name_module[20];          // name of module
String          ref_module;               // référence du module
String          version_module;           // version of module
char            ip_server[15];            // adress ip of the module
int unsigned    port_server;              // port of server
String          local_mac;                // adress mac of the module
String          local_ip;                 // adress ip of module
String          gate_way; 

ESP8266WebServer server(80); 
    
String      ssid="";
String      password="";
    
#include    <TimeLib.h>              // Use for take date in internet
#include    <NtpClientLib.h>

/* setting to update server */

const char* update_path = "/firmware";
const char* update_username = "innogreentech";
const char* update_password = "innogreentech";
    
ESP8266HTTPUpdateServer httpUpdater;



/* solar energy conpensation */

#define NUMBER_OF_READ    1                     // allow to improve stability
unsigned long  number_of_read;

#define STEP_RECOVER      5                  // Allow to increase  power recover in % 

long         power_recover;
unsigned int total_heat;

byte            read_done=0;
unsigned int    total_injection;

/* Control if no suffisant sun   to heat water */

const unsigned long PERIOD_LEGIONEL = 7*24*3600*1000;            //time between two rich ligionelose (7 days max)
#define TEMP_LEGIONEL   65   

unsigned long last_legionel_reach;               

#define AUTONOMIE_FACTOR      2                       // Estimate autonomie number of hour = temp reach x AUTONOMIE_FACTOR
#define MIN_TEMP_AUTONOMIE    55                      // This is the min temp to reach to start the counter
unsigned long last_autonomie_reach;
unsigned long autonomie;                      

void setup() {



  /* setting Pin */
    
  pinMode(PUSH_BUTTOM,INPUT);
  pinMode(ENABLE_HEATING,INPUT_PULLUP);
  pinMode(LED,OUTPUT);  
  pinMode(PWM_PIN,OUTPUT);
  
  /* Setting serial output */
      
  Serial.begin(1200,SERIAL_7E1);
  //Serial.begin(115200);
  Serial.setDebugOutput(0);

  /* Read EEPROM */
  
  EEPROM.begin(256);

  char carac='1';   // Name of module
  int a=0;
  String read_mem="";
  while (!(carac=='\0')&&a<20){carac=char (EEPROM.read(MEM_NAME_MODULE+a));read_mem+= carac; a++;} 
  if ( a==20){read_mem=F("InnoGreenTech");}
  int c = read_mem.length() + 1;                               // longueur de la chaîne de caractéres
  read_mem.toCharArray(name_module, c);    
              
  read_mem="";    // read memory ip adress server
  carac='1';
  a=0;
  while (!(carac=='\0')&&a<20){carac=char (EEPROM.read(MEM_ADRESS_SERVER+a));read_mem+= carac; a++;}
  if (a==20){read_mem=F("InnoGreenTech");} 
  c = read_mem.length() + 1;       
  read_mem.toCharArray(ip_server, c);
                         
  byte mem[2];
  for (int a=0;a<3;a++){mem[a]= EEPROM.read(MEM_PORT_SERVER+a);}     // load the port server         
  port_server= mem[0]|mem[1]<<8;
              
  for (int a=0;a<3;a++){mem[a]= EEPROM.read(MEM_PERIOD+a);}          // load the time period
  period= mem[0]|mem[1]<<8;
  delay_sent=period*1000;

  for (int a=0;a<3;a++){mem[a]= EEPROM.read(MEM_POWER_HEATING+a);}     // load the port server  $$
  int b =   mem[0]|mem[1]<<8;     
  power_heating= b;
  if (power_heating > 30000){power_heating=3000;}

  carac='1';                
  a=0;
  while (!(carac=='\0')&&a<100){carac=char (EEPROM.read(MEM_SSID+a));
  if (!(carac=='\0')){ssid +=carac;} a++;}  //récupération du lien d'information
  if (ssid=='\0'||a>50){ssid=F("");}
  Serial.println(ssid);
            
  carac='1';
  a=0;
  while (!(carac=='\0')&&a<100){carac=char (EEPROM.read(MEM_PASSWORD+a));
  if (!(carac=='\0')){password +=carac;} a++;}  //récupération du lien d'information
  if (password=='\0'||a>50){password=F("");}
  Serial.println(password);

  /* Connexion wifi */

  WiFi.mode(WIFI_STA);
           
  c =ssid.length()+1;
  char char_ssid[50];
  ssid.toCharArray(char_ssid, c);

  c =password.length()+1;
  char char_password[50];
  password.toCharArray(char_password, c);
        
  WiFi.begin(char_ssid,char_password);
           
  unsigned long time_out_connection = millis();
  
  while (WiFi.status() != WL_CONNECTED)
  {
    if (millis() - time_out_connection > 20000)
    {
      Serial.println F("");
      Serial.println F ("connection Wifi fail !");            
      break;
    }
    delay(100);
    Serial.print('-');
  }
           
  if ( WiFi.status()!= WL_CONNECTED)        
  {
    Serial.println F ("Open setting gate !");
    WiFi.disconnect();
    WiFi.mode(WIFI_AP); 
    delay(200);
    WiFi.softAP("InnoGreenTech", "innogreentech");
    server.on ( "/index.html", handleRoot );    // go to set up wifi page
    delay(100);
    server.on ("/style.css", cssRoot);     // envoi au fichier de configuration de style
    local_mac = WiFi.macAddress();
    local_ip="Wifi no connected";
    delay(100);
    server.begin();
  }
  else
  {     
    /*Read the hour and date on internet */
            
    NTP.begin("pool.ntp.org", 1, true);  //configuration de la récupération de la date,  Serveur+1 heure, Heure été/hivers
    NTP.setInterval(3600);                // Toutes les heures
                      
    /*information of module*/

    local_ip=WiFi.localIP().toString().c_str();              
    local_mac=WiFi.macAddress();//.toString().c_str();
    gate_way=WiFi.gatewayIP().toString().c_str();
    ref_module="hot_tank_water";
    version_module="V1";
                             
    /* setting of web server */
              
    server.on ( "/index.html", indexRoot );    // Renvoi à la page de index
    delay(100);
    server.on ( "/", indexRoot );    // Renvoi à la page de index
    delay(100);
    server.on ( "/setup", setup_page );    // Renvoi à la page de index
    delay(100);    
    server.on ( "/add_module.csv", add_module );    // Page for add or modify  module by php server 
    delay(100);
    server.on ("/style.css", cssRoot);     // envoi au fichier de configuration de style
    delay(100);
               
    httpUpdater.setup(&server, update_path, update_username, update_password);
}

  
  
  /* start oled screen */
    
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 64x48)
  display.clearDisplay();
  display.display();
  
  /* search temperature sensor DS18B20 */
  
  sensor.begin();
  sensor.setWaitForConversion(0);
  display.setTextSize(1.5);
  display.setTextColor(WHITE, BLACK);
  display.setCursor(0,0);

  if (sensor.getAddress(water_thermometer,0)){
    display.println("Sensor Found"); 
    for (uint8_t i = 0; i < 8; i++)
      {
        if (water_thermometer[i] < 16) Serial.print("0");
        display.print(water_thermometer[i], HEX);
      }
    display.println("");
  }
  else{display.println("Sensor Not Found!");}

  
  display.println(local_ip);
  display.display();
  
  /* PID control 

  heating_PID.setTimeStep(1000);


  /*listen the server*/
              
  server.begin();

  delay(5000);
  /* timer configuration */

  timer1_attachInterrupt(pwm_timer);
  timer1_write(625/2*SHARPNESS);
  timer1_enable(TIM_DIV256, TIM_EDGE, TIM_LOOP);

  screen_on=0;

}

void loop() {

  server.handleClient();

  sensor.requestTemperatures();
  temperature_water=sensor.getTempC(water_thermometer);
  if (!sensor.isConnected(water_thermometer)){alarm=1;}
  else if (temperature_water>71){alarm=2;}
  else {alarm=0;}
  

  if (screen_on and time_screen_on>DELAY_SCREEN){
    screen_on=0;    
  }

  if (screen_on){
    display.setTextSize(1.5);
    display.setTextColor(WHITE, BLACK);
    display.setCursor(0,0);
    display.print((end_signal*100)/PERIOD);display.print(" ");
    display.println("%");
    display.print(temperature_water);display.println(" C");
    display.print(p_app);display.println(" VA");
    display.print(i_inst);display.println(" A");
   
    display.print("Forcee: ");display.println(forced);
    display.display();
  }
  else  {
    display.clearDisplay();
    display.display();
  }
  
  if (push_buttom()){
  
      if(screen_on and forced==0){forced=1;}
      else if(screen_on and forced==1){forced=0;}

      screen_on=1;
      time_screen_on=0;
  }
  heating_water();
  
  read_linky();
  if (bitRead(statut_message,DATA_READY)){
    check_message();  
  }
  if (millis()>last_message+delay_sent){send_data();}
}
