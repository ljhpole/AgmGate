/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   GateContext.h
 * Author: ljhpole
 *
 * Created on April 25, 2018, 12:26 AM
 */

#ifndef GATECONTEXT_H
#define GATECONTEXT_H

class CGateContex
{
    DECLARE_SINGLETON(CGateContex);

private:
    UINT WINAPI WorkerDisplayThreadProc(LPVOID pv);      
    CThread<CGateContex, VOID, UINT>  m_thWorker;
    void WaitForWorkerThreadEnd();  
    
public:
    CGateContex(IGlobalDataManager* pGDM);
    ~CGateContex(){WaitForWorkerThreadEnd();this->DeleteInstance();};

string GetGateWorkMode();
void SetGateWorkModeValue(eGateWorkMode eMode);
    void InitStart();
    void Stop(){};
    int initEntryReader();
    int  initExitReader();
    int  initExitCollectReader();    
    int  initTpuModule();
    string ConfMode2GateMode(string sConfMode);
    void  SetDisplayType(tagResPonseInfoData  *pResPonseInfoData,CardAnalysisInfo_t *pOutAnalysisInfo,tagUpdateCmdInfo *InUpdateInfo);
    void  GateDisplay();
    uint16_t    GetGateRunningMode();
    void    SetGateRunningMode(uint16_t wMode);
    void    SetLeftPassNum(BYTE bytEntryNum,BYTE bytExitNum);
    
    void   PleaseTryAgain(BYTE bytMode);
static    void    delete_file(void);
        bool lock_file(const char *filename); 
     static LPVOID  DisplayWorkThread ( LPVOID lParam ); 
     
    int InitIoModule();
    
    
public:
    IGlobalDataManager*  m_pGDM;
    string   m_sGateWorkMode;
    string   m_sGateWorkModeConf;
    string   m_sInitInfo;
    DisplayInfo   m_GateDisplayInfo;
    UiDisplay     m_GateDisplay;
    PARAMCONFIG * m_ptrParam;
    LocalSLEInfo  m_LocalSleInfo;
    
    bool m_bThreadStatusMonitorRun;
    
    //std::unique_lock<std::mutex> m_lock;
    std::mutex  m_SynMutexCv;
    std::condition_variable  m_cv;
    pthread_cond_t    m_condvalue;  
    pthread_mutex_t   m_Syncmutex; 

    std::atomic<int>              m_iChangeFlag;	  //

    //    
    BYTE   m_bytDisplayType;
    BYTE   m_bytRecycleFlag;
    
    uint16_t   m_GateMode;
    tagResPonseInfoData  m_ResPonseInfoData;
    CardAnalysisInfo_t m_OutAnalysisInfo;
    tagUpdateCmdInfo m_InUpdateInfo;
    BYTE  m_bytLeftEntryNum;
    BYTE  m_bytLeftExitNum;
    
    MODULE_IO_CONFIG   m_IoModuleData;
    pthread_t  m_pthreadId;
    
};

#endif /* GATECONTEXT_H */

