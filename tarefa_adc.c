#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"
#include "bibliotecas/ssd1306.h"
#include "bibliotecas/font.h"


#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define ENDERECO 0x3C

#define INTENSIDADE 0.01

#define GPIO_LED_R 13
#define GPIO_LED_G 11
#define GPIO_LED_B 12

#define BUTTON_A 5
#define BUTTON_JOYSTIC 22

#define JOYSTICK_X 26
#define JOYSTICK_Y 27

ssd1306_t ssd; // Inicializa a estrutura do display

void configuracao(){

    gpio_init(GPIO_LED_R);
    gpio_init(GPIO_LED_G);
    gpio_init(GPIO_LED_B);
    gpio_init(BUTTON_A);
    gpio_init(BUTTON_JOYSTIC);

    gpio_set_dir(GPIO_LED_R,GPIO_OUT);
    gpio_set_dir(GPIO_LED_G,GPIO_OUT);
    gpio_set_dir(GPIO_LED_B,GPIO_OUT);
    gpio_set_dir(BUTTON_A,GPIO_IN);
    gpio_set_dir(BUTTON_JOYSTIC,GPIO_IN);

    gpio_pull_up(BUTTON_A);
    gpio_pull_up(BUTTON_JOYSTIC);

    gpio_put(GPIO_LED_R,false);
    gpio_put(GPIO_LED_G,false);
    gpio_put(GPIO_LED_B,false);


    i2c_init(I2C_PORT, 400 * 1000);

    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C); // Set the GPIO pin function to I2C
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C); // Set the GPIO pin function to I2C
    gpio_pull_up(I2C_SDA); // Pull up the data line
    gpio_pull_up(I2C_SCL); // Pull up the clock line
    
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, ENDERECO, I2C_PORT); // Inicializa o display
    ssd1306_config(&ssd); // Configura o display
    ssd1306_send_data(&ssd); // Envia os dados para o display

    // Limpa o display. O display inicia com todos os pixels apagados.
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);

    adc_init();
    adc_gpio_init(JOYSTICK_X);
    adc_gpio_init(JOYSTICK_Y);
}

int main()
{
    stdio_init_all();

    configuracao();

    while (true) {
        printf("Hello, world!\n");
        sleep_ms(1000);
    }
}
