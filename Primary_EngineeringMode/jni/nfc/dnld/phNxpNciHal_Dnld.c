/*
 * Copyright (C) 2012-2014 NXP Semiconductors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <phTmlNfc.h>
#include <phDnldNfc.h>
#include <phNxpNciHal_Dnld.h>
#include <phNxpNciHal_utils.h>
#include <phNxpLog.h>
#include <phNxpConfig.h>

/* Macro */
#define PHLIBNFC_IOCTL_DNLD_MAX_ATTEMPTS 3
#define PHLIBNFC_IOCTL_DNLD_GETVERLEN          (0x0BU)
#define PHLIBNFC_IOCTL_DNLD_GETVERLEN_MRA2_1   (0x09U)
#define PHLIBNFC_DNLD_MEM_READ (0xECU)
#define PHLIBNFC_DNLD_MEM_WRITE (0xEDU)
#define PHLIBNFC_DNLD_READ_LOG  (0xEEU)
#define NFC_MEM_READ (0xD0U)
#define NFC_MEM_WRITE (0xD1U)
#define NFC_FW_DOWNLOAD (0x09F7U)

/* External global variable to get FW version */
extern uint16_t wFwVer;
extern uint16_t wMwVer;

/* RF Configuration structure */
typedef struct phLibNfc_IoctlSetRfConfig
{
    uint8_t  bNumOfParams;    /* Number of Rf configurable parameters to be set */
    uint8_t  *pInputBuffer;   /* Buffer containing Rf configurable parameters */
    uint8_t  bSetSysPmuFlag;  /* Flag to decide wether to set SystemPmu or no from the first byte */
}phLibNfc_IoctlSetRfConfig;

/* Structure to hold information from EEPROM */
typedef struct phLibNfc_EELogParams
{
    uint16_t     wCurrMwVer;        /* Holds current MW version on the chip */
    uint16_t     wCurrFwVer;        /* Holds current FW version on the chip */
    uint16_t     wNumDnldTrig;      /* Total number of times dnld has been attempted */
    uint16_t     wNumDnldSuccess;   /* Total number of times dnld has been successful */
    uint16_t     wNumDnldFail;      /* Total number of times dnld has Failed */
    uint16_t     wDnldFailCnt;      /* holds the number of times dnld has failed,will be reset on success */
    bool_t       bConfig;           /* Flag to be set in dnld mode after successful dnld,to be reset in NCI Mode
                                      after setting the NCI configuration */
} phLibNfc_EELogParams_t;

/* FW download module context structure */
typedef struct
{
    bool_t                       bDnldEepromWrite;  /* Flag to indicate eeprom write request*/
    bool_t                       bSkipSeq;       /* Flag to indicate FW download sequence to be skipped or not */
    bool_t                       bSkipReset;     /* Flag to indicate Reset cmd to be skipped or not in FW download sequence */
    bool_t                       bSkipForce;     /* Flag to indicate Force cmd to be skipped or not in FW recovery sequence */
    bool_t                       bPrevSessnOpen; /* Flag to indicate previous download session is open or not */
    bool_t                       bLibNfcCtxtMem; /* flag to indicate if mem was allocated for gpphLibNfc_Context */
    bool_t                       bDnldInitiated; /* Flag to indicate if fw upgrade was initiated */
    bool_t                       bSendNciCmd;    /* Flag to indicate if NCI cmd to be sent or not,after PKU */
    uint8_t                      bChipVer;       /* holds the hw chip version */
    bool_t                       bDnldRecovery;  /* Flag to indicate if dnld recovery sequence needs to be triggered */
    bool_t                       bForceDnld;     /* Flag to indicate if forced download option is enabled */
    bool_t                       bRetryDnld;     /* Flag to indicate retry download after successful recovery complete */
    uint8_t                      bDnldAttempts;  /* Holds the count of no. of dnld attempts made.max 3 */
    uint16_t                     IoctlCode;      /* Ioctl code*/
    bool_t                       bDnldAttemptFailed;  /* Flag to indicate last download attempt failed */
    NFCSTATUS                    bLastStatus;    /* Holds the actual download write attempt status */
    phLibNfc_EELogParams_t       tLogParams;     /* holds the params that could be logged to reserved EE address */
    uint8_t                      bClkSrcVal;     /* Holds the System clock source read from config file */
    uint8_t                      bClkFreqVal;    /* Holds the System clock frequency read from config file */
} phNxpNciHal_fw_Ioctl_Cntx_t;


/* Global variables used in this file only*/
static phNxpNciHal_fw_Ioctl_Cntx_t gphNxpNciHal_fw_IoctlCtx;

/* Local function prototype */
static NFCSTATUS
phNxpNciHal_fw_dnld_reset(void* pContext, NFCSTATUS status, void* pInfo);

static void
phNxpNciHal_fw_dnld_reset_cb(void* pContext, NFCSTATUS status, void* pInfo);

static NFCSTATUS
phNxpNciHal_fw_dnld_force(void* pContext, NFCSTATUS status, void* pInfo);

static void
phNxpNciHal_fw_dnld_force_cb(void* pContext, NFCSTATUS status, void* pInfo);

static void
phNxpNciHal_fw_dnld_normal_cb(void* pContext, NFCSTATUS status, void* pInfo);

static NFCSTATUS
phNxpNciHal_fw_dnld_normal(void* pContext, NFCSTATUS status, void* pInfo);

static void
phNxpNciHal_fw_dnld_get_version_cb(void* pContext, NFCSTATUS status, void* pInfo);

static NFCSTATUS
phNxpNciHal_fw_dnld_get_version(void* pContext, NFCSTATUS status, void* pInfo);

static void
phNxpNciHal_fw_dnld_get_sessn_state_cb(void* pContext, NFCSTATUS status, void* pInfo);

static NFCSTATUS
phNxpNciHal_fw_dnld_get_sessn_state(void* pContext, NFCSTATUS status, void* pInfo);

static void
phNxpNciHal_fw_dnld_log_read_cb(void* pContext, NFCSTATUS status, void* pInfo);

static NFCSTATUS
phNxpNciHal_fw_dnld_log_read(void* pContext, NFCSTATUS status, void* pInfo);

static void
phNxpNciHal_fw_dnld_write_cb(void* pContext, NFCSTATUS status, void* pInfo);

static NFCSTATUS
phNxpNciHal_fw_dnld_write(void* pContext, NFCSTATUS status, void* pInfo);

static void
phNxpNciHal_fw_dnld_chk_integrity_cb(void* pContext, NFCSTATUS status, void* pInfo);

static NFCSTATUS
phNxpNciHal_fw_dnld_chk_integrity(void* pContext, NFCSTATUS status, void* pInfo);

static void
phNxpNciHal_fw_dnld_log_cb(void* pContext, NFCSTATUS status, void* pInfo);

static NFCSTATUS
phNxpNciHal_fw_dnld_log(void* pContext, NFCSTATUS status, void* pInfo);

static void
phNxpNciHal_fw_dnld_send_ncicmd_Cb(void* pContext, NFCSTATUS status, void* pInfo);

static NFCSTATUS
phNxpNciHal_fw_dnld_send_ncicmd(void* pContext, NFCSTATUS status, void* pInfo);

static NFCSTATUS
phNxpNciHal_fw_dnld_recover(void* pContext, NFCSTATUS status, void* pInfo);

static NFCSTATUS
phNxpNciHal_fw_dnld_complete(void* pContext, NFCSTATUS status, void* pInfo);

/** Internal function to verify Crc Status byte received during CheckIntegrity */
static NFCSTATUS
phLibNfc_VerifyCrcStatus(uint8_t bCrcStatus);

static void phNxpNciHal_fw_dnld_recover_cb(void* pContext, NFCSTATUS status,
        void* pInfo);

static NFCSTATUS phNxpNciHal_fw_seq_handler(NFCSTATUS (*seq_handler[])(void* pContext, NFCSTATUS status, void* pInfo));

/* Array of pointers to start fw download seq */
static NFCSTATUS (*phNxpNciHal_dwnld_seqhandler[])(
        void* pContext, NFCSTATUS status, void* pInfo) = {
    phNxpNciHal_fw_dnld_normal,
    phNxpNciHal_fw_dnld_normal,
    phNxpNciHal_fw_dnld_get_sessn_state,
    phNxpNciHal_fw_dnld_get_version,
    phNxpNciHal_fw_dnld_log_read,
    phNxpNciHal_fw_dnld_write,
    phNxpNciHal_fw_dnld_get_sessn_state,
    phNxpNciHal_fw_dnld_get_version,
    phNxpNciHal_fw_dnld_log,
    phNxpNciHal_fw_dnld_chk_integrity,
    NULL
};

/* Download Recovery Sequence */
static NFCSTATUS (*phNxpNciHal_dwnld_rec_seqhandler[])(
        void* pContext, NFCSTATUS status, void* pInfo) = {
    phNxpNciHal_fw_dnld_reset,
    phNxpNciHal_fw_dnld_force,
    phNxpNciHal_fw_dnld_recover,
    phNxpNciHal_fw_dnld_send_ncicmd,
    NULL
};

/* Download Log Sequence */
static NFCSTATUS (*phNxpNciHal_dwnld_log_seqhandler[])(
        void* pContext, NFCSTATUS status, void* pInfo) = {
    phNxpNciHal_fw_dnld_log,
    NULL
};

/*******************************************************************************
**
** Function         phNxpNciHal_fw_dnld_reset_cb
**
** Description      Download Reset callback
**
** Returns          None
**
*******************************************************************************/
static void phNxpNciHal_fw_dnld_reset_cb(void* pContext, NFCSTATUS status,
        void* pInfo)
{
    phNxpNciHal_Sem_t *p_cb_data = (phNxpNciHal_Sem_t*) pContext;

    if (NFCSTATUS_SUCCESS == status)
    {
        NXPLOG_FWDNLD_D("phNxpNciHal_fw_dnld_reset_cb - Request Successful");
    }
    else
    {
        NXPLOG_FWDNLD_E("phNxpNciHal_fw_dnld_reset_cb - Request Failed!!");
    }
    p_cb_data->status = status;

    SEM_POST(p_cb_data);

    return;
}

/*******************************************************************************
**
** Function         phNxpNciHal_fw_dnld_reset
**
** Description      Download Reset
**
** Returns          NFCSTATUS_SUCCESS if success
**
*******************************************************************************/
static NFCSTATUS phNxpNciHal_fw_dnld_reset(void* pContext, NFCSTATUS status,
        void* pInfo)
{
    NFCSTATUS wStatus = NFCSTATUS_SUCCESS;
    phNxpNciHal_Sem_t cb_data;

    if((TRUE == (gphNxpNciHal_fw_IoctlCtx.bSkipSeq)) || (TRUE == (gphNxpNciHal_fw_IoctlCtx.bSkipReset)))
    {
        if(TRUE == (gphNxpNciHal_fw_IoctlCtx.bSkipReset))
        {
            (gphNxpNciHal_fw_IoctlCtx.bSkipReset) = FALSE;
        }
        return NFCSTATUS_SUCCESS;
    }

    if (phNxpNciHal_init_cb_data(&cb_data, NULL) != NFCSTATUS_SUCCESS)
    {
        NXPLOG_FWDNLD_E("phNxpNciHal_fw_dnld_reset Create dnld_cb_data  failed");
        return NFCSTATUS_FAILED;
    }
    wStatus = phDnldNfc_Reset((pphDnldNfc_RspCb_t)&phNxpNciHal_fw_dnld_reset_cb, (void*) &cb_data);

    if (wStatus != NFCSTATUS_PENDING)
    {
        NXPLOG_FWDNLD_E("phDnldNfc_Reset failed");
        wStatus = NFCSTATUS_FAILED;
        goto clean_and_return;
    }

    /* Wait for callback response */
    if (SEM_WAIT(cb_data))
    {
        NXPLOG_FWDNLD_E("phNxpNciHal_fw_dnld_reset semaphore error");
        wStatus = NFCSTATUS_FAILED;
        goto clean_and_return;
    }

    if (cb_data.status != NFCSTATUS_SUCCESS)
    {
        NXPLOG_FWDNLD_E("phNxpNciHal_fw_dnld_reset cb failed");
        wStatus = NFCSTATUS_FAILED;
        goto clean_and_return;
    }

    wStatus = NFCSTATUS_SUCCESS;

clean_and_return:
    phNxpNciHal_cleanup_cb_data(&cb_data);


    return wStatus;
}

/*******************************************************************************
**
** Function         phNxpNciHal_fw_dnld_normal_cb
**
** Description      Download Normal callback
**
** Returns          None
**
*******************************************************************************/
static void phNxpNciHal_fw_dnld_normal_cb(void* pContext, NFCSTATUS status,
        void* pInfo)
{
    phNxpNciHal_Sem_t *p_cb_data = (phNxpNciHal_Sem_t*) pContext;

    if (NFCSTATUS_SUCCESS == status)
    {
        NXPLOG_FWDNLD_D("phNxpNciHal_fw_dnld_normal_cb - Request Successful");
    }
    else
    {
        NXPLOG_FWDNLD_E("phNxpNciHal_fw_dnld_normal_cb - Request Failed!!");
        /* In this fail scenario trick the sequence handler to call next recover sequence */
        status = NFCSTATUS_SUCCESS;
    }
    p_cb_data->status = status;

    SEM_POST(p_cb_data);
    usleep(1000 * 10);

    return;
}

/*******************************************************************************
**
** Function         phNxpNciHal_fw_dnld_force_cb
**
** Description      Download Force callback
**
** Returns          None
**
*******************************************************************************/
static void phNxpNciHal_fw_dnld_force_cb(void* pContext, NFCSTATUS status,
        void* pInfo)
{
    phNxpNciHal_Sem_t *p_cb_data = (phNxpNciHal_Sem_t*) pContext;

    if (NFCSTATUS_SUCCESS == status)
    {
        NXPLOG_FWDNLD_D("phLibNfc_DnldForceCb - Request Successful");
        (gphNxpNciHal_fw_IoctlCtx.bDnldRecovery) = FALSE;
        (gphNxpNciHal_fw_IoctlCtx.bRetryDnld) = TRUE;
        (gphNxpNciHal_fw_IoctlCtx.bSkipReset) = TRUE;
    }
    else
    {
        /* In this fail scenario trick the sequence handler to call next recover sequence */
        status = NFCSTATUS_SUCCESS;
        NXPLOG_FWDNLD_E("phLibNfc_DnldForceCb - Request Failed!!");

    }
    p_cb_data->status = status;

    SEM_POST(p_cb_data);
    usleep(1000 * 10);

    return;
}

/*******************************************************************************
**
** Function         phNxpNciHal_fw_dnld_normal
**
** Description      Download Normal
**
** Returns          NFCSTATUS_SUCCESS if success
**
*******************************************************************************/
static NFCSTATUS phNxpNciHal_fw_dnld_normal(void* pContext, NFCSTATUS status,
        void* pInfo)
{
    NFCSTATUS wStatus = NFCSTATUS_SUCCESS;
    uint8_t bClkVal[2];
    phDnldNfc_Buff_t tData;
    phNxpNciHal_Sem_t cb_data;

    if(TRUE == (gphNxpNciHal_fw_IoctlCtx.bSkipForce))
    {
        return NFCSTATUS_SUCCESS;
    }
    else
    {
        /*
        bClkVal[0] = NXP_SYS_CLK_SRC_SEL;
        bClkVal[1] = NXP_SYS_CLK_FREQ_SEL;
        */
        bClkVal[0] = gphNxpNciHal_fw_IoctlCtx.bClkSrcVal;
        bClkVal[1] = gphNxpNciHal_fw_IoctlCtx.bClkFreqVal;

        (tData.pBuff) = bClkVal;
        (tData.wLen) = sizeof(bClkVal);

        if(TRUE == (gphNxpNciHal_fw_IoctlCtx.bDnldRecovery))
        {
            (gphNxpNciHal_fw_IoctlCtx.bDnldAttempts)++;
        }

        if (phNxpNciHal_init_cb_data(&cb_data, NULL) != NFCSTATUS_SUCCESS)
        {
            NXPLOG_FWDNLD_E("phNxpNciHal_fw_dnld_reset Create dnld_cb_data  failed");
            return NFCSTATUS_FAILED;
        }
        wStatus = phDnldNfc_Force(&tData,(pphDnldNfc_RspCb_t)&phNxpNciHal_fw_dnld_normal_cb, (void*) &cb_data);

        if(NFCSTATUS_PENDING != wStatus)
        {
            NXPLOG_FWDNLD_E("phDnldNfc_Normal failed");
            (gphNxpNciHal_fw_IoctlCtx.bSkipForce) = FALSE;
            (gphNxpNciHal_fw_IoctlCtx.bRetryDnld) = FALSE;
            goto clean_and_return;
        }
    }

    /* Wait for callback response */
    if (SEM_WAIT(cb_data))
    {
        NXPLOG_FWDNLD_E("phNxpNciHal_fw_dnld_normal semaphore error");
        wStatus = NFCSTATUS_FAILED;
        goto clean_and_return;
    }

    if (cb_data.status != NFCSTATUS_SUCCESS)
    {
        NXPLOG_FWDNLD_E("phNxpNciHal_fw_dnld_normal cb failed");
        wStatus = NFCSTATUS_FAILED;
        goto clean_and_return;
    }

    wStatus = NFCSTATUS_SUCCESS;

clean_and_return:
    phNxpNciHal_cleanup_cb_data(&cb_data);

    return wStatus;
}

/*******************************************************************************
**
** Function         phNxpNciHal_fw_dnld_force
**
** Description      Download Force
**
** Returns          NFCSTATUS_SUCCESS if success
**
*******************************************************************************/
static NFCSTATUS phNxpNciHal_fw_dnld_force(void* pContext, NFCSTATUS status,
        void* pInfo)
{
    NFCSTATUS wStatus = NFCSTATUS_SUCCESS;
    uint8_t bClkVal[2];
    phDnldNfc_Buff_t tData;
    phNxpNciHal_Sem_t cb_data;

    if(TRUE == (gphNxpNciHal_fw_IoctlCtx.bSkipForce))
    {
        return NFCSTATUS_SUCCESS;
    }
    else
    {
        /*
        bClkVal[0] = NXP_SYS_CLK_SRC_SEL;
        bClkVal[1] = NXP_SYS_CLK_FREQ_SEL;
        */
        bClkVal[0] = gphNxpNciHal_fw_IoctlCtx.bClkSrcVal;
        bClkVal[1] = gphNxpNciHal_fw_IoctlCtx.bClkFreqVal;

        (tData.pBuff) = bClkVal;
        (tData.wLen) = sizeof(bClkVal);

        if(TRUE == (gphNxpNciHal_fw_IoctlCtx.bDnldRecovery))
        {
            (gphNxpNciHal_fw_IoctlCtx.bDnldAttempts)++;
        }

        if (phNxpNciHal_init_cb_data(&cb_data, NULL) != NFCSTATUS_SUCCESS)
        {
            NXPLOG_FWDNLD_E("phNxpNciHal_fw_dnld_reset Create dnld_cb_data  failed");
            return NFCSTATUS_FAILED;
        }
        wStatus = phDnldNfc_Force(&tData,(pphDnldNfc_RspCb_t)&phNxpNciHal_fw_dnld_force_cb, (void*) &cb_data);

        if(NFCSTATUS_PENDING != wStatus)
        {
            NXPLOG_FWDNLD_E("phDnldNfc_Force failed");
            (gphNxpNciHal_fw_IoctlCtx.bSkipForce) = FALSE;
            (gphNxpNciHal_fw_IoctlCtx.bRetryDnld) = FALSE;
            goto clean_and_return;
        }
    }

    /* Wait for callback response */
    if (SEM_WAIT(cb_data))
    {
        NXPLOG_FWDNLD_E("phNxpNciHal_fw_dnld_force semaphore error");
        wStatus = NFCSTATUS_FAILED;
        goto clean_and_return;
    }

    if (cb_data.status != NFCSTATUS_SUCCESS)
    {
        NXPLOG_FWDNLD_E("phNxpNciHal_fw_dnld_force cb failed");
        wStatus = NFCSTATUS_FAILED;
        goto clean_and_return;
    }

    wStatus = NFCSTATUS_SUCCESS;

clean_and_return:
    phNxpNciHal_cleanup_cb_data(&cb_data);

    return wStatus;
}

/*******************************************************************************
**
** Function         phNxpNciHal_fw_dnld_get_version_cb
**
** Description      Download Get version callback
**
** Returns          None
**
*******************************************************************************/
static void phNxpNciHal_fw_dnld_get_version_cb(void* pContext,
        NFCSTATUS status, void* pInfo)
{
    phNxpNciHal_Sem_t *p_cb_data = (phNxpNciHal_Sem_t*) pContext;
    NFCSTATUS wStatus = status;
    pphDnldNfc_Buff_t   pRespBuff;
    uint16_t wFwVern = 0;
    uint16_t wMwVern = 0;
    uint8_t bHwVer = 0;
    uint8_t bExpectedLen = 0;
    uint8_t bNewVer[2];
    uint8_t bCurrVer[2];

    if ((NFCSTATUS_SUCCESS == wStatus) && (NULL != pInfo))
    {
        NXPLOG_FWDNLD_D ("phNxpNciHal_fw_dnld_get_version_cb - Request Successful");

        pRespBuff = (pphDnldNfc_Buff_t) pInfo;

        if ((0 != pRespBuff->wLen) && (NULL != pRespBuff->pBuff))
        {
            bHwVer = (pRespBuff->pBuff[0]);
            bHwVer &= 0x0F; /* 0x0F is the mask to extract chip version */

            if ((PHDNLDNFC_HWVER_MRA2_1 == bHwVer) || (PHDNLDNFC_HWVER_MRA2_2 == bHwVer))
            {
                bExpectedLen = PHLIBNFC_IOCTL_DNLD_GETVERLEN_MRA2_1;
                (gphNxpNciHal_fw_IoctlCtx.bChipVer) = bHwVer;
            }
            else if ((bHwVer >= PHDNLDNFC_HWVER_MRA1_0) && (bHwVer
                        <= PHDNLDNFC_HWVER_MRA2_0))
            {
                bExpectedLen = PHLIBNFC_IOCTL_DNLD_GETVERLEN;
                (gphNxpNciHal_fw_IoctlCtx.bChipVer) = bHwVer;
            }
            else
            {
                wStatus = NFCSTATUS_FAILED;
                NXPLOG_FWDNLD_E("phNxpNciHal_fw_dnld_get_version_cb - Invalid ChipVersion!!");
            }
        }
        else
        {
            wStatus = NFCSTATUS_FAILED;
            NXPLOG_FWDNLD_E("phNxpNciHal_fw_dnld_get_version_cb - Version Resp Buff Invalid...\n");
        }

        if ((NFCSTATUS_SUCCESS == wStatus) && (bExpectedLen == pRespBuff->wLen)
                && (NULL != pRespBuff->pBuff))
        {
            NXPLOG_FWDNLD_D("phNxpNciHal_fw_dnld_get_version_cb - Valid Version Resp Buff!!...\n");

            /* Validate version details to confirm if continue with the next sequence of Operations. */
            memcpy(bCurrVer, &(pRespBuff->pBuff[bExpectedLen - 2]),
                    sizeof(bCurrVer));
            wFwVern = wFwVer;
            wMwVern = wMwVer;

            memcpy(bNewVer,&wFwVern,sizeof(bNewVer));

            /* check if the ROM code version and FW Major version is valid for the chip*/
            /* ES2.2 Rom Version - 0x7 and Valid FW Major Version - 0x1 */
            if ((pRespBuff->pBuff[1] == 0x07) &&(bNewVer[1] !=0x01))
            {
                NXPLOG_FWDNLD_E("C1 FW on C2 chip is not allowed - FW Major Version!= 1 on ES2.2");
                wStatus = NFCSTATUS_NOT_ALLOWED;
            }
            /* Major Version number check */
            else if((FALSE == (gphNxpNciHal_fw_IoctlCtx.bDnldInitiated)) && (bNewVer[1] < bCurrVer[1]))
            {
                NXPLOG_FWDNLD_E("Version Check Failed - MajorVerNum Mismatch\n");
                wStatus = NFCSTATUS_NOT_ALLOWED;
            }
            /* Minor Version number check - before download.*/
            else if((FALSE == (gphNxpNciHal_fw_IoctlCtx.bDnldInitiated)) && ((bNewVer[0] == bCurrVer[0]) &&
                        (bNewVer[1] == bCurrVer[1])))
            {
                wStatus = NFCSTATUS_SUCCESS;
#if (PH_LIBNFC_ENABLE_FORCE_DOWNLOAD == 0)
                NXPLOG_FWDNLD_D("Version Already UpToDate!!\n");
                (gphNxpNciHal_fw_IoctlCtx.bSkipSeq) = TRUE;
#else
                (gphNxpNciHal_fw_IoctlCtx.bForceDnld) = TRUE;
#endif

            }
            /* Minor Version number check - after download
             * after download, we should get the same version information.*/
            else if ((TRUE == (gphNxpNciHal_fw_IoctlCtx.bDnldInitiated)) && ((bNewVer[0] != bCurrVer[0]) ||
                        (bNewVer[1] != bCurrVer[1])))
            {
                NXPLOG_FWDNLD_E("Version Not Updated After Download!!\n");
                wStatus = NFCSTATUS_FAILED;
            }
            else
            {
                NXPLOG_FWDNLD_D("Version Check Successful\n");
                /* Store the Mw & Fw Version for updating in EEPROM Log Area after successful download */
                if(TRUE == (gphNxpNciHal_fw_IoctlCtx.bDnldInitiated))
                {
                    NXPLOG_FWDNLD_W("Updating Fw & Mw Versions..");
                    (gphNxpNciHal_fw_IoctlCtx.tLogParams.wCurrMwVer) = wMwVern;
                    (gphNxpNciHal_fw_IoctlCtx.tLogParams.wCurrFwVer) = wFwVern;
                }

            }
        }
        else
        {
            NXPLOG_FWDNLD_E("phNxpNciHal_fw_dnld_get_version_cb - Version Resp Buff Invalid...\n");
        }
    }
    else
    {
        wStatus = NFCSTATUS_FAILED;
        NXPLOG_FWDNLD_E("phNxpNciHal_fw_dnld_get_version_cb - Request Failed!!");
    }

    p_cb_data->status = wStatus;
    SEM_POST(p_cb_data);
    return;
}

/*******************************************************************************
**
** Function         phNxpNciHal_fw_dnld_get_version
**
** Description      Download Get version
**
** Returns          NFCSTATUS_SUCCESS if success
**
*******************************************************************************/
static NFCSTATUS phNxpNciHal_fw_dnld_get_version(void* pContext,
        NFCSTATUS status, void* pInfo)
{
    NFCSTATUS wStatus = NFCSTATUS_SUCCESS;
    phNxpNciHal_Sem_t cb_data;
    static uint8_t bGetVerRes[11];
    phDnldNfc_Buff_t tDnldBuff;

    if((TRUE == (gphNxpNciHal_fw_IoctlCtx.bSkipSeq)) ||
            (TRUE == (gphNxpNciHal_fw_IoctlCtx.bPrevSessnOpen)))
    {
        return NFCSTATUS_SUCCESS;
    }

    if (phNxpNciHal_init_cb_data(&cb_data, NULL) != NFCSTATUS_SUCCESS)
    {
        NXPLOG_FWDNLD_E("phNxpNciHal_fw_dnld_get_version cb_data creation failed");
        return NFCSTATUS_FAILED;
    }

    tDnldBuff.pBuff = bGetVerRes;
    tDnldBuff.wLen = sizeof(bGetVerRes);

    wStatus = phDnldNfc_GetVersion(&tDnldBuff,
            (pphDnldNfc_RspCb_t) &phNxpNciHal_fw_dnld_get_version_cb,
            (void*) &cb_data);
    if (wStatus != NFCSTATUS_PENDING)
    {
        NXPLOG_FWDNLD_E("phNxpNciHal_fw_dnld_get_version failed");
        status = NFCSTATUS_FAILED;
        goto clean_and_return;
    }
    /* Wait for callback response */
    if (SEM_WAIT(cb_data))
    {
        NXPLOG_FWDNLD_E("phNxpNciHal_fw_dnld_get_version semaphore error");
        wStatus = NFCSTATUS_FAILED;
        goto clean_and_return;
    }

    if (cb_data.status != NFCSTATUS_SUCCESS)
    {
        NXPLOG_FWDNLD_E("phNxpNciHal_fw_dnld_get_version cb failed");
        wStatus = NFCSTATUS_FAILED;
        goto clean_and_return;
    }

    wStatus = NFCSTATUS_SUCCESS;

clean_and_return:
    phNxpNciHal_cleanup_cb_data(&cb_data);

    return wStatus;
}

/*******************************************************************************
**
** Function         phNxpNciHal_fw_dnld_get_sessn_state_cb
**
** Description      Download Get session state callback
**
** Returns          None
**
*******************************************************************************/
static void phNxpNciHal_fw_dnld_get_sessn_state_cb(void* pContext,
        NFCSTATUS status, void* pInfo)
{
    phNxpNciHal_Sem_t *p_cb_data = (phNxpNciHal_Sem_t*) pContext;
    NFCSTATUS wStatus = status;
    pphDnldNfc_Buff_t pRespBuff;
    if ((NFCSTATUS_SUCCESS == wStatus) && (NULL != pInfo))
    {
        NXPLOG_FWDNLD_D("phNxpNciHal_fw_dnld_get_sessn_state_cb - Request Successful");

        pRespBuff = (pphDnldNfc_Buff_t) pInfo;

        if ((3 == (pRespBuff->wLen)) && (NULL != (pRespBuff->pBuff)))
        {
            NXPLOG_FWDNLD_D("phNxpNciHal_fw_dnld_get_sessn_state_cb - Valid Session State Resp Buff!!...");

            if (phDnldNfc_LCOper == pRespBuff->pBuff[2])
            {
                if (PHLIBNFC_FWDNLD_SESSNOPEN == pRespBuff->pBuff[0])
                {
                    NXPLOG_FWDNLD_E("Prev Fw Upgrade Session still Open..");
                    (gphNxpNciHal_fw_IoctlCtx.bPrevSessnOpen) = TRUE;
                    if(TRUE == (gphNxpNciHal_fw_IoctlCtx.bDnldInitiated))
                    {
                        NXPLOG_FWDNLD_D("Session still Open after Prev Fw Upgrade attempt!!");

                        if((gphNxpNciHal_fw_IoctlCtx.bDnldAttempts) < PHLIBNFC_IOCTL_DNLD_MAX_ATTEMPTS)
                        {
                            NXPLOG_FWDNLD_W("Setting Dnld Retry ..");
                            (gphNxpNciHal_fw_IoctlCtx.bRetryDnld) = TRUE;
                        }
                        else
                        {
                            NXPLOG_FWDNLD_E("Max Dnld Retry Counts Exceeded!!");
                            (gphNxpNciHal_fw_IoctlCtx.bRetryDnld) = FALSE;
                        }
                        wStatus = NFCSTATUS_FAILED;
                    }
                }
                else
                {
                    gphNxpNciHal_fw_IoctlCtx.bPrevSessnOpen = FALSE;
                }
            }
            else
            {
                wStatus = NFCSTATUS_FAILED;
                NXPLOG_FWDNLD_E("PN547 not in Operational State..Fw Upgrade not allowed!!");
            }
        }
        else
        {
            wStatus = NFCSTATUS_FAILED;
            NXPLOG_FWDNLD_E("phNxpNciHal_fw_dnld_get_sessn_state_cb - Session State Resp Buff Invalid...");
        }
    }
    else
    {
        wStatus = NFCSTATUS_FAILED;
        NXPLOG_FWDNLD_E("phNxpNciHal_fw_dnld_get_sessn_state_cb - Request Failed!!");
    }

    p_cb_data->status = wStatus;

    SEM_POST(p_cb_data);

    return;
}

/*******************************************************************************
**
** Function         phNxpNciHal_fw_dnld_get_sessn_state
**
** Description      Download Get session state
**
** Returns          NFCSTATUS_SUCCESS if success
**
*******************************************************************************/
static NFCSTATUS phNxpNciHal_fw_dnld_get_sessn_state(void* pContext,
        NFCSTATUS status, void* pInfo)
{
    phDnldNfc_Buff_t tDnldBuff;
    static uint8_t bGSnStateRes[3];
    NFCSTATUS wStatus = NFCSTATUS_SUCCESS;
    phNxpNciHal_Sem_t cb_data;

    if (TRUE == gphNxpNciHal_fw_IoctlCtx.bSkipSeq)
    {
        return NFCSTATUS_SUCCESS;
    }

    if (phNxpNciHal_init_cb_data(&cb_data, NULL) != NFCSTATUS_SUCCESS)
    {
        NXPLOG_FWDNLD_E("phNxpNciHal_fw_dnld_get_version cb_data creation failed");
        return NFCSTATUS_FAILED;
    }

    tDnldBuff.pBuff = bGSnStateRes;
    tDnldBuff.wLen = sizeof(bGSnStateRes);

    wStatus = phDnldNfc_GetSessionState(&tDnldBuff,
            &phNxpNciHal_fw_dnld_get_sessn_state_cb, (void *) &cb_data);
    if (wStatus != NFCSTATUS_PENDING)
    {
        NXPLOG_FWDNLD_E("phDnldNfc_GetSessionState failed");
        status = NFCSTATUS_FAILED;
        goto clean_and_return;
    }

    /* Wait for callback response */
    if (SEM_WAIT(cb_data))
    {
        NXPLOG_FWDNLD_E("phDnldNfc_GetSessionState semaphore error");
        wStatus = NFCSTATUS_FAILED;
        goto clean_and_return;
    }

    if (cb_data.status != NFCSTATUS_SUCCESS)
    {
        NXPLOG_FWDNLD_E("phDnldNfc_GetSessionState cb failed");
        wStatus = NFCSTATUS_FAILED;
        goto clean_and_return;
    }

    wStatus = NFCSTATUS_SUCCESS;

clean_and_return:
    phNxpNciHal_cleanup_cb_data(&cb_data);

    return wStatus;
}

/*******************************************************************************
**
** Function         phNxpNciHal_fw_dnld_log_read_cb
**
** Description      Download Logread callback
**
** Returns          None
**
*******************************************************************************/
static void phNxpNciHal_fw_dnld_log_read_cb(void* pContext, NFCSTATUS status,
        void* pInfo)
{
    phNxpNciHal_Sem_t *p_cb_data = (phNxpNciHal_Sem_t*) pContext;

    if((NFCSTATUS_SUCCESS == status) && (NULL != pInfo))
    {
        NXPLOG_FWDNLD_D("phNxpNciHal_fw_dnld_log_read_cb - Request Successful");
    }
    else
    {
        status = NFCSTATUS_FAILED;
        NXPLOG_FWDNLD_E("phNxpNciHal_fw_dnld_log_read_cb - Request Failed!!");
    }

    p_cb_data->status = status;
    SEM_POST(p_cb_data);

    return;
}

/*******************************************************************************
**
** Function         phNxpNciHal_fw_dnld_log_read
**
** Description      Download Log Read
**
** Returns          NFCSTATUS_SUCCESS if success
**
*******************************************************************************/
static NFCSTATUS phNxpNciHal_fw_dnld_log_read(void* pContext, NFCSTATUS status,
        void* pInfo)
{
    NFCSTATUS wStatus = NFCSTATUS_SUCCESS;
    phNxpNciHal_Sem_t cb_data;
    phDnldNfc_Buff_t Data;

    if((((TRUE == (gphNxpNciHal_fw_IoctlCtx.bSkipSeq)) || (TRUE == (gphNxpNciHal_fw_IoctlCtx.bForceDnld))) &&
                (FALSE == (gphNxpNciHal_fw_IoctlCtx.bPrevSessnOpen))) || (((TRUE == (gphNxpNciHal_fw_IoctlCtx.bPrevSessnOpen))) &&
                    (TRUE == (gphNxpNciHal_fw_IoctlCtx.bRetryDnld))))

    {
        return NFCSTATUS_SUCCESS;
    }

    if (phNxpNciHal_init_cb_data(&cb_data, NULL) != NFCSTATUS_SUCCESS)
    {
        NXPLOG_FWDNLD_E("phNxpNciHal_fw_dnld_log_read cb_data creation failed");
        return NFCSTATUS_FAILED;
    }

    (Data.pBuff) = (void *)&(gphNxpNciHal_fw_IoctlCtx.tLogParams);
    (Data.wLen) = sizeof(phLibNfc_EELogParams_t);

    wStatus = phDnldNfc_ReadLog(&Data,
            (pphDnldNfc_RspCb_t) &phNxpNciHal_fw_dnld_log_read_cb,
            (void *) &cb_data);
    if (wStatus != NFCSTATUS_PENDING)
    {
        NXPLOG_FWDNLD_E("phNxpNciHal_fw_dnld_log_read failed");
        status = NFCSTATUS_FAILED;
        goto clean_and_return;
    }

    /* Wait for callback response */
    if (SEM_WAIT(cb_data))
    {
        NXPLOG_FWDNLD_E("phNxpNciHal_fw_dnld_log_read semaphore error");
        wStatus = NFCSTATUS_FAILED;
        goto clean_and_return;
    }

    if (cb_data.status != NFCSTATUS_SUCCESS)
    {
        NXPLOG_FWDNLD_E("phNxpNciHal_fw_dnld_log_read cb failed");
        wStatus = NFCSTATUS_FAILED;
        goto clean_and_return;
    }

    wStatus = NFCSTATUS_SUCCESS;

clean_and_return:
    phNxpNciHal_cleanup_cb_data(&cb_data);

    return wStatus;
}

/*******************************************************************************
**
** Function         phNxpNciHal_fw_dnld_write_cb
**
** Description      Download Write callback
**
** Returns          None
**
*******************************************************************************/
static void phNxpNciHal_fw_dnld_write_cb(void* pContext, NFCSTATUS status,
        void* pInfo)
{
    phNxpNciHal_Sem_t *p_cb_data = (phNxpNciHal_Sem_t*) pContext;

    if (NFCSTATUS_SUCCESS == status)
    {
        NXPLOG_FWDNLD_D("phNxpNciHal_fw_dnld_write_cb - Request Successful");
        (gphNxpNciHal_fw_IoctlCtx.bDnldEepromWrite) = FALSE;
        if(TRUE == (gphNxpNciHal_fw_IoctlCtx.bDnldInitiated))
        {
            (gphNxpNciHal_fw_IoctlCtx.tLogParams.wNumDnldSuccess) += 1;

            if((gphNxpNciHal_fw_IoctlCtx.tLogParams.wDnldFailCnt) > 0)
            {
                NXPLOG_FWDNLD_D("phNxpNciHal_fw_dnld_write_cb - Resetting DnldFailCnt");
                (gphNxpNciHal_fw_IoctlCtx.tLogParams.wDnldFailCnt) = 0;
            }

            if(FALSE == (gphNxpNciHal_fw_IoctlCtx.tLogParams.bConfig))
            {
                NXPLOG_FWDNLD_D("phNxpNciHal_fw_dnld_write_cb - Setting bConfig for use by NCI mode");
                (gphNxpNciHal_fw_IoctlCtx.tLogParams.bConfig) = TRUE;
            }
        }

        /* Reset the previously set DnldAttemptFailed flag */
        if(TRUE == (gphNxpNciHal_fw_IoctlCtx.bDnldAttemptFailed))
        {
            (gphNxpNciHal_fw_IoctlCtx.bDnldAttemptFailed) = FALSE;
        }
    }
    else
    {
        if(TRUE == (gphNxpNciHal_fw_IoctlCtx.bDnldInitiated))
        {
            (gphNxpNciHal_fw_IoctlCtx.tLogParams.wNumDnldFail) += 1;
            (gphNxpNciHal_fw_IoctlCtx.tLogParams.wDnldFailCnt) += 1;
            (gphNxpNciHal_fw_IoctlCtx.tLogParams.bConfig) = FALSE;
        }
        if(NFCSTATUS_WRITE_FAILED == status)
        {
            (gphNxpNciHal_fw_IoctlCtx.bSkipSeq) = TRUE;
            (gphNxpNciHal_fw_IoctlCtx.bDnldRecovery) = TRUE;
        }
        status = NFCSTATUS_FAILED;

        NXPLOG_FWDNLD_E("phNxpNciHal_fw_dnld_write_cb - Request Failed!!");
    }

    p_cb_data->status = status;
    SEM_POST(p_cb_data);

    return;
}

/*******************************************************************************
**
** Function         phNxpNciHal_fw_dnld_write
**
** Description      Download Write
**
** Returns          NFCSTATUS_SUCCESS if success
**
*******************************************************************************/
static NFCSTATUS phNxpNciHal_fw_dnld_write(void* pContext, NFCSTATUS status,
        void* pInfo)
{
    NFCSTATUS wStatus = NFCSTATUS_SUCCESS;
    phNxpNciHal_Sem_t cb_data;

    if(TRUE == (gphNxpNciHal_fw_IoctlCtx.bRetryDnld))
    {
        (gphNxpNciHal_fw_IoctlCtx.bRetryDnld) = FALSE;
    }

    if((TRUE == (gphNxpNciHal_fw_IoctlCtx.bSkipSeq))
            && (FALSE == (gphNxpNciHal_fw_IoctlCtx.bPrevSessnOpen)))
    {
        return NFCSTATUS_SUCCESS;
    }

    if (phNxpNciHal_init_cb_data(&cb_data, NULL) != NFCSTATUS_SUCCESS)
    {
        NXPLOG_FWDNLD_E("phNxpNciHal_fw_dnld_write cb_data creation failed");
        return NFCSTATUS_FAILED;
    }
    if(FALSE == (gphNxpNciHal_fw_IoctlCtx.bForceDnld))
    {
        NXPLOG_FWDNLD_D("phNxpNciHal_fw_dnld_write - Incrementing NumDnldTrig..");
        (gphNxpNciHal_fw_IoctlCtx.bDnldInitiated) = TRUE;
        (gphNxpNciHal_fw_IoctlCtx.bDnldAttempts)++;
        (gphNxpNciHal_fw_IoctlCtx.tLogParams.wNumDnldTrig) += 1;
    }
    wStatus = phDnldNfc_Write(FALSE, NULL,
            (pphDnldNfc_RspCb_t) &phNxpNciHal_fw_dnld_write_cb,
            (void *) &cb_data);
    if(FALSE == (gphNxpNciHal_fw_IoctlCtx.bForceDnld))
    {
        if (wStatus != NFCSTATUS_PENDING)
        {
            NXPLOG_FWDNLD_E("phNxpNciHal_fw_dnld_write failed");
            status = NFCSTATUS_FAILED;
            (gphNxpNciHal_fw_IoctlCtx.tLogParams.wNumDnldFail) += 1;
            (gphNxpNciHal_fw_IoctlCtx.tLogParams.wDnldFailCnt) += 1;
            (gphNxpNciHal_fw_IoctlCtx.tLogParams.bConfig) = FALSE;
            goto clean_and_return;
        }
    }
    /* Wait for callback response */
    if (SEM_WAIT(cb_data))
    {
        NXPLOG_FWDNLD_E("phNxpNciHal_fw_dnld_write semaphore error");
        wStatus = NFCSTATUS_FAILED;
        goto clean_and_return;
    }

    if (cb_data.status != NFCSTATUS_SUCCESS)
    {
        NXPLOG_FWDNLD_E("phNxpNciHal_fw_dnld_write cb failed");
        wStatus = NFCSTATUS_FAILED;
        goto clean_and_return;
    }

    wStatus = NFCSTATUS_SUCCESS;

clean_and_return:
    phNxpNciHal_cleanup_cb_data(&cb_data);

    return wStatus;
}

/*******************************************************************************
**
** Function         phNxpNciHal_fw_dnld_chk_integrity_cb
**
** Description      Download Check Integrity callback
**
** Returns          None
**
*******************************************************************************/
static void phNxpNciHal_fw_dnld_chk_integrity_cb(void* pContext,
        NFCSTATUS status, void* pInfo)
{
    phNxpNciHal_Sem_t *p_cb_data = (phNxpNciHal_Sem_t*) pContext;
    NFCSTATUS wStatus = status;
    pphDnldNfc_Buff_t pRespBuff;
    //uint8_t bUserDataCrc[4];

    if ((NFCSTATUS_SUCCESS == wStatus) && (NULL != pInfo))
    {
        NXPLOG_FWDNLD_D("phNxpNciHal_fw_dnld_chk_integrity_cb - Request Successful");
        pRespBuff = (pphDnldNfc_Buff_t) pInfo;

        if ((31 == (pRespBuff->wLen)) && (NULL != (pRespBuff->pBuff)))
        {
            NXPLOG_FWDNLD_D("phNxpNciHal_fw_dnld_chk_integrity_cb - Valid Resp Buff!!...\n");
            wStatus = phLibNfc_VerifyCrcStatus(pRespBuff->pBuff[0]);
            /*
            memcpy(bUserDataCrc, &(pRespBuff->pBuff[27]),
                    sizeof(bUserDataCrc));*/
        }
        else
        {
            NXPLOG_FWDNLD_E("phNxpNciHal_fw_dnld_chk_integrity_cb - Resp Buff Invalid...\n");
        }
    }
    else
    {
        wStatus = NFCSTATUS_FAILED;
        NXPLOG_FWDNLD_E("phNxpNciHal_fw_dnld_chk_integrity_cb - Request Failed!!");
    }

    p_cb_data->status = wStatus;

    SEM_POST(p_cb_data);

    return;
}

/*******************************************************************************
**
** Function         phNxpNciHal_fw_dnld_chk_integrity
**
** Description      Download Check Integrity
**
** Returns          NFCSTATUS_SUCCESS if success
**
*******************************************************************************/
static NFCSTATUS phNxpNciHal_fw_dnld_chk_integrity(void* pContext,
        NFCSTATUS status, void* pInfo)
{
    NFCSTATUS wStatus = NFCSTATUS_SUCCESS;
    phNxpNciHal_Sem_t cb_data;
    phDnldNfc_Buff_t tDnldBuff;
    static uint8_t bChkIntgRes[31];

    if(TRUE == gphNxpNciHal_fw_IoctlCtx.bPrevSessnOpen)
    {
        NXPLOG_FWDNLD_D("Previous Upload session is open..Cannot issue ChkIntegrity Cmd!!");
        return NFCSTATUS_SUCCESS;
    }

    if(TRUE == (gphNxpNciHal_fw_IoctlCtx.bSkipSeq))
    {
        return NFCSTATUS_SUCCESS;
    }
    else if(TRUE == gphNxpNciHal_fw_IoctlCtx.bPrevSessnOpen)
    {
        NXPLOG_FWDNLD_E("Previous Upload session is open..Cannot issue ChkIntegrity Cmd!!");
        return NFCSTATUS_SUCCESS;
    }

    tDnldBuff.pBuff = bChkIntgRes;
    tDnldBuff.wLen = sizeof(bChkIntgRes);

    if (phNxpNciHal_init_cb_data(&cb_data, NULL) != NFCSTATUS_SUCCESS)
    {
        NXPLOG_FWDNLD_E("phNxpNciHal_fw_dnld_chk_integrity cb_data creation failed");
        return NFCSTATUS_FAILED;
    }

    wStatus = phDnldNfc_CheckIntegrity((gphNxpNciHal_fw_IoctlCtx.bChipVer),
            &tDnldBuff, &phNxpNciHal_fw_dnld_chk_integrity_cb,
            (void *) &cb_data);
    if (wStatus != NFCSTATUS_PENDING)
    {
        NXPLOG_FWDNLD_E("phNxpNciHal_fw_dnld_chk_integrity failed");
        status = NFCSTATUS_FAILED;
        goto clean_and_return;
    }

    /* Wait for callback response */
    if (SEM_WAIT(cb_data))
    {
        NXPLOG_FWDNLD_E("phNxpNciHal_fw_dnld_chk_integrity semaphore error");
        wStatus = NFCSTATUS_FAILED;
        goto clean_and_return;
    }

    if (cb_data.status != NFCSTATUS_SUCCESS)
    {
        NXPLOG_FWDNLD_E("phNxpNciHal_fw_dnld_chk_integrity cb failed");
        wStatus = NFCSTATUS_FAILED;
        goto clean_and_return;
    }

    wStatus = NFCSTATUS_SUCCESS;

clean_and_return:
    phNxpNciHal_cleanup_cb_data(&cb_data);

    return wStatus;
}

/*******************************************************************************
**
** Function         phNxpNciHal_fw_dnld_recover
**
** Description      Download Recover
**
** Returns          NFCSTATUS_SUCCESS if success
**
*******************************************************************************/
static NFCSTATUS  phNxpNciHal_fw_dnld_recover(void* pContext, NFCSTATUS status,
        void* pInfo)
{
    NFCSTATUS wStatus = NFCSTATUS_SUCCESS;
    phNxpNciHal_Sem_t cb_data;

    UNUSED(pInfo);
    UNUSED(status);

    if(TRUE == (gphNxpNciHal_fw_IoctlCtx.bDnldRecovery))
    {
        if (phNxpNciHal_init_cb_data(&cb_data, NULL) != NFCSTATUS_SUCCESS)
        {
            NXPLOG_FWDNLD_E("phNxpNciHal_fw_dnld_recover cb_data creation failed");
            return NFCSTATUS_FAILED;
        }
        (gphNxpNciHal_fw_IoctlCtx.bDnldAttempts)++;

        /* resetting this flag to avoid cyclic issuance of recovery sequence in case of failure */
        (gphNxpNciHal_fw_IoctlCtx.bDnldRecovery) = FALSE;

        wStatus = phDnldNfc_Write(TRUE,NULL,(pphDnldNfc_RspCb_t)&phNxpNciHal_fw_dnld_recover_cb, (void*) &cb_data);

        if(NFCSTATUS_PENDING != wStatus)
        {
            (gphNxpNciHal_fw_IoctlCtx.bSkipForce) = FALSE;
            (gphNxpNciHal_fw_IoctlCtx.bRetryDnld) = FALSE;
            goto clean_and_return;
        }
        /* Wait for callback response */
        if (SEM_WAIT(cb_data))
        {
            NXPLOG_FWDNLD_E("phNxpNciHal_fw_dnld_recover semaphore error");
            wStatus = NFCSTATUS_FAILED;
            goto clean_and_return;
        }

        if (cb_data.status != NFCSTATUS_SUCCESS)
        {
            NXPLOG_FWDNLD_E("phNxpNciHal_fw_dnld_recover cb failed");
            wStatus = NFCSTATUS_FAILED;
            goto clean_and_return;
        }
        wStatus = NFCSTATUS_SUCCESS;

clean_and_return:
        phNxpNciHal_cleanup_cb_data(&cb_data);
    }

    return wStatus;
}

/*******************************************************************************
**
** Function         phNxpNciHal_fw_dnld_recover_cb
**
** Description      Download Recover callback
**
** Returns          None
**
*******************************************************************************/
static void phNxpNciHal_fw_dnld_recover_cb(void* pContext, NFCSTATUS status,
        void* pInfo)
{
    phNxpNciHal_Sem_t *p_cb_data = (phNxpNciHal_Sem_t*) pContext;
    NFCSTATUS wStatus = status;
    UNUSED(pContext);
    UNUSED(pInfo);

    if(NFCSTATUS_SUCCESS == wStatus)
    {
        if(FALSE == (gphNxpNciHal_fw_IoctlCtx.bSkipForce))
        {
            NXPLOG_FWDNLD_D("phNxpNciHal_fw_dnld_recoverCb - Request Successful");
            (gphNxpNciHal_fw_IoctlCtx.bRetryDnld) = TRUE;
        }
        else
        {
            NXPLOG_FWDNLD_D("phNxpNciHal_fw_dnld_recoverCb - Production key update Request Successful");
            (gphNxpNciHal_fw_IoctlCtx.bSendNciCmd) = TRUE;
        }
    }
    else
    {
        wStatus = NFCSTATUS_FAILED;
        NXPLOG_FWDNLD_D("phNxpNciHal_fw_dnld_recoverCb - Request Failed!!");
    }

    /* resetting this flag to avoid cyclic issuance of recovery sequence in case of failure */
    (gphNxpNciHal_fw_IoctlCtx.bDnldRecovery) = FALSE;

    /* reset previously set SkipForce */
    (gphNxpNciHal_fw_IoctlCtx.bSkipForce) = FALSE;
    p_cb_data->status = wStatus;

    SEM_POST(p_cb_data);

    return;
}

/*******************************************************************************
**
** Function         phNxpNciHal_fw_dnld_send_ncicmd_cb
**
** Description      Download Send NCI Command callback
**
** Returns          None
**
*******************************************************************************/
static  void phNxpNciHal_fw_dnld_send_ncicmd_cb(void* pContext, NFCSTATUS status,
        void* pInfo)
{
    phNxpNciHal_Sem_t *p_cb_data = (phNxpNciHal_Sem_t*) pContext;
    NFCSTATUS wStatus = status;
    pphDnldNfc_Buff_t pRespBuff;
    UNUSED(pContext);

    if(NFCSTATUS_SUCCESS == wStatus)
    {
        NXPLOG_FWDNLD_D("phNxpNciHal_fw_dnld_send_ncicmdCb - Request Successful");
        pRespBuff = (pphDnldNfc_Buff_t)pInfo;

        if((0 != (pRespBuff->wLen)) && (NULL != (pRespBuff->pBuff)))
        {
            if(0 == (pRespBuff->pBuff[3]))
            {
                NXPLOG_FWDNLD_D("Successful Response received for Nci Reset Cmd");
            }
            else
            {
                NXPLOG_FWDNLD_E("Nci Reset Request Failed!!");
            }
        }
        else
        {
            NXPLOG_FWDNLD_E("Invalid Response received for Nci Reset Request!!");
        }
        /* Call Tml Ioctl to enable download mode */
        wStatus = phTmlNfc_IoCtl(phTmlNfc_e_EnableDownloadMode);

        if(NFCSTATUS_SUCCESS == wStatus)
        {
            NXPLOG_FWDNLD_D("Switched Successfully to dnld mode..");
            (gphNxpNciHal_fw_IoctlCtx.bRetryDnld) = TRUE;
        }
        else
        {
            NXPLOG_FWDNLD_E("Switching back to dnld mode Failed!!");
            (gphNxpNciHal_fw_IoctlCtx.bRetryDnld) = FALSE;
            wStatus = NFCSTATUS_FAILED;
        }
    }
    else
    {
        NXPLOG_FWDNLD_E("phNxpNciHal_fw_dnld_send_ncicmdCb - Request Failed!!");
    }

    (gphNxpNciHal_fw_IoctlCtx.bSendNciCmd) = FALSE;
    p_cb_data->status = wStatus;

    SEM_POST(p_cb_data);

    return;
}

/*******************************************************************************
**
** Function         phNxpNciHal_fw_dnld_send_ncicmd
**
** Description      Download Send NCI Command
**
** Returns          NFCSTATUS_SUCCESS if success
**
*******************************************************************************/
static NFCSTATUS phNxpNciHal_fw_dnld_send_ncicmd(void* pContext, NFCSTATUS status,
        void* pInfo)
{
    NFCSTATUS wStatus = NFCSTATUS_SUCCESS;
    static uint8_t bNciCmd[4] = {0x20,0x00,0x01,0x00};  /* Nci Reset Cmd with KeepConfig option */
    static uint8_t bNciResp[6];
    phDnldNfc_Buff_t tsData;
    phDnldNfc_Buff_t trData;
    phNxpNciHal_Sem_t cb_data;

    UNUSED(pInfo);
    UNUSED(status);

    if(FALSE == (gphNxpNciHal_fw_IoctlCtx.bSendNciCmd))
    {
        return NFCSTATUS_SUCCESS;
    }
    else
    {
        /* Call Tml Ioctl to enable/restore normal mode */
        wStatus = phTmlNfc_IoCtl(phTmlNfc_e_EnableNormalMode);

        if(NFCSTATUS_SUCCESS != wStatus)
        {
            NXPLOG_FWDNLD_E("Switching to NormalMode Failed!!");
            (gphNxpNciHal_fw_IoctlCtx.bRetryDnld) = FALSE;
            (gphNxpNciHal_fw_IoctlCtx.bSendNciCmd) = FALSE;
        }
        else
        {
            if (phNxpNciHal_init_cb_data(&cb_data, NULL) != NFCSTATUS_SUCCESS)
            {
                NXPLOG_FWDNLD_E("phNxpNciHal_fw_dnld_send_ncicmd cb_data creation failed");
                return NFCSTATUS_FAILED;
            }
            (tsData.pBuff) = bNciCmd;
            (tsData.wLen) = sizeof(bNciCmd);
            (trData.pBuff) = bNciResp;
            (trData.wLen) = sizeof(bNciResp);

            wStatus = phDnldNfc_RawReq(&tsData,&trData,
                    (pphDnldNfc_RspCb_t)&phNxpNciHal_fw_dnld_send_ncicmd_cb, (void*) &cb_data);
            if(NFCSTATUS_PENDING != wStatus)
            {
                goto clean_and_return;
            }
            /* Wait for callback response */
            if (SEM_WAIT(cb_data))
            {
                NXPLOG_FWDNLD_E("phNxpNciHal_fw_dnld_send_ncicmd semaphore error");
                wStatus = NFCSTATUS_FAILED;
                goto clean_and_return;
            }

            if (cb_data.status != NFCSTATUS_SUCCESS)
            {
                NXPLOG_FWDNLD_E("phNxpNciHal_fw_dnld_send_ncicmd cb failed");
                wStatus = NFCSTATUS_FAILED;
                goto clean_and_return;
            }
            wStatus = NFCSTATUS_SUCCESS;

clean_and_return:
            phNxpNciHal_cleanup_cb_data(&cb_data);
        }
    }

    return wStatus;
}

/*******************************************************************************
**
** Function         phNxpNciHal_fw_dnld_log_cb
**
** Description      Download Log callback
**
** Returns          None
**
*******************************************************************************/
static void phNxpNciHal_fw_dnld_log_cb(void* pContext, NFCSTATUS status,
        void* pInfo)
{
    phNxpNciHal_Sem_t *p_cb_data = (phNxpNciHal_Sem_t*) pContext;
    NFCSTATUS wStatus = status;
    UNUSED(pContext);
    UNUSED(pInfo);

    if(NFCSTATUS_SUCCESS == wStatus)
    {
        NXPLOG_FWDNLD_D("phLibNfc_DnldLogCb - Request Successful");
        (gphNxpNciHal_fw_IoctlCtx.bDnldInitiated) = FALSE;
    }
    else
    {
        wStatus = NFCSTATUS_FAILED;
        NXPLOG_FWDNLD_E("phLibNfc_DnldLogCb - Request Failed!!");
    }
    p_cb_data->status = wStatus;

    SEM_POST(p_cb_data);
    return;
}

/*******************************************************************************
**
** Function         phNxpNciHal_fw_dnld_log
**
** Description      Download Log
**
** Returns          NFCSTATUS_SUCCESS if success
**
*******************************************************************************/
static NFCSTATUS phNxpNciHal_fw_dnld_log(void* pContext, NFCSTATUS status,
        void* pInfo)
{
    NFCSTATUS wStatus = NFCSTATUS_SUCCESS;
    phNxpNciHal_Sem_t cb_data;
    phDnldNfc_Buff_t tData;

    UNUSED(pInfo);
    UNUSED(status);

    if(((TRUE == (gphNxpNciHal_fw_IoctlCtx.bSkipSeq)) ||
                (TRUE == (gphNxpNciHal_fw_IoctlCtx.bForceDnld))) &&
            (FALSE == (gphNxpNciHal_fw_IoctlCtx.bDnldInitiated)))
    {
        return NFCSTATUS_SUCCESS;
    }
    else
    {
        if (phNxpNciHal_init_cb_data(&cb_data, NULL) != NFCSTATUS_SUCCESS)
        {
            NXPLOG_FWDNLD_E("phNxpNciHal_fw_dnld_log cb_data creation failed");
            return NFCSTATUS_FAILED;
        }
        (tData.pBuff) = (void *)&(gphNxpNciHal_fw_IoctlCtx.tLogParams);
        (tData.wLen) = sizeof(gphNxpNciHal_fw_IoctlCtx.tLogParams);

        wStatus = phDnldNfc_Log(&tData,(pphDnldNfc_RspCb_t)&phNxpNciHal_fw_dnld_log_cb, (void*) &cb_data);

        if (wStatus != NFCSTATUS_PENDING)
        {
            NXPLOG_FWDNLD_E("phDnldNfc_Log failed");
            (gphNxpNciHal_fw_IoctlCtx.bDnldInitiated) = FALSE;
            wStatus = NFCSTATUS_FAILED;
            goto clean_and_return;
        }
        /* Wait for callback response */
        if (SEM_WAIT(cb_data))
        {
            NXPLOG_FWDNLD_E("phNxpNciHal_fw_dnld_log semaphore error");
            wStatus = NFCSTATUS_FAILED;
            goto clean_and_return;
        }

        if (cb_data.status != NFCSTATUS_SUCCESS)
        {
            NXPLOG_FWDNLD_E("phNxpNciHal_fw_dnld_log_cb failed");
            wStatus = NFCSTATUS_FAILED;
            goto clean_and_return;
        }

        wStatus = NFCSTATUS_SUCCESS;

clean_and_return:
        phNxpNciHal_cleanup_cb_data(&cb_data);

        return wStatus;
    }

}

/*******************************************************************************
**
** Function         phNxpNciHal_fw_seq_handler
**
** Description      Sequence Handler
**
** Returns          NFCSTATUS_SUCCESS if sequence completed uninterrupted
**
*******************************************************************************/
static NFCSTATUS phNxpNciHal_fw_seq_handler(NFCSTATUS (*seq_handler[])(void* pContext, NFCSTATUS status, void* pInfo))
{
    char *pContext = "FW-Download";
    int16_t seq_counter = 0;
    phDnldNfc_Buff_t pInfo;
    NFCSTATUS status = NFCSTATUS_FAILED;

    status = phTmlNfc_ReadAbort();
    if(NFCSTATUS_SUCCESS != status)
    {
      NXPLOG_FWDNLD_E("Tml Read Abort failed!!");
      return status;
    }

    while(seq_handler[seq_counter] != NULL )
    {
        status = NFCSTATUS_FAILED;
        status = (seq_handler[seq_counter])(pContext, status, &pInfo );
        if(NFCSTATUS_SUCCESS != status)
        {
            NXPLOG_FWDNLD_E(" phNxpNciHal_fw_seq_handler : FAILED");
            break;
        }
        seq_counter++;
    }
    return status;
}

/*******************************************************************************
**
** Function         phNxpNciHal_fw_dnld_complete
**
** Description      Download Sequence Complete
**
** Returns          NFCSTATUS_SUCCESS if success
**
*******************************************************************************/
static  NFCSTATUS phNxpNciHal_fw_dnld_complete(void* pContext,NFCSTATUS status,
        void* pInfo)
{
    NFCSTATUS wStatus = NFCSTATUS_SUCCESS;
    NFCSTATUS fStatus = status;
    UNUSED(pInfo);
    UNUSED(pContext);

    if(NFCSTATUS_WRITE_FAILED == status)
    {
        if((gphNxpNciHal_fw_IoctlCtx.bDnldAttempts) < PHLIBNFC_IOCTL_DNLD_MAX_ATTEMPTS)
        {
            (gphNxpNciHal_fw_IoctlCtx.bDnldRecovery) = TRUE;
        }
        else
        {
            NXPLOG_FWDNLD_E("Max Dnld Retry Counts Exceeded!!");
            (gphNxpNciHal_fw_IoctlCtx.bDnldRecovery) = FALSE;
            (gphNxpNciHal_fw_IoctlCtx.bRetryDnld) = FALSE;
        }
    }
    else if(NFCSTATUS_REJECTED == status)
    {
        if((gphNxpNciHal_fw_IoctlCtx.bDnldAttempts) < PHLIBNFC_IOCTL_DNLD_MAX_ATTEMPTS)
        {
            (gphNxpNciHal_fw_IoctlCtx.bDnldRecovery) = TRUE;

            /* in case of signature error we need to try recover sequence directly bypassing the force cmd */
            (gphNxpNciHal_fw_IoctlCtx.bSkipForce) = TRUE;
        }
        else
        {
            NXPLOG_FWDNLD_E("Max Dnld Retry Counts Exceeded!!");
            (gphNxpNciHal_fw_IoctlCtx.bDnldRecovery) = FALSE;
            (gphNxpNciHal_fw_IoctlCtx.bRetryDnld) = FALSE;
        }
    }

    if(TRUE == (gphNxpNciHal_fw_IoctlCtx.bDnldInitiated))
    {
        (gphNxpNciHal_fw_IoctlCtx.bLastStatus) = status;
        (gphNxpNciHal_fw_IoctlCtx.bDnldAttemptFailed) = TRUE;

        NXPLOG_FWDNLD_E("Invoking Pending Download Log Sequence..");
        (gphNxpNciHal_fw_IoctlCtx.bDnldInitiated) = FALSE;
        /* Perform the Logging sequence */
        wStatus = phNxpNciHal_fw_seq_handler(phNxpNciHal_dwnld_log_seqhandler);
        status = phNxpNciHal_fw_dnld_complete(pContext, wStatus, &pInfo);
        if (NFCSTATUS_SUCCESS == status)
        {
            NXPLOG_FWDNLD_D(" phNxpNciHal_fw_dnld_complete : SUCCESS");
        }
        else
        {
            NXPLOG_FWDNLD_E(" phNxpNciHal_fw_dnld_complete : FAILED");
        }

    }
    else if(TRUE == (gphNxpNciHal_fw_IoctlCtx.bDnldRecovery))
    {
        NXPLOG_FWDNLD_E("Invoking Download Recovery Sequence..");

        if(NFCSTATUS_SUCCESS == wStatus)
        {
            /* Perform the download Recovery sequence */
            wStatus = phNxpNciHal_fw_seq_handler(phNxpNciHal_dwnld_rec_seqhandler);

            status = phNxpNciHal_fw_dnld_complete(pContext, wStatus, &pInfo);
            if (NFCSTATUS_SUCCESS == status)
            {
                NXPLOG_FWDNLD_D(" phNxpNciHal_fw_dnld_complete : SUCCESS");
            }
            else
            {
                NXPLOG_FWDNLD_E(" phNxpNciHal_fw_dnld_complete : FAILED");
            }
        }
    }
    else if(TRUE == (gphNxpNciHal_fw_IoctlCtx.bRetryDnld))
    {
        (gphNxpNciHal_fw_IoctlCtx.bPrevSessnOpen) = FALSE;
        (gphNxpNciHal_fw_IoctlCtx.bDnldInitiated) = FALSE;
        (gphNxpNciHal_fw_IoctlCtx.bForceDnld) = FALSE;
        (gphNxpNciHal_fw_IoctlCtx.bSkipSeq) = FALSE;
        (gphNxpNciHal_fw_IoctlCtx.bSkipForce) = FALSE;
        (gphNxpNciHal_fw_IoctlCtx.bDnldRecovery) = FALSE;
        (gphNxpNciHal_fw_IoctlCtx.bSendNciCmd) = FALSE;

        /* Perform the download sequence ... after successful recover attempt */
        wStatus = phNxpNciHal_fw_seq_handler(phNxpNciHal_dwnld_seqhandler);

        status = phNxpNciHal_fw_dnld_complete(pContext, wStatus, &pInfo);
        if (NFCSTATUS_SUCCESS == status)
        {
            NXPLOG_FWDNLD_D(" phNxpNciHal_fw_dnld_complete : SUCCESS");
        }
        else
        {
            NXPLOG_FWDNLD_E(" phNxpNciHal_fw_dnld_complete : FAILED");
        }
    }
    else
    {
        if(FALSE == (gphNxpNciHal_fw_IoctlCtx.bSkipSeq))
        {
            if(NFCSTATUS_SUCCESS == status)
            {
                if(NFC_FW_DOWNLOAD == gphNxpNciHal_fw_IoctlCtx.IoctlCode)
                {
                    NXPLOG_FWDNLD_E("Fw Download success.. ");
                }
                else if(PHLIBNFC_DNLD_MEM_READ == gphNxpNciHal_fw_IoctlCtx.IoctlCode)
                {
                    NXPLOG_FWDNLD_E("Read Request success.. ");
                }
                else if(PHLIBNFC_DNLD_MEM_WRITE == gphNxpNciHal_fw_IoctlCtx.IoctlCode)
                {
                    NXPLOG_FWDNLD_E("Write Request success.. ");
                }
                else if(PHLIBNFC_DNLD_READ_LOG == gphNxpNciHal_fw_IoctlCtx.IoctlCode)
                {
                    NXPLOG_FWDNLD_E("ReadLog Request success.. ");
                }
                else
                {
                    NXPLOG_FWDNLD_E("Invalid Request!!");
                }
            }
            else
            {
                if(NFC_FW_DOWNLOAD == gphNxpNciHal_fw_IoctlCtx.IoctlCode)
                {
                    NXPLOG_FWDNLD_E("Fw Download Failed!!");
                }
                else if(NFC_MEM_READ == gphNxpNciHal_fw_IoctlCtx.IoctlCode)
                {
                    NXPLOG_FWDNLD_E("Read Request Failed!!");
                }
                else if(NFC_MEM_WRITE == gphNxpNciHal_fw_IoctlCtx.IoctlCode)
                {
                    NXPLOG_FWDNLD_E("Write Request Failed!!");
                }
                else if(PHLIBNFC_DNLD_READ_LOG == gphNxpNciHal_fw_IoctlCtx.IoctlCode)
                {
                    NXPLOG_FWDNLD_E("ReadLog Request Failed!!");
                }
                else
                {
                    NXPLOG_FWDNLD_E("Invalid Request!!");
                }
            }
        }

        if(FALSE == gphNxpNciHal_fw_IoctlCtx.bSendNciCmd)
        {
            /* Call Tml Ioctl to enable/restore normal mode */
            wStatus = phTmlNfc_IoCtl(phTmlNfc_e_EnableNormalMode);

            if(NFCSTATUS_SUCCESS != wStatus)
            {
                NXPLOG_FWDNLD_E("Switching to NormalMode Failed!!");
            }
            else
            {
                wStatus = fStatus;
            }
        }

        (gphNxpNciHal_fw_IoctlCtx.bPrevSessnOpen) = FALSE;
        (gphNxpNciHal_fw_IoctlCtx.bDnldInitiated) = FALSE;
        (gphNxpNciHal_fw_IoctlCtx.bChipVer) = 0;
        (gphNxpNciHal_fw_IoctlCtx.bSkipSeq) = FALSE;
        (gphNxpNciHal_fw_IoctlCtx.bForceDnld) = FALSE;
        (gphNxpNciHal_fw_IoctlCtx.bDnldRecovery) = FALSE;
        (gphNxpNciHal_fw_IoctlCtx.bRetryDnld) = FALSE;
        (gphNxpNciHal_fw_IoctlCtx.bSkipReset) = FALSE;
        (gphNxpNciHal_fw_IoctlCtx.bSkipForce) = FALSE;
        (gphNxpNciHal_fw_IoctlCtx.bSendNciCmd) = FALSE;
        (gphNxpNciHal_fw_IoctlCtx.bDnldAttempts) = 0;

        if(FALSE == gphNxpNciHal_fw_IoctlCtx.bDnldAttemptFailed)
        {
        }
        else
        {
            NXPLOG_FWDNLD_E("Returning Download Failed Status to Caller!!");

            (gphNxpNciHal_fw_IoctlCtx.bLastStatus) = NFCSTATUS_SUCCESS;
            (gphNxpNciHal_fw_IoctlCtx.bDnldAttemptFailed) = FALSE;
        }
        phDnldNfc_CloseFwLibHandle();
    }

    return wStatus;
}

/*******************************************************************************
**
** Function         phNxpNciHal_fw_download_seq
**
** Description      Download Sequence
**
** Returns          NFCSTATUS_SUCCESS if success
**
*******************************************************************************/
NFCSTATUS phNxpNciHal_fw_download_seq(uint8_t bClkSrcVal, uint8_t bClkFreqVal)
{
    NFCSTATUS status = NFCSTATUS_FAILED;
    phDnldNfc_Buff_t pInfo;
    char *pContext = "FW-Download";

    /* reset the global flags */
    gphNxpNciHal_fw_IoctlCtx.IoctlCode = NFC_FW_DOWNLOAD;
    (gphNxpNciHal_fw_IoctlCtx.bPrevSessnOpen) = FALSE;
    (gphNxpNciHal_fw_IoctlCtx.bDnldInitiated) = FALSE;
    (gphNxpNciHal_fw_IoctlCtx.bChipVer) = 0;
    (gphNxpNciHal_fw_IoctlCtx.bSkipSeq) = FALSE;
    (gphNxpNciHal_fw_IoctlCtx.bForceDnld) = FALSE;
    (gphNxpNciHal_fw_IoctlCtx.bDnldRecovery) = FALSE;
    (gphNxpNciHal_fw_IoctlCtx.bRetryDnld) = FALSE;
    (gphNxpNciHal_fw_IoctlCtx.bSkipReset) = FALSE;
    (gphNxpNciHal_fw_IoctlCtx.bSkipForce) = FALSE;
    (gphNxpNciHal_fw_IoctlCtx.bSendNciCmd) = FALSE;
    (gphNxpNciHal_fw_IoctlCtx.bDnldAttempts) = 0;
    (gphNxpNciHal_fw_IoctlCtx.bClkSrcVal) = bClkSrcVal;
    (gphNxpNciHal_fw_IoctlCtx.bClkFreqVal) = bClkFreqVal;
    /* Get firmware version */
    if (NFCSTATUS_SUCCESS == phDnldNfc_InitImgInfo())
    {
        NXPLOG_FWDNLD_D("phDnldNfc_InitImgInfo:SUCCESS");
        status = phNxpNciHal_fw_seq_handler(phNxpNciHal_dwnld_seqhandler);
    }
    else
    {
        NXPLOG_FWDNLD_E("phDnldNfc_InitImgInfo: FAILED");
    }

    /* Chage to normal mode */
    status = phNxpNciHal_fw_dnld_complete(pContext, status, &pInfo);
    if (NFCSTATUS_SUCCESS == status)
    {
        NXPLOG_FWDNLD_D(" phNxpNciHal_fw_dnld_complete : SUCCESS");
    }
    else
    {
        NXPLOG_FWDNLD_E(" phNxpNciHal_fw_dnld_complete : FAILED");
    }

    return status;
}

static
NFCSTATUS
phLibNfc_VerifyCrcStatus(uint8_t bCrcStatus)
{
    uint8_t bBitPos = 0;
    uint8_t bShiftVal = 1;
    NFCSTATUS wStatus = NFCSTATUS_SUCCESS;
    while(bBitPos < 7)
    {
        if(!(bCrcStatus & bShiftVal))
        {
            switch(bBitPos)
            {
                case 0:
                {
                    NXPLOG_FWDNLD_E("User Data Crc is NOT OK!!");
                    wStatus = NFCSTATUS_FAILED;
                    break;
                }
                case 1:
                {
                    NXPLOG_FWDNLD_E("Trim Data Crc is NOT OK!!");
                    wStatus = NFCSTATUS_FAILED;
                    break;
                }
                case 2:
                {
                    NXPLOG_FWDNLD_E("Protected Data Crc is NOT OK!!");
                    wStatus = NFCSTATUS_FAILED;
                    break;
                }
                case 3:
                {
                    NXPLOG_FWDNLD_E("Patch Code Crc is NOT OK!!");
                    wStatus = NFCSTATUS_FAILED;
                    break;
                }
                case 4:
                {
                    NXPLOG_FWDNLD_E("Function Code Crc is NOT OK!!");
                    wStatus = NFCSTATUS_FAILED;
                    break;
                }
                case 5:
                {
                    NXPLOG_FWDNLD_E("Patch Table Crc is NOT OK!!");
                    wStatus = NFCSTATUS_FAILED;
                    break;
                }
                case 6:
                {
                    NXPLOG_FWDNLD_E("Function Table Crc is NOT OK!!");
                    wStatus = NFCSTATUS_FAILED;
                    break;
                }
                default:
                {
                    break;
                }
            }
        }

        bShiftVal <<= 1;
        ++bBitPos;
    }

    return wStatus;
}
