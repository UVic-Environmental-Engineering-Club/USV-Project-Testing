#include "i2c.h"

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

static char mag_tag[] = "Compass: ";


void app_main() 
{
    i2c_init();

    sensor_t acc_gyr;
    acc_gyr.id = ACC_GYR_ID;
    acc_gyr.addr = ACC_GYR_ADDR;
    acc_gyr.number_of_registers = ACC_GYR_NUMREG;

    sensor_t mag;
    mag.id = MAG_ID;
    mag.addr = MAG_ADDR;
    mag.number_of_registers = MAG_NUMREG;

    byte acc_who_am_i = 0;
    byte mag_who_am_i = 0;

    byte accelx1 = 0;
    byte accelx2 = 0;

    byte magx1 = 0;
    byte magx2 = 0;
    byte magy1 = 0;
    byte magy2 = 0;
    byte magz1 = 0;
    byte magz2 = 0;

    byte pwr = 0;

    i2c_yeet(acc_gyr, ACC_GYR_PWRMGM_REG, 0x00, ACC_GYR_REG_SIZE);
    i2c_yeet(mag, MAG_CTRL_REG3, 0x00, MAG_REG_SIZE);

    while(1)
    {
        i2c_yoink(acc_gyr, ACC_GYR_WHO_AM_I_REG, &acc_who_am_i, ACC_GYR_REG_SIZE);
        i2c_yoink(acc_gyr, ACC_GYR_ACC_X_1_REG, &accelx1, ACC_GYR_REG_SIZE);
        i2c_yoink(acc_gyr, ACC_GYR_ACC_X_2_REG, &accelx2, ACC_GYR_REG_SIZE);
        i2c_yoink(acc_gyr, ACC_GYR_PWRMGM_REG, &pwr, ACC_GYR_REG_SIZE);
        int accelx = (accelx1 << 8) | accelx2;

        i2c_yoink(mag, MAG_WHO_AM_I_REG, &mag_who_am_i, MAG_REG_SIZE);
        i2c_yoink(mag, MAG_OUT_X_L, &magx1, MAG_REG_SIZE);
        i2c_yoink(mag, MAG_OUT_X_H, &magx2, MAG_REG_SIZE);
        i2c_yoink(mag, MAG_OUT_Y_L, &magy1, MAG_REG_SIZE);
        i2c_yoink(mag, MAG_OUT_Y_H, &magy2, MAG_REG_SIZE);
        i2c_yoink(mag, MAG_OUT_Z_L, &magz1, MAG_REG_SIZE);
        i2c_yoink(mag, MAG_OUT_Z_H, &magz2, MAG_REG_SIZE);
        int magx = (magx1 << 8) | magx2;
        int magy = (magy1 << 8) | magy2;
        int magz = (magz1 << 8) | magz2;
        
     /*   printf("%X\n", acc_who_am_i);
        printf("%X\n", pwr);
        printf("%d\n", accelx);
        printf("\n");
        printf("%X\n", mag_who_am_i);
        printf("%X\n", pwr); */
        printf("X: %duT\t, Y: %duT\t, Z: %duT\n", magx, magy, magz);


        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}