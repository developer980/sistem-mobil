#include "msp.h"
#include "driverlib.h"
#include "gpio.h"
#include <string.h> // For strcpy, strlen
#include <stdlib.h> // For malloc, free

char *remove_first_character(char *control){
//    int i = 0;
//    while(control[i] != '\0'){
//        control[i] = control[i + 1];
//        i++;
//    }

    return control + 1;
}

void initiate_movement(char *control){
    char *speed = remove_first_character(control);
    char com[1] = control[0];

    int speed_code = atoi(speed);
    int speed_value = speed_code * 30 + 3000;

//    printf("speed value %d", speed_value);

    Timer_A_setCompareValue(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_3, speed_value);
    Timer_A_setCompareValue(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_4, speed_value);




    //printf("speed: %d \n", speed_value);


    switch (atoi(com)){
        case 0:
            move_forward_control();
            break;
        case 1:
            move_backward_control();
            break;
        case 2:
            rotate_left_control();
            break;
        case 3:
            rotate_right_control();
            break;
        case 4:
            stop_control();
            break;
    }
}

void initiate_vehicle_charge_control(char *control){
    char *charge_control_char = remove_first_character(remove_first_character(control));
    int charge_control_code = atoi(charge_control_char);

    if (charge_control_code == 0){
        stop_vehicle_charging();
    }

    if(charge_control_code == 1){
        start_vehicle_charging();
    }
}

void process_controls(char string[6]){
    const char com[1] = string[0];
    switch (atoi(com)){
        case 0:
//            printf("do nothing");
            break;
        case 1:
//            printf("move");
            initiate_movement(remove_first_character(string));
            break;
        case 2:
            initiate_vehicle_charge_control(remove_first_character(string));
            break;
        case 3:
//            printf("vehicle charge control");
            break;
    }
}






