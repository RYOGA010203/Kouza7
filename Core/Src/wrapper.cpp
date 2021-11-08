
#include <main.h>
#include "StepperClass.h"			// ステッピングモータを使用するためのライブラリ
#include "stm32f1xx_hal_uart.h"		// uart通信を開始するためのライブラリ

extern UART_HandleTypeDef huart2;
extern uint8_t RxBuffer[8];

extern uint8_t B;
extern uint8_t A;
extern uint8_t X;
extern uint8_t Y;
extern uint8_t RIGHT;
extern uint8_t DOWN;
extern uint8_t LEFT;
extern uint8_t UP;
extern uint8_t R1;
extern uint8_t R2;
extern uint8_t L1;
extern uint8_t L2;
extern uint8_t START;
extern uint8_t BACK;
extern uint8_t RightAxisX;
extern uint8_t RightAxisY;
extern uint8_t LeftAxisX;
extern uint8_t LeftAxisY;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;

// 実体化、使用するステッピングモータの数だけ（最大4つ）
StepperClass stepper1;
//StepperClass stepper2;
//StepperClass stepper3;


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM1)
	{
		// タイマの割り込みで、現在位置を計算
		if(stepper1.rotation_direction == 0)
		{
			stepper1.current_pos++;
		}
		else
		{
			stepper1.current_pos--;
		}
	}
	if(htim->Instance == TIM2)
	{
		/*
		if(stepper2.rotation_direction == 0)
		{
			stepper2.current_pos++;
		}
		else
		{
			stepper2.current_pos--;
		}
		*/
	}
	if(htim->Instance == TIM3)
	{
		/*
		if(stepper3.rotation_direction == 1)
		{
			stepper3.current_pos++;
		}
		else
		{
			stepper3.current_pos--;
		}
		*/
	}
	if(htim->Instance == TIM4)
	{
		stepper1.update();
		//stepper2.update();
		//stepper3.update();
	}
}

void main_cpp(void)
{
	// 初期化
	HAL_UART_Receive_IT(&huart2, RxBuffer, 8);
	HAL_TIM_Base_Start_IT(&htim4);

	// タイマの割り当て
	stepper1.initialize(&htim1);
	//stepper2.initialize(&htim2);
	//stepper3.initialize(&htim3);

	// ステッピングモータの有効化
	stepper1.enable();
	//stepper2.enable();
	//stepper3.enable();

	// 直前のボタンの状態を保存　0:押されてない　1:押されてる
	uint8_t lastA = 0;
	uint8_t lastB = 0;

	// メインループ
	while(1)
	{
		stepper1.target_vel = (int32_t)( (64 - LeftAxisY) * 1600 );
		if(A != lastA)
		{
			if(A)
			{
				stepper1.disable();
			}
			lastA = A;
		}
		if(B != lastB)
		{
			if(B)
			{
				stepper1.enable();
			}
			lastB = B;
		}
	}
}
