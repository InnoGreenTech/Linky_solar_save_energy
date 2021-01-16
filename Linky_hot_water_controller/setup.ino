
void setup_page(){ 
  
      timer1_disable();  // stop timer before update
      
      digitalWrite(PWM_PIN, 0);
      digitalWrite(LED,0);             
      server.send ( 200, "text/html", page_setup() );   // envoi de la page
      
    }
  


String page_setup(){
  String page =F("<html><head><meta http-equiv='Refresh' content='0; url=firmware' /></head></html>");       
return page;
}
