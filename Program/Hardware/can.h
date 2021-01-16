#ifndef __CAN_H__
#define __CAN_H__

void CAN_Config(void);
void CAN_Transmit(unsigned int StandardID, unsigned char *buf);

#endif