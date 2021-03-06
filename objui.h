#ifndef OBJUI_H
#define OBJUI_H

#include <QObject>
#include <QStateMachine>
#include <QtGui/QKeyEvent>
#include <QtGui/QKeyEventTransition>
#include <QTcpSocket>
#include <QLocale>
#include <QTimer>
#include <QDateTime>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QAbstractSocket>
#include <QUdpSocket>
#include <QHostAddress>

#include <iostream>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/input.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include <arpa/inet.h>

#include "objpara.h"
#include "objstatus.h"
#include "numeditor.h"
#include "objpage.h"
#include "editorcallid.h"
#include "editoridstr.h"
#include "editorratec.h"

//#include "kt0000.h"
#include "jsonrpc/cu_rpc_control.h"

class objui : public objPage
{
    Q_OBJECT
public:
    explicit objui(QObject *parent = 0);

    bool m_boolStatusPageP2P;
    // status struct
    struct RadioLinkParamsChanged m_RadioLinkParamsChanged;
    struct P2PMode m_P2PMode;// status.p2p txFreq , txRate
    struct RadioLinkStateChanged m_RadioLinkStateChanged; // state.c txFreq txRate
    struct CUState m_CUState;
    void setBUCfreq(qint64 freq);
    void setLNBfreq(qint64 freq);

    bool m_bInitDone;

    int m_nCountTimerCall;

    bool m_bMenuCall_login;
    bool m_bMenuCall_logout;
    bool m_bMenuCall_call;
    bool m_bMenuCall_hangup;
    int m_nCursorMenuCall;

    CuRpcControl m_cu;
    struct TDMStruct m_stTDM;

    QTcpSocket *m_pTcp;
    void writeTcp(QByteArray ba);

    bool m_bErrChangeWorkMode;

    QUdpSocket *m_pUDPkey;
    void initUDPkey();

    int m_nShowStatusPage1;
    //bool m_bEnableKeyboard;
    bool m_bEnable1s;

    objPara m_para;
    objstatus m_status;
    numEditor m_numEditor;
    //editorCallID m_editorCallID;
    editorIDstr m_editorIDstr;
    editorRateC m_editorRate;

    QStateMachine *m_pMachine;

    QState *m_pStateGroupTimeout;

    QState *m_pStateLogo;
    QState *m_pStateAbout;
    QState *m_pStateStatusPage1;
    QState *m_pStateStatusPage2;
    QState *m_pStateStatusPage3;

    QState *m_pStateMenuTestCtrl;
    QState *m_pStateMenuTestFreq;
    QState *m_pStateMenuTestPwr;
    QState *m_pStateEditorTestPwr;
    QState *m_pStateEditorTestFreq;

    QState *m_pStateMenuRadioSetting;// menu03
    QState *m_pStateMenuCtrl;
    QState *m_pStateMenuPara;
    QState *m_pStateMenuWorkMode;

    QState *m_pStateBUCpwr;
    QState *m_pStateBUC10m;
    QState *m_pStateLNBpwr;
    QState *m_pStateLNB10m;

    QState *m_pStateMenuBUCpwr;
    QState *m_pStateMenuBUC10m;
    QState *m_pStateMenuBUCfreq;
    QState *m_pStateMenuLNBpwr;
    QState *m_pStateMenuLNB10m;
    QState *m_pStateMenuLNBfreq;

    QState *m_pStateWorkMode1;// p2p menu.1
    QState *m_pStateWorkMode2;// central menu.2

    QState *m_pStateDevMode1;// bridge
    QState *m_pStateDevMode2;// router

    QState *m_pStateMenuCall;
    //QState *m_pStateMenu11;
    QState *m_pStateMsgZZHJ;// zheng.zai.hu.jiao
    QState *m_pStateMsgHJCG;//
    QState *m_pStateMsgZZGD;//
    QState *m_pStateMsgGDCG;//

    QState *m_pStateMsgSetBUCpwr;//
    QState *m_pStateMsgSetBUC10m;//
    QState *m_pStateMsgSetLNBpwr;//
    QState *m_pStateMsgSetLNB10m;//

    QState *m_pStateMsgZZRW;// ru.wang
    QState *m_pStateMsgZZTW;// tui.wang

    QState *m_pStateEditorBUCfreq;
    QState *m_pStateEditorLNBfreq;

    QState *m_pStateEditorTxFreq;
    QState *m_pStateEditorRxFreq;
    QState *m_pStateEditorTxRate;
    QState *m_pStateEditorRxRate;
    QState *m_pStateEditorTxRatePSK;
    QState *m_pStateEditorRxRatePSK;
    QState *m_pStateEditorPower;

    QState *m_pStateEditorTDM;
    QState *m_pStateEditorTDM2;
    //QState *m_pStateEditorNumber;
    QState *m_pStateEditorTxRateC;
    QState *m_pStateEditorRxRateC;
    QState *m_pStateEditorPowerCentral;

    //QState *m_pStateParaPage1;// txFreq
    //QState *m_pStateParaPage11;// rxFreq
    //QState *m_pStateParaPage12;//
    //QState *m_pStateParaPage13;
    QState *m_pStateParaPage1a;// txFreq
    QState *m_pStateParaPage11a;// rxFreq
    QState *m_pStateParaPage12a;// BUCfreq
    QState *m_pStateParaPage13a;// LNBfreq

    QState *m_pStateParaPage2;
    QState *m_pStateParaPage21;
    //QState *m_pStateParaPage22;
    //QState *m_pStateParaPage23;

    //QState *m_pStateParaPage30;// txRate
    //QState *m_pStateParaPage31;
    //QState *m_pStateParaPage32;
    //QState *m_pStateParaPage40;// rxRate
    //QState *m_pStateParaPage41;
    //QState *m_pStateParaPage42;

    QState *m_pStateParaPage30a;// txRate
    QState *m_pStateParaPage31a;
    QState *m_pStateParaPage40a;// rxRate
    QState *m_pStateParaPage41a;

    QState *m_pStateEditRxPSK;
    QState *m_pStateEditTxPSK;

    QState *m_pStateParaP11c;// tdm1
    QState *m_pStateParaP12c;// tdm2
    QState *m_pStateParaP13c;// power
    QState *m_pStateParaP14c;// callID
    QState *m_pStateEditCallID;

    // ==> CP11 , CP12 , ...
    //QState *m_pStateParaPage1c;// txFreq
    //QState *m_pStateParaPage11c;// rxFreq
    //QState *m_pStateParaPage12c;//
    //QState *m_pStateParaPage13c;
    QState *m_pStateParaPage2c;
    QState *m_pStateParaPage21c;
    QState *m_pStateParaPage22c;

    QState *m_pStateParaP21c; // txRate
    QState *m_pStateParaP22c; // RxRate
    QState *m_pStateParaP23c; // devMode


    QTimer *m_pTimer60;
    QTimer *m_pTimer1s;
    QTimer *m_pTimer2s;
    QTimer *m_pTimerKey;// 200ms

    //void buildMachine();

    void getColorMenuCall(int n,int *pc,int *pbg);

    void doMenuCall();
    void doCallP2P();
    void doCallP2Pagain();
    void doDisconnectP2P();
    void doMenuPara();// enter paraPage1
    void changeSelectMenu10(int step);

    void doLogin();
    void doLogout();

    void cmdLogin();
    void cmdLogout();
    void cmdNCCcall();
    void cmdNCChangup();

    void showDataParaPage1c();
    void showDataParaPage2();
    void showDataParaPage2c();

    void showDataParaP2c();

    void showDataParaPage1();
    void showDataParaPage1a();

    void showDataParaP1c();

    void showP3a();
    void showP3();
    void showP4();

    QString getTimeSpan();
    QString getTimeRunning();

    void showStatusPage1c();

    void getPara();// tdm.freq , power
    void getParaModem();
    void setTDM();
    void setPower100c();

    void showPSK(int psk);

    void showBUCLNB();

    void setBUCpwr();
    void setLNBpwr();
    void setBUC10m();
    void setLNB10m();

    QString getSWstr(QString str,int sw);

    void showMenuCallP2P();
    void showMenuCallNCC();
    void showMenuCallNCClogout();
    void showMenuCallgray();

    bool isEnableLogin();
    bool isEnableLogout();
    bool isEnableCall();
    bool isEnableHang();

    void setEnableMenuCall();
    void moveCursorMenuCall(int inc);

    bool setCMD(int n);

    void statusP2Pcall();
    void statusP2Phangup();
    void statusNCClogout();
    void statusNCClogin();
    void statusNCCcall();
    void statusNCChangup();

    void getCUstate();

    void showTestCtrl();
    void doStartTest();
    void doStopTest();

    bool isStatusP2P();
signals:
    void sigEVzzhjBack();
    void sigEVzzhjBackM();
    void sigEVzzhjPaint();

    void sigStateTransitionRepaint();

    void sigStateTransitionUp();
    void sigStateTransitionDown();
    void sigStateTransitionLeft();
    void sigStateTransitionRight();
    void sigStateTransitionEnter();
    void sigStateTransitionBackspace();

    void sigStateTransition1s();
    void sigStateTransitionTimeout();
    void sigStateTransitionNext();
    void sigStateTransitionBack();
    void sigStateTransitionP2P();
    void sigStateTransitionCentral();

    void sig0000();
    void sigKeyEnter();
    void sigKeyBack();
    void sigKeyUp();
    void sigKeyDown();
    void sigKeyLeft();
    void sigKeyRight();

public slots:
    void slotGetRadioParams();
    void slotGetSWStatus();// BUC LNB pwr, 10m

    void slotShowMenuTestCtrl();
    void slotShowMenuTestFreq();
    void slotShowMenuTestPwr();
    void slotShowEditTestPwr();
    void slotShowEditTestFreq();

    void slotUDPkey();
    void slotTcpOn();
    void slotTcpOff();
    void slotInitTcp();

    void slotKeyEnable();
    void slotGetCUstate();
    void slotGetCUstateInit();// init , only once
    void slotGetCUstateInitLogout();// init exit waiting.tdm
    void initMachine();

    void slotTDMConfig(QByteArray ba);
    void slotModemConfig(QByteArray ba);
    void slotPowerConfig(QByteArray ba);
    void slotP2PmodeParam(QByteArray ba);
    void slotCUState(QByteArray ba);
    void slotSessionState(QByteArray ba);
    void slotRadioLinkState(QByteArray ba);
    void slotRadioLinkParams(QByteArray ba);
    void slotReadTCP();
    void slotErrTCP();

    void slotTest();
    void slotTestStop();
    void slot00enter();
    void slot00exit();
    void slot01enter();

    void slotKET();
    void slot0000();

    virtual void slotKey(int);

    void slotEVzzhjBack();
    void slotEVzzhjBackM();
    void slotEVzzhjPaint();

    void slotStateTransitionUp();
    void slotStateTransitionDown();
    void slotStateTransitionLeft();
    void slotStateTransitionRight();
    void slotStateTransitionEnter();
    void slotStateTransitionBackspace();
    void slotStateTransitionNext();
    void slotStateTransitionBack();
    void slotStateTransitionP2P();
    void slotStateTransitionCentral();
    void slotStateTransitionRepaint();

    void slotShowStatusPage1();
    void slotShowStatusPage2();
    void slotShowStatusPage3();

    void slotShowAbout();

    void slotShowMenuCall();
    void slotShowMenu11();
    void slotShowMenu12();

    void slotShowMode1();
    void slotShowMode2();
    void slotShowDevMode1();
    void slotShowDevMode2();

    void slotShowParaPage1();
    void slotShowParaPage11();
//    void slotShowParaPage12();
//    void slotShowParaPage13();

    void slotShowParaPage1a();
    void slotShowParaPage11a();
    void slotShowParaPage12a();
    void slotShowParaPage13a();

    void slotShowParaPage2();
    void slotShowParaPage21();
    void slotShowParaPage22();
    void slotShowParaPage23();

    void slotShowP30();
    void slotShowP31();
    void slotShowP32();
    void slotShowP40();
    void slotShowP41();
    void slotShowP42();

    void slotShowP30a();
    void slotShowP31a();
    void slotShowP40a();
    void slotShowP41a();

    void slotShowParaP11c();
    void slotShowParaP12c();
    void slotShowParaP13c();
    void slotShowParaP14c();

    void slotShowParaPage1c();
    void slotShowParaPage11c();
    void slotShowParaPage12c();
    //void slotShowParaPage13c();
    void slotShowParaPage2c();
    void slotShowParaPage21c();
    void slotShowParaPage22c();

    void slotShowParaP21c();
    void slotShowParaP22c();
    void slotShowParaP23c();


    void slotShowEditBUCfreq();
    void slotShowEditLNBfreq();

    void slotShowEditTxFreq();
    void slotShowEditRxFreq();
    void slotShowEditTxRate();
    void slotShowEditRxRate();
    void slotShowEditTxRatePSK();
    void slotShowEditRxRatePSK();
    void slotShowEditPower();

    void slotShowEditTDM();
    void slotShowEditTDM2();
    void slotShowEditNumber();
    //void slotShowEditCallID();
    void slotShowEditIDstr();
    void slotShowEditTxRateC();
    void slotShowEditRxRateC();
    void slotShowEditPowerCentral();

    void slotShowMenu00();
    void slotShowMenu01();
    void slotShowMenu02();
    void slotShowMenu03();// radio setting ( BUC , LNB ..

    void slotShow_BUCpwr();
    void slotShow_BUC10m();
    void slotShow_LNBpwr();
    void slotShow_LNB10m();

    void slotShowBUCpwr();
    void slotShowBUC10m();
    void slotShowBUCfreq();
    void slotShowLNBpwr();
    void slotShowLNB10m();
    void slotShowLNBfreq();

    void slotShowMsgSetBUCpwr();
    void slotShowMsgSetBUC10m();
    void slotShowMsgSetLNBpwr();
    void slotShowMsgSetLNB10m();

    void slotShowMsgZZHJ();
    void slotShowMsgHJCG();
    void slotShowMsgZZGD();
    void slotShowMsgGDCG();

    void slotShowMsgZZRW();
    void slotShowMsgZZTW();

    void slotShowLogo();

    void slotGetP2Pstatus();
    void slotKey1s();
    void slotKey2s();

    void slotShowEditRxPSK();
    void slotShowEditTxPSK();

};

#endif // OBJUI_H
