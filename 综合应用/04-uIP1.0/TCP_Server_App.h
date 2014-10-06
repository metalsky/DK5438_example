
#ifndef __UIP_EXAM_H__
#define __UIP_EXAM_H__

#include "uip.h"
void TCP_Server_Init(void);
void TCP_Server_App(void);
#ifndef UIP_APPCALL
#define UIP_APPCALL TCP_Server_App
#endif


#endif /* __UIP_ARP_H__ */
