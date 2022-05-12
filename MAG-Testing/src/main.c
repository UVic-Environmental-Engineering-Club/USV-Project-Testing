#include "i2c.h"
#include <math.h>

#define ACC_GYR_ID                  0
#define ACC_GYR_ADDR                0x68
#define ACC_GYR_REG_SIZE            1
#define ACC_GYR_WHO_AM_I_REG        0x75
#define ACC_GYR_ACC_X_1_REG         0x3B
#define ACC_GYR_ACC_X_2_REG         0x3C
#define ACC_GYR_PWRMGM_REG          0x6B
#define ACC_GYR_NUMREG              1

#define MAG_ID                      1
#define MAG_ADDR                    0x1C
#define MAG_REG_SIZE                1
#define MAG_WHO_AM_I_REG            0x0F
#define MAG_OUT_X_L                 0x28
#define MAG_OUT_X_H                 0x29
#define MAG_OUT_Y_L                 0x2A
#define MAG_OUT_Y_H                 0x2B
#define MAG_OUT_Z_L                 0x2C
#define MAG_OUT_Z_H                 0x2D
#define MAG_CTRL_REG3               0x22                
#define MAG_NUMREG                  1
#define PI 3.14159

double heading;



void app_main() 
{
    i2c_init();

    sensor_t mag;
    mag.id = MAG_ID;
    mag.addr = MAG_ADDR;
    mag.number_of_registers = MAG_NUMREG;

    byte mag_who_am_i = 0;

    byte magx1 = 0;
    byte magx2 = 0;
    byte magy1 = 0;
    byte magy2 = 0;
    byte magz1 = 0;
    byte magz2 = 0;

    i2c_yeet(mag, MAG_CTRL_REG3, 0x00, MAG_REG_SIZE);

    while(1)
    {
        i2c_yoink(mag, MAG_WHO_AM_I_REG, &mag_who_am_i, MAG_REG_SIZE);
        i2c_yoink(mag, MAG_OUT_X_L, &magx1, MAG_REG_SIZE);
        i2c_yoink(mag, MAG_OUT_X_H, &magx2, MAG_REG_SIZE);
        i2c_yoink(mag, MAG_OUT_Y_L, &magy1, MAG_REG_SIZE);
        i2c_yoink(mag, MAG_OUT_Y_H, &magy2, MAG_REG_SIZE);
        i2c_yoink(mag, MAG_OUT_Z_L, &magz1, MAG_REG_SIZE);
        i2c_yoink(mag, MAG_OUT_Z_H, &magz2, MAG_REG_SIZE);
        double magx = ((magx1 << 8) | magx2)/6842;
        double magy = ((magy1 << 8) | magy2)/6842;
        double magz = ((magz1 << 8) | magz2)/6842;
        heading = atan(magy/magx)*180/PI;

        if (heading < 0)
            heading = 360 + heading;

        printf("X: %fGauss\t Y: %fGauss\t Z: %fGauss\n", magx, magy, magz);
        printf("The heading is: %f°\n", heading);

        vTaskDelay(50/ portTICK_RATE_MS);
    }
}