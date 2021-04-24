void add_module(){
  
      timer1_disable();
      String   reception;

      if ( server.hasArg("ip_server") ) 
              {      
              reception=server.arg("ip_server");
              int c = reception.length() + 1;         // longueur de la chaîne de caractéres                    
              if ( c>16){ Serial.println(F("adresse ip invalide")); return;}
              reception.toCharArray(ip_server, c); 
              for (int i = 0; i < c; i++) { 
                                            EEPROM.write((i+MEM_ADRESS_SERVER),reception[i]);  // save adresse IP in EEPROM
                                            }  
              }

      if ( server.hasArg("port") ) 
              {       
              reception = server.arg("port");
              port_server=reception.toInt();           
              int b=port_server;
              for (int a=0;a<2;a++){EEPROM.write(MEM_PORT_SERVER+a ,b);b=b>>8;} // Save configuration port
              } 

      if ( server.hasArg("name") ) 
              { 
              reception=server.arg("name"); 
              int c = reception.length() + 1;                               
              if ( c>20){Serial.println(F("Nom trop long"));return;} 
              reception.toCharArray(name_module, c); 
              for (int i = 0; i < c; i++) { 
                                            EEPROM.write((i+MEM_NAME_MODULE),reception[i]);  // save adresse IP in EEPROM
                                            }  
              } 

      if ( server.hasArg("period") )       // Period to send datas
              {       
              reception = server.arg("period");
              period=reception.toInt();           
              int b=period;
              delay_sent=period*1000;
              for (int a=0;a<2;a++){EEPROM.write(MEM_PERIOD+a ,b);b=b>>8;} 
              } 
      EEPROM.commit();     // Enregistrement

      server.send ( 200, "text/csv", answer_add_module() );   // envoi de la page  
      
      timer1_attachInterrupt(pwm_timer);
      timer1_write(625/2*SHARPNESS);
      timer1_enable(TIM_DIV256, TIM_EDGE, TIM_LOOP);
}

/* file csv with information for php server */

String answer_add_module(){
  String page =name_module; page +=F(";");  page +=ref_module;  page +=F(";");  page +=local_mac;  page +=F(";");
         page +=local_ip; page +=F(";");  page +=version_module; page +=F("\r\n");                                      // line 1 informations module
         
         // type( probe or actuator,id,name,type_probe)

         
         page +=F("probe;p1;i_inst;LINKY_I\r\n");    
         page +=F("probe;p2;p_app;LINKY_APP\r\n"); 
         page +=F("probe;p3;base;LINKY_WH\r\n");     
         page +=F("probe;p4;petc;LINKY_PETC\r\n");
         page +=F("probe;p5;temperature_water;DS18b20\r\n");
         page +=F("probe;p6;kwh;LINKY_KWH\r\n");
                                   
         page +=F("actuator;a1;heat_water;SSR\r\n");    
         page +=F("actuator;a2;solar_recover;SSR\r\n"); 
 
          
         return page;
  }
