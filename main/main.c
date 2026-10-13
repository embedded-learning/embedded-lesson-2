#include <stdbool.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_random.h"
#include <stdio.h>

#define RED_LED GPIO_NUM_15
#define BLUE_LED GPIO_NUM_16
#define BLINK_MODE_BUTTON GPIO_NUM_21
#define BUTTON_ON_CLICK_DELAY_MS 50
#define LED_OFF 0
#define LED_ON 1
#define FAST_MODE 200
#define SLOW_MODE 1000

void separeteSlowMode();
void combineFastMode();
typedef enum {
    MODE_SLOW,
    MODE_FAST
} mod_t;
volatile mod_t mode = MODE_SLOW;
void ledHandlerTask(void *taskParameter);
void buttonHandlerTask(void *taskParameter);

void app_main(void) {

    gpio_reset_pin(RED_LED);
    gpio_reset_pin(BLUE_LED);
    gpio_reset_pin(BLINK_MODE_BUTTON);

    gpio_set_direction(BLINK_MODE_BUTTON, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BLINK_MODE_BUTTON, GPIO_PULLUP_ONLY);

    gpio_set_direction(RED_LED, GPIO_MODE_OUTPUT);
    gpio_set_direction(BLUE_LED, GPIO_MODE_OUTPUT);

    gpio_set_level(BLUE_LED, LED_OFF);
    gpio_set_level(RED_LED, LED_OFF);

    xTaskCreate( buttonHandlerTask, "CLICK_TASK", 2048, NULL, 20, NULL);
    xTaskCreate( ledHandlerTask, "BLINK_TASK", 2048, NULL, 10, NULL);

}

void ledHandlerTask(void *taskParameter){
    while (1) {
    if (mode == MODE_SLOW) {
        separeteSlowMode();
    }else {
        combineFastMode();
    }
}}

void buttonHandlerTask(void *taskParameter){
    while (1) {
        if(gpio_get_level(BLINK_MODE_BUTTON) == 0){
            vTaskDelay(pdMS_TO_TICKS(BUTTON_ON_CLICK_DELAY_MS));
            if (gpio_get_level(BLINK_MODE_BUTTON) == 0) {
                mode = !mode;
                while(gpio_get_level(BLINK_MODE_BUTTON) == 0){
                  vTaskDelay(pdMS_TO_TICKS(20));
                }
            }
        }
        vTaskDelay(pdMS_TO_TICKS(20));
        }
}

void combineFastMode(){
    gpio_set_level(BLUE_LED, LED_ON);
    gpio_set_level(RED_LED, LED_ON);
    vTaskDelay(pdMS_TO_TICKS(FAST_MODE));
    gpio_set_level(BLUE_LED, LED_OFF);
    gpio_set_level(RED_LED, LED_OFF);
    vTaskDelay(pdMS_TO_TICKS(FAST_MODE));
}

void separeteSlowMode(){
    gpio_set_level(BLUE_LED, LED_ON);
    gpio_set_level(RED_LED, LED_OFF);
    vTaskDelay(pdMS_TO_TICKS(SLOW_MODE));
    gpio_set_level(BLUE_LED, LED_OFF);
    gpio_set_level(RED_LED, LED_ON);
    vTaskDelay(pdMS_TO_TICKS(SLOW_MODE));
}


