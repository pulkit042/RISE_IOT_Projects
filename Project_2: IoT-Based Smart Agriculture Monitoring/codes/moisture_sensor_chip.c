#include "wokwi-api.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  pin_t pin;
  float moisture;
} chip_data_t;

void chip_timer_callback(void *data) 
{
  chip_data_t *chip_data = (chip_data_t*)data;
  float moisture = attr_read(chip_data->moisture);
  float volts = 5 * (moisture / 1023.0); // Convert analog input to voltage
  pin_dac_write(chip_data->pin, volts);
}

void chip_init() 
{
  chip_data_t *chip_data = (chip_data_t*)malloc(sizeof(chip_data_t));
  chip_data->moisture = attr_init("moisture", 512.0);  // Default value
  chip_data->pin = pin_init("SIG", ANALOG);

  const timer_config_t config = 
  {
    .callback = chip_timer_callback,
    .user_data = chip_data,
  };

  timer_t timer_id = timer_init(&config);
  timer_start(timer_id, 1000, true);
}
