void rampAngle(){   //calculates and displays ramp angle
    float ax = a.acceleration.x;  //takes the pitch acceleration

    float currentAngle = asin(ax/9.81)*180/PI;  //coverts to degrees
    lcd.clear();
    lcd.print("Ramp Angle: ");
    lcd.setCursor(0,1);
    lcd.print(currentAngle);
    lcd.print("deg");

    delay(3000);  
}