//
// Created by Lao·Zhu on 2021/1/14.
//

#include "can_package.h"

void CAN_ReceiveCallback(unsigned int StandardID, unsigned char *buf) {
    unsigned char Buffer[8];
    unsigned char counter;
    for (counter = 0; counter < 8; counter++) {
        Buffer[counter] = *buf;
        buf++;
    }

}
