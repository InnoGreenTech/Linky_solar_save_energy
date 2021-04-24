void heating_water(){

  /* initialise timer */

  if (temperature_water>=MIN_TEMP_AUTONOMIE){                                                // Calcul the autonomie of the ballon
    unsigned long  test_autonomie= (temperature_water/AUTONOMIE_FACTOR)*3600000;
    if(autonomie>(millis()-last_autonomie_reach)){
      if (test_autonomie>(autonomie-(millis()-last_autonomie_reach))){autonomie=test_autonomie; last_autonomie_reach= millis();}
    }
    else{autonomie=test_autonomie; last_autonomie_reach= millis();}
  }

  if(temperature_water>=TEMP_LEGIONEL) {last_legionel_reach=millis();}                 // Control the last time when the protect legionnel was been reach
       

  /* control temperature */
  
  if (temperature_water>67 || alarm>0){end_signal=0;}
  else if (forced and temperature_water <= 65){end_signal=PERIOD;}
  else if (!digitalRead(ENABLE_HEATING)){ 
        if(read_done==1){
          if(number_of_read>NUMBER_OF_READ){
            number_of_read=0;
            if (p_app == 0 ){power_recover = power_recover + STEP_RECOVER;}                          
            else {power_recover = power_recover -(( p_app*100)/power_heating);number_of_read--;}          
            if (power_recover>100){power_recover=100;};
            if (power_recover<0){power_recover=0;}            
          }
          number_of_read++;
          read_done=0;
          end_signal=int((power_recover*PERIOD)/100);  
        }
        
        if ((millis()- last_legionel_reach)> PERIOD_LEGIONEL){
          end_signal=PERIOD;
        }

        if ((millis()- last_autonomie_reach)> autonomie){
          end_signal=PERIOD;
        }

      }
  else {end_signal=0;}

  /* power off security */

  if (end_signal==0){digitalWrite(PWM_PIN,0);}
  if(end_signal>PERIOD){end_signal=PERIOD;}  
  
}
