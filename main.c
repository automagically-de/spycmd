#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "spylib.h"
#include "serport.h"
#include "nqc.h"
#include "input.h"

#define SERIAL_PORT "/dev/ttyS0"

int main(int argc, char *argv[])
{
	int ser;
	char *port;

	port = getenv("RCX_PORT");
	if(port == NULL)
		port = SERIAL_PORT;

	printf("opening %s\n", port);
	ser = serport_connect(port);

	nqc_SetPower(ser, OUT_B, OUT_LOW);

	input_init();

	nqc_SetLED(ser, LED_MODE_ON, 0);

	/*
	 * OUT_A: right motor (REV: turn right, FWD: turn left)
	 * OUT_B: left motor
	 */

	input_loop(ser);

	close(ser);

	return EXIT_SUCCESS;
}
