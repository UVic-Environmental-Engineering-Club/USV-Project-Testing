#include "Status_LED.h"
#include "driver/gpio.h"
#include "i2c.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include "esp_log.h"

#define GREEN 0x0A0000
#define RED 0x001500
#define BLUE 0x000015
#define YELLOW 0x0A1200

sensor_t PCA9554A = {1, 0x3F, 4};

i2c_register_t config = 0x03;
i2c_register_t polarity_inversion = 0x02;

static esp_adc_cal_characteristics_t adc1_chars;
static int adc_val;

void app_main()
{
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
  gpio_set_direction(GPIO_NUM_14, GPIO_MODE_OUTPUT);
  gpio_set_level(GPIO_NUM_14, 1);
  gpio_set_direction(GPIO_NUM_32, GPIO_MODE_OUTPUT);
  gpio_set_level(GPIO_NUM_32, 1);
  gpio_set_direction(GPIO_NUM_18, GPIO_MODE_OUTPUT);
  gpio_set_pull_mode(GPIO_NUM_18, GPIO_PULLDOWN_ENABLE);
  gpio_set_level(GPIO_NUM_18, 0);

  i2c_yeet(PCA9554A, config, 0x00, 8);
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  i2c_yeet(PCA9554A, polarity_inversion, 0xFF, 8);

  int read_raw;
  adc2_config_channel_atten(ADC2_CHANNEL_3, ADC_ATTEN_11db);

  esp_err_t r = adc2_get_raw(ADC2_CHANNEL_3, ADC_WIDTH_12Bit, &read_raw);
  while (r == ESP_OK)
  {
    printf("%d\n", read_raw);
  }
}