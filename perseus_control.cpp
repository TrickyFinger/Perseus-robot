#include "ROBOTERRA.h"

/*--------------------- STEP 1 ---------------------*/
//Objects initialization and vars definition
RoboTerraRoboCore perseus;
RoboTerraLED red;
RoboTerraLED green;
RoboTerraTapeSensor eye;
RoboTerraMotor rwheel;
RoboTerraMotor lwheel;
RoboTerraJoystick stick;
RoboTerraServo servo1;
RoboTerraServo servo2;
RoboTerraButton button;
RoboTerraButton button2;
RoboTerraSoundSensor ear;

int angle = 0;
int delta = 14;
int mode = 0;  //0:servo 1:motor
int xspeed;    //left wheel speed
int yspeed;    //right wheel speed
int flag;      //control mode
/*--------------------- STEP 2 ---------------------*/
// Attach electronics to physical ports on RoboCore.
void attachRoboTerraElectronics() {
    perseus.attach(red, DIO_1);
    perseus.attach(green, DIO_2);
    perseus.attach(eye, DIO_3);
    perseus.attach(rwheel, MOTOR_A);
    perseus.attach(lwheel, MOTOR_B);
    perseus.attach(stick, AI_1, AI_2);
    perseus.attach(servo1, SERVO_A);
    perseus.attach(servo2, SERVO_B);
    perseus.attach(button, DIO_4);
    perseus.attach(button2, DIO_6);
    perseus.attach(ear, DIO_5);
}

/*--------------------- STEP 3 ---------------------*/
//Control functions
void handleRoboTerraEvent() {
    if (EVENT.isType(ROBOCORE_LAUNCH)) {
        rwheel.activate();
        lwheel.activate();
        rwheel.rotate(0);
        lwheel.rotate(0);
        eye.activate();
        stick.activate();
        servo1.activate(40);
        servo2.activate(40);
        button.activate();
        ear.activate();
        red.turnOff();
        green.turnOn();

    }

    //First press the button to start running
    //Second to stop
    if (EVENT.isFrom(button)){
        if (EVENT.isType(BUTTON_PRESS)){
            if(EVENT.getData()%2==1){
                rwheel.rotate(3);
                lwheel.rotate(3);
                red.turnOff();
                green.turnOn();
                flag = 1;
            }
        }
    }
    if (EVENT.isFrom(button)){
        if (EVENT.isType(BUTTON_PRESS)){
            if(EVENT.getData()%2==0){
                rwheel.rotate(0);
                lwheel.rotate(0);
                red.turnOn();
                green.turnOff();
                flag = 0;
            }
        }
    }
    //switch the control object of the joystick

    if(EVENT.isFrom(button2)){
        if (EVENT.isType(BUTTON_PRESS)){
            if(EVENT.getData()%2==0){
                mode = 1;
            }
            if(EVENT.getData()%2==1){
                mode = 0;
            }
        }
    }

    //Obstacle ahead, stop. Red light on
    if (EVENT.isFrom(eye)){
        red.turnOn();
        green.turnOff();
        rwheel.pause();
        lwheel.pause();
    }

    //Use joystick to control the lifting mechanism
    if(mode == 0){
      if (EVENT.isFrom(stick)){
        //servo1 at joint1
        if(EVENT.isType(JOYSTICK_X_UPDATE)){
            if(EVENT.getData()>0){
                servo1.rotate(180, 2);
                servo1.resume();

            }
            if(EVENT.getData()==0){
                servo1.pause();
            }
            if(EVENT.getData()<0){
                servo1.rotate(0, 2);
                servo1.resume();
            }
        }
        //servo2 at joint2
        if(EVENT.isType(JOYSTICK_Y_UPDATE)){
            if(EVENT.getData()>0){
                servo2.rotate(180, 2);
                servo2.resume();
            }
            if(EVENT.getData()==0){
                servo2.pause();
            }
            if(EVENT.getData()<0){
                servo2.rotate(0, 2);
                servo2.resume();
            }
        }
      }
    }
    //Use joystick to control the movement
    if(flag == 1){
      if(mode == 1){
        if(EVENT.isFrom(stick)){
            red.turnOff();
            green.turnOn();
            //joystick position represents the speed
            if(EVENT.isType(JOYSTICK_X_UPDATE)){
                xspeed = EVENT.getData();
            }
            if(EVENT.isType(JOYSTICK_Y_UPDATE)){
                yspeed = EVENT.getData();
            }
            lwheel.rotate(xspeed - yspeed);
            rwheel.rotate(xspeed + yspeed);
        }
      }
    }
    //detect the first sound to go back
    //the second sound to stop
    if(EVENT.isFrom(ear)){
        if(EVENT.isType(SOUND_BEGIN)){
            rwheel.resume();
            lwheel.resume();
            rwheel.reverse();
            lwheel.reverse();
            red.turnOff();
            green.turnOn();
        }
    }
}
