#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <winscard.h>
#include <yakisoba.h>

#define CHAR	char
#define CARD_NAME "SOBACAS@OishiiSlurper"

//? SCARD_IO_REQUEST g_rgSCardT1Pci_;
const SCARD_IO_REQUEST g_rgSCardT0Pci = { SCARD_PROTOCOL_T0, sizeof(SCARD_IO_REQUEST) };
const SCARD_IO_REQUEST g_rgSCardT1Pci = { SCARD_PROTOCOL_T1, sizeof(SCARD_IO_REQUEST) };
const SCARD_IO_REQUEST g_rgSCardRawPci = { SCARD_PROTOCOL_RAW, sizeof(SCARD_IO_REQUEST) };

static const BYTE atr[] = {
    0x3b,0xf0,0x12,0x00,0xff,0x91,0x81,0xb1, 0x7c,0x45,0x1f,0x03,0x99};

char *pcsc_stringify_error(const LONG pcscError){return "unknown";}

LONG SCardEstablishContext(DWORD dwScope, LPCVOID pvReserved1,LPCVOID pvReserved2, LPSCARDCONTEXT phContext)
{
    if (!phContext) return SCARD_E_INVALID_PARAMETER;
	*phContext=(SCARDCONTEXT)0x35313239;
	return SCARD_S_SUCCESS;
}

LONG SCardReleaseContext(SCARDCONTEXT hContext){return SCARD_S_SUCCESS;}

LONG SCardConnect(SCARDCONTEXT hContext, LPCSTR szReader, DWORD dwShareMode, DWORD dwPreferredProtocols, LPSCARDHANDLE phCard, LPDWORD pdwActiveProtocol)
{
    if (!phCard || !pdwActiveProtocol) return SCARD_E_INVALID_PARAMETER;
    *phCard=(SCARDHANDLE)0x35313239;
    *pdwActiveProtocol=SCARD_PROTOCOL_T1;
    return SCARD_S_SUCCESS;
}

LONG SCardReconnect(SCARDHANDLE hCard, DWORD dwShareMode, DWORD dwPreferredProtocols, DWORD dwInitialization, LPDWORD pdwActiveProtocol)
{
    if (pdwActiveProtocol) *pdwActiveProtocol=SCARD_PROTOCOL_T1;
    return SCARD_S_SUCCESS;
}

LONG SCardDisconnect(SCARDHANDLE hCard, DWORD dwDisposition){return SCARD_S_SUCCESS;}

LONG SCardBeginTransaction(SCARDHANDLE hCard){return SCARD_S_SUCCESS;}

LONG SCardEndTransaction(SCARDHANDLE hCard, DWORD dwDisposition){return SCARD_S_SUCCESS;}

LONG SCardStatus(SCARDHANDLE hCard, LPSTR mszReaderName, LPDWORD pcchReaderLen, LPDWORD pdwState, LPDWORD pdwProtocol, LPBYTE pbAtr, LPDWORD pcbAtrLen)
{
    static const CHAR *name = CARD_NAME;
    DWORD rlen, alen;
    DWORD dummy = 0;
    char *buf = NULL;

    if (pdwState)
        *pdwState = SCARD_PRESENT | SCARD_POWERED | SCARD_SPECIFIC;

    if (pdwProtocol)
        *pdwProtocol = SCARD_PROTOCOL_T1;

    if (!pcchReaderLen)
        pcchReaderLen = &dummy;
    if (!pcbAtrLen)
        pcbAtrLen = &dummy;

    rlen = *pcchReaderLen;
    alen = *pcbAtrLen;

    *pcchReaderLen = strlen(name) + 1;
    *pcbAtrLen = sizeof atr;

    if (rlen == SCARD_AUTOALLOCATE) {
        if (!mszReaderName)
            return SCARD_E_INVALID_PARAMETER;
        rlen = *pcchReaderLen;
        buf = malloc(*pcchReaderLen);
        *(char **)mszReaderName = buf;
        if (!buf)
            return SCARD_E_NO_MEMORY;
    } else
        buf = mszReaderName;

    if (buf) {
        strncpy(buf, name, rlen);
        if (rlen < *pcchReaderLen)
            return SCARD_E_INSUFFICIENT_BUFFER;
    }

    if (alen == SCARD_AUTOALLOCATE) {
        if (!pbAtr)
            return SCARD_E_INVALID_PARAMETER;
        alen = *pcbAtrLen;
        buf = malloc(*pcbAtrLen);
        *(LPBYTE *)pbAtr = buf;
        if (!buf)
            return SCARD_E_NO_MEMORY;
    } else
        buf = pbAtr;

    if (buf) {
        memcpy(buf, atr, (alen < *pcbAtrLen) ? alen : *pcbAtrLen);
        if (alen < *pcbAtrLen)
            return SCARD_E_INSUFFICIENT_BUFFER;
    }

    return SCARD_S_SUCCESS;
}

LONG SCardGetStatusChange(SCARDCONTEXT hContext, DWORD dwTimeout, LPSCARD_READERSTATE rgReaderStates, DWORD cReaders)
{
    struct timeval tv;

    if (!rgReaderStates && cReaders > 0)
        return SCARD_E_INVALID_PARAMETER;

    tv.tv_sec = dwTimeout / 1000;
    tv.tv_usec = (dwTimeout % 1000) * 1000;
    if (!rgReaderStates || cReaders <= 0 ||
            rgReaderStates[0].dwCurrentState != SCARD_STATE_UNAWARE)
        select(0, NULL, NULL, NULL, (dwTimeout == INFINITE) ? NULL : &tv);

    if (cReaders <= 0)
        return SCARD_S_SUCCESS;
    rgReaderStates[0].dwEventState = rgReaderStates[0].dwCurrentState & 0xffff0000;
    rgReaderStates[0].dwEventState |= SCARD_STATE_PRESENT | SCARD_STATE_INUSE;
    rgReaderStates[0].cbAtr = sizeof atr;
    memcpy(rgReaderStates[0].rgbAtr, atr, sizeof atr);
    return SCARD_S_SUCCESS;
}

LONG SCardControl(SCARDHANDLE hCard, DWORD dwControlCode, LPCVOID pbSendBuffer, DWORD cbSendLength, LPVOID pbRecvBuffer, DWORD cbRecvLength, LPDWORD lpBytesReturned)
{
    return SCARD_E_UNSUPPORTED_FEATURE;
}

LONG SCardGetAttrib(SCARDHANDLE hCard, DWORD dwAttrId, LPBYTE pbAttr, LPDWORD pcbAttrLen)
{
    return SCARD_E_UNSUPPORTED_FEATURE;
}

LONG SCardSetAttrib(SCARDHANDLE hCard, DWORD dwAttrId, LPCBYTE pbAttr, DWORD cbAttrLen)
{
    return SCARD_E_UNSUPPORTED_FEATURE;
}

LONG SCardTransmit(SCARDHANDLE hCard, LPCSCARD_IO_REQUEST pioSendPci, LPCBYTE pbSendBuffer, DWORD cbSendLength, LPSCARD_IO_REQUEST pioRecvPci, LPBYTE pbRecvBuffer, LPDWORD pcbRecvLength)
{
    if (!pbSendBuffer || cbSendLength < 2 || !pbRecvBuffer)
        return SCARD_E_INVALID_PARAMETER;

    switch(pbSendBuffer[1])
    {
    case 0x30:{
        BYTE r[]={
            0x00,0x39,0x00,0x00,0x21,0x00,0x00,0x05, 0xc0,0xff,0xee,0xc0,0xff,0xee,0x01,0x50,
            0x36,0x31,0x04,0x66,0x4b,0x17,0xea,0x5c, 0x32,0xdf,0x9c,0xf5,0xc4,0xc3,0x6c,0x1b,
            0xec,0x99,0x39,0x21,0x68,0x9d,0x4b,0xb7, 0xb7,0x4e,0x40,0x84,0x0d,0x2e,0x7d,0x98,
            0xfe,0x27,0x19,0x99,0x19,0x69,0x09,0x11, 0x01,0x02,0x01,0x90,0x00};
        if (*pcbRecvLength < sizeof r) return SCARD_E_INSUFFICIENT_BUFFER;
        memcpy(pbRecvBuffer,r,sizeof r);
        *pcbRecvLength=sizeof r;
        break;}
    case 0x32:{
        BYTE r[]={
            0x00,0x0f,0x00,0x00,0x21,0x00,0x01,0x54, 0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
            0x00,0x90,0x00};
        if (*pcbRecvLength < sizeof r) return SCARD_E_INSUFFICIENT_BUFFER;
        memcpy(pbRecvBuffer,r,sizeof r);
        *pcbRecvLength=sizeof r;
        break;}
    case 0x34:{
        BYTE r[]={
            0x00,0x15,0x00,0x00,0x08,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
            0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x90, 0x00};
        if (*pcbRecvLength < sizeof r) return SCARD_E_INSUFFICIENT_BUFFER;
        bcas_decodeECM(pbSendBuffer+5,pbSendBuffer[4],r+6,NULL);
        memcpy(pbRecvBuffer,r,sizeof r);
        *pcbRecvLength=sizeof r;
        break;}
    case 0x80:{
        BYTE r[]={
            0x00,0x10,0x00,0x00,0xA1,0x01,0x00,0x00, 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
            0x00,0x00,0x90,0x00};
        if (*pcbRecvLength < sizeof r) return SCARD_E_INSUFFICIENT_BUFFER;
        memcpy(pbRecvBuffer,r,sizeof r);
        *pcbRecvLength=sizeof r;
        break;}
    default:{
        BYTE r[]={
            0x00,0x04,0x00,0x00,0xa1,0xfe,0x90,0x00};
        if (*pcbRecvLength < sizeof r) return SCARD_E_INSUFFICIENT_BUFFER;
        memcpy(pbRecvBuffer,r,sizeof r);
        *pcbRecvLength=sizeof r;
        break;}
    }
    if (pioRecvPci)
        *pioRecvPci = *SCARD_PCI_T1;
    return SCARD_S_SUCCESS;
}

LONG SCardListReaders(SCARDCONTEXT hContext, LPCSTR mszGroups, LPSTR mszReaders, LPDWORD pcchReaders)
{
    static const CHAR r[]= CARD_NAME "\0";
    char *buf;
    DWORD rlen;

    if (!pcchReaders)
        return SCARD_E_INVALID_PARAMETER;

    rlen = *pcchReaders;
    *pcchReaders = sizeof r;
    if (rlen == SCARD_AUTOALLOCATE) {
        if (!mszReaders)
            return SCARD_E_INVALID_PARAMETER;
        buf = malloc(sizeof r);
        if (!buf)
            return SCARD_E_NO_MEMORY;
        *(char **)mszReaders = buf;
    } else {
        buf = mszReaders;
        if (mszReaders && rlen < sizeof r) {
            return SCARD_E_INSUFFICIENT_BUFFER;
        }
    }

    if(mszReaders)
        memcpy(mszReaders, r, sizeof r);

    return SCARD_S_SUCCESS;
}

LONG SCardFreeMemory(SCARDCONTEXT hContext, LPCVOID pvMem){free((void *)pvMem); return SCARD_S_SUCCESS;}

LONG SCardListReaderGroups(SCARDCONTEXT hContext, LPSTR mszGroups, LPDWORD pcchGroups)
{
    LONG rv = SCARD_S_SUCCESS;
    const char grp[] = "SCard$DefaultReaders\0";
    const DWORD glen = sizeof grp;

    char *buf = NULL;

    if (*pcchGroups == SCARD_AUTOALLOCATE) {
        buf = malloc(glen);
        if (!buf) {
            rv = SCARD_E_NO_MEMORY;
            goto end;
        }
        if (!mszGroups) {
            rv = SCARD_E_INVALID_PARAMETER;
            goto end;
        }
        *(char **)mszGroups = buf;
    }
    else
    {
        buf = mszGroups;

        if (mszGroups && *pcchGroups < glen) {
            rv = SCARD_E_INSUFFICIENT_BUFFER;
            goto end;
        }
    }
    if (buf)
        memcpy(buf, grp, glen);

end:
    *pcchGroups = glen;
    return rv;
}

LONG SCardCancel(SCARDCONTEXT hContext){return SCARD_S_SUCCESS;}

LONG SCardIsValidContext(SCARDCONTEXT hContext) {return SCARD_S_SUCCESS;}

