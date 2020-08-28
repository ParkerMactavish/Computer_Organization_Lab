#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "embARC.h"
#include "embARC_debug.h"
#define BOARD_CPU_CLOCK			CLK_CPU
#define MAX_COUNT 0x0fffffff
#define IOTDK_USI_ID DFSS_GPIO_8B2_ID
#define IOTDK_USI_1_0 0
#define IOTDK_USI_1_1 3
static void GPIO_INIT();
static void USITran();//for timer0 
float DistenceDetect();
void DelayFuntion();
void PrintDistence();
static DEV_GPIO_PTR gpio_USI;
static DEV_PWM_TIMER_PTR pwm_timer_moter;
int INTime=0;
static uint32_t USI_1=0 ;
int DelayOn=0;
int INTtime=0;

int main(void)
{

	GPIO_INIT();
	while(1){
		printf("Distence = %f\n",DistenceDetect());
	}

Exit:
	exit(0);
}

void GPIO_INIT(){
	gpio_USI=gpio_get_dev(IOTDK_USI_ID);
	gpio_USI->gpio_open((1 << IOTDK_USI_1_0));
	gpio_USI->gpio_open((1 << IOTDK_USI_1_1));

//UltraSonic
	gpio_USI->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)(1 << IOTDK_USI_1_0));
	gpio_USI->gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT, (void *)(1 << IOTDK_USI_1_1));


}

//Detect with USI_1
float DistenceDetect(){
	
	gpio_USI->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)(1 << IOTDK_USI_1_0));
	timer_stop(TIMER_0); // Stop it first since it might be enabled before 
	int_handler_install(INTNO_TIMER0,USITran);
	int_pri_set(INTNO_TIMER0, INT_PRI_MIN);
	int_enable(INTNO_TIMER0);
	timer_start(TIMER_0, TIMER_CTRL_IE, 0.000001 * BOARD_CPU_CLOCK);
		
	gpio_USI->gpio_write(0<< IOTDK_USI_1_0, 1 << IOTDK_USI_1_0);
	// INTtime=5;
	INTtime=2;
	DelayFuntion();
	gpio_USI->gpio_write(1<< IOTDK_USI_1_0, 1 << IOTDK_USI_1_0);
	// INTtime=10;
	INTtime=5;
	DelayFuntion();
	gpio_USI->gpio_write(0<< IOTDK_USI_1_0, 1 << IOTDK_USI_1_0);
	timer_stop(TIMER_0);
	// _arc_aux_write(AUX_TIMER0_CTRL, 0);
	// _arc_aux_write(AUX_TIMER0_LIMIT,0);
	// _arc_aux_write(AUX_TIMER0_CNT, 0);
	// _arc_aux_write(AUX_TIMER0_LIMIT, MAX_COUNT);
	// _arc_aux_write(AUX_TIMER0_CTRL, TIMER_CTRL_NH);

	gpio_USI->gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT, (void *)(1 << IOTDK_USI_1_0));
	timer_start(TIMER_0, TIMER_CTRL_NH, MAX_COUNT);

	USI_1=0;
	while(USI_1==0){
		// gpio_USI->gpio_read(&USI_1, 1 << IOTDK_USI_1_1);
		gpio_USI->gpio_read(&USI_1, 1 << IOTDK_USI_1_0);
	}
	uint32_t start_cnt = _arc_aux_read(AUX_TIMER0_CNT);
	while(USI_1!=0){
		// gpio_USI->gpio_read(&USI_1, 1 << IOTDK_USI_1_1);
		gpio_USI->gpio_read(&USI_1, 1 << IOTDK_USI_1_0);
	}

	uint32_t end_cnt = _arc_aux_read(AUX_TIMER0_CNT);
	uint32_t time = (end_cnt-start_cnt);
	float dis=((float)time/144000000)*17000;
	return dis;
}

//For Delay Use
void DelayFuntion(){
	DelayOn=1;
	while(DelayOn==1){
		EMBARC_PRINTF(".\n");
	}
		
}

static void USITran(){
	timer_int_clear(TIMER_0);
	if(DelayOn==1){
		INTime++;
		if(INTime==INTtime){
			INTime=0;
			DelayOn=0;
			INTtime=0;
		}
	}
}

