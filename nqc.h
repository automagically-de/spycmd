#ifndef _NQC_H
#define _NQC_H

#define LED_MODE_ON            0
#define LED_MODE_BLINK         1
#define LED_MODE_DURATION      2
#define LED_MODE_YELLOW       10

#define OUT_A       0x01
#define OUT_B       0x02
#define OUT_C       0x04

#define OUT_FLOAT   0
#define OUT_OFF     0x40
#define OUT_ON      0x80

#define OUT_REV     0
#define OUT_TOGGLE  0x40
#define OUT_FWD     0x80

#define OUT_LOW     0
#define OUT_HALF    3
#define OUT_FULL    7

int nqc_Alive(int fd);

int nqc_Set(int fd, unsigned short dst, unsigned short src);
int nqc_SetLED(int fd, unsigned char mode, unsigned short v);

int nqc_SetOutput(int fd, unsigned char o, unsigned char m);
int nqc_SetDirection(int fd, unsigned char o, unsigned char d);
int nqc_SetPower(int fd, unsigned char o, unsigned char p);

int nqc_PlaySound(int fd, unsigned s);

#endif
