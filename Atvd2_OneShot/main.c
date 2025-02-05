#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "hardware/gpio.h"

#define BUTTON 5
#define LED_GREEN 11
#define LED_RED 12
#define LED_BLUE 13

volatile bool timer_active = false;

// Função para desligar o último LED e liberar o botão
int64_t turn_off_last_led_callback(alarm_id_t id, void *user_data) 
{
    gpio_put(LED_GREEN, 0);
    printf("LED verde desligado.\n");
    printf("Todos os LEDs foram desligados.\n");
    timer_active = false; 
    return 0;
}

// Função para desligar o segundo LED e programar o próximo callback
int64_t turn_off_second_led_callback(alarm_id_t id, void *user_data) 
{
    gpio_put(LED_BLUE, 0);
    printf("LED azul desligado.\n");
    add_alarm_in_ms(3000, turn_off_last_led_callback, NULL, false);
    return 0;
}

// Função para desligar o primeiro LED e programar o próximo callback
int64_t turn_off_first_led_callback(alarm_id_t id, void *user_data) 
{
    gpio_put(LED_RED, 0);
    printf("LED vermelho desligado.\n");
    add_alarm_in_ms(3000, turn_off_second_led_callback, NULL, false);
    return 0;
}

// Callback do botão com debounce
void button_callback(uint gpio, uint32_t events) 
{
    static uint32_t last_press_time = 0;
    uint32_t current_time = to_ms_since_boot(get_absolute_time());

    if (gpio == BUTTON && !timer_active && (current_time - last_press_time > 200)) 
    { 
        last_press_time = current_time; // Atualiza o tempo do último pressionamento
        timer_active = true;

        gpio_put(LED_BLUE, 1);
        gpio_put(LED_GREEN, 1);
        gpio_put(LED_RED, 1);
        printf("Todos os LEDs acesos.\n");

        add_alarm_in_ms(3000, turn_off_first_led_callback, NULL, false);
    }
}

void init() 
{
    stdio_init_all();
    gpio_init(LED_BLUE);
    gpio_init(LED_RED);
    gpio_init(LED_GREEN);
    gpio_init(BUTTON);

    gpio_set_dir(LED_BLUE, GPIO_OUT);
    gpio_set_dir(LED_RED, GPIO_OUT);
    gpio_set_dir(LED_GREEN, GPIO_OUT);
    gpio_set_dir(BUTTON, GPIO_IN);
    gpio_pull_up(BUTTON);

    // Configura interrupção para o botão corretamente
    gpio_set_irq_enabled_with_callback(BUTTON, GPIO_IRQ_EDGE_FALL, true, button_callback);
}

int main() 
{
    init();
    while (1) 
    {
        sleep_ms(1000);
    }
}
