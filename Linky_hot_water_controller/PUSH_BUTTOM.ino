byte push_buttom(){

  if(!digitalRead(PUSH_BUTTOM)){
    push_buttom_start++;
  }
  else {push_buttom_start=0; return 0;}

  if(push_buttom_start==PUSH_BUTTOM_DELAY){
    return 1;
  }
  else { return 0;}  
  
}
