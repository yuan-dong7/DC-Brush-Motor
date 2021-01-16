//
// Created by Lao·Zhu on 2021/1/14.
//

#ifndef _DEVICE_SC60224_H_
#define _DEVICE_SC60224_H_

#define CMD_ANGLE            0xffff
#define CMD_AGC              0x7ffd
#define CMD_MAG              0x7ffe
#define CMD_CLAER            0x4001
#define CMD_NOP              0xc000

unsigned short SC60224_ReadData(unsigned short TxData);
float SC60224_GetAngle(void);

#endif //_DEVICE_SC60224_H_
