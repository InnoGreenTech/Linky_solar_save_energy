void send_data(){

      //timer1_disable();
      
      digitalWrite(PWM_PIN,0);


      String ip=WiFi.localIP().toString().c_str();    // Allow at the server to know the new address if it have changed
      String data; 
      
      /* Création des données json */

      
       
      const size_t bufferSize =JSON_OBJECT_SIZE(13);
      DynamicJsonDocument root(bufferSize);

     root["mac_adress"]=WiFi.macAddress();
     root["ip_adress"]=ip; 

     root["p1"]=(total_i/index_i);    
     root["p2"]=(total_app/index_app);
     root["p3"]=base;    
     root["p9"]=ptec;
     root["p10"]=temperature_water;
     root["p11"]=kwh;
     
     root["a1"]=(total_heat/index_app);
     root["a2"]=(total_injection/index_app);

      
     serializeJson(root,data);    
     //root.printTo(data);
     String adresse="http://";
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
     http.end();  //Close connection 
     index_i=0;
     index_app=0;
     total_i=0;
     total_app=0;
     total_injection=0;
     total_heat=0;
     last_message= millis();
     
     /*timer1_attachInterrupt(pwm_timer);
     timer1_write(625/2*SHARPNESS);
     timer1_enable(TIM_DIV256, TIM_EDGE, TIM_LOOP);*/
}
