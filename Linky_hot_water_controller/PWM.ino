void pwm_timer()
{
  start_period++;
  if(start_period>PERIOD){start_period=0; out=1;}
  if(start_period>=end_signal){out=0;} 
  digitalWrite(PWM_PIN, out);
  digitalWrite(LED,out);
  if (screen_on){time_screen_on++;}
}
