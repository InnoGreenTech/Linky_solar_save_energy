void heating_water(){

  /* set temperature folow color day */

  if(tomorow_color=="rouge"){
    if (today_color=="blanc" || today_color=="bleu"){temperature_setting=65;}
    else {temperature_setting=50;}
    }
  else if(tomorow_color=="blanc"){
    if (today_color=="bleu"){temperature_setting=65;}
    else {temperature_setting=50;}
    }
  else {
       if (today_color=="blanc" || today_color=="rouge"){temperature_setting=50;}
       else if (weekday()==2){temperature_setting=65;}  
       else {temperature_setting=55;}
       }

  /* control temperature */
  
  if (temperature_water>67 || alarm>0){end_signal=0;}
  else if (forced and temperature_water<65){end_signal=PERIOD;}
  else if (!digitalRead(ENABLE_HEATING)){   

    if (hour_statut=="Creuse" and hour()>2 and hour()<8){ 
          if (temperature_water < temperature_setting) {end_signal=PERIOD;}
          else {end_signal=0;}
        }
  
    else {
 
        if(read_done==1){
          if (p_app == 0 ){power_recover = power_recover +((i_inst*23000)/power_heating); }
          else {power_recover = power_recover -( (i_inst*23000)/power_heating);}
          if (power_recover>100){power_recover=100;};
          if (power_recover<0){power_recover=0;}
          read_done=0;
        }
        end_signal=int((power_recover*PERIOD)/100);       
      }
  }
  else {end_signal=0;}

  /* power off security */

  if (end_signal==0){digitalWrite(PWM_PIN,0);}
  
  
}
