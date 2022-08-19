void send_data(){

      timer1_disable();
      
      digitalWrite(PWM_PIN,0);
      digitalWrite(LED,0); 



      String ip=WiFi.localIP().toString().c_str();    // Allow at the server to know the new address if it have changed
      String data;
      
      /* Création des données json */

      
       
      const size_t bufferSize =JSON_OBJECT_SIZE(9);
      DynamicJsonDocument root(bufferSize);

     root["mac_adress"]=WiFi.macAddress();
     root["ip_adress"]=ip; 

     if (index_i>0){
       root["I_Linky"]=(total_i/index_i);  
       if (index_app>0){root["S_Linky"]=(total_app/index_app);}
       root["E_Linky"]=base;    
       // root["Periode_Linky"]=ptec;
       root["T_Water"]=temperature_water; 
       if (index_app>0){ root["Puissance_chauffe"]=(total_heat/index_app);}

     }


      
     serializeJson(root,data);   

if (!EC.connected()){
     mqtt_connection();
    }
    
 if (EC.connected()){
       
       char tampon[255]; 
       //Serial.println("");
       //Serial.println(data.length());
       data.toCharArray(tampon, data.length()+1);    
       //Serial.println(tampon);   
       EC.publish(topicR, tampon);
     }
     //root.printTo(data);
 /*    String adresse="http://";
     adresse+=ip_server;
     adresse+="/InnoGreenTech/controller/json_probe.php";
     String data1= "data=";
     data1 +=data;
     //Serial.println(data1);

     HTTPClient http;
     int httpCode = 0;
     http.begin(adresse);
     http.addHeader("Content-Type", "application/x-www-form-urlencoded"); //http.addHeader("Content-Type", "application/json");
     httpCode = http.POST(data1);
     String payload = http.getString();
     //Serial.println(httpCode);   //Print HTTP return code
     //Serial.println(payload);
     http.end();  //Close connection */
     
     index_i=0;
     index_app=0;
     total_i=0;
     total_app=0;
     total_injection=0;
     total_heat=0;
     last_message= millis();
     
     timer1_attachInterrupt(pwm_timer);
     timer1_write(625*SHARPNESS);
     timer1_enable(TIM_DIV256, TIM_EDGE, TIM_LOOP);
}
