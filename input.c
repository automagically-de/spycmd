#include "SDL.h"

#include "nqc.h"

int input_init(void)
{
	SDL_Surface *screen;
	SDL_Joystick *js0 = NULL;

	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0)
	{
		fprintf(stderr, "E: %s\n", SDL_GetError());
		return -1;
	}

	screen = SDL_SetVideoMode(160, 120, 8, SDL_SWSURFACE);

	atexit(SDL_Quit);

	/* init joystick */
	SDL_JoystickEventState(SDL_ENABLE);
	if(SDL_NumJoysticks() > 0)
		js0 = SDL_JoystickOpen(0);

	if(js0)
	{
		printf("js0: %d axes, %d buttons\n",
			SDL_JoystickNumAxes(js0),
			SDL_JoystickNumButtons(js0));
	}

	return 0;
}

#define AXIS_THRESHOLD 0x1000

static int axis0(int fd, int speed)
{
	if(speed == 0)
	{
		nqc_SetOutput(fd, OUT_A, OUT_OFF);
		nqc_SetLED(fd, LED_MODE_BLINK, 0x00);
		nqc_SetLED(fd, LED_MODE_ON, 0x00);
		return 0;
	}

	if(speed < 0)
	{
		nqc_SetDirection(fd, OUT_A, OUT_FWD);
		nqc_SetLED(fd, LED_MODE_BLINK, 0x07);
	}
	else
	{
		nqc_SetDirection(fd, OUT_A, OUT_REV);
		nqc_SetLED(fd, LED_MODE_BLINK, 0x38);
	}

	nqc_SetPower(fd, OUT_A, (speed == 0) ? 0 : OUT_FULL);
	nqc_SetOutput(fd, OUT_A, OUT_ON);
	return 0;
}

static int axis1(int fd, int speed)
{
	nqc_SetPower(fd, OUT_B, OUT_LOW);
	if(speed == 0)
	{
		nqc_SetOutput(fd, OUT_B, OUT_OFF);
		return 0;
	}

	if(speed < 0)
		nqc_SetDirection(fd, OUT_B, OUT_REV);
	else
		nqc_SetDirection(fd, OUT_B, OUT_FWD);

	nqc_SetOutput(fd, OUT_B, OUT_ON);

	return 0;
}

int input_loop(int fd)
{
	SDL_Event event;
	int quit = 0, tmp;
	static int js0_pval_0 = 0;
	static int js0_pval_1 = 0;

	while(!quit)
	{
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				case SDL_JOYBUTTONDOWN:
					nqc_PlaySound(fd, event.jbutton.button);
					break;

				case SDL_JOYAXISMOTION:
					if(event.jaxis.axis == 0)
					{
						/* left-right */
						tmp = (abs(event.jaxis.value) < AXIS_THRESHOLD) ?
							0 :
							((event.jaxis.value > 0) ? 1 : -1);
						if(tmp != js0_pval_0)
							axis0(fd, tmp);
						js0_pval_0 = tmp;
					}
					else if(event.jaxis.axis == 1)
					{
						/* up-down */
						if(abs(event.jaxis.value) >= AXIS_THRESHOLD)
						{
							if(js0_pval_1 != 1)
								axis1(fd, event.jaxis.value);
							js0_pval_1 = 1;
						}
						else
						{
							if(js0_pval_1 != 0)
								axis1(fd, 0);
							js0_pval_1 = 0;
						}
					}
					break;

				case SDL_KEYDOWN:
					switch(event.key.keysym.sym)
					{
						case SDLK_DOWN:
							nqc_SetDirection(fd, OUT_B, OUT_FWD);
							nqc_SetOutput(fd, OUT_B, OUT_ON);
							break;

						case SDLK_UP:
							nqc_SetDirection(fd, OUT_B, OUT_REV);
							nqc_SetOutput(fd, OUT_B, OUT_ON);
							break;

						case SDLK_LEFT:
							nqc_SetDirection(fd, OUT_A, OUT_FWD);
							nqc_SetOutput(fd, OUT_A, OUT_ON);
							nqc_SetLED(fd, LED_MODE_BLINK, 0x07);
							break;

						case SDLK_RIGHT:
							nqc_SetDirection(fd, OUT_A, OUT_REV);
							nqc_SetOutput(fd, OUT_A, OUT_ON);
							nqc_SetLED(fd, LED_MODE_BLINK, 0x38);
							break;

						default:
							break;
					}
					break;

				case SDL_KEYUP:
					switch(event.key.keysym.sym)
					{
						case SDLK_LEFT:
						case SDLK_RIGHT:
							nqc_SetOutput(fd, OUT_A, OUT_OFF);
							nqc_SetLED(fd, LED_MODE_BLINK, 0x00);
							nqc_SetLED(fd, LED_MODE_ON, 0x00);
							break;

						case SDLK_UP:
						case SDLK_DOWN:
							nqc_SetOutput(fd, OUT_B, OUT_OFF);
							break;

						case SDLK_q:
							quit = 1;
							return 0;
							break;

						case SDLK_RETURN:
							/* kill switch */
							nqc_SetOutput(fd, OUT_A | OUT_B, OUT_OFF);
							nqc_SetLED(fd, LED_MODE_BLINK, 0x00);
							nqc_SetLED(fd, LED_MODE_ON, 0x00);
							break;

						case SDLK_0 ... SDLK_9:
							nqc_PlaySound(fd, event.key.keysym.sym - SDLK_0);
							break;

						default:
							break;
					}
					break;

				case SDL_QUIT:
					quit = 1;
					return 0;
					break;

				default:
					break;
			}


		}
		SDL_Delay(50);
	}
	return -1;
}
