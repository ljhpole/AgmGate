/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   shAgm.h
 * Author: ljhpole
 *
 * Created on March 28, 2018, 3:33 PM
 */

#ifndef _SHAGM_LINUX_H
#define _SHAGM_LINUX_H


#include <chrono>
#include <mutex>
#include <condition_variable>
#include <atomic>

#include "../ModuleSh/include/GlobalStruct_Sh.h"
#include "../ModuleSh/include/GlobalDefine.h"
#include "../ModuleSh/common/include/Iniproxy_pub.h"
#include "../ModuleSh/common/include/trace_pub.h"
#include "../ModuleSh/common/include/Utils_pub.h"
#include "../ModuleSh/BusinessProcess/libParamModule/Parameter_pub.h"
#include "../ModuleSh/libSysDevice/device_include/Reader_pub.h"
#include "../ModuleSh/libSysDevice/device_include/mcpmodule_pub.h"
#include "../ModuleSh/BusinessProcess/businessinclude/Analysis_pub.h"
#include "../ModuleSh/common/include/DBManagerProxy_pub.h"
#include "../ModuleSh/include/GlobalDataManager_pub.h"
#include "../ModuleSh/BusinessProcess/businessinclude/MsgCom_pub.h"
#include "../ModuleSh/BusinessProcess/businessinclude/event.h"
#include "../ModuleSh/libSysDevice/device_include/mcpmodule_pub.h"
#include "../ModuleSh/libSysDevice/libPassage/include/GuFlap_pub.h"
#include "../ModuleSh/libSysDevice/device_include/TpuModule_pub.h"
#include "../ModuleSh/libSysDevice/libIoModule/IoModuleCtrlUnit_pub.h"


#include "uidisplay.h"

#include "GateContext.h"
#include "DevStatusMonitor.h"
// ״̬����ĸￄ1�7ָ������ģʽ����5λָ������״̬��


#define   SH_GATE_DEBUG_FLAG      1
#define WM_SYSCMD_INFORM_MAIN_PROCESS    201

#define LOCKFILE        "inesanet.agm"
#define   SH_GATE_SNAP_VERSION      1



// 
//���潻���õ��ĺ궨��
#define TYPE_CSC_OTC_BLOCK_1            0
#define TYPE_CSC_OTC_BLOCK_2        1
#define TYPE_CSC_CPU_BLOCK_1        2
#define TYPE_CSC_CPU_BLOCK_2        3
#define TYPE_CSC_MP_OTC_EXIT        4
#define TYPE_CSC_MP_CPU_EXIT        5
#define TYPE_CSC_MP_OTC_LOCKCARD    6
#define TYPE_CSC_MP_CPU_LOCKCARD    7
#define TYPE_CSC_MP_OTC_ENTRY       8
#define TYPE_CSC_MP_CPU_ENTRY       9
#define TYPE_CSC_OTT_BLOCK          10
#define TYPE_CSC_MP_OTT_LOCKCARD    11
#define TYPE_CSC_MP_OTT_EXIT        12
#define TYPE_CSC_MP_OTT_ENTRY       13
#define TYPE_CSC_MP_OTT_ENTRY2      14
#define TYPE_TPU_MP_EXIT            20
//
#define FPRINTLN(fd, fmt, ...)			fprintf((fd), fmt "\n", ##__VA_ARGS__)
#define PRINTLN(fmt, ...)				FPRINTLN(stdout, fmt, ##__VA_ARGS__)

#if defined(DEBUG) && defined(DEBUG_TRACE)
	#define TRACE(fmt, ...)				PRINTLN("> TRC (0x%8X, %d) " fmt, SELF_THREAD_ID, SELF_NATIVE_THREAD_ID, ##__VA_ARGS__)
#else
	#define TRACE(fmt, ...)
#endif
//
enum  eGateShReaderType{eEntryCscReader=0x20,eExitCscReader=0x40,eExitTpuReader=0x60};

int InitShParameter();
int InitSNAPMATripod();


void ReaderAbort(tagResPonseInfoData * pResInfoData);
void ReaderReadTicket(tagResPonseInfoData * pResInfoData,BYTE bytMode);
void TicketCardTransCmd(tagResPonseInfoData * pResInfoData,tagUpdateCmdInfo InUpdateInfo,int &iRtn);
int  GetUpdateSeqSn(DWORD &dwSeqSn,BYTE bytInOutType,eDevSeqSnType_t eSnType);


enum eAnalysisPreType{eNone,eStartAnalysis = 0x1,eTransInOneMinute,eGenTransRec,eGetProofTac,eExitTransCmd,eEntryTransCmd,eSameCardInfo };
class tagCardOtcTransDataBak
{
    
public:
    BYTE       m_bytReadInfoType;
    uint16_t   m_wFare;    // �ۿ����,�������ȡ
    BYTE  m_bytTransRespCode;    // ����ָ��Ӧ���룬����ָ���ȡ
    uint32_t   m_dwTransTimePre;  // ����ʱ�䱸��
    uint32_t   m_dwTransSn;    // ������ˮ��
    uint8_t    m_bytCurrentTicketBox;   // Ʊ��λ��
    union 
    {
        CSC_MP_READ_CARDINFO  m_OtcReadInfo;
        CSC_MP_OTT_READCARD   m_OttReadInfo;
        CSC_MP_PBOC_READCARD  m_PbocReadInfo;
        CSC_MP_PBOC_EMPLOYEE_READCARD  m_PbocEmployeeReadInfo;
        CSC_MP_QR_READCARD    m_QrReadInfo;
    }uCardInfoBak; 
public:
    tagCardOtcTransDataBak(){
        m_bytReadInfoType = m_wFare = m_bytTransRespCode = 0;
    };
    ~tagCardOtcTransDataBak(){
    };
    void Reset()
    {
        m_bytReadInfoType = m_wFare = m_bytTransRespCode = 0; 
        memset(&uCardInfoBak.m_OtcReadInfo,0,sizeof(CSC_MP_READ_CARDINFO));
    };
    
    eAnalysisPreType IsAnalysisSameCard(tagResPonseInfoData *ptrReaderInfo)
    {
	eAnalysisPreType  eRtn =  eStartAnalysis;
        uint32_t   dwCurrTime = time(NULL)+8*60*60;

	if(m_bytReadInfoType != ptrReaderInfo->bytRecardInfoCmdCode)
	{
            Reset();
            m_bytReadInfoType = ptrReaderInfo->bytRecardInfoCmdCode;
            return eStartAnalysis;
	}
	switch(ptrReaderInfo->bytRecardInfoCmdCode)
	{
	case eCSC_OTC_READCARD:
		{			
                    if(!memcmp(&uCardInfoBak.m_OtcReadInfo.strOtcBlock1.strM1Block.dwCardUniqueCode,&ptrReaderInfo->tagReaderOtcResp.strOtcBlock1.strM1Block.dwCardUniqueCode, \
                            sizeof(ptrReaderInfo->tagReaderOtcResp.strOtcBlock1.strM1Block.dwCardUniqueCode)) )  // same card No
                    {
                        if(( (ptrReaderInfo->bytWorkMode == 0x20) && (0 != m_bytTransRespCode) && (ptrReaderInfo->tagReaderOtcResp.strEntryRec.bsEntryOrExitFlag != 0x2)) \
                             ||   ( (ptrReaderInfo->bytWorkMode == 0x40) && (0 != m_bytTransRespCode) && (ptrReaderInfo->tagReaderOtcResp.strEntryRec.bsEntryOrExitFlag != 0x1) \
                                         && ( uCardInfoBak.m_OtcReadInfo.GetCardBalance()-m_wFare == ptrReaderInfo->tagReaderOtcResp.GetCardBalance() )
                                  )
                                )  // ����վ���ϱʽ��״���Ӧ���롢�ұ�־�Ѿ��޸ġ� ���� ����վ���ϱʽ��״���Ӧ���롢Ǯ���ۡ���־�޸ġ� ִ����ȡTAC
                        {
                            eRtn = eGetProofTac;
                        }
                        else if(( (ptrReaderInfo->bytWorkMode == 0x40) && (0 != m_bytTransRespCode) && (ptrReaderInfo->tagReaderOtcResp.strEntryRec.bsEntryOrExitFlag != 0x1))
                                )  // ����վ���ϱʽ��״���Ӧ���롢Ǯû�пۡ���־�޸ġ� ִ�г�վָ��
                        {
                            eRtn = eExitTransCmd;
                        }
                        else if( dwCurrTime - m_dwTransTimePre < 5 )    // Trans In One Minute
                        {
                            eRtn = eTransInOneMinute;
                        }
                        else
                        {
                            //eRtn = eSameCardInfo;
                        }
                    }
                    else
                    {
                        //eRtn = eSameCardInfo;
                    }
		}
		break;
	case eCSC_OTT_READCARD:
		{
                    if(!memcmp(uCardInfoBak.m_OttReadInfo.struDataBlk.bytCardUniqueCode,ptrReaderInfo->tagReaderReadOttResp.struDataBlk.bytCardUniqueCode, \
                            sizeof(ptrReaderInfo->tagReaderReadOttResp.struDataBlk.bytCardUniqueCode)))  // same ott card No
                    {
                        if( (ptrReaderInfo->bytWorkMode == 0x20) && (0 != m_bytTransRespCode) && (memcmp(ptrReaderInfo->tagReaderReadOttResp.struDataBlk.bytTransTime,&m_dwTransTimePre,sizeof(m_dwTransTimePre))) \
                           )  // ����վ���ϱʽ��״���Ӧ���롢����ʱ�����ٴζ�������Ϣ�Ľ���ʱ����ͬ��
                        {
                            eRtn = eEntryTransCmd;
                        }
                        else if(( (ptrReaderInfo->bytWorkMode == 0x40) && (0 != m_bytTransRespCode))
                                )  // ����վ���ϱʽ��״���Ӧ���롢Ǯû�пۡ���־�޸ġ� ִ�г�վָ��
                        {
                            eRtn = eExitTransCmd;
                        }
                        else if( dwCurrTime - m_dwTransTimePre < 5 )    // Trans In One Minute
                        {
                            eRtn = eTransInOneMinute;
                        }
                        else
                        {
                            //eRtn = eSameCardInfo;
                        }
                    }
                    else
                    {
                        //eRtn = eSameCardInfo;
                    }
		}
		break;
	case eCSC_MTC_READCARD:
		{
                    
		}
		break;
	case eCSC_PBOC_READCARD:
		{
                    if(!memcmp(uCardInfoBak.m_PbocReadInfo.strPbocBlock1.bytMainAppCode,ptrReaderInfo->tagReaderPbocInfoResp.strPbocBlock1.bytMainAppCode, \
                            sizeof(ptrReaderInfo->tagReaderPbocInfoResp.strPbocBlock1.bytMainAppCode)))
                    {
                    }
                    else
                    {
                        //eRtn = eSameCardInfo;
                    }
		}
		break;
	case eCSC_EMPLOY_READCARD:
		{
                    if(!memcmp(uCardInfoBak.m_PbocEmployeeReadInfo.strPbocEmpoyeeBlock1.bytMainAppCode,ptrReaderInfo->tagReaderPbocEmployeeInfoResp.strPbocEmpoyeeBlock1.bytMainAppCode, \
                            sizeof(ptrReaderInfo->tagReaderPbocEmployeeInfoResp.strPbocEmpoyeeBlock1.bytMainAppCode)))
                    {
                        
                    }
                    else
                    {
                        //eRtn = eSameCardInfo;
                    }
		}
		break;
        case eCSC_OTQ_READCARD:
                {
                    if(!memcmp(uCardInfoBak.m_QrReadInfo.struDataBlk.bytUserToken,ptrReaderInfo->tagReaderReadOtqResp.struDataBlk.bytUserToken, \
                            sizeof(ptrReaderInfo->tagReaderReadOtqResp.struDataBlk.bytUserToken)))
                    {
                    }
                    else
                    {
                          //eRtn = eSameCardInfo;
                    }
                }
                break;
	}
	return eRtn;        
    }
};


extern IGlobalDataManager * g_pMainGDM;
extern PARAMCONFIG * g_paramPtr;
extern TRAC_O_CLASSE  g_traceInfo;
extern LocalSLEInfo g_LocalSleInfo;
extern tagResPonseInfoData  g_ReaderResInfo;
extern tagCardOtcTransDataBak  g_CardDataBak;
extern CardAnalysisInfo_t     g_CardAnalysisInfo;
extern tagUpdateCmdInfo       g_CardTransCmdInfo;
extern ReaderMessageData_t    g_ReaderCmdData;
extern IN_OUT_UPDATE_SEQ_SN g_InOutUpdateSeqSn;
extern UiDisplay            g_UiDisplay;
extern DisplayInfo          g_UiDisplayInfo;
extern string               g_DisplayMode;

extern tagResPonseInfoData  g_ExitReaderResInfo;
extern tagCardOtcTransDataBak  g_ExitCardDataBak;
extern CardAnalysisInfo_t     g_ExitCardAnalysisInfo;
extern tagUpdateCmdInfo       g_ExitCardTransCmdInfo;


extern tagResPonseInfoData  g_TpuExitReaderResInfo;
extern tagCardOtcTransDataBak  g_TpuExitCardDataBak;
extern CardAnalysisInfo_t     g_TpuExitCardAnalysisInfo;
extern tagUpdateCmdInfo       g_TpuExitCardTransCmdInfo;


extern IN_OUT_MSG_TXN_REC   g_InOutMsgTxnRec;

extern  UiDisplay            g_ExitUiDisplay;
extern DisplayInfo          g_ExitUiDisplayInfo;
extern string               g_ExitDisplayMode;

extern CDevStatusMonitor  g_DevStausMonitor;
#endif /* SHAGM_H */

