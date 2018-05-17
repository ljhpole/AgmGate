//
// Created by zhujia on 3/28/18.
//
#include "uidisplay.h"
#include "agm.h"
#include <iostream>
#include <unistd.h>
#include <sys/msg.h>

using namespace std;
using json = nlohmann::json;

class UiDisplay::impl{
public:
    void connectServer();

    void pkgAndSend(string cmd, DisplayInfo disInfo);

    int msgid;
    string m_sDisplayPre;
};

UiDisplay::UiDisplay() : angel(new impl){ this->angel->connectServer(); }

UiDisplay::~UiDisplay(){ delete angel; }

#define MSGPERM 0600    // msg queue permission

void UiDisplay::impl::connectServer(){
    key_t msgkey = ftok("/var", 'c');
    if(-1 != msgkey){
        msgid = msgget(msgkey, 0644 | IPC_CREAT);
    }
}

void UiDisplay::impl::pkgAndSend(string cmd, DisplayInfo disInfo){
    char buff[2024];
    //disInfo.PrintInfo();
    int iIndex = 0;
//        printf("begin %d\n",iIndex++);
    json j;
    j[K_CMD] = cmd;
    j[K_DIRECTION] = IN == disInfo.direction ? "in" : "out";
//        printf("begin %d\n",iIndex++);    
    j[K_REJECT] = disInfo.rejectCode;
//        printf("begin %d\n",iIndex++);    
    j[K_PASSENGER] = disInfo.passNum;
//        printf("begin %d\n",iIndex++);      
    j[K_EVENT] = disInfo.eventList;
//            printf("begin %d\n",iIndex++);  
    j[K_REMAIN] = disInfo.remainAmt;

//        printf("begin %d\n",iIndex++);  
    string s = j.dump();
    
    if(m_sDisplayPre!=s)
    {
        m_sDisplayPre = s;
        printf("end %s\n",s.c_str());
    }

    msgui_t uimsg = {0};
    uimsg.msgtype = MSG_TYPE_UI;
    memcpy(uimsg.data, s.c_str(), s.length());
    msgsnd(msgid, &uimsg, sizeof(uimsg.data), IPC_NOWAIT);
}

void UiDisplay::normal(DisplayInfo disInfo){ this->angel->pkgAndSend("normal", disInfo); }

void UiDisplay::sjtonly(DisplayInfo disInfo){ this->angel->pkgAndSend("sjtonly", disInfo); }

void UiDisplay::svtonly(DisplayInfo disInfo){ this->angel->pkgAndSend("svtonly", disInfo); }

void UiDisplay::initial(string mode, string devinfo){
    char buff[1024];
    json j;
    j["mode"] = mode;
    j["device"] = devinfo;

    string s = j.dump();

    msgui_t uimsg = {0};
    uimsg.msgtype = MSG_TYPE_UI;
    memcpy(uimsg.data, s.c_str(), s.length());
    msgsnd(angel->msgid, &uimsg, sizeof(uimsg.data), IPC_NOWAIT);
}

void UiDisplay::stopsvc(DisplayInfo disInfo){ this->angel->pkgAndSend("stopsvc", disInfo); }

void UiDisplay::outofsvc(DisplayInfo disInfo){ this->angel->pkgAndSend("outofsvc", disInfo); }

void UiDisplay::gotosvc(DisplayInfo disInfo){ this->angel->pkgAndSend("gotosvc", disInfo); }

void UiDisplay::tryagain(DisplayInfo disInfo){ this->angel->pkgAndSend("tryagain", disInfo); }

void UiDisplay::instkt(DisplayInfo disInfo){ this->angel->pkgAndSend("instkt", disInfo); }

void UiDisplay::offpeak(DisplayInfo disInfo){ this->angel->pkgAndSend("offpeak", disInfo); }

void UiDisplay::peak(DisplayInfo disInfo){ this->angel->pkgAndSend("peak", disInfo); }

void UiDisplay::maintenance(DisplayInfo disInfo){ this->angel->pkgAndSend("maintenance", disInfo); }

void UiDisplay::entry(DisplayInfo disInfo){ this->angel->pkgAndSend("entry", disInfo); }

void UiDisplay::exit(DisplayInfo disInfo){ this->angel->pkgAndSend("exit", disInfo); }

void UiDisplay::entry_v(DisplayInfo disInfo){ this->angel->pkgAndSend("entry_v", disInfo); }

void UiDisplay::exit_v(DisplayInfo disInfo){ this->angel->pkgAndSend("exit_v", disInfo); }

void UiDisplay::entry_t(DisplayInfo disInfo){ this->angel->pkgAndSend("entry_t", disInfo); }

void UiDisplay::exit_t(DisplayInfo disInfo){ this->angel->pkgAndSend("exit_t", disInfo); }

void UiDisplay::entry_dt(DisplayInfo disInfo){ this->angel->pkgAndSend("entry_dt", disInfo); }

void UiDisplay::exit_dt(DisplayInfo disInfo){ this->angel->pkgAndSend("exit_dt", disInfo); }

void UiDisplay::emergency(DisplayInfo disInfo){ this->angel->pkgAndSend("emergency", disInfo); }

void UiDisplay::entry_e(DisplayInfo disInfo){ this->angel->pkgAndSend("entry_e", disInfo); }

void UiDisplay::exit_e(DisplayInfo disInfo){ this->angel->pkgAndSend("exit_e", disInfo); }
