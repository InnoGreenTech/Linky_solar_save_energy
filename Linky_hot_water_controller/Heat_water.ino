void heating_water(){

  /* set temperature folow color day */

  if(tomorow_color=="rouge"){
    if (today_color=="blanc" || today_color=="bleu"){temperature_setting=65;}
    else {temperature_setting=55;}
    }
  else if(tomorow_color=="blanc"){
    if (today_color=="bleu"){temperature_setting=65;}
    else {temperature_setting=55;}
    }
  else {
       if (today_color=="blanc" || today_color=="rouge"){temperature_setting=55;}
       else {temperature_setting=60;}  
       }

  /* control temperature */
  
  if (temperature_water>70 || alarm>0){heating_PID.stop(); end_signal=0;}
  else if (forced and temperature_water<60){heating_PID.stop(); end_signal=PERIOD;}
  else if (!digitalRead(ENABLE_HEATING)){   

    if (hour_statut=="Creuse"){ 
          out_injection=0;             
          heating_PID.run(); //call every loop, updates automatically at certain time interval
          end_signal=int(out_heating);   
        }
  
    else {
        heating_PID.stop();
        if(read_done==1){
          if (p_app<p_setting){out_injection=out_injection+STEP_INJECTION;}
          else if (out_injection>0){out_injection=out_injection-STEP_INJECTION;}
          read_done=0;
        }
        end_signal= out_injection;        
      }
  }
  else {heating_PID.stop();end_signal=0;}

  /* power off security */

  if (end_signal==0){digitalWrite(PWM_PIN,0);}
  
  
}
