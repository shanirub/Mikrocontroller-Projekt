#include <Arduino.h>
#include <driver/adc.h>
#include <ESP32AnalogRead.h>
// ESP32AnalogRead adc;

#define j_sw 33   // ADC1 5
adc1_channel_t adc_j_sw = ADC1_CHANNEL_5;
#define j_y 32    // ADC1 4
adc1_channel_t adc_j_y = ADC1_CHANNEL_4;
#define j_x 35    // ADC1 7
adc1_channel_t adc_j_x = ADC1_CHANNEL_7;
#define led 22

int x_value = 0;
int y_value = 0;
int sw_value = 0;

void setup() {
  Serial.begin(9600);
  pinMode(led, OUTPUT);
  pinMode(j_sw, INPUT_PULLUP);
  pinMode(j_y, ANALOG);
  pinMode(j_x, ANALOG);
}

void loop() {

  /*
  adc1_config_width(ADC_WIDTH_BIT_12);
  adc1_config_channel_atten(adc_j_x, ADC_ATTEN_MAX);
  adc1_config_channel_atten(adc_j_y, ADC_ATTEN_MAX);
  adc1_config_channel_atten(adc_j_sw, ADC_ATTEN_MAX);
  
  x_value = adc1_get_raw(adc_j_x);
  y_value = adc1_get_raw(adc_j_y);
  sw_value = adc1_get_raw(adc_j_sw);
  */

  // adc.attach(j_x);
  // x_value = adc.readVoltage();
  x_value = analogRead(j_x);
  y_value = analogRead(j_y);
  sw_value = digitalRead(j_sw);

  Serial.print("X ");
  Serial.print(x_value);
  Serial.print("| Y ");
  Serial.print(y_value);
  Serial.print("| SW ");
  Serial.print(sw_value);
  Serial.println("");
  delay(500);
}