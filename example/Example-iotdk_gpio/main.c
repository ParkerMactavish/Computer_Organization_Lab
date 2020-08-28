/* ------------------------------------------
 * Copyright (c) 2017, Synopsys, Inc. All rights reserved.

 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:

 * 1) Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.

 * 2) Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation and/or
 * other materials provided with the distribution.

 * 3) Neither the name of the Synopsys, Inc., nor the names of its contributors may
 * be used to endorse or promote products derived from this software without
 * specific prior written permission.

 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
--------------------------------------------- */
#include "embARC.h"
#include "embARC_debug.h"

#define GPIO4B2_0_OFFSET	0
#define GPIO4B2_1_OFFSET	1

DEV_GPIO_PTR gpio_4b2;

int main(void)
{

	uint32_t value_read = 0;
	uint32_t value_set  = 1;

	gpio_4b2 = gpio_get_dev(DFSS_GPIO_4B2_ID); // get GPIO_4B2 handler

	gpio_4b2->gpio_open(1 << GPIO4B2_0_OFFSET ); // open
	gpio_4b2->gpio_open(1 << GPIO4B2_1_OFFSET); // open
	gpio_4b2->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)(1 << GPIO4B2_0_OFFSET )); // set dir
	gpio_4b2->gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT, (void *)(1 << GPIO4B2_1_OFFSET)); // set GPIO4B2_1_OFFSET dir to input.
	//connect ARDUINO_PIN_0 and ARDUINO_PIN_1;
	while (1)
	{	
		gpio_4b2->gpio_write(1 << GPIO4B2_0_OFFSET, 1 << GPIO4B2_0_OFFSET); // GPIO4B2_0_OFFSET set high output.
		EMBARC_PRINTF("PIN0 set: Hight \r\n");
		if(value_read)
			EMBARC_PRINTF("PIN1 get: Hight \r\n");
		else
			EMBARC_PRINTF("PIN1 get: Low \r\n");
		board_delay_ms(1000,1);
		gpio_4b2->gpio_read(&value_read, 1 << GPIO4B2_1_OFFSET);
		EMBARC_PRINTF("PIN0 set: Hight \r\n");
		if(value_read)
			EMBARC_PRINTF("PIN1 get: Hight \r\n");
		else
			EMBARC_PRINTF("PIN1 get: Low \r\n");
		board_delay_ms(1000,1);
		gpio_4b2->gpio_write(0 << GPIO4B2_0_OFFSET, 1 << GPIO4B2_0_OFFSET); // GPIO4B2_0_OFFSET set low output.
		EMBARC_PRINTF("PIN0 set: Low \r\n");
		if(value_read)
			EMBARC_PRINTF("PIN1 get: Hight \r\n");
		else
			EMBARC_PRINTF("PIN1 get: Low \r\n");
		board_delay_ms(1000,1);
		gpio_4b2->gpio_read(&value_read, 1 << GPIO4B2_1_OFFSET);
		EMBARC_PRINTF("PIN0 set: Low \r\n");
		if(value_read)
			EMBARC_PRINTF("PIN1 get: Hight \r\n");
		else
			EMBARC_PRINTF("PIN1 get: Low \r\n");
		board_delay_ms(1000,1);
	}

	return E_SYS;
}


