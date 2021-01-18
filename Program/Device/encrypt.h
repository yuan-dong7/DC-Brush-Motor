#ifndef _ENCRYPT_H_
#define _ENCRYPT_H_

typedef struct {
    unsigned char PID;
    unsigned char Data[8];
} DTP_Package_t;

void DTP_Transmit(DTP_Package_t *dtp_package);

#endif
