//
// Created by zhujia on 3/28/18.
//

#ifndef AGM_UIDISPLAY_H
#define AGM_UIDISPLAY_H

#include <string>
#include <memory>
#include <iostream>
using namespace std;


#pragma pack(push, 1)
enum AgmDirection{IN, OUT};
enum eGateWorkMode{eEn=1,eEx,eBi,eBien,eBiex};

struct DisplayInfo{
    string rejectCode;    //拒绝砄1�7
    int passNum;          //刷卡未�1�7�行人数
    string eventList; //事件砄1�7
    float remainAmt; //单位：元
    AgmDirection direction;
    void PrintInfo()
    {
        cout<<"rejectCode:"<<rejectCode<<endl;
        cout<<"passNum:"<<passNum<<endl;
        cout<<"evetnList:"<<eventList<<endl;
        cout<<"remainAmt:"<<remainAmt<<endl;
    }
    void Reset()
    {
        rejectCode.clear();
        passNum = 0;
        eventList.clear();
        remainAmt = 0;
        direction = IN;
    }
    DisplayInfo &operator=(DisplayInfo &tt)
    {
        rejectCode = tt.rejectCode;
        passNum = tt.passNum;
        eventList = tt.eventList;
        remainAmt = tt.remainAmt;
        direction = tt.direction;
    }
};
#pragma pack(pop)



class UiDisplay{
public:
    UiDisplay();
    ~UiDisplay();
    UiDisplay(const UiDisplay &) = delete;

    UiDisplay(UiDisplay &&) = delete;

    UiDisplay &operator=(const UiDisplay &) = delete;

    UiDisplay &operator=(UiDisplay &&) = delete;

    /*
     * 设备处于正常工作状�1�7�，可正常刷卄1�7
     */
    void normal(DisplayInfo disInfo);

    /*
     * 设备储�1�7�票处理部分故障，只能接收单程票
     */
    void sjtonly(DisplayInfo disInfo);

    /*
     * 设备单程票处理部分故障，只能接收储�1�7�票
     */
    void svtonly(DisplayInfo disInfo);

    /*
     * 设备主控程序启动过程丄1�7
     * mode的取值范围：en - 进站模式，ex - 出站模式，bi - 双向模式＄1�7 bien - 双向闸机单进模式＄1�7 biex - 双向闸机单出模式
     */
    void initial(string mode, string devinfo);

    /*
     * 设备运营时间到或接收到关闭服务命仄1�7
     */
    void stopsvc(DisplayInfo disInfo);

    /*
     * 设备发生无法提供任何棄1�7票服务的事件
     */
    void outofsvc(DisplayInfo disInfo);

    /*
     * 乘客刷卡结果为不允许进站
     * 出站时同样存在此类问题，扄1�7以对于双向闸机来说，霄1�7要提供direction(IN/OUT)
     */
    void gotosvc(DisplayInfo disInfo);

    /*
     * 设备处理票卡信息不完整，对于双向闸机，需要提供direction(IN/OUT)
     */
    void tryagain(DisplayInfo disInfo);

    /*
     * 乘客将应回收的车票放置在外置读卡器天线处
     */
    void instkt(DisplayInfo disInfo);

    /*
     * 乘客在非高峰时段使用只允许在高峰时段内使用的车票，对于双向闸机，霄1�7要提供direction(IN/OUT)
     */
    void offpeak(DisplayInfo disInfo);

    /*
     * 乘客在高峰时段使用只允许在非高峰时段内使用的车票，对于双向闸机，霄1�7要提供direction(IN/OUT)
     */
    void peak(DisplayInfo disInfo);

    /*
     * 设备处于维修丄1�7
     */
    void maintenance(DisplayInfo disInfo);

    /*
     * 乘客刷卡结果为允许进站（不显示票卡信息的情况，参规1�71-5＄1�7
     */
    void entry(DisplayInfo disInfo);

    /*
     * 乘客刷卡结果为允许出站（不显示票卡信息的情况，参规1�71-5＄1�7
     */
    void exit(DisplayInfo disInfo);

    /*
     * 乘客刷卡结果为允许进站（计程车票＄1�7
     */
    void entry_v(DisplayInfo disInfo);

    /*
     * 乘客刷卡结果为允许出站（计程车票＄1�7
     */
    void exit_v(DisplayInfo disInfo);

    /*
     * 乘客刷卡结果为允许进站（计次车票＄1�7
     */
    void entry_t(DisplayInfo disInfo);

    /*
     * 乘客刷卡结果为允许出站（计次车票＄1�7
     */
    void exit_t(DisplayInfo disInfo);

    /*
     * 乘客刷卡结果为允许进站（计时计次票，霄1�7显示票卡面额的情况）
     */
    void entry_dt(DisplayInfo disInfo);

    /*
     * 乘客刷卡结果为允许出站（计时计次票，霄1�7显示票卡面额的情况）
     */
    void exit_dt(DisplayInfo disInfo);

    /*
     * 设备处于紧�1�7�模式（进站棄1�7票机或双向检票机进站端）
     */
    void emergency(DisplayInfo disInfo);

    /*
     * 乘客刷卡结果为允许进站（公务票）
     */
    void entry_e(DisplayInfo disInfo);

    /*
     * 乘客刷卡结果为允许出站（公务票）
     */
    void exit_e(DisplayInfo disInfo);

private:
    class impl;
    impl* angel;
};

#endif //AGM_UIDISPLAY_H
