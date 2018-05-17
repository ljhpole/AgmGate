/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "shAgm.h"
#include "GateContext.h"
#include <chrono>
#include <mutex>
#include <condition_variable>

using namespace std;


DEFINE_SINGLETON(CGateContex);

CGateContex::CGateContex(IGlobalDataManager* pGDM)
{
     this->CreateInstance();
    if(NULL != pGDM)
    {
        m_pGDM = pGDM;
        string strTempValue;
        strTempValue = m_pGDM->GetINIProxy()->ReadStrIni((char*)"GateContex",(char*)"WorkMode");
        if(0 == strTempValue.size())
        {
                m_pGDM->GetINIProxy()->WriteStrToIni((char*)"GateContex",(char*)"WorkMode",(char*)"0");
                m_sGateWorkMode = "0";
        }
        
        m_sGateWorkMode = strTempValue;
        m_GateDisplayInfo.Reset();
        m_GateDisplay.initial(m_sGateWorkMode,string(""));
    }
   
}
void CGateContex::WaitForWorkerThreadEnd()
{
	if(!m_thWorker.IsRunning())
		return;

	if(m_thWorker.IsInMyThread())
		m_thWorker.Detatch();
	else
	{
		m_thWorker.Join();
	}
}
string CGateContex::ConfMode2GateMode(string sConfMode)
{
    string sModeTmp = "";
    switch(atoi(sConfMode.c_str()))
    {
            case eEn:
            {
                if(m_LocalSleInfo.bytSleId[2] == 0x1B)
                        sModeTmp = "bien";
                else
                        sModeTmp = "en";
            }
            break;
            case eBien:            
            {
                sModeTmp = "bien";
            }
            break;
            case eEx:
            {
                if(m_LocalSleInfo.bytSleId[2] == 0x1B)
                    sModeTmp = "biex";
                else
                    sModeTmp = "ex";
            }
            break;
            case eBiex:
            {
                sModeTmp = "biex";
            }
            break;
            case eBi:
            {
                sModeTmp = "bi";
            }
            break;
    }
    return sModeTmp;

}

int  CGateContex::initTpuModule()
{
	string  strTempValue;
        int  iIndex = 0;
printf("initTpuModule:%d\n",iIndex++);
	try{
		strTempValue = m_pGDM->GetINIProxy()->ReadStrIni((char*)"TPUMODULEINFO",(char*)"PORT");
		if(0 == strTempValue.size())
		{
			m_pGDM->GetINIProxy()->WriteStrToIni((char*)"TPUMODULEINFO",(char*)"PORT",(char*)"/dev/ttyS0");
			return -1;
		}
                m_pGDM->GetTpuModule()->SetComPort((char*)strTempValue.c_str());

		strTempValue.clear();
                
		strTempValue = m_pGDM->GetINIProxy()->ReadStrIni((char*)"TPUMODULEINFO",(char*)"BandRate");
		if(0 == strTempValue.size())
		{
			m_pGDM->GetINIProxy()->WriteStrToIni((char*)"TPUMODULEINFO",(char*)"BandRate",(char*)"57600");
			return -2;
		}
                m_pGDM->GetTpuModule()->SetComBaudrate((char*)strTempValue.c_str());
                m_pGDM->GetTpuModule()->Start();
        }
        catch(...)
	{
		printf("CGateContex:: initTpuModule Failed!\n");
		return -1;
	}
printf("initTpuModule:%d\n",iIndex++);

        return 0;
}
int CGateContex::initEntryReader()
{
	string  strTempValue;
	READER_CONFIG  m_configReader;
        int  iIndex = 0;
printf("CGateContex::initEntryReader*****************************initEntryReader:%d\n",iIndex++);
	try{
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
		memcpy(m_configReader.m_bytDevSleCode,m_LocalSleInfo.bytSleId,sizeof(m_LocalSleInfo.bytSleId));

		m_configReader.m_dwRunningMode = LM_REVENUE;
		m_configReader.m_dwStatus = (eShWorkEntryMode + eIdle );

printf("CGateContex::initEntryReader*****************************initEntryReader:%d\n",iIndex++);
		m_pGDM->GetReader()->SetConfigReader(&m_configReader);
printf("CGateContex::initEntryReader*****************************iinitEntryReader:%d\n",iIndex++);
                printf("EntryReader Port:%s,BaundRate:%s,\r\n",m_configReader.m_strPort.c_str(),m_configReader.m_strBaudrate.c_str());
	}
	catch(...)
	{
		printf("CGateContex:: initEntryReader Failed!\n");
		return -1;
	}


	return 0;
}
int  CGateContex::initExitCollectReader()
{
    int iRtn = -1;
	string  strTempValue;
	READER_CONFIG  m_configReader;
        int  iIndex = 0;
	try{
printf("CGateContex::initExitCollectReader*****************************initExitReader:%d\n",iIndex++);            
		m_configReader.Reset();
		strTempValue = m_pGDM->GetINIProxy()->ReadStrIni((char*)"EXITCOLLECTREADERINFO",(char*)"PORT");
		if(0 == strTempValue.size())
		{
			m_pGDM->GetINIProxy()->WriteStrToIni((char*)"EXITCOLLECTREADERINFO",(char*)"PORT",(char*)"/dev/ttyS10");
			iRtn = -2;
                        return iRtn;
		}
		m_configReader.m_strPort = strTempValue;          

		strTempValue.clear();
		strTempValue = m_pGDM->GetINIProxy()->ReadStrIni((char*)"EXITCOLLECTREADERINFO",(char*)"BandRate");
		if(0 == strTempValue.size())
		{
			m_pGDM->GetINIProxy()->WriteStrToIni((char*)"EXITCOLLECTREADERINFO",(char*)"BandRate",(char*)"57600");
			iRtn = -3;
                        return iRtn;
		}
		m_configReader.m_strBaudrate = strTempValue;           

		m_configReader.eConfigType = eAll;
		m_configReader.m_bIsConnect = false;
		memcpy(m_configReader.m_bytDevSleCode,m_LocalSleInfo.bytSleId,sizeof(m_LocalSleInfo.bytSleId));

		m_configReader.m_dwRunningMode = LM_REVENUE;
		m_configReader.m_dwStatus = (eShWorkExitMode + eIdle );

		m_pGDM->GetExitCollectReader()->SetConfigReader(&m_configReader);
                printf("CGateContex::initExitCollectReader Port:%s,BaundRate:%s,\r\n",m_configReader.m_strPort.c_str(),m_configReader.m_strBaudrate.c_str());

	}
	catch(...)
	{
		printf("CGateContex::initExitCollectReader Failed!\n");
		iRtn = -4;
                return iRtn;
	}    
    
    return iRtn;
}
int  CGateContex::initExitReader()
{
	string  strTempValue;
	READER_CONFIG  m_configReader;
        int  iIndex = 0;
	try{
printf("CGateContex::initExitReader*****************************initExitReader:%d\n",iIndex++);            
		m_configReader.Reset();
		strTempValue = m_pGDM->GetINIProxy()->ReadStrIni((char*)"EXITREADERINFO",(char*)"PORT");
		if(0 == strTempValue.size())
		{
			m_pGDM->GetINIProxy()->WriteStrToIni((char*)"EXITREADERINFO",(char*)"PORT",(char*)"/dev/ttyS1");
			return -1;
		}
		m_configReader.m_strPort = strTempValue;          

		strTempValue.clear();
		strTempValue = m_pGDM->GetINIProxy()->ReadStrIni((char*)"EXITREADERINFO",(char*)"BandRate");
		if(0 == strTempValue.size())
		{
			m_pGDM->GetINIProxy()->WriteStrToIni((char*)"EXITREADERINFO",(char*)"BandRate",(char*)"57600");
			return -2;
		}
		m_configReader.m_strBaudrate = strTempValue;           

		m_configReader.eConfigType = eAll;
		m_configReader.m_bIsConnect = false;
		memcpy(m_configReader.m_bytDevSleCode,m_LocalSleInfo.bytSleId,sizeof(m_LocalSleInfo.bytSleId));

		m_configReader.m_dwRunningMode = LM_REVENUE;
		m_configReader.m_dwStatus = (eShWorkExitMode + eIdle );

		m_pGDM->GetExitReader()->SetConfigReader(&m_configReader);
printf("CGateContex::initExitReader***************************** initExitReader:%d\n",iIndex++);            
                printf("CGateContex::initExitReader Port:%s,BaundRate:%s,\r\n",m_configReader.m_strPort.c_str(),m_configReader.m_strBaudrate.c_str());

	}
	catch(...)
	{
		printf("CGateContex::initExitReader Failed!\n");
		return -1;
	}


	return 0;    

}
void CGateContex::InitStart()
{
        m_ptrParam = m_pGDM->GetParameter()->get_shcurparam();
        m_pGDM->GetParameter()->GetLocalSLEInfo(&m_LocalSleInfo);    
        char buf[128] = {0};
        snprintf(buf,sizeof(buf),"%02X-%02X-%02X-%02X %s",m_LocalSleInfo.bytSleId[0], \
                m_LocalSleInfo.bytSleId[1],m_LocalSleInfo.bytSleId[2],m_LocalSleInfo.bytSleId[3],SH_GATE_Version);
        m_sInitInfo = buf;
        switch(atoi((char*)m_sGateWorkMode.c_str()))
        {
            case eEn:
            case eBien:            
            {
                m_GateDisplayInfo.direction = IN;
                initEntryReader();
                memset(buf,0,sizeof(buf));
                snprintf(buf,sizeof(buf),"%s","11");
            }
            break;
            case eEx:
            case eBiex:
            {
                m_GateDisplayInfo.direction = OUT;            
                initExitReader();
                initTpuModule();
                initExitCollectReader();
                memset(buf,0,sizeof(buf));
                snprintf(buf,sizeof(buf),"%s","12");                                
            }
            break;
            case eBi:
            {
                m_GateDisplayInfo.direction = IN;
                initEntryReader();
                initExitReader();
                initTpuModule();
                initExitCollectReader();
                memset(buf,0,sizeof(buf));                
                snprintf(buf,sizeof(buf),"%s","13");                                
            }
            break;
            default:
            {
                switch(m_LocalSleInfo.bytSleId[2])
                {
                    case 0x1A:
                    {
                        m_GateDisplayInfo.direction = IN;
                        initEntryReader();
                        m_sGateWorkMode = "1";
                        memset(buf,0,sizeof(buf));
                        snprintf(buf,sizeof(buf),"%s","11");                                
                        
                    }
                    break;
                    case 0x1B:
                    {
                        m_GateDisplayInfo.direction = IN; 
                        initEntryReader();
                        initExitReader();  
                        initTpuModule();
                        initExitCollectReader();
                        m_sGateWorkMode = "3";
                        memset(buf,0,sizeof(buf));
                        snprintf(buf,sizeof(buf),"%s","13");                                                        
                    }
                    break;
                    case 0x15:
                    {
                        m_GateDisplayInfo.direction = OUT;                    
                        initExitReader();
                        initTpuModule();  
                        initExitCollectReader();
                        m_sGateWorkMode = "2";
                        memset(buf,0,sizeof(buf));
                        snprintf(buf,sizeof(buf),"%s","12");                                                        
                    }
                    break;
                    default:
                    {
                        memset(buf,0,sizeof(buf));
                        snprintf(buf,sizeof(buf),"%s","13");                          
                    }
                }
                m_pGDM->GetINIProxy()->WriteStrToIni((char*)"GateContex",(char*)"WorkMode",(char*)(m_sGateWorkMode.c_str()));
            }
            break;
        }

#ifdef SH_GATE_SNAP_VERSION        
       this->InitIoModule();
#endif
        
        m_pGDM->GetSnapMaFlap()->SetMCPMode(buf);
        m_pGDM->GetSnapMaFlap()->SetRunningMode(LM_REVENUE);        
        m_pGDM->GetINIProxy()->WriteStrToIni((char*)"MASNAPTripod",(char*)"DevType",buf);

        SetGateRunningMode(LM_REVENUE);
        m_GateDisplay.initial(ConfMode2GateMode(m_sGateWorkMode),m_sInitInfo);
        m_GateDisplay.normal(m_GateDisplayInfo);


        
        pthread_create(&m_pthreadId, NULL, DisplayWorkThread, (PVOID)this);

        
        // m_thWorker.Start(this, &CGateContex::WorkerDisplayThreadProc,this);
 
}
string CGateContex::GetGateWorkMode()
{
    return m_sGateWorkMode;
}
void CGateContex::SetGateWorkModeValue(eGateWorkMode eMode)
{
    switch(eMode)
    {
        case eEn:
        case eBien:            
        {
            m_sGateWorkMode="1";
        }
        break;
        case eEx:
        case eBiex:
        {
            m_sGateWorkMode="2";
        }
        break;
        case eBi:
        {
            m_sGateWorkMode="3";
        }
        break; 
        default:
            break;
    }
    m_pGDM->GetINIProxy()->WriteStrToIni((char*)"GateContex",(char*)"WorkMode",(char*)(m_sGateWorkMode.c_str()));   
//    std::unique_lock<std::mutex> lck(m_SynMutexCv);
//    m_cv.notify_one();
     m_iChangeFlag++;
}

void    CGateContex::SetLeftPassNum(BYTE bytEntryNum,BYTE bytExitNum)
{
    
}
void   CGateContex::PleaseTryAgain(BYTE bytMode)
{
    switch(atoi((char*)(m_sGateWorkMode.c_str())))
    {
        case eEn:
        case eBien:            
        {
            m_GateDisplayInfo.direction = IN;
            m_GateDisplayInfo.passNum = m_pGDM->GetSnapMaFlap()->GetEnterNum();
        }
        break;
        case eEx:
        case eBiex:
        {
            m_GateDisplayInfo.direction = OUT; 
            m_GateDisplayInfo.passNum = m_pGDM->GetSnapMaFlap()->GetExitNum();
        }
        break;
        case eBi:
        {
            m_GateDisplayInfo.direction = IN;
            if(m_pGDM->GetSnapMaFlap()->GetEnterNum() >0)
                m_GateDisplayInfo.passNum = m_pGDM->GetSnapMaFlap()->GetEnterNum();
            else if(m_pGDM->GetSnapMaFlap()->GetExitNum() > 0)
                m_GateDisplayInfo.passNum = m_pGDM->GetSnapMaFlap()->GetExitNum();

        }
        break;
    } 
    m_GateDisplayInfo.eventList = m_pGDM->GetMsgComSC()->GetMsgTxnEventList();
    m_GateDisplayInfo.remainAmt=0;
    switch(bytMode)
    {
        case mtEFO:
        {
            m_GateDisplayInfo.direction = OUT;
            m_GateDisplay.initial(ConfMode2GateMode(m_sGateWorkMode),m_sInitInfo);             

        }
        break;
        case mtBOM:
            {
                m_GateDisplayInfo.direction = IN;
                m_GateDisplay.initial(ConfMode2GateMode(m_sGateWorkMode),m_sInitInfo); 

            } 
        break;
    }
    m_GateDisplay.tryagain(m_GateDisplayInfo);
//    std::unique_lock<std::mutex> lck(m_SynMutexCv);
//    m_cv.notify_one();
     m_iChangeFlag++;

}
void  CGateContex::SetDisplayType(tagResPonseInfoData  *pResPonseInfoData,CardAnalysisInfo_t *pOutAnalysisInfo,tagUpdateCmdInfo *InUpdateInfo)
{
    if(NULL != InUpdateInfo)
    {
        m_InUpdateInfo.bytDisplayType = InUpdateInfo->bytDisplayType;
        m_InUpdateInfo.bytRecycleFlag = InUpdateInfo->bytRecycleFlag;
        m_InUpdateInfo.dwAfterTransValue = InUpdateInfo->dwAfterTransValue;
        m_InUpdateInfo.dwBeforeTransValue = InUpdateInfo->dwBeforeTransValue;
        m_ResPonseInfoData.iCurrentFormView = pResPonseInfoData->iCurrentFormView;
        m_ResPonseInfoData.bytWorkMode = pResPonseInfoData->bytWorkMode;
        m_OutAnalysisInfo.CanUpdate = pOutAnalysisInfo->CanUpdate;
        m_OutAnalysisInfo.strAnalysisResult = pOutAnalysisInfo->strAnalysisResult;
    }
    
    m_bytDisplayType = m_InUpdateInfo.bytDisplayType;
    m_bytRecycleFlag = m_InUpdateInfo.bytRecycleFlag;
    switch(atoi((char*)(m_sGateWorkMode.c_str())))
    {
        case eEn:
        case eBien:            
        {
            m_GateDisplayInfo.direction = IN;
            m_GateDisplayInfo.passNum = m_pGDM->GetSnapMaFlap()->GetEnterNum();
        }
        break;
        case eEx:
        case eBiex:
        {
            m_GateDisplayInfo.direction = OUT; 
            m_GateDisplayInfo.passNum = m_pGDM->GetSnapMaFlap()->GetExitNum();
        }
        break;
        case eBi:
        {
            m_GateDisplayInfo.direction = IN;
            if(m_pGDM->GetSnapMaFlap()->GetEnterNum() >0)
                m_GateDisplayInfo.passNum = m_pGDM->GetSnapMaFlap()->GetEnterNum();
            else if(m_pGDM->GetSnapMaFlap()->GetExitNum() > 0)
                m_GateDisplayInfo.passNum = m_pGDM->GetSnapMaFlap()->GetExitNum();

        }
        break;
    } 
    m_GateDisplayInfo.eventList = m_pGDM->GetMsgComSC()->GetMsgTxnEventList();
    m_GateDisplayInfo.remainAmt=0;


    switch(m_ResPonseInfoData.iCurrentFormView)
    {
        case mtEFO:
        {
            m_GateDisplayInfo.direction = OUT;
            m_GateDisplay.initial(ConfMode2GateMode(m_sGateWorkMode),m_sInitInfo);             
            if(m_ResPonseInfoData.bytWorkMode == 0x40 && m_bytRecycleFlag == 0x01)
            {
                m_GateDisplay.instkt(m_GateDisplayInfo);
            }
            else  if(!m_OutAnalysisInfo.CanUpdate)
            {
                m_GateDisplayInfo.rejectCode = m_OutAnalysisInfo.strAnalysisResult;
                m_GateDisplay.gotosvc(m_GateDisplayInfo);               
            }
            else
            {
                m_GateDisplayInfo.direction = OUT;
                m_GateDisplay.initial(ConfMode2GateMode(m_sGateWorkMode),m_sInitInfo);                
                if(0x1 == m_bytDisplayType)
                {
                    m_GateDisplay.exit(m_GateDisplayInfo);
                }
                else if(0x2 == m_bytDisplayType)
                {
                    m_GateDisplayInfo.remainAmt = m_InUpdateInfo.dwAfterTransValue/100.00;
                    m_GateDisplay.exit_v(m_GateDisplayInfo);
                }
                else if(0x4 == m_bytDisplayType)
                {
                    m_GateDisplay.exit_e(m_GateDisplayInfo);
                }
                else if(0x5 == m_bytDisplayType)
                {
                    m_GateDisplayInfo.remainAmt = m_InUpdateInfo.dwBeforeTransValue;
                    m_GateDisplay.exit_t(m_GateDisplayInfo);                    
                }
                else if(0x6 == m_bytDisplayType)
                {
                    m_GateDisplayInfo.remainAmt = m_InUpdateInfo.dwBeforeTransValue;
                    m_GateDisplay.exit_t(m_GateDisplayInfo); 
                }
                else if(0x3 == m_bytDisplayType)  //0x3:显示计次类票卡信息
                {
                    m_GateDisplayInfo.remainAmt = m_InUpdateInfo.dwAfterTransValue;
                    m_GateDisplay.exit_dt(m_GateDisplayInfo); 
                }
                else
                {
                    return ;                    
                }
            }            
        }
        break;
        case mtBOM:
            {
                m_GateDisplayInfo.direction = IN;
                m_GateDisplay.initial(ConfMode2GateMode(m_sGateWorkMode),m_sInitInfo); 
                if(!m_OutAnalysisInfo.CanUpdate)
                {
                    m_GateDisplayInfo.rejectCode = m_OutAnalysisInfo.strAnalysisResult;
                    m_GateDisplay.gotosvc(m_GateDisplayInfo);                      
                }
                else if(0x1 == m_bytDisplayType)
                {
                    m_GateDisplay.entry(m_GateDisplayInfo);
                }
                else if(0x2 == m_bytDisplayType)
                {
                    m_GateDisplayInfo.remainAmt = m_InUpdateInfo.dwBeforeTransValue/100.00;
                    m_GateDisplay.entry_v(m_GateDisplayInfo);
                }
                else if(0x4 == m_bytDisplayType)
                {
                    m_GateDisplay.entry_e(m_GateDisplayInfo);
                }
                else if(0x5 == m_bytDisplayType)
                {
                    m_GateDisplayInfo.remainAmt = m_InUpdateInfo.dwBeforeTransValue;
                    m_GateDisplay.entry_t(m_GateDisplayInfo);                    
                }
                else if(0x6 == m_bytDisplayType)
                {
                    m_GateDisplayInfo.remainAmt = m_InUpdateInfo.dwBeforeTransValue;
                    m_GateDisplay.entry_t(m_GateDisplayInfo);                      
                }
                else if(0x3 == m_bytDisplayType)
                {
                    m_GateDisplayInfo.remainAmt = m_InUpdateInfo.dwBeforeTransValue;
                    m_GateDisplay.entry_dt(m_GateDisplayInfo); 
                }
                else
                {
                    return ;                    
                }
            } 
        break;
    }
//    std::unique_lock<std::mutex> lck(m_SynMutexCv);
//    m_cv.notify_one();
 m_iChangeFlag++;
    return ;
}
void  CGateContex::GateDisplay()
{    
    // m_GateDisplay.initial(ConfMode2GateMode(m_sGateWorkMode),m_sInitInfo);    
    // m_GateDisplay.normal(m_GateDisplayInfo);
    //printf("CGateContex::WorkerDisplayThreadProc -----------------------------------std::cv_status::timeout\n");
    
    return ;
}
void    CGateContex::SetGateRunningMode(uint16_t wMode)
{
    m_GateMode = wMode;
    m_iChangeFlag++;
}
uint16_t  CGateContex::GetGateRunningMode()
{
//    uint16_t   wMode = 0;
//    
//    if(wMode != m_GateMode)
//    {
//        m_GateMode = wMode;
//    }
//    else
//    {
//        wMode = LM_MODE_NOCHANGE;
//    }
    return m_GateMode; 
}
UINT WINAPI CGateContex::WorkerDisplayThreadProc(LPVOID lpParam)
{
    return 0;
}
LPVOID  CGateContex::DisplayWorkThread ( LPVOID lpParam )
{
//    CGateContex* pThis = (CGateContex*)lParam;
//    pThis->m_thWorker.Start(pThis, &CGateContex::WorkerDisplayThreadProc,pThis);
//}
//UINT WINAPI CGateContex::WorkerDisplayThreadProc(LPVOID lpParam)
//{
	TRACE("---------------> WorkerDisplayThreadProc Monitor Thread 0x%08X started --------------->\n", SELF_THREAD_ID);
        CGateContex* pThis = (CGateContex*)lpParam;
	int temp=0,iRtn = -1,iIndex = -1;
	pThis->m_bThreadStatusMonitorRun = true;
        char buf[128] = {0};
        snprintf(buf,sizeof(buf),"%02X-%02X-%02X-%02X",pThis->m_LocalSleInfo.bytSleId[0], \
        pThis->m_LocalSleInfo.bytSleId[1],pThis->m_LocalSleInfo.bytSleId[2],pThis->m_LocalSleInfo.bytSleId[3] );
        pThis->m_sInitInfo = buf;
//printf("---------------> WorkerDisplayThreadProc Monitor Thread 0x%08X  %d started --------------->\n", SELF_THREAD_ID,iIndex++);        
//        std::unique_lock<std::mutex> lck(pThis->m_SynMutexCv); 
//printf("---------------> WorkerDisplayThreadProc Monitor Thread 0x%08X  %d started --------------->\n", SELF_THREAD_ID,iIndex++);        
	while(pThis->m_bThreadStatusMonitorRun)
	{	            
		//try
		{  
//printf("---------------> WorkerDisplayThreadProc Monitor Thread 0x%08X  %d started ---start time:%ld------------>\n", SELF_THREAD_ID,iIndex++,std::chrono::system_clock::now());                                      
//std::cv_status cvRtn = pThis->m_cv.wait_for(lck,std::chrono::seconds(2));  
//printf("---------------> WorkerDisplayThreadProc Monitor Thread 0x%08X  %d started --------------->\n", SELF_THREAD_ID,iIndex++);                    
//                    
                       
                        //if( std::cv_status::no_timeout == cvRtn)
                        if(pThis->m_iChangeFlag == 0)
                        {
                           pThis->GateDisplay();
                           usleep(500000);
                           continue;
                        }
                        else
                        {
                            pThis->m_iChangeFlag = 0;
                        }
                        switch(atoi((char*)(pThis->m_sGateWorkMode.c_str())))
                        {
                            case eEn:
                            case eBien:            
                            {
                                pThis->m_GateDisplayInfo.direction = IN;
                                if(pThis->m_pGDM->GetSnapMaFlap()->GetEnterNum() > 0)
                                {
                                    pThis->m_GateDisplayInfo.passNum = pThis->m_pGDM->GetSnapMaFlap()->GetEnterNum();                                    
                                    continue;
                                }
                            }
                            break;
                            case eEx:
                            case eBiex:
                            {
                                pThis->m_GateDisplayInfo.direction = OUT; 
                                if(pThis->m_pGDM->GetSnapMaFlap()->GetExitNum() > 0)
                                {
                                    continue;
                                }
                            }
                            break;
                            case eBi:
                            {
                                pThis->m_GateDisplayInfo.direction = IN;
                                if(pThis->m_pGDM->GetSnapMaFlap()->GetExitNum() > 0  || pThis->m_pGDM->GetSnapMaFlap()->GetEnterNum() > 0)
                                {
                                    continue;
                                }
                            }
                            break;
                        }
                }

                
                pThis->m_GateDisplayInfo.eventList = pThis->m_pGDM->GetMsgComSC()->GetMsgTxnEventList();
                pThis->m_GateDisplayInfo.rejectCode="";                
                pThis->m_GateDisplayInfo.remainAmt=0;
                pThis->m_GateDisplay.initial(pThis->ConfMode2GateMode(pThis->m_sGateWorkMode),pThis->m_sInitInfo);
                switch(pThis->m_GateMode)
                {
                    case LM_REVENUE:
                    {
                        pThis->m_GateDisplay.normal(pThis->m_GateDisplayInfo);
                    }
                    break;
                    case LM_OUTOFSERVICE:
                    case LM_TRAINMALFUNCTION:                        
                    {
                        pThis->m_GateDisplay.outofsvc(pThis->m_GateDisplayInfo);
                    }
                    break;
                    case LM_EMERGENCY:
                    {
                        pThis->m_GateDisplay.emergency(pThis->m_GateDisplayInfo);
                    }
                    break;
                    case LM_CLOSE:
                    {
                        pThis->m_GateDisplay.stopsvc(pThis->m_GateDisplayInfo);
                    }
                    break;
                    case LM_TEST:
                    {
                        pThis->m_GateDisplay.maintenance(pThis->m_GateDisplayInfo);
                    }
                    break;
                    case LM_STORE_TICKET:
                    {
                        pThis->m_GateDisplay.svtonly(pThis->m_GateDisplayInfo);
                    }
                    break;
                    case LM_SIGLE_TICKET_ONLY:
                    {
                        pThis->m_GateDisplay.sjtonly(pThis->m_GateDisplayInfo);
                    }
                    break;
                }
                
        } 
	TRACE("---------------> WorkerDisplayThreadProc Monitor Thread 0x%08X Stoped <---------------", SELF_THREAD_ID);
        
        return 0;
}


int CGateContex::InitIoModule()
{
	string  strTempValue;
        BYTE    bytTmp = 0;
	try{		

		strTempValue = m_pGDM->GetINIProxy()->ReadStrIni((char*)"IOMODULE",(char*)"PORT");
		if(0 == strTempValue.size())
		{
                    m_pGDM->GetINIProxy()->WriteStrToIni((char*)"IOMODULE",(char*)"PORT",(char*)"/dev/ttyS15");
                    return -1;		
                }
                m_IoModuleData.m_strPort = strTempValue;
                

		strTempValue.clear();
		strTempValue = m_pGDM->GetINIProxy()->ReadStrIni((char*)"IOMODULE",(char*)"BandRate");
		if(0 == strTempValue.size())
		{
			m_pGDM->GetINIProxy()->WriteStrToIni((char*)"IOMODULE",(char*)"BandRate",(char*)"57600");
			return -2;
		}   
                m_IoModuleData.m_strBaudrate =  strTempValue;               
                
                m_IoModuleData.m_eConfigType = eAll;
                m_IoModuleData.m_bIsConnect = false;
// m_IoModuleData.m_strWorkMode 
                m_pGDM->GetIoModule()->SetModuleIoConfig(&m_IoModuleData);
		m_pGDM->GetUtils()->DebugMsg(&g_traceInfo,"IOMODULE","IOMODULE::InitIoModule() Start OpenComm! ");


	}
	catch(...)
	{
		return -3;
	}


	return 0;    
}

void CGateContex::delete_file(void)
{
    remove(LOCKFILE);
}

//
bool CGateContex::lock_file(const char *filename)
{
   int fd = -1;
    fd = open(LOCKFILE, O_RDWR | O_TRUNC | O_CREAT | O_EXCL, 0664);
    if (fd < 0)
    {
        if (errno == EEXIST)
        {
            
            return false;
        }
        return false;
    }
    else
    {
       return true; 
    }
}

