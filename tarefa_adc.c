#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"
#include "hardware/timer.h"
#include "bibliotecas/ssd1306.h"
#include "bibliotecas/font.h"
#include "hardware/pwm.h"
#include "pico/bootrom.h"
#include <math.h>


#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define ENDERECO 0x3C

#define INTENSIDADE 0.01

#define GPIO_LED_R 13
#define GPIO_LED_G 11
#define GPIO_LED_B 12

#define BUTTON_A 5
#define BUTTON_B 6
#define BUTTON_JOYSTIC 22

#define JOYSTICK_Y 26
#define JOYSTICK_X 27

#define PWM_WRAP 2048
#define TOLERANCIA 15

int16_t ajuste_fino_x = 0;
int16_t ajuste_fino_y = 0;

volatile bool LEDs_PWM_ativados = true;
volatile bool borda_dupla=false;
float xp=0.0,yp=0.0;

ssd1306_t ssd; // Inicializa a estrutura do display
volatile uint32_t last_time=0;

void pinta_borda_display(){
    ssd1306_rect(&ssd,3,3,WIDTH-5,HEIGHT-5,true,false);
    if(borda_dupla){
        ssd1306_rect(&ssd,1,1,WIDTH-3,HEIGHT-3,true,false);
        ssd1306_rect(&ssd,0,0,WIDTH-1,HEIGHT-1,true,false);
    }
    ssd1306_send_data(&ssd);
}

void gpio_irq_handler(uint gpio, uint32_t events){
    uint32_t atual = to_ms_since_boot(get_absolute_time());
    if((atual-last_time)>200){
        last_time = atual;
        if(gpio==BUTTON_A){
            LEDs_PWM_ativados = !LEDs_PWM_ativados;
        }
        if(gpio==BUTTON_B){
            reset_usb_boot(0, 0);
        }
        if(gpio==BUTTON_JOYSTIC){
            gpio_put(GPIO_LED_G,!gpio_get(GPIO_LED_G));
            borda_dupla = !borda_dupla;
            pinta_borda_display();
        }
    }
}

void configuracao(){

    gpio_init(GPIO_LED_G);
    gpio_init(BUTTON_A);
    gpio_init(BUTTON_B);
    gpio_init(BUTTON_JOYSTIC);

    gpio_set_dir(GPIO_LED_G,GPIO_OUT);
    gpio_set_dir(BUTTON_A,GPIO_IN);
    gpio_set_dir(BUTTON_B, GPIO_IN);
    gpio_set_dir(BUTTON_JOYSTIC,GPIO_IN);

    gpio_pull_up(BUTTON_A);
    gpio_pull_up(BUTTON_B);
    gpio_pull_up(BUTTON_JOYSTIC);

    gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(BUTTON_B, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(BUTTON_JOYSTIC,GPIO_IRQ_EDGE_FALL,true,&gpio_irq_handler);

    gpio_put(GPIO_LED_G,false);


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

    gpio_set_function(GPIO_LED_R, GPIO_FUNC_PWM);
    gpio_set_function(GPIO_LED_B,GPIO_FUNC_PWM);
    uint slice1 = pwm_gpio_to_slice_num(GPIO_LED_R);
    pwm_set_wrap(slice1,PWM_WRAP);
    pwm_set_clkdiv(slice1,4);
    pwm_set_enabled(slice1,true);
    uint slice2 = pwm_gpio_to_slice_num(GPIO_LED_B);
    pwm_set_wrap(slice2,PWM_WRAP);
    pwm_set_clkdiv(slice2,4);
    pwm_set_enabled(slice2,true);
}

uint16_t getIntensidadeLED(int16_t medida){
    
    if(medida>2048){
        return medida-2048;
    }
    return 2048-medida;
}

void pintaQuadradoDisplay(float x, float y){
    ssd1306_fill(&ssd,false);
    pinta_borda_display();
    uint8_t pixel_x = (uint8_t)(x*(float)WIDTH);
    uint8_t pixel_y = (uint8_t)(HEIGHT-y*(float)HEIGHT);
    if(pixel_x<0){pixel_x=0;}
    if(pixel_x>(WIDTH-8)){pixel_x = WIDTH-8;}
    if(pixel_y<0){pixel_y=0;}
    if(pixel_y>(HEIGHT-8)){pixel_y=HEIGHT-8;}
    ssd1306_rect(&ssd,pixel_y,pixel_x,8,8,true,true);
    ssd1306_send_data(&ssd);
}

int main()
{
    stdio_init_all();

    configuracao();
    adc_select_input(1);
    ajuste_fino_x = 2048-adc_read();
    adc_select_input(0);
    ajuste_fino_y = 2048-adc_read();
    pwm_set_gpio_level(GPIO_LED_R,0);
    pwm_set_gpio_level(GPIO_LED_B,0);

    while (true) {
        adc_select_input(1);
        
        uint16_t leituraX = adc_read();
        xp = ((float)leituraX)/4095.0; 
        adc_select_input(0);
        uint16_t leituraY = adc_read();
        yp = ((float)leituraY)/4095.0;
        pintaQuadradoDisplay(xp,yp);
        uint16_t corrigidoX = getIntensidadeLED(leituraX+ajuste_fino_x);
        uint16_t corrigidoY = getIntensidadeLED(leituraY+ajuste_fino_y);
        if((corrigidoX>TOLERANCIA)&&(corrigidoY>TOLERANCIA)&&(LEDs_PWM_ativados)){
            pwm_set_gpio_level(GPIO_LED_R,corrigidoX);
            pwm_set_gpio_level(GPIO_LED_B,corrigidoY);
        }else{
            pwm_set_gpio_level(GPIO_LED_R,0);
            pwm_set_gpio_level(GPIO_LED_B,0);
        }
        
        sleep_ms(100);
    }
}
