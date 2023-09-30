#include <stdint.h>
#include "MDR32F9Qx_port.h"
#include "MDR32F9Qx_rst_clk.h"
#include "MDR32F9Qx_nvic.h"


#define LED1_PIN    PORT_Pin_2   
#define LED2_PIN    PORT_Pin_3   
#define LED3_PIN    PORT_Pin_1   
#define LED4_PIN    PORT_Pin_5   


#define LED_ON      1
#define LED_OFF     0

void delay(uint32_t delay_count) {
    for (volatile uint32_t i = 0; i < delay_count; ++i) {
    
    }
}

void init_leds() {

    RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTC, ENABLE);
    RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTA, ENABLE);


    PORT_InitTypeDef port_c_init;
    PORT_InitTypeDef port_a_init;
    PORT_StructInit(&port_c_init);
    PORT_StructInit(&port_a_init);

    port_c_init.PORT_Pin = LED1_PIN;
    port_c_init.PORT_OE = PORT_OE_OUT;
    PORT_Init(MDR_PORTC, &port_c_init);
	

    port_a_init.PORT_Pin = LED2_PIN | LED3_PIN | LED4_PIN;
    port_a_init.PORT_OE = PORT_OE_OUT;
    PORT_Init(MDR_PORTA, &port_a_init);
}
void init_button() {

    RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTC, ENABLE);


    PORT_InitTypeDef port_init;
    PORT_StructInit(&port_init);
    port_init.PORT_Pin = PORT_Pin_0;
    port_init.PORT_OE = PORT_OE_IN;
    PORT_Init(MDR_PORTC, &port_init);


    PORT_ITConfig(MDR_PORTC, PORT_Pin_0, ENABLE);


    NVIC_EnableIRQ(PORTC_IRQn);
    NVIC_SetPriority(PORTC_IRQn, 1);
}

void PORTC_IRQHandler(void) {

    if (PORT_GetITStatus(MDR_PORTC, PORT_IT_PIN_0) == 0) {

        //PORT_ClearITPendingBit(MDR_PORTC, PORT_IT_PIN_0);
     
        button_counter++;

        PORT_WriteBit(MDR_PORTA, LED1_PIN, (button_counter & 1) ? 1 : 0);
        PORT_WriteBit(MDR_PORTA, LED2_PIN, (button_counter & 2) ? 1 : 0);
        PORT_WriteBit(MDR_PORTA, LED3_PIN, (button_counter & 4) ? 1 : 0);
        PORT_WriteBit(MDR_PORTA, LED4_PIN, (button_counter & 8) ? 1 : 0);
    }
}
int main() {
    // ????????????? ???????????
    init_leds();

    while (1) {
        
    }
}
