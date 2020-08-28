
#ifndef _CONF_H_
#define _CONF_H_

//define
#define IOTDK_MOTER_ID DFSS_GPIO_8B2_ID
#define IOTDK_PWM_1_0 0
#define IOTDK_PWM_1_1 1
#define IOTDK_PWM_2_0 2
#define IOTDK_PWM_2_1 3
#define IOTDK_USI_1_0 4
#define IOTDK_USI_1_1 5
#define IOTDK_USI_2_0 6
#define IOTDK_USI_2_1 7
#define IOTDK_LED_ID DFSS_GPIO_4B2_ID
#define IOTDK_LED_0 0

//#define IOTDK_PWM_ID DFSS_GPIO_8B3_ID
//#define IOTDK_PWM 0

#define IOTDK_MOTER_ID DFSS_GPIO_8B2_ID
#define IOTDK_PWM_1_0 0
#define IOTDK_PWM_1_1 1
#define IOTDK_PWM_2_0 2
#define IOTDK_PWM_2_1 3
#define IOTDK_ENC_1_0 4
#define IOTDK_ENC_1_1 5
#define IOTDK_ENC_2_0 6
#define IOTDK_ENC_2_1 7
#define IOTDK_LED_ID DFSS_GPIO_4B2_ID
#define IOTDK_LED_0 0
#define IOTDK_LED_1 1
#define IOTDK_LED_2 2
#define IOTDK_LED_3 3

#if defined(BOARD_IOTDK) || defined(BOARD_EMSDP)
#define MPU9250_IIC_ID DFSS_IIC_0_ID
#else
#define MPU9250_IIC_ID 0
#endif

typedef enum {
	MOV_STOP,
	MOV_GO_FRONT,
	MOV_TURN_LEFT_90,
	MOV_TURN_LEFT_45,
	MOV_TURN_RIGHT_90,
	MOV_TURN_RIGHT_45,
	MOV_TURN_AROUND
} MOV_DRECT;

#define	DIR_LEFT 0
#define	DIR_RIGHT 1

#define DEG_45 0x2D//45
#define	DEG_90 0x5A//90

/* cmdID type enum use as return of function Spotter_GetResultId() & use in function doRecognize() */
typedef enum {
	CMD_TURN_RIGHT_DEG_45,
	CMD_TURN_RIGHT_DEG_90,
	CMD_TURN_LEFT_DEG_45,
	CMD_TURN_LEFT_DEG_90,
	CMD_BACK2HOME,
	CMD_STOP ,
	CMD_GO_FRONT ,
	CMD_TURN_AROUND ,
	CMD_START_RECORD
} VOICE_CMD;

typedef enum{
    LED_DARK,
    LED_LIGHT,
	LED_DARK_TASK
} LED_MODE;

typedef enum{
	RECON_VOICE,
	BACK2HOME,
	EXE_CMD,
	RECORD_MAP
} WorkStatus;

extern LED_MODE led;
extern void set_work_status_EXE_CMD();

void EMBARC_PRINTF_FlOAT(char* str,float x) ;


#endif  /* _CONF_H_ */