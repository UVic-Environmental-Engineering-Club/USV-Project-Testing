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

/**********************************************************************************************************************************************
* This function sets up a GPIO pin to output a specific frequency with a duty cycle of 0
* timer resolution is the bit resolution of the PWM duty cycle
* timer source is the source of the channel (0 - 3)
* speed mode is either LEDC_HIGH_SPEED_MODE or LEDC_LOW_SPEED_MODE
* channel is the LEDC channel to use (0 - 7)
**********************************************************************************************************************************************/

void PWM_config_init(ledc_timer_bit_t timer_resolution, int frequency, int timer_source, ledc_mode_t speed_mode, int gpio_number, int channel){
    /*
     * Prepare and set configuration of timers
     * that will be used by LED Controller
     */
    ledc_timer_config_t ledc_timer = {
        .duty_resolution = timer_resolution, // resolution of PWM duty
        .freq_hz = frequency,                      // frequency of PWM signal
        .speed_mode = speed_mode,           // timer mode
        .timer_num = timer_source,            // selects the timer source of channel (0 - 3)
        .clk_cfg = LEDC_AUTO_CLK,              // Auto selects the source clock
    };

    /*
     * Prepare and set configuration of channel
     * that will be used by LED Controller
     */
    ledc_channel_config_t ledc_channel = {
        .speed_mode = speed_mode,     //LEDC speed mode HIGH or LOW
        .channel = channel,       // LEDC channel (0 - 7)
        .gpio_num = gpio_number,     // LEDC output gpio number
        .timer_sel = timer_source,     // selects the timer source of channel (0 - 3)
        .duty = 0       // [0, (2**duty_resolution)]
    };

    ledc_timer_config(&ledc_timer);
    ledc_channel_config(&ledc_channel);
}

/**********************************************************************************************************************************************
* This function updates the duty cycle of the PWM pin attached to the specific LEDC channel and GPIO
**********************************************************************************************************************************************/

void PWM_duty_cycle_update(int channel, int gpio_num, int duty_cycle){
    /*
     * Prepare and set configuration of channel
     * that will be used by LED Controller
     */
    ledc_channel_config_t ledc_channel = {
        .channel = channel,       // LEDC channel (0 - 7)
        .gpio_num = gpio_num,     // LEDC output gpio number
        .duty = duty_cycle       // [0, (2**duty_resolution)]
    };
    ledc_channel_config(&ledc_channel);
}

/**********************************************************************************************************************************************
* This function shuts down the duty cycle of the PWM pin attached to the specific LEDC channel and GPIO
**********************************************************************************************************************************************/

void PWM_duty_cycle_reset(int channel, int gpio_num){
    /*
     * Prepare and set configuration of channel
     * that will be used by LED Controller
     */
    ledc_channel_config_t ledc_channel = {
        .channel = channel,       // LEDC channel (0 - 7)
        .gpio_num = gpio_num,     // LEDC output gpio number
        .duty = 0       // [0, (2**duty_resolution)]
    };
    ledc_channel_config(&ledc_channel);
}

/**********************************************************************************************************************************************
* This function shuts down the duty cycle of all LEDC channels which theoretically shuts down all GPIOs PWMs
**********************************************************************************************************************************************/

void PWM_shutdown(){
    for (size_t i = 0; i < 8; i++)
    {
        ledc_channel_config_t ledc_channel = {
        .channel = i,       // LEDC channel (0 - 7)
        .duty = 0       // [0, (2**duty_resolution)]
        };
        ledc_channel_config(&ledc_channel);
    }  
}
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