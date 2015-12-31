#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <sys/time.h>
#include <chrono>
#include <thread>
#include <iostream>

#define PCAN_CHANNEL  PCAN_USBBUS1
#define PCAN_BAUDRATE PCAN_BAUD_500K

#ifndef __APPLE__
#include <asm/types.h>
#define DWORD  __u32
#define WORD   unsigned short
#define BYTE   unsigned char
#define LPSTR  char*
#include "PCANBasic.h"
#else
#include "PCBUSB.h"
#endif

void sigterm(int signo)
{
    /*printf("got signal %d\n", signo);*/
    CAN_Uninitialize(PCAN_NONEBUS);
    exit(0);
}

int main(int argc, char *argv[])
{
    TPCANMsg message;
    TPCANStatus status;
    int fd;
    fd_set fds;
    
    if((signal(SIGTERM, sigterm) == SIG_ERR) ||
       (signal(SIGINT, sigterm) == SIG_ERR)) {
        perror("Error");
        return errno;
    }
    status = CAN_Initialize(PCAN_CHANNEL, PCAN_BAUDRATE, 0, 0, 0);
    printf("Initialize CAN: 0x%lx\n", status);
    if(status != PCAN_ERROR_OK) goto leave;
    
    status = CAN_GetValue(PCAN_CHANNEL, PCAN_RECEIVE_EVENT, &fd, sizeof(int));
    if(status != PCAN_ERROR_OK) goto leave;
    
    FD_ZERO(&fds);
    FD_SET(fd, &fds);
    
    while(select(fd+1, &fds, NULL, NULL, NULL) > 0) {
        status = CAN_Read(PCAN_CHANNEL, &message, NULL);
        if (status != PCAN_ERROR_OK) {
            printf("Error 0x%lx\n", status);
            break;
        }
        // 245-361 : gas peddal
        // 025 : steering angle
        // 224 : brake pedal
        // 230 : parking brake
        // 1c4 : sends data while gas engine is on
        // 260 : data[1..2] seems to be torque sensor
        if(message.ID == 0x260) {
          if(true) {
            printf("  - R ID:%4x LEN:%1x DATA:%02x %02x %02x %02x %02x %02x %02x %02x\n",
                   (int) message.ID, (int) message.LEN, (int) message.DATA[0],
                   (int) message.DATA[1], (int) message.DATA[2],
                   (int) message.DATA[3], (int) message.DATA[4],
                   (int) message.DATA[5], (int) message.DATA[6],
                   (int) message.DATA[7]);
          }
          if(false) {
            std::cout << "data for ID(" << message.ID << ") is: " << (signed short int)(((short int)message.DATA[1]<<8)|(message.DATA[2] & 0xFF)) << std::endl;
          }
        }
//        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
leave:
    CAN_Uninitialize(PCAN_NONEBUS);
    return 0;
}
