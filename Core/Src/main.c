/**
  ******************************************************************************
  * @file    main.c
  * @author  nnd@isep.ipp.pt
  * @version V2.0
  * @date    14/09/2020
  * @brief   SISTR Example project for STM32F103RBT6
  ******************************************************************************
*/

#include <lcd.h>
#include "stm32f10x.h"

/* @PT
 * main.c: configura o GPIOA5 como saÃ­da e activa-a (ligando o LED da placa)
 * Abrir o ficheiro RM0008  - Reference Manual do STM32F103xx.
 * A pÃ¡gina 170 explica o MODE e CNF de cada GPIO.
 * Validar esta configuraÃ§ao usando a perspectiva de Debug do STM32CubeIDE fazendo:
 * Run->Debug; Run-> Resume; Run->Suspend;
 * Duplo click no GPIOA na janela SFR (direita) para obter os valores do microcontrolador.
 *
 * @EN
 * main.c: Configures GPIOA5 as an output and activate it (turns on the LED in the board)
 * Open the file RM0008 - STM32F103xx Reference Manual.
 * The page 170 explains the MODE and CNF for each GPIO.
 * Validate this configuration using the STM32CubeIDE's debug perspective by doing:
 * Run->Debug; Run-> Resume; Run->Suspend;
 * Double-click on GPIOA in the SRF Window (right) to fetch the values from the microcontroller.
 *
 * */

		/* HSE com PLL */      							// freq HSE 12MHz/2MHz * 12 = 72MHz
void RCC_Config_HSE_PLL_Max() {

	// RCC Config HSE + PLL
	ErrorStatus HSEStartUpStatus; 							// variable HSE

	// 1 - Enable HSE, Disable HSI
	RCC_HSICmd(DISABLE); 									// Disable HSI

	// 1.1 - Check if HSE is ON
	HSEStartUpStatus = RCC_WaitForHSEStartUp();
	if(!(HSEStartUpStatus == SUCCESS)) {
		RCC_HSEConfig(RCC_HSE_ON);							// Enable HSE
		while( RCC_GetFlagStatus( RCC_FLAG_HSERDY ) == RESET );

		// 2 - Flash
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable); 	// Buffer prefetch
		FLASH_SetLatency(FLASH_Latency_2); 											// wait states


		// 3- AHB Config
		RCC_HCLKConfig(RCC_SYSCLK_Div1); 						// Prescaler Div1 - HCLK = SYSCLK / Div1 => HCLK = SYSCLK/1


		// 4- APB1, APB2  Prescalers Config
		RCC_PCLK1Config(RCC_HCLK_Div2);							// APB1  max 36MHz  - Freq/Div2
		RCC_PCLK2Config(RCC_HCLK_Div1);							// APB2 max 72MHz  - Freq/Div1

		// 5- PLL config
		RCC_PLLConfig(RCC_PLLSource_HSE_Div2, RCC_PLLMul_12);  	// PLL entry as HSI, and multiplication factor x12;   12/2 * 12 = 72MHz

		// 6- enable PLL
		RCC_PLLCmd(ENABLE);
		while( RCC_GetFlagStatus( RCC_FLAG_PLLRDY ) == RESET );

		// 7- Config SYSCLK
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
		while(RCC_GetSYSCLKSource() != 0x08);					// 0x08 - PLL

	}
}

void blinkWithTIM3()
{
	TIM_Cmd(TIM3, ENABLE);		//Dá enable ao temporizador 3
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);		//Limpa a flag do update event do temporizador 3
	GPIO_SetBits(GPIOB, GPIO_Pin_6);		//Ativa o bit associado ao Pin 6 do GPIOB (LED)
	while(TIM_GetFlagStatus(TIM3,TIM_FLAG_Update) == RESET);		//Espera que haja um update event no temporizador 3 (demora 0.5 segundos)
	TIM_Cmd(TIM3, ENABLE);		//Dá enable ao temporizador 3
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);		//Limpa a flag do update event do temporizador 3
	GPIO_ResetBits(GPIOB, GPIO_Pin_6);		//Desativa o bit associado ao Pin 6 do GPIOB (LED)
	while(TIM_GetFlagStatus(TIM3,TIM_FLAG_Update) == RESET);		//Espera que haja um update event no temporizador 3 (demora 0.5 segundos)
}

void blinkWithTIM4()
{
	TIM_Cmd(TIM4, ENABLE);		//Dá enable ao temporizador 4

	// Ex 4
//	TIM_SetCompare4(TIM4, 5); // 5% 100*.05 = 5
//	TIM_SetCompare4(TIM4, 25); // 25% 100*.25 = 25
//	TIM_SetCompare4(TIM4, 75); // 75% 100*.75 = 75
}


int main(void)
{
//		//	Ex 1
//		//Configuração do Temporizador 3 para base de tempo de 1s
//		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//Ativar o Temporizador 3 do barramento APB1
//
//		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
//
//		TIM_TimeBaseStructure.TIM_Period = 7999;
//		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
//		TIM_TimeBaseStructure.TIM_Prescaler = 8999;
//		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
//
//		TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

		////////////////////////////////////////////////////////////////////////////

//		//	Ex 2
//		//Configuração do Temporizador 3 para base de tempo de 4s
//		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//Ativar o Temporizador 3 do barramento APB1
//
//		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
//
//		TIM_TimeBaseStructure.TIM_Period = 31999;
//		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
//		TIM_TimeBaseStructure.TIM_Prescaler = 8999;
//		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
//
//		TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

		////////////////////////////////////////////////////////////////////////////

//		//	Ex 3
//		//Configuração do Temporizador 4 para base de tempo de 4s
//		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	//Ativar o Temporizador 4 do barramento APB1
//
//		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
//
//		TIM_TimeBaseStructure.TIM_Period = 31999;
//		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
//		TIM_TimeBaseStructure.TIM_Prescaler = 8999;
//		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
//
//		TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
//
//		/* Configura o Canal1 do TIM4 em Toggle Mode */
//		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
//
//		TIM_OCInitTypeDef TIM_OCInitStructure;
//
//		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Toggle;
//		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//		TIM_OCInitStructure.TIM_Pulse = 0x7FF;	/*0 a 65535*/
//		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
//
//		TIM_OC1Init(TIM4, &TIM_OCInitStructure);

		////////////////////////////////////////////////////////////////////////////

		//	Ex 4
		//Configuração do Temporizador 4 para base de tempo de 4s
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	//Ativar o Temporizador 4 do barramento APB1

		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

		TIM_TimeBaseStructure.TIM_Period = 99;
		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
		TIM_TimeBaseStructure.TIM_Prescaler = 719;
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

		TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

		/* Configura o Canal1 do TIM4 em Toggle Mode */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

		TIM_OCInitTypeDef TIM_OCInitStructure;

		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
		TIM_OCInitStructure.TIM_Pulse = 0x7FF;	/*0 a 65535*/
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

		TIM_OC1Init(TIM4, &TIM_OCInitStructure);

		////////////////////////////////////////////////////////////////////////////

//		// Ex 1 & 2 GPIOB6 config
//		// Enable the GPIOB Clock
//		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
//
//		// GPIOB Configuration
//		GPIO_InitTypeDef GPIO_InitStructure;
//		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
//		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//
//		// GPIOB Init
//		GPIO_Init(GPIOB, &GPIO_InitStructure);


		// Ex 3 & 4 GPIOB6 config
		// Enable the GPIOB Clock
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

		// GPIOB Configuration
		GPIO_InitTypeDef GPIO_InitStructure;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

		// GPIOB Init
		GPIO_Init(GPIOB, &GPIO_InitStructure);

		////////////////////////////////////////////////////////////////////////////

		//	Shows CLKs in screen
		RCC_Config_HSE_PLL_Max();
		rcc_lcd_info();


    while(1){
    	//	Ex 1 & 2
//    	blinkWithTIM3();
    	//	Ex 3 & 4
    	blinkWithTIM4();
    }
  }




