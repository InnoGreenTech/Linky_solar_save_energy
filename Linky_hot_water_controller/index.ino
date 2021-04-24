
void indexRoot(){

             String  get_information="";

              timer1_attachInterrupt(pwm_timer);
              timer1_write(625/2*SHARPNESS);
              timer1_enable(TIM_DIV256, TIM_EDGE, TIM_LOOP);

              if(server.hasArg("POWER_HEATING")){
                get_information = server.arg("POWER_HEATING");
                power_heating=get_information.toInt();               
                int b=int(power_heating);
                for (int a=0;a<2;a++){EEPROM.write(MEM_POWER_HEATING+a ,b);b=b>>8;} 
                timer1_disable();
                EEPROM.commit();
                delay (500);
                ESP.restart();
              }
              
              
              if ( server.hasArg("envoi")) 
              {      
                send_data();
              } 
               server.send ( 200, "text/html", indexPage() );   // envoi de la page
                }
  

String indexPage(){
                    String page =F("<!DOCTYPE html> <html lang=fr-FR><head><meta charset='utf-8'><link rel='stylesheet'type='text/css'href='style.css'><title>Hot tank water</title><meta http-equiv='Refresh' content='30; url=index.html' /></head>");
                           page +=F("<body>");
                           page +=F("<nav> <ul id='menu'><li><a href='index.html'> Accueil </a></li><li><a href='setup'> Update </a></li</ul></nav>");
                          
                           page +=F("<div id='page'>");
                           
                           page +=F("<header><h1>Hot tank water</h1></header>");

                           page +=F("<div id='corp'>");
                           page +=F("<section id='datedujour'><h2>");
                           page +=NTP.getDateStr();
                           page +=F("</h2><h3>");
                           page +=NTP.getTimeStr();
                           page +=F("</h3>");
                           page +=F("<Table><tr><td>Serveur:</td><td></td><td>");
                           page +=ip_server;
                           page +=F("</td></tr><tr><td>Port serveur:</td><td></td><td>");
                           page +=port_server;
                           page +=F("</td></tr><tr><td>Adresse MAC:</td><td></td><td>");
                           page +=local_mac;
                           page +=F("</td></tr><tr><td>Version:</td><td></td><td>");
                           page +=version_module;
                           page +=F("</td></tr></tr></Table></section>");
      
                           page +=F("<section id='Statut'><h2> Statut </h2><table><tr><td>Intensité:</td><td></td><td>");
                           page +=(total_i/index_i);
                           page +=F(" A</tr><tr><td>Puissance:</td><td></td><td>");
                           page +=(total_app/index_app);
                           page +=F(" VA</td></tr><tr><td>Chauffe:</td><td></td><td>");
                           page +=(end_signal*100)/PERIOD;
                           page +=F(" %</td></tr><tr><td>Température:</td><td></td><td>");
                           page +=temperature_water;
                           page +=F(" °C</td></tr><tr><td>Autonomie restante:</td><td></td><td>");
                           page +=(autonomie-(millis()-last_autonomie_reach))/3600000;
                           page +=F(" Heures</td></tr><tr><td>Prochaine décontamination:</td><td></td><td>");
                           page +=(PERIOD_LEGIONEL-(millis()- last_legionel_reach))/(3600000*24);
                           page +=F(" Jours");
                           page +=F("</td></tr></table></section>"); 
                                                                 
                           page +=F("<section id='Consommation'><h2> Consommation </h2><table><tr><td>Consommation :</td><td></td><td>");
                           page +=base;
                           page +=F(" Wh</td></tr><tr><td>Consommation totale:</td><td></td><td>");
                           page +=kwh;                          
                           page +=F(" KWh</td></tr><tr><td>Statut du compteur:</td><td></td><td>");
                           page +=mode_linky[0];
                           page +=mode_linky[1];
                           page +=mode_linky[2];
                           page +=mode_linky[3];
                           page +=mode_linky[4];
                           page +=mode_linky[5];
                           page +=F("</td></tr></table></section>");
                           

                           page +=F("<section id='sent'><h2> Envoi des données  </h2><table><tr><td>Périodicité actuelle ");
                           page +=period;
                           page +=F(" secondes</td><td></td><td><form method='get'><input type='hidden' name='envoi' value='1'/><input type='submit' value='Envoi données'/>");                      
                           page +=F("</form></td></tr></table>");
                           page +=F("<table><tr><td>Puissance en watts du chauffe eau (redémarrage): ");
                           page +=F("</td><td></td><td><form method='get'><input type='num' name='POWER_HEATING' id='POWER_HEATING'  value='");
                           page +=power_heating;
                           page +=F("'/><input type='submit' value='Modifier'/>");                      
                           page +=F("</form></td></tr></table></section>");
                                 
                           page +=F("</div>");
                           
                           page +=F("<footer><table><tr><td><a href='http://innogreentech.fr'>InnoGreenTech  </a></td><td></td><td><a href='mailto: info@innogreentech.fr'> Contactez moi</a></td></tr></table></footer>");
                           page +=F("</body></html>");                 
                           return page;
                          }
