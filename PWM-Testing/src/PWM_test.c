/* 
PWM Test Code to abstract PWM usage throughout the USV using the ESP32's LEDC driver.
Written by Miguel Manguerra.
*/
#pragma once
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "driver/ledc.h"
#include "esp_err.h"

void app_main(void)
{
    PWM_config_init(LEDC_TIMER_8_BIT, 50, 0, LEDC_HIGH_SPEED_MODE, 14, 0);
    PWM_duty_cycle_update(0, 14, 50);
    PWM_config_init(LEDC_TIMER_8_BIT, 50, 0, LEDC_HIGH_SPEED_MODE, 13, 2);
    PWM_duty_cycle_update(2, 13, 50);
    PWM_config_init(LEDC_TIMER_8_BIT, 50, 1, LEDC_HIGH_SPEED_MODE, 12, 1);
    PWM_duty_cycle_update(1, 12, 50);
    vTaskDelay (20000 / portTICK_RATE_MS);
    PWM_shutdown();
}