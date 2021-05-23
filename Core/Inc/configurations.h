/*
 * configurations.h
 *
 *  Created on: Apr 29, 2021
 *      Author: mames
 */

#ifndef INC_CONFIGURATIONS_H_
#define INC_CONFIGURATIONS_H_

#define BIAS 0
#define KATODE 1

#define ADDR "04.21.00"
#define ADDRLEN sizeof(ADDR)-1

#define MAXPACKETSIZE 40
#define PACKETSINBUF 100
#define BUFSIZE PACKETSINBUF*MAXPACKETSIZE

#define UARTRESSIZE 15
#define CMDARGSIZE 15

#endif /* INC_CONFIGURATIONS_H_ */
