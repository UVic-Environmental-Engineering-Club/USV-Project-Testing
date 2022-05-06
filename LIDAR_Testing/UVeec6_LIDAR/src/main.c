#include "main.h"
int reg1 = 0;
void app_main() {

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

    i2c_init();





    sensor_t LIDAR1 = {1,0x62,23};
    byte lidar_val1 = 0x01;
    byte lidar_val2 = 0x00;

    
    
    
    while(lidar_val1 & 0x01 && lidar_val2 != 0x15){     //will make 20 checks to see if lidar is ready to transmit before timing out. lidar is ready when bit 0 of register 0x01 is 0
        i2c_yeet(LIDAR1,0x00,0x04,1);
        i2c_yoink(LIDAR1,0x01,&lidar_val1,1);
        lidar_val2++;
    }
    while(1){
    i2c_yoink(LIDAR1,0x0F,&lidar_val1,1);
    i2c_yoink(LIDAR1,0x10,&lidar_val2,1);
    int reading = (lidar_val1 << 8) | lidar_val2;

    printf("1 is %d, 2 is %d\n",lidar_val1,lidar_val2);
    }















    /*char out [10];

    sprintf(out,"%06dcm\n",reading);                            
    uart_write_bytes(UART_NUM_0, (const char *) out, 9);    

    vTaskDelay(5000/portTICK_PERIOD_MS);
}*/
}
