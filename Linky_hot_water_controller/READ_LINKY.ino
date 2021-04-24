void read_linky(void){
  
  char c;

  while(Serial.available() and !bitRead(statut_message, MESSAGE_READY)){
    
    c=Serial.read();
    
    if(c==0x0D and bitRead(statut_message,MESSAGE_IN_READ))
    {
      bitSet(statut_message,MESSAGE_READY);      
    }
    else if (c==0x0A)
    {
      bitSet(statut_message,MESSAGE_IN_READ);
    }
    else if(bitRead(statut_message,MESSAGE_IN_READ))
    {
      message += c;
    }
  }

    
   
  if (bitRead(statut_message, MESSAGE_READY)&&!bitRead(statut_message, DATA_READY)){   

    int index;
    
    etiquette="";
    donnee="";    
    checksum=0;

    for (index = 0; message[index]!=0x20 && message[index] ; index++){
      etiquette += message[index];
      checksum=checksum+message[index];
    }
    checksum=checksum+message[index];
    for (index=index+1; message[index]!=0x20 && message[index]; index++){
      donnee += message[index];
      checksum=checksum+message[index];
    }
    
    if (checksum != message[index+1]){
      statut_message=0;
      message="";
    }
    
    bitSet(statut_message,DATA_READY);
  }
}

void check_message(){
       
           if (etiquette=="BASE"){base=strtoul(donnee.c_str(),NULL,10);kwh=base/1000;}      
                   
           else if(etiquette=="PTEC"){ptec=donnee;}
   
           else if(etiquette=="IINST"){i_inst=donnee.toInt();total_i=total_i+i_inst;index_i++; read_done=1;} 
           else if(etiquette=="PAPP"){p_app=donnee.toInt(); total_app=total_app+p_app; index_app++; total_injection=total_injection+power_recover;total_heat=total_heat+((end_signal*100)/PERIOD);}
           else if(etiquette=="HHPHC"){h_hp_hc=donnee;}
           else if(etiquette=="MOTDETAT"){for (int i=0; i<6; i++){mode_linky[i]=donnee[i];}}
           else if(etiquette=="ISOUSC"){i_sousc=donnee.toInt();}
           else if(etiquette=="ADC0"){for (int i=0; i<12; i++){adresse[i]=donnee[12];}}
           else if(etiquette=="OPTARIF"){op_tarif=donnee.toInt();}
           else{} 
           statut_message=0;
           message="";                                                                                                                                 
                                
}
