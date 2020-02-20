#include <math.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/rtc_io.h"
#include "esp_log.h"
#include "esp_sleep.h"
#include "soc/rtc.h"
#include "dht.h"

#define uS_TO_MIN_FACTOR 60000000

static const char* TAG = "cooling_main";
static const dht_sensor_type_t sensor_type = DHT_TYPE_AM2301;
static const uint64_t cooldown_sleep_time = 5 * uS_TO_MIN_FACTOR;
static const uint64_t equilibrium_sleep_time = 30 * uS_TO_MIN_FACTOR;  

// gpios
static const gpio_num_t dht_gpio_mirror = 26;
static const gpio_num_t dht_gpio_ambient = 25;
static const gpio_num_t fan_gpio = 33; 

// set as RTC_DATA_ATTR so that pinState will survive deep sleep.
//static RTC_DATA_ATTR bool pinState = 1;


bool near_equilibrium(float mirrorTemp, float ambientTemp) {
    return fabs(ambientTemp - mirrorTemp) < 1.5;
}
void put_to_sleep(uint64_t time_to_sleep) {
    ESP_LOGI(TAG, "Putting ESP32 to sleep for %" PRIu64 " minutes\n", time_to_sleep / uS_TO_MIN_FACTOR);
    esp_sleep_enable_timer_wakeup(time_to_sleep);
    esp_deep_sleep_start();
}
void toggle_fan(int pinState) {
    rtc_gpio_hold_dis(fan_gpio);
    rtc_gpio_set_level(fan_gpio, pinState);
    rtc_gpio_hold_en(fan_gpio); // hold the current pin state to survive through deep sleep
}
void init_fan_rtc_gpio() {
    gpio_pad_select_gpio(fan_gpio);

    // set up fan gpio as an rtc gpio to survive deep sleep 
    rtc_gpio_init(fan_gpio);
    rtc_gpio_set_direction(fan_gpio, GPIO_MODE_OUTPUT);
    rtc_gpio_set_level(fan_gpio, 1);
}
// Get the internal and external temps from DHT sensors and compare the values.
// When at or near equilibrium, we interrupt and kill the fan.
void DHT_temps_task(void *pvParameters) {
    
    bool pinState = 1; 

    float mirrorTemp = 0.0, ambientTemp = 0.0;
    float mirrorHum = 0.0, ambientHum = 0.0;
    
    while(1) {
        esp_err_t mirror_result = dht_read_float_data(sensor_type, dht_gpio_mirror, &mirrorHum, &mirrorTemp);
        esp_err_t ambient_result = dht_read_float_data(sensor_type, dht_gpio_ambient, &ambientHum, &ambientTemp);
        vTaskDelay(3000 / portTICK_PERIOD_MS);

        if(!mirror_result == ESP_OK)
            ESP_LOGI(TAG, "Error reading mirror temp/hum");
        else if(!ambient_result == ESP_OK)
            ESP_LOGI(TAG, "Error reading ambient temp/hum");
        else {
            ESP_LOGI(TAG, "-----------\n");
            ESP_LOGI(TAG, "Mirror temp: %f\n", mirrorTemp);
            ESP_LOGI(TAG, "Ambient temp: %f\n", ambientTemp);
        }

        if(near_equilibrium(mirrorTemp, ambientTemp)) {
            ESP_LOGI(TAG, "Equilibrium reached. Switching to maintenance mode.\n");
            pinState = 0;
            toggle_fan(pinState);
            put_to_sleep(equilibrium_sleep_time);
        }
        else {     
            ESP_LOGI(TAG, "In cooldown mode...");
            toggle_fan(pinState);
            put_to_sleep(cooldown_sleep_time);
        }
    }
}
void app_main(void) {
    // set up fan and delay before starting main temp task
    init_fan_rtc_gpio();
    vTaskDelay(3000 / portTICK_PERIOD_MS);
    xTaskCreatePinnedToCore(&DHT_temps_task, "temps_equal_task", configMINIMAL_STACK_SIZE * 3, NULL, 5, NULL, 0);
}







