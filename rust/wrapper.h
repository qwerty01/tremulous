#include <stdbool.h>
#include <qcommon/qcommon.h>
#include <qcommon/files.h>
#include <qcommon/cvar.h>
#include <client/client.h>
#include <qcommon/net.h>

extern "C" {
//int main2(int argc, char **argv);
void tNET_Init(void);
void tNET_Shutdown(void);
void tNET_Restart_f(void);
void tNET_Config(bool enableNetworking);
void tNET_FlushPacketQueue(void);
void tNET_SendPacket(netsrc_t sock, int length, const void *data, struct netadr_t to);
void tNET_OutOfBandPrint(netsrc_t net_socket, struct netadr_t adr, const char *format, ...)
    __attribute__((format(printf, 3, 4)));
void tNET_OutOfBandData(netsrc_t sock, struct netadr_t adr, uint8_t *format, int len);

bool tNET_CompareAdr(struct netadr_t a, struct netadr_t b);
bool tNET_CompareBaseAdrMask(struct netadr_t a, struct netadr_t b, int netmask);
bool tNET_CompareBaseAdr(struct netadr_t a, struct netadr_t b);
bool tNET_IsLocalAddress(struct netadr_t adr);
const char *tNET_AdrToString(struct netadr_t a);
const char *tNET_AdrToStringwPort(struct netadr_t a);
int tNET_StringToAdr(const char *s, struct netadr_t *a, enum netadrtype_t family);
bool tNET_GetLoopPacket(netsrc_t sock, struct netadr_t *net_from, struct msg_t *net_message);
void tNET_JoinMulticast6(void);
void tNET_LeaveMulticast6(void);
void tNET_Sleep(int msec);
}
