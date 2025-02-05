#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"

#define LED_PIN_RED 11
#define LED_PIN_BLUE 12
#define LED_PIN_GREEN 13

// Variável para controlar o estado do LED.
int led_state = 0;

// Callback que será chamado repetidamente de acordo com o temporizador.
bool repeating_timer_callback(struct repeating_timer *t) {

    // Atualiza a cor do LED com base no estado atual.
    if (led_state == 0) {
        gpio_put(LED_PIN_RED, true);
        gpio_put(LED_PIN_BLUE, false);
        gpio_put(LED_PIN_GREEN, false);
        printf("PARE! SINAL VERMELHO!\n");
    } else if (led_state == 1) { // Para rodar o amarelo na simulação.
        gpio_put(LED_PIN_RED, false);
        gpio_put(LED_PIN_BLUE, true);
        gpio_put(LED_PIN_GREEN, false);
        printf("ATENÇÃO! SINAL AMARELO!\n");
    } /*else if (led_state == 1) { // Para rodar o amarelo na placa BitDogLab.
        gpio_put(LED_PIN_RED, true);
        gpio_put(LED_PIN_BLUE, false);
        gpio_put(LED_PIN_GREEN, true);
        printf("ATENÇÃO! SINAL AMARELO!\n");} */
    else {
        gpio_put(LED_PIN_RED, false);
        gpio_put(LED_PIN_BLUE, false);
        gpio_put(LED_PIN_GREEN, true);
        printf("SIGA! SINAL VERDE!\n");
    }

    // Atualiza o estado do LED para a próxima cor.
    led_state = (led_state + 1) % 3;

    return true;
}

int main() {
    stdio_init_all();

    gpio_init(LED_PIN_RED);
    gpio_set_dir(LED_PIN_RED, true);
    gpio_init(LED_PIN_BLUE);
    gpio_set_dir(LED_PIN_BLUE, true);
    gpio_init(LED_PIN_GREEN);
    gpio_set_dir(LED_PIN_GREEN, true);

    // Estrutura de um temporizador para repetição.
    struct repeating_timer timer;

    // Configuração do temporizador para chamar a função de callback a cada 3 segundos.
    add_repeating_timer_ms(3000, repeating_timer_callback, NULL, &timer);

    while (true) {
        printf("O semaforo mudará em 3...\n");
        sleep_ms(1000);
        printf("O semaforo mudará em 2...\n");
        sleep_ms(1000);
        printf("O semaforo mudará em 1...\n");
        sleep_ms(1000);
    }

    return 0;
}
