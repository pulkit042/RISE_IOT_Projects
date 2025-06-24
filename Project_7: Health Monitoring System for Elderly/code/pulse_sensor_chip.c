#include "wokwi-api.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  pin_t pin;
  int pulseValue;
} chip_data_t;

void chip_timer_callback(void *data) {
  chip_data_t *chip_data = (chip_data_t*)data;

  // Read pulseValue in pulseValue variable
  int pulseValue = attr_read(chip_data->pulseValue);
  // Calculate volts
  float volts = (pulseValue*3.3)/ 675;
  // Send volts on the pin
  printf("%f \n", volts);
  pin_dac_write(chip_data->pin, volts);
  
}

void chip_init() {
  printf("Hello from custom chip!\n");
  chip_data_t *chip_data = (chip_data_t*)malloc(sizeof(chip_data_t));

  // Initialize pulseValue
  chip_data->pulseValue = attr_init("pulseValue", 0); 
  // Initialize pin
  chip_data->pin = pin_init("OUT0", ANALOG);
  

  const timer_config_t config = {
    .callback = chip_timer_callback,
    .user_data = chip_data,
  };

  timer_t timer_id = timer_init(&config);
  timer_start(timer_id, 1000, true); 
}
