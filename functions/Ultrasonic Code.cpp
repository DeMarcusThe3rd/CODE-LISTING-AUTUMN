void avoidObstacle(){
    if (hc.dist()>20.00){
        forward(0,150);
    }

    else{
        turn(0,255,-255);
    }
}
