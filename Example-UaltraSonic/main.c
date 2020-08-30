#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "embARC.h"
#include "embARC_debug.h"
#define BOARD_CPU_CLOCK CLK_CPU
#define MAX_COUNT 0x0fffffff
#define IOTDK_USI_ID DFSS_GPIO_8B2_ID
#define IOTDK_USI_1_0 0
#define IOTDK_USI_1_1 3
static void GPIO_INIT();
static void USITran(); //for timer0
float DistenceDetect();
void DelayFuntion();
void PrintDistence();
static DEV_GPIO_PTR gpio_USI;
static DEV_PWM_TIMER_PTR pwm_timer_moter;
int IntTime = 0;
static uint32_t USI_1 = 0;
int DelayOn = 0;
int IntTimeTotal = 0;

int main(void)
{

	GPIO_INIT();
	while (1)
	{
		/**
		 * 印出透過超音波測距得到的公分數
		 * {
		 */
		printf("Distence = %f\n", DistenceDetect());
		/**
		 * }
		 */
	}

Exit:
	exit(0);
}

void GPIO_INIT()
{
	gpio_USI = gpio_get_dev(IOTDK_USI_ID);
	gpio_USI->gpio_open((1 << IOTDK_USI_1_0));
	gpio_USI->gpio_open((1 << IOTDK_USI_1_1));

	//UltraSonic
	gpio_USI->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)(1 << IOTDK_USI_1_0));
	gpio_USI->gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT, (void *)(1 << IOTDK_USI_1_1));
}

//Detect with USI_1
float DistenceDetect()
{
	/* 設定gpio_USI port上的IOTDK_USI_1_0腳位為輸出 */
	gpio_USI->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)(1 << IOTDK_USI_1_0));

	/* 停止TIMER_0以避免之前被啟動過*/
	timer_stop(TIMER_0);
	/* 設定TIMER_0的ISR為USITran */
	int_handler_install(INTNO_TIMER0, USITran);
	/* 設定TIMER_0中斷優先程度為最小 */
	int_pri_set(INTNO_TIMER0, INT_PRI_MIN);
	/* 啟動TIMER_0中斷 */
	int_enable(INTNO_TIMER0);
	/* 啟動TIMER_0，帶有中斷，同時每數到1E-6*CPU Cycle中斷一次 */
	/* 因為TIMER_0每個CPU Cycle都會增加1，所以要數到1E-6*CPU Cycle就經過了1E-6秒 */
	timer_start(TIMER_0, TIMER_CTRL_IE, 0.000001 * BOARD_CPU_CLOCK);

	gpio_USI->gpio_write(0 << IOTDK_USI_1_0, 1 << IOTDK_USI_1_0);
	// IntTimeTotal=5;
	IntTimeTotal = 2;
	DelayFuntion();

	/* 設定gpio_USI port上的IOTDK_USI_1_0腳位輸出高電位 */
	gpio_USI->gpio_write(1 << IOTDK_USI_1_0, 1 << IOTDK_USI_1_0);
	// IntTimeTotal=10;
	/* 設定等待5微秒 */
	IntTimeTotal = 5;
	/* 等待涵式 */
	DelayFuntion();
	/* 設定gpio_USI port上的IOTDK_USI_1_0腳位輸出低電位 */
	gpio_USI->gpio_write(0 << IOTDK_USI_1_0, 1 << IOTDK_USI_1_0);
	/* 暫停TIMER_0 */
	timer_stop(TIMER_0);

	// _arc_aux_write(AUX_TIMER0_CTRL, 0);
	// _arc_aux_write(AUX_TIMER0_LIMIT,0);
	// _arc_aux_write(AUX_TIMER0_CNT, 0);
	// _arc_aux_write(AUX_TIMER0_LIMIT, MAX_COUNT);
	// _arc_aux_write(AUX_TIMER0_CTRL, TIMER_CTRL_NH);

	/**
	 *  將gpio_USI port上的IOTDK_USI_1_0腳位設定為輸入
	 * {
	 */
	gpio_USI->gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT, (void *)(1 << IOTDK_USI_1_0));
	/**
	 * }
	 */

	/**
	 * 開啟TIMER_0，模式設定為Not Halted(TIMER_CTRL_NH)，上限數值設為MAX_COUNT
	 * {
	 */
	timer_start(TIMER_0, TIMER_CTRL_NH, MAX_COUNT);
	/**
	 * }
	 */

	/**
	 * 設定一個變數，透過變數讀取gpio_USI上的IOTDK_USI_1_0腳位的電位
	 * 如果電位為0就持續執行迴圈等待
	 * {
	 */
	USI_1 = 0;
	while (USI_1 == 0)
	{
		// gpio_USI->gpio_read(&USI_1, 1 << IOTDK_USI_1_1);
		gpio_USI->gpio_read(&USI_1, 1 << IOTDK_USI_1_0);
	}
	/**
	 * }
	 */

	/**
	 * 利用一個變數紀錄TIMER_0目前的計數，作為開始的計數
	 * 如果電位為1就持續執行迴圈等待
	 * {
	 */
	uint32_t start_cnt = _arc_aux_read(AUX_TIMER0_CNT);
	while (USI_1 != 0)
	{
		// gpio_USI->gpio_read(&USI_1, 1 << IOTDK_USI_1_1);
		gpio_USI->gpio_read(&USI_1, 1 << IOTDK_USI_1_0);
	}
	/**
	 * }
	 */

	/**
	 * 再用第二個變數紀錄TIMER_0目前的技術，作為結束的計數
	 * 透過兩個計數相差的數量除以CPU Cycle(BOARD_CPU_CLOCK)得到秒數
	 * 再乘以340(m/s)*100(cm/m)/2(來回)得到公分數後回傳
	 * {
	 */
	uint32_t end_cnt = _arc_aux_read(AUX_TIMER0_CNT);
	uint32_t time = (end_cnt - start_cnt);
	float dis = ((float)time / 144000000) * 17000;
	return dis;
	/**
	 * }
	 */
}

void DelayFuntion()
{
	DelayOn = 1;
	while (DelayOn == 1)
	{
		EMBARC_PRINTF(".\n");
	}
}

static void USITran()
{
	timer_int_clear(TIMER_0);
	if (DelayOn == 1)
	{
		IntTime++;
		if (IntTime == IntTimeTotal)
		{
			IntTime = 0;
			DelayOn = 0;
			IntTimeTotal = 0;
		}
	}
}
