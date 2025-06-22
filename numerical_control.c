#include "msp.h"
#include "driverlib.h"
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

void process_controls(char string[6]){
    const char com[1] = string[0];
    switch (atoi(com)){
        case 0:
            printf("do nothing");
            break;
        case 1:
            printf("move");
            initiate_movement(remove_first_character(string));
            break;
        case 9:
            printf("solar charge control");
            break;
        case 3:
            printf("vehicle charge control");
            break;
    }
}

void initiate_movement(char *control){
//    char *speed = remove_first_character(remove_first_character(control);
    char com[1] = control[0];

//    MAP_Timer_A_setCompareValue(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_1, atoi(speed) * 100);
//    MAP_Timer_A_outputPWM(TIMER_A0_BASE, &pwmConfig);

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




