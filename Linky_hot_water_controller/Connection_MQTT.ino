void mqtt_connection(){
              unsigned long time_out_connection = millis();
              int test;

           /*   while (!EC.connected())
              //{   */  

               timer1_disable();      
               digitalWrite(PWM_PIN,0);
               digitalWrite(LED,0); 
               
               if (EC.connect("BOIL-CONTROLER", MQTT_USER, MQTT_PASS)){
                delay(20);
               //Serial.println("Public emqx mqtt broker connected");
                //break;
                EC.subscribe("hass/power/disponible");
               
               } 
                  /*else {
               // Serial.print("failed with state ");
                Serial.print(EC.state());
             
                   if (millis() - time_out_connection > 10000)
                    {
                     // Serial.println F("");
                     // Serial.println F ("connection MQTT fail !");            
                      break;
                    }
                    delay(50);
                    //Serial.print('-');
                    }


                } */
                timer1_attachInterrupt(pwm_timer);
                timer1_write(625*SHARPNESS); 
                timer1_enable(TIM_DIV256, TIM_EDGE, TIM_LOOP);
                
return;
 }


      
