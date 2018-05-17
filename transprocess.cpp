/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "shAgm.h"
#include "transprocess.h"

using namespace std;

void FillTransResponseData(tagResPonseInfoData  *pResPonseInfoData,Gate_msg *pReaderMsg,tagCardOtcTransDataBak *pCardTransData)
{
    pCardTransData->m_dwTransTimePre = time(NULL) + 8*60*60;
    switch(pReaderMsg->msg_type)
    {
        case eExitCollectReader_type:
        {
            switch(pReaderMsg->u.reader.m_bytRecardInfoCmdCode)
            {
                case eCSC_OTT_EXIT:
                {
                    memcpy(&pResPonseInfoData->tagOttExitResp,pReaderMsg->u.reader.m_msgReaderData,sizeof(pResPonseInfoData->tagOttExitResp));       
                    memcpy(&pCardTransData->uCardInfoBak.m_OttReadInfo,&pResPonseInfoData->tagReaderReadOttResp,sizeof(pCardTransData->uCardInfoBak.m_OttReadInfo));
                }
                break;
            }
        }
        break;
        case eExReader_type:
        {
            switch(pReaderMsg->u.reader.m_bytRecardInfoCmdCode)
            {
                case eCSC_OTT_EXIT:
                {
                    memcpy(&pResPonseInfoData->tagOttExitResp,pReaderMsg->u.reader.m_msgReaderData,sizeof(pResPonseInfoData->tagOttExitResp));       
                    memcpy(&pCardTransData->uCardInfoBak.m_OttReadInfo,&pResPonseInfoData->tagReaderReadOttResp,sizeof(pCardTransData->uCardInfoBak.m_OttReadInfo));
                }
                break;
                case eCSC_OTQ_EXIT:
                {
                    memcpy(&pResPonseInfoData->tagQrExitResp,pReaderMsg->u.reader.m_msgReaderData,sizeof(CSC_MP_QR_EXIT));                    
                    memcpy(&pCardTransData->uCardInfoBak.m_QrReadInfo,&pResPonseInfoData->tagReaderReadOtqResp,sizeof(pCardTransData->uCardInfoBak.m_QrReadInfo));
                }
                break;
                case eCSC_OTC_EXIT:
                {
                   memcpy(&pResPonseInfoData->tagOtcExitResp,pReaderMsg->u.reader.m_msgReaderData,sizeof(CSC_MP_OTC_EXIT));
                   memcpy(&pCardTransData->uCardInfoBak.m_OtcReadInfo,&pResPonseInfoData->tagReaderOtcResp,sizeof(pCardTransData->uCardInfoBak.m_OtcReadInfo));   
                }
                break;
            }
        }
        break;
        case eEnReader_type:
        {
            switch(pReaderMsg->u.reader.m_bytRecardInfoCmdCode)
            {
		case eCSC_OTT_ENTRY:
		case eCSC_OTT_ENTRY2:
                {
                    memcpy(&pResPonseInfoData->tagOttEntryResp,pReaderMsg->u.reader.m_msgReaderData,sizeof(CSC_MP_OTT_ENTRY)); 
                    memcpy(&pCardTransData->uCardInfoBak.m_OttReadInfo,&pResPonseInfoData->tagReaderReadOttResp,sizeof(pCardTransData->uCardInfoBak.m_OttReadInfo));
                    
                }
                    break;
   		case eCSC_OTQ_ENTRY:
			{
                                memcpy(&pResPonseInfoData->tagQrEntryResp,pReaderMsg->u.reader.m_msgReaderData,sizeof(CSC_MP_QR_ENTRY));
                                memcpy(&pCardTransData->uCardInfoBak.m_QrReadInfo,&pResPonseInfoData->tagReaderReadOtqResp,sizeof(pCardTransData->uCardInfoBak.m_QrReadInfo));
                        }
			break;
		case eCSC_OTC_ENTRY:
			{
                                memcpy(&pResPonseInfoData->tagOtcEntryResp,pReaderMsg->u.reader.m_msgReaderData,sizeof(CSC_MP_OTC_ENTRY));
                             memcpy(&pCardTransData->uCardInfoBak.m_OtcReadInfo,&pResPonseInfoData->tagReaderOtcResp,sizeof(pCardTransData->uCardInfoBak.m_OtcReadInfo));   
                        }
			break;
                 
            
            }

        }
        break;
    }
    return;
}

//void ModifyRegist(IGlobalDataManager * ptrGDM,tagResPonseInfoData  *pResPonseInfoData,TRAC_O_CLASSE *pTracelog,CardAnalysisInfo_t *pAnalysisInfo,BYTE bytUpdateMode,tagUpdateCmdInfo *pUpdateCmdInfo,IN_OUT_UPDATE_SEQ_SN *pInOutSeqSn,int &iRtn)
//{
//	BYTE bytType;
//	iRtn = 0;
//	pInOutSeqSn->eInOutUpdateFlag = eOutFlag;
//	pInOutSeqSn->eSeqSnType = eBomRegist6000Sn;
//	memset(&pInOutSeqSn->lBomRegist6000[0],0,sizeof(pInOutSeqSn->lBomRegist6000));
//	iRtn = ptrGDM->GetSQLITEDBManagerProxy()->DBInOutModifySeqSn(pInOutSeqSn);
//	if(0 != iRtn)
//	{
//		ptrGDM->GetUtils()->DebugMsg(pTracelog,"ModifyRegist","DBInOutModifySeqSn Get Failed! iRtn = %d",iRtn);
//		return;
//	}
///*﻿00     进站数（所有票种）
//01     出站数（所有票种）
//02     用储值交通卡进站数-成人（票种04h）
//03     用单程票进站数（票种64h）
//04     用乘次票进站数（票种78h）
//05     用员工票1进站数（公共交通卡类型，票种28h）
//06     用员工票2进站数（轨道交通专用票类型，票种82h
//07     用打折类型1A(D1-交通卡，儿童)票进站数（票种30h）
//08     用打折类型1B(D2-交通卡，老人)票进站数（票种19h）
//09     用VP(VP-纪念，旅行者)票进站数（票种80h
//10     入口处拒绝数
//11     用储值交通卡出站数-成人（票种04h）
//12     用单程票出站数（票种64h）
//13     用乘次票出站数（票种78h）
//14     用员工票1出站数（公共交通卡类型，票种28h）
//15     用员工票2出站数（轨道交通专用票类型，票种82h
//16     用打折类型1A(D1-交通卡，儿童)票出站数（票种30h）
//17     用打折类型1B(D2-交通卡，老人)票出站数（票种19h）
//18     用VP(VP-纪念，旅行者)票出站数（票种80h
//19     出站检票机处拒绝数
//20     回收数
//21     储值交通卡-成人扣款值（票种04h）
//22     单程票扣款值（票种64h）
//23     员工票1扣款值（公共交通卡类型，票种28h）
//24     员工票2扣款值（轨道交通专用票类型，票种82h
//25     打折类型1A(D1-交通卡，儿童)票扣款值（票种30h）
//26     打折类型1B(D2-交通卡，老人)票扣款值（票种06h）
//27     VP(VP-纪念，旅行者)票扣款值（票种80h
//28     非高峰时段出站数
//29     自由出站数
//30     同站出站数
//31     合法的测试票数
//32     不足额，储值交通卡的最后乘次奖励
//33     单程票剩余票值
//34     不足额，一票通的最后乘次奖励
//35     交通卡优惠次数
//36     交通卡优惠金额
//37     PBOC卡进站人数
//38     PBOC卡出站人数
//39     PBOC卡-扣款值
//40     PBOC卡优惠次数
//41     PBOC卡优惠金额
//42     闸机紧急模式进站数
//43     闸机紧急模式出站数44    计时计次票进站数（票种84h）
//45    计时计次票出站数（票种84h）
//46 ~ 63    [未定义]
//*/
//	// bytType = pUpdateCmdInfo->bytUpdateMode;
//
//	switch(pUpdateCmdInfo->bytTransEventCode)
//	{
//            case 0x24:  // 票扣款
//            case 0x25:  // 卡扣款
//            case 0x40:  // ﻿出站联乘扣款
//            case 0x41:  // ﻿出站联乘扣款
//                    {
//                        switch(pAnalysisInfo->bytTicketType)
//                        {
//                            pInOutSeqSn->lBomRegist6000[1]++;  // ﻿出站数（所有票种）
//                            case 0x04://
//                            {
//                                pInOutSeqSn->lBomRegist6000[11]++;//用储值交通卡进站数-成人（票种04h）
//                                pInOutSeqSn->lBomRegist6000[21] += pUpdateCmdInfo->dwTransValue;
//                            }
//                            break;
//                            case 0x64:
//                            {
//                                pInOutSeqSn->lBomRegist6000[12]++;
//                                pInOutSeqSn->lBomRegist6000[22] += pUpdateCmdInfo->dwTransValue;                            
//                            }
//                            break;
//                            case 0x78:
//                            {
//                                pInOutSeqSn->lBomRegist6000[12]++;
//                                pInOutSeqSn->lBomRegist6000[22] += pUpdateCmdInfo->dwTransValue;                            
//                            }
//                            break;
//                        }
//                        if(0x1E == pUpdateCmdInfo->bytSamStationExit)
//                        {
//                            pInOutSeqSn->lBomRegist6000[30]++;
//                        }
//                        if(0x1C == pUpdateCmdInfo->bytPeekOffFlag)
//                        {
//                            pInOutSeqSn->lBomRegist6000[28]++;
//                        }
//                    }
//            break;
//            
//            case 0x26:   // ﻿出站联乘进站 ticket
//            case 0x27:   //  ﻿出站联乘进站  card
//            case 0x2A:
//            case 0x2B:
//            case 0x3E:  // ﻿尾程优惠
//            case 0x3F:  // ﻿设置交通卡累积消费金额
//            {
//                
//            }
//            break;
//            case 0x35:  // ﻿锁卡更改卡状态交易		票/交通卡
//            case 0x3B:  // ﻿锁卡清空卡上钱包交易
//            {
//            }
//            break;
///*
// ﻿91h	反向进/出站	0	车票被拒绝
//92h	超过时间限制	0	车票被拒绝
//93h	超乘车票	0	车票被拒绝
//94h	校验和错误	0	车票被拒绝
//95h	测试/收益	0	车票被拒绝
//96h	非法（未定义的票卡类型）	0	车票被拒绝
//97h	不可接受的票卡类型	0	入口处被拒绝
//98h	再次进站	0	入口处被拒绝
//99h	可用日以前使用	0	入口处被拒绝
//9Ah	票值为0拒绝	0	入口处被拒绝
//9Bh	仅可同站进出	0	出站处被拒绝
//9Ch	剩余票值低于最低票价	0	入口处被拒绝
//9Dh	同站进出	0	出站处被拒绝
//*/            
//            case 0x88:  // ﻿黑名单票在出站时被拒绝
//            case 0x89:  // ﻿黑名单票在进站时被拒绝                
//            case 0x8A:  // ﻿黑名单票入口处被接受
//            case 0x8D:  // ﻿锁卡失败
//            case 0x90:  //过期
//            case 0x91:
//            case 0x92:
//            case 0x93:
//            case 0x94:
//            case 0x95:
//            case 0x96:
//            case 0x97:
//            case 0x98:
//            case 0x99:
//            case 0x9A:
//            case 0x9B:
//            case 0x9C:
//            case 0x9D:
//            {
//               if(0x20 == pResPonseInfoData->bytWorkMode) // 进站
//               {
//                   pInOutSeqSn->lBomRegist6000[10]++;
//               }
//               else if(0x40 == pResPonseInfoData->bytWorkMode)
//               {
//                   pInOutSeqSn->lBomRegist6000[19]++;                   
//               }
//            }
//            break;
//
//            
//            
//        
//                            
//
//	}
//	pInOutSeqSn->eInOutUpdateFlag = eUpdateFlag;
//	iRtn = ptrGDM->GetSQLITEDBManagerProxy()->DBInOutModifySeqSn(pInOutSeqSn);
//	if(0 != iRtn)
//	{
//		ptrGDM->GetUtils()->DebugMsg(pTracelog,"ModifyRegist","DBInOutModifySeqSn Update Failed! iRtn = %d",iRtn);
//		return;
//	}
//
//	return;
//}



//void FillRecord ( IGlobalDataManager * ptrGDM,tagUpdateCmdInfo *pUpdateCmdInfo,tagResPonseInfoData  *pResPonseInfoData ,IN_OUT_MSG_TXN_REC *pInTxnRec,tagCardOtcTransDataBak* pTransDataBak,LocalSLEInfo * pLocalInfo,BYTE &bytRtn)
//{
//    OTT_TRADE_REC *pOttTradeRec;
//    OTC_TRADE_REC *pOtcTradeRec;
//    uint32_t     ltmp = 0;
//    uint16_t     wTmp = 0;
//    
//    if((NULL == pResPonseInfoData) || (NULL==pInTxnRec))
//    {
//            ptrGDM->GetUtils()->DebugMsg(&g_traceInfo,"CBOMAnalysisDlg::FillUpdateTxn","(NULL == pResPonseInfoData) || (NULL==pInTxnRec)");
//            return;
//    }
//    
//    
//    pInTxnRec->eInOutFlag = eInFlag;
//    pInTxnRec->eMsgOrTxnFlag = eTxnTradeCodeFlag;
//    pInTxnRec->struInTxnRec.ltxnPosSeqSn = pTransDataBak->m_dwTransSn;  //交易流水号
//    pInTxnRec->struInTxnRec.sSleId = pLocalInfo->local_SLE_id;
//
//    char cTempValue[MAX_PATH];
//    memset(cTempValue,'\0',MAX_PATH);
//    ptrGDM->GetUtils()->UTCToLocal(pTransDataBak->m_dwTransTimePre,(char*)"YMDhms",cTempValue);
//    pInTxnRec->struInTxnRec.sTxnTime = cTempValue;
//    pInTxnRec->struInTxnRec.sLastTime = cTempValue;
//    pInTxnRec->struInTxnRec.lSendFlag = 10; // 发送标记 
//    
//    
//    {
//        switch ( pUpdateCmdInfo->bytChipType )
//        {
//        case 1:      // 单程票
//        {
//            CSC_OTT_BLOCK          *pCSC_OTT_BLOCK;
//            pCSC_OTT_BLOCK = &pResPonseInfoData->tagReaderReadOttResp.struDataBlk;
//
//            pInTxnRec->struInTxnRec.eMessageCode = eOttTradeCode;
//            pInTxnRec->struInTxnRec.lTradecode = eOttTradeCode;
//            pInTxnRec->struInTxnRec.lPackBodyLen = sizeof(OTT_TRADE_REC);
//            
//            pOttTradeRec = ( OTT_TRADE_REC * ) (&pInTxnRec->struInTxnRec.uTradeRec.ottTradeRec);
//            pOttTradeRec->lSAM = pResPonseInfoData->tagReaderResetResp.dwTPUSAMCard;
//// SAM卡号
//            pOttTradeRec->lSAM = pResPonseInfoData->tagReaderResetResp.dwTPUSAMCard;
//            memcpy ( pOttTradeRec->bytSLEID, pLocalInfo->bytSleId, sizeof(pLocalInfo->bytSleId) );
//            
//// 票类型
//            pOttTradeRec->bytTicketType = pCSC_OTT_BLOCK->bytCardType;
//        
//// 卡号        
//        ltmp = *(uint32_t*)(pCSC_OTT_BLOCK->bytCardUniqueCode);
//        pOttTradeRec->lOldCardNumber  = htonl(ltmp);     
//        pOttTradeRec->lNewCardNumber = pOttTradeRec->lOldCardNumber;        
//// 售票设备        
//        pOttTradeRec->bytSaleDevID[0] = pCSC_OTT_BLOCK->uniAllTicket.struDataSJT.bytSaleEq[2];
//        pOttTradeRec->bytSaleDevID[1] = pCSC_OTT_BLOCK->uniAllTicket.struDataSJT.bytSaleEq[1];
//        pOttTradeRec->bytSaleDevID[2] = pCSC_OTT_BLOCK->uniAllTicket.struDataSJT.bytSaleEq[0];
//// 交易前票值       
//        wTmp = *(uint16_t*)(pCSC_OTT_BLOCK->bytBalance);
//        ltmp = wTmp;
//        pOttTradeRec->lValueBeforeTrade = htonl(ltmp);
//// 交易金额        
//        wTmp = *(uint16_t*)(pCSC_OTT_BLOCK->bytBalance);
//        if ( wTmp < pUpdateCmdInfo->dwTransValue )
//        {
//            pOttTradeRec->wTradeVal = wTmp;
//        }
//        else
//        {
//            pOttTradeRec->wTradeVal = pUpdateCmdInfo->dwTransValue;
//        }        
//        pOttTradeRec->wTradeVal = htons(wTmp);  
//// ﻿剩余乘次 ﻿(仅用于乘次票)
//        wTmp = *(uint16_t*)(pCSC_OTT_BLOCK->bytBalance); 
//        wTmp = (wTmp < pUpdateCmdInfo->dwTransValue)?0:(wTmp - pUpdateCmdInfo->dwTransValue);
//        pOttTradeRec->bytRestUseTimes =  wTmp & 0xFF;       
////  ﻿票计数器       
//        wTmp = *(uint16_t*)pCSC_OTT_BLOCK->bytUseTimes;
//        pOttTradeRec->wTicketCounter = htons(wTmp);
////  当前车站  
//        memcpy(pOttTradeRec->bytCurrentStationID,pLocalInfo->bytSleId,sizeof(pLocalInfo->bytSleId));          
//        pOttTradeRec->bytLastTimeStationID[0] = pCSC_OTT_BLOCK->bytTransLine;
//        pOttTradeRec->bytLastTimeStationID[1] = pCSC_OTT_BLOCK->bytTransStation;
//        pOttTradeRec->bytLastTimeStationID[2] = 0x10;
//        pOttTradeRec->bytLastTimeStationID[3] = 0;
//
//        if ( 1 )
//            {
//                if ( pResPonseInfoData->bytRecardInfoCmdCode == CSC_OTT_ENTRY )
//                {
//                    //ptrGDM->GetUtils()->DebugMsg(&g_traceInfo, "FillRecord CSC_OTT_ENTRY\n" );
//                    pOttTradeRec->bytTradeEventCode = pUpdateCmdInfo->bytTransEventCode;
//                    pOttTradeRec->lTradeTime = pTransDataBak->m_dwTransTimePre;
//                    //FillTransRecOTT ( pOttTradeRec, &pResPonseInfoData->tagOttEntryResp, TYPE_CSC_MP_OTT_ENTRY, 0, 0, 0 );
//                }
//                else if ( pResPonseInfoData->bytRecardInfoCmdCode == CSC_OTT_ENTRY2 )
//                {
//                    //ptrGDM->GetUtils()->DebugMsg(&g_traceInfo,"FillRecord CSC_OTT_ENTRY2\n" );
//                    pOttTradeRec->bytTradeEventCode = pUpdateCmdInfo->bytTransEventCode;
//                    pOttTradeRec->lTradeTime = pTransDataBak->m_dwTransTimePre;
//                    //FillTransRecOTT ( pOttTradeRec, &pResPonseInfoData->tagOttSalecardResp, TYPE_CSC_MP_OTT_ENTRY2, 0, 0, 0 );
//                }
//                else if ( pResPonseInfoData->bytRecardInfoCmdCode == CSC_OTT_LOCKCARD )
//                {
//                    //ptrGDM->GetUtils()->DebugMsg(&g_traceInfo,"FillRecord CSC_OTT_LOCKCARD\n" );
//                    pOttTradeRec->lTradeTime = pTransDataBak->m_dwTransTimePre;
//                    //FillTransRecOTT ( pOttTradeRec, &pResPonseInfoData->tagOttLockResp, TYPE_CSC_MP_OTT_LOCKCARD, 0, 0, 0 );
//                }
//                else if ( pResPonseInfoData->bytRecardInfoCmdCode == CSC_OTT_EXIT )
//                {                
//                    pOttTradeRec->wDiscount = pUpdateCmdInfo->wYouHuiValue;
//                    pOttTradeRec->bytTradeEventCode = pUpdateCmdInfo->bytTransEventCode;
//                    pOttTradeRec->lTradeTime = pTransDataBak->m_dwTransTimePre;                 
//                    memcpy ( pOttTradeRec->bytTAC, ( BYTE * ) ( &pResPonseInfoData->tagOttExitResp.dwTAC ), 4 );                
//                }
//                else if ( pResPonseInfoData->bytRecardInfoCmdCode == TPU_EXIT )
//                {
//                    pOttTradeRec->bytTradeEventCode = pUpdateCmdInfo->bytTransEventCode;
//                    pOttTradeRec->wTradeVal = pUpdateCmdInfo->dwTransValue;
//                    pOttTradeRec->lTradeTime = pTransDataBak->m_dwTransTimePre; 
//                    pOttTradeRec->wDiscount = 0;
//                    memcpy ( pOttTradeRec->bytTAC, ( BYTE * ) ( &pResPonseInfoData->tagOttExitResp.dwTAC ), 4 );
//                }
//                else if ( pResPonseInfoData->bytRecardInfoCmdCode == CSC_OTT_LOCKCARD )
//                {
//                    pOttTradeRec->wDiscount = 0;
//                    pOttTradeRec->bytTradeEventCode = pUpdateCmdInfo->bytTransEventCode;
//                    memcpy ( pOttTradeRec->bytTAC, ( BYTE * ) ( &pResPonseInfoData->tagOttLockResp.bytTac ), 4 );
//                }
//
//                if ( pResPonseInfoData->bytRecardInfoCmdCode == CSC_OTT_ENTRY || pResPonseInfoData->bytRecardInfoCmdCode == CSC_OTT_ENTRY2
//                     || pResPonseInfoData->bytRecardInfoCmdCode == CSC_OTT_LOCKCARD || pResPonseInfoData->bytRecardInfoCmdCode == CSC_OTT_EXIT \
//                     || pResPonseInfoData->bytRecardInfoCmdCode == TPU_EXIT   )
//                {
//                    //ptrGDM->GetUtils()->DebugMsg(&g_traceInfo,"FillRecord g_dwSLEOttSerialNo++\n" );
//                    uint32_t  dwSn = pTransDataBak->m_dwTransSn;
//                    ( ( OTT_TRADE_REC * ) pOttTradeRec )->lSLESerialNo = htonl(dwSn);
//                    dwSn++;
//                    GetUpdateSeqSn(dwSn,2,eOttSeqSn);
//                }
//                
//                //ptrGDM->GetUtils()->DebugMsg(&g_traceInfo, "FillRecord TickEND\n" );
//            }
//        }
//        break;
//
//        case 2:      // 交通卡
//        {
//            CSC_MP_READ_CARDINFO *pOtcCardInfo;
//            pOtcCardInfo = &(pResPonseInfoData->tagReaderOtcResp);            
//            pOtcTradeRec = ( OTC_TRADE_REC * ) (&pInTxnRec->struInTxnRec.uTradeRec.otcTradeRec);
//
//            pInTxnRec->struInTxnRec.eMessageCode = eOtcTradeCode;
//            pInTxnRec->struInTxnRec.lTradecode = eOtcTradeCode;
//            pInTxnRec->struInTxnRec.lPackBodyLen = sizeof(OTC_TRADE_REC);
//
//            memset(pOtcTradeRec,0,sizeof(OTC_TRADE_REC));
//
//            pOtcTradeRec->lSAM = pResPonseInfoData->tagReaderResetResp.dwCSCSAMCard;
//            memcpy ( pOtcTradeRec->bytSLEID, pLocalInfo->bytSleId, 4 );
//            
//            ltmp= pUpdateCmdInfo->dwTransTime;
//            pOtcTradeRec->lTradeTime = htonl(ltmp);
//
//// 
//            ltmp =  pOtcCardInfo->GetCardBalance();   
//            pOtcTradeRec->lValueBeforeTrade = htonl(ltmp);
//            
////          
//          
//            if ( pOtcCardInfo->bytDataBlkNo == 0x82 )     // M1卡
//            {
//                CSC_OTC_81MODE_BLOCK_1_AGM       *pCSC_OTC_BLOCK_1;
//                CSC_OTC_TRANS_RECORD       *pCSC_OTC_BLOCK_2;
//                
//                pCSC_OTC_BLOCK_1 = &pOtcCardInfo->strOtcBlock1.strM1Block;
//                pCSC_OTC_BLOCK_2 = &pOtcCardInfo->strEntryRec;
//                // Fill Record By BLock 1                
//                //FillTransRecOTC ( pOtcTradeRec, &pResPonseInfoData->tagReaderOtcResp.strOtcBlock1.strM1Block, TYPE_CSC_OTC_BLOCK_1, g_bIsExitTEntryStation, 0, 0 );
//                pOtcTradeRec->bytTicketType = pCSC_OTC_BLOCK_1->bytCardType;
//                pOtcTradeRec->lOldCardNumber = pCSC_OTC_BLOCK_1->dwCardUniqueCode;
//
//// ？？？               
//                if ( pUpdateCmdInfo-> bytOldFlag) // 老人卡标记
//                {
//                    pOtcTradeRec->lOperatorCode |= 1 << 31;
//                }
//// 卡计数器
//                wTmp = pUpdateCmdInfo->wCardCounter;
//                pOtcTradeRec->wCardCounter =  htons(wTmp);
//// 城市代码                
//            wTmp = pCSC_OTC_BLOCK_1->wCityID;
//            pOtcTradeRec->wCityCode = htons(wTmp);  
//
//            pOtcTradeRec->bytAuthorizationTime[5] = 0x01;
//            pOtcTradeRec->bytAuthorizationTime[6] = pUpdateCmdInfo->bytCardVersion;
//
//
//
//
//            if ( pUpdateCmdInfo->bytExitTEntryFlag)  // 是出站换乘进站
//            {
//                //memcpy(pOtcTradeRec->bytCurrentStationID,pCSC_OTC_BLOCK_2->bytLastUsedLine;//->bytSleId,sizeof(pLocalInfo->bytSleId)); //当前车站
//// ﻿上传扣款金额为上次出站记录中的扣款金额、当前车站为出站记录中的交易设备车站计算机的节点标识码、上次使用车站为进站记录中的交易设备车站计算机的节点标识码
//                wTmp = pCSC_OTC_BLOCK_2->uniTrans.strNormal.wTransAmount;
//                pOtcTradeRec->wTradeVal = htons(wTmp);
//                
//                pOtcTradeRec->bytCurrentStationID[0] = pCSC_OTC_BLOCK_2->bytLastUsedLine;
//                pOtcTradeRec->bytCurrentStationID[1] = pCSC_OTC_BLOCK_2->bytLastUsedStation;
//                pOtcTradeRec->bytCurrentStationID[2] = 0x10;
//                pOtcTradeRec->bytCurrentStationID[3] = 0x0;
//                    
//                pOtcTradeRec->bytLastTimeStationID[0] = pCSC_OTC_BLOCK_1->uniOTCDifInfo.struNormalOTC.bytAnotherTransLine;//->bytLastUsedLine;
//                pOtcTradeRec->bytLastTimeStationID[1] = pCSC_OTC_BLOCK_1->uniOTCDifInfo.struNormalOTC.bytAnotherTransStation;
//                pOtcTradeRec->bytLastTimeStationID[2] = pCSC_OTC_BLOCK_1->uniOTCDifInfo.struNormalOTC.bytAnotherTransEqCode;
//                pOtcTradeRec->bytLastTimeStationID[3] = 0; //pCSC_OTC_BLOCK_2->bytLastUsedEqCode;
//            } 
//            else
//            {
//                memcpy(pOtcTradeRec->bytCurrentStationID,pLocalInfo->bytSleId,sizeof(pLocalInfo->bytSleId)); //当前车站
//                pOtcTradeRec->wTradeVal = 0;             
//                pOtcTradeRec->bytLastTimeStationID[0] = pCSC_OTC_BLOCK_2->bytLastUsedLine;
//                pOtcTradeRec->bytLastTimeStationID[1] = pCSC_OTC_BLOCK_2->bytLastUsedStation;
//                pOtcTradeRec->bytLastTimeStationID[2] = 0x10;
//                pOtcTradeRec->bytLastTimeStationID[3] = 0x0;
//            }
//                // Fill Record By BLock 2                
//                // FillTransRecOTC ( pOtcTradeRec, &pResPonseInfoData->tagReaderOtcResp.strEntryRec, TYPE_CSC_OTC_BLOCK_2, g_bIsExitTEntryStation, 0, 0 );  
//                if ( pResPonseInfoData->bytRecardInfoCmdCode == CSC_OTC_ENTRY )
//                {
//                    //FillTransRecOTC ( pOtcTradeRec, &g_RespCmd, TYPE_CSC_MP_OTC_ENTRY, 0, 0, g_lOTCEntryCumuValue );
//                   pOtcTradeRec->bytTradeEventCode = pUpdateCmdInfo->bytTransEventCode;
//                   pOtcTradeRec->wDiscount = 0;        //进站交易优惠金额一律填零
//                   uint32_t  dwCummValue = pOtcCardInfo->strEntryRec.uniTrans.strNormal.GetAccumuBalance();
//                   pOtcTradeRec->bytCumuValue[0] = ( BYTE ) ( ( dwCummValue >> 16 ) & 0xFF );
//                   pOtcTradeRec->bytCumuValue[1] = ( BYTE ) ( ( dwCummValue >> 8 ) & 0xFF );
//                   pOtcTradeRec->bytCumuValue[2] = ( BYTE ) ( dwCummValue & 0xFF );
//                   pOtcTradeRec->bytCardTradeFlag = 0x88;      //bytOtcTransFlag;
//                   memcpy ( pOtcTradeRec->bytTAC, ( BYTE * ) ( &pResPonseInfoData->tagOtcEntryResp.dwTAC ), 4 );
//                }
//                else if ( pResPonseInfoData->bytRecardInfoCmdCode == CSC_OTC_LOCKCARD )
//                {
//                    // FillTransRecOTC ( pOtcTradeRec, &g_RespCmd, TYPE_CSC_MP_OTC_LOCKCARD, 0, 0, 0 );
//                    pOtcTradeRec->wTradeVal = 0;
//                    pOtcTradeRec->bytTradeEventCode = pUpdateCmdInfo->bytTransEventCode;;
//                    pOtcTradeRec->wDiscount = 0;
//                    if ( pResPonseInfoData->tagLockcardResp.bytRespCode == 1 )
//                    {
//                        pOtcTradeRec->bytCardTradeFlag = 0x99;
//                    }
//                    else
//                    {
//                        pOtcTradeRec->bytCardTradeFlag = 0x98;
//                    }
//                    memset ( pOtcTradeRec->bytTAC, 0x00, 4 );                    
//                }
//                else if( pResPonseInfoData->bytRecardInfoCmdCode == CSC_OTC_EXIT )
//                {
//                    wTmp = pUpdateCmdInfo->dwTransValue;;
//                    pOtcTradeRec->wTradeVal = htons(wTmp);
//                    pOtcTradeRec->bytTradeEventCode = pUpdateCmdInfo->bytTransEventCode;
//                    
//                    wTmp = pUpdateCmdInfo->wYouHuiValue;
//                    pOtcTradeRec->wDiscount = htons(wTmp);
//                    uint32_t  dwCummValue = pOtcCardInfo->strEntryRec.uniTrans.strNormal.GetAccumuBalance()+pUpdateCmdInfo->dwTransValue;
//                    pOtcTradeRec->bytCumuValue[0] = ( BYTE ) ( ( dwCummValue >> 16 ) & 0xFF );
//                    pOtcTradeRec->bytCumuValue[1] = ( BYTE ) ( ( dwCummValue >> 8 ) & 0xFF );
//                    pOtcTradeRec->bytCumuValue[2] = ( BYTE ) ( dwCummValue & 0xFF );
//                    pOtcTradeRec->bytCardTradeFlag = pUpdateCmdInfo->bytTransFlag;
//                    memcpy ( pOtcTradeRec->bytTAC, ( BYTE * ) ( &pResPonseInfoData->tagOtcExitResp.dwTAC ), 4 );
//                }
//            }
//            else if ( pOtcCardInfo->bytDataBlkNo == 0x92 ) // CPU卡
//            {
//                // fill CPU Block1
//                //FillTransRecOTC ( pOtcTradeRec, g_EntryCpuBlk.pBlk1, TYPE_CSC_CPU_BLOCK_1, g_bIsExitTEntryStation, 0, 0 );
//                CSC_CPU_91MODE_BLOCK_1_AGM       *pCSC_CPU_BLOCK_1;
//                CSC_OTC_TRANS_RECORD       *pCSC_CPU_BLOCK_2;
//                pCSC_CPU_BLOCK_1 = &pResPonseInfoData->tagReaderOtcResp.strOtcBlock1.strCpuBlock;
//            pCSC_CPU_BLOCK_2 = &pResPonseInfoData->tagReaderOtcResp.strEntryRec;
//            pOtcTradeRec->bytTicketType = pCSC_CPU_BLOCK_1->bytCardType;
//            
//            ltmp = pCSC_CPU_BLOCK_1->dwAppCode1;
//            pOtcTradeRec->lOldCardNumber = htonl(ltmp);
//            
//            ltmp = pCSC_CPU_BLOCK_1->bytAppCode2;
//            pOtcTradeRec->lNewCardNumber = htonl(ltmp);
//            
//            wTmp = pCSC_CPU_BLOCK_1->wCardCounter;
//            pOtcTradeRec->wCardCounter = htons(wTmp);
//            
//            ltmp = pOtcCardInfo->GetCardBalance();
//            pOtcTradeRec->lValueBeforeTrade = htonl(ltmp);
//            
//            wTmp = pCSC_CPU_BLOCK_1->wCityID;
//            pOtcTradeRec->wCityCode = htons(wTmp);
//            
//            ltmp = pUpdateCmdInfo->dwTransTime;
//            pOtcTradeRec->lTradeTime = htonl(ltmp);
//                     
//                     
//            pOtcTradeRec->bytAuthorizationTime[5] = ( pCSC_CPU_BLOCK_1->bytArithmeticID & 0x0F );
//            pOtcTradeRec->bytAuthorizationTime[6] = pCSC_CPU_BLOCK_1->bytCardVersion;
//            
//            
//            if ( pUpdateCmdInfo->bytExitTEntryFlag)  // 是出站换乘进站
//            {
//                //memcpy(pOtcTradeRec->bytCurrentStationID,pCSC_OTC_BLOCK_2->bytLastUsedLine;//->bytSleId,sizeof(pLocalInfo->bytSleId)); //当前车站
//// ﻿上传扣款金额为上次出站记录中的扣款金额、当前车站为出站记录中的交易设备车站计算机的节点标识码、上次使用车站为进站记录中的交易设备车站计算机的节点标识码
//                wTmp = pCSC_CPU_BLOCK_2->uniTrans.strNormal.wTransAmount;
//                pOtcTradeRec->wTradeVal = htons(wTmp);
//                
//                pOtcTradeRec->bytCurrentStationID[0] = pCSC_CPU_BLOCK_2->bytLastUsedLine;
//                pOtcTradeRec->bytCurrentStationID[1] = pCSC_CPU_BLOCK_2->bytLastUsedStation;
//                pOtcTradeRec->bytCurrentStationID[2] = 0x10;
//                pOtcTradeRec->bytCurrentStationID[3] = 0x0;
//                
//                pOtcTradeRec->bytLastTimeStationID[0] = pCSC_CPU_BLOCK_1->bytAnotherTransLine;//->bytLastUsedLine;
//                pOtcTradeRec->bytLastTimeStationID[1] = pCSC_CPU_BLOCK_1->bytAnotherTransStation;
//                pOtcTradeRec->bytLastTimeStationID[2] = pCSC_CPU_BLOCK_1->bytAnotherTransEqCode;
//                pOtcTradeRec->bytLastTimeStationID[3] = 0; //pCSC_OTC_BLOCK_2->bytLastUsedEqCode;
//            } 
//            else
//            {
//                memcpy(pOtcTradeRec->bytCurrentStationID,pLocalInfo->bytSleId,sizeof(pLocalInfo->bytSleId)); //当前车站
//                pOtcTradeRec->wTradeVal = 0;             
//                pOtcTradeRec->bytLastTimeStationID[0] = pCSC_CPU_BLOCK_2->bytLastUsedLine;
//                pOtcTradeRec->bytLastTimeStationID[1] = pCSC_CPU_BLOCK_2->bytLastUsedStation;
//                pOtcTradeRec->bytLastTimeStationID[2] = 0x10;
//                pOtcTradeRec->bytLastTimeStationID[3] = 0x0;
//            }               
//               // fill CPU Block2
//                //FillTransRecOTC ( pOtcTradeRec, g_EntryCpuBlk.pBlk2, TYPE_CSC_CPU_BLOCK_2, g_bIsExitTEntryStation, 0, 0 );
//
//                if ( pResPonseInfoData->bytRecardInfoCmdCode == CSC_OTC_ENTRY )
//                {
//                    //FillTransRecOTC ( pOtcTradeRec, &g_RespCmd, TYPE_CSC_MP_CPU_ENTRY, 0, 0, g_lOTCEntryCumuValue );
//                    //交易金额在填写进站交易时已填写
//                    pOtcTradeRec->bytTradeEventCode = pUpdateCmdInfo->bytTransEventCode;;
//                    pOtcTradeRec->wDiscount = 0;        //进站交易优惠金额一律填零
//                    uint32_t  dwCummValue = pOtcCardInfo->strEntryRec.uniTrans.strNormal.GetAccumuBalance();        
//                    pOtcTradeRec->bytCumuValue[0] = ( BYTE ) ( ( dwCummValue >> 16 ) & 0xFF );
//                    pOtcTradeRec->bytCumuValue[1] = ( BYTE ) ( ( dwCummValue >> 8 ) & 0xFF );
//                    pOtcTradeRec->bytCumuValue[2] = ( BYTE ) ( dwCummValue & 0xFF );
//                    pOtcTradeRec->bytCardTradeFlag = 0x88;//bytOtcTransFlag;
//                    memcpy ( pOtcTradeRec->bytTAC, ( BYTE * ) ( &pResPonseInfoData->tagOtcEntryResp.dwTAC ), 4 );
//                
//                }
//                else if ( pResPonseInfoData->bytRecardInfoCmdCode == CSC_CPU_LOCKCARD )
//                {
//                    //FillTransRecOTC ( pOtcTradeRec, &g_RespCmd, TYPE_CSC_MP_CPU_LOCKCARD, 0, 0, 0 );
//                    pOtcTradeRec->wTradeVal = 0;
//                    pOtcTradeRec->bytTradeEventCode = pUpdateCmdInfo->bytTransEventCode;;
//                    pOtcTradeRec->wCardCounter = 0;
//                    pOtcTradeRec->wDiscount = 0;
//                    if ( pResPonseInfoData->tagLockcardResp.bytRespCode == 1 )
//                    {
//                        pOtcTradeRec->bytCardTradeFlag = 0x99;
//                    }
//                    else
//                    {
//                        pOtcTradeRec->bytCardTradeFlag = 0x98;
//                    }
//                    memset ( pOtcTradeRec->bytTAC, 0x00, 4 );                    
//                }
//                else if( pResPonseInfoData->bytRecardInfoCmdCode == CSC_OTC_EXIT )
//                {
//                    wTmp = pUpdateCmdInfo->dwTransValue;;
//                    pOtcTradeRec->wTradeVal = htons(wTmp);
//                    
//                    pOtcTradeRec->bytTradeEventCode = pUpdateCmdInfo->bytTransEventCode;
//                    
//                    wTmp = pUpdateCmdInfo->wYouHuiValue;
//                    pOtcTradeRec->wDiscount = htons(wTmp);
//                    
//                    uint32_t  dwCummValue = pOtcCardInfo->strEntryRec.uniTrans.strNormal.GetAccumuBalance()+pUpdateCmdInfo->dwTransValue;
//                    pOtcTradeRec->bytCumuValue[0] = ( BYTE ) ( ( dwCummValue >> 16 ) & 0xFF );
//                    pOtcTradeRec->bytCumuValue[1] = ( BYTE ) ( ( dwCummValue >> 8 ) & 0xFF );
//                    pOtcTradeRec->bytCumuValue[2] = ( BYTE ) ( dwCummValue & 0xFF );
//                    pOtcTradeRec->bytCardTradeFlag = pUpdateCmdInfo->bytTransFlag;
//                    memcpy ( pOtcTradeRec->bytTAC, ( BYTE * ) ( &pResPonseInfoData->tagOtcExitResp.dwTAC ), 4 );
//                }
//            }
//            else
//            {
//                ptrGDM->GetUtils()->DebugMsg(&g_traceInfo, "FillRecord OTC Invalid! NO M1 or CPU 卡\n" ,"OK");
//                
//            }
//
//            if ( pResPonseInfoData->bytRecardInfoCmdCode == CSC_OTC_ENTRY || pResPonseInfoData->bytRecardInfoCmdCode == CSC_OTC_LOCKCARD
//                 || pResPonseInfoData->bytRecardInfoCmdCode == CSC_CPU_LOCKCARD || pResPonseInfoData->bytRecardInfoCmdCode == CSC_OTC_EXIT )
//            {
//                uint32_t  dwSn = pTransDataBak->m_dwTransSn;                
//                ( ( OTC_TRADE_REC * ) pOtcTradeRec )->lSLESerialNo = htonl(dwSn);
//                dwSn++;
//                GetUpdateSeqSn(dwSn,2,eOtcSeqSn);
//            }
//        }
//        break;
//
//        default:
//            break;
//        }
//    }
//	bytRtn = ptrGDM->GetSQLITEDBManagerProxy()->DBInsertRecInfo(IN_TRADE_REC_FLAG,&pInTxnRec->struInTxnRec,NULL);
//	if(bytRtn)
//	{
//		ptrGDM->GetUtils()->DebugMsg(&g_traceInfo,"FillRecord","DBInsertRecInfo(IN_TRADE_REC_FLAG) Failed! bytRtn = %d",bytRtn);
//		return;
//	}
//
////	ModifyRegist(bytUpdateTypeTemp,pUpdateCmdInfo,&m_pParent->m_InOutUpdateSeqSn,(int&)bytRtn);
////	if(bytRtn)
////	{
////		m_pGDM->GetUtils()->DebugMsg(&m_tracelog,"CBOMAnalysisDlg::FillUpdateTxn","ModifyRegist eCSC_OTT_UPDATE Failed! bytRtn = %d",bytRtn);
////		return;
////	}
//	return;
//
//}

void FillRecord ( IGlobalDataManager * ptrGDM,tagUpdateCmdInfo *pUpdateCmdInfo,tagResPonseInfoData  *pResPonseInfoData ,IN_OUT_MSG_TXN_REC *pInTxnRec,tagCardOtcTransDataBak* pTransDataBak,LocalSLEInfo * pLocalInfo,BYTE &bytRtn)
{
    OTT_TRADE_REC      *pOttTradeRec;
    OTC_TRADE_REC      *pOtcTradeRec;
   
    
    uint32_t     ltmp = 0;
    uint16_t     wTmp = 0;
    
    if((NULL == pResPonseInfoData) || (NULL==pInTxnRec))
    {
            ptrGDM->GetUtils()->DebugMsg(ptrGDM->GetTracelog(),"CBOMAnalysisDlg::FillUpdateTxn","(NULL == pResPonseInfoData) || (NULL==pInTxnRec)");
            return;
    }
     
    pInTxnRec->eInOutFlag = eInFlag;
    pInTxnRec->eMsgOrTxnFlag = eTxnTradeCodeFlag;
    pInTxnRec->struInTxnRec.ltxnPosSeqSn = 0;  //交易流水号
    pInTxnRec->struInTxnRec.sSleId = pLocalInfo->local_SLE_id;

    char cTempValue[MAX_PATH];
    memset(cTempValue,'\0',MAX_PATH);
    ptrGDM->GetUtils()->getLocalTime((char*)"YMDhms",cTempValue);
    pInTxnRec->struInTxnRec.sTxnTime = cTempValue;
    pInTxnRec->struInTxnRec.sLastTime = cTempValue;
    pInTxnRec->struInTxnRec.lSendFlag = 10; // 发送标记 
    pInTxnRec->struInTxnRec.lTradeEventCode = pUpdateCmdInfo->bytTransEventCode;
    
    
    {
        switch ( pUpdateCmdInfo->bytChipType )
        {
        case 1:      // 单程票
        {
            CSC_OTT_BLOCK          *pCSC_OTT_BLOCK;

            
            pCSC_OTT_BLOCK = &pResPonseInfoData->tagReaderReadOttResp.struDataBlk;

            pInTxnRec->struInTxnRec.eMessageCode = eOttTradeCode;
            pInTxnRec->struInTxnRec.lTradecode = eOttTradeCode;
            pInTxnRec->struInTxnRec.lPackBodyLen = sizeof(OTT_TRADE_REC);
            
            pOttTradeRec = ( OTT_TRADE_REC * ) (&pInTxnRec->struInTxnRec.uTradeRec.ottTradeRec);
            memset(pOttTradeRec,0,sizeof(OTT_TRADE_REC));
// SAM卡号
            ltmp = pResPonseInfoData->tagReaderResetResp.dwTPUSAMCard;
            pOttTradeRec->lSAM = htonl(ltmp);
            memcpy ( pOttTradeRec->bytSLEID, pLocalInfo->bytSleId, sizeof(pLocalInfo->bytSleId) );
            
// 票类型
         pOttTradeRec->bytTicketType = pCSC_OTT_BLOCK->bytCardType;
// 卡号        
        ltmp = *(uint32_t*)(pCSC_OTT_BLOCK->bytCardUniqueCode);
        pOttTradeRec->lOldCardNumber  = htonl(ltmp);     
        pOttTradeRec->lNewCardNumber = 0;        
// 售票设备        
        pOttTradeRec->bytSaleDevID[0] = pCSC_OTT_BLOCK->uniAllTicket.struDataSJT.bytSaleEq[2];
        pOttTradeRec->bytSaleDevID[1] = pCSC_OTT_BLOCK->uniAllTicket.struDataSJT.bytSaleEq[1];
        pOttTradeRec->bytSaleDevID[2] = pCSC_OTT_BLOCK->uniAllTicket.struDataSJT.bytSaleEq[0];
// 交易前票值       
        wTmp = *(uint16_t*)(pCSC_OTT_BLOCK->bytBalance);
        ltmp = wTmp;//pUpdateCmdInfo->dwAfterTransValue;
        pOttTradeRec->lValueBeforeTrade = htonl(ltmp);
 
// 交易时间
        ltmp= pUpdateCmdInfo->dwTransTime;
        pOttTradeRec->lTradeTime = htonl(ltmp);        
// ??剩余乘次 ??(仅用于乘次票)
        if(pUpdateCmdInfo->bytDisplayType != 0x1)
        {
//            wTmp = *(uint16_t*)(pCSC_OTT_BLOCK->bytBalance); 
//            wTmp = (wTmp < pUpdateCmdInfo->dwTransValue)?0:(wTmp - pUpdateCmdInfo->dwTransValue);
            pOttTradeRec->bytRestUseTimes =  pUpdateCmdInfo->dwAfterTransValue;   //wTmp & 0xFF;   
        }
//  ??票计数器       
        wTmp = *(uint16_t*)pCSC_OTT_BLOCK->bytUseTimes;
        pOttTradeRec->wTicketCounter = htons(wTmp);
//  当前车站  
        memcpy(pOttTradeRec->bytCurrentStationID,pLocalInfo->bytSleId,sizeof(pLocalInfo->bytSleId));  
        pOttTradeRec->bytCurrentStationID[2] = 0x10;
        pOttTradeRec->bytCurrentStationID[3] = 0x0;        
        pOttTradeRec->bytLastTimeStationID[0] = pCSC_OTT_BLOCK->bytTransLine;
        pOttTradeRec->bytLastTimeStationID[1] = pCSC_OTT_BLOCK->bytTransStation;
        pOttTradeRec->bytLastTimeStationID[2] = 0x10;
        pOttTradeRec->bytLastTimeStationID[3] = 0;

        if ( 1 )
            {
                if ( pResPonseInfoData->bytRecardInfoCmdCode == CSC_OTT_ENTRY )
                {
  
                    pOttTradeRec->bytTradeEventCode = pUpdateCmdInfo->bytTransEventCode;
//                    pOttTradeRec->lTradeTime = pTransDataBak->m_dwTransTimePre;
                    memcpy ( pOttTradeRec->bytTAC, ( BYTE * ) ( &pResPonseInfoData->tagOttEntryResp.dwTAC ), 4 );
   
                }
                else if ( pResPonseInfoData->bytRecardInfoCmdCode == CSC_OTT_ENTRY2 )
                {
            
                    pOttTradeRec->bytTradeEventCode = pUpdateCmdInfo->bytTransEventCode;
                    memcpy ( pOttTradeRec->bytTAC, ( BYTE * ) ( &pResPonseInfoData->tagOttEntryResp.dwTAC ), 4 );

//                    pOttTradeRec->lTradeTime = pTransDataBak->m_dwTransTimePre;
               
                }
                else if ( pResPonseInfoData->bytRecardInfoCmdCode == CSC_OTT_LOCKCARD )
                {
        
//                    pOttTradeRec->lTradeTime = pTransDataBak->m_dwTransTimePre;
                  
                }
                else if ( pResPonseInfoData->bytRecardInfoCmdCode == CSC_OTT_EXIT )
                {                
                    pOttTradeRec->wDiscount = pUpdateCmdInfo->wYouHuiValue;
                    pOttTradeRec->bytTradeEventCode = pUpdateCmdInfo->bytTransEventCode;
                    ltmp = pUpdateCmdInfo->dwEnTime;
                    pOttTradeRec->lOperatorCode = htonl(ltmp);
// 交易金额        
        wTmp = pUpdateCmdInfo->dwTransValue;       
        pOttTradeRec->wTradeVal = htons(wTmp);                     
                    memcpy ( pOttTradeRec->bytTAC, ( BYTE * ) ( &pResPonseInfoData->tagOttExitResp.dwTAC ), 4 );                
                }
                else if ( pResPonseInfoData->bytRecardInfoCmdCode == TPU_EXIT )
                {
                    pOttTradeRec->bytTradeEventCode = pUpdateCmdInfo->bytTransEventCode;
                    pOttTradeRec->wTradeVal = pUpdateCmdInfo->dwTransValue;
                    
                    ltmp = pUpdateCmdInfo->dwEnTime;
                    pOttTradeRec->lOperatorCode = htonl(ltmp);
// 交易金额        
            wTmp = pUpdateCmdInfo->dwTransValue;       
            pOttTradeRec->wTradeVal = htons(wTmp);                     
                    pOttTradeRec->wDiscount = 0;
                    memcpy ( pOttTradeRec->bytTAC, ( BYTE * ) ( &pResPonseInfoData->tagOttExitResp.dwTAC ), 4 );
                }
                else if ( pResPonseInfoData->bytRecardInfoCmdCode == CSC_OTT_LOCKCARD )
                {
                    pOttTradeRec->wDiscount = 0;
                    pOttTradeRec->bytTradeEventCode = pUpdateCmdInfo->bytTransEventCode;
                    memcpy ( pOttTradeRec->bytTAC, ( BYTE * ) ( &pResPonseInfoData->tagOttLockResp.bytTac ), 4 );
                }

                if ( pResPonseInfoData->bytRecardInfoCmdCode == CSC_OTT_ENTRY || pResPonseInfoData->bytRecardInfoCmdCode == CSC_OTT_ENTRY2
                     || pResPonseInfoData->bytRecardInfoCmdCode == CSC_OTT_LOCKCARD || pResPonseInfoData->bytRecardInfoCmdCode == CSC_OTT_EXIT \
                     || pResPonseInfoData->bytRecardInfoCmdCode == TPU_EXIT   )
                {
                  
                    uint32_t  dwSn =0;
                    GetUpdateSeqSn(dwSn,1,eOttSeqSn);
                    pInTxnRec->struInTxnRec.ltxnPosSeqSn = dwSn;
                    ( ( OTT_TRADE_REC * ) pOttTradeRec )->lSLESerialNo = htonl(dwSn);
                    dwSn++;
                    GetUpdateSeqSn(dwSn,2,eOttSeqSn);
                }
            }            
        }
        break;

        case 2:      // 交通卡
        {
            CSC_MP_READ_CARDINFO *pOtcCardInfo;
            pOtcCardInfo = &(pResPonseInfoData->tagReaderOtcResp);            
            pOtcTradeRec = ( OTC_TRADE_REC * ) (&pInTxnRec->struInTxnRec.uTradeRec.otcTradeRec);

            pInTxnRec->struInTxnRec.eMessageCode = eOtcTradeCode;
            pInTxnRec->struInTxnRec.lTradecode = eOtcTradeCode;
            pInTxnRec->struInTxnRec.lPackBodyLen = sizeof(OTC_TRADE_REC);

            memset(pOtcTradeRec,0,sizeof(OTC_TRADE_REC));

            pOtcTradeRec->lSAM = pResPonseInfoData->tagReaderResetResp.dwCSCSAMCard;
            memcpy ( pOtcTradeRec->bytSLEID, pLocalInfo->bytSleId, 4 );
            
            ltmp= pUpdateCmdInfo->dwTransTime;
            pOtcTradeRec->lTradeTime = htonl(ltmp);

// 
            ltmp =  pOtcCardInfo->GetCardBalance();   
            pOtcTradeRec->lValueBeforeTrade = htonl(ltmp);
            
//          
          
            if ( pOtcCardInfo->bytDataBlkNo == 0x83 )     // M1卡
            {
                CSC_OTC_81MODE_BLOCK_1_AGM       *pCSC_OTC_BLOCK_1;
                CSC_OTC_TRANS_RECORD       *pCSC_OTC_BLOCK_2;
                
                pCSC_OTC_BLOCK_1 = &pOtcCardInfo->strOtcBlock1.strM1Block;
                pCSC_OTC_BLOCK_2 = &pOtcCardInfo->strEntryRec;
    
                pOtcTradeRec->bytTicketType = pCSC_OTC_BLOCK_1->bytCardType;
                
                ltmp = pCSC_OTC_BLOCK_1->dwCardUniqueCode;
                pOtcTradeRec->lOldCardNumber = (ltmp);

// ？？？               
//                if ( pUpdateCmdInfo-> bytOldFlag) // 老人卡标记
//                {
//                    pOtcTradeRec->lOperatorCode |= 1 << 31;
//                }
// 卡计数器
                wTmp = pUpdateCmdInfo->wCardCounter;
                pOtcTradeRec->wCardCounter =  (wTmp);  
// 城市代码                
            wTmp = pCSC_OTC_BLOCK_1->wCityID;
            pOtcTradeRec->wCityCode = (wTmp);  

            pOtcTradeRec->bytAuthorizationTime[5] = 0x01;
            pOtcTradeRec->bytAuthorizationTime[6] = pUpdateCmdInfo->bytCardVersion;




            if ( pUpdateCmdInfo->bytExitTEntryFlag)  // 是出站换乘进站
            {
// ??上传扣款金额为上次出站记录中的扣款金额、当前车站为出站记录中的交易设备车站计算机的节点标识码、上次使用车站为进站记录中的交易设备车站计算机的节点标识码
                wTmp = pCSC_OTC_BLOCK_2->uniTrans.strNormal.wTransAmount;
                pOtcTradeRec->wTradeVal = htons(wTmp);
                
                pOtcTradeRec->bytCurrentStationID[0] = pCSC_OTC_BLOCK_2->bytLastUsedLine;
                pOtcTradeRec->bytCurrentStationID[1] = pCSC_OTC_BLOCK_2->bytLastUsedStation;
                pOtcTradeRec->bytCurrentStationID[2] = 0x10;
                pOtcTradeRec->bytCurrentStationID[3] = 0x0;
                    
                pOtcTradeRec->bytLastTimeStationID[0] = pCSC_OTC_BLOCK_1->uniOTCDifInfo.struNormalOTC.bytAnotherTransLine;//->bytLastUsedLine;
                pOtcTradeRec->bytLastTimeStationID[1] = pCSC_OTC_BLOCK_1->uniOTCDifInfo.struNormalOTC.bytAnotherTransStation;
                pOtcTradeRec->bytLastTimeStationID[2] = pCSC_OTC_BLOCK_1->uniOTCDifInfo.struNormalOTC.bytAnotherTransEqCode;
                pOtcTradeRec->bytLastTimeStationID[3] = 0; //pCSC_OTC_BLOCK_2->bytLastUsedEqCode;
            } 
            else
            {
                memcpy(pOtcTradeRec->bytCurrentStationID,pLocalInfo->bytSleId,sizeof(pLocalInfo->bytSleId)); //当前车站
                pOtcTradeRec->bytCurrentStationID[2] = 0x10;
                pOtcTradeRec->bytCurrentStationID[3] = 0x0;                
                pOtcTradeRec->wTradeVal = 0;             
                pOtcTradeRec->bytLastTimeStationID[0] = pCSC_OTC_BLOCK_2->bytLastUsedLine;
                pOtcTradeRec->bytLastTimeStationID[1] = pCSC_OTC_BLOCK_2->bytLastUsedStation;
                pOtcTradeRec->bytLastTimeStationID[2] = 0x10;
                pOtcTradeRec->bytLastTimeStationID[3] = 0x0;
            }
                if ( pResPonseInfoData->bytRecardInfoCmdCode == CSC_OTC_ENTRY )
                {
// 卡计数器
                wTmp = pResPonseInfoData->tagOtcEntryResp.wTransCount;
                pOtcTradeRec->wCardCounter =  (wTmp);  
                
                   pOtcTradeRec->bytTradeEventCode = pUpdateCmdInfo->bytTransEventCode;
                   pOtcTradeRec->wDiscount = 0;        //进站交易优惠金额一律填零
                   uint32_t  dwCummValue = pOtcCardInfo->strEntryRec.uniTrans.strNormal.GetAccumuBalance();
                   pOtcTradeRec->bytCumuValue[0] = ( BYTE ) ( ( dwCummValue >> 16 ) & 0xFF );
                   pOtcTradeRec->bytCumuValue[1] = ( BYTE ) ( ( dwCummValue >> 8 ) & 0xFF );
                   pOtcTradeRec->bytCumuValue[2] = ( BYTE ) ( dwCummValue & 0xFF );
                   pOtcTradeRec->bytCardTradeFlag = 0x88;      //bytOtcTransFlag;
                   memcpy ( pOtcTradeRec->bytTAC, ( BYTE * ) ( &pResPonseInfoData->tagOtcEntryResp.dwTAC ), 4 );
                }
                else if ( pResPonseInfoData->bytRecardInfoCmdCode == CSC_OTC_LOCKCARD )
                {
                    pOtcTradeRec->wTradeVal = 0;
                    pOtcTradeRec->bytTradeEventCode = pUpdateCmdInfo->bytTransEventCode;;
                    pOtcTradeRec->wDiscount = 0;
                    if ( pResPonseInfoData->tagLockcardResp.bytRespCode == 1 )
                    {
                        pOtcTradeRec->bytCardTradeFlag = 0x99;
                    }
                    else
                    {
                        pOtcTradeRec->bytCardTradeFlag = 0x98;
                    }
                    memset ( pOtcTradeRec->bytTAC, 0x00, 4 );                    
                }
                else if( pResPonseInfoData->bytRecardInfoCmdCode == CSC_OTC_EXIT )
                {
// 卡计数器
                wTmp = pResPonseInfoData->tagOtcExitResp.wTransCount;
                pOtcTradeRec->wCardCounter =  (wTmp); 
//                 
                    wTmp = pUpdateCmdInfo->dwTransValue;
                    pOtcTradeRec->wTradeVal = htons(wTmp);
                    pOtcTradeRec->bytTradeEventCode = pUpdateCmdInfo->bytTransEventCode;
//
                    ltmp = pUpdateCmdInfo->dwEnTime;
                    pOtcTradeRec->lOperatorCode = htonl(ltmp);
//                    
                    wTmp = pUpdateCmdInfo->wYouHuiValue;
                    pOtcTradeRec->wDiscount = htons(wTmp);
                    uint32_t  dwCummValue = pOtcCardInfo->strEntryRec.uniTrans.strNormal.GetAccumuBalance()+pUpdateCmdInfo->dwTransValue;
                    pOtcTradeRec->bytCumuValue[0] = ( BYTE ) ( ( dwCummValue >> 16 ) & 0xFF );
                    pOtcTradeRec->bytCumuValue[1] = ( BYTE ) ( ( dwCummValue >> 8 ) & 0xFF );
                    pOtcTradeRec->bytCumuValue[2] = ( BYTE ) ( dwCummValue & 0xFF );
                    pOtcTradeRec->bytCardTradeFlag = pUpdateCmdInfo->bytTransFlag;
                    memcpy ( pOtcTradeRec->bytTAC, ( BYTE * ) ( &pResPonseInfoData->tagOtcExitResp.dwTAC ), 4 );
                }
            }
            else if ( pOtcCardInfo->bytDataBlkNo == 0x93 ) // CPU卡
            {
                CSC_CPU_91MODE_BLOCK_1_AGM       *pCSC_CPU_BLOCK_1;
                CSC_OTC_TRANS_RECORD       *pCSC_CPU_BLOCK_2;
                pCSC_CPU_BLOCK_1 = &pResPonseInfoData->tagReaderOtcResp.strOtcBlock1.strCpuBlock;
                pCSC_CPU_BLOCK_2 = &pResPonseInfoData->tagReaderOtcResp.strEntryRec;
                pOtcTradeRec->bytTicketType = pCSC_CPU_BLOCK_1->bytCardType;
            
            ltmp = pCSC_CPU_BLOCK_1->dwAppCode1;
            pOtcTradeRec->lOldCardNumber = (ltmp);
            
            ltmp = pCSC_CPU_BLOCK_1->bytAppCode2;
            pOtcTradeRec->lNewCardNumber = (ltmp);
            
            wTmp = pCSC_CPU_BLOCK_1->wCardCounter;
            pOtcTradeRec->wCardCounter = (wTmp);
            
            ltmp = pOtcCardInfo->GetCardBalance();
            pOtcTradeRec->lValueBeforeTrade = htonl(ltmp);
            
            wTmp = pCSC_CPU_BLOCK_1->wCityID;
            pOtcTradeRec->wCityCode = (wTmp);
            
                     
                     
            pOtcTradeRec->bytAuthorizationTime[5] = ( pCSC_CPU_BLOCK_1->bytArithmeticID & 0x0F );
            pOtcTradeRec->bytAuthorizationTime[6] = pCSC_CPU_BLOCK_1->bytCardVersion;
            
            
            if ( pUpdateCmdInfo->bytExitTEntryFlag)  // 是出站换乘进站
            {
// ??上传扣款金额为上次出站记录中的扣款金额、当前车站为出站记录中的交易设备车站计算机的节点标识码、上次使用车站为进站记录中的交易设备车站计算机的节点标识码
                wTmp = pCSC_CPU_BLOCK_2->uniTrans.strNormal.wTransAmount;
                pOtcTradeRec->wTradeVal = htons(wTmp);
                
                pOtcTradeRec->bytCurrentStationID[0] = pCSC_CPU_BLOCK_2->bytLastUsedLine;
                pOtcTradeRec->bytCurrentStationID[1] = pCSC_CPU_BLOCK_2->bytLastUsedStation;
                pOtcTradeRec->bytCurrentStationID[2] = 0x10;
                pOtcTradeRec->bytCurrentStationID[3] = 0x0;
                
                pOtcTradeRec->bytLastTimeStationID[0] = pCSC_CPU_BLOCK_1->bytAnotherTransLine;//->bytLastUsedLine;
                pOtcTradeRec->bytLastTimeStationID[1] = pCSC_CPU_BLOCK_1->bytAnotherTransStation;
                pOtcTradeRec->bytLastTimeStationID[2] = pCSC_CPU_BLOCK_1->bytAnotherTransEqCode;
                pOtcTradeRec->bytLastTimeStationID[3] = 0; //pCSC_OTC_BLOCK_2->bytLastUsedEqCode;
            } 
            else
            {
                memcpy(pOtcTradeRec->bytCurrentStationID,pLocalInfo->bytSleId,sizeof(pLocalInfo->bytSleId)); //当前车站
                pOtcTradeRec->bytCurrentStationID[2] = 0x10;
                pOtcTradeRec->bytCurrentStationID[3] = 0x0;
                pOtcTradeRec->wTradeVal = 0;             
                pOtcTradeRec->bytLastTimeStationID[0] = pCSC_CPU_BLOCK_2->bytLastUsedLine;
                pOtcTradeRec->bytLastTimeStationID[1] = pCSC_CPU_BLOCK_2->bytLastUsedStation;
                pOtcTradeRec->bytLastTimeStationID[2] = 0x10;
                pOtcTradeRec->bytLastTimeStationID[3] = 0x0;
            }               
               // fill CPU Block2
                //FillTransRecOTC ( pOtcTradeRec, g_EntryCpuBlk.pBlk2, TYPE_CSC_CPU_BLOCK_2, g_bIsExitTEntryStation, 0, 0 );

                if ( pResPonseInfoData->bytRecardInfoCmdCode == CSC_OTC_ENTRY )
                {
// 卡计数器
                wTmp = pResPonseInfoData->tagOtcEntryResp.wTransCount;
                pOtcTradeRec->wCardCounter =  (wTmp);                     
                    //FillTransRecOTC ( pOtcTradeRec, &g_RespCmd, TYPE_CSC_MP_CPU_ENTRY, 0, 0, g_lOTCEntryCumuValue );
                    //交易金额在填写进站交易时已填写
                    pOtcTradeRec->bytTradeEventCode = pUpdateCmdInfo->bytTransEventCode;;
                    pOtcTradeRec->wDiscount = 0;        //进站交易优惠金额一律填零
                    uint32_t  dwCummValue = pOtcCardInfo->strEntryRec.uniTrans.strNormal.GetAccumuBalance();        
                    pOtcTradeRec->bytCumuValue[0] = ( BYTE ) ( ( dwCummValue >> 16 ) & 0xFF );
                    pOtcTradeRec->bytCumuValue[1] = ( BYTE ) ( ( dwCummValue >> 8 ) & 0xFF );
                    pOtcTradeRec->bytCumuValue[2] = ( BYTE ) ( dwCummValue & 0xFF );
                    pOtcTradeRec->bytCardTradeFlag = 0x88;//bytOtcTransFlag;
                    memcpy ( pOtcTradeRec->bytTAC, ( BYTE * ) ( &pResPonseInfoData->tagOtcEntryResp.dwTAC ), 4 );
                
                }
                else if ( pResPonseInfoData->bytRecardInfoCmdCode == CSC_CPU_LOCKCARD )
                {
                    //FillTransRecOTC ( pOtcTradeRec, &g_RespCmd, TYPE_CSC_MP_CPU_LOCKCARD, 0, 0, 0 );
                    pOtcTradeRec->wTradeVal = 0;
                    pOtcTradeRec->bytTradeEventCode = pUpdateCmdInfo->bytTransEventCode;;
                    pOtcTradeRec->wCardCounter = 0;
                    pOtcTradeRec->wDiscount = 0;
                    if ( pResPonseInfoData->tagLockcardResp.bytRespCode == 1 )
                    {
                        pOtcTradeRec->bytCardTradeFlag = 0x99;
                    }
                    else
                    {
                        pOtcTradeRec->bytCardTradeFlag = 0x98;
                    }
                    memset ( pOtcTradeRec->bytTAC, 0x00, 4 );                    
                }
                else if( pResPonseInfoData->bytRecardInfoCmdCode == CSC_OTC_EXIT )
                {
// 卡计数器
                wTmp = pResPonseInfoData->tagOtcExitResp.wTransCount;
                pOtcTradeRec->wCardCounter =  (wTmp);  
                
                    wTmp = pUpdateCmdInfo->dwTransValue;;
                    pOtcTradeRec->wTradeVal = htons(wTmp);
                    
                    pOtcTradeRec->bytTradeEventCode = pUpdateCmdInfo->bytTransEventCode;
//lOperatorCode
                    ltmp = pUpdateCmdInfo->dwEnTime;
                    pOtcTradeRec->lOperatorCode = htonl(ltmp); 
                    
                    wTmp = pUpdateCmdInfo->wYouHuiValue;
                    pOtcTradeRec->wDiscount = htons(wTmp);
                    
                    uint32_t  dwCummValue = pOtcCardInfo->strEntryRec.uniTrans.strNormal.GetAccumuBalance()+pUpdateCmdInfo->dwTransValue;
                    pOtcTradeRec->bytCumuValue[0] = ( BYTE ) ( ( dwCummValue >> 16 ) & 0xFF );
                    pOtcTradeRec->bytCumuValue[1] = ( BYTE ) ( ( dwCummValue >> 8 ) & 0xFF );
                    pOtcTradeRec->bytCumuValue[2] = ( BYTE ) ( dwCummValue & 0xFF );
                    pOtcTradeRec->bytCardTradeFlag = pUpdateCmdInfo->bytTransFlag;
                    memcpy ( pOtcTradeRec->bytTAC, ( BYTE * ) ( &pResPonseInfoData->tagOtcExitResp.dwTAC ), 4 );
                }
            }
            else
            {
                ptrGDM->GetUtils()->DebugMsg(ptrGDM->GetTracelog(), "FillRecord OTC Invalid! NO M1 or CPU 卡\n" ,"OK");
                
            }

            if ( pResPonseInfoData->bytRecardInfoCmdCode == CSC_OTC_ENTRY || pResPonseInfoData->bytRecardInfoCmdCode == CSC_OTC_LOCKCARD
                 || pResPonseInfoData->bytRecardInfoCmdCode == CSC_CPU_LOCKCARD || pResPonseInfoData->bytRecardInfoCmdCode == CSC_OTC_EXIT )
            {
                uint32_t  dwSn =0;
                GetUpdateSeqSn(dwSn,1,eOtcSeqSn); 
                pInTxnRec->struInTxnRec.ltxnPosSeqSn = dwSn;
printf("dwSn = %d\n",dwSn);                
                ( ( OTC_TRADE_REC * ) pOtcTradeRec )->lSLESerialNo = htonl(dwSn);
                dwSn++;
                GetUpdateSeqSn(dwSn,2,eOtcSeqSn);
printf("dwSn = %d\n",dwSn);                  
            }
        }
        break;

        case 4:  //pboc卡
        {
            MTC_NEW_TRADE_REC  *pPbocTxn;
            CSC_MP_PBOC_READCARD *pPbocCardInfo;

            CSC_MP_PBOC_ENTRY *pPbocEntryAck;
            CSC_MP_PBOC_EXIT  *pPbocExitAck;
            
            pPbocTxn =( MTC_NEW_TRADE_REC * )  &(pInTxnRec->struInTxnRec.uTradeRec.mtcNewTradeRec);
            pPbocCardInfo = &(pResPonseInfoData->tagReaderPbocInfoResp);
       

            pInTxnRec->struInTxnRec.eMessageCode = ePbocTradeCode;
            pInTxnRec->struInTxnRec.lTradecode = 0x6023;
            pInTxnRec->struInTxnRec.lPackBodyLen = sizeof(MTC_NEW_TRADE_REC );

            memset(pPbocTxn,0,sizeof(MTC_NEW_TRADE_REC));

            pPbocTxn->bytTradeEventCode=pUpdateCmdInfo->bytTransEventCode;
            // 车票类型码 参数4015 映射后的类型
            pPbocTxn->bytTicketType = pUpdateCmdInfo->bytTicketType; // 映射后的票类型
            // 应用序列号  BCD -> BYTE[]
            memcpy(pPbocTxn->bytAppCode,pPbocCardInfo->strPbocBlock1.bytMainAppCode,sizeof(pPbocCardInfo->strPbocBlock1.bytMainAppCode));
            // 交易日期时间
            ltmp = pUpdateCmdInfo->dwTransTime;
            pPbocTxn->lTradeTime = htonl(ltmp);	
            // 交易前钱包实际金额
            ltmp = *(uint32_t*)(pPbocCardInfo->strPbocBlock1.bytWalletBalance);
            pPbocTxn->lValueBeforeTrade = htonl(ltmp);
            // 透支限额
            ltmp = *(uint32_t*)(pPbocCardInfo->strPbocBlock1.bytDraftBalance);
            pPbocTxn->lDraftDueBalance = htonl(ltmp);
            // 交易金额
            wTmp = pUpdateCmdInfo->dwTransValue;
            pPbocTxn->lTradeVal = htonl(wTmp);
            // 优惠金额
            wTmp = 0;//pUpdateCmdInfo->wYouHuiValue;
            pPbocTxn->lDiscountVal = htonl(wTmp);
            // 当前车站节点标识码
            ptrGDM->GetUtils()->HexStrToBytes(pLocalInfo->SC_id,pPbocTxn->bytCurrentStationID,4);
            
            // 上次使用车站节点标识码
            
            // 终端设备标识
            memcpy(pPbocTxn->bytSLEID,pLocalInfo->bytSleId,4);
            // 终端流水号
            if ( pResPonseInfoData->bytRecardInfoCmdCode == eCSC_PBOC_ENTRY || pResPonseInfoData->bytRecardInfoCmdCode == eCSC_PBOC_EXIT)
            {
                pPbocTxn->bytTradeEventCode = pUpdateCmdInfo->bytTransEventCode;
                uint32_t  dwSn = 0;
                GetUpdateSeqSn(dwSn,1,ePbocSeqSn);
                pInTxnRec->struInTxnRec.ltxnPosSeqSn = dwSn;

                memcpy(pPbocTxn->bytSLESerialNo,((BYTE*)(&dwSn))+1,3);
                dwSn++;
                GetUpdateSeqSn(dwSn,2,ePbocSeqSn);
                
                
                if( pResPonseInfoData->bytRecardInfoCmdCode == eCSC_PBOC_ENTRY )
                {
                    pPbocEntryAck = &pResPonseInfoData->tagPbocEntryResp;
                    // 应用密文
                    memcpy(pPbocTxn->bytKey,pPbocEntryAck->bytAppKey,sizeof(pPbocEntryAck->bytAppKey));
                    // 应用信息数据
                    memcpy(pPbocTxn->bytAppInfoData,pPbocEntryAck->bytAppInforData,sizeof(pPbocEntryAck->bytAppInforData));
                    // 发卡行应用数据
                    memcpy(pPbocTxn->bytIssueAppData,pPbocEntryAck->bytIssuerAppData,sizeof(pPbocEntryAck->bytIssuerAppData));
                    // 不可预知数
                    memcpy(pPbocTxn->bytExcepData,pPbocEntryAck->bytUnpredictData,sizeof(pPbocEntryAck->bytUnpredictData));
                    // 票卡计数器
                    memcpy(pPbocTxn->bytCardCount,pPbocEntryAck->bytTicketCardCount,sizeof(pPbocEntryAck->bytTicketCardCount));
                    // 终端验证结果
                    memcpy(pPbocTxn->bytValidResult,pPbocEntryAck->bytTerminalConfirm,sizeof(pPbocEntryAck->bytTerminalConfirm));
                    // 应用交互特征
                    memcpy(pPbocTxn->bytAppCommType,pPbocEntryAck->bytAppInteractFeature,sizeof(pPbocEntryAck->bytAppInteractFeature));
                    // 终端性能
                    memcpy(pPbocTxn->bytDevAble,pPbocEntryAck->bytTerminalCapability,sizeof(pPbocEntryAck->bytTerminalCapability));
                    // 应用版本号
                    memcpy(pPbocTxn->bytAppVerCode,pPbocEntryAck->bytAppVersion,sizeof(pPbocEntryAck->bytAppVersion));
                    // 电子现金发卡行授权码
                    memcpy(pPbocTxn->bytEleCashCode,pPbocEntryAck->bytCashIssuerAutorCode,sizeof(pPbocEntryAck->bytCashIssuerAutorCode));
                    // 产品信息标识
                    memcpy(pPbocTxn->bytProductFlag,pPbocEntryAck->bytProductInforID,sizeof(pPbocEntryAck->bytProductInforID));
                }
                else if(pResPonseInfoData->bytRecardInfoCmdCode == eCSC_PBOC_EXIT)
                {
                    pPbocExitAck = &pResPonseInfoData->tagPbocExitResp;
                    // 应用密文
                    memcpy(pPbocTxn->bytKey,pPbocExitAck->bytAppKey,sizeof(pPbocExitAck->bytAppKey));
                    // 应用信息数据
                    memcpy(pPbocTxn->bytAppInfoData,pPbocExitAck->bytAppInforData,sizeof(pPbocExitAck->bytAppInforData));
                    // 发卡行应用数据
                    memcpy(pPbocTxn->bytIssueAppData,pPbocExitAck->bytIssuerAppData,sizeof(pPbocExitAck->bytIssuerAppData));
                    // 不可预知数
                    memcpy(pPbocTxn->bytExcepData,pPbocExitAck->bytUnpredictData,sizeof(pPbocExitAck->bytUnpredictData));
                    // 票卡计数器
                    memcpy(pPbocTxn->bytCardCount,pPbocExitAck->bytTicketCardCount,sizeof(pPbocExitAck->bytTicketCardCount));
                    // 终端验证结果
                    memcpy(pPbocTxn->bytValidResult,pPbocExitAck->bytTerminalConfirm,sizeof(pPbocExitAck->bytTerminalConfirm));
                    // 应用交互特征
                    memcpy(pPbocTxn->bytAppCommType,pPbocExitAck->bytAppInteractFeature,sizeof(pPbocExitAck->bytAppInteractFeature));
                    // 终端性能
                    memcpy(pPbocTxn->bytDevAble,pPbocExitAck->bytTerminalCapability,sizeof(pPbocExitAck->bytTerminalCapability));
                    // 应用版本号
                    memcpy(pPbocTxn->bytAppVerCode,pPbocExitAck->bytAppVersion,sizeof(pPbocExitAck->bytAppVersion));
                    // 电子现金发卡行授权码
                    memcpy(pPbocTxn->bytEleCashCode,pPbocExitAck->bytCashIssuerAutorCode,sizeof(pPbocExitAck->bytCashIssuerAutorCode));
                    // 产品信息标识
                    memcpy(pPbocTxn->bytProductFlag,pPbocExitAck->bytProductInforID,sizeof(pPbocExitAck->bytProductInforID));
                }
            }
            // 特殊交易标志 ?? 
            pPbocTxn->bytSpecialTransFlag = pUpdateCmdInfo->bytTransFlag;
            // 当月累积金额
            ltmp = *(uint32_t*)(pPbocCardInfo->strPbocBlock2.bytAccumuBalance);
            ltmp += pUpdateCmdInfo->dwTransValue;
            ltmp = htonl(ltmp);
            memcpy(pPbocTxn->bytAccumuValue,&ltmp,3);
            // 票卡有效期
            memcpy(pPbocTxn->bytValidDate,pPbocCardInfo->strPbocBlock1.bytAppExpiredDate+1,2);
            // 主帐号序列号
            pPbocTxn->bytMainAccount = pPbocCardInfo->strPbocBlock1.bytMainAccoutSn;
            // 发卡行代码
            memcpy(pPbocTxn->bytIssueCardCode,pPbocCardInfo->strPbocBlock1.bytIssuerBankCode,2);
            // 卡业务类型
            pPbocTxn->bytCardBusType = pPbocCardInfo->strPbocBlock1.bytBusinessType;
            // AID
            memcpy(pPbocTxn->bytAppId,pPbocCardInfo->strPbocBlock2.bytAid,sizeof(pPbocCardInfo->strPbocBlock2.bytAid));
            // DDA完成标志
            pPbocTxn->bytDdaFlag = 1;
//          //   操作员编号/上一笔进站时间操作员号
//               ltmp=pUpdateCmdInfo->wOperatorID;//atoi((const char *));
//                pPbocTxn->bytOperatorID=htonl(ltmp);
            
        }
        break; 
        case 5:   //pboc员工卡
        {
            MTC_EMPLOYEE_TRADE_REC  *pEPbocTxn;
            CSC_MP_PBOC_EMPLOYEE_READCARD *pEPbocCardInfo;
           // CSC_MP_PBOC_EMPLOREE_UPDATE   *pEPbocUpdateAck;
            
            CSC_MP_PBOC_EMPLOYEE_ENTRY  *pEPbocEntryAck;
            CSC_MP_PBOC_EMPLOYEE_EXIT  *pEPbocExitAck;
            CSC_MP_PBOC_EMPLOYEE_LOCKCARD *pEPbocLockAck;
            
            pEPbocTxn =( MTC_EMPLOYEE_TRADE_REC * )  &(pInTxnRec->struInTxnRec.uTradeRec.mtcEmployeeTradeRec);
            pEPbocCardInfo = &(pResPonseInfoData->tagReaderPbocEmployeeInfoResp);
          //  pEPbocUpdateAck = &(pResPonseInfoData->tagPbocEmployeeUpdateResp);

            pInTxnRec->struInTxnRec.eMessageCode = ePbocEmployeeTradeCode;
            pInTxnRec->struInTxnRec.lTradecode = 0x6033;
            pInTxnRec->struInTxnRec.lPackBodyLen = sizeof(MTC_EMPLOYEE_TRADE_REC );


            memset(pEPbocTxn,0,sizeof(MTC_EMPLOYEE_TRADE_REC));
            // 交易事件码
            pEPbocTxn->bytTradeEventCode = pInTxnRec->struInTxnRec.lTradecode;
            pEPbocTxn->bytTicketType = pUpdateCmdInfo->bytTicketType; // 映射后的票类型
            // 票卡标识 应用序列号后4字节
            memcpy(pEPbocTxn->bytTicketFlag,pEPbocCardInfo->strPbocEmpoyeeBlock1.bytMainAppCode+6,sizeof(pEPbocTxn->bytTicketFlag));

            // 应用序列号
            memcpy(pEPbocTxn->bytAppCode,pEPbocCardInfo->strPbocEmpoyeeBlock1.bytMainAppCode,sizeof(pEPbocCardInfo->strPbocEmpoyeeBlock1.bytMainAppCode));
            // SAM卡号（终端机编号）
            pEPbocTxn->bytSAM[0] = 0x20;
            pEPbocTxn->bytSAM[1] = 0x00;        
            ltmp = pResPonseInfoData->tagReaderResetResp.dwCSCSAMCard;
            memcpy(pEPbocTxn->bytSAM+2,&ltmp,sizeof(ltmp));
            // 交易日期时间
            ltmp = pUpdateCmdInfo->dwTransTime;
            pEPbocTxn->lTradeTime = htonl(ltmp);	


            // 交易前钱包实际金额
            ltmp = *(uint32_t*)(pEPbocCardInfo->strPbocEmpoyeeBlock1.bytWalletBalance);
            pEPbocTxn->lValueBeforeTrade = htonl(ltmp);
            // 交易金额
            wTmp=pUpdateCmdInfo->dwTransValue;
            pEPbocTxn->wTradeVal = htons(wTmp);

            // 当前车站节点标识码
            ptrGDM->GetUtils()->HexStrToBytes(pLocalInfo->SC_id,pEPbocTxn->bytCurrentStationID,4);
            // memcpy(pEPbocTxn->bytCurrentStationID,m_pParent->m_LocalSleInfo.SC_id,4);
            // 上次使用车站节点标识码

            // 终端设备标识
            memcpy(pEPbocTxn->bytSLEID,pLocalInfo->bytSleId,4);
            // 终端流水号
            if ( pResPonseInfoData->bytRecardInfoCmdCode == eCSC_ET_ENTRY || pResPonseInfoData->bytRecardInfoCmdCode == eCSC_ET_EXIT
                    || pResPonseInfoData->bytRecardInfoCmdCode == eCSC_ET_LOCKCARD )
            {
                pEPbocTxn->bytTradeEventCode = pUpdateCmdInfo->bytTransEventCode;
                uint32_t  dwSn =   0;
                GetUpdateSeqSn(dwSn,1,ePbocEmployeeSeqSn);
                pInTxnRec->struInTxnRec.ltxnPosSeqSn = dwSn;
                
               // ((MTC_EMPLOYEE_TRADE_REC * )pEPbocTxn) ->bytSLESerialNo = htonl(dwSn);
                   memcpy(pEPbocTxn->bytSLESerialNo,((BYTE*)(&dwSn))+1,3); 
                dwSn++;
                GetUpdateSeqSn(dwSn,2,ePbocEmployeeSeqSn);
                
                if(pResPonseInfoData->bytRecardInfoCmdCode == eCSC_ET_ENTRY )
                {
                    pEPbocEntryAck = &pResPonseInfoData->tagPbocEmployeeEntryResp;
                     // 终端交易序号
                    ltmp = *(uint32_t*)pEPbocEntryAck->bytDevTxnSn;
                    pEPbocTxn->dwTransFlag = htonl(ltmp);
                    // 电子钱包交易序号
                    wTmp = *(uint16_t*)(pEPbocEntryAck->bytOfflineTransFlag);
                    pEPbocTxn->wOfflineTransFlag = htons(wTmp);
                    // 密钥索引
                    pEPbocTxn->bytKeyIndex = pEPbocEntryAck->bytKeyIndex;
                    // 密钥版本号
                    pEPbocTxn->bytKeyVersionNo = pEPbocEntryAck->bytKeyVersionNo;
                      // TAC
                     memcpy(pEPbocTxn->bytTAC,pEPbocEntryAck->bytTAC,sizeof(pEPbocEntryAck->bytTAC));
                }
                else if(pResPonseInfoData->bytRecardInfoCmdCode == eCSC_ET_EXIT)
                {
                    pEPbocExitAck = &pResPonseInfoData->tagPbocEmployeeExitResp;
                     // 终端交易序号
                    ltmp = *(uint32_t*)pEPbocExitAck->bytDevTxnSn;
                    pEPbocTxn->dwTransFlag = htonl(ltmp);
                    // 电子钱包交易序号
                    wTmp = *(uint16_t*)(pEPbocExitAck->bytOfflineTransFlag);
                    pEPbocTxn->wOfflineTransFlag = htons(wTmp);
                    // 密钥索引
                    pEPbocTxn->bytKeyIndex = pEPbocExitAck->bytKeyIndex;
                    // 密钥版本号
                    pEPbocTxn->bytKeyVersionNo = pEPbocExitAck->bytKeyVersionNo;
                      // TAC
                    memcpy(pEPbocTxn->bytTAC,pEPbocExitAck->bytTAC,sizeof(pEPbocExitAck->bytTAC));
                }
                else if(pResPonseInfoData->bytRecardInfoCmdCode == eCSC_ET_LOCKCARD)
                {
                    pEPbocLockAck = &pResPonseInfoData->tagPbocEmployeeLockResp;
                  // 终端交易序号
                    ltmp = *(uint32_t*)pEPbocLockAck->bytDevTxnSn;
                    pEPbocTxn->dwTransFlag = htonl(ltmp);
                    // 电子钱包交易序号
                    wTmp = *(uint16_t*)(pEPbocLockAck->bytOfflineTransFlag);
                    pEPbocTxn->wOfflineTransFlag = htons(wTmp);
                    // 密钥索引
                    pEPbocTxn->bytKeyIndex = pEPbocLockAck->bytKeyIndex;
                    // 密钥版本号
                    pEPbocTxn->bytKeyVersionNo = pEPbocLockAck->bytKeyVersionNo;
                    // 锁卡结果
                    pEPbocTxn->bytLockCardResult = pEPbocLockAck->bytLockResult;
                      // TAC
                    memcpy(pEPbocTxn->bytTAC,pEPbocLockAck->bytTAC,sizeof(pEPbocLockAck->bytTAC));
                }
            }
          
            // 优惠金额  ??
            wTmp = 0;//pUpdateCmdInfo->wYouHuiValue;
            pEPbocTxn->wDiscount = htons(wTmp);
            // 锁卡结果
           // pEPbocTxn->bytLockCardResult = pEPbocUpdateAck->
            // 特殊交易标志
            pEPbocTxn->bytSpecialTransFlag = pUpdateCmdInfo->bytTransFlag;
            // 当月累积金额  ?? 
            ltmp = *(uint32_t*)(pEPbocCardInfo->strPbocEmpoyeeBlock2.bytAccumuBalance);
            ltmp += pUpdateCmdInfo->dwTransValue;
            ltmp = htonl(ltmp);
            memcpy(pEPbocTxn->bytAccumuValue,&ltmp,3);
          
            // 操作员编号/上一笔进站时间操作员号
//              ltmp=pUpdateCmdInfo->wOperatorID;//atoi((const char *));
//              pEPbocTxn->bytOperatorID=htonl(ltmp);
        }     
        break;        case 6:  //二维码
        {
            MTC_TD_TRADE_REC  *pMtcTdTxn;
            CSC_MP_QR_READCARD *pMtcTdCardInfo;
            
            CSC_MP_QR_ENTRY    *pMtcTdEntryAck;
            CSC_MP_QR_EXIT     *pMtcTdExitAck;
            
            pMtcTdTxn =( MTC_TD_TRADE_REC * ) &(pInTxnRec->struInTxnRec.uTradeRec.mtcTdTradeRec);
            pMtcTdCardInfo = &(pResPonseInfoData->tagReaderReadOtqResp);
            //pMtcTdUpdateAck = &(pResPonseInfoData->tagOtqUpdateResp);
            
            pInTxnRec->struInTxnRec.eMessageCode = eTdTradeCode;
            pInTxnRec->struInTxnRec.lTradecode = eTdTradeCode;
            pInTxnRec->struInTxnRec.lPackBodyLen = sizeof(MTC_TD_TRADE_REC );
            
              memset(pMtcTdTxn,0,sizeof(MTC_TD_TRADE_REC));
            // 交易事件码
            pMtcTdTxn->bytTradeEventCode = pUpdateCmdInfo->bytTransEventCode;
            pMtcTdTxn->bytTicketType = pMtcTdCardInfo->struDataBlk.bytCardType; 
           //应用序列号 账户号码
             memcpy(pMtcTdTxn->bytAppCode,pMtcTdCardInfo->struDataBlk.bytUserToken,sizeof(pMtcTdCardInfo->struDataBlk.bytUserToken));
             //账户认证码
             memcpy(pMtcTdTxn->bytUserAuth,pMtcTdCardInfo->struDataBlk.bytUserAuth,sizeof(pMtcTdCardInfo->struDataBlk.bytUserAuth));
            // SAM卡号
           //  pMtcTdTxn->bytSAM = (BYTE*)pResPonseInfoData->tagReaderResetResp.dwTPUSAMCard;
             memcpy(pMtcTdTxn->bytSAM,(BYTE*)(&pResPonseInfoData->tagReaderResetResp.dwTPUSAMCard),sizeof(pMtcTdTxn->bytSAM)); 
             //发卡方认证MAC
              memcpy(pMtcTdTxn->bytIssuerMAC,pMtcTdCardInfo->struDataBlk.bytIssuerMAC,sizeof(pMtcTdCardInfo->struDataBlk.bytIssuerMAC));
              //交易日期时间
             ltmp = pUpdateCmdInfo->dwTransTime;
             pMtcTdTxn->lTradeTime = htonl(ltmp);	
             //交易金额
             wTmp=pUpdateCmdInfo->dwTransValue;
            pMtcTdTxn->wTradeVal = htons(wTmp);
             //优惠金额
             wTmp = 0;//pUpdateCmdInfo->wYouHuiValue;
            pMtcTdTxn->wDiscount = htons(wTmp);
            //当前车站
            // m_pGDM->GetUtils()->HexStrToBytes(pLocalInfo.SC_id,pMtcTdTxn->bytCurrentStationID,4)
             memcpy(pMtcTdTxn->bytCurrentStationID,pLocalInfo->bytSleId,sizeof(pLocalInfo->bytSleId)); 
             pMtcTdTxn->bytCurrentStationID[2]  = 0x10;
             pMtcTdTxn->bytCurrentStationID[3] = 0x0;
//上次使用日期时间
            // pMtcTdTxn->lLastTime=pUpdateCmdInfo.
//上次使用车站    

        //     pMtcTdTxn->bytLastStationID=
             //终端设备标识
                memcpy(pMtcTdTxn->bytSLEID,pLocalInfo->bytSleId,4);
            // 终端流水号
             if(pResPonseInfoData->bytRecardInfoCmdCode == eCSC_OTQ_ENTRY || pResPonseInfoData->bytRecardInfoCmdCode == eCSC_OTQ_EXIT)
            {
                uint32_t  dwSn = 0;
                GetUpdateSeqSn(dwSn,1,eMtcSeqSn);
                pInTxnRec->struInTxnRec.ltxnPosSeqSn = dwSn;
                
                ((MTC_TD_TRADE_REC * )pMtcTdTxn) ->dwSLESerialNo = htonl(dwSn);
                dwSn++;
                GetUpdateSeqSn(dwSn,2,eMtcSeqSn);
                
                if(pResPonseInfoData->bytRecardInfoCmdCode == eCSC_OTQ_ENTRY)
                {
                    pMtcTdEntryAck = &pResPonseInfoData->tagQrEntryResp;
                    memcpy(&pMtcTdTxn->lPhoneTime,pMtcTdEntryAck->bytTransGMT,sizeof(pMtcTdTxn->lPhoneTime)); //->dwPhoneTime;
                      // TAC
                    memcpy(pMtcTdTxn->bytTAC,pMtcTdEntryAck->bytTac,sizeof(pMtcTdEntryAck->bytTac));                    
                }
                else if(pResPonseInfoData->bytRecardInfoCmdCode == eCSC_OTQ_EXIT)
                {
                    pMtcTdExitAck = &pResPonseInfoData->tagQrExitResp;
                    memcpy(&pMtcTdTxn->lPhoneTime,pMtcTdExitAck->bytTransGMT,sizeof(pMtcTdExitAck->bytTransGMT)); //->dwPhoneTime;
                      // TAC
                    memcpy(pMtcTdTxn->bytTAC,pMtcTdExitAck->bytTac,sizeof(pMtcTdExitAck->bytTac));                                        
                }
                
            }
                //申请有效内二维码生 成次数
               // wTmp=pMtcTdCardInfo.
                wTmp=pMtcTdCardInfo->struDataBlk.wTdApplyCount;
                pMtcTdTxn->wApplyTdCount=htons(wTmp);
                 //特殊交易标志
                  pMtcTdTxn->bytSpecialTransFlag = pUpdateCmdInfo->bytTransFlag;
             //手机操作系统
            pMtcTdTxn->bytPhoneSystem=pMtcTdCardInfo->struDataBlk.bytSystem;
          //    memcpy(pMtcTdTxn->bytPhoneSystem,pMtcTdCardInfo->struDataBlk.bytSystem,sizeof(pMtcTdTxn->bytPhoneSystem));   
           //当月轨道交通累计金额 
            ltmp = *(uint32_t*)(&pMtcTdCardInfo->struDataBlk.dwAccumuBalance);
            ltmp += pUpdateCmdInfo->lDiscountBeginValue;
            pMtcTdTxn->dwAccumuValue = htonl(ltmp);
           // memcpy(pMtcTdTxn->dwAccumuValue,&ltmp,3);
            //当月轨道交通累计次数
            wTmp = *(uint16_t*)(&pMtcTdCardInfo->struDataBlk.wAccumuCount);
            wTmp += pUpdateCmdInfo->wDiscountBeginTimes;
            pMtcTdTxn->wAccumuCount = htons(wTmp);
           // memcpy(pMtcTdTxn->wAccumuCount,&ltmp,3);
        //分散因子
        memcpy(pMtcTdTxn->bytDispersingFactor,pMtcTdCardInfo->struDataBlk.bytDispersingFactor,sizeof(pMtcTdTxn->bytDispersingFactor));

        
         // 操作员编号/上一笔进站时间操作员号
//            ltmp = atoi(pUpdateCmdInfo->wOperatorID.c_str()); 
//            pMtcTdTxn->lOperatorID = htonl(ltmp);
        ltmp=pUpdateCmdInfo->wOperatorID;//atoi((const char *));
        pMtcTdTxn->lOperatorID=htonl(ltmp);
        //    memcpy(pMtcTdTxn->lOperatorID,pUpdateCmdInfo->wOperatorID,sizeof(pMtcTdTxn->lOperatorID));
        //回写标志
        pMtcTdTxn->bytBackFlag = pUpdateCmdInfo->bytBackFlag;
           // memcpy(pMtcTdTxn->bytBackFlag,pUpdateCmdInfo->bytBackFlag,sizeof(pMtcTdTxn->bytBackFlag));
            

        }
        break;
        default:
            break;
        }
    }
	bytRtn = ptrGDM->GetSQLITEDBManagerProxy()->DBInsertRecInfo(IN_TRADE_REC_FLAG,&pInTxnRec->struInTxnRec,NULL);
	if(bytRtn)
	{
		ptrGDM->GetUtils()->DebugMsg(ptrGDM->GetTracelog(),"FillRecord","DBInsertRecInfo(IN_TRADE_REC_FLAG) Failed! bytRtn = %d",bytRtn);
		return;
	}

//	ModifyRegist(bytUpdateTypeTemp,pUpdateCmdInfo,&m_pParent->m_InOutUpdateSeqSn,(int&)bytRtn);
//	if(bytRtn)
//	{
//		m_pGDM->GetUtils()->DebugMsg(&m_tracelog,"CBOMAnalysisDlg::FillUpdateTxn","ModifyRegist eCSC_OTT_UPDATE Failed! bytRtn = %d",bytRtn);
//		return;
//	}
	return;

}


void ModifyRegist(IGlobalDataManager * ptrGDM,tagResPonseInfoData  *pResPonseInfoData,TRAC_O_CLASSE *pTracelog,CardAnalysisInfo_t *pAnalysisInfo,BYTE bytUpdateMode,tagUpdateCmdInfo *pUpdateCmdInfo,IN_OUT_UPDATE_SEQ_SN *pInOutSeqSn,int &iRtn)
{
	BYTE bytType;

        // Collect 5040
        if( (0x60 == pResPonseInfoData->bytWorkMode) && (0x1 == pUpdateCmdInfo->bytChipType) && (pUpdateCmdInfo->bytUpdateMode == 0xd))
        {
            iRtn = 0;            
            pInOutSeqSn->eInOutUpdateFlag = eOutFlag;
            pInOutSeqSn->eSeqSnType = eBomRegist6001Sn;
            memset(&pInOutSeqSn->lBomRegist6001[0],0,sizeof(pInOutSeqSn->lBomRegist6001));
            iRtn = ptrGDM->GetSQLITEDBManagerProxy()->DBInOutModifySeqSn(pInOutSeqSn);
            if(0 != iRtn)
            {
                    ptrGDM->GetUtils()->DebugMsg(pTracelog,"ModifyRegist","DBInOutModifySeqSn Get 6001 Failed! iRtn = %d",iRtn);
                    return;
            }
            pInOutSeqSn->lBomRegist6001[bytUpdateMode]++;
            pInOutSeqSn->lBomRegist6001[9]++;
            
            pInOutSeqSn->eInOutUpdateFlag = eUpdateFlag;
            iRtn = ptrGDM->GetSQLITEDBManagerProxy()->DBInOutModifySeqSn(pInOutSeqSn);
            if(0 != iRtn)
            {
                    ptrGDM->GetUtils()->DebugMsg(pTracelog,"ModifyRegist","DBInOutModifySeqSn Update 6001 Register! iRtn = %d",iRtn);
                    return;
            }                   
        }
            
        // 6000     
	iRtn = 0;            
	pInOutSeqSn->eInOutUpdateFlag = eOutFlag;
	pInOutSeqSn->eSeqSnType = eBomRegist6000Sn;
	memset(&pInOutSeqSn->lBomRegist6000[0],0,sizeof(pInOutSeqSn->lBomRegist6000));
	iRtn = ptrGDM->GetSQLITEDBManagerProxy()->DBInOutModifySeqSn(pInOutSeqSn);
	if(0 != iRtn)
	{
		ptrGDM->GetUtils()->DebugMsg(pTracelog,"ModifyRegist","DBInOutModifySeqSn Get Failed! iRtn = %d",iRtn);
		return;
	}
/*??00     进站数（所有票种）
01     出站数（所有票种）
02     用储值交通卡进站数-成人（票种04h）
03     用单程票进站数（票种64h）
04     用乘次票进站数（票种78h）
05     用员工票1进站数（公共交通卡类型，票种28h）
06     用员工票2进站数（轨道交通专用票类型，票种82h
07     用打折类型1A(D1-交通卡，儿童)票进站数（票种30h）
08     用打折类型1B(D2-交通卡，老人)票进站数（票种19h）
09     用VP(VP-纪念，旅行者)票进站数（票种80h
10     入口处拒绝数
11     用储值交通卡出站数-成人（票种04h）
12     用单程票出站数（票种64h）
13     用乘次票出站数（票种78h）
14     用员工票1出站数（公共交通卡类型，票种28h）
15     用员工票2出站数（轨道交通专用票类型，票种82h
16     用打折类型1A(D1-交通卡，儿童)票出站数（票种30h）
17     用打折类型1B(D2-交通卡，老人)票出站数（票种19h）
18     用VP(VP-纪念，旅行者)票出站数（票种80h
19     出站检票机处拒绝数
20     回收数
21     储值交通卡-成人扣款值（票种04h）
22     单程票扣款值（票种64h）
23     员工票1扣款值（公共交通卡类型，票种28h）
24     员工票2扣款值（轨道交通专用票类型，票种82h
25     打折类型1A(D1-交通卡，儿童)票扣款值（票种30h）
26     打折类型1B(D2-交通卡，老人)票扣款值（票种06h）
27     VP(VP-纪念，旅行者)票扣款值（票种80h
28     非高峰时段出站数
29     自由出站数
30     同站出站数
31     合法的测试票数
32     不足额储值交通卡的最后乘次奖励
33     单程票剩余票值，储值交通卡的最后乘次奖励
33     单程票剩余票值
34     不足额，一票通的最后乘次奖励
35     交通卡优惠次数
36     交通卡优惠金额
37     PBOC卡进站人数
38     PBOC卡出站人数
39     PBOC卡-扣款值
40     PBOC卡优惠次数
41     PBOC卡优惠金额
42     闸机紧急模式进站数
43     闸机紧急模式出站数44    计时计次票进站数（票种84h）
45    计时计次票出站数（票种84h）
46 ~ 63    [未定义]
*/
	// bytType = pUpdateCmdInfo->bytUpdateMode;

	switch(pUpdateCmdInfo->bytTransEventCode)
	{
            case 0x24:  // 票扣款
            case 0x25:  // 卡扣款
            case 0x40:  // ??出站联乘扣款
            case 0x41:  // ??出站联乘扣款
                    {
                        pInOutSeqSn->lBomRegist6000[1]++;  // ??出站数（所有票种）
                        switch(pAnalysisInfo->bytTicketType)
                        {
                            
                            case 0x04://
                            {
                                pInOutSeqSn->lBomRegist6000[11]++;//用储值交通卡进站数-成人（票种04h）
                                pInOutSeqSn->lBomRegist6000[21] += pUpdateCmdInfo->dwTransValue;
                            }
                            break;
                            case 0x64:     //用单程票出站数（票种64h）
                            {
                                pInOutSeqSn->lBomRegist6000[12]++;
                                pInOutSeqSn->lBomRegist6000[20]++;                                
                                pInOutSeqSn->lBomRegist6000[22] += pUpdateCmdInfo->dwTransValue;                            
                            }
                            break;
                            case 0x78: //  用乘次票出站数（票种78h）
                            {
                                pInOutSeqSn->lBomRegist6000[13]++;                   
                            }
                            break;
                            case 0x28:  // 用员工票1出站数（公共交通卡类型，票种28h）
                            {
                                pInOutSeqSn->lBomRegist6000[14]++;
                                pInOutSeqSn->lBomRegist6000[23] += pUpdateCmdInfo->dwTransValue;         
                            }
                            break;
                            case 0x82:  // 用员工票2出站数（轨道交通专用票类型，票种82h
                            {
                                pInOutSeqSn->lBomRegist6000[15]++;
                                pInOutSeqSn->lBomRegist6000[24] += pUpdateCmdInfo->dwTransValue;      
                            }    
                            break;
                            case 0x30:  //用打折类型1A(D1-交通卡，儿童)票出站数（票种30h）
                            {
                                pInOutSeqSn->lBomRegist6000[16]++;
                                pInOutSeqSn->lBomRegist6000[25] += pUpdateCmdInfo->dwTransValue;      
                            } 
                            break;
                            case 0x19:  //  用打折类型1B(D2-交通卡，老人)票出站数（票种19
                            {
                                pInOutSeqSn->lBomRegist6000[17]++;
                                pInOutSeqSn->lBomRegist6000[26] += pUpdateCmdInfo->dwTransValue;      
                            } 
                                break;
                            case 0x80:  // 用VP(VP-纪念，旅行者)票出站数（票种80h
                                {
                                pInOutSeqSn->lBomRegist6000[18]++;
                                pInOutSeqSn->lBomRegist6000[27] += pUpdateCmdInfo->dwTransValue;      
                            } 
                                break;
                        }
                        if(0x1E == pUpdateCmdInfo->bytSamStationExit)
                        {
                            pInOutSeqSn->lBomRegist6000[30]++;
                        }
                        if(0x1C == pUpdateCmdInfo->bytPeekOffFlag)
                        {
                            pInOutSeqSn->lBomRegist6000[28]++;
                        }
                    }
            break;
            
            case 0x26:   // ??出站联乘进站 ticket
            case 0x27:   //  ??出站联乘进站  card
            case 0x2A:
            case 0x2B:
            case 0x3E:  // ??尾程优惠
            case 0x3F:  // ??设置交通卡累积消费金额
            {
                pInOutSeqSn->lBomRegist6000[0]++;  // ??进站数（所有票种）                
                switch(pAnalysisInfo->bytTicketType)
                {
                    case 0x04://
                    {
                        pInOutSeqSn->lBomRegist6000[2]++;//   用储值交通卡进站数-成人（票种04h）
                    }
                    break;
                    case 0x64:     // 用单程票进站数（票种64h）
                    {
                        pInOutSeqSn->lBomRegist6000[3]++;         
                    }
                    break;
                    case 0x78: //   用乘次票进站数（票种78h）
                    {
                        pInOutSeqSn->lBomRegist6000[4]++;                   
                    }
                    break;
                    case 0x28:  //     用员工票1进站数（公共交通卡类型，票种28h
                    {
                        pInOutSeqSn->lBomRegist6000[5]++;
                    }
                    break;
                    case 0x82:  //  用员工票2进站数（轨道交通专用票类型，票种82h
                    {
                        pInOutSeqSn->lBomRegist6000[6]++;
                    }    
                    break;
                    case 0x30:  // 用打折类型1A(D1-交通卡，儿童)票进站数（票种30h）
                    {
                        pInOutSeqSn->lBomRegist6000[7]++;  
                    } 
                    break;
                    case 0x19:  // 用打折类型1B(D2-交通卡，老人)票进站数（票种19h）
                    {
                        pInOutSeqSn->lBomRegist6000[8]++;
                    } 
                        break;
                    case 0x80:  //用VP(VP-纪念，旅行者)票进站数（票种80h
                    {
                        pInOutSeqSn->lBomRegist6000[9]++;   
                    } 
                      break;  
                }
            }
            break;
            case 0x35:  // ??锁卡更改卡状态交易		票/交通卡
            case 0x3B:  // ??锁卡清空卡上钱包交易
            {
                
            }
            break;
/*
 ??91h	反向进/出站	0	车票被拒绝
92h	超过时间限制	0	车票被拒绝
93h	超乘车票	0	车票被拒绝
94h	校验和错误	0	车票被拒绝
95h	测试/收益	0	车票被拒绝
96h	非法（未定义的票卡类型）	0	车票被拒绝
97h	不可接受的票卡类型	0	入口处被拒绝
98h	再次进站	0	入口处被拒绝
99h	可用日以前使用	0	入口处被拒绝
9Ah	票值为0拒绝	0	入口处被拒绝
9Bh	仅可同站进出	0	出站处被拒绝
9Ch	剩余票值低于最低票价	0	入口处被拒绝
9Dh	同站进出	0	出站处被拒绝
*/            
            case 0x88:  // ??黑名单票在出站时被拒绝
            case 0x89:  // ??黑名单票在进站时被拒绝                
            case 0x8A:  // ??黑名单票入口处被接受
            case 0x8D:  // ??锁卡失败
            case 0x90:  //过期
            case 0x91:
            case 0x92:
            case 0x93:
            case 0x94:
            case 0x95:
            case 0x96:
            case 0x97:
            case 0x98:
            case 0x99:
            case 0x9A:
            case 0x9B:
            case 0x9C:
            case 0x9D:
            {
               if(0x20 == pResPonseInfoData->bytWorkMode) // 进站
               {
                   pInOutSeqSn->lBomRegist6000[10]++;
               }
               else if(0x40 == pResPonseInfoData->bytWorkMode)
               {
                   pInOutSeqSn->lBomRegist6000[19]++;                   
               }
            }
            break;
	}
	pInOutSeqSn->eInOutUpdateFlag = eUpdateFlag;
	iRtn = ptrGDM->GetSQLITEDBManagerProxy()->DBInOutModifySeqSn(pInOutSeqSn);
	if(0 != iRtn)
	{
		ptrGDM->GetUtils()->DebugMsg(pTracelog,"ModifyRegist","DBInOutModifySeqSn Update Failed! iRtn = %d",iRtn);
		return;
	}
        
        if(0x60 == pResPonseInfoData->bytWorkMode) // 回收机构
        {
            pInOutSeqSn->lBomRegist6000[10]++;
        }

	return;
}

