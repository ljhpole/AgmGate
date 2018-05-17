/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include <vector>

#include "shAgm.h"
#include "DevStatusMonitor.h"
CDevStatusMonitor::CDevStatusMonitor()
{
}
void CDevStatusMonitor::WaitForWorkerThreadEnd()
{
	if(!m_thWorker.IsRunning())
		return;

	if(m_thWorker.IsInMyThread())
		m_thWorker.Detatch();
	else
	{
		//m_evStop.Set();
		m_thWorker.Join();
	}
}

BOOL CDevStatusMonitor::CreateWorkerThread()
{
	return m_thWorker.Start(this, &CDevStatusMonitor::WorkerThreadProc,this);
}

int  CDevStatusMonitor::UploadAuditData(eAuditType eAuditTypeInfo)
{
	int iRtn = -1;
	int eLoginAck = 0;
	LocalSLEInfo  localtemp ;
	char localtimetmp[15] = {0},sSleId[9] = {0};
	long  lOperatorIdtmp = 0,lCollectTime = 0;
	TS_6000    stru6000;
	TS_6001    stru6001;
        	TS_5040    stru5040;
	IGlobalDataManager *sm_pGDM = nullptr;
	sm_pGDM = m_pGDM;

	if(NULL == m_pGDM)
	{
		printf("CDevStatusMonitor::UploadAuditData: NULL == m_pGDM");
		return iRtn ;
	}

	localtemp.Reset();
	// 设备节点信息
	sm_pGDM->GetParameter()->GetLocalSLEInfo(&localtemp);
	m_InOutMsgTxnRec.struInMsgRec.sSleId=localtemp.local_SLE_id;
	// 消息时间及最后时间
	sm_pGDM->GetUtils()->getLocalTime("YMDhms",(char*)localtimetmp);
	m_InOutMsgTxnRec.struInMsgRec.sMsgTime.clear();
	m_InOutMsgTxnRec.struInMsgRec.sMsgTime.resize(sizeof(localtimetmp));
	m_InOutMsgTxnRec.struInMsgRec.sMsgTime.assign(localtimetmp,strlen(localtimetmp));
	m_InOutMsgTxnRec.struInMsgRec.sLastTime = m_InOutMsgTxnRec.struInMsgRec.sMsgTime;
	// 发送标记
	m_InOutMsgTxnRec.struInMsgRec.lSendFlag = eSendingFlag;

	// 采集时间
	lOperatorIdtmp = 0;
	lCollectTime = htonl((DWORD)((time(NULL))+8*60*60));
	// 收益寄存器1 6000报文数据
	// 获取Regist6000Sn数据
	memset(&stru6000,0,sizeof(TS_6000));
	m_InOutUpdateSeqSn.eInOutUpdateFlag = eOutFlag;
	m_InOutUpdateSeqSn.eSeqSnType = eBomRegist6000Sn;
	memset(&m_InOutUpdateSeqSn.lBomRegist6000[0],0,sizeof(m_InOutUpdateSeqSn.lBomRegist6000));
	sm_pGDM->GetSQLITEDBManagerProxy()->DBInOutModifySeqSn(&m_InOutUpdateSeqSn);
	// 6000结构体数据填充
	m_InOutMsgTxnRec.eInOutFlag = eInFlag;
	m_InOutMsgTxnRec.eMsgOrTxnFlag = eMsgCodeFlag;
	m_InOutMsgTxnRec.struInMsgRec.eMessageCode = eRegistBom1;
	m_InOutMsgTxnRec.struInMsgRec.lMsgCode = eRegistBom1;
	m_InOutMsgTxnRec.struInMsgRec.lPackBodyLen = sizeof(m_InOutMsgTxnRec.struInMsgRec.uMsgRec.uRegistBom1);

	stru6000.bytAuditType = eAuditTypeInfo;  // 审计类型
	memcpy(stru6000.bytDevID,localtemp.bytSleId,sizeof(localtemp.bytSleId));
	stru6000.bytNodeType = localtemp.bytSleId[2];
	stru6000.bytReserved1 = 0;
	stru6000.lOperatorId = lOperatorIdtmp;
	stru6000.lCollectionTime = lCollectTime;
	memcpy(&stru6000.lRegData[0],&m_InOutUpdateSeqSn.lBomRegist6000[0],sizeof(stru6000.lRegData));
	memcpy(&m_InOutMsgTxnRec.struInMsgRec.uMsgRec.uRegistBom1,&stru6000,sizeof(TS_6000));
	if(nullptr == sm_pGDM->GetMsgComSC())
	{
		sm_pGDM->GetUtils()->DebugMsg(sm_pGDM->GetTracelog(),"nullptr == sm_pGDM->GetMsgComSC()","6000与车站SC断开!");			
		return -2;
	}
	else
	{
		sm_pGDM->GetMsgComSC()->StoreTrans(&(m_InOutMsgTxnRec));
	}

	// 收益寄存器1 5040报文数据  维护寄存器数据获取  ？？？？？？
	// 获取Regist6001Sn数据
	memset(&stru6001,0,sizeof(TS_6001));
        memset(&stru5040,0,sizeof(stru5040));
	m_InOutUpdateSeqSn.eInOutUpdateFlag = eOutFlag;
	m_InOutUpdateSeqSn.eSeqSnType = eBomRegist6001Sn;
	memset(&m_InOutUpdateSeqSn.lBomRegist6001,0,sizeof(m_InOutUpdateSeqSn.lBomRegist6001));
	sm_pGDM->GetSQLITEDBManagerProxy()->DBInOutModifySeqSn(&m_InOutUpdateSeqSn);

	m_InOutMsgTxnRec.eInOutFlag = eInFlag;
	m_InOutMsgTxnRec.eMsgOrTxnFlag = eMsgCodeFlag;
	m_InOutMsgTxnRec.struInMsgRec.eMessageCode = eRegistBom2;
	m_InOutMsgTxnRec.struInMsgRec.lMsgCode = eRegistBom2;

	m_InOutMsgTxnRec.struInMsgRec.lPackBodyLen = sizeof(m_InOutMsgTxnRec.struInMsgRec.uMsgRec.uRegistBom2);

	

        stru5040.bytAuditType = eAuditTypeInfo;
        memcpy(stru5040.bytDevID,localtemp.bytSleId,sizeof(localtemp.bytSleId));
        stru5040.lCollectionTime = lCollectTime;
        memcpy(&stru5040.lRegData[0],&m_InOutUpdateSeqSn.lBomRegist6001[0],sizeof(stru5040.lRegData));
        memcpy(&m_InOutMsgTxnRec.struInMsgRec.uMsgRec.uRegistBom2,&stru5040,sizeof(TS_5040));
        
	if(nullptr == sm_pGDM->GetMsgComSC())
	{
		sm_pGDM->GetUtils()->DebugMsg(sm_pGDM->GetTracelog(),"nullptr == sm_pGDM->GetMsgComSC()","6001,与车站SC断开!");
		return -3;
	}
	else
	{
		sm_pGDM->GetMsgComSC()->StoreTrans(&(m_InOutMsgTxnRec));
	}
        
        if(eTicketBoxOut == eAuditTypeInfo)
        {
            iRtn = 0;            
            m_InOutUpdateSeqSn.eInOutUpdateFlag = eOutFlag;
            m_InOutUpdateSeqSn.eSeqSnType = eBomRegist6001Sn;
            memset(&m_InOutUpdateSeqSn.lBomRegist6001[0],0,sizeof(m_InOutUpdateSeqSn.lBomRegist6001));
            iRtn = sm_pGDM->GetSQLITEDBManagerProxy()->DBInOutModifySeqSn(&m_InOutUpdateSeqSn);
            if(0 != iRtn)
            {
                return -4;
            }
            m_InOutUpdateSeqSn.lBomRegist6001[0] = 0;
            m_InOutUpdateSeqSn.lBomRegist6001[1] = 0;
            m_InOutUpdateSeqSn.lBomRegist6001[2] = 0;
            m_InOutUpdateSeqSn.eInOutUpdateFlag = eUpdateFlag;
            iRtn = sm_pGDM->GetSQLITEDBManagerProxy()->DBInOutModifySeqSn(&m_InOutUpdateSeqSn);
            if(0 != iRtn)
            {
                return -5;
            }                        
        }
        
        
	iRtn = 0;
	return iRtn;
}
void CDevStatusMonitor::FillDevEvent(eShBomDevEventCode eEventCode,BOOL bisOn,BYTE *bytOut)
{
	BYTE bytEvent[2] = {0};
	uint16_t  wTmp = eEventCode;
	wTmp = htons(wTmp);
	memcpy(bytEvent,&wTmp,sizeof(bytEvent));
	bisOn?(BIT_SET(bytEvent[0],7)):(BIT_CLEAR(bytEvent[0],7));
	memcpy(bytOut,bytEvent,2);
	return ;
}


int  CDevStatusMonitor::UploadDevStatusEvent(DEV_STATUS_EVENT_REC *tInStatusEvent)
{
	int  iRtn = -1;
	long  lHappenTime = 0;

	if(NULL == m_pGDM)
	{
		m_pGDM->GetUtils()->DebugMsg(m_pGDM->GetTracelog(),"CDevStatusMonitor::UploadDevStatusEvent","NULL == m_pGDM");
		return -2 ;
	}
//	if(m_pGDM->GetMsgComSC()->getState())
//	{
//		m_pGDM->GetUtils()->DebugMsg(m_pGDM->GetTracelog(),"CDevStatusMonitor::UploadDevStatusEvent","5041状态信息与车站SC断开!");
//		return -3;
//	}
	else
	{
		// 5041结构体数据填充
		m_InOutMsgTxnRec.eInOutFlag = eInFlag;
		m_InOutMsgTxnRec.eMsgOrTxnFlag = eMsgCodeFlag;
		m_InOutMsgTxnRec.struInMsgRec.eMessageCode = eDevStatusCode;
		m_InOutMsgTxnRec.struInMsgRec.lMsgCode = eDevStatusCode;
		m_InOutMsgTxnRec.struInMsgRec.lPackBodyLen = sizeof(m_InOutMsgTxnRec.struInMsgRec.uMsgRec.uDevStatusRec);
		lHappenTime= time(NULL) + 8*60*60;
		tInStatusEvent->lHappenedTime  = htonl(lHappenTime);
                memcpy(tInStatusEvent->bytDevID,m_pGDM->GetParameter()->get_shcurparam()->m_ParamLocalInfo.bytSleId,4);
                tInStatusEvent->bytNodeType = tInStatusEvent->bytDevID[2];
		m_InOutMsgTxnRec.struInMsgRec.uMsgRec.uDevStatusRec = *tInStatusEvent;
		m_pGDM->GetMsgComSC()->StoreTrans(&(m_InOutMsgTxnRec));
                
                printf("tInStatusEvent->bytDevID:%02x%02x%02x%02x,lHappenedTime=%d",tInStatusEvent->bytDevID[0],tInStatusEvent->bytDevID[1],\
                        tInStatusEvent->bytDevID[2],tInStatusEvent->bytDevID[3],tInStatusEvent->lHappenedTime );
		iRtn = 0;
		// Sleep(200);
	}

	return iRtn;
}


void  CDevStatusMonitor::InitDevStatus()
{
	// 设置事件4
	memset(m_DevStatusEvent.uMachineEventStatus.bytDevStatus,0,2);
	FillDevEvent(eMachineNoInit,TRUE,m_DevStatusEvent.uMachineEventStatus.bytDevStatus+2);
	UploadDevStatusEvent(&m_DevStatusEvent);
	sleep(1);
	//清除事件4
	FillDevEvent(eMachineNoInit,FALSE,m_DevStatusEvent.uMachineEventStatus.bytDevStatus+2);
	UploadDevStatusEvent(&m_DevStatusEvent);
	sleep(1);
	// 发送事件71
	m_DevStatusEvent.uMachineEventStatus.StatusEvent.bytMachine0IsOnBit0 = 1;
	FillDevEvent(eDevStartService,TRUE,m_DevStatusEvent.uMachineEventStatus.bytDevStatus+2);
	UploadDevStatusEvent(&m_DevStatusEvent);
	sleep(1);
        UploadDevModuleInfo();
        //
        	sleep(1);
        UploadAuditData(eAuditInit);
}

int CDevStatusMonitor::ProcessStartAndStopService()
{
	time_t NowTimeOfDay = (time(NULL) + (8 * 3600)) / (24 * 60 * 60) * (24 * 3600) ;
	time_t NowTime = time(NULL) + (8 * 3600);
	time_t StartTime =  NowTimeOfDay + (m_pGDM->GetParameter()->get_shcurparam()->m_st3002.sstTime.bytStartServT * 15 * 60);
	time_t StopTime = NowTimeOfDay + (m_pGDM->GetParameter()->get_shcurparam()->m_st3002.sstTime.bytStopServT * 15 * 60);
	BOOL IsOperatingTime = FALSE;
	if (StartTime < StopTime)
	{
		IsOperatingTime = (NowTime >= StartTime) && (NowTime < StopTime);
	}
	else
	{
		IsOperatingTime = (NowTime >= StartTime) || (NowTime < StopTime);
	}

	if (IsOperatingTime)
	{
		if (!m_ServiceStarted)
		{
			BYTE bytCloseFlag = eOpenCmd;
			SendMessage(WM_SYSCMD_INFORM_MAIN_PROCESS,(WPARAM)&bytCloseFlag,NULL);
			m_ServiceStarted = TRUE;
                        UploadAuditData(eStartService);
		}
	}
	else
	{
		if (m_ServiceStarted)
		{
			BYTE bytCloseFlag = eCloseCmd;
			SendMessage(WM_SYSCMD_INFORM_MAIN_PROCESS,(WPARAM)&bytCloseFlag,NULL);
			m_ServiceStarted = FALSE;
                        UploadAuditData(eStopService);
		}
	}
        return 0;
}

int CDevStatusMonitor::ProcessSysCommandFromSc()
{
    WORD wIndex = 0;
    LocalSLEInfo *pTr = NULL;
	pTr = &(m_pGDM->GetParameter()->get_shcurparam()->m_ParamLocalInfo);
	BYTE *p = NULL;
	char Ip[20] = {0};
	string Path = "";
	for(wIndex = 0; wIndex < pTr->vSysCommandCode.size();wIndex++)
	{
		//m_pGDM->GetUtils()->DebugMsg(&writelog,"CBOMMainDlg::ProcessSysCommandFromSc","wIndex:%d,Value:%d!",wIndex,pTr->m_bitsAcceptSysCmd[wIndex]);

		switch(wIndex)
		{
		case eUploadRegistData:
		case eUploadRegistAuditData:
			{
				if(pTr->m_bitsAcceptSysCmd[eUploadRegistAuditData] || pTr->m_bitsAcceptSysCmd[eUploadRegistData])
				{					
					if(pTr->m_bitsAcceptSysCmd[eUploadRegistAuditData])
						pTr->m_bitsAcceptSysCmd.reset(eUploadRegistAuditData);
					if(pTr->m_bitsAcceptSysCmd[eUploadRegistData])
						pTr->m_bitsAcceptSysCmd.reset(eUploadRegistData);
					m_pGDM->GetUtils()->DebugMsg(m_pGDM->GetTracelog(),"CDevStatusMonitor::ProcessSysCommandFromSc","Upload RegistData!");
					UploadAuditData(eAuditRequest);

				}
			}
			break;
		case eOpenCmd:
			{
				if(pTr->m_bitsAcceptSysCmd[eOpenCmd])
				{
					if(pTr->m_bitsAcceptSysCmd[eOtherCmd])
					{
						m_pGDM->GetUtils()->DebugMsg(m_pGDM->GetTracelog(),"CDevStatusMonitor::ProcessSysCommandFromSc","Receive Open Command!");
						pTr->m_bitsAcceptSysCmd.reset(eCloseCmd);
						pTr->m_bitsAcceptSysCmd.reset(eOtherCmd);
                                                m_DevStatusEvent.uMachineEventStatus.StatusEvent.bytMachine0IsOnBit0 = 1;
                                                m_DevStatusEvent.uMachineEventStatus.StatusEvent.bytMachine0IsStopBit1 = 0;
						BYTE bytCloseFlag = eOpenCmd;
					FillDevEvent(eDevClose,false,m_DevStatusEvent.uMachineEventStatus.bytDevStatus+2);
					UploadDevStatusEvent(&m_DevStatusEvent);
                                                m_GateContexpTr->SetGateRunningMode(LM_REVENUE);
                                        }
				}
			}
			break;
		case eCloseCmd:
			{
				if(pTr->m_bitsAcceptSysCmd[eCloseCmd])
				{
					if(pTr->m_bitsAcceptSysCmd[eOtherCmd+1])
					{
						m_pGDM->GetUtils()->DebugMsg(m_pGDM->GetTracelog(),"CDevStatusMonitor::ProcessSysCommandFromSc","收到关闭命令");					
						pTr->m_bitsAcceptSysCmd.reset(eOpenCmd);
						pTr->m_bitsAcceptSysCmd.reset(eOtherCmd+1);
						BYTE bytCloseFlag = eCloseCmd;
                                        m_DevStatusEvent.uMachineEventStatus.StatusEvent.bytMachine0IsOnBit0 = 0;
                                        m_DevStatusEvent.uMachineEventStatus.StatusEvent.bytMachine0IsStopBit1 = 1;        
					FillDevEvent(eDevClose,TRUE,m_DevStatusEvent.uMachineEventStatus.bytDevStatus+2);
					UploadDevStatusEvent(&m_DevStatusEvent);
                                                
                                                m_GateContexpTr->SetGateRunningMode(LM_CLOSE);
					}
				}
			}
			break;
		case eDelaySendBegin:
			{
				if(pTr->m_bitsAcceptSysCmd[eDelaySendBegin])
				{
					m_pGDM->GetUtils()->DebugMsg(m_pGDM->GetTracelog(),"CDevStatusMonitor::ProcessSysCommandFromSc","收到延迟发送命令");
					pTr->m_bitsAcceptSysCmd.reset(eDelaySendEnd);
				}
			}
			break;
		case eDelaySendEnd:
			{
				if(pTr->m_bitsAcceptSysCmd[eDelaySendEnd])
				{
					m_pGDM->GetUtils()->DebugMsg(m_pGDM->GetTracelog(),"CDevStatusMonitor::ProcessSysCommandFromSc","收到取消延迟发送命令");
					pTr->m_bitsAcceptSysCmd.reset(eDelaySendBegin);
				}
			}
			break;
		case  eUploadStatus:
			{
				if(pTr->m_bitsAcceptSysCmd[eUploadStatus])
				{
					m_pGDM->GetUtils()->DebugMsg(m_pGDM->GetTracelog(),"CDevStatusMonitor::ProcessSysCommandFromSc","收到获取状态命令");
					FillDevEvent(eStatusSyncError,TRUE,m_DevStatusEvent.uMachineEventStatus.bytDevStatus+2);
					UploadDevStatusEvent(&m_DevStatusEvent);
					//sleep(1);
					pTr->m_bitsAcceptSysCmd.reset(eUploadStatus);
				}

			}
			break;
		case eDownSoftWave:
			if(pTr->m_bitsAcceptSysCmd[eDownSoftWave])
			{

				m_pGDM->GetUtils()->DebugMsg(m_pGDM->GetTracelog(),"CDevStatusMonitor::ProcessSysCommandFromSc","软件下发开始");
				// pTr->m_bitsAcceptSysCmd.reset(eDownSoftWave);
				m_pGDM->GetUtils()->DebugMsg(m_pGDM->GetTracelog(),"CDevStatusMonitor::ProcessSysCommandFromSc","软件下发结束");
			}
			break;
		case eUseMainIp:
			if(pTr->m_bitsAcceptSysCmd[eUseMainIp])
			{
				// m_pGDM->GetMsgComSC()->Close();
				//m_pGDM->GetMsgComSC()->SetMsgTxnConfigParam(TRUE);
				pTr->m_bitsAcceptSysCmd.reset(eUseMainIp);
				m_pGDM->GetUtils()->DebugMsg(m_pGDM->GetTracelog(),"CDevStatusMonitor::ProcessSysCommandFromSc","切换主IP");
			}
			break;
		case eUseSecondIp:
			if(pTr->m_bitsAcceptSysCmd[eUseSecondIp])
			{
				// m_pGDM->GetMsgComSC()->Close();
				//m_pGDM->GetMsgComSC()->SetMsgTxnConfigParam(FALSE);
				pTr->m_bitsAcceptSysCmd.reset(eUseSecondIp);
				m_pGDM->GetUtils()->DebugMsg(m_pGDM->GetTracelog(),"CDevStatusMonitor::ProcessSysCommandFromSc","切换备IP");
			}
			break;
                    case eBiEntryMode:
                    {
			if(pTr->bytSleId[2] == 0x1B && pTr->m_bitsAcceptSysCmd[eBiEntryMode])
			{
				pTr->m_bitsAcceptSysCmd.reset(eBiEntryMode);
				m_pGDM->GetUtils()->DebugMsg(m_pGDM->GetTracelog(),"CDevStatusMonitor::ProcessSysCommandFromSc","Gate --> EnGate");
                                m_DevStatusEvent.uMachineEventStatus.StatusEvent.bytMachine1IsBOMEFOBit0 = 0;
                                m_DevStatusEvent.uMachineEventStatus.StatusEvent.bytMachine1IsLoginBit1 = 0;
                                FillDevEvent(eCheckInMode,TRUE,m_DevStatusEvent.uMachineEventStatus.bytDevStatus+2);
				UploadDevStatusEvent(&m_DevStatusEvent);
				// sleep(1);
				FillDevEvent(eCheckOutMode,FALSE,m_DevStatusEvent.uMachineEventStatus.bytDevStatus+2);
				UploadDevStatusEvent(&m_DevStatusEvent);
                                // m_GateContexpTr->m_sGateWorkMode = ;
                                m_GateContexpTr->SetGateWorkModeValue(eBien);
                        }                        
                    }
                    break;
                    case eBiExitMode:
                    {
			if(pTr->bytSleId[2] == 0x1B && pTr->m_bitsAcceptSysCmd[eBiExitMode])
			{
				pTr->m_bitsAcceptSysCmd.reset(eBiExitMode);
				m_pGDM->GetUtils()->DebugMsg(m_pGDM->GetTracelog(),"CDevStatusMonitor::ProcessSysCommandFromSc","Gate --> ExGate");
                                m_DevStatusEvent.uMachineEventStatus.StatusEvent.bytMachine1IsBOMEFOBit0 = 1;
                                m_DevStatusEvent.uMachineEventStatus.StatusEvent.bytMachine1IsLoginBit1 = 0;                                
                                FillDevEvent(eCheckOutMode,TRUE,m_DevStatusEvent.uMachineEventStatus.bytDevStatus+2);
				UploadDevStatusEvent(&m_DevStatusEvent);
				// sleep(1);
				FillDevEvent(eCheckInMode,FALSE,m_DevStatusEvent.uMachineEventStatus.bytDevStatus+2);
				UploadDevStatusEvent(&m_DevStatusEvent);
                                m_GateContexpTr->SetGateWorkModeValue(eBiex);                                
                        }                         
                    }
                    break;
                    case eBiDirMode:  // always close mode
                    {
			if(pTr->bytSleId[2] == 0x1B && pTr->m_bitsAcceptSysCmd[eBiDirMode])
			{
				pTr->m_bitsAcceptSysCmd.reset(eBiDirMode);
                                //m_DevStatusEvent.uMachineEventStatus.StatusEvent.bytMachine1IsBOMEFOBit0 = 1;
                                m_DevStatusEvent.uMachineEventStatus.StatusEvent.bytMachine1IsLoginBit1 = 1;  
                                FillDevEvent(eNormal,TRUE,m_DevStatusEvent.uMachineEventStatus.bytDevStatus+2);
				UploadDevStatusEvent(&m_DevStatusEvent);
				// sleep(1);
//				FillDevEvent(eCheckInMode,TRUE,m_DevStatusEvent.uMachineEventStatus.bytDevStatus+2);
//				UploadDevStatusEvent(&m_DevStatusEvent);                                
				m_pGDM->GetUtils()->DebugMsg(m_pGDM->GetTracelog(),"CDevStatusMonitor::ProcessSysCommandFromSc","Gate --> BiGate");
                                m_GateContexpTr->SetGateWorkModeValue(eBi);			
                        }                         
                    }
                    break;
                    case eFlapOpen:  // always open mode
                    {
			if(pTr->m_bitsAcceptSysCmd[eFlapOpen])
			{
				pTr->m_bitsAcceptSysCmd.reset(eFlapOpen);
                                m_pGDM->GetSnapMaFlap()->SetMCPMode("23");
				m_pGDM->GetUtils()->DebugMsg(m_pGDM->GetTracelog(),"CDevStatusMonitor::ProcessSysCommandFromSc","Gate - Flap - Open!");
			}                         
                    }
                    break;
                    case eFlapClose:
                    {
			if(pTr->m_bitsAcceptSysCmd[eFlapClose])
			{
				pTr->m_bitsAcceptSysCmd.reset(eFlapClose);
                                m_pGDM->GetSnapMaFlap()->SetMCPMode("13");                                                                
				m_pGDM->GetUtils()->DebugMsg(m_pGDM->GetTracelog(),"CDevStatusMonitor::ProcessSysCommandFromSc","Gate - Flap - Close!");
			}                          
                    }
                    break;
		default:
			{
				if(pTr->m_bitsAcceptSysCmd[wIndex])
				{
					pTr->m_bitsAcceptSysCmd.reset(wIndex);
				}
			}
		}
	}
    return 0;
}

// bytAction 1:取出，2：放回，3:满，4：卡票
//bytTicketBoxId： 1:front,2:behind
void CDevStatusMonitor::TicketBoxIsOut(BYTE bytAction,BYTE bytTicketBoxId)
{
    int iRtn = -1;
    if(0x1 == bytAction)  // 取出票盒
    {
        switch(bytTicketBoxId)
        {
            case 1:
            {
                FillDevEvent(eBox1TakenOut,TRUE,m_DevStatusEvent.uMachineEventStatus.bytDevStatus+2);
		UploadDevStatusEvent(&m_DevStatusEvent);
                
                FillDevEvent(eBoxes1Full,false,m_DevStatusEvent.uMachineEventStatus.bytDevStatus+2);
		UploadDevStatusEvent(&m_DevStatusEvent);  
                
                iRtn = UploadAuditData(eTicketBoxOut);
                
                printf("\n@@@@@@@@@@@@@@@@@@@@@@@CDevStatusMonitor::TicketBoxIsOut,UploadAuditData(eTicketBoxOut):iRtn=%d\n",iRtn);
            }
            break;
            case 2:
            {
                FillDevEvent(eBox2TakenOut,TRUE,m_DevStatusEvent.uMachineEventStatus.bytDevStatus+2);
		UploadDevStatusEvent(&m_DevStatusEvent); 
                
                FillDevEvent(eBoxes2Full,false,m_DevStatusEvent.uMachineEventStatus.bytDevStatus+2);
		UploadDevStatusEvent(&m_DevStatusEvent);  
                
                UploadAuditData(eTicketBoxOut);
            }
            break;
        }
    }
    else if(0x2 == bytAction) // 放回票盒
    {
        switch(bytTicketBoxId)
        {
            case 1:
            {
                FillDevEvent(eBox1TakenOut,FALSE,m_DevStatusEvent.uMachineEventStatus.bytDevStatus+2);
		UploadDevStatusEvent(&m_DevStatusEvent);
            }
            break;
            case 2:
            {
                FillDevEvent(eBox2TakenOut,FALSE,m_DevStatusEvent.uMachineEventStatus.bytDevStatus+2);
		UploadDevStatusEvent(&m_DevStatusEvent);   
            }
            break;
        }       
    }
    else if(0x3 == bytAction)  // 满
    {
        switch(bytTicketBoxId)
        {
            case 1:
            {
                FillDevEvent(eBoxes1Full,TRUE,m_DevStatusEvent.uMachineEventStatus.bytDevStatus+2);
		UploadDevStatusEvent(&m_DevStatusEvent);
            }
            break;
            case 2:
            {
                FillDevEvent(eBoxes2Full,TRUE,m_DevStatusEvent.uMachineEventStatus.bytDevStatus+2);
		UploadDevStatusEvent(&m_DevStatusEvent);   
            }
            break;
        }       
    }
    else if(0x4 == bytAction)  // 卡票
    {
        switch(bytTicketBoxId)
        {
            case 1:
            {
                FillDevEvent(ePileTicketAgent1Stuck,TRUE,m_DevStatusEvent.uMachineEventStatus.bytDevStatus+2);
		UploadDevStatusEvent(&m_DevStatusEvent);
            }
            break;
            case 2:
            {
                FillDevEvent(ePileTicketAgent2Stuck,TRUE,m_DevStatusEvent.uMachineEventStatus.bytDevStatus+2);
		UploadDevStatusEvent(&m_DevStatusEvent);   
            }
            break;
        }       
    }
}
void CDevStatusMonitor::UploadSysModeAuditData(BYTE bytIndex,bool bIsOn)
{
	switch(bytIndex)
	{
            case  0:	//return "紧急模式";
            {
                if(bIsOn)
                {
                    UploadAuditData(eEmergencySet);
                }
                else
                {
                    UploadAuditData(eEmergencyCancel);                
                } 
                ProcessEmergencyMode(bIsOn);
            }
            break;
            case 1://return "进出站免检";
            {
                if(bIsOn)
                {
                    UploadAuditData(eEnExitSetMJ);
                }
                else
                {
                    UploadAuditData(eEnExCancelMj);                
                }              
            }
            break;
            case 2://return "日期免检";
            case 3://return "时间免检";
            {
                if(bIsOn)
                {
                    UploadAuditData(eDateTimeSetMj);
                }
                else
                {
                    UploadAuditData(eDateTimeCancelMj);                
                }  
            }
            break;
            case 4://return "列车故障";
            {
                if(bIsOn)
                {
                    UploadAuditData(eTrainSetMj);
                }
                else
                {
                    UploadAuditData(eTrainCancelMj);                
                }            
            }
            break;
            case 5:   //return "超程免检";
            {
                if(bIsOn)
                {
                    UploadAuditData(eOverRunSetMj);
                }
                else
                {
                    UploadAuditData(eOverRunCancelMj);                
                }
            }
            break;
            default:
                    break; //return "";
            }
}
int  CDevStatusMonitor::ProcessSysModeFromSc()
{
	string  strTmp = "";
        LocalSLEInfo m_LocalSleInfo;
	if(NULL == m_pGDM)
	{
		m_pGDM->GetUtils()->DebugMsg(m_pGDM->GetTracelog(),"CBOMMainDlg::ProcessSysModeFromSc","NULL == m_pGDM");
		return 0;
	}
	m_pGDM->GetParameter()->GetLocalSLEInfo(&m_LocalSleInfo);
	if(m_bitsMainSysCurMode != m_LocalSleInfo.m_bitsCurMode)
	{
		for(BYTE i = 0; i < 8; i++)
		{
			
			ModifyMachineStatus(i,m_LocalSleInfo.m_bitsCurMode[i]);
			if(m_bitsMainSysCurMode[i] != m_LocalSleInfo.m_bitsCurMode[i])
			{
				m_pGDM->GetUtils()->DebugMsg(m_pGDM->GetTracelog(),"CBOMMainDlg::ProcessSysModeFromSc","当前模式：%s",strTmp.c_str());
				UploadDevStatusEvent(&m_DevStatusEvent);
				// sleep(1);
                                UploadSysModeAuditData(i,m_LocalSleInfo.m_bitsCurMode[i]);
			}
                        
                        
			m_bitsMainSysCurMode[i] = m_LocalSleInfo.m_bitsCurMode[i];

		}
	}

}

uint8_t CDevStatusMonitor::ProcessDeviceRunningMode()
{
    uint8_t  bytMode = LM_MODE_NOCHANGE,bytCurMode = LM_MODE_NOCHANGE;
    bytCurMode = m_GateContexpTr->GetGateRunningMode();
    
    
    bool bStatus = false;
    //从实际模块中获取状态，并且改写BOMstatus状态值，
    // Flap Status   outofservice
    
    
    //EntryReaderStatus
    bStatus = m_pGDM->GetReader()->GetCommState();
    if(m_bIsEntryReaderStatusFlag != bStatus)
    {
        if(!bStatus)
        {
            FillDevEvent(eReaderFailure,TRUE,m_DevStatusEvent.uMachineEventStatus.bytDevStatus+2);
            UploadDevStatusEvent(&m_DevStatusEvent);
        }

        if(bStatus)
        {
            FillDevEvent(eReaderFailure,FALSE,m_DevStatusEvent.uMachineEventStatus.bytDevStatus+2);
            UploadDevStatusEvent(&m_DevStatusEvent);                                
        }
        m_bIsEntryReaderStatusFlag = bStatus;
    }
    // ExitReaderStatus
    
    // Tpu and CollectReader Status
    
    
    //SC通讯连接状态
    bStatus = m_pGDM->GetMsgComSC()->getState();
    
    
    // Set Running Mode
    if(bytMode !=  m_GateContexpTr->GetGateRunningMode())
        m_GateContexpTr->SetGateRunningMode(bytMode);
    
    
}
void CDevStatusMonitor::ProcessEmergencyMode(bool bIsOn)
{
    if(bIsOn)
    {
        printf(" \n #########################EmergencyMode Set!\n");
        m_GateContexpTr->SetGateRunningMode(LM_EMERGENCY);
        m_pGDM->GetSnapMaFlap()->SetMCPMode("09");
        m_pGDM->GetIoModule()->TopLightControlControl(eTopLightRedOn);
        
    }
    else
    {
        printf(" \n #########################EmergencyMode Reset!\n") 
        m_GateContexpTr->SetGateRunningMode(LM_REVENUE);
        m_pGDM->GetSnapMaFlap()->SetMCPMode("13");
        m_pGDM->GetIoModule()->TopLightControlControl(eTopLightRedClose);
    }
}
void CDevStatusMonitor::UploadDevModuleInfo()
{
	m_InOutMsgTxnRec.eInOutFlag = eInFlag;
	m_InOutMsgTxnRec.eMsgOrTxnFlag = eMsgCodeFlag;
	m_InOutMsgTxnRec.struInMsgRec.eMessageCode = eDevModuleCode;
        
        IN_MSG_REC     InTmpData;
        DEV_MODULE_REC  tmp;
        tmp.bytManufacturersCode = 0x12;
        tmp.bytModuleType = 0x1C;
        InTmpData.eMessageCode = eDevModuleCode;
        memcpy(&InTmpData.uMsgRec.uDevModuleRec,&tmp,sizeof(DEV_MODULE_REC));
        m_InOutMsgTxnRec.struOutMsgRec.vMsgRec.push_back(InTmpData);
//	m_InOutMsgTxnRec.struInMsgRec.lMsgCode = eDevModuleCode;
//	m_InOutMsgTxnRec.struInMsgRec.lPackBodyLen = sizeof(m_InOutMsgTxnRec.struInMsgRec.uMsgRec.uDevModuleRec);
//	m_InOutMsgTxnRec.struInMsgRec.uMsgRec.uDevModuleRec.bytManufacturersCode = 0x12;//盛博
//	m_InOutMsgTxnRec.struInMsgRec.uMsgRec.uDevModuleRec.bytModuleType = 0x1C;//工控机
	m_pGDM->GetMsgComSC()->StoreTrans(&(m_InOutMsgTxnRec));
}
void  CDevStatusMonitor::ModifyMachineStatus(BYTE bytIndex,BOOL bIsSet)
{
	switch(bytIndex)
	{
	case  0:
		{
                        m_DevStatusEvent.uMachineEventStatus.StatusEvent.bytMachine0IsStopBit1 = bIsSet;  
			m_DevStatusEvent.uMachineEventStatus.StatusEvent.bytMachine1IsEmergencyBit7 = bIsSet;
			FillDevEvent(eDevEmergencyOpen,bIsSet,m_DevStatusEvent.uMachineEventStatus.bytDevStatus+2);
		}
		break;
	case 1:
		{
			m_DevStatusEvent.uMachineEventStatus.StatusEvent.bytMachine0IsEnExitNoCheckBit4 = bIsSet;
			FillDevEvent(eDevEnExNoCheck,bIsSet,m_DevStatusEvent.uMachineEventStatus.bytDevStatus+2);
		}
		break;
	case 2:
		{
			m_DevStatusEvent.uMachineEventStatus.StatusEvent.bytMachine0IsDataNoCheckBit3 = bIsSet;
			FillDevEvent(eDevDateTimeNoCheck,bIsSet,m_DevStatusEvent.uMachineEventStatus.bytDevStatus+2);
		}
		break;
	case 3:
		{
			m_DevStatusEvent.uMachineEventStatus.StatusEvent.bytMachine1IsTimeNoCheckBit2 = bIsSet;
			FillDevEvent(eDevDateTimeNoCheck,bIsSet,m_DevStatusEvent.uMachineEventStatus.bytDevStatus+2);
		}
		break;
	case 4:
		{
			m_DevStatusEvent.uMachineEventStatus.StatusEvent.bytMachine1IsTrainErrorBit4 = bIsSet;
			FillDevEvent(eDevTrainError,bIsSet,m_DevStatusEvent.uMachineEventStatus.bytDevStatus+2);		
		}
		break;
	case 5:
		{
			m_DevStatusEvent.uMachineEventStatus.StatusEvent.bytMachine0NoFreeNoCheckdBit6 = bIsSet;
			FillDevEvent(eDevNoFreeNoCheck,bIsSet,m_DevStatusEvent.uMachineEventStatus.bytDevStatus+2);		
		}
		break;
        case 9:  // 单向模式与双向模式
                {
//			m_DevStatusEvent.uMachineEventStatus.StatusEvent.bytMachine1IsBOMEFOBit0 = bIsSet;
//			FillDevEvent(eDevNoFreeNoCheck,bIsSet,m_DevStatusEvent.uMachineEventStatus.bytDevStatus+2);		
                }
                break;
	}
}

uint8_t CDevStatusMonitor::Start(IGlobalDataManager *pTrIn,CGateContex *pTrGateContex)
{
    if(NULL == pTrIn || NULL == pTrGateContex)
    {
        printf("!!!!!!!!!!!!!!!!!!!!!!!!CDevStatusMonitor::Start : NULL == pTrIn || NULL == pTrGateContex\n");
    }
    m_pGDM = pTrIn;
    m_GateContexpTr = pTrGateContex;
    CreateWorkerThread();
    return 0;
}
uint8_t CDevStatusMonitor::Stop()
{
    return 0;
}

void CDevStatusMonitor::SendMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
    return ;
}
UINT WINAPI CDevStatusMonitor::WorkerThreadProc(LPVOID lpParam)
{
	TRACE("---------------> Client Status Monitor Thread 0x%08X started <---------------", SELF_THREAD_ID);
	CDevStatusMonitor* pThis = (CDevStatusMonitor*)lpParam;
	IGlobalDataManager *pGDM = pThis->m_pGDM;
	pThis->m_bThreadStatusMonitorRun = 1;
	int temp=0,iRtn = -1;
	static bool  bIsInit =  FALSE;
        
	
	while(pThis->m_bThreadStatusMonitorRun)
	{	
//		try
//		{
                    pGDM->GetUtils()->select_sleep(3);
                    if(!bIsInit)
                    {
                        bIsInit = true;
                        InitDevStatus();
                    }

			if(++temp % 10 != 0)
			{
				pThis->ProcessSysCommandFromSc();
				continue;
			}

			pThis->ProcessSysModeFromSc();
			pThis->ProcessStartAndStopService();

                        pThis->ProcessDeviceRunningMode();

		//}
//		catch (exception* e)
//		{
//                    string sException(e->what()) ;
////                    sException = 
//                    printf("Client Worker Thread exception:%s",sException.c_str());
//			//TRACE(sException.c_str());
//			delete e;
//		}
        }
        TRACE("---------------> Client Worker Thread 0x%08X stoped <---------------", SELF_THREAD_ID);

	return 0;
}



