/* LEDC (LED Controller) fade example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "driver/ledc.h"
#include "esp_err.h"

/*
 * About this example
 *
 * 1. Start with initializing LEDC module:
 *    a. Set the timer of LEDC first, this determines the frequency
 *       and resolution of PWM.
 *    b. Then set the LEDC channel you want to use,
 *       and bind with one of the timers.
 *
 * 2. You need first to install a default fade function,
 *    then you can use fade APIs.
 *
 * 3. You can also set a target duty directly without fading.
 *
 * 4. On ESP32, GPIO18/19/4/5 are used as the LEDC outputs:
 *              GPIO18/19 are from the high speed channel group
 *              GPIO4/5 are from the low speed channel group
 *
 *    On other targets, GPIO8/9/4/5 are used as the LEDC outputs,
 *    and they are all from the low speed channel group.
 *
 * 5. All the LEDC outputs change the duty repeatedly.
 *
 */

// add function to config channel and timer into one config function
// one function to manipulate duty cycle
// shutdown all and shutdown a single channel functions

void init_ledc_timer_config(){
    /*
     * Prepare and set configuration of timers
     * that will be used by LED Controller
     */
    ledc_timer_config_t ledc_timer = {
        .duty_resolution = LEDC_TIMER_8_BIT, // resolution of PWM duty
        .freq_hz = 5000,                      // frequency of PWM signal
        .speed_mode = LEDC_HIGH_SPEED_MODE,           // timer mode
        .timer_num = 0,            // timer index
        .clk_cfg = LEDC_AUTO_CLK,              // Auto select the source clock
    };
    ledc_timer_config(&ledc_timer);
}

void init_ledc_channel_config(int duty_cycle){
    /*
     * Prepare and set configuration of channel
     * that will be used by LED Controller
     */
    ledc_channel_config_t ledc_channel = {
        .speed_mode = LEDC_HIGH_SPEED_MODE,     //LEDC speed mode HIGH or LOW
        .channel = 0,       // LEDC channel (0 - 7)
        .gpio_num = 18,     // LEDC output gpio number
        .timer_sel = 0,     // selects the timer source of channel (0 - 3)
        .duty = duty_cycle       // [0, (2**duty_resolution)]
    };
    ledc_channel_config(&ledc_channel);
}

void app_main(void)
{
    init_ledc_timer_config();
    init_ledc_channel_config(127);

    vTaskDelay (5000 / portTICK_RATE_MS);
    //ledc_set_duty(LEDC_HIGH_SPEED_MODE, 0, 255);
    init_ledc_channel_config(0);
    vTaskDelay (5000 / portTICK_RATE_MS);
    init_ledc_channel_config(255);
    //ledc_set_duty(LEDC_HIGH_SPEED_MODE, 0, 0);

}