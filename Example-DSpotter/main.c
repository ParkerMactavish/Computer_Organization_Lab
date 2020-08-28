#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Spotter.h"
#include "recorder.h"

#include "embARC.h"
DEV_GPIO_PTR gpio_8b2;

static int doRecognize(){
	int nRet;
	nRet= Recorder_Init();
	int16_t *pData;
	if(nRet!= 0){
		printf("record init fail: %d\r\n", nRet);
		return 1;
	}
	Spotter_Reset();
	Recorder_Start();
	while(1){
		pData= Recorder_GetBuffer();
		if(pData== 0) continue;
		nRet= Spotter_AddSample(pData, Recorder_GetBufferSize());
		if(nRet== SPOTTER_SUCCESS){
			printf("Spotter detect result gpID=%d cmdID=%d\n", Spotter_GetResultGroupId(), Spotter_GetResultId());//辨識成功會在這邊顯示
			gpio_8b2->gpio_write(1 << 4, 1 << 4);
			board_delay_ms(1000,1);//LED 亮一秒
			gpio_8b2->gpio_write(0 << 4, 1 << 4);
		}else if(nRet!= SPOTTER_ERR_NeedMoreSample){
			printf("recognize fail: %d\n" , nRet);
			break;
		}
		Recorder_NextBuffer();
	}
	Recorder_Stop();
	Recorder_Release();
	return nRet;
}

#include "CYBase.mod.h"
#include "Group_1.mod.h"
#include "Group_2.mod.h"

//use fixed size memory to avoid heap too small
#define pMem ((void*)0x30000000)
#define nMemSize (0x00020000)

int main(int argc, char **argv){
	int nRet;
	//for LED use
	gpio_8b2 = gpio_get_dev(DFSS_GPIO_8B2_ID); 
	gpio_8b2->gpio_open(1 << 4 );
	gpio_8b2->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)(1 << 4 ));
	gpio_8b2->gpio_write(0 << 4, 1 << 4);
	printf("\r\nDSpotter version: %s\r\n", Spotter_GetVersion());


	nRet= Spotter_Init((void*)pCYBase_mod, (const void*[]){(void*)pGroup_1_mod, (void*)pGroup_2_mod, 0}, pMem, nMemSize);
	if(nRet!= 0){
		goto Exit;
	}
	Spotter_Reset();
	printf("Begin recognize\r\n");
	doRecognize();
	printf("Done\r\n");
 Exit:
 	Spotter_Release();

	return 0;
}
