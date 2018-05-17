/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   DevStatusMonitor.h
 * Author: ljhpole
 *
 * Created on April 17, 2018, 8:21 PM
 */

#ifndef DEVSTATUSMONITOR_H
#define DEVSTATUSMONITOR_H


class CDevStatusMonitor
{
private:
    bool CreateWorkerThread();
    void WaitForWorkerThreadEnd();    
   UINT WINAPI WorkerThreadProc(LPVOID pv);
   void SendMessage(UINT message, WPARAM wParam, LPARAM lParam);
private:

    CThread<CDevStatusMonitor, VOID, UINT>  m_thWorker;
public:
    bool m_bThreadStatusMonitorRun;
    IGlobalDataManager *m_pGDM;
    IN_OUT_MSG_TXN_REC m_InOutMsgTxnRec;
    IN_OUT_UPDATE_SEQ_SN m_InOutUpdateSeqSn;
    DEV_STATUS_EVENT_REC   m_DevStatusEvent;
    BOOL m_ServiceStarted;
    std::bitset<BOM_MODE_BITES_SIZE> m_bitsMainSysCurMode;
    string   m_GateWorkMode;
    CGateContex  *m_GateContexpTr;
    
    bool    m_bIsEntryReaderStatusFlag;
    bool    m_bIsExitReaderStatusFlag;
    bool    m_bIsExitColecteReaderStatusFlag;
public:
    CDevStatusMonitor();    
    uint8_t Start(IGlobalDataManager *pTrIn,CGateContex *pTrGateContex);
    uint8_t Stop();
    int ProcessSysCommandFromSc();
    int ProcessSysModeFromSc();
    int ProcessStartAndStopService();
    int  UploadAuditData(eAuditType eAuditTypeInfo);
    // 设备状态及事件成员方法
    int  UploadDevStatusEvent(DEV_STATUS_EVENT_REC *tInStatusEvent);
    void FillDevEvent(eShBomDevEventCode eEventCode,BOOL bisOn,BYTE *bytOut);
    void  InitDevStatus();
    void  ModifyMachineStatus(BYTE bytIndex,BOOL bIsSet);
    void UploadDevModuleInfo();
    void UploadSysModeAuditData(BYTE bytIndex,bool bIsOn);
    
    void TicketBoxIsOut(BYTE bytAction,BYTE bytTicketBoxId); // bytAction 1:取出，2：放回，bytTicketBoxId： 1:front,2:behind
    void ProcessEmergencyMode(bool bIsOn);
    uint8_t ProcessDeviceRunningMode();
};

#endif /* DEVSTATUSMONITOR_H */

