/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: ljhpole
 *
 * Created on March 21, 2018, 3:24 PM
 */

#include <cstdlib>
#include <pthread.h>
#include "shAgm.h"


using namespace std;



// Gloable
IGlobalDataManager * g_pMainGDM;
PARAMCONFIG * g_paramPtr = 0;
TRAC_O_CLASSE  g_traceInfo;
LocalSLEInfo g_LocalSleInfo;
ReaderMessageData_t    g_ReaderCmdData;
IN_OUT_UPDATE_SEQ_SN g_InOutUpdateSeqSn;
IN_OUT_MSG_TXN_REC   g_InOutMsgTxnRec;
// Entry Reader
tagResPonseInfoData  g_ReaderResInfo;
tagCardOtcTransDataBak  g_CardDataBak;
CardAnalysisInfo_t     g_CardAnalysisInfo;
tagUpdateCmdInfo       g_CardTransCmdInfo;
// Exit Reader
tagResPonseInfoData  g_ExitReaderResInfo;
tagCardOtcTransDataBak  g_ExitCardDataBak;
CardAnalysisInfo_t     g_ExitCardAnalysisInfo;
tagUpdateCmdInfo       g_ExitCardTransCmdInfo;

// Collect Exit Reader
tagResPonseInfoData  g_TpuExitReaderResInfo;
tagCardOtcTransDataBak  g_TpuExitCardDataBak;
CardAnalysisInfo_t     g_TpuExitCardAnalysisInfo;
tagUpdateCmdInfo       g_TpuExitCardTransCmdInfo;
// status
CDevStatusMonitor  g_DevStausMonitor;
// 显示
UiDisplay            g_UiDisplay;
DisplayInfo          g_UiDisplayInfo;
string               g_DisplayMode;
//UiDisplay            g_ExitUiDisplay;
DisplayInfo          g_ExitUiDisplayInfo;
string               g_ExitDisplayMode;

//LocalSLEInfo g_SLEInfo;
//PARAMCONFIG *g_pParam;
/*
 * 
 */
int main(int argc, char** argv) {
    
    //try
    {
        // 全局数据管理接口初始化
        g_pMainGDM = GetGlobalDataManager();
        if(NULL == g_pMainGDM)
        {
                //OutputDebugString("NULL == g_pGDM");
                return 0;
        }

        g_pMainGDM->Start();
        if(0 != InitShParameter())
        {
                //OutputDebugString("EXECUTE_OK != InitShParameter()");
                return 0;
        }
    // 
    }

    return 0;
}

