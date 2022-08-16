#ifndef WS2812_CONTROL_H
#define WS2812_CONTROL_H
#include <stdint.h>

#define NUM_LEDS                3       //Number of LEDs in the WS2812 strip
#define LED_RMT_TX_CHANNEL		0       //RMT Channel that will be used to modulate the signal
#define LED_RMT_TX_GPIO			27       //GPIO number where the WS2812 strip is connected
#define T0H                     14      //0 bit high time. Time interval the signal should be high for when a 0 bit is transmitted
#define T1H                     52      //1 bit high time. Time interval the signal should be high when a 1 bit is transmitted
#define TL                      52      //low time for either bit. Time interval the signal should be low followed by the high time for either bit

#define GREEN   0x0A0000
#define RED     0x001500
#define BLUE    0x000015
#define YELLOW  0x0A1200

// This structure is used for indicating what the colors of each LED should be set to.
// There is a 32bit value for each LED. Only the lower 3 bytes are used and they hold the
// Red (byte 2), Green (byte 1), and Blue (byte 0) values to be set.
struct led_state {
    uint32_t leds[NUM_LEDS];
};

// Setup the hardware peripheral. Only call this once.
void Status_LED_control_init(void);

// Update the LEDs to the new state. Call as needed.
// This function will block the current task until the RMT peripheral is finished sending 
// the entire sequence.
void Status_LED_write(struct led_state new_state);

#endif