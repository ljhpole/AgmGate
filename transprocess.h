/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   transprocess.h
 * Author: ljhpole
 *
 * Created on April 7, 2018, 6:49 PM
 */

#ifndef TRANSPROCESS_H
#define TRANSPROCESS_H
void ModifyRegist(IGlobalDataManager * ptrGDM,tagResPonseInfoData  *pResPonseInfoData,TRAC_O_CLASSE *pTracelog,CardAnalysisInfo_t *pAnalysisInfo,BYTE bytUpdateMode,tagUpdateCmdInfo *pUpdateCmdInfo,IN_OUT_UPDATE_SEQ_SN *pInOutSeqSn,int &iRtn);
void FillRecord ( IGlobalDataManager * ptrGDM,tagUpdateCmdInfo *pUpdateCmdInfo,tagResPonseInfoData  *pResPonseInfoData ,IN_OUT_MSG_TXN_REC *pInTxnRec,tagCardOtcTransDataBak* pTransDataBak,LocalSLEInfo * pLocalInfo,BYTE &bytRtn);
void FillTransResponseData(tagResPonseInfoData  *pResPonseInfoData,Gate_msg *pReaderMsg,tagCardOtcTransDataBak *pCardTransData);
#endif /* TRANSPROCESS_H */

