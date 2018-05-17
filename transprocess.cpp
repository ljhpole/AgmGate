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
///*�1�300     ��վ��������Ʊ�֣�
//01     ��վ��������Ʊ�֣�
//02     �ô�ֵ��ͨ����վ��-���ˣ�Ʊ��04h��
//03     �õ���Ʊ��վ����Ʊ��64h��
//04     �ó˴�Ʊ��վ����Ʊ��78h��
//05     ��Ա��Ʊ1��վ����������ͨ�����ͣ�Ʊ��28h��
//06     ��Ա��Ʊ2��վ���������ͨר��Ʊ���ͣ�Ʊ��82h
//07     �ô�������1A(D1-��ͨ������ͯ)Ʊ��վ����Ʊ��30h��
//08     �ô�������1B(D2-��ͨ��������)Ʊ��վ����Ʊ��19h��
//09     ��VP(VP-���������)Ʊ��վ����Ʊ��80h
//10     ��ڴ��ܾ���
//11     �ô�ֵ��ͨ����վ��-���ˣ�Ʊ��04h��
//12     �õ���Ʊ��վ����Ʊ��64h��
//13     �ó˴�Ʊ��վ����Ʊ��78h��
//14     ��Ա��Ʊ1��վ����������ͨ�����ͣ�Ʊ��28h��
//15     ��Ա��Ʊ2��վ���������ͨר��Ʊ���ͣ�Ʊ��82h
//16     �ô�������1A(D1-��ͨ������ͯ)Ʊ��վ����Ʊ��30h��
//17     �ô�������1B(D2-��ͨ��������)Ʊ��վ����Ʊ��19h��
//18     ��VP(VP-���������)Ʊ��վ����Ʊ��80h
//19     ��վ��Ʊ�����ܾ���
//20     ������
//21     ��ֵ��ͨ��-���˿ۿ�ֵ��Ʊ��04h��
//22     ����Ʊ�ۿ�ֵ��Ʊ��64h��
//23     Ա��Ʊ1�ۿ�ֵ��������ͨ�����ͣ�Ʊ��28h��
//24     Ա��Ʊ2�ۿ�ֵ�������ͨר��Ʊ���ͣ�Ʊ��82h
//25     ��������1A(D1-��ͨ������ͯ)Ʊ�ۿ�ֵ��Ʊ��30h��
//26     ��������1B(D2-��ͨ��������)Ʊ�ۿ�ֵ��Ʊ��06h��
//27     VP(VP-���������)Ʊ�ۿ�ֵ��Ʊ��80h
//28     �Ǹ߷�ʱ�γ�վ��
//29     ���ɳ�վ��
//30     ͬվ��վ��
//31     �Ϸ��Ĳ���Ʊ��
//32     ������ֵ��ͨ�������˴ν���
//33     ����Ʊʣ��Ʊֵ
//34     ����һƱͨ�����˴ν���
//35     ��ͨ���Żݴ���
//36     ��ͨ���Żݽ��
//37     PBOC����վ����
//38     PBOC����վ����
//39     PBOC��-�ۿ�ֵ
//40     PBOC���Żݴ���
//41     PBOC���Żݽ��
//42     բ������ģʽ��վ��
//43     բ������ģʽ��վ��44    ��ʱ�ƴ�Ʊ��վ����Ʊ��84h��
//45    ��ʱ�ƴ�Ʊ��վ����Ʊ��84h��
//46 ~ 63    [δ����]
//*/
//	// bytType = pUpdateCmdInfo->bytUpdateMode;
//
//	switch(pUpdateCmdInfo->bytTransEventCode)
//	{
//            case 0x24:  // Ʊ�ۿ�
//            case 0x25:  // ���ۿ�
//            case 0x40:  // �1�3��վ���˿ۿ�
//            case 0x41:  // �1�3��վ���˿ۿ�
//                    {
//                        switch(pAnalysisInfo->bytTicketType)
//                        {
//                            pInOutSeqSn->lBomRegist6000[1]++;  // �1�3��վ��������Ʊ�֣�
//                            case 0x04://
//                            {
//                                pInOutSeqSn->lBomRegist6000[11]++;//�ô�ֵ��ͨ����վ��-���ˣ�Ʊ��04h��
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
//            case 0x26:   // �1�3��վ���˽�վ ticket
//            case 0x27:   //  �1�3��վ���˽�վ  card
//            case 0x2A:
//            case 0x2B:
//            case 0x3E:  // �1�3β���Ż�
//            case 0x3F:  // �1�3���ý�ͨ���ۻ����ѽ��
//            {
//                
//            }
//            break;
//            case 0x35:  // �1�3�������Ŀ�״̬����		Ʊ/��ͨ��
//            case 0x3B:  // �1�3������տ���Ǯ������
//            {
//            }
//            break;
///*
// �1�391h	�����/��վ	0	��Ʊ���ܾ�
//92h	����ʱ������	0	��Ʊ���ܾ�
//93h	���˳�Ʊ	0	��Ʊ���ܾ�
//94h	У��ʹ���	0	��Ʊ���ܾ�
//95h	����/����	0	��Ʊ���ܾ�
//96h	�Ƿ���δ�����Ʊ�����ͣ�	0	��Ʊ���ܾ�
//97h	���ɽ��ܵ�Ʊ������	0	��ڴ����ܾ�
//98h	�ٴν�վ	0	��ڴ����ܾ�
//99h	��������ǰʹ��	0	��ڴ����ܾ�
//9Ah	ƱֵΪ0�ܾ�	0	��ڴ����ܾ�
//9Bh	����ͬվ����	0	��վ�����ܾ�
//9Ch	ʣ��Ʊֵ�������Ʊ��	0	��ڴ����ܾ�
//9Dh	ͬվ����	0	��վ�����ܾ�
//*/            
//            case 0x88:  // �1�3������Ʊ�ڳ�վʱ���ܾ�
//            case 0x89:  // �1�3������Ʊ�ڽ�վʱ���ܾ�                
//            case 0x8A:  // �1�3������Ʊ��ڴ�������
//            case 0x8D:  // �1�3����ʧ��
//            case 0x90:  //����
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
//               if(0x20 == pResPonseInfoData->bytWorkMode) // ��վ
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
//    pInTxnRec->struInTxnRec.ltxnPosSeqSn = pTransDataBak->m_dwTransSn;  //������ˮ��
//    pInTxnRec->struInTxnRec.sSleId = pLocalInfo->local_SLE_id;
//
//    char cTempValue[MAX_PATH];
//    memset(cTempValue,'\0',MAX_PATH);
//    ptrGDM->GetUtils()->UTCToLocal(pTransDataBak->m_dwTransTimePre,(char*)"YMDhms",cTempValue);
//    pInTxnRec->struInTxnRec.sTxnTime = cTempValue;
//    pInTxnRec->struInTxnRec.sLastTime = cTempValue;
//    pInTxnRec->struInTxnRec.lSendFlag = 10; // ���ͱ�� 
//    
//    
//    {
//        switch ( pUpdateCmdInfo->bytChipType )
//        {
//        case 1:      // ����Ʊ
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
//// SAM����
//            pOttTradeRec->lSAM = pResPonseInfoData->tagReaderResetResp.dwTPUSAMCard;
//            memcpy ( pOttTradeRec->bytSLEID, pLocalInfo->bytSleId, sizeof(pLocalInfo->bytSleId) );
//            
//// Ʊ����
//            pOttTradeRec->bytTicketType = pCSC_OTT_BLOCK->bytCardType;
//        
//// ����        
//        ltmp = *(uint32_t*)(pCSC_OTT_BLOCK->bytCardUniqueCode);
//        pOttTradeRec->lOldCardNumber  = htonl(ltmp);     
//        pOttTradeRec->lNewCardNumber = pOttTradeRec->lOldCardNumber;        
//// ��Ʊ�豸        
//        pOttTradeRec->bytSaleDevID[0] = pCSC_OTT_BLOCK->uniAllTicket.struDataSJT.bytSaleEq[2];
//        pOttTradeRec->bytSaleDevID[1] = pCSC_OTT_BLOCK->uniAllTicket.struDataSJT.bytSaleEq[1];
//        pOttTradeRec->bytSaleDevID[2] = pCSC_OTT_BLOCK->uniAllTicket.struDataSJT.bytSaleEq[0];
//// ����ǰƱֵ       
//        wTmp = *(uint16_t*)(pCSC_OTT_BLOCK->bytBalance);
//        ltmp = wTmp;
//        pOttTradeRec->lValueBeforeTrade = htonl(ltmp);
//// ���׽��        
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
//// �1�3ʣ��˴� �1�3(�����ڳ˴�Ʊ)
//        wTmp = *(uint16_t*)(pCSC_OTT_BLOCK->bytBalance); 
//        wTmp = (wTmp < pUpdateCmdInfo->dwTransValue)?0:(wTmp - pUpdateCmdInfo->dwTransValue);
//        pOttTradeRec->bytRestUseTimes =  wTmp & 0xFF;       
////  �1�3Ʊ������       
//        wTmp = *(uint16_t*)pCSC_OTT_BLOCK->bytUseTimes;
//        pOttTradeRec->wTicketCounter = htons(wTmp);
////  ��ǰ��վ  
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
//        case 2:      // ��ͨ��
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
//            if ( pOtcCardInfo->bytDataBlkNo == 0x82 )     // M1��
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
//// ������               
//                if ( pUpdateCmdInfo-> bytOldFlag) // ���˿����
//                {
//                    pOtcTradeRec->lOperatorCode |= 1 << 31;
//                }
//// ��������
//                wTmp = pUpdateCmdInfo->wCardCounter;
//                pOtcTradeRec->wCardCounter =  htons(wTmp);
//// ���д���                
//            wTmp = pCSC_OTC_BLOCK_1->wCityID;
//            pOtcTradeRec->wCityCode = htons(wTmp);  
//
//            pOtcTradeRec->bytAuthorizationTime[5] = 0x01;
//            pOtcTradeRec->bytAuthorizationTime[6] = pUpdateCmdInfo->bytCardVersion;
//
//
//
//
//            if ( pUpdateCmdInfo->bytExitTEntryFlag)  // �ǳ�վ���˽�վ
//            {
//                //memcpy(pOtcTradeRec->bytCurrentStationID,pCSC_OTC_BLOCK_2->bytLastUsedLine;//->bytSleId,sizeof(pLocalInfo->bytSleId)); //��ǰ��վ
//// �1�3�ϴ��ۿ���Ϊ�ϴγ�վ��¼�еĿۿ����ǰ��վΪ��վ��¼�еĽ����豸��վ������Ľڵ��ʶ�롢�ϴ�ʹ�ó�վΪ��վ��¼�еĽ����豸��վ������Ľڵ��ʶ��
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
//                memcpy(pOtcTradeRec->bytCurrentStationID,pLocalInfo->bytSleId,sizeof(pLocalInfo->bytSleId)); //��ǰ��վ
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
//                   pOtcTradeRec->wDiscount = 0;        //��վ�����Żݽ��һ������
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
//            else if ( pOtcCardInfo->bytDataBlkNo == 0x92 ) // CPU��
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
//            if ( pUpdateCmdInfo->bytExitTEntryFlag)  // �ǳ�վ���˽�վ
//            {
//                //memcpy(pOtcTradeRec->bytCurrentStationID,pCSC_OTC_BLOCK_2->bytLastUsedLine;//->bytSleId,sizeof(pLocalInfo->bytSleId)); //��ǰ��վ
//// �1�3�ϴ��ۿ���Ϊ�ϴγ�վ��¼�еĿۿ����ǰ��վΪ��վ��¼�еĽ����豸��վ������Ľڵ��ʶ�롢�ϴ�ʹ�ó�վΪ��վ��¼�еĽ����豸��վ������Ľڵ��ʶ��
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
//                memcpy(pOtcTradeRec->bytCurrentStationID,pLocalInfo->bytSleId,sizeof(pLocalInfo->bytSleId)); //��ǰ��վ
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
//                    //���׽������д��վ����ʱ����д
//                    pOtcTradeRec->bytTradeEventCode = pUpdateCmdInfo->bytTransEventCode;;
//                    pOtcTradeRec->wDiscount = 0;        //��վ�����Żݽ��һ������
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
//                ptrGDM->GetUtils()->DebugMsg(&g_traceInfo, "FillRecord OTC Invalid! NO M1 or CPU ��\n" ,"OK");
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
    pInTxnRec->struInTxnRec.ltxnPosSeqSn = 0;  //������ˮ��
    pInTxnRec->struInTxnRec.sSleId = pLocalInfo->local_SLE_id;

    char cTempValue[MAX_PATH];
    memset(cTempValue,'\0',MAX_PATH);
    ptrGDM->GetUtils()->getLocalTime((char*)"YMDhms",cTempValue);
    pInTxnRec->struInTxnRec.sTxnTime = cTempValue;
    pInTxnRec->struInTxnRec.sLastTime = cTempValue;
    pInTxnRec->struInTxnRec.lSendFlag = 10; // ���ͱ�� 
    pInTxnRec->struInTxnRec.lTradeEventCode = pUpdateCmdInfo->bytTransEventCode;
    
    
    {
        switch ( pUpdateCmdInfo->bytChipType )
        {
        case 1:      // ����Ʊ
        {
            CSC_OTT_BLOCK          *pCSC_OTT_BLOCK;

            
            pCSC_OTT_BLOCK = &pResPonseInfoData->tagReaderReadOttResp.struDataBlk;

            pInTxnRec->struInTxnRec.eMessageCode = eOttTradeCode;
            pInTxnRec->struInTxnRec.lTradecode = eOttTradeCode;
            pInTxnRec->struInTxnRec.lPackBodyLen = sizeof(OTT_TRADE_REC);
            
            pOttTradeRec = ( OTT_TRADE_REC * ) (&pInTxnRec->struInTxnRec.uTradeRec.ottTradeRec);
            memset(pOttTradeRec,0,sizeof(OTT_TRADE_REC));
// SAM����
            ltmp = pResPonseInfoData->tagReaderResetResp.dwTPUSAMCard;
            pOttTradeRec->lSAM = htonl(ltmp);
            memcpy ( pOttTradeRec->bytSLEID, pLocalInfo->bytSleId, sizeof(pLocalInfo->bytSleId) );
            
// Ʊ����
         pOttTradeRec->bytTicketType = pCSC_OTT_BLOCK->bytCardType;
// ����        
        ltmp = *(uint32_t*)(pCSC_OTT_BLOCK->bytCardUniqueCode);
        pOttTradeRec->lOldCardNumber  = htonl(ltmp);     
        pOttTradeRec->lNewCardNumber = 0;        
// ��Ʊ�豸        
        pOttTradeRec->bytSaleDevID[0] = pCSC_OTT_BLOCK->uniAllTicket.struDataSJT.bytSaleEq[2];
        pOttTradeRec->bytSaleDevID[1] = pCSC_OTT_BLOCK->uniAllTicket.struDataSJT.bytSaleEq[1];
        pOttTradeRec->bytSaleDevID[2] = pCSC_OTT_BLOCK->uniAllTicket.struDataSJT.bytSaleEq[0];
// ����ǰƱֵ       
        wTmp = *(uint16_t*)(pCSC_OTT_BLOCK->bytBalance);
        ltmp = wTmp;//pUpdateCmdInfo->dwAfterTransValue;
        pOttTradeRec->lValueBeforeTrade = htonl(ltmp);
 
// ����ʱ��
        ltmp= pUpdateCmdInfo->dwTransTime;
        pOttTradeRec->lTradeTime = htonl(ltmp);        
// ??ʣ��˴� ??(�����ڳ˴�Ʊ)
        if(pUpdateCmdInfo->bytDisplayType != 0x1)
        {
//            wTmp = *(uint16_t*)(pCSC_OTT_BLOCK->bytBalance); 
//            wTmp = (wTmp < pUpdateCmdInfo->dwTransValue)?0:(wTmp - pUpdateCmdInfo->dwTransValue);
            pOttTradeRec->bytRestUseTimes =  pUpdateCmdInfo->dwAfterTransValue;   //wTmp & 0xFF;   
        }
//  ??Ʊ������       
        wTmp = *(uint16_t*)pCSC_OTT_BLOCK->bytUseTimes;
        pOttTradeRec->wTicketCounter = htons(wTmp);
//  ��ǰ��վ  
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
// ���׽��        
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
// ���׽��        
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

        case 2:      // ��ͨ��
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
          
            if ( pOtcCardInfo->bytDataBlkNo == 0x83 )     // M1��
            {
                CSC_OTC_81MODE_BLOCK_1_AGM       *pCSC_OTC_BLOCK_1;
                CSC_OTC_TRANS_RECORD       *pCSC_OTC_BLOCK_2;
                
                pCSC_OTC_BLOCK_1 = &pOtcCardInfo->strOtcBlock1.strM1Block;
                pCSC_OTC_BLOCK_2 = &pOtcCardInfo->strEntryRec;
    
                pOtcTradeRec->bytTicketType = pCSC_OTC_BLOCK_1->bytCardType;
                
                ltmp = pCSC_OTC_BLOCK_1->dwCardUniqueCode;
                pOtcTradeRec->lOldCardNumber = (ltmp);

// ������               
//                if ( pUpdateCmdInfo-> bytOldFlag) // ���˿����
//                {
//                    pOtcTradeRec->lOperatorCode |= 1 << 31;
//                }
// ��������
                wTmp = pUpdateCmdInfo->wCardCounter;
                pOtcTradeRec->wCardCounter =  (wTmp);  
// ���д���                
            wTmp = pCSC_OTC_BLOCK_1->wCityID;
            pOtcTradeRec->wCityCode = (wTmp);  

            pOtcTradeRec->bytAuthorizationTime[5] = 0x01;
            pOtcTradeRec->bytAuthorizationTime[6] = pUpdateCmdInfo->bytCardVersion;




            if ( pUpdateCmdInfo->bytExitTEntryFlag)  // �ǳ�վ���˽�վ
            {
// ??�ϴ��ۿ���Ϊ�ϴγ�վ��¼�еĿۿ����ǰ��վΪ��վ��¼�еĽ����豸��վ������Ľڵ��ʶ�롢�ϴ�ʹ�ó�վΪ��վ��¼�еĽ����豸��վ������Ľڵ��ʶ��
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
                memcpy(pOtcTradeRec->bytCurrentStationID,pLocalInfo->bytSleId,sizeof(pLocalInfo->bytSleId)); //��ǰ��վ
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
// ��������
                wTmp = pResPonseInfoData->tagOtcEntryResp.wTransCount;
                pOtcTradeRec->wCardCounter =  (wTmp);  
                
                   pOtcTradeRec->bytTradeEventCode = pUpdateCmdInfo->bytTransEventCode;
                   pOtcTradeRec->wDiscount = 0;        //��վ�����Żݽ��һ������
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
// ��������
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
            else if ( pOtcCardInfo->bytDataBlkNo == 0x93 ) // CPU��
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
            
            
            if ( pUpdateCmdInfo->bytExitTEntryFlag)  // �ǳ�վ���˽�վ
            {
// ??�ϴ��ۿ���Ϊ�ϴγ�վ��¼�еĿۿ����ǰ��վΪ��վ��¼�еĽ����豸��վ������Ľڵ��ʶ�롢�ϴ�ʹ�ó�վΪ��վ��¼�еĽ����豸��վ������Ľڵ��ʶ��
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
                memcpy(pOtcTradeRec->bytCurrentStationID,pLocalInfo->bytSleId,sizeof(pLocalInfo->bytSleId)); //��ǰ��վ
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
// ��������
                wTmp = pResPonseInfoData->tagOtcEntryResp.wTransCount;
                pOtcTradeRec->wCardCounter =  (wTmp);                     
                    //FillTransRecOTC ( pOtcTradeRec, &g_RespCmd, TYPE_CSC_MP_CPU_ENTRY, 0, 0, g_lOTCEntryCumuValue );
                    //���׽������д��վ����ʱ����д
                    pOtcTradeRec->bytTradeEventCode = pUpdateCmdInfo->bytTransEventCode;;
                    pOtcTradeRec->wDiscount = 0;        //��վ�����Żݽ��һ������
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
// ��������
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
                ptrGDM->GetUtils()->DebugMsg(ptrGDM->GetTracelog(), "FillRecord OTC Invalid! NO M1 or CPU ��\n" ,"OK");
                
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

        case 4:  //pboc��
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
            // ��Ʊ������ ����4015 ӳ��������
            pPbocTxn->bytTicketType = pUpdateCmdInfo->bytTicketType; // ӳ����Ʊ����
            // Ӧ�����к�  BCD -> BYTE[]
            memcpy(pPbocTxn->bytAppCode,pPbocCardInfo->strPbocBlock1.bytMainAppCode,sizeof(pPbocCardInfo->strPbocBlock1.bytMainAppCode));
            // ��������ʱ��
            ltmp = pUpdateCmdInfo->dwTransTime;
            pPbocTxn->lTradeTime = htonl(ltmp);	
            // ����ǰǮ��ʵ�ʽ��
            ltmp = *(uint32_t*)(pPbocCardInfo->strPbocBlock1.bytWalletBalance);
            pPbocTxn->lValueBeforeTrade = htonl(ltmp);
            // ͸֧�޶�
            ltmp = *(uint32_t*)(pPbocCardInfo->strPbocBlock1.bytDraftBalance);
            pPbocTxn->lDraftDueBalance = htonl(ltmp);
            // ���׽��
            wTmp = pUpdateCmdInfo->dwTransValue;
            pPbocTxn->lTradeVal = htonl(wTmp);
            // �Żݽ��
            wTmp = 0;//pUpdateCmdInfo->wYouHuiValue;
            pPbocTxn->lDiscountVal = htonl(wTmp);
            // ��ǰ��վ�ڵ��ʶ��
            ptrGDM->GetUtils()->HexStrToBytes(pLocalInfo->SC_id,pPbocTxn->bytCurrentStationID,4);
            
            // �ϴ�ʹ�ó�վ�ڵ��ʶ��
            
            // �ն��豸��ʶ
            memcpy(pPbocTxn->bytSLEID,pLocalInfo->bytSleId,4);
            // �ն���ˮ��
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
                    // Ӧ������
                    memcpy(pPbocTxn->bytKey,pPbocEntryAck->bytAppKey,sizeof(pPbocEntryAck->bytAppKey));
                    // Ӧ����Ϣ����
                    memcpy(pPbocTxn->bytAppInfoData,pPbocEntryAck->bytAppInforData,sizeof(pPbocEntryAck->bytAppInforData));
                    // ������Ӧ������
                    memcpy(pPbocTxn->bytIssueAppData,pPbocEntryAck->bytIssuerAppData,sizeof(pPbocEntryAck->bytIssuerAppData));
                    // ����Ԥ֪��
                    memcpy(pPbocTxn->bytExcepData,pPbocEntryAck->bytUnpredictData,sizeof(pPbocEntryAck->bytUnpredictData));
                    // Ʊ��������
                    memcpy(pPbocTxn->bytCardCount,pPbocEntryAck->bytTicketCardCount,sizeof(pPbocEntryAck->bytTicketCardCount));
                    // �ն���֤���
                    memcpy(pPbocTxn->bytValidResult,pPbocEntryAck->bytTerminalConfirm,sizeof(pPbocEntryAck->bytTerminalConfirm));
                    // Ӧ�ý�������
                    memcpy(pPbocTxn->bytAppCommType,pPbocEntryAck->bytAppInteractFeature,sizeof(pPbocEntryAck->bytAppInteractFeature));
                    // �ն�����
                    memcpy(pPbocTxn->bytDevAble,pPbocEntryAck->bytTerminalCapability,sizeof(pPbocEntryAck->bytTerminalCapability));
                    // Ӧ�ð汾��
                    memcpy(pPbocTxn->bytAppVerCode,pPbocEntryAck->bytAppVersion,sizeof(pPbocEntryAck->bytAppVersion));
                    // �����ֽ𷢿�����Ȩ��
                    memcpy(pPbocTxn->bytEleCashCode,pPbocEntryAck->bytCashIssuerAutorCode,sizeof(pPbocEntryAck->bytCashIssuerAutorCode));
                    // ��Ʒ��Ϣ��ʶ
                    memcpy(pPbocTxn->bytProductFlag,pPbocEntryAck->bytProductInforID,sizeof(pPbocEntryAck->bytProductInforID));
                }
                else if(pResPonseInfoData->bytRecardInfoCmdCode == eCSC_PBOC_EXIT)
                {
                    pPbocExitAck = &pResPonseInfoData->tagPbocExitResp;
                    // Ӧ������
                    memcpy(pPbocTxn->bytKey,pPbocExitAck->bytAppKey,sizeof(pPbocExitAck->bytAppKey));
                    // Ӧ����Ϣ����
                    memcpy(pPbocTxn->bytAppInfoData,pPbocExitAck->bytAppInforData,sizeof(pPbocExitAck->bytAppInforData));
                    // ������Ӧ������
                    memcpy(pPbocTxn->bytIssueAppData,pPbocExitAck->bytIssuerAppData,sizeof(pPbocExitAck->bytIssuerAppData));
                    // ����Ԥ֪��
                    memcpy(pPbocTxn->bytExcepData,pPbocExitAck->bytUnpredictData,sizeof(pPbocExitAck->bytUnpredictData));
                    // Ʊ��������
                    memcpy(pPbocTxn->bytCardCount,pPbocExitAck->bytTicketCardCount,sizeof(pPbocExitAck->bytTicketCardCount));
                    // �ն���֤���
                    memcpy(pPbocTxn->bytValidResult,pPbocExitAck->bytTerminalConfirm,sizeof(pPbocExitAck->bytTerminalConfirm));
                    // Ӧ�ý�������
                    memcpy(pPbocTxn->bytAppCommType,pPbocExitAck->bytAppInteractFeature,sizeof(pPbocExitAck->bytAppInteractFeature));
                    // �ն�����
                    memcpy(pPbocTxn->bytDevAble,pPbocExitAck->bytTerminalCapability,sizeof(pPbocExitAck->bytTerminalCapability));
                    // Ӧ�ð汾��
                    memcpy(pPbocTxn->bytAppVerCode,pPbocExitAck->bytAppVersion,sizeof(pPbocExitAck->bytAppVersion));
                    // �����ֽ𷢿�����Ȩ��
                    memcpy(pPbocTxn->bytEleCashCode,pPbocExitAck->bytCashIssuerAutorCode,sizeof(pPbocExitAck->bytCashIssuerAutorCode));
                    // ��Ʒ��Ϣ��ʶ
                    memcpy(pPbocTxn->bytProductFlag,pPbocExitAck->bytProductInforID,sizeof(pPbocExitAck->bytProductInforID));
                }
            }
            // ���⽻�ױ�־ ?? 
            pPbocTxn->bytSpecialTransFlag = pUpdateCmdInfo->bytTransFlag;
            // �����ۻ����
            ltmp = *(uint32_t*)(pPbocCardInfo->strPbocBlock2.bytAccumuBalance);
            ltmp += pUpdateCmdInfo->dwTransValue;
            ltmp = htonl(ltmp);
            memcpy(pPbocTxn->bytAccumuValue,&ltmp,3);
            // Ʊ����Ч��
            memcpy(pPbocTxn->bytValidDate,pPbocCardInfo->strPbocBlock1.bytAppExpiredDate+1,2);
            // ���ʺ����к�
            pPbocTxn->bytMainAccount = pPbocCardInfo->strPbocBlock1.bytMainAccoutSn;
            // �����д���
            memcpy(pPbocTxn->bytIssueCardCode,pPbocCardInfo->strPbocBlock1.bytIssuerBankCode,2);
            // ��ҵ������
            pPbocTxn->bytCardBusType = pPbocCardInfo->strPbocBlock1.bytBusinessType;
            // AID
            memcpy(pPbocTxn->bytAppId,pPbocCardInfo->strPbocBlock2.bytAid,sizeof(pPbocCardInfo->strPbocBlock2.bytAid));
            // DDA��ɱ�־
            pPbocTxn->bytDdaFlag = 1;
//          //   ����Ա���/��һ�ʽ�վʱ�����Ա��
//               ltmp=pUpdateCmdInfo->wOperatorID;//atoi((const char *));
//                pPbocTxn->bytOperatorID=htonl(ltmp);
            
        }
        break; 
        case 5:   //pbocԱ����
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
            // �����¼���
            pEPbocTxn->bytTradeEventCode = pInTxnRec->struInTxnRec.lTradecode;
            pEPbocTxn->bytTicketType = pUpdateCmdInfo->bytTicketType; // ӳ����Ʊ����
            // Ʊ����ʶ Ӧ�����кź�4�ֽ�
            memcpy(pEPbocTxn->bytTicketFlag,pEPbocCardInfo->strPbocEmpoyeeBlock1.bytMainAppCode+6,sizeof(pEPbocTxn->bytTicketFlag));

            // Ӧ�����к�
            memcpy(pEPbocTxn->bytAppCode,pEPbocCardInfo->strPbocEmpoyeeBlock1.bytMainAppCode,sizeof(pEPbocCardInfo->strPbocEmpoyeeBlock1.bytMainAppCode));
            // SAM���ţ��ն˻���ţ�
            pEPbocTxn->bytSAM[0] = 0x20;
            pEPbocTxn->bytSAM[1] = 0x00;        
            ltmp = pResPonseInfoData->tagReaderResetResp.dwCSCSAMCard;
            memcpy(pEPbocTxn->bytSAM+2,&ltmp,sizeof(ltmp));
            // ��������ʱ��
            ltmp = pUpdateCmdInfo->dwTransTime;
            pEPbocTxn->lTradeTime = htonl(ltmp);	


            // ����ǰǮ��ʵ�ʽ��
            ltmp = *(uint32_t*)(pEPbocCardInfo->strPbocEmpoyeeBlock1.bytWalletBalance);
            pEPbocTxn->lValueBeforeTrade = htonl(ltmp);
            // ���׽��
            wTmp=pUpdateCmdInfo->dwTransValue;
            pEPbocTxn->wTradeVal = htons(wTmp);

            // ��ǰ��վ�ڵ��ʶ��
            ptrGDM->GetUtils()->HexStrToBytes(pLocalInfo->SC_id,pEPbocTxn->bytCurrentStationID,4);
            // memcpy(pEPbocTxn->bytCurrentStationID,m_pParent->m_LocalSleInfo.SC_id,4);
            // �ϴ�ʹ�ó�վ�ڵ��ʶ��

            // �ն��豸��ʶ
            memcpy(pEPbocTxn->bytSLEID,pLocalInfo->bytSleId,4);
            // �ն���ˮ��
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
                     // �ն˽������
                    ltmp = *(uint32_t*)pEPbocEntryAck->bytDevTxnSn;
                    pEPbocTxn->dwTransFlag = htonl(ltmp);
                    // ����Ǯ���������
                    wTmp = *(uint16_t*)(pEPbocEntryAck->bytOfflineTransFlag);
                    pEPbocTxn->wOfflineTransFlag = htons(wTmp);
                    // ��Կ����
                    pEPbocTxn->bytKeyIndex = pEPbocEntryAck->bytKeyIndex;
                    // ��Կ�汾��
                    pEPbocTxn->bytKeyVersionNo = pEPbocEntryAck->bytKeyVersionNo;
                      // TAC
                     memcpy(pEPbocTxn->bytTAC,pEPbocEntryAck->bytTAC,sizeof(pEPbocEntryAck->bytTAC));
                }
                else if(pResPonseInfoData->bytRecardInfoCmdCode == eCSC_ET_EXIT)
                {
                    pEPbocExitAck = &pResPonseInfoData->tagPbocEmployeeExitResp;
                     // �ն˽������
                    ltmp = *(uint32_t*)pEPbocExitAck->bytDevTxnSn;
                    pEPbocTxn->dwTransFlag = htonl(ltmp);
                    // ����Ǯ���������
                    wTmp = *(uint16_t*)(pEPbocExitAck->bytOfflineTransFlag);
                    pEPbocTxn->wOfflineTransFlag = htons(wTmp);
                    // ��Կ����
                    pEPbocTxn->bytKeyIndex = pEPbocExitAck->bytKeyIndex;
                    // ��Կ�汾��
                    pEPbocTxn->bytKeyVersionNo = pEPbocExitAck->bytKeyVersionNo;
                      // TAC
                    memcpy(pEPbocTxn->bytTAC,pEPbocExitAck->bytTAC,sizeof(pEPbocExitAck->bytTAC));
                }
                else if(pResPonseInfoData->bytRecardInfoCmdCode == eCSC_ET_LOCKCARD)
                {
                    pEPbocLockAck = &pResPonseInfoData->tagPbocEmployeeLockResp;
                  // �ն˽������
                    ltmp = *(uint32_t*)pEPbocLockAck->bytDevTxnSn;
                    pEPbocTxn->dwTransFlag = htonl(ltmp);
                    // ����Ǯ���������
                    wTmp = *(uint16_t*)(pEPbocLockAck->bytOfflineTransFlag);
                    pEPbocTxn->wOfflineTransFlag = htons(wTmp);
                    // ��Կ����
                    pEPbocTxn->bytKeyIndex = pEPbocLockAck->bytKeyIndex;
                    // ��Կ�汾��
                    pEPbocTxn->bytKeyVersionNo = pEPbocLockAck->bytKeyVersionNo;
                    // �������
                    pEPbocTxn->bytLockCardResult = pEPbocLockAck->bytLockResult;
                      // TAC
                    memcpy(pEPbocTxn->bytTAC,pEPbocLockAck->bytTAC,sizeof(pEPbocLockAck->bytTAC));
                }
            }
          
            // �Żݽ��  ??
            wTmp = 0;//pUpdateCmdInfo->wYouHuiValue;
            pEPbocTxn->wDiscount = htons(wTmp);
            // �������
           // pEPbocTxn->bytLockCardResult = pEPbocUpdateAck->
            // ���⽻�ױ�־
            pEPbocTxn->bytSpecialTransFlag = pUpdateCmdInfo->bytTransFlag;
            // �����ۻ����  ?? 
            ltmp = *(uint32_t*)(pEPbocCardInfo->strPbocEmpoyeeBlock2.bytAccumuBalance);
            ltmp += pUpdateCmdInfo->dwTransValue;
            ltmp = htonl(ltmp);
            memcpy(pEPbocTxn->bytAccumuValue,&ltmp,3);
          
            // ����Ա���/��һ�ʽ�վʱ�����Ա��
//              ltmp=pUpdateCmdInfo->wOperatorID;//atoi((const char *));
//              pEPbocTxn->bytOperatorID=htonl(ltmp);
        }     
        break;        case 6:  //��ά��
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
            // �����¼���
            pMtcTdTxn->bytTradeEventCode = pUpdateCmdInfo->bytTransEventCode;
            pMtcTdTxn->bytTicketType = pMtcTdCardInfo->struDataBlk.bytCardType; 
           //Ӧ�����к� �˻�����
             memcpy(pMtcTdTxn->bytAppCode,pMtcTdCardInfo->struDataBlk.bytUserToken,sizeof(pMtcTdCardInfo->struDataBlk.bytUserToken));
             //�˻���֤��
             memcpy(pMtcTdTxn->bytUserAuth,pMtcTdCardInfo->struDataBlk.bytUserAuth,sizeof(pMtcTdCardInfo->struDataBlk.bytUserAuth));
            // SAM����
           //  pMtcTdTxn->bytSAM = (BYTE*)pResPonseInfoData->tagReaderResetResp.dwTPUSAMCard;
             memcpy(pMtcTdTxn->bytSAM,(BYTE*)(&pResPonseInfoData->tagReaderResetResp.dwTPUSAMCard),sizeof(pMtcTdTxn->bytSAM)); 
             //��������֤MAC
              memcpy(pMtcTdTxn->bytIssuerMAC,pMtcTdCardInfo->struDataBlk.bytIssuerMAC,sizeof(pMtcTdCardInfo->struDataBlk.bytIssuerMAC));
              //��������ʱ��
             ltmp = pUpdateCmdInfo->dwTransTime;
             pMtcTdTxn->lTradeTime = htonl(ltmp);	
             //���׽��
             wTmp=pUpdateCmdInfo->dwTransValue;
            pMtcTdTxn->wTradeVal = htons(wTmp);
             //�Żݽ��
             wTmp = 0;//pUpdateCmdInfo->wYouHuiValue;
            pMtcTdTxn->wDiscount = htons(wTmp);
            //��ǰ��վ
            // m_pGDM->GetUtils()->HexStrToBytes(pLocalInfo.SC_id,pMtcTdTxn->bytCurrentStationID,4)
             memcpy(pMtcTdTxn->bytCurrentStationID,pLocalInfo->bytSleId,sizeof(pLocalInfo->bytSleId)); 
             pMtcTdTxn->bytCurrentStationID[2]  = 0x10;
             pMtcTdTxn->bytCurrentStationID[3] = 0x0;
//�ϴ�ʹ������ʱ��
            // pMtcTdTxn->lLastTime=pUpdateCmdInfo.
//�ϴ�ʹ�ó�վ    

        //     pMtcTdTxn->bytLastStationID=
             //�ն��豸��ʶ
                memcpy(pMtcTdTxn->bytSLEID,pLocalInfo->bytSleId,4);
            // �ն���ˮ��
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
                //������Ч�ڶ�ά���� �ɴ���
               // wTmp=pMtcTdCardInfo.
                wTmp=pMtcTdCardInfo->struDataBlk.wTdApplyCount;
                pMtcTdTxn->wApplyTdCount=htons(wTmp);
                 //���⽻�ױ�־
                  pMtcTdTxn->bytSpecialTransFlag = pUpdateCmdInfo->bytTransFlag;
             //�ֻ�����ϵͳ
            pMtcTdTxn->bytPhoneSystem=pMtcTdCardInfo->struDataBlk.bytSystem;
          //    memcpy(pMtcTdTxn->bytPhoneSystem,pMtcTdCardInfo->struDataBlk.bytSystem,sizeof(pMtcTdTxn->bytPhoneSystem));   
           //���¹����ͨ�ۼƽ�� 
            ltmp = *(uint32_t*)(&pMtcTdCardInfo->struDataBlk.dwAccumuBalance);
            ltmp += pUpdateCmdInfo->lDiscountBeginValue;
            pMtcTdTxn->dwAccumuValue = htonl(ltmp);
           // memcpy(pMtcTdTxn->dwAccumuValue,&ltmp,3);
            //���¹����ͨ�ۼƴ���
            wTmp = *(uint16_t*)(&pMtcTdCardInfo->struDataBlk.wAccumuCount);
            wTmp += pUpdateCmdInfo->wDiscountBeginTimes;
            pMtcTdTxn->wAccumuCount = htons(wTmp);
           // memcpy(pMtcTdTxn->wAccumuCount,&ltmp,3);
        //��ɢ����
        memcpy(pMtcTdTxn->bytDispersingFactor,pMtcTdCardInfo->struDataBlk.bytDispersingFactor,sizeof(pMtcTdTxn->bytDispersingFactor));

        
         // ����Ա���/��һ�ʽ�վʱ�����Ա��
//            ltmp = atoi(pUpdateCmdInfo->wOperatorID.c_str()); 
//            pMtcTdTxn->lOperatorID = htonl(ltmp);
        ltmp=pUpdateCmdInfo->wOperatorID;//atoi((const char *));
        pMtcTdTxn->lOperatorID=htonl(ltmp);
        //    memcpy(pMtcTdTxn->lOperatorID,pUpdateCmdInfo->wOperatorID,sizeof(pMtcTdTxn->lOperatorID));
        //��д��־
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
/*??00     ��վ��������Ʊ�֣�
01     ��վ��������Ʊ�֣�
02     �ô�ֵ��ͨ����վ��-���ˣ�Ʊ��04h��
03     �õ���Ʊ��վ����Ʊ��64h��
04     �ó˴�Ʊ��վ����Ʊ��78h��
05     ��Ա��Ʊ1��վ����������ͨ�����ͣ�Ʊ��28h��
06     ��Ա��Ʊ2��վ���������ͨר��Ʊ���ͣ�Ʊ��82h
07     �ô�������1A(D1-��ͨ������ͯ)Ʊ��վ����Ʊ��30h��
08     �ô�������1B(D2-��ͨ��������)Ʊ��վ����Ʊ��19h��
09     ��VP(VP-���������)Ʊ��վ����Ʊ��80h
10     ��ڴ��ܾ���
11     �ô�ֵ��ͨ����վ��-���ˣ�Ʊ��04h��
12     �õ���Ʊ��վ����Ʊ��64h��
13     �ó˴�Ʊ��վ����Ʊ��78h��
14     ��Ա��Ʊ1��վ����������ͨ�����ͣ�Ʊ��28h��
15     ��Ա��Ʊ2��վ���������ͨר��Ʊ���ͣ�Ʊ��82h
16     �ô�������1A(D1-��ͨ������ͯ)Ʊ��վ����Ʊ��30h��
17     �ô�������1B(D2-��ͨ��������)Ʊ��վ����Ʊ��19h��
18     ��VP(VP-���������)Ʊ��վ����Ʊ��80h
19     ��վ��Ʊ�����ܾ���
20     ������
21     ��ֵ��ͨ��-���˿ۿ�ֵ��Ʊ��04h��
22     ����Ʊ�ۿ�ֵ��Ʊ��64h��
23     Ա��Ʊ1�ۿ�ֵ��������ͨ�����ͣ�Ʊ��28h��
24     Ա��Ʊ2�ۿ�ֵ�������ͨר��Ʊ���ͣ�Ʊ��82h
25     ��������1A(D1-��ͨ������ͯ)Ʊ�ۿ�ֵ��Ʊ��30h��
26     ��������1B(D2-��ͨ��������)Ʊ�ۿ�ֵ��Ʊ��06h��
27     VP(VP-���������)Ʊ�ۿ�ֵ��Ʊ��80h
28     �Ǹ߷�ʱ�γ�վ��
29     ���ɳ�վ��
30     ͬվ��վ��
31     �Ϸ��Ĳ���Ʊ��
32     ����ֵ��ͨ�������˴ν���
33     ����Ʊʣ��Ʊֵ����ֵ��ͨ�������˴ν���
33     ����Ʊʣ��Ʊֵ
34     ����һƱͨ�����˴ν���
35     ��ͨ���Żݴ���
36     ��ͨ���Żݽ��
37     PBOC����վ����
38     PBOC����վ����
39     PBOC��-�ۿ�ֵ
40     PBOC���Żݴ���
41     PBOC���Żݽ��
42     բ������ģʽ��վ��
43     բ������ģʽ��վ��44    ��ʱ�ƴ�Ʊ��վ����Ʊ��84h��
45    ��ʱ�ƴ�Ʊ��վ����Ʊ��84h��
46 ~ 63    [δ����]
*/
	// bytType = pUpdateCmdInfo->bytUpdateMode;

	switch(pUpdateCmdInfo->bytTransEventCode)
	{
            case 0x24:  // Ʊ�ۿ�
            case 0x25:  // ���ۿ�
            case 0x40:  // ??��վ���˿ۿ�
            case 0x41:  // ??��վ���˿ۿ�
                    {
                        pInOutSeqSn->lBomRegist6000[1]++;  // ??��վ��������Ʊ�֣�
                        switch(pAnalysisInfo->bytTicketType)
                        {
                            
                            case 0x04://
                            {
                                pInOutSeqSn->lBomRegist6000[11]++;//�ô�ֵ��ͨ����վ��-���ˣ�Ʊ��04h��
                                pInOutSeqSn->lBomRegist6000[21] += pUpdateCmdInfo->dwTransValue;
                            }
                            break;
                            case 0x64:     //�õ���Ʊ��վ����Ʊ��64h��
                            {
                                pInOutSeqSn->lBomRegist6000[12]++;
                                pInOutSeqSn->lBomRegist6000[20]++;                                
                                pInOutSeqSn->lBomRegist6000[22] += pUpdateCmdInfo->dwTransValue;                            
                            }
                            break;
                            case 0x78: //  �ó˴�Ʊ��վ����Ʊ��78h��
                            {
                                pInOutSeqSn->lBomRegist6000[13]++;                   
                            }
                            break;
                            case 0x28:  // ��Ա��Ʊ1��վ����������ͨ�����ͣ�Ʊ��28h��
                            {
                                pInOutSeqSn->lBomRegist6000[14]++;
                                pInOutSeqSn->lBomRegist6000[23] += pUpdateCmdInfo->dwTransValue;         
                            }
                            break;
                            case 0x82:  // ��Ա��Ʊ2��վ���������ͨר��Ʊ���ͣ�Ʊ��82h
                            {
                                pInOutSeqSn->lBomRegist6000[15]++;
                                pInOutSeqSn->lBomRegist6000[24] += pUpdateCmdInfo->dwTransValue;      
                            }    
                            break;
                            case 0x30:  //�ô�������1A(D1-��ͨ������ͯ)Ʊ��վ����Ʊ��30h��
                            {
                                pInOutSeqSn->lBomRegist6000[16]++;
                                pInOutSeqSn->lBomRegist6000[25] += pUpdateCmdInfo->dwTransValue;      
                            } 
                            break;
                            case 0x19:  //  �ô�������1B(D2-��ͨ��������)Ʊ��վ����Ʊ��19
                            {
                                pInOutSeqSn->lBomRegist6000[17]++;
                                pInOutSeqSn->lBomRegist6000[26] += pUpdateCmdInfo->dwTransValue;      
                            } 
                                break;
                            case 0x80:  // ��VP(VP-���������)Ʊ��վ����Ʊ��80h
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
            
            case 0x26:   // ??��վ���˽�վ ticket
            case 0x27:   //  ??��վ���˽�վ  card
            case 0x2A:
            case 0x2B:
            case 0x3E:  // ??β���Ż�
            case 0x3F:  // ??���ý�ͨ���ۻ����ѽ��
            {
                pInOutSeqSn->lBomRegist6000[0]++;  // ??��վ��������Ʊ�֣�                
                switch(pAnalysisInfo->bytTicketType)
                {
                    case 0x04://
                    {
                        pInOutSeqSn->lBomRegist6000[2]++;//   �ô�ֵ��ͨ����վ��-���ˣ�Ʊ��04h��
                    }
                    break;
                    case 0x64:     // �õ���Ʊ��վ����Ʊ��64h��
                    {
                        pInOutSeqSn->lBomRegist6000[3]++;         
                    }
                    break;
                    case 0x78: //   �ó˴�Ʊ��վ����Ʊ��78h��
                    {
                        pInOutSeqSn->lBomRegist6000[4]++;                   
                    }
                    break;
                    case 0x28:  //     ��Ա��Ʊ1��վ����������ͨ�����ͣ�Ʊ��28h
                    {
                        pInOutSeqSn->lBomRegist6000[5]++;
                    }
                    break;
                    case 0x82:  //  ��Ա��Ʊ2��վ���������ͨר��Ʊ���ͣ�Ʊ��82h
                    {
                        pInOutSeqSn->lBomRegist6000[6]++;
                    }    
                    break;
                    case 0x30:  // �ô�������1A(D1-��ͨ������ͯ)Ʊ��վ����Ʊ��30h��
                    {
                        pInOutSeqSn->lBomRegist6000[7]++;  
                    } 
                    break;
                    case 0x19:  // �ô�������1B(D2-��ͨ��������)Ʊ��վ����Ʊ��19h��
                    {
                        pInOutSeqSn->lBomRegist6000[8]++;
                    } 
                        break;
                    case 0x80:  //��VP(VP-���������)Ʊ��վ����Ʊ��80h
                    {
                        pInOutSeqSn->lBomRegist6000[9]++;   
                    } 
                      break;  
                }
            }
            break;
            case 0x35:  // ??�������Ŀ�״̬����		Ʊ/��ͨ��
            case 0x3B:  // ??������տ���Ǯ������
            {
                
            }
            break;
/*
 ??91h	�����/��վ	0	��Ʊ���ܾ�
92h	����ʱ������	0	��Ʊ���ܾ�
93h	���˳�Ʊ	0	��Ʊ���ܾ�
94h	У��ʹ���	0	��Ʊ���ܾ�
95h	����/����	0	��Ʊ���ܾ�
96h	�Ƿ���δ�����Ʊ�����ͣ�	0	��Ʊ���ܾ�
97h	���ɽ��ܵ�Ʊ������	0	��ڴ����ܾ�
98h	�ٴν�վ	0	��ڴ����ܾ�
99h	��������ǰʹ��	0	��ڴ����ܾ�
9Ah	ƱֵΪ0�ܾ�	0	��ڴ����ܾ�
9Bh	����ͬվ����	0	��վ�����ܾ�
9Ch	ʣ��Ʊֵ�������Ʊ��	0	��ڴ����ܾ�
9Dh	ͬվ����	0	��վ�����ܾ�
*/            
            case 0x88:  // ??������Ʊ�ڳ�վʱ���ܾ�
            case 0x89:  // ??������Ʊ�ڽ�վʱ���ܾ�                
            case 0x8A:  // ??������Ʊ��ڴ�������
            case 0x8D:  // ??����ʧ��
            case 0x90:  //����
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
               if(0x20 == pResPonseInfoData->bytWorkMode) // ��վ
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
        
        if(0x60 == pResPonseInfoData->bytWorkMode) // ���ջ���
        {
            pInOutSeqSn->lBomRegist6000[10]++;
        }

	return;
}

