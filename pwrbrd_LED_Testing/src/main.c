#include "Status_LED.h"
#include "driver/gpio.h"

#define GREEN   0x0A0000
#define RED     0x001500
#define BLUE    0x000015
#define YELLOW  0x0A1200

void app_main() {
  Status_LED_control_init();

  struct led_state new_state;
  new_state.leds[0] = RED;
  new_state.leds[1] = YELLOW;
  new_state.leds[2] = GREEN;

  Status_LED_write(new_state);

  gpio_set_direction(GPIO_NUM_2, GPIO_MODE_OUTPUT);
  gpio_set_level(GPIO_NUM_2, 1);
  gpio_set_direction(GPIO_NUM_4, GPIO_MODE_OUTPUT);
  gpio_set_level(GPIO_NUM_4, 1);
  gpio_set_direction(GPIO_NUM_5, GPIO_MODE_OUTPUT);
  gpio_set_level(GPIO_NUM_5, 1);
}