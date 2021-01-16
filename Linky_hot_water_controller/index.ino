
void indexRoot(){

              timer1_attachInterrupt(pwm_timer);
              timer1_write(625/2*SHARPNESS);
              timer1_enable(TIM_DIV256, TIM_EDGE, TIM_LOOP);
  
              
              
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
                           page +=F(" °C</td></tr><tr><td>Aujourd'hui:</td><td></td><td class='");    
                           page +=today_color;
                           page +=F("'></td></tr><tr><td>Demain:</td><td></td><td class='");
                           page +=tomorow_color;
                           page +=F("'></td></tr><tr><td>Période:</td><td></td><td>");
                           page +=hour_statut;
                           page +=F("</td></tr></table></section>"); 
                                                                 
                           page +=F("<section id='Consommation'><h2> Consommation </h2><table><tr><td>Bleu heures pleines :</td><td></td><td>");
                           page +=hpjb;
                           page +=F(" Wh</td></tr><tr><td>Bleu heures creuses</td><td></td><td>");
                           page +=hcjb;
                           page +=F(" Wh</td></tr><tr><td>Blanc heures pleines:</td><td></td><td>");
                           page +=hpjw;
                           page +=F(" Wh</td></tr><tr><td>Blanc heures creuses:</td><td></td><td>");
                           page +=hcjw;
                           page +=F(" Wh</td></tr><tr><td>Rouge heures pleines:</td><td></td><td>");
                           page +=hpjr;
                           page +=F(" Wh</td></tr><tr><td>Rouge heures creuses:</td><td></td><td>");
                           page +=hcjr; 
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
                           page +=F(" secondes</td><td></td><td><form method='get'><input type='hidden' name='envoi' value='1'/><input type='submit' value='envoi'/>");                      
                           page +=F("</form></td></tr></table></section>");
                                 
                           page +=F("</div>");
                           
                           page +=F("<footer><table><tr><td><a href='http://innogreentech.fr'>InnoGreenTech  </a></td><td></td><td><a href='mailto: info@innogreentech.fr'> Contactez moi</a></td></tr></table></footer>");
                           page +=F("</body></html>");                 
                           return page;
                          }
