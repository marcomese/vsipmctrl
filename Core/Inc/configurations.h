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
#define VREFINT 2
#define VREFINTCAL_ADDR 0x1FFFF7BA

#define ADDR "04.21.00"
#define ADDRLEN sizeof(ADDR)-1

#define MAXPACKETSIZE 40
#define PACKETSINBUF 100
#define BUFSIZE PACKETSINBUF*MAXPACKETSIZE

#define UARTRESSIZE 15
#define CMDARGSIZE 15

#define MAXBIAS 1.5
#define MAXKATODE 2.0

#define NOOP 0
#define IDNCMD 1
#define VOLTCMD 2
#define MAXCMD 3
#define SENDTOADDRCMD 4
#define STATECMD 5
#define PULSERATECMD 6

#define NODESECT "NODE:"
#define IDNSECT "*IDN?"
#define BIASSECT "BIAS"
#define KATODESECT "KATODE"
#define MONITORSECT "MONITOR"

#define NODELEN (sizeof(NODESECT)-1)
#define IDNLEN (sizeof(IDNSECT)-1)
#define BIASLEN (sizeof(BIASSECT)-1)
#define KATODELEN (sizeof(KATODESECT)-1)
#define MONITORLEN (sizeof(MONITORSECT)-1)

#define VOLTCMDSTR "VOLT"
#define MAXCMDSTR "MAX"
#define STATECMDSTR "STATE"

#define VOLTLEN (sizeof(VOLTCMDSTR)-1)
#define MAXLEN (sizeof(MAXCMDSTR)-1)
#define STATELEN (sizeof(STATECMDSTR)-1)

#endif /* INC_CONFIGURATIONS_H_ */
