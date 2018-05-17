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
    string rejectCode;    //鎷掔粷鐮�
    int passNum;          //鍒峰崱鏈�氳浜烘暟
    string eventList; //浜嬩欢鐮�
    float remainAmt; //鍗曚綅锛氬厓
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
     * 璁惧澶勪簬姝ｅ父宸ヤ綔鐘舵�侊紝鍙甯稿埛鍗�
     */
    void normal(DisplayInfo disInfo);

    /*
     * 璁惧鍌ㄥ�肩エ澶勭悊閮ㄥ垎鏁呴殰锛屽彧鑳芥帴鏀跺崟绋嬬エ
     */
    void sjtonly(DisplayInfo disInfo);

    /*
     * 璁惧鍗曠▼绁ㄥ鐞嗛儴鍒嗘晠闅滐紝鍙兘鎺ユ敹鍌ㄥ�肩エ
     */
    void svtonly(DisplayInfo disInfo);

    /*
     * 璁惧涓绘帶绋嬪簭鍚姩杩囩▼涓�
     * mode鐨勫彇鍊艰寖鍥达細en - 杩涚珯妯″紡锛宔x - 鍑虹珯妯″紡锛宐i - 鍙屽悜妯″紡锛� bien - 鍙屽悜闂告満鍗曡繘妯″紡锛� biex - 鍙屽悜闂告満鍗曞嚭妯″紡
     */
    void initial(string mode, string devinfo);

    /*
     * 璁惧杩愯惀鏃堕棿鍒版垨鎺ユ敹鍒板叧闂湇鍔″懡浠�
     */
    void stopsvc(DisplayInfo disInfo);

    /*
     * 璁惧鍙戠敓鏃犳硶鎻愪緵浠讳綍妫�绁ㄦ湇鍔＄殑浜嬩欢
     */
    void outofsvc(DisplayInfo disInfo);

    /*
     * 涔樺鍒峰崱缁撴灉涓轰笉鍏佽杩涚珯
     * 鍑虹珯鏃跺悓鏍峰瓨鍦ㄦ绫婚棶棰橈紝鎵�浠ュ浜庡弻鍚戦椄鏈烘潵璇达紝闇�瑕佹彁渚沝irection(IN/OUT)
     */
    void gotosvc(DisplayInfo disInfo);

    /*
     * 璁惧澶勭悊绁ㄥ崱淇℃伅涓嶅畬鏁达紝瀵逛簬鍙屽悜闂告満锛岄渶瑕佹彁渚沝irection(IN/OUT)
     */
    void tryagain(DisplayInfo disInfo);

    /*
     * 涔樺灏嗗簲鍥炴敹鐨勮溅绁ㄦ斁缃湪澶栫疆璇诲崱鍣ㄥぉ绾垮
     */
    void instkt(DisplayInfo disInfo);

    /*
     * 涔樺鍦ㄩ潪楂樺嘲鏃舵浣跨敤鍙厑璁稿湪楂樺嘲鏃舵鍐呬娇鐢ㄧ殑杞︾エ锛屽浜庡弻鍚戦椄鏈猴紝闇�瑕佹彁渚沝irection(IN/OUT)
     */
    void offpeak(DisplayInfo disInfo);

    /*
     * 涔樺鍦ㄩ珮宄版椂娈典娇鐢ㄥ彧鍏佽鍦ㄩ潪楂樺嘲鏃舵鍐呬娇鐢ㄧ殑杞︾エ锛屽浜庡弻鍚戦椄鏈猴紝闇�瑕佹彁渚沝irection(IN/OUT)
     */
    void peak(DisplayInfo disInfo);

    /*
     * 璁惧澶勪簬缁翠慨涓�
     */
    void maintenance(DisplayInfo disInfo);

    /*
     * 涔樺鍒峰崱缁撴灉涓哄厑璁歌繘绔欙紙涓嶆樉绀虹エ鍗′俊鎭殑鎯呭喌锛屽弬瑙�1-5锛�
     */
    void entry(DisplayInfo disInfo);

    /*
     * 涔樺鍒峰崱缁撴灉涓哄厑璁稿嚭绔欙紙涓嶆樉绀虹エ鍗′俊鎭殑鎯呭喌锛屽弬瑙�1-5锛�
     */
    void exit(DisplayInfo disInfo);

    /*
     * 涔樺鍒峰崱缁撴灉涓哄厑璁歌繘绔欙紙璁＄▼杞︾エ锛�
     */
    void entry_v(DisplayInfo disInfo);

    /*
     * 涔樺鍒峰崱缁撴灉涓哄厑璁稿嚭绔欙紙璁＄▼杞︾エ锛�
     */
    void exit_v(DisplayInfo disInfo);

    /*
     * 涔樺鍒峰崱缁撴灉涓哄厑璁歌繘绔欙紙璁℃杞︾エ锛�
     */
    void entry_t(DisplayInfo disInfo);

    /*
     * 涔樺鍒峰崱缁撴灉涓哄厑璁稿嚭绔欙紙璁℃杞︾エ锛�
     */
    void exit_t(DisplayInfo disInfo);

    /*
     * 涔樺鍒峰崱缁撴灉涓哄厑璁歌繘绔欙紙璁℃椂璁℃绁紝闇�鏄剧ず绁ㄥ崱闈㈤鐨勬儏鍐碉級
     */
    void entry_dt(DisplayInfo disInfo);

    /*
     * 涔樺鍒峰崱缁撴灉涓哄厑璁稿嚭绔欙紙璁℃椂璁℃绁紝闇�鏄剧ず绁ㄥ崱闈㈤鐨勬儏鍐碉級
     */
    void exit_dt(DisplayInfo disInfo);

    /*
     * 璁惧澶勪簬绱ф�ユā寮忥紙杩涚珯妫�绁ㄦ満鎴栧弻鍚戞绁ㄦ満杩涚珯绔級
     */
    void emergency(DisplayInfo disInfo);

    /*
     * 涔樺鍒峰崱缁撴灉涓哄厑璁歌繘绔欙紙鍏姟绁級
     */
    void entry_e(DisplayInfo disInfo);

    /*
     * 涔樺鍒峰崱缁撴灉涓哄厑璁稿嚭绔欙紙鍏姟绁級
     */
    void exit_e(DisplayInfo disInfo);

private:
    class impl;
    impl* angel;
};

#endif //AGM_UIDISPLAY_H
