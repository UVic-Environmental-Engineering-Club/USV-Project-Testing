#include "sdkconfig.h"
#include "driver/adc_common.h"
#include "driver/uart.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "driver/ledc.h"
#include "esp_err.h"
#include "driver/gpio.h"
#include <math.h>
#include "PWM.h"

int isno( char in ) { //Checks if an input char is a digit, returns that digit as an int. If not, returns 500. used in UART_run()
    if(in >= 48 && in <= 57){
        return in - 48;
    }
    return 500;
}

uint16_t angle_to_duty(uint8_t angle){

    if (angle > 180) //Don't send bad angles
        angle = 90;
    
    float fraction = angle/180.0; //Fractional multiplier
    uint16_t duty_val = floor(3276*fraction); //Converts above fraction to a duty value
    return (3276+duty_val); //Adds the duty value to a 1ms duty since servo is 1-2ms duty
}

void app_main() {


    int DELAY_LENGTH = 1000;
    uint8_t kevin = 90; // input angle in degrees

    gpio_set_direction(GPIO_NUM_2, GPIO_MODE_DEF_OUTPUT);

    PWM_config_init(LEDC_TIMER_16_BIT, 50, 0, LEDC_HIGH_SPEED_MODE, 12, 0);
    PWM_duty_cycle_update(0, 12, angle_to_duty(kevin));
    printf("set to 90\n");
    vTaskDelay (DELAY_LENGTH / portTICK_PERIOD_MS); 

 int accelx = 0;
 int accely = 0;
 int accelz = 0;
 int gyrx = 0;
 int gyry = 0;
 int gyrz = 0;
 int lid1 = 0;
 int lid2 = 0;
 int lid3 = 0;
 int heading = 0;
 int rpm1 = 0;
 int rpm2 = 0;
 int tmp1 = 0;
 int tmp2 = 0;
 int wet1 = 0;
 int wet2 = 0;
 int longitude = 0;
 int latitude = 0;


    const uart_port_t uart_num = UART_NUM_0;
    uart_config_t uart_config = {                                //uart config
        .baud_rate = 9600,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    };
    uart_param_config(uart_num, &uart_config);
    uart_driver_install(UART_NUM_0, 2048, 0, 0, NULL, 0);




    for(int t = 1;1;t++){

        

    char whut[2];
    sprintf(whut,"\n");
    uart_write_bytes(UART_NUM_0, (const char *) whut, 1); 
           
    int output_length = 16;                                             //the number of characters in each output message
    char out[output_length + 1];                                        //buffer to store output message. Needs extra index at end or sprintf adds an endline character over message.
    char gps_buffer[24+1];
    int input_length = 7;
    char in[input_length + 1];
    char confirmation[23];
    int ran;
    int mpr;

    in[0]='a';

    sprintf(out,"ACC-%03d-%03d-%03d\n",accelx,accely,accelz);                              //output message to be sent -> can be copypasted to to make messages for any (int/char/float/etc.) global variable, just not sure how many we'll need yet. Might need edits if float value is >999
    uart_write_bytes(UART_NUM_0, (const char *) out, output_length);    //outputs buffer to port0, each character is 1 byte as per ASCII, output must have exactly the same number of characters as the output_length variable

    sprintf(out,"GYR-%03d-%03d-%03d\n",gyrx,gyry,gyrz);                              
    uart_write_bytes(UART_NUM_0, (const char *) out, output_length);

    sprintf(out,"LID-%03d-%03d-%03d\n",lid1,lid2,lid3);                              
    uart_write_bytes(UART_NUM_0, (const char *) out, output_length);

    sprintf(out,"COM-%03d\n",heading);                              
    uart_write_bytes(UART_NUM_0, (const char *) out, output_length-8);

    sprintf(out,"RPM-%03d-%03d\n",rpm1,rpm2);                              
    uart_write_bytes(UART_NUM_0, (const char *) out, output_length-4);

    sprintf(out,"TMP-%03d-%03d\n",tmp1,tmp2);                              
    uart_write_bytes(UART_NUM_0, (const char *) out, output_length-4);

    sprintf(out,"WET-%03d-%03d\n",wet1,wet2);                              
    uart_write_bytes(UART_NUM_0, (const char *) out, output_length-4);

    sprintf(gps_buffer,"GPS-%09d-%09d\n",longitude,latitude);                              
    uart_write_bytes(UART_NUM_0, (const char *) gps_buffer, 24);

    uart_read_bytes(UART_NUM_0, in, input_length, 500/ portTICK_PERIOD_MS);
    if(in[0] == 'm' && in[1] == 'p' && in[2] == 'r' && in[3] == '-'){
        unsigned int read = 0;
        read = isno(in[4])*100 + isno(in[5])*10 + isno(in[6]);
        if(read > 100){
            sprintf(confirmation, "err, invalid motor   \n");
        }else{
            mpr = read;
            sprintf(confirmation, "motor updated to %03d \n", mpr);
        }
        uart_write_bytes(UART_NUM_0, (const char *) confirmation, 22);
    }else if(in[0] == 'r' && in[1] == 'a' && in[2] == 'n' && in[3] == '-'){
        unsigned int read = 0;
        read = isno(in[4])*100 + isno(in[5])*10 + isno(in[6]);
        if(read > 180){
            sprintf(confirmation, "err, invalid rudder  \n");
        }else{
            ran = read;
            sprintf(confirmation, "rudder updated to %03d\n", ran);
            PWM_duty_cycle_update(0, 12, angle_to_duty(ran));
            vTaskDelay (100 / portTICK_RATE_MS);
        }
        uart_write_bytes(UART_NUM_0, (const char *) confirmation, 22);



        
    }

 accelx += 13;
 accely += 3;
 accelz += 10;
 gyrx +=15;
 gyry +=8;
 gyrz +=5;
 lid1 +=2;
 lid2 +=1;
 lid3 +=3;
 heading +=5;
 rpm1 +=6;
 rpm2 +=7;
 tmp1 +=7;
 tmp2 +=8;
 wet1 +=9;
 wet2 +=3;
 longitude += 46;
 latitude +=37;

vTaskDelay(5000/portTICK_PERIOD_MS);
    }
}
