/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "shAgm.h"
#include <pthread.h>
#include <string>
#include "transprocess.h"
#include "uidisplay.h"
#include "DevStatusMonitor.h"
#include "GateContext.h"
using namespace std;

void ReaderAbort(tagResPonseInfoData * pResInfoData)
{
    
	g_ReaderCmdData.Reset();
	g_ReaderCmdData.eCommandCode = eCSC_OTC_ABORT;
	g_ReaderCmdData.RequestDataBody_u.databuff[0] = 0;
        switch(pResInfoData->bytWorkMode)
        {
            case 0x20:  // 进站读写器
            {
                g_pMainGDM->GetReader()->SendReaderCmd(&g_ReaderCmdData,1);
            }
            break;
            case 0x40:  // 出站读写器
            {
                g_pMainGDM->GetExitReader()->SendReaderCmd(&g_ReaderCmdData,1);
            }
            break;
            case 0x60:  // Collect Reader
            {
                g_pMainGDM->GetExitCollectReader()->SendReaderCmd(&g_ReaderCmdData,1);
            }
            break;
        }
        return;
	
}
void ReaderReadTicket(tagResPonseInfoData * pResInfoData,BYTE bytMode)
{
	g_ReaderCmdData.Reset();
                
	g_ReaderCmdData.eCommandCode = eCSC_OTC_READCARD;
	g_ReaderCmdData.RequestDataBody_u.tagReaderReadcardReq.bytReadMode = bytMode;
        switch(pResInfoData->bytWorkMode)
        {
            case 0x20:  // 进站读写器
            {
                g_pMainGDM->GetReader()->SendReaderCmd(&g_ReaderCmdData);
            }
            break;
            case 0x40:  // 出站读写器
            {
                g_pMainGDM->GetExitReader()->SendReaderCmd(&g_ReaderCmdData);
            }
            break;
            case 0x60:  // Collect Reader
            {
                g_pMainGDM->GetExitCollectReader()->SendReaderCmd(&g_ReaderCmdData);
            }
            break;
        }
        return;    
}
int  GetUpdateSeqSn(DWORD &dwSeqSn,BYTE bytInOutType,eDevSeqSnType_t eSnType)
{
	int  iRtn = 0;
	switch(bytInOutType)
	{
	case  0x1:   // 获取
		{
			g_InOutUpdateSeqSn.eInOutUpdateFlag = eOutFlag;
		}
		break;
	case 0x2:   // 更新会话流水号
		{
			g_InOutUpdateSeqSn.eInOutUpdateFlag = eUpdateFlag;
			switch(eSnType)
			{
			case eOttSeqSn:
				{
					g_InOutUpdateSeqSn.lOttSeqSn = dwSeqSn;
				}
				break;
			case eOtcSeqSn:
				{
					g_InOutUpdateSeqSn.lOtcSeqSn = dwSeqSn;
				}
				break;
			case eMtcSeqSn:
				{
					g_InOutUpdateSeqSn.lMtcSeqSn = dwSeqSn;
				}
				break;
			case ePbocSeqSn:
				{
					g_InOutUpdateSeqSn.lPbocSeqSn = dwSeqSn;
				}
				break;
			case ePbocEmployeeSeqSn:
				{
					g_InOutUpdateSeqSn.lPbocEmployeeSeqSn = dwSeqSn;
				}
				break;
			case eOtcCargeSeqSn:
				{
					g_InOutUpdateSeqSn.lOtcChargeSeqSn = dwSeqSn;
				}
				break;
			case eSessionSeqSn:
				{
					g_InOutUpdateSeqSn.lSessionSeqSn = dwSeqSn;
				}
				break;
			}
		}
		break;
	}

	g_InOutUpdateSeqSn.eSeqSnType = eSnType;

	iRtn = g_pMainGDM->GetSQLITEDBManagerProxy()->DBInOutModifySeqSn(&g_InOutUpdateSeqSn);

	if(0 == iRtn)
	{
		if(0x02 == bytInOutType )
		{
			g_pMainGDM->GetUtils()->DebugMsg(&g_traceInfo,"CBOMMainDlg::GetUpdateSeqSn 更新流水号OK!","SeqSn = %d,Action:%d,Type=%d",dwSeqSn,g_InOutUpdateSeqSn.eInOutUpdateFlag,bytInOutType);
			return iRtn;
		}
		switch(eSnType)
		{
		case eOttSeqSn:
			{
				dwSeqSn = g_InOutUpdateSeqSn.lOttSeqSn;
			}
			break;
		case eOtcSeqSn:
			{
				dwSeqSn = g_InOutUpdateSeqSn.lOtcSeqSn;
			}
			break;
		case eMtcSeqSn:
			{
				dwSeqSn = g_InOutUpdateSeqSn.lMtcSeqSn;
			}
			break;
		case ePbocSeqSn:
			{
				dwSeqSn = g_InOutUpdateSeqSn.lPbocSeqSn;
			}
			break;
		case ePbocEmployeeSeqSn:
			{
				dwSeqSn = g_InOutUpdateSeqSn.lPbocEmployeeSeqSn;
			}
			break;
		case eOtcCargeSeqSn:
			{
				dwSeqSn = g_InOutUpdateSeqSn.lOtcChargeSeqSn;
			}
			break;
		}
		g_pMainGDM->GetUtils()->DebugMsg(&g_traceInfo,"CBOMMainDlg::GetUpdateSeqSn OK!","SeqSn = %d,Action:%d,Type=%d",dwSeqSn,g_InOutUpdateSeqSn.eInOutUpdateFlag,bytInOutType);

	}
	else
	{
		g_pMainGDM->GetUtils()->DebugMsg(&g_traceInfo,"CBOMMainDlg::GetUpdateSeqSn Failed!","SeqSn = %d,Action:%d,Type=%d",dwSeqSn,g_InOutUpdateSeqSn.eInOutUpdateFlag,bytInOutType);
	}
	return iRtn;
}
void TicketCardTransCmd(tagResPonseInfoData * pResInfoData,tagUpdateCmdInfo *InUpdateInfo,int &iRtn)
{
	iRtn = 0;
	int iCurForm = 0;
	DWORD  dwSeqSn = 0,iGetSnRtn = 0;	
	
	if(!InUpdateInfo->bIsUpdate)
	{
		iRtn = -1;
		return;
	}
        g_pMainGDM->GetUtils()->DebugMsg(&g_traceInfo,"TicketCardTransCmd:GetUpdateSeqSn","workmode = %02x",pResInfoData->bytWorkMode);

	iCurForm = pResInfoData->bytWorkMode;
	g_ReaderCmdData.Reset();
	switch(InUpdateInfo->bytChipType)
	{
	case 0x1:  // 单程票
		{
                        // 获取流水号
			dwSeqSn = 0;
			iGetSnRtn = GetUpdateSeqSn(dwSeqSn,0x1,eOttSeqSn);
			if(0 != iGetSnRtn)
			{
				g_pMainGDM->GetUtils()->DebugMsg(&g_traceInfo,"TicketCardTransCmd:GetUpdateSeqSn","iGetSnRtn = %d",iGetSnRtn);
				iRtn = iGetSnRtn;
				goto ErrorFlag;
			}
                        
                        if(0x20 == iCurForm)  // 进站读写器
                        {
                            switch(InUpdateInfo->bytUpdateMode)
                            {
                                case 0x09: // 普通进站指令
                                {
                                    g_ReaderCmdData.eCommandCode = eCSC_OTT_ENTRY;
                                    g_ReaderCmdData.bytRetrySN = 0;
                                    g_ReaderCmdData.RequestDataBody_u.tagOttEntryReq.bytEntryFlag = InUpdateInfo->bytTransFlag;
                                    g_ReaderCmdData.RequestDataBody_u.tagOttEntryReq.lTransTime = InUpdateInfo->dwTransTime;
                                    g_ReaderCmdData.RequestDataBody_u.tagOttEntryReq.dwTransTraceNum = dwSeqSn;
                               }
                                break;
                                case 0x0a: // 专用进站指令
                                {
                                    g_ReaderCmdData.eCommandCode = eCSC_OTT_ENTRY2;
                                    g_ReaderCmdData.bytRetrySN = 0;
                                    g_ReaderCmdData.RequestDataBody_u.tagOttSpecialEntryReq.lTransTime = InUpdateInfo->dwTransTime;
                                    g_ReaderCmdData.RequestDataBody_u.tagOttSpecialEntryReq.dwTransTraceNum = dwSeqSn;
                                    g_ReaderCmdData.RequestDataBody_u.tagOttSpecialEntryReq.bytEntryFlag = InUpdateInfo->bytTransFlag;
                                    g_ReaderCmdData.RequestDataBody_u.tagOttSpecialEntryReq.wBalance = InUpdateInfo->dwAfterTransValue;
                                    g_ReaderCmdData.RequestDataBody_u.tagOttSpecialEntryReq.bytWRTicketFlag = InUpdateInfo->bytWriteCardFlag;                                    
                                }
                                break;

                                default:
                                {
                                    g_pMainGDM->GetUtils()->DebugMsg(&g_traceInfo,"TicketCardTransCmd::GetUpdateSeqSn","TransCmd = %d",InUpdateInfo->bytUpdateMode);                                   
                                    goto ErrorFlag;
                                }
                                break;
                            }
                        }
                        else if(0x40 == iCurForm)  // 出站读写器
                        {
                            switch(InUpdateInfo->bytUpdateMode)
                            {
                                case 0x0b:
                                {
                                    g_ReaderCmdData.eCommandCode = eCSC_OTT_EXIT;
                                    g_ReaderCmdData.bytRetrySN = 0;
                                    g_ReaderCmdData.RequestDataBody_u.tagOttExitReq.lTransTime = InUpdateInfo->dwTransTime;
                                    g_ReaderCmdData.RequestDataBody_u.tagOttExitReq.dwTransTraceNum = dwSeqSn;
                                    g_ReaderCmdData.RequestDataBody_u.tagOttExitReq.bytTransFlag = InUpdateInfo->bytTransFlag;
                                    g_ReaderCmdData.RequestDataBody_u.tagOttExitReq.shTransAmount = InUpdateInfo->dwTransValue;
                                    g_ReaderCmdData.RequestDataBody_u.tagOttExitReq.shBalance = InUpdateInfo->dwAfterTransValue;
                                    g_ReaderCmdData.RequestDataBody_u.tagOttExitReq.bytWRTicketFlag = InUpdateInfo->bytWriteCardFlag;                                                                            
                                    
                                }
                                    break;
                            }
                            
                        }
                        else if(0x60 == iCurForm)  // 出站读写器
                        {
                            switch(InUpdateInfo->bytUpdateMode)
                            {
                                case 0x0d:
                                {
                                    g_ReaderCmdData.eCommandCode = eCSC_OTT_EXIT;
                                    g_ReaderCmdData.bytRetrySN = 0;
                                    g_ReaderCmdData.RequestDataBody_u.tagOttExitReq.lTransTime = InUpdateInfo->dwTransTime;
                                    g_ReaderCmdData.RequestDataBody_u.tagOttExitReq.dwTransTraceNum = dwSeqSn;
                                    g_ReaderCmdData.RequestDataBody_u.tagOttExitReq.bytTransFlag = InUpdateInfo->bytTransFlag;
                                    g_ReaderCmdData.RequestDataBody_u.tagOttExitReq.shTransAmount = InUpdateInfo->dwTransValue;
                                    g_ReaderCmdData.RequestDataBody_u.tagOttExitReq.shBalance = InUpdateInfo->dwAfterTransValue;
                                    g_ReaderCmdData.RequestDataBody_u.tagOttExitReq.bytWRTicketFlag = InUpdateInfo->bytWriteCardFlag;                                                                            
                                    
                                }
                                    break;
                            }
                            
                        }
                        else
                        {
                            
                            goto ErrorFlag;
                        }

			// 流水号备份
			g_CardDataBak.m_dwTransSn = dwSeqSn;
			// 交易时间备份
			g_CardDataBak.m_dwTransTimePre = InUpdateInfo->dwTransTime;//time(NULL)+8*60*60;
			// 交易金额备份
			g_CardDataBak.m_wFare = InUpdateInfo->dwTransValue;
		}
		break;
	case 0x2: // 交通卡
		{                    
                    	// 流水号
			dwSeqSn = 0;
                        BYTE  bytTxnDateTime[7] = {0};
			iGetSnRtn = GetUpdateSeqSn(dwSeqSn,0x1,eOtcSeqSn);
			if(0 != iGetSnRtn)
			{
				g_pMainGDM->GetUtils()->DebugMsg(&g_traceInfo,"TicketCardTransCmd::GetUpdateSeqSn","iGetSnRtn = %d",iGetSnRtn);
				iRtn = iGetSnRtn;
				goto ErrorFlag;
			}
                        
                        // 交易时间
			uint32_t dwUpdateTimeSn = InUpdateInfo->dwTransTime;
			time_t TransTime = dwUpdateTimeSn-(8*60*60);
			struct tm tTransTm;
                        localtime_r(&TransTime,&tTransTm);
			bytTxnDateTime[0] = INT2BCD((BYTE)((tTransTm.tm_year +1900)/ 100));
			bytTxnDateTime[1] = INT2BCD((BYTE) (tTransTm.tm_year % 100));
			bytTxnDateTime[2] = INT2BCD((BYTE) (tTransTm.tm_mon+1));
			bytTxnDateTime[3] = INT2BCD((BYTE) (tTransTm.tm_mday));
			bytTxnDateTime[4] = INT2BCD((BYTE) (tTransTm.tm_hour));
			bytTxnDateTime[5] = INT2BCD((BYTE) (tTransTm.tm_min));
			bytTxnDateTime[6] = INT2BCD((BYTE) (tTransTm.tm_sec));
                        
                        if(0x20 == iCurForm)  // 进站读写器
                        {
                            switch(InUpdateInfo->bytUpdateMode)
                            {
                                case 0x09: // 普通进站指令
                                {
                                    g_ReaderCmdData.eCommandCode = eCSC_OTC_ENTRY;
                                    g_ReaderCmdData.bytRetrySN = 0;
                                    memcpy(g_ReaderCmdData.RequestDataBody_u.tagOtcEntryReq.bytTransDateTime,bytTxnDateTime,sizeof(bytTxnDateTime));//= InUpdateInfo.bytTransFlag;
                                    
                                    g_ReaderCmdData.RequestDataBody_u.tagOtcEntryReq.bytTransTraceNum[0] = (BYTE) ((dwSeqSn >> 16) & 0xFF);//InUpdateInfo.dwTransTime;
                                    g_ReaderCmdData.RequestDataBody_u.tagOtcEntryReq.bytTransTraceNum[1] = (BYTE) ((dwSeqSn >> 8) & 0xFF);//InUpdateInfo.dwTransTime;
                                    g_ReaderCmdData.RequestDataBody_u.tagOtcEntryReq.bytTransTraceNum[2] = (BYTE) ((dwSeqSn) & 0xFF);//InUpdateInfo.dwTransTime;
                                    g_ReaderCmdData.RequestDataBody_u.tagOtcEntryReq.bytOverrideFlag = InUpdateInfo->bytTransFlag;
                               }
                                break;
                                default:
                                {
                                    g_pMainGDM->GetUtils()->DebugMsg(&g_traceInfo,"TicketCardTransCmd::GetUpdateSeqSn","TransCmd = %d",InUpdateInfo->bytUpdateMode);                                   
                                    goto ErrorFlag;
                                }
                                break;
                            }
                        }
                        else if(0x40 == iCurForm)  // 出站读写器
                        {
                                    g_ReaderCmdData.eCommandCode = eCSC_OTC_EXIT;
                                    g_ReaderCmdData.bytRetrySN = 0;
                                    memcpy(g_ReaderCmdData.RequestDataBody_u.tagOtcExitReq.bytTransDateTime,bytTxnDateTime,sizeof(bytTxnDateTime));//= InUpdateInfo.bytTransFlag;
                                    
                                    g_ReaderCmdData.RequestDataBody_u.tagOtcExitReq.bytTransTraceNum[0] = (BYTE) ((dwSeqSn >> 16) & 0xFF);//InUpdateInfo.dwTransTime;
                                    g_ReaderCmdData.RequestDataBody_u.tagOtcExitReq.bytTransTraceNum[1] = (BYTE) ((dwSeqSn >> 8) & 0xFF);//InUpdateInfo.dwTransTime;
                                    g_ReaderCmdData.RequestDataBody_u.tagOtcExitReq.bytTransTraceNum[2] = (BYTE) ((dwSeqSn) & 0xFF);//InUpdateInfo.dwTransTime;
                                    memcpy(g_ReaderCmdData.RequestDataBody_u.tagOtcExitReq.bytTransAmount,&InUpdateInfo->dwTransValue,3);// = InUpdateInfo.bytTransFlag;
                                    g_ReaderCmdData.RequestDataBody_u.tagOtcExitReq.bytTransType = InUpdateInfo->bytUpdateType;//dwTransValue,3);// = InUpdateInfo.bytTransFlag;
                        }

			// 流水号备份
			g_CardDataBak.m_dwTransSn = dwSeqSn;
			// 交易时间备份
			g_CardDataBak.m_dwTransTimePre = InUpdateInfo->dwTransTime;//time(NULL)+8*60*60;
			// 交易金额备份
			g_CardDataBak.m_wFare = InUpdateInfo->dwTransValue;
		} 
		break;
	case 0x3:  // 手机移动卡
		{

		}
		break;
	case 0x4:  // PBOC卡
                {
            		dwSeqSn = 0;
                        BYTE  bytTxnDateTime[7] = {0};
			iGetSnRtn = GetUpdateSeqSn(dwSeqSn,0x1,ePbocSeqSn);
			if(0 != iGetSnRtn)
			{
				g_pMainGDM->GetUtils()->DebugMsg(&g_traceInfo,"TicketCardTransCmd::GetUpdateSeqSn","iGetSnRtn = %d",iGetSnRtn);
				iRtn = iGetSnRtn;
				goto ErrorFlag;
			}
                        
                        // 交易时间
			uint32_t dwUpdateTimeSn = InUpdateInfo->dwTransTime;
			time_t TransTime = dwUpdateTimeSn-(8*60*60);
			struct tm tTransTm;
                        localtime_r(&TransTime,&tTransTm);
			bytTxnDateTime[0] = INT2BCD((BYTE)((tTransTm.tm_year +1900)/ 100));
			bytTxnDateTime[1] = INT2BCD((BYTE) (tTransTm.tm_year % 100));
			bytTxnDateTime[2] = INT2BCD((BYTE) (tTransTm.tm_mon+1));
			bytTxnDateTime[3] = INT2BCD((BYTE) (tTransTm.tm_mday));
			bytTxnDateTime[4] = INT2BCD((BYTE) (tTransTm.tm_hour));
			bytTxnDateTime[5] = INT2BCD((BYTE) (tTransTm.tm_min));
			bytTxnDateTime[6] = INT2BCD((BYTE) (tTransTm.tm_sec));

                        
                        g_ReaderCmdData.Reset();
                        if(0x20 == iCurForm)  // Entry Reader
                        {
                            g_ReaderCmdData.eCommandCode = eCSC_PBOC_ENTRY;
                            g_ReaderCmdData.bytRetrySN = 0;

                            memcpy(g_ReaderCmdData.RequestDataBody_u.tagPbocEntryReq.bytTransDateTime,bytTxnDateTime,sizeof(g_ReaderCmdData.RequestDataBody_u.tagPbocEntryReq.bytTransDateTime));

                            g_ReaderCmdData.RequestDataBody_u.tagPbocEntryReq.bytTxnSeqSn[0] = (dwSeqSn>>16) & 0xFF;
                            g_ReaderCmdData.RequestDataBody_u.tagPbocEntryReq.bytTxnSeqSn[1] = (dwSeqSn>>8) & 0xFF;
                            g_ReaderCmdData.RequestDataBody_u.tagPbocEntryReq.bytTxnSeqSn[2] = (dwSeqSn) & 0xFF;
                            g_ReaderCmdData.RequestDataBody_u.tagPbocEntryReq.bytTimeOutFlag = InUpdateInfo->bytTransFlag;            


                        }
                        else if(0x40 == iCurForm)
                        {

                        }
                   }                    
                break;
	case 0x5:  // 员工卡
		{

		}
		break;
	//二维码
	case 0x06:
        {
            		dwSeqSn = 0;
                        BYTE  bytTxnDateTime[7] = {0};
			iGetSnRtn = GetUpdateSeqSn(dwSeqSn,0x1,eMtcSeqSn);
			if(0 != iGetSnRtn)
			{
				g_pMainGDM->GetUtils()->DebugMsg(&g_traceInfo,"TicketCardTransCmd::GetUpdateSeqSn","iGetSnRtn = %d",iGetSnRtn);
				iRtn = iGetSnRtn;
				goto ErrorFlag;
			}
                        
                        // 交易时间
			uint32_t dwUpdateTimeSn = InUpdateInfo->dwTransTime;
                        uint16_t  QrTimes = InUpdateInfo->wTdApplyCount;
                        g_ReaderCmdData.Reset();
                        if(0x20 == iCurForm)  // 进站读写器
                        {
                            g_ReaderCmdData.eCommandCode = eCSC_OTQ_ENTRY;
                            g_ReaderCmdData.bytRetrySN = 0;

                            memcpy(g_ReaderCmdData.RequestDataBody_u.tagQrEntryReq.bytSleID,g_LocalSleInfo.bytSleId,sizeof(g_LocalSleInfo.bytSleId));
                            g_ReaderCmdData.RequestDataBody_u.tagQrEntryReq.bytQrProduceTimes[0] = (QrTimes) & 0xFF;
                            g_ReaderCmdData.RequestDataBody_u.tagQrEntryReq.bytQrProduceTimes[1] = (QrTimes>>8) & 0xFF;

                            g_ReaderCmdData.RequestDataBody_u.tagQrEntryReq.bytTransSn[0] = (dwSeqSn>>24) & 0xFF;
                            g_ReaderCmdData.RequestDataBody_u.tagQrEntryReq.bytTransSn[1] = (dwSeqSn>>16) & 0xFF;
                            g_ReaderCmdData.RequestDataBody_u.tagQrEntryReq.bytTransSn[2] = (dwSeqSn>>8) & 0xFF;
                            g_ReaderCmdData.RequestDataBody_u.tagQrEntryReq.bytTransSn[3] = (dwSeqSn) & 0xFF;


                            g_ReaderCmdData.RequestDataBody_u.tagQrEntryReq.bytTransTime[0] = (dwUpdateTimeSn>>24) & 0xFF;
                            g_ReaderCmdData.RequestDataBody_u.tagQrEntryReq.bytTransTime[1] = (dwUpdateTimeSn>>16) & 0xFF;
                            g_ReaderCmdData.RequestDataBody_u.tagQrEntryReq.bytTransTime[2] = (dwUpdateTimeSn>>8) & 0xFF;
                            g_ReaderCmdData.RequestDataBody_u.tagQrEntryReq.bytTransTime[3] = (dwUpdateTimeSn) & 0xFF;


                            g_ReaderCmdData.RequestDataBody_u.tagQrEntryReq.bytTransFlag = 0;            


                        }
                        else if(0x40 == iCurForm)
                        {
                            g_ReaderCmdData.eCommandCode = eCSC_OTQ_EXIT;
                            g_ReaderCmdData.bytRetrySN = 0;
                            memcpy(g_ReaderCmdData.RequestDataBody_u.tagQrExitReq.bytSleID,g_LocalSleInfo.bytSleId,sizeof(g_LocalSleInfo.bytSleId));
                            g_ReaderCmdData.RequestDataBody_u.tagQrExitReq.bytQrProduceTimes[0] = (QrTimes) & 0xFF;
                            g_ReaderCmdData.RequestDataBody_u.tagQrExitReq.bytQrProduceTimes[1] = (QrTimes>>8) & 0xFF;

                            g_ReaderCmdData.RequestDataBody_u.tagQrExitReq.bytTransSn[0] = (dwSeqSn>>24) & 0xFF;
                            g_ReaderCmdData.RequestDataBody_u.tagQrExitReq.bytTransSn[1] = (dwSeqSn>>16) & 0xFF;
                            g_ReaderCmdData.RequestDataBody_u.tagQrExitReq.bytTransSn[2] = (dwSeqSn>>8) & 0xFF;
                            g_ReaderCmdData.RequestDataBody_u.tagQrExitReq.bytTransSn[3] = (dwSeqSn) & 0xFF;

                            g_ReaderCmdData.RequestDataBody_u.tagQrExitReq.bytTransTime[0] = (dwUpdateTimeSn>>24) & 0xFF;
                            g_ReaderCmdData.RequestDataBody_u.tagQrExitReq.bytTransTime[1] = (dwUpdateTimeSn>>16) & 0xFF;
                            g_ReaderCmdData.RequestDataBody_u.tagQrExitReq.bytTransTime[2] = (dwUpdateTimeSn>>8) & 0xFF;
                            g_ReaderCmdData.RequestDataBody_u.tagQrExitReq.bytTransTime[3] = (dwUpdateTimeSn) & 0xFF;
                            
                            memcpy(g_ReaderCmdData.RequestDataBody_u.tagQrExitReq.bytTransValue,&InUpdateInfo->dwTransValue,2);// = InUpdateInfo.bytTransFlag;

                        }
       }
            break;
	}
        switch(pResInfoData->bytWorkMode)
        {
            case 0x20:  // 进站读写器
            {
                g_pMainGDM->GetReader()->SendReaderCmd(&g_ReaderCmdData);
            }
            break;
            case 0x40:  // 出站读写器
            {
                g_pMainGDM->GetExitReader()->SendReaderCmd(&g_ReaderCmdData);
            }
            break;
            case 0x60:  // Collect Reader
            {
                g_pMainGDM->GetExitCollectReader()->SendReaderCmd(&g_ReaderCmdData);
            }
            break;
            default:
            {
                iRtn = -3;
            }
            break;
        }
        return ;

ErrorFlag:
	iRtn = -2;
	return;
}

int InitShParameter()
{
	DWORD dwRtn;
	int  iRtn = -1;

	// 
	g_traceInfo.trac_fichier("AGateShRunningStatus","./log",1,2,20000);
        // g_pMainGDM->GetUtils()->DebugMsg(&g_traceInfo,"��ȡ������Ϣ�ɹ�!","GetLocalSLEInfo(&g_LocalSleInfo)!");
        
        //
        CGateContex   m_GateContex(g_pMainGDM);
        
        if(m_GateContex.lock_file(LOCKFILE))
        {
            printf("!!!!!!!!!!!!!!!!!!!!!!!!!--Processed，不要重复执行\n");
            g_pMainGDM->Stop();
            exit(0);
        }
       atexit(m_GateContex.delete_file);	// 注册进程清理函数
            
        //
        BYTE   bytIsTicketReadFlag = 0;
        uint16_t  wExitCounter = 0;
        Gate_msg msginfo,msgInfoPre;
        while(1)
        {
            int  iRtn = -1;
            int  iCounter = 0;
            msginfo.Reset();
            iRtn = msgrcv(g_pMainGDM->GetMsgQueueId(),&msginfo,sizeof(Gate_msg),0,IPC_NOWAIT);
            // iRtn = msgrcv(g_pMainGDM->GetMsgQueueId(),&msginfo,sizeof(Gate_msg),0,0);


            if(-1 == iRtn)
            {
                usleep(50000);
                // printf("msgrcv %s,No message Comming!\r\n","Failed!");
                continue;
            }
            else
            {
                if( msgInfoPre.msg_type != msginfo.msg_type && (msgInfoPre.event != msginfo.event))
                {    
                    msgInfoPre.msg_type = msginfo.msg_type;
                    msgInfoPre.event = msginfo.event;
                    printf("msgrcv %s type=%d event =%d,msgInfoPre.msg_type=%d\r\n","Ok!",msginfo.msg_type,msginfo.event,msgInfoPre.msg_type);
                }
            }
            switch(msginfo.msg_type)
            {
                case eExReader_type: // Exit CSC Reader Module
                {
                    switch(msginfo.event)
                    {
                        case eEvtReset_ok:
                        {
                            memcpy(&g_ExitReaderResInfo.tagReaderResetResp,msginfo.u.reader.m_msgReaderData,sizeof(CSC_MP_RESETREADER)); 
                        }
                        break;
                        case  eEvtReadCard_ok:  // 读到票卡信息
                        {   
                            if(0 ==g_ExitCardDataBak.m_bytReadInfoType)  // 票卡第一次读到票信息，需要清应答信息数据
                            {
                                g_ExitReaderResInfo.Reset(0x40);
                            }
                            g_ExitReaderResInfo.bytRecardInfoCmdCode = msginfo.u.reader.m_bytRecardInfoCmdCode;
                            g_ExitReaderResInfo.iCurrentFormView = mtEFO;
                            switch(msginfo.u.reader.m_bytRecardInfoCmdCode)
                            {
                                case eCSC_OTC_READCARD:
                                {
                                    memcpy(&g_ExitReaderResInfo.tagReaderOtcResp,msginfo.u.reader.m_msgReaderData,sizeof(CSC_MP_READ_CARDINFO));
                                    if(0x82 == g_ExitReaderResInfo.tagReaderOtcResp.bytDataBlkNo || 0x92 == g_ExitReaderResInfo.tagReaderOtcResp.bytDataBlkNo)
                                           g_ExitReaderResInfo.tagReaderOtcResp.bytDataBlkNo += 1;
                                }
                                break;
                                case eCSC_OTT_READCARD:
                                {
                                    memcpy(&g_ExitReaderResInfo.tagReaderReadOttResp,msginfo.u.reader.m_msgReaderData,sizeof(CSC_MP_OTT_READCARD));                            
                                }
                                break;
                                case eCSC_PBOC_READCARD:
                                {
                                    memcpy(&g_ExitReaderResInfo.tagReaderPbocInfoResp,msginfo.u.reader.m_msgReaderData,sizeof(CSC_MP_PBOC_READCARD));
                                }
                                break;
                                case eCSC_EMPLOY_READCARD:
                                {
                                    memcpy(&g_ExitReaderResInfo.tagReaderPbocEmployeeInfoResp,msginfo.u.reader.m_msgReaderData,sizeof(CSC_MP_PBOC_EMPLOYEE_READCARD));
                                }
                                break;
                                case eCSC_OTQ_READCARD:
                                {
                                    memcpy(&g_ExitReaderResInfo.tagReaderReadOtqResp,msginfo.u.reader.m_msgReaderData,sizeof(CSC_QR_BLOCK));            
                                }
                                break;
                            }               
                            // 分析票卡业务
                            eAnalysisPreType eResult = g_ExitCardDataBak.IsAnalysisSameCard(&g_ExitReaderResInfo);
                            switch(eResult)
                            {
                                case eEntryTransCmd:
                                case eExitTransCmd:
                                case eStartAnalysis:  // 分析票卡业务
                                {
                                    g_pMainGDM->GetAnalysisModule()->Analysis(&g_ExitReaderResInfo,g_ExitCardAnalysisInfo,g_ExitCardTransCmdInfo);
                                    //m_GateContex.SetDisplayType(&g_ExitReaderResInfo,&g_ExitCardTransCmdInfo);
                                       
                                    // 显示拒绝或者发出站指令
                                    if(!g_ExitCardAnalysisInfo.CanUpdate || (0x1 == g_ExitCardTransCmdInfo.bytRecycleFlag) )  // 分析结果是: 否，显示拒绝码信息
                                    { 
                                        m_GateContex.SetDisplayType(&g_ExitReaderResInfo,&g_ExitCardAnalysisInfo,&g_ExitCardTransCmdInfo);
                                            ReaderAbort(&g_ExitReaderResInfo);
                                    }
                                    else 
                                    {
                                    // Is Exit 
                                        int  iRtn = 0;
                                        TicketCardTransCmd(&g_ExitReaderResInfo,&g_ExitCardTransCmdInfo,iRtn);  // 下发交易命令
                                         if(iRtn)
                                         {
                                             cout<<"\nAnalysis End ExitReader Send TicketCardTransCmd"<<":"<<iRtn<<"\n"<<endl;
                                             ReaderAbort(&g_ExitReaderResInfo);
                                         }
                                         else
                                         {
                                             cout<<"\nAnalysis End ExitReader Send TicketCardTransCmd "<<":"<<iRtn<<"\n"<<endl; 
                                         }

                                    }
                                }
                                break;
                                case eTransInOneMinute:  // 一分钟内连续刷卡 没有通行
                                {
                                    ReaderAbort(&g_ExitReaderResInfo);                             
                                }
                                break;
                                case  eGetProofTac:   // 重取TAC，防拔处理
                                {
                                }
                                break;
                                case eSameCardInfo:
                                {
                                    cout<<"\nAnalysis End  SamCard!"<<":"<<iRtn<<"\n"<<endl;
                                    ReaderAbort(&g_ExitReaderResInfo);                            
                                }
                                break;
                            }

                        }
                        break;
                        case eEvtExit_ok:  // 收到出站指令后的业务处理
                        {
                            // 过人
                            g_pMainGDM->GetSnapMaFlap()->AddExitNum(1);
                            // cout<<"\nAnalysis End ExitReader g_pMainGDM->GetSnapMaFlap()->AddExitNum(1)\n"<<endl;                             g_ExitReaderResInfo.bytRecardInfoCmdCode = msginfo.u.reader.m_bytRecardInfoCmdCode;
                            ReqPass passOut{0x3, 1, PD_EXIT, PL_GREEN_ON, WL_LIGHT_OFF, 0, 0};
                            g_pMainGDM->GetGuFlap()->passAuth(passOut); 

                            g_ExitReaderResInfo.bytRecardInfoCmdCode = msginfo.u.reader.m_bytRecardInfoCmdCode;  // important       
                            m_GateContex.SetDisplayType(&g_ExitReaderResInfo,&g_ExitCardAnalysisInfo,&g_ExitCardTransCmdInfo);



                            FillTransResponseData(&g_ExitReaderResInfo,&msginfo,&g_ExitCardDataBak);                     
                            // 写交易记录
                            BYTE  bytRtn = 0;
                            FillRecord(g_pMainGDM,&g_ExitCardTransCmdInfo,&g_ExitReaderResInfo,&g_InOutMsgTxnRec,&g_ExitCardDataBak,&g_LocalSleInfo,bytRtn);
                            //
                            int iRtn = 0;
                            ModifyRegist(g_pMainGDM,&g_ExitReaderResInfo,&g_traceInfo,&g_ExitCardAnalysisInfo,0,&g_ExitCardTransCmdInfo,&g_InOutUpdateSeqSn,iRtn);
                            ReaderAbort(&g_ExitReaderResInfo);
                        }
                        break;
                        case eEvtExit_fail:
                        {
                            m_GateContex.PleaseTryAgain(g_ExitReaderResInfo.iCurrentFormView);                           
                        }
                        break;

                    }
                }            
                break;
                case eEnReader_type:  // Entry读写器模块
                {
                    switch(msginfo.event)
                    {
                        case eEvtReset_ok:
                        {
                            memcpy(&g_ReaderResInfo.tagReaderResetResp,msginfo.u.reader.m_msgReaderData,sizeof(CSC_MP_RESETREADER)); 
                        }
                        break;
                        case  eEvtReadCard_ok:  // 读到票卡信息
                        {   
                            if(0 ==g_CardDataBak.m_bytReadInfoType)  // 票卡第一次读到票信息，需要清应答信息数据
                            {
                                g_ReaderResInfo.Reset(0x20);
                            }
                            g_ReaderResInfo.bytRecardInfoCmdCode = msginfo.u.reader.m_bytRecardInfoCmdCode;
                            g_ReaderResInfo.iCurrentFormView = mtBOM;
                            switch(msginfo.u.reader.m_bytRecardInfoCmdCode)
                            {
                                case eCSC_OTC_READCARD:
                                {
                                    memcpy(&g_ReaderResInfo.tagReaderOtcResp,msginfo.u.reader.m_msgReaderData,sizeof(CSC_MP_READ_CARDINFO));
                                    if(0x82 == g_ReaderResInfo.tagReaderOtcResp.bytDataBlkNo || 0x92 == g_ReaderResInfo.tagReaderOtcResp.bytDataBlkNo)
                                           g_ReaderResInfo.tagReaderOtcResp.bytDataBlkNo += 1;
                                }
                                break;
                                case eCSC_OTT_READCARD:
                                {
                                    memcpy(&g_ReaderResInfo.tagReaderReadOttResp,msginfo.u.reader.m_msgReaderData,sizeof(CSC_MP_OTT_READCARD));                            
                                }
                                break;
                                case eCSC_PBOC_READCARD:
                                {
                                    memcpy(&g_ReaderResInfo.tagReaderPbocInfoResp,msginfo.u.reader.m_msgReaderData,sizeof(CSC_MP_PBOC_READCARD));
                                }
                                break;
                                case eCSC_EMPLOY_READCARD:
                                {
                                    memcpy(&g_ReaderResInfo.tagReaderPbocEmployeeInfoResp,msginfo.u.reader.m_msgReaderData,sizeof(CSC_MP_PBOC_EMPLOYEE_READCARD));
                                }
                                break;
                                case eCSC_OTQ_READCARD:
                                {
                                    memcpy(&g_ReaderResInfo.tagReaderReadOtqResp,msginfo.u.reader.m_msgReaderData,sizeof(CSC_QR_BLOCK));            
                                }
                                break;                        
                            }

                            // 分析票卡业务
                            eAnalysisPreType eResult = g_CardDataBak.IsAnalysisSameCard(&g_ReaderResInfo);
                            switch(eResult)
                            {
                                case eEntryTransCmd:
                                case eExitTransCmd:
                                case eStartAnalysis:  // 分析票卡业务
                                {
                                    g_pMainGDM->GetAnalysisModule()->Analysis(&g_ReaderResInfo,g_CardAnalysisInfo,g_CardTransCmdInfo);
                                    //ReaderAbort(&g_ReaderResInfo);


                            // 显示拒绝或者发进站指令
                        if(!g_CardAnalysisInfo.CanUpdate)  // 分析结果是不可以交易，显示拒绝码信息
                                    {
                                           g_CardAnalysisInfo.iRejectCode = atoi(g_CardAnalysisInfo.strAnalysisResult.c_str());
                                           m_GateContex.SetDisplayType(&g_ReaderResInfo,&g_CardAnalysisInfo,&g_CardTransCmdInfo);
                            
                                           //g_CardDataBak.m_bytReadInfoType = g_ReaderResInfo.bytRecardInfoCmdCode;
                                           ReaderAbort(&g_ReaderResInfo);
                                           cout<<"\nAnalysis End EntryReader Send Abort \n" <<"Reject Code:"<<g_CardAnalysisInfo.strAnalysisResult<<endl;                            
                        }
                                    else 
                                    {
                                        int iRtn = 0;
                                        TicketCardTransCmd(&g_ReaderResInfo,&g_CardTransCmdInfo,iRtn);  // 下发交易命令
                                         if(iRtn)
                                         {
                                             cout<<"\nAnalysis End EntryReader Send TicketCardTransCmd"<<":"<<iRtn<<"\n"<<endl;
                                             ReaderAbort(&g_ReaderResInfo);
                                         }
                                         else
                                         {
                                             cout<<"\nAnalysis End EntryReader Send TicketCardTransCmd "<<":"<<iRtn<<"\n"<<endl; 
                                         }
                                    }
                                }
                                break;
                                case eTransInOneMinute:  // 一分钟内连续刷卡 没有通行
                                {
                                    ReaderAbort(&g_ReaderResInfo); 
                                }
                                break;
                                case  eGetProofTac:   // 重取TAC，防拔处理
                                {
                                    ReaderAbort(&g_ReaderResInfo);
                                }
                                break;
                                case eSameCardInfo:
                                {                            
                                    ReaderAbort(&g_ReaderResInfo); 
                                }
                                break;
                            }

                        }
                        break;
                        case eEvtEntry_ok:  // 收到进站指令后的业务处理
                        {
                            // 过人                
                            g_pMainGDM->GetSnapMaFlap()->AddEnterNum(1);
                            
                            
                           g_ReaderResInfo.bytRecardInfoCmdCode = msginfo.u.reader.m_bytRecardInfoCmdCode;         
                           m_GateContex.SetDisplayType(&g_ReaderResInfo,&g_CardAnalysisInfo,&g_CardTransCmdInfo);



                            ReqPass passOut{0x3, 1, PD_ENTRY, PL_GREEN_ON, WL_LIGHT_OFF, 0, 0};
                            g_pMainGDM->GetGuFlap()->passAuth(passOut);                    
                            // 
                            FillTransResponseData(&g_ReaderResInfo,&msginfo,&g_CardDataBak);                     
                            // 写交易记录
                            BYTE  bytRtn = 0;
                            FillRecord(g_pMainGDM,&g_CardTransCmdInfo,&g_ReaderResInfo,&g_InOutMsgTxnRec,&g_CardDataBak,&g_LocalSleInfo,bytRtn);
                            //
                            int iRtn = 0;
                            ModifyRegist(g_pMainGDM,&g_ReaderResInfo,&g_traceInfo,&g_CardAnalysisInfo,0,&g_CardTransCmdInfo,&g_InOutUpdateSeqSn,iRtn);

                            ReaderAbort(&g_ReaderResInfo);
                        }
                        break;
                        case eEvtEntry_fail:
                        {
                            m_GateContex.PleaseTryAgain(g_ReaderResInfo.iCurrentFormView);
                        }
                        break;

                    }
                }
                break;
                case eParam_type:
                {
                            // ��ȡ���ػ�����Ϣ����
                    switch(msginfo.event)
                    { 
                        case eEvtParamCompleteFlag:
                        {
                            g_paramPtr = g_pMainGDM->GetParameter()->get_shcurparam();
                            g_pMainGDM->GetParameter()->GetLocalSLEInfo(&g_LocalSleInfo);
                            
                            // msg comm
                            g_pMainGDM->GetMsgComSC()->SetMsgTxnConfigParam(NULL);
                            sleep(5);

                            // flap
                            InitSNAPMATripod();


                            // Monitor status
                            g_DevStausMonitor.Start(g_pMainGDM,&m_GateContex);

                            // m_GateContex start
                            m_GateContex.InitStart();                   
                            printf("Param Complete Flag\n");
                        }
                        break;
                        case eEvtParam3014:
                        {
                            switch(msginfo.u.param.m_bytCompleteFlag)
                            {
                                case 0x2:// IP Not Correct
                                {
                                        printf("IP Error!\n");                                    
                                }
                                break;
                                case 0x3:  // Node Info Error
                                {
                                    printf("\n !!!!!!!!!!!!!!!!!!!!!!!!   Node Info ID Error!\n");                                     
                                    g_pMainGDM->GetSQLITEDBManagerProxy()->DeleteDB();
                                }
                                break;
                            }
                        }
                        break;
                    }


                }
                break;
                case eGuFlapPassage_type:
                {
                    switch(msginfo.event){
                        case ERROR:
                            //printf("err: %s\n", msginfo.u.content.data.err);
                            break;
                        case POLL:
                            printf("rsp:\n");
                            //LOG_HEX_ADD((uint8_t *) &content.data.rspPoll, sizeof(RspPoll));
                            break;
                        default:
                            //LOG_ADD("rsp:");
                            //LOG_HEX_ADD((uint8_t *) &content.data.rspCommon, sizeof(RspCommon));
                            break;            
                    }
                }
                break;
                // MA SNAP
                case ePassage_type:
                {
                    switch(msginfo.event)
                    {
                        case 1:
                        {
                            m_GateContex.SetDisplayType(NULL,NULL,NULL);
                        }
                        break;
                        case bEntryIntrude:
                        case bExitIntrude:
                        case bEntryReverse:
                        case bExitReverse:
                        case bEntryTimeOut:
                        case bExitTimeOut:
                        case bEntryTrailing:
                        case bExitTrailing:
                        case eEntryObjectStore:
                        case eExitObjectStore:
                        {
                            printf("\n #####################Gate Exception\n");
                            g_pMainGDM->GetIoModule()->TopLightControlControl(eTopLightRedOn);
                            //g_pMainGDM->GetIoModule()->SideLightControl(eSideLightRedOn);
                            //g_pMainGDM->GetUtils()->select_usleep(150000);
                            // g_pMainGDM->GetIoModule()->TopLightControlControl(eTopLightRedClose);
                            // g_pMainGDM->GetIoModule()->SideLightControl(eSideLightRedClose);
                        }
                        break;
                        case eEntryObjectLeave:
                        case eExitObjectLeave:
                        {
                            printf("\n ##################### Gate OK!\n");
                            g_pMainGDM->GetIoModule()->TopLightControlControl(eTopLightRedClose);                            
                        }
                        break;
                        case 21:
                        {
                            
                        }
                            break;
                        case 22:
                        {
                            
                        }
                            break;
                    }
                    char TpuBuffTmp[MAX_PATH] = {0};
                    if(memcmp(&msgInfoPre.u.door,&msginfo.u.door,sizeof(msginfo.u.door)))
                    {
                        g_pMainGDM->GetUtils()->BytesToHexStr((char*)TpuBuffTmp,(BYTE*)&msginfo.u.door,sizeof(msginfo.u.door));
                        g_pMainGDM->GetUtils()->DebugMsg(&g_traceInfo,"ePassage_type Receive Data","msgInfoPre.u.door:%s",TpuBuffTmp);

                        memcpy(&msgInfoPre.u.door,&msginfo.u.door,sizeof(msginfo.u.door)); 
                        
                        if(msginfo.u.door.bFlag == 0x1 || msginfo.u.door.bFlag == 0x2)
                        {
                            m_GateContex.SetDisplayType(NULL,NULL,NULL);
                        }
                    }                    
                }
                break;
                // Collect Module
                case eSnapTPU_type:  
                {                        
                    switch(msginfo.event){
                        case bFault:
                        {

                        }
                            break;
                        case bStatusCode:
                        {
                            if( (!msgInfoPre.u.tpumsg.statusCode.bitRWPosition) && msginfo.u.tpumsg.statusCode.bitRWPosition)
                            {
                                 ReaderReadTicket(&g_TpuExitReaderResInfo,0x90);
                            }
                            else if(msginfo.u.tpumsg.statusCode.bitRWPosition||msginfo.u.tpumsg.statusMark.bitReading)
                            {
                                // 退票
                                //msgInfoPre.u.tpumsg.statusCode.bitRWPosition = 0;
                                if(wExitCounter++ > 20)
                                {
                                    msgInfoPre.u.tpumsg.statusCode.bitRWPosition = 0;
                                    //g_pMainGDM->GetTpuModule()->TPU_TicketExit(); 
                                    wExitCounter = 0;
                                }
                            }                             
                        }
                            break;
                        case bStatusParm:
                        {
                            if( (!msgInfoPre.u.tpumsg.statusCode.bitRWPosition) && msginfo.u.tpumsg.statusCode.bitRWPosition)
                            {
                                 // ReaderReadTicket(&g_TpuExitReaderResInfo,0x90);
                            }
                            else if(msginfo.u.tpumsg.statusCode.bitRWPosition||msginfo.u.tpumsg.statusMark.bitReading)
                            {
                                // 退票
                                if(wExitCounter++ > 20)
                                {
                                    msgInfoPre.u.tpumsg.statusCode.bitRWPosition = 0;
                                    //g_pMainGDM->GetTpuModule()->TPU_TicketExit(); 
                                    wExitCounter = 0;
                                }
                            } 
                            
                            g_TpuExitCardDataBak.m_bytCurrentTicketBox = msginfo.u.tpumsg.statusCode.bitCurrentTicketBox;
//  票盒不在
                            if((msginfo.u.tpumsg.statusMark.bitPressOrOpenBox ))  
                            {

//					g_DevStausMonitor.FillDevEvent(eBox1TakenOut,TRUE,m_DevStatusEvent.uMachineEventStatus.bytDevStatus+2);
//					g_DevStausMonitor.UploadDevStatusEvent(&m_DevStatusEvent); 
                                if((!msgInfoPre.u.tpumsg.statusMark.bitFrontBoxExist && msginfo.u.tpumsg.statusMark.bitFrontBoxExist) )
                                {
                                printf("@@@@@@@@@@@@@@@@@@@@TicketBox 1 Fetch Out\n");                                    
                                    g_DevStausMonitor.TicketBoxIsOut(1,1);
                                }
                                if(!msgInfoPre.u.tpumsg.statusMark.bitBehindBoxExist && msginfo.u.tpumsg.statusMark.bitBehindBoxExist)
                                {
                                     printf("@@@@@@@@@@@@@@@@@@@@TicketBox 2 Fetch Out\n");
                                     g_DevStausMonitor.TicketBoxIsOut(1,2);                                   
                                }
                            }
                            
                            if(!msginfo.u.tpumsg.statusMark.bitPressOrOpenBox ) 
                            // && (( !msginfo.u.tpumsg.statusMark.bitFrontBoxEntrance) && ( !msginfo.u.tpumsg.statusMark.bitBehindBoxExist)))
                            {

                                if((msgInfoPre.u.tpumsg.statusMark.bitFrontBoxExist && !msginfo.u.tpumsg.statusMark.bitFrontBoxExist) )
                                {
                                    printf("@@@@@@@@@@@@@@@@@@@@TicketBox 1 Fetch In Back\n");                                   
                                    g_DevStausMonitor.TicketBoxIsOut(2,1);
                                }
                                if(msgInfoPre.u.tpumsg.statusMark.bitBehindBoxExist && !msginfo.u.tpumsg.statusMark.bitBehindBoxExist)
                                {
                                     printf("@@@@@@@@@@@@@@@@@@@@TicketBox 2 Fetch In Back\n");  
                                     g_DevStausMonitor.TicketBoxIsOut(2,2);                                   
                                }
                            }
                        }
                        case 4:
                        {
                            //memset(7msgInfoPre.u.tpumsg,0,)
                            msgInfoPre.u.tpumsg.statusCode.bitRWPosition = 0;
                                if(wExitCounter++ > 20)
                                {
                                    //g_pMainGDM->GetTpuModule()->TPU_TicketExit(); 
                                    wExitCounter = 0;
                                }                            
                        }
                        break;
                        case bTicketEntryExit:
                        {
                            msgInfoPre.u.tpumsg.statusCode.bitRWPosition = 0;
                            
                        }
                        break;
                        default:
                            break; 
                    }
                    char TpuBuffTmp[MAX_PATH] = {0};
                    if(memcmp(&msgInfoPre.u.tpumsg,&msginfo.u.tpumsg,sizeof(msginfo.u.tpumsg)))
                    {
                        g_pMainGDM->GetUtils()->BytesToHexStr((char*)TpuBuffTmp,(BYTE*)&msginfo.u.tpumsg.faultCode,sizeof(msginfo.u.tpumsg.faultCode));
                        g_pMainGDM->GetUtils()->DebugMsg(&g_traceInfo,"TpuModule Receive Data","faultCode_Data:%s",TpuBuffTmp);

                        g_pMainGDM->GetUtils()->BytesToHexStr((char*)TpuBuffTmp,(BYTE*)&msginfo.u.tpumsg.statusCode,sizeof(msginfo.u.tpumsg.statusCode));
                        g_pMainGDM->GetUtils()->DebugMsg(&g_traceInfo,"TpuModule Receive Data","statusCode_Data:%s",TpuBuffTmp);          

                        g_pMainGDM->GetUtils()->BytesToHexStr((char*)TpuBuffTmp,(BYTE*)&msginfo.u.tpumsg.statusMark,sizeof(msginfo.u.tpumsg.statusMark));
                        g_pMainGDM->GetUtils()->DebugMsg(&g_traceInfo,"TpuModule Receive Data","statusMark_Data:%s",TpuBuffTmp);                                       
                        memcpy(&msgInfoPre.u.tpumsg,&msginfo.u.tpumsg,sizeof(msginfo.u.tpumsg));  
                    }  
                    if(msginfo.event != bStatusCode)
                    {
                         msgInfoPre.u.tpumsg.statusCode.bitRWPosition = 0;
                    }

                }
                break;
                // Exit Collect Reader  
                case eExitCollectReader_type:
                {
                    switch(msginfo.event)
                    {
                        case eEvtReset_ok:
                        {
                            g_TpuExitReaderResInfo.Reset(0x60);
                            memcpy(&g_TpuExitReaderResInfo.tagReaderResetResp,msginfo.u.reader.m_msgReaderData,sizeof(CSC_MP_RESETREADER)); 
                        }
                        break;
                        case eEvtReadCard_ok:
                        {
                            if(0 ==g_TpuExitCardDataBak.m_bytReadInfoType)  // 票卡第一次读到票信息，需要清应答信息数据
                            {
                                g_TpuExitReaderResInfo.Reset(0x60);
                            }
                            //bytIsTicketReadFlag = 0;
                            if(msginfo.u.reader.m_bytLparam != 0x81  || msginfo.u.reader.m_msgReaderData[0])
                            {
                                // 退票
                                g_pMainGDM->GetTpuModule()->TPU_TicketExit();
                                ReaderAbort(&g_TpuExitReaderResInfo); 
                                
                            }
                            else
                            {
                                g_TpuExitReaderResInfo.bytRecardInfoCmdCode = msginfo.u.reader.m_bytRecardInfoCmdCode;
                                g_TpuExitReaderResInfo.iCurrentFormView = mtEFO;
                                switch(msginfo.u.reader.m_bytRecardInfoCmdCode)
                                {
                                    case eCSC_OTT_READCARD:
                                    {
                                        memcpy(&g_TpuExitReaderResInfo.tagReaderReadOttResp,msginfo.u.reader.m_msgReaderData,sizeof(CSC_MP_OTT_READCARD));                            
                                    }
                                }               
                                // 分析票卡业务
                                eAnalysisPreType eResult = eStartAnalysis;//g_TpuExitCardDataBak.IsAnalysisSameCard(&g_TpuExitReaderResInfo);
                                switch(eResult)
                                {
                                    case eEntryTransCmd:
                                    case eExitTransCmd:
                                    case eStartAnalysis:  // 分析票卡业务    
                                    {
                                            g_pMainGDM->GetAnalysisModule()->Analysis(&g_TpuExitReaderResInfo,g_TpuExitCardAnalysisInfo,g_TpuExitCardTransCmdInfo);                                       
                                            // 显示拒绝或者发出站指令
                                            if( !g_TpuExitCardAnalysisInfo.CanUpdate )  // 分析结果是: 否，显示拒绝码信息
                                            { 
                                                // 退票
                                                g_pMainGDM->GetTpuModule()->TPU_TicketExit();                                                
                                                m_GateContex.SetDisplayType(&g_TpuExitReaderResInfo,&g_TpuExitCardAnalysisInfo,&g_TpuExitCardTransCmdInfo);
                                                    ReaderAbort(&g_TpuExitReaderResInfo);
                                                g_TpuExitCardDataBak.m_bytReadInfoType = g_TpuExitReaderResInfo.bytRecardInfoCmdCode;                                            
                                            }
                                            else 
                                            {
                                                int  iRtn = 0;
                                                if(0x1 != g_TpuExitCardTransCmdInfo.bytRecycleFlag )
                                                {
                                                    // 退票
                                                    g_pMainGDM->GetTpuModule()->TPU_TicketExit();
                                                }
                                                else
                                                {
                                                    TicketCardTransCmd(&g_TpuExitReaderResInfo,&g_TpuExitCardTransCmdInfo,iRtn);  // 下发交易命令
                                                    if(iRtn)
                                                    {
                                                        cout<<"\nAnalysis End ExitReader Send TicketCardTransCmd"<<":"<<iRtn<<"\n"<<endl;
                                                           ReaderAbort(&g_TpuExitReaderResInfo);
                                                     }
                                                    else
                                                    {
                                                        cout<<"\nAnalysis End ExitReader Send TicketCardTransCmd "<<":"<<iRtn<<"\n"<<endl; 
                                                    }
                                                }

                                            }
                                    }
                                    break;
                                    case eTransInOneMinute:  // 一分钟内连续刷卡 没有通行
                                    case  eGetProofTac:   // 重取TAC，防拔处理
                                    case eSameCardInfo:
                                    {
                                        cout<<"\nAnalysis End  SamCard!"<<":"<<iRtn<<"\n"<<endl;
                                        ReaderAbort(&g_TpuExitReaderResInfo);                            
                                    }
                                    break;
                                }                                
                            }
                        }
                        break;
                        case eEvtExit_ok:  // 收到出站指令后的业务处理
                        {
                             g_TpuExitReaderResInfo.bytRecardInfoCmdCode = msginfo.u.reader.m_bytRecardInfoCmdCode;

                            if(msginfo.u.reader.m_msgReaderData[0])
                            {
                                ReaderAbort(&g_TpuExitReaderResInfo); 
                                // 退票
                                g_pMainGDM->GetTpuModule()->TPU_TicketExit();                                
                            }
                            else
                            {   
                                g_pMainGDM->GetTpuModule()->TPU_TicketEntry();                          
                                m_GateContex.SetDisplayType(&g_TpuExitReaderResInfo,&g_TpuExitCardAnalysisInfo,&g_TpuExitCardTransCmdInfo);

                               // 过人
                               g_pMainGDM->GetSnapMaFlap()->AddExitNum(1);
                               cout<<"\nAnalysis End ExitReader g_pMainGDM->GetSnapMaFlap()->AddExitNum(1)\n"<<endl;


                               FillTransResponseData(&g_TpuExitReaderResInfo,&msginfo,&g_TpuExitCardDataBak);                     
                               // 写交易记录
                               BYTE  bytRtn = 0;
                               FillRecord(g_pMainGDM,&g_TpuExitCardTransCmdInfo,&g_TpuExitReaderResInfo,&g_InOutMsgTxnRec,&g_TpuExitCardDataBak,&g_LocalSleInfo,bytRtn);
                               //
                               int iRtn = 0;
                               ModifyRegist(g_pMainGDM,&g_TpuExitReaderResInfo,&g_traceInfo,&g_TpuExitCardAnalysisInfo,g_TpuExitCardDataBak.m_bytCurrentTicketBox,&g_TpuExitCardTransCmdInfo,&g_InOutUpdateSeqSn,iRtn);
                               ReaderAbort(&g_TpuExitReaderResInfo);
                               // collect Ticket

                            }
                        }
                        break;                        
                    }
                    break;
                }
                break;
            }
        }    
        g_pMainGDM->Stop();
	g_pMainGDM->GetUtils()->DebugMsg(&g_traceInfo,"Main Exit!","MonitroParamStatus");

	return iRtn;
}

// ��ʼ���Ĵ��Ż���SNAP
int InitSNAPMATripod()
{
	string  strTempValue;
        BYTE    bytTmp = 0;
	try{
		//g_pMainGDM->GetSnapMaFlap()->->SetGateMode(GATE_MODE_NORMAL);		
		
		strTempValue = g_pMainGDM->GetINIProxy()->ReadStrIni((char*)"MASNAPTripod",(char*)"DevType");
		if(0 == strTempValue.size())
		{
			g_pMainGDM->GetINIProxy()->WriteStrToIni((char*)"MASNAPTripod",(char*)"DevType",(char*)"13");
			return -1;
		}
                g_pMainGDM->GetSnapMaFlap()->SetMCPMode((char*)strTempValue.c_str());
		//g_pMainGDM->GetSnapMaFlap()->(atoi((char*)strTempValue.c_str()));

		strTempValue = g_pMainGDM->GetINIProxy()->ReadStrIni((char*)"MASNAPTripod",(char*)"PORT");
		if(0 == strTempValue.size())
		{
			g_pMainGDM->GetINIProxy()->WriteStrToIni((char*)"MASNAPTripod",(char*)"PORT",(char*)"4");
			return -1;
		}
                
		g_pMainGDM->GetSnapMaFlap()->SetComPort((char*)strTempValue.c_str());

		strTempValue.clear();
		strTempValue = g_pMainGDM->GetINIProxy()->ReadStrIni((char*)"MASNAPTripod",(char*)"BandRate");
		if(0 == strTempValue.size())
		{
			g_pMainGDM->GetINIProxy()->WriteStrToIni((char*)"MASNAPTripod",(char*)"BandRate",(char*)"57600");
			return -2;
		}
		g_pMainGDM->GetSnapMaFlap()->SetComBaudrate((char*)strTempValue.c_str());

		strTempValue.clear();
		strTempValue = g_pMainGDM->GetINIProxy()->ReadStrIni((char*)"MASNAPTripod",(char*)"TripodMaxNum");
		if(0 == strTempValue.size())
		{
			g_pMainGDM->GetINIProxy()->WriteStrToIni((char*)"MASNAPTripod",(char*)"TripodMaxNum",(char*)"15");
			return -2;
		}
                bytTmp = stoi(strTempValue);
		g_pMainGDM->GetSnapMaFlap()->SetMaxNum(bytTmp);


		strTempValue.clear();
		strTempValue = g_pMainGDM->GetINIProxy()->ReadStrIni((char*)"MASNAPTripod",(char*)"TripodOverTime");
		if(0 == strTempValue.size())
		{
			g_pMainGDM->GetINIProxy()->WriteStrToIni((char*)"MASNAPTripod",(char*)"TripodOverTime",(char*)"60");
			return -2;
		}
                bytTmp = stoi(strTempValue);
		g_pMainGDM->GetSnapMaFlap()->SetOverTime(bytTmp);


		//g_pMainGDM->GetSnapMaFlap()->SetParentWnd(g_MainWnd);

		g_pMainGDM->GetSnapMaFlap()->InitComm();
		g_pMainGDM->GetUtils()->DebugMsg(&g_traceInfo,"InitSNAPMATripod","GetSnapMaFlap::InitComm() Start OpenComm! ");


	}
	catch(...)
	{
		return -1;
	}


	return 0;
}

// ��ʼ����վ��д��
int initEntryReader()
{
	string  strTempValue;
	LocalSLEInfo tempLocalInfo ;
	READER_CONFIG  m_configReader;
	IGlobalDataManager * m_pGDM = g_pMainGDM;
        int  iIndex = 0;
printf("initEntryReader:%d\n",iIndex++);
	try{
		m_pGDM->GetParameter()->GetLocalSLEInfo(&tempLocalInfo);
printf("initEntryReader:%d\n",iIndex++);                
		m_configReader.Reset();
		strTempValue = m_pGDM->GetINIProxy()->ReadStrIni((char*)"ENTRYREADERINFO",(char*)"PORT");
		if(0 == strTempValue.size())
		{
			m_pGDM->GetINIProxy()->WriteStrToIni((char*)"ENTRYREADERINFO",(char*)"PORT",(char*)"/dev/ttys0");
			return -1;
		}
		m_configReader.m_strPort = strTempValue;

		strTempValue.clear();
		strTempValue = m_pGDM->GetINIProxy()->ReadStrIni((char*)"ENTRYREADERINFO",(char*)"BandRate");
		if(0 == strTempValue.size())
		{
			m_pGDM->GetINIProxy()->WriteStrToIni((char*)"ENTRYREADERINFO",(char*)"BandRate",(char*)"57600");
			return -2;
		}
		m_configReader.m_strBaudrate = strTempValue;
		m_configReader.eConfigType = eAll;
		m_configReader.m_bIsConnect = false;
		memcpy(m_configReader.m_bytDevSleCode,tempLocalInfo.bytSleId,sizeof(tempLocalInfo.bytSleId));

		m_configReader.m_dwRunningMode = LM_REVENUE;
		m_configReader.m_dwStatus = (eShWorkEntryMode + eIdle );
		// m_configReader.m_hWnd = g_MainWnd;
printf("initEntryReader:%d\n",iIndex++);
		m_pGDM->GetReader()->SetConfigReader(&m_configReader);
printf("initEntryReader:%d\n",iIndex++);
                m_pGDM->GetUtils()->DebugMsg(&g_traceInfo,"configInfo","EntryReader Port:%s,BaundRate:%s,\r\n",m_configReader.m_strPort.c_str(),m_configReader.m_strBaudrate.c_str());
	}
	catch(...)
	{
		m_pGDM->GetUtils()->DebugMsg(&g_traceInfo,"initEntryReader ��ʼ���쳣","�˳�����Ӧ��!");
		return -1;
	}


	return 0;
}

// ��ʼ����վ��д��
int initExitReader()
{
	string  strTempValue;
	LocalSLEInfo tempLocalInfo ;
	READER_CONFIG  m_configReader;
	IGlobalDataManager * m_pGDM = g_pMainGDM;
        int  iIndex = 0;
	try{
printf("initExitReader:%d\n",iIndex++);            
		m_configReader.Reset();
		strTempValue = m_pGDM->GetINIProxy()->ReadStrIni((char*)"EXITREADERINFO",(char*)"PORT");
		if(0 == strTempValue.size())
		{
			m_pGDM->GetINIProxy()->WriteStrToIni((char*)"EXITREADERINFO",(char*)"PORT",(char*)"/dev/ttys1");
			return -1;
		}
		m_configReader.m_strPort = strTempValue;
printf("initExitReader:%d\n",iIndex++);            

		strTempValue.clear();
		strTempValue = m_pGDM->GetINIProxy()->ReadStrIni((char*)"EXITREADERINFO",(char*)"BandRate");
		if(0 == strTempValue.size())
		{
			m_pGDM->GetINIProxy()->WriteStrToIni((char*)"EXITREADERINFO",(char*)"BandRate",(char*)"57600");
			return -2;
		}
		m_configReader.m_strBaudrate = strTempValue;
printf("initExitReader:%d\n",iIndex++);            

		m_pGDM->GetParameter()->GetLocalSLEInfo(&tempLocalInfo);

		m_configReader.eConfigType = eAll;
		m_configReader.m_bIsConnect = false;
		memcpy(m_configReader.m_bytDevSleCode,tempLocalInfo.bytSleId,sizeof(tempLocalInfo.bytSleId));

		m_configReader.m_dwRunningMode = LM_REVENUE;
		m_configReader.m_dwStatus = (eShWorkExitMode + eIdle );
		// m_configReader.m_hWnd = g_MainWnd;

		m_pGDM->GetExitReader()->SetConfigReader(&m_configReader);
printf("initExitReader:%d\n",iIndex++);            
                m_pGDM->GetUtils()->DebugMsg(&g_traceInfo,"configInfo","ExitReader Port:%s,BaundRate:%s,\r\n",m_configReader.m_strPort.c_str(),m_configReader.m_strBaudrate.c_str());

	}
	catch(...)
	{
		m_pGDM->GetUtils()->DebugMsg(&g_traceInfo,"initExitReader ��ʼ���쳣","�˳�����Ӧ��!");
		return -1;
	}


	return 0;
}



