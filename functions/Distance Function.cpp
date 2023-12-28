int initPulse = digitalRead(ROT);

void countDistance(){
    int pulse = digitalRead(ROT);

    if(pulse != initPulse){
        counter++;
    }
    initPulse = pulse;

    distance = counter*0.05*PI*radius;
    lcd.clear();
    lcd.print("Distance: ");
    lcd.print(distance);
    lcd.print("m");
}