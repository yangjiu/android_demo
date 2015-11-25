#include "jni.h"
#include "JNIHelp.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/time.h>
#include <linux/pn544.h>
#include <android/log.h>
#include "phNxpNciHal_SelfTest.h"
#include <cutils/properties.h>


#define MAX_SINGLE_CMD_SIZE     50

#define CMD_TYPE_SEND_NORMAL      0		//normal
#define CMD_TYPE_CONDITIONAL      1		//conditional
#define CMD_TYPE_WAIT_EVENT       2		//waiting event
#define CMD_TYPE_STOP_HERE        3		//stop here
#define CMD_TYPE_SEND_EVENT       4		//sending event


typedef struct
{
	int type;									//0: normal, 1: conditional, 2: waiting event, 3: stop here
	int action;									//0: if not matching, return
												//1: if not matching, ignore and continue anyway
												//2: if not matching, compare to next command line
												//3: if no response, send current command again
	char scriptCmdLine[MAX_SINGLE_CMD_SIZE];    //script format
	char expectdResult[MAX_SINGLE_CMD_SIZE];	//response with content
} SINGLE_CMD_LINE;

SINGLE_CMD_LINE SendCarrier_Array[];

int ChannelOpen(void);
void ChannelClose();
int ScriptBatchRun(SINGLE_CMD_LINE *tmpCmdLineElem, unsigned char *lastRespCmd, int *lastRespLenPtr, unsigned char *frameNsPtr, unsigned char *frameNrPtr);
int ScriptCmdExecute(unsigned char *scriptReqCmd, int scriptReqLen, unsigned char *scriptRespCmd, int *scriptRespLenPtr, unsigned char *frameNsPtr, unsigned char *frameNrPtr, int cmdType);
int BCD_2_ASC(unsigned char *src, unsigned char *dest, int *srcLen);
int ASC_2_BCD(unsigned char *src, unsigned char *dest, int *srcLen);

unsigned char nfcChip[PROPERTY_VALUE_MAX];

unsigned char llcSendBuf[34];
int llcSendLen;

unsigned char llcRecvBuf[34];
int llcRecvLen;

int fp = 0;

#define TAG "nfclib"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)

SINGLE_CMD_LINE SendCarrier_Array[] = {
	{0, 3, "URSET0400", ""},
	{0, 0, "I8103", ""},
	{0, 0, "I8114", ""},
	{0, 0, "I8103", ""},
	{0, 0, "I8110200090", ""},
	{0, 0, "I8203", ""},
	{0, 0, "I82250300", ""},

	{3, 0, "", ""},
};


SINGLE_CMD_LINE ReaderMode_Array[] = {
	{0, 0, "URSET0400", ""},
	{0, 0, "I8103", ""},
	{0, 0, "I8114", ""},
	{0, 0, "I8103", ""},
	{0, 0, "I8110200094", ""},
	{0, 0, "I8203", ""},
	{0, 0, "I8201067F", ""},
	{0, 0, "I820206", ""},
	{0, 0, "I8110130013", ""},
	{0, 0, "I8303", ""},
	{0, 0, "I8110200090", ""},
	{0, 0, "I8403", ""},
	{0, 0, "I840202", ""},
	{0, 0, "I840102FF", ""},
	{0, 0, "I83011000", ""},
	{4, 3, "I8350", ""},
	//{2, 3, "", ""},
	{2, 0, "", ""},
	{0, 0, "I830202", ""},
	{3, 0, "", ""},
};



jboolean SendCarrier_pn544() {
	unsigned char lastRespBuf[80];
	int lastRespLen;

	unsigned char ascUidBuf[80];
	int ascUidLen = 0;

	int ret = 0;
	unsigned char frameNs = 0;
	unsigned char frameNr = 0;

	SINGLE_CMD_LINE *curCmdLineElement;

	curCmdLineElement = SendCarrier_Array;

	jboolean result = JNI_FALSE;
	//TODO: need to modify(depend on HW)
	if(ChannelOpen() != 0) {
		LOGI("Can't open I2C port!");
		return JNI_FALSE;
	}

	//hardware reset
	ioctl(fp, PN544_SET_PWR, 1);
	ioctl(fp, PN544_SET_PWR, 0);
	ioctl(fp, PN544_SET_PWR, 1);

	ret = ScriptBatchRun(curCmdLineElement, lastRespBuf, &lastRespLen, &frameNs, &frameNr);

	if(ret == 0) {
		memset(ascUidBuf, 0x00, sizeof(ascUidBuf));
		ascUidLen = (lastRespLen - 3) * 2;
		BCD_2_ASC(lastRespBuf + 3, ascUidBuf, &ascUidLen);
	//	LOGI("Script executed successfully!");
		result = JNI_TRUE;
	} else {
//		LOGI("Script executed failed here!");
		result = JNI_FALSE;
	}

	//ChannelClose();

	return result;
}

jboolean StartReaderMode_pn544() {
	unsigned char lastRespBuf[80];
	int lastRespLen;

	unsigned char ascUidBuf[80];
	int ascUidLen = 0;

	int ret = 0;
	unsigned char frameNs = 0;
	unsigned char frameNr = 0;

	SINGLE_CMD_LINE *curCmdLineElement;

	curCmdLineElement = ReaderMode_Array;

	jboolean result = JNI_FALSE;
	//TODO: need to modify(depend on HW)
	if(ChannelOpen() != 0) {
		LOGI("Can't open I2C port!");
		return JNI_FALSE;
	}

	//hardware reset
	ioctl(fp, PN544_SET_PWR, 1);
	ioctl(fp, PN544_SET_PWR, 0);
	ioctl(fp, PN544_SET_PWR, 1);

	ret = ScriptBatchRun(curCmdLineElement, lastRespBuf, &lastRespLen, &frameNs, &frameNr);

	if(ret == 0) {
		memset(ascUidBuf, 0x00, sizeof(ascUidBuf));
		ascUidLen = (lastRespLen - 3) * 2;
		BCD_2_ASC(lastRespBuf + 3, ascUidBuf, &ascUidLen);

	//	AppendInfo("Script executed successfully!");
		result = JNI_TRUE;
	} else {
		LOGI("Script executed failed here!");
		result = JNI_FALSE;
	}

	ChannelClose();

	return result;
}


int ScriptBatchRun(SINGLE_CMD_LINE *tmpCmdLineElem, unsigned char *lastRespCmd, int *lastRespLenPtr,
		unsigned char *frameNsPtr, unsigned char *frameNrPtr) {

	unsigned char scriptReqBuf[70];
	int scriptReqLen;
	unsigned char scriptRespBuf[70];
	int scriptRespLen;

	unsigned char condBuf[70];
	int condBufLen;

	int ret;
	int idx;
	int retryCount;
	int condBranchNeededFlag;

	char tmpBuf[30];

	memset(scriptReqBuf, 0x00, sizeof(scriptReqBuf));
	scriptReqLen = 0;
	memset(scriptRespBuf, 0x00, sizeof(scriptRespBuf));
	scriptRespLen = 0;

	idx = 0;
	ret = 0;
	retryCount = 0;
	condBranchNeededFlag = 0;
	while(1) {
		if(tmpCmdLineElem[idx].type == CMD_TYPE_STOP_HERE) {
			break;
		}

		if(condBranchNeededFlag == 1) {
			if(tmpCmdLineElem[idx].type == CMD_TYPE_CONDITIONAL) {
				condBufLen = strlen(tmpCmdLineElem[idx].expectdResult);
				ASC_2_BCD((unsigned char *)tmpCmdLineElem[idx].expectdResult, condBuf, &condBufLen);
				if(memcmp(scriptRespBuf + 1, condBuf, scriptRespLen - 1) == 0) {
					strcpy((char *)scriptReqBuf, tmpCmdLineElem[idx].scriptCmdLine);
					scriptReqLen = strlen((char *)scriptReqBuf);
					ret = ScriptCmdExecute(scriptReqBuf, scriptReqLen, scriptRespBuf, &scriptRespLen, frameNsPtr, frameNrPtr, tmpCmdLineElem[idx].type);
					if(ret != 0) {
						return -1;
					}

					condBranchNeededFlag = 0;
				}
			}

			retryCount = 0;
		} else {
			strcpy((char *)scriptReqBuf, tmpCmdLineElem[idx].scriptCmdLine);
			scriptReqLen = strlen((char *)scriptReqBuf);
			ret = ScriptCmdExecute(scriptReqBuf, scriptReqLen, scriptRespBuf, &scriptRespLen, frameNsPtr, frameNrPtr, tmpCmdLineElem[idx].type);
			if(ret == 0) {
				condBufLen = strlen(tmpCmdLineElem[idx].expectdResult);
				ASC_2_BCD((unsigned char *)tmpCmdLineElem[idx].expectdResult, condBuf, &condBufLen);
				if((strlen(tmpCmdLineElem[idx].expectdResult) > 0) && (memcmp(scriptRespBuf + 1, condBuf, scriptRespLen - 1) != 0)) {
					AppendInfo("point 1");
					if(tmpCmdLineElem[idx].action == 0) {
						//return error directly
						return -1;
					} else if(tmpCmdLineElem[idx].action == 1) {
						//continue anyway
					} else if(tmpCmdLineElem[idx].action == 2) {
						//need condtional branch in next command
						condBranchNeededFlag = 1;
					}
				}

				retryCount = 0;
			} else {
				if(tmpCmdLineElem[idx].action == 3) {
					retryCount ++;
					if(retryCount < 10) {
						usleep(2000);
						continue;
					} else {
						return -2;
					}
				} else if(tmpCmdLineElem[idx].type == 2) {
					return ret;
				}
			}
		}

		idx ++;

		memset(tmpBuf, 0x00, sizeof(tmpBuf));
		//sprintf(tmpBuf, "idx: %d, ret: %d\n", idx - 1, ret);
		//AppendInfo(tmpBuf);
	}

	memcpy(lastRespCmd, scriptRespBuf, scriptRespLen);
	*lastRespLenPtr = scriptRespLen;

	return 0;
}



//commandType, 0: sending command or event, 1: waiting event
int ScriptCmdExecute(unsigned char *scriptReqCmd, int scriptReqLen, unsigned char *scriptRespCmd,
		int *scriptRespLenPtr, unsigned char *frameNsPtr, unsigned char *frameNrPtr, int cmdType) {
	unsigned char tmpContentSendBuf[50];
	unsigned char tmpContentSendLen;
	unsigned char tmpContentRecvBuf[50];
	unsigned char tmpContentRecvLen;

	int tmpAscLen;
	int tmpBcdLen;

	unsigned char tmpNumber = 0;

	int ret;

	memset(tmpContentSendBuf, 0x00, sizeof(tmpContentSendBuf));
	tmpContentSendLen = 0;
	memset(tmpContentRecvBuf, 0x00, sizeof(tmpContentRecvBuf));
	tmpContentRecvLen = 0;

	if(cmdType == CMD_TYPE_SEND_NORMAL) {
		printf("--COMMDAND--\n");
	} else if(cmdType == CMD_TYPE_CONDITIONAL) {
		printf("---CONDITIONAL---\n");
	} else if(cmdType == CMD_TYPE_SEND_EVENT) {
		printf("---SEND EVENT---\n");
	} else if(cmdType == CMD_TYPE_WAIT_EVENT) {
		printf("---WAIT EVENT---\n");
	} else {
		printf("---STOP HERE---\n");
	}

	if((cmdType == CMD_TYPE_SEND_NORMAL) || (cmdType == CMD_TYPE_SEND_EVENT)
		|| (cmdType == CMD_TYPE_CONDITIONAL)) {
		if(scriptReqCmd[0] == 'U') {
			if(memcmp(scriptReqCmd + 1, "RSET", 4) == 0) {
				tmpContentSendBuf[0] = 0xF9;

				tmpAscLen = scriptReqLen - 5;
				tmpBcdLen = ASC_2_BCD(scriptReqCmd + 5, tmpContentSendBuf + 1, &tmpAscLen);
				tmpContentSendLen = 1 + tmpBcdLen;
			} else {
				return -1;
			}
		} else if(scriptReqCmd[0] == 'I') {
			tmpContentSendBuf[0] = 0x80|((*frameNsPtr)<<3)|(*frameNrPtr);
			tmpAscLen = scriptReqLen - 1;
			tmpBcdLen = ASC_2_BCD(scriptReqCmd + 1, tmpContentSendBuf + 1, &tmpAscLen);
			tmpContentSendLen = 1 + tmpBcdLen;

			if(*frameNsPtr < 7) {
				*frameNsPtr = *frameNsPtr + 1;
			} else {
				*frameNsPtr = 0;
			}
		} else if(scriptReqCmd[0] == 'U') {
			return -2;
		} else {
			return -1;
		}
	}

	int idx;
	unsigned char sFrameType;
	int orgCmdSendFlag;

	orgCmdSendFlag = 1;
	for(idx = 0; idx < 3; idx ++) {
		if(orgCmdSendFlag == 1) {
			if((cmdType == CMD_TYPE_SEND_NORMAL) || (cmdType == CMD_TYPE_SEND_EVENT)
				|| (cmdType == CMD_TYPE_CONDITIONAL)) {
				ret = ChannelSend(tmpContentSendBuf, tmpContentSendLen);
				if(ret != 0) {
					return -3;
				}
			}
		}

		ret = ChannelReceive(tmpContentRecvBuf, &tmpContentRecvLen);
		if(ret != 0) {
			return -4;
		}

		if((tmpContentRecvBuf[0] & 0xC0) == 0x80) {
			tmpNumber = (tmpContentRecvBuf[0] & 0x38) >> 3;
			if(tmpNumber != *frameNrPtr) {
				orgCmdSendFlag = 1;
				continue;
			} else {
				if((tmpNumber + 1) <= 7) {
					*frameNrPtr = tmpNumber + 1;
				}
				else {
					*frameNrPtr = 0;
				}
			}
			*frameNsPtr = tmpContentRecvBuf[0] & 0x07;

			//send s-frame with RR
			tmpContentSendBuf[0] = 0xC0|(*frameNrPtr);
			tmpContentSendLen = 1;
			ret = ChannelSend(tmpContentSendBuf, tmpContentSendLen);
			if(ret != 0) {
				return -3;
			}

			break;
		} else if((tmpContentRecvBuf[0] & 0xE0) == 0xC0) { //s-frame
			sFrameType = (tmpContentRecvBuf[0] & 0x18) >> 3;
			if(sFrameType == 0x00) {			//RR
				*frameNsPtr = tmpContentRecvBuf[0] & 0x07;
				if(cmdType == CMD_TYPE_SEND_EVENT) {
					break;
				} else {
					orgCmdSendFlag = 0;
					continue;
				}
			} else if(sFrameType == 0x01) { //REJ
				orgCmdSendFlag = 1;
				continue;
			} else if(sFrameType == 0x02)	{	//RNR
				orgCmdSendFlag = 1;
				continue;
			} else {					//SREJ
				return -5;
			}
		} else if((tmpContentRecvBuf[0] & 0xE0) == 0xE0) { //u-frame
			break;
		} else {
			return -5;
		}
	}
	if(idx >= 3) {
		printf("frame retransmission exceeds 3.\n");
		return -6;
	}

	memcpy(scriptRespCmd, tmpContentRecvBuf, tmpContentRecvLen);
	*scriptRespLenPtr = tmpContentRecvLen;

	return 0;
}


int BCD_2_ASC(unsigned char *src, unsigned char *dest, int *srcLen) {
    int i, ascLen;
    char achar, bcdLen;
	unsigned char *bcd, *asc;

	dest[*srcLen] = 0;
	bcd = src;
	asc = dest;
	ascLen = *srcLen;
	*srcLen = (*srcLen +1) >>1;

	if ( ascLen & 0x01 ) {
		achar = *bcd++ & 0x0f;
        *asc++ = (achar > 9) ? achar + 0x41 - 10 : achar + 0x30;
        ascLen--;
    }

	bcdLen = ascLen >>1;
    for(i = 0; i < bcdLen; i++) {
        achar = (*bcd & 0xf0) >> 4;
        *asc++ = (achar > 9) ? achar + 0x41 - 10 : achar + 0x30;
        achar = *bcd++ & 0x0f;
        *asc++ = (achar > 9) ? achar + 0x41 - 10 : achar + 0x30;
    }

	return (int)(asc - dest);
}

unsigned char bcdChar(unsigned char achar) {
	unsigned char result;

	if (isalnum(achar)) {
	   result = toupper(achar);
	} else {
	   result = 'D';
	}
	if (result > 0x46) {
	   result = 'D';
	}
	return result;
}


int ASC_2_BCD(unsigned char *src, unsigned char *dest, int *srcLen) {
    int bcdLen, i, ascLen;
	unsigned char *asc, *bcd, achar;

	asc = src;
	bcd = dest;
	ascLen = *srcLen;

    if ( ascLen & 0x01) {
		achar = bcdChar(*asc++);
        *bcd++ = (achar <= 0x39)? achar - 0x30 : achar - 0x41 + 10;
        ascLen--;
    }

	bcdLen = ascLen >>1;

    for(i=0;i<bcdLen;i++) {
		achar = bcdChar(*asc++);
        *bcd = ((achar <= 0x39) ? achar - 0x30 : achar - 0x41 + 10);
        *bcd = *bcd << 4;
		achar = bcdChar(*asc++);
        *bcd += ((achar <= 0x39) ? achar - 0x30 : achar - 0x41 + 10);
		bcd++;
    }

	return (int)(bcd - dest);
}


void phLlcNfc_H_ComputeCrc(uint8_t *pData, uint8_t length, uint8_t *pCrc1, uint8_t *pCrc2) {
    uint8_t     crc_byte = 0,
                index = 0;
    uint16_t    crc = 0;

#ifdef CRC_A
        crc = 0x6363; /* ITU-V.41 */
#else
        crc = 0xFFFF; /* ISO/IEC 13239 (formerly ISO/IEC 3309) */
#endif /* #ifdef CRC_A */

    do {
        crc_byte = pData[index];
        phLlcNfc_H_UpdateCrc(crc_byte, &crc);
        index++;
    } while (index < length);

#ifndef INVERT_CRC
    crc = ~crc; /* ISO/IEC 13239 (formerly ISO/IEC 3309) */
#endif /* #ifndef INVERT_CRC */

    *pCrc1 = (uint8_t) (crc & 0xFF);
    *pCrc2 = (uint8_t) ((crc >> 8) & 0xFF);
    return;
}

void phLlcNfc_H_UpdateCrc(uint8_t crcByte, uint16_t  *pCrc) {
    crcByte = (crcByte ^ (uint8_t)((*pCrc) & 0x00FF));
    crcByte = (crcByte ^ (uint8_t)(crcByte << 4));
    *pCrc = (*pCrc >> 8) ^ ((uint16_t)crcByte << 8) ^
                ((uint16_t)crcByte << 3) ^
                ((uint16_t)crcByte >> 4);
}


//================================================================================================
// Need to modify: depend on Platform
//================================================================================================

int ChannelOpen(void) {
	//return SerOpen(portNo);
	int ret = 0;
	if ((ret = (fp = open("/dev/pn544", O_RDWR))) < 0) {
		LOGI("pn544 open error retcode = %d, errno = %d\n", ret, errno);
		return	1;
	} else {
		return	0;
	}
}


void ChannelClose() {
	//SerClose();
	int ret = 0;
	
	ret = close(fp);
	LOGI("pn544 close ret = %d\n",ret);
}


int ChannelSend(unsigned char *contSendBuf, int contSendLen) {
//	unsigned char calCRC[2];
	int tmplen;
	int i;
	memset(llcSendBuf, 0x00, sizeof(llcSendBuf));
	llcSendLen = 0;

	//prepare buffer for llc layer
	llcSendBuf[0] = contSendLen + 2;
	memcpy(llcSendBuf + 1, contSendBuf, contSendLen);

	phLlcNfc_H_ComputeCrc(llcSendBuf, contSendLen + 1, llcSendBuf + contSendLen + 1, llcSendBuf + contSendLen + 2);
	llcSendLen = contSendLen + 3;

	//send llc layer command request to chip
	//ret = SerWrite(llcSendBuf, llcSendLen, 3);
	if ((tmplen = (write(fp, llcSendBuf, llcSendLen)))<0)
	{
		printf("pn544 write error retcode = %d, errno = %d\n", tmplen, errno);
	}
	if(tmplen != llcSendLen)
	{
		return -1;
	}

	//dump log from PC to IFD
	//DumpBuffer((char *)llcSendBuf, llcSendLen, 0);
	LOGI("PC->IFD:");

	for(i = 0; i < llcSendLen; i++) {
		LOGI(" %02X", llcSendBuf[i]);
	}
	LOGI("\n");
	return 0;
}


int ChannelReceive(unsigned char *contRecvBuf, unsigned char *contRecvLenPtr) {
	unsigned char calCRC[2];
	int tmplen;
	int i;

	memset(llcRecvBuf, 0x00, sizeof(llcRecvBuf));
	llcRecvLen = 0;

	//receive llc layer command response from chip
	llcRecvLen = 0;
	//tmplen = SerRead(llcRecvBuf, 1, 2);
	if ((tmplen = (read(fp, &llcRecvBuf[0], 1))) < 0) 
	{
		printf("\npn544 read error retcode = %d, errno = %d", tmplen, errno);
	}
	if(tmplen < 1) {
		return -1;
	}
	llcRecvLen += tmplen;

	//tmplen = SerRead(llcRecvBuf + 1, llcRecvBuf[0], 1);
	if ((tmplen = (read(fp, &llcRecvBuf[1], llcRecvBuf[0]))) < 0) 
	{
		AppendInfo("\npn544 read error retcode = %d, errno = %d", tmplen, errno);
	}
	if(tmplen < llcRecvBuf[0]) {
		return -2;
	}
	llcRecvLen += tmplen;
	phLlcNfc_H_ComputeCrc(llcRecvBuf, llcRecvLen - 2, calCRC, calCRC + 1);

	if(memcmp(llcRecvBuf + llcRecvLen - 2, calCRC, 2) == 0) {
		memcpy(contRecvBuf, llcRecvBuf + 1, llcRecvLen - 3);
		*contRecvLenPtr = llcRecvLen - 3;

		//dump log from IFD to PC
		//DumpBuffer((char *)llcRecvBuf, llcRecvLen, 1);
		LOGI("IFD->PC:");
		for(i = 0; i < llcRecvLen; i++) {
			LOGI(" %02X", llcRecvBuf[i]);
		}
		LOGI("\n");
		return 0;
	} else {
		return -3;
	}
}

void resetNfc_ex_pn544() {
	//TODO: need to modify(depend on HW)
	if(fp == 0 && ChannelOpen() != 0) {
		LOGI("Can't open I2C port!");
		return;
	}

	if(fp != 0) {
		ioctl(fp, PN544_SET_PWR, 1);
 		ioctl(fp, PN544_SET_PWR, 0);
		ioctl(fp, PN544_SET_PWR, 1);
		ChannelClose(fp);
	}
}

//direction, 0: PC to IFD; 1: IFD to PC
/*void DumpBuffer(char *buffer, int len, int direction)
{
	char finalStr[200];
	char ascBuf[150];
	int ascLen;

	ascLen = len * 2;
	BCD_2_ASC((unsigned char *)buffer, (unsigned char *)ascBuf, &ascLen);

	if(direction == 0)
	{
		sprintf(finalStr, "PC -> IFD: %s\r\n\r\n", ascBuf);
	}
	else
	{
		sprintf(finalStr, "IFD -> PC: %s\r\n\r\n", ascBuf);
	}

	for(i = 0; i < ascLen; i++)
	{
		printk(" %02X", tmp[i]);
	}
	printk("\n");
}*/


void AppendInfo(char *infoStr)
{
/*	char finalStr[200];

	sprintf(finalStr, "%s\n", infoStr);

	int totallen = m_EdtCmdLog.GetWindowTextLengthA();
	m_EdtCmdLog.SetSel(totallen, totallen);
	m_EdtCmdLog.ReplaceSel(infoStr);*/
}







jboolean SendCarrier_pn547() {

    int ret = 0;

	jboolean result = JNI_FALSE;
	//TODO: need to modify(depend on HW)
	if(phNxpNciHal_TestMode_open() != 0) {
		LOGI("Can't open I2C port!");
		return JNI_FALSE;
	}
	ret = phNxpNciHal_PrbsTestStart(0,0);
	if(ret == 0) {
		LOGI("phNxpNciHal_PrbsTestStart executed successfully!");
		result = JNI_TRUE;
	} else {
		LOGI("phNxpNciHal_PrbsTestStart executed failed here!");
		result = JNI_FALSE;
	}

	phNxpNciHal_TestMode_close();

	return result;
}

jboolean StartReaderMode_pn547() {

	return JNI_TRUE;
}

void resetNfc_ex_pn547() {
	//TODO: need to modify(depend on HW)
	if(phNxpNciHal_TestMode_open() != 0) {
		LOGI("Can't open I2C port!");
		return;
	}
     if(phNxpNciHal_PrbsTestStop() !=0){
		LOGI("phNxpNciHal_PrbsTestStop executed failed here!");
	 }else{
	    LOGI("phNxpNciHal_PrbsTestStop executed successfully!");
	 }
	      
     phNxpNciHal_TestMode_close();
}

void  getNfcChip()
{
	memset(nfcChip, 0x00, sizeof(nfcChip));
	property_get("persist.sys.nfcchip", nfcChip, "pn547");
}


/*
 * Class:     com_android_engineeringmode_util_NfcNativeMethodsUtils
 * Method:    startSendCarrier
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL startSendCarrier(JNIEnv *env, jclass jcls) {

    getNfcChip();
	if(strcmp(nfcChip,"pn544") ==0)
		return SendCarrier_pn544();
	else 
	    return SendCarrier_pn547();
}

/*
 * Class:     com_android_engineeringmode_util_NfcNativeMethodsUtils
 * Method:    startReaderMode
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL startReaderMode(JNIEnv *env, jclass jcls) {
    getNfcChip();
	if(strcmp(nfcChip,"pn544") ==0)
		return StartReaderMode_pn544();
	else 
	    return StartReaderMode_pn547();
}


JNIEXPORT void JNICALL resetNfc(JNIEnv *env, jclass jcls) {
    getNfcChip();
	if(strcmp(nfcChip,"pn544") ==0)
		return resetNfc_ex_pn544();
	else 
	    return resetNfc_ex_pn547();
}

JNINativeMethod gMethods[] = {
	{ "startSendCarrier", "()Z",  (void*) startSendCarrier },
    { "startReaderMode", "()Z",  (void*) startReaderMode },
	{ "resetNfc", "()V",  (void*) resetNfc },
};

int registerNativeMethods(JNIEnv* env, const char* className, JNINativeMethod* gMethods, int numMethods) {
    jclass clazz;

    clazz = (*env)->FindClass(env, className);
    if (clazz == NULL) {
    	LOGI("Native registration unable to find class '%s'\n", className);
        return JNI_FALSE;

    }

    if ((*env)->RegisterNatives(env, clazz, gMethods, numMethods) < 0) {
    	LOGI("RegisterNatives failed for '%s'\n", className);
        return JNI_FALSE;
    }

    return JNI_TRUE;
}



int registerNatives(JNIEnv *env) {
    const char* const kClassPathName = "com/android/engineeringmode/util/NfcNativeMethodsUtils";
    return registerNativeMethods(env, kClassPathName, gMethods, sizeof(gMethods) / sizeof(gMethods[0]));
}

/*
 * Returns the JNI version on success, -1 on failure.
 */

jint JNI_OnLoad(JavaVM* vm, void* reserved) {
    JNIEnv* env = NULL;
    jint result = -1;

    if ((*vm)->GetEnv(vm,(void**) &env, JNI_VERSION_1_4) != JNI_OK) {
    	LOGI("ERROR: GetEnv failed\n");
		goto bail;
	}

    if (!registerNatives(env)) {
    	LOGI("ERROR: BinaryDictionary native registration failed\n");
        goto bail;
    }

    /* success -- return valid version number */
    result = JNI_VERSION_1_4;

bail:
    return result;

}
