/*
 * configurations.h
 *
 *  Created on: Apr 29, 2021
 *      Author: mames
 */

#ifndef INC_CONFIGURATIONS_H_
#define INC_CONFIGURATIONS_H_

#define ADDR "04.21.00"
#define ADDRLEN sizeof(ADDR)

#define MAXPACKETSIZE 40
#define PACKETSINBUF 1000
#define BUFSIZE PACKETSINBUF*MAXPACKETSIZE

#define UARTRESSIZE 15
#define CMDARGSIZE 15

#endif /* INC_CONFIGURATIONS_H_ */
