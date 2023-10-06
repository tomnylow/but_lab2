#include <stdint.h>
#include "MDR32F9Qx_port.h"
#include "MDR32F9Qx_rst_clk.h"

#define LED1_PIN    PORT_Pin_2   
#define LED2_PIN    PORT_Pin_3   
#define LED3_PIN    PORT_Pin_5   
#define LED4_PIN    PORT_Pin_1   


volatile static uint8_t but_flag = 0;


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


    PORT_InitTypeDef port_init;
    PORT_StructInit(&port_init);
    port_init.PORT_Pin = PORT_Pin_0;
    port_init.PORT_OE = PORT_OE_IN;
		port_init.PORT_FUNC = PORT_FUNC_ALTER;
    PORT_Init(MDR_PORTA, &port_init);

		NVIC_SetPriority(EXT_INT1_IRQn, 0);
    NVIC_EnableIRQ(EXT_INT1_IRQn);
    
}

void EXT_INT1_IRQHandler(void) {
		but_flag = 1;
    
}
int main() {
	
		uint8_t button_cnt = 0;
    init_leds();
		init_button();
		while(1){
			if (but_flag) {
				
				NVIC_DisableIRQ(EXT_INT1_IRQn);
				for(uint32_t i=0; i<500000; i++);
				but_flag = 0;
				NVIC_EnableIRQ(EXT_INT1_IRQn);
				
        //PORT_ClearITPendingBit(MDR_PORTC, PORT_IT_PIN_0);
				if (!but_flag) {
					button_cnt++;
					if (button_cnt > 16) button_cnt = 0;
					
					PORT_WriteBit(MDR_PORTC, LED1_PIN, (button_cnt & 1) ? 1 : 0);
					PORT_WriteBit(MDR_PORTA, LED2_PIN, (button_cnt & 2) ? 1 : 0);
					PORT_WriteBit(MDR_PORTA, LED3_PIN, (button_cnt & 4) ? 1 : 0);
					PORT_WriteBit(MDR_PORTA, LED4_PIN, (button_cnt & 8) ? 1 : 0);
					
				}
			}
		}
    
}
