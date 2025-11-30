#include "msp.h"
#include "driverlib.h"
#include "gpio.h"
#include <string.h>
#include <stdlib.h>

// remove the first character from a string
char *remove_first_character(char *control){
    return control + 1;
}

// initiate motor rotation direction and speed via PWM based on the received control
void initiate_movement(char *control){
    char *speed = remove_first_character(control);
    char com[1] = control[0];

    int speed_code = atoi(speed);
    int speed_value = speed_code * 30 + 3000;

    Timer_A_setCompareValue(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_3, speed_value);
    Timer_A_setCompareValue(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_4, speed_value);

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

// activate the charging port for other vehicles
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

// activate the charging process for the charging vehicle
void initiate_solar_charge_control(char *control){
    char *charge_control_char = remove_first_character(remove_first_character(control));
    int charge_control_code = atoi(charge_control_char);

    if (charge_control_code == 0){
        stop_solar_charging();
    }

    if(charge_control_code == 1){
        start_solar_charging();
    }
}

// select the process based on the received control
void process_controls(char *string){
    const char com[1] = string[0];

    switch (atoi(com)){
        case 0:
//            printf("do nothing");
            break;
        case 1:
            initiate_movement(remove_first_character(string));
            break;
        case 2:
            initiate_vehicle_charge_control(remove_first_character(string));
            break;
        case 3:
            initiate_solar_charge_control(remove_first_character(string));
            break;
    }
}






