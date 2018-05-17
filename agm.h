//
// Created by zhujia on 3/27/18.
//

#ifndef AGM_AGM_H
#define AGM_AGM_H

#include "json.hpp"

using json = nlohmann::json;

enum AgmMode{
    Entry, Exit, Bidirection, BidirectionEntry, BidirectionExit
};

extern AgmMode agmMode;

extern json agmStyle;

#define K_CMD "cmd"
#define K_DIRECTION "direction"
#define K_MODE "mode"
#define K_REJECT "reject"
#define K_PASSENGER "passenger"
#define K_GUIDE_ZH "guide-zh"
#define K_GUIDE_EN "guide-en"
#define K_REMAIN "remain"
#define K_EVENT "event"
#define K_DEVICE "device"

#define MSG_TYPE_UI 0xABCD
struct msgui_t{
    long msgtype;
    char data[1024];
};


#endif // AGM_AGM_H
