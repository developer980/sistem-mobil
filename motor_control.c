/*
 * motor_control.c
 *
 *  Created on: Jun 11, 2025
 *      Author: tudor
 */

// both motors forward
#include "msp.h"
#include "driverlib.h"

void move_forward_control(){
    GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN2);
    GPIO_setOutputHighOnPin(GPIO_PORT_P3, GPIO_PIN3);
    GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN1);
    GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN3);
    printf("Moving forward \n");
}

// both motors backward
void move_backward_control(){
    GPIO_setOutputHighOnPin(GPIO_PORT_P3, GPIO_PIN2);
    GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN3);
    GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN1);
    GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN3);
    printf("Moving backward \n");
}

// left motor backward and right motor forward
void rotate_left_control(){
    GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN2);
    GPIO_setOutputHighOnPin(GPIO_PORT_P3, GPIO_PIN3);
    GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN1);
    GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN3);
    printf("Rotate left \n");
}

// left motor forward and right motor backward
void rotate_right_control(){
    GPIO_setOutputHighOnPin(GPIO_PORT_P3, GPIO_PIN2);
    GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN3);
    GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN1);
    GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN3);
    printf("Rotate right \n");
}

void stop_control(){
    GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN2);
    GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN3);
    GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN1);
    GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN3);
    printf("Stop \n");
}
