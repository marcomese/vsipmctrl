/*
 * configurations.h
 *
 *  Created on: Apr 29, 2021
 *      Author: mames
 */

#ifndef INC_CONFIGURATIONS_H_
#define INC_CONFIGURATIONS_H_

#define BIAS 0
#define BIASHV 3
#define KATODE 1
#define VREFINT 2
#define VREFINTCAL_ADDR 0x1FFFF7BA

#define ADDR "04.21.00"
#define ADDRLEN sizeof(ADDR)-1

#define MAXPACKETSIZE 40
#define PACKETSINBUF 10
#define BUFSIZE PACKETSINBUF*MAXPACKETSIZE

#define UARTRESSIZE 30
#define CMDARGSIZE 15
#define UART1 0xFF
#define UART2 0xFE

#define MAXBIAS 1.5
#define MAXKATODE 2.0

#define NOOP 0
#define IDNCMD 1
#define VOLTCMD 2
#define MAXCMD 3
#define SENDTOADDRCMD 4
#define SENDTOUART 5
#define HVCMD 6

#define NODESECT "NODE:"
#define IDNSECT "*IDN?"
#define BIASSECT "BIAS"
#define KATODESECT "KATODE"
#define RESP "R:"
#define ERR "E:"

#define NODELEN (sizeof(NODESECT)-1)
#define IDNLEN (sizeof(IDNSECT)-1)
#define BIASLEN (sizeof(BIASSECT)-1)
#define KATODELEN (sizeof(KATODESECT)-1)
#define RESPLEN (sizeof(RESP)-1)
#define ERRLEN (sizeof(ERR)-1)

#define VOLTCMDSTR "VOLT"
#define MAXCMDSTR "MAX"
#define HVCMDSTR "HV"

#define VOLTLEN (sizeof(VOLTCMDSTR)-1)
#define MAXLEN (sizeof(MAXCMDSTR)-1)
#define HVLEN (sizeof(HVCMDSTR)-1)

#define PIDKp 0.3
#define PIDKi 0.1
#define PIDkd 0.1

#endif /* INC_CONFIGURATIONS_H_ */
