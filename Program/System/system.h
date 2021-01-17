#ifndef _SYSTEM_H_
#define _SYSTEM_H_

void System_Config(void);
void Delayms(unsigned long count);
void delay_decrement(void);
void Delayus(unsigned int xus);
unsigned short FloatToInt16(float data0);
float Int16ToFloat(unsigned short data0);

extern float Curve[];

#endif /* _SYSTEM_H_ */
