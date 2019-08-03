#include "objui.h"

#include "kt0000.h"

#include "ketmenu00.h"

objui::objui(QObject *parent) :
    objPage(parent)
{
    m_bInitDone = false;

    m_boolStatusPageP2P = true;

    m_nCountTimerCall = 0;

    m_bMenuCall_login = false;
    m_bMenuCall_logout = false;
    m_bMenuCall_call = false;
    m_bMenuCall_hangup = false;
    m_nCursorMenuCall = 0;// 0--3  login,logout,call,hangup

    m_bErrChangeWorkMode = false;
    m_nShowStatusPage1= 0;
    m_bEnable1s = true;

    m_pTcp = new QTcpSocket(this);
    connect(m_pTcp,SIGNAL(readyRead()),this,SLOT(slotReadTCP()));
    connect(m_pTcp,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(slotErrTCP()));
    connect(m_pTcp,SIGNAL(connected()),this,SLOT(slotTcpOn()));
    connect(m_pTcp,SIGNAL(disconnected()),this,SLOT(slotTcpOff()));

    slotInitTcp();

    connect(m_cu.getRecvNotifyObject(),SIGNAL(sigCUState(QByteArray)),this,SLOT(slotCUState(QByteArray)));
    connect(m_cu.getRecvNotifyObject(),SIGNAL(sigRadioLinkState(QByteArray)),this,SLOT(slotRadioLinkState(QByteArray)));
    connect(m_cu.getRecvResponseObject(),SIGNAL(sigCUState(QByteArray)),this,SLOT(slotCUState(QByteArray)));
    connect(m_cu.getRecvResponseObject(),SIGNAL(sigSessionState(QByteArray)),this,SLOT(slotSessionState(QByteArray)));
    connect(m_cu.getRecvResponseObject(),SIGNAL(sigRadioLinkState(QByteArray)),this,SLOT(slotRadioLinkState(QByteArray)));
    connect(m_cu.getRecvResponseObject(),SIGNAL(sigP2PModeParam(QByteArray)),this,SLOT(slotP2PmodeParam(QByteArray)));
    connect(m_cu.getRecvResponseObject(),SIGNAL(sigTDMConfig(QByteArray)),this,SLOT(slotTDMConfig(QByteArray)));
    connect(m_cu.getRecvResponseObject(),SIGNAL(sigPowerConfig(QByteArray)),this,SLOT(slotPowerConfig(QByteArray)));
    connect(m_cu.getRecvResponseObject(),SIGNAL(sigModemConfig(QByteArray)),this,SLOT(slotModemConfig(QByteArray)));
    connect(m_cu.getRecvResponseObject(),SIGNAL(sigRadioLinkParams(QByteArray)),this,SLOT(slotRadioLinkParams(QByteArray)));

    m_pTimer60=new QTimer(this);
    m_pTimer60->setInterval(30000);
    m_pTimer60->setSingleShot(true);
    connect(m_pTimer60,SIGNAL(timeout()),this,SIGNAL(sigStateTransitionTimeout()));
    m_pTimer1s=new QTimer(this);
    m_pTimer1s->setInterval(1000);
    connect(m_pTimer1s,SIGNAL(timeout()),this,SLOT(slotKey1s()));
    m_pTimer2s=new QTimer(this);
    m_pTimer2s->setInterval(1500);
    connect(m_pTimer2s,SIGNAL(timeout()),this,SLOT(slotKey2s()));
    m_pTimerKey=new QTimer(this);
    m_pTimerKey->setInterval(4000);
    m_pTimerKey->setSingleShot(true);
    connect(m_pTimerKey,SIGNAL(timeout()),this,SLOT(slotKeyEnable()));

    m_pMachine = new QStateMachine;
    connect(this,SIGNAL(sigReadyOled()),this,SLOT(initMachine()));

    //m_cu.setConfigurationTDM
    //m_cu.getConfiguration(std:string("getConfigurationTDM"));
    //m_cu.getConfiguration(QString("getConfiguration").toStdString());

    initUDPkey();
}
void objui::slotTcpOn()
{
    writeLog(QByteArray("tcp connected\n"));
}
void objui::slotTcpOff()
{
    writeLog(QByteArray("tcp disconnected\n"));
}

void objui::writeTcp(QByteArray ba)
{
    if(m_pTcp->state()!=QAbstractSocket::ConnectedState) return;
    m_pTcp->write(ba);
}
void objui::setBUCpwr()
{
    std::string stdstr;
    struct ModemOutputStruct modem;

    if(m_para.m_swBUCpwr>0) modem.bucPowerSupply=1;
    else if(m_para.m_swBUCpwr<0) modem.bucPowerSupply=0;

    stdstr=m_cu.setConfigurationModem(modem);
    writeTcp(QString::fromStdString(stdstr).toUtf8());

}
void objui::setLNBpwr()
{
    std::string stdstr;
    struct ModemOutputStruct modem;

    if(m_para.m_swLNBpwr>0) modem.lnbPowerSupply=1;
    else if(m_para.m_swLNBpwr<0) modem.lnbPowerSupply=0;

    stdstr=m_cu.setConfigurationModem(modem);
    writeTcp(QString::fromStdString(stdstr).toUtf8());

}
void objui::setBUC10m()
{
    std::string stdstr;
    struct ModemOutputStruct modem;

    if(m_para.m_swBUC10m>0) modem.buc10MRef=1;
    else if(m_para.m_swBUC10m<0) modem.buc10MRef=0;

    stdstr=m_cu.setConfigurationModem(modem);
    writeTcp(QString::fromStdString(stdstr).toUtf8());

}
void objui::setLNB10m()
{
    std::string stdstr;
    struct ModemOutputStruct modem;

    if(m_para.m_swLNB10m>0) modem.lnb10MRef=1;
    else if(m_para.m_swLNB10m<0) modem.lnb10MRef=0;

    stdstr=m_cu.setConfigurationModem(modem);
    writeTcp(QString::fromStdString(stdstr).toUtf8());

}

void objui::setTDM()
{
    std::string stdstr;
    struct TDMStruct tdm;

    tdm.initTDMFrequency.clear();
    if(m_para.m_TDMfreq1>0) tdm.initTDMFrequency.push_back(m_para.m_TDMfreq1);
    if(m_para.m_TDMfreq2>0) tdm.initTDMFrequency.push_back(m_para.m_TDMfreq2);

    stdstr=m_cu.setConfigurationTDM(tdm);
    writeTcp(QString::fromStdString(stdstr).toUtf8());

}
void objui::setPower100c()
{
    std::string stdstr;
    struct PowerStruct pst;

    pst.powerAdjust = 1.0 * m_para.m_power100Central;

    stdstr=m_cu.setConfigurationPower(pst);
    writeTcp(QString::fromStdString(stdstr).toUtf8());
}

void objui::slotKey2s()
{
    m_bEnable1s = true;
}
void objui::slotKey1s()
{
    if(m_bEnable1s){
        emit sigStateTransition1s();
    }
}

void objui:: slotKeyEnable()
{
    if(0==m_semKey.available()) m_semKey.release();
}

void objui::slotInitTcp()
{
    m_pTcp->abort();
    m_pTcp->connectToHost("127.0.0.1",6790);
}
void objui::slotErrTCP()
{
    //writeLog(QByteArray("tcp error\n"));
    m_cu.reset();
    QTimer::singleShot(4000,this,SLOT(slotInitTcp()));
}
void objui::slotReadTCP()
{
    QByteArray ba=m_pTcp->readAll();
    //QByteArray ba1,ba2;
    //ba1=ba.mid(0,ba.size()>>1);
    //ba2=ba.mid(ba.size()>>1);
    //m_cu.recv(QString(ba1).toStdString());
    m_cu.recv(QString(ba).toStdString());
    //qDebug("%s",ba.data());
}

void objui::initMachine()
{
    if(m_pMachine->isRunning()) return;
    //m_pMachine = new QStateMachine;
    qDebug("   objui.initMachine()");

    m_pStateGroupTimeout = new QState(m_pMachine);

    m_pStateStatusPage1 = new QState(m_pMachine);
    m_pStateStatusPage2 = new QState(m_pStateGroupTimeout);
    m_pStateStatusPage3 = new QState(m_pStateGroupTimeout);

    m_pStateAbout = new QState(m_pStateGroupTimeout);
    m_pStateLogo = new QState(m_pMachine);

    m_pStateMsgSetBUCpwr = new QState(m_pMachine);
    m_pStateMsgSetBUC10m = new QState(m_pMachine);
    m_pStateMsgSetLNBpwr = new QState(m_pMachine);
    m_pStateMsgSetLNB10m = new QState(m_pMachine);

    m_pStateMsgZZHJ = new QState(m_pMachine);
    m_pStateMsgHJCG = new QState(m_pMachine);
    m_pStateMsgZZGD = new QState(m_pMachine);
    m_pStateMsgGDCG = new QState(m_pMachine);

    m_pStateMsgZZRW = new QState(m_pMachine);
    m_pStateMsgZZTW = new QState(m_pMachine);

    m_pStateMenuTestCtrl = new QState(m_pMachine);
    m_pStateMenuTestFreq = new QState(m_pStateGroupTimeout);//
    m_pStateMenuTestPwr = new QState(m_pStateGroupTimeout);//
    m_pStateEditorTestPwr = new QState(m_pStateGroupTimeout);//
    m_pStateEditorTestFreq = new QState(m_pStateGroupTimeout);//

    m_pStateBUCpwr = new QState(m_pStateGroupTimeout);//
    m_pStateBUC10m = new QState(m_pStateGroupTimeout);//
    m_pStateLNBpwr = new QState(m_pStateGroupTimeout);//
    m_pStateLNB10m = new QState(m_pStateGroupTimeout);//


    m_pStateMenuBUCpwr = new QState(m_pStateGroupTimeout);//
    m_pStateMenuBUC10m = new QState(m_pStateGroupTimeout);//
    m_pStateMenuBUCfreq = new QState(m_pStateGroupTimeout);//
    m_pStateMenuLNBpwr = new QState(m_pStateGroupTimeout);//
    m_pStateMenuLNB10m = new QState(m_pStateGroupTimeout);//
    m_pStateMenuLNBfreq = new QState(m_pStateGroupTimeout);//

    m_pStateMenuRadioSetting = new QState(m_pStateGroupTimeout);//
    m_pStateMenuCtrl = new QState(m_pStateGroupTimeout);//
    m_pStateMenuPara = new QState(m_pStateGroupTimeout);
    m_pStateMenuWorkMode = new QState(m_pStateGroupTimeout);

    m_pStateWorkMode1 = new QState(m_pStateGroupTimeout);
    m_pStateWorkMode2 = new QState(m_pStateGroupTimeout);
    m_pStateDevMode1 = new QState(m_pStateGroupTimeout);
    m_pStateDevMode2 = new QState(m_pStateGroupTimeout);

    m_pStateMenuCall = new QState(m_pStateGroupTimeout);// p2p and central

    m_pStateEditorBUCfreq = new QState(m_pStateGroupTimeout);
    m_pStateEditorLNBfreq = new QState(m_pStateGroupTimeout);

    m_pStateEditorTxFreq = new QState(m_pStateGroupTimeout);
    m_pStateEditorRxFreq = new QState(m_pStateGroupTimeout);
    m_pStateEditorTxRate = new QState(m_pStateGroupTimeout);
    m_pStateEditorRxRate = new QState(m_pStateGroupTimeout);
    m_pStateEditorTxRatePSK = new QState(m_pStateGroupTimeout);
    m_pStateEditorRxRatePSK = new QState(m_pStateGroupTimeout);
    m_pStateEditorPower = new QState(m_pStateGroupTimeout);

    m_pStateEditorTDM = new QState(m_pStateGroupTimeout);
    m_pStateEditorTDM2 = new QState(m_pStateGroupTimeout);
    m_pStateEditorTxRateC = new QState(m_pStateGroupTimeout);
    m_pStateEditorRxRateC = new QState(m_pStateGroupTimeout);
    m_pStateEditorPowerCentral = new QState(m_pStateGroupTimeout);

    //m_pStateParaPage1 = new QState(m_pStateGroupTimeout);
    //m_pStateParaPage11 = new QState(m_pStateGroupTimeout);
    //m_pStateParaPage12 = new QState(m_pStateGroupTimeout);
    //m_pStateParaPage13 = new QState(m_pStateGroupTimeout);
    m_pStateParaPage1a = new QState(m_pStateGroupTimeout);
    m_pStateParaPage11a = new QState(m_pStateGroupTimeout);
    m_pStateParaPage12a = new QState(m_pStateGroupTimeout);
    m_pStateParaPage13a = new QState(m_pStateGroupTimeout);

    m_pStateParaPage2 = new QState(m_pStateGroupTimeout);
    m_pStateParaPage21 = new QState(m_pStateGroupTimeout);
    //m_pStateParaPage22 = new QState(m_pStateGroupTimeout);
    //m_pStateParaPage23 = new QState(m_pStateGroupTimeout);

    //m_pStateParaPage30 = new QState(m_pStateGroupTimeout);
    //m_pStateParaPage31 = new QState(m_pStateGroupTimeout);
    //m_pStateParaPage32 = new QState(m_pStateGroupTimeout);
    //m_pStateParaPage40 = new QState(m_pStateGroupTimeout);
    //m_pStateParaPage41 = new QState(m_pStateGroupTimeout);
    //m_pStateParaPage42 = new QState(m_pStateGroupTimeout);

    m_pStateParaPage30a = new QState(m_pStateGroupTimeout);
    m_pStateParaPage31a = new QState(m_pStateGroupTimeout);
    m_pStateParaPage40a = new QState(m_pStateGroupTimeout);
    m_pStateParaPage41a = new QState(m_pStateGroupTimeout);

    m_pStateEditRxPSK = new QState(m_pStateGroupTimeout);
    m_pStateEditTxPSK = new QState(m_pStateGroupTimeout);

    m_pStateParaP11c = new QState(m_pStateGroupTimeout);
    m_pStateParaP12c = new QState(m_pStateGroupTimeout);
    m_pStateParaP13c = new QState(m_pStateGroupTimeout);
    m_pStateParaP14c = new QState(m_pStateGroupTimeout);
    m_pStateEditCallID = new QState(m_pStateGroupTimeout);

    // rm ==> m_pStateParaP11c , CP12c , ...
    //m_pStateParaPage1c = new QState(m_pStateGroupTimeout);
    //m_pStateParaPage11c = new QState(m_pStateGroupTimeout);
    //m_pStateParaPage12c = new QState(m_pStateGroupTimeout);
    //m_pStateParaPage13c = new QState(m_pStateGroupTimeout);
    m_pStateParaPage2c = new QState(m_pStateGroupTimeout);
    m_pStateParaPage21c = new QState(m_pStateGroupTimeout);
    m_pStateParaPage22c = new QState(m_pStateGroupTimeout);

    m_pStateParaP21c = new QState(m_pStateGroupTimeout);
    m_pStateParaP22c = new QState(m_pStateGroupTimeout);
    m_pStateParaP23c = new QState(m_pStateGroupTimeout);

    m_pStateGroupTimeout->setInitialState(m_pStateMenuCtrl);
    m_pMachine->setInitialState(m_pStateLogo);//m_pStateStatusPage1);

    connect(m_pStateStatusPage1,SIGNAL(entered()),this,SLOT(slotShowStatusPage1()));
    connect(m_pStateStatusPage2,SIGNAL(entered()),this,SLOT(slotShowStatusPage2()));
    connect(m_pStateStatusPage3,SIGNAL(entered()),this,SLOT(slotShowStatusPage3()));

    connect(m_pStateAbout,SIGNAL(entered()),this,SLOT(slotShowAbout()));
    connect(m_pStateLogo,SIGNAL(entered()),this,SLOT(slotShowLogo()));

    connect(m_pStateMenuTestCtrl,SIGNAL(entered()),this,SLOT(slotShowMenuTestCtrl()));
    connect(m_pStateMenuTestFreq,SIGNAL(entered()),this,SLOT(slotShowMenuTestFreq()));
    connect(m_pStateMenuTestPwr,SIGNAL(entered()),this,SLOT(slotShowMenuTestPwr()));
    connect(m_pStateEditorTestPwr,SIGNAL(entered()),this,SLOT(slotShowEditTestPwr()));
    connect(m_pStateEditorTestFreq,SIGNAL(entered()),this,SLOT(slotShowEditTestFreq()));


    connect(m_pStateMsgSetBUCpwr,SIGNAL(entered()),this,SLOT(slotShowMsgSetBUCpwr()));
    connect(m_pStateMsgSetBUC10m,SIGNAL(entered()),this,SLOT(slotShowMsgSetBUC10m()));
    connect(m_pStateMsgSetLNBpwr,SIGNAL(entered()),this,SLOT(slotShowMsgSetLNBpwr()));
    connect(m_pStateMsgSetLNB10m,SIGNAL(entered()),this,SLOT(slotShowMsgSetLNB10m()));

    connect(m_pStateMsgZZHJ,SIGNAL(entered()),this,SLOT(slotShowMsgZZHJ()));
    connect(m_pStateMsgHJCG,SIGNAL(entered()),this,SLOT(slotShowMsgHJCG()));
    connect(m_pStateMsgZZGD,SIGNAL(entered()),this,SLOT(slotShowMsgZZGD()));
    connect(m_pStateMsgGDCG,SIGNAL(entered()),this,SLOT(slotShowMsgGDCG()));

    connect(m_pStateMsgZZRW,SIGNAL(entered()),this,SLOT(slotShowMsgZZRW()));
    connect(m_pStateMsgZZTW,SIGNAL(entered()),this,SLOT(slotShowMsgZZTW()));

    connect(m_pStateBUCpwr,SIGNAL(entered()),this,SLOT(slotShow_BUCpwr()));
    connect(m_pStateBUC10m,SIGNAL(entered()),this,SLOT(slotShow_BUC10m()));
    connect(m_pStateLNBpwr,SIGNAL(entered()),this,SLOT(slotShow_LNBpwr()));
    connect(m_pStateLNB10m,SIGNAL(entered()),this,SLOT(slotShow_LNB10m()));

    connect(m_pStateMenuBUCpwr,SIGNAL(entered()),this,SLOT(slotShowBUCpwr()));
    connect(m_pStateMenuBUC10m,SIGNAL(entered()),this,SLOT(slotShowBUC10m()));
    connect(m_pStateMenuBUCfreq,SIGNAL(entered()),this,SLOT(slotShowBUCfreq()));
    connect(m_pStateMenuLNBpwr,SIGNAL(entered()),this,SLOT(slotShowLNBpwr()));
    connect(m_pStateMenuLNB10m,SIGNAL(entered()),this,SLOT(slotShowLNB10m()));
    connect(m_pStateMenuLNBfreq,SIGNAL(entered()),this,SLOT(slotShowLNBfreq()));

    connect(m_pStateMenuRadioSetting,SIGNAL(entered()),this,SLOT(slotShowMenu03()));
    connect(m_pStateMenuCtrl,SIGNAL(entered()),this,SLOT(slotShowMenu00()));
    connect(m_pStateMenuPara,SIGNAL(entered()),this,SLOT(slotShowMenu01()));
    connect(m_pStateMenuWorkMode,SIGNAL(entered()),this,SLOT(slotShowMenu02()));

    connect(m_pStateWorkMode1,SIGNAL(entered()),this,SLOT(slotShowMode1()));
    connect(m_pStateWorkMode2,SIGNAL(entered()),this,SLOT(slotShowMode2()));

    connect(m_pStateDevMode1,SIGNAL(entered()),this,SLOT(slotShowDevMode1()));
    connect(m_pStateDevMode2,SIGNAL(entered()),this,SLOT(slotShowDevMode2()));

    connect(m_pStateMenuCall,SIGNAL(entered()),this,SLOT(slotShowMenuCall()));

    //connect(m_pStateParaPage30,SIGNAL(entered()),this,SLOT(slotShowP30()));
    //connect(m_pStateParaPage31,SIGNAL(entered()),this,SLOT(slotShowP31()));
    //connect(m_pStateParaPage32,SIGNAL(entered()),this,SLOT(slotShowP32()));
    //connect(m_pStateParaPage40,SIGNAL(entered()),this,SLOT(slotShowP40()));
    //connect(m_pStateParaPage41,SIGNAL(entered()),this,SLOT(slotShowP41()));
    //connect(m_pStateParaPage42,SIGNAL(entered()),this,SLOT(slotShowP42()));

    connect(m_pStateParaPage30a,SIGNAL(entered()),this,SLOT(slotShowP30a()));
    connect(m_pStateParaPage31a,SIGNAL(entered()),this,SLOT(slotShowP31a()));
    connect(m_pStateParaPage40a,SIGNAL(entered()),this,SLOT(slotShowP40a()));
    connect(m_pStateParaPage41a,SIGNAL(entered()),this,SLOT(slotShowP41a()));

    connect(m_pStateEditRxPSK,SIGNAL(entered()),this,SLOT(slotShowEditRxPSK()));
    connect(m_pStateEditTxPSK,SIGNAL(entered()),this,SLOT(slotShowEditTxPSK()));


    //connect(m_pStateParaPage1,SIGNAL(entered()),this,SLOT(slotShowParaPage1()));
    //connect(m_pStateParaPage11,SIGNAL(entered()),this,SLOT(slotShowParaPage11()));
    //connect(m_pStateParaPage12,SIGNAL(entered()),this,SLOT(slotShowParaPage12()));
    //connect(m_pStateParaPage13,SIGNAL(entered()),this,SLOT(slotShowParaPage13()));
    connect(m_pStateParaPage1a,SIGNAL(entered()),this,SLOT(slotShowParaPage1a()));
    connect(m_pStateParaPage11a,SIGNAL(entered()),this,SLOT(slotShowParaPage11a()));
    connect(m_pStateParaPage12a,SIGNAL(entered()),this,SLOT(slotShowParaPage12a()));
    connect(m_pStateParaPage13a,SIGNAL(entered()),this,SLOT(slotShowParaPage13a()));


    connect(m_pStateParaPage2,SIGNAL(entered()),this,SLOT(slotShowParaPage2()));
    connect(m_pStateParaPage21,SIGNAL(entered()),this,SLOT(slotShowParaPage21()));
    //connect(m_pStateParaPage22,SIGNAL(entered()),this,SLOT(slotShowParaPage22()));
    //connect(m_pStateParaPage23,SIGNAL(entered()),this,SLOT(slotShowParaPage23()));

    connect(m_pStateParaP11c,SIGNAL(entered()),this,SLOT(slotShowParaP11c()));
    connect(m_pStateParaP12c,SIGNAL(entered()),this,SLOT(slotShowParaP12c()));
    connect(m_pStateParaP13c,SIGNAL(entered()),this,SLOT(slotShowParaP13c()));
    connect(m_pStateParaP14c,SIGNAL(entered()),this,SLOT(slotShowParaP14c()));
    connect(m_pStateEditCallID,SIGNAL(entered()),this,SLOT(slotShowEditIDstr()));


    connect(m_pStateParaP21c,SIGNAL(entered()),this,SLOT(slotShowParaP21c()));
    connect(m_pStateParaP22c,SIGNAL(entered()),this,SLOT(slotShowParaP22c()));
    connect(m_pStateParaP23c,SIGNAL(entered()),this,SLOT(slotShowParaP23c()));

    //connect(m_pStateParaPage1c,SIGNAL(entered()),this,SLOT(slotShowParaPage1c()));
    //connect(m_pStateParaPage11c,SIGNAL(entered()),this,SLOT(slotShowParaPage11c()));
    //connect(m_pStateParaPage12c,SIGNAL(entered()),this,SLOT(slotShowParaPage12c()));
    //connect(m_pStateParaPage13c,SIGNAL(entered()),this,SLOT(slotShowParaPage13c()));
    connect(m_pStateParaPage2c,SIGNAL(entered()),this,SLOT(slotShowParaPage2c()));
    connect(m_pStateParaPage21c,SIGNAL(entered()),this,SLOT(slotShowParaPage21c()));
    connect(m_pStateParaPage22c,SIGNAL(entered()),this,SLOT(slotShowParaPage22c()));

    connect(m_pStateEditorBUCfreq,SIGNAL(entered()),this,SLOT(slotShowEditBUCfreq()));
    connect(m_pStateEditorLNBfreq,SIGNAL(entered()),this,SLOT(slotShowEditLNBfreq()));

    connect(m_pStateEditorTxFreq,SIGNAL(entered()),this,SLOT(slotShowEditTxFreq()));
    connect(m_pStateEditorRxFreq,SIGNAL(entered()),this,SLOT(slotShowEditRxFreq()));
    connect(m_pStateEditorTxRate,SIGNAL(entered()),this,SLOT(slotShowEditTxRate()));
    connect(m_pStateEditorRxRate,SIGNAL(entered()),this,SLOT(slotShowEditRxRate()));
    connect(m_pStateEditorTxRatePSK,SIGNAL(entered()),this,SLOT(slotShowEditTxRatePSK()));
    connect(m_pStateEditorRxRatePSK,SIGNAL(entered()),this,SLOT(slotShowEditRxRatePSK()));
    connect(m_pStateEditorPower,SIGNAL(entered()),this,SLOT(slotShowEditPower()));

    connect(m_pStateEditorTDM,SIGNAL(entered()),this,SLOT(slotShowEditTDM()));
    connect(m_pStateEditorTDM2,SIGNAL(entered()),this,SLOT(slotShowEditTDM2()));
    connect(m_pStateEditorTxRateC,SIGNAL(entered()),this,SLOT(slotShowEditTxRateC()));
    connect(m_pStateEditorRxRateC,SIGNAL(entered()),this,SLOT(slotShowEditRxRateC()));
    connect(m_pStateEditorPowerCentral,SIGNAL(entered()),this,SLOT(slotShowEditPowerCentral()));

    m_pStateGroupTimeout->addTransition(this,SIGNAL(sigStateTransitionTimeout()),m_pStateStatusPage1);


    ketBUCpwr *pKetBUCpwr = new ketBUCpwr(this);
    m_pStateBUCpwr->addTransition(pKetBUCpwr);
    m_pStateBUCpwr->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateMenuBUCpwr);
    m_pStateBUCpwr->addTransition(this,SIGNAL(sigStateTransitionRight()),m_pStateBUCpwr);
    m_pStateBUCpwr->addTransition(this,SIGNAL(sigStateTransitionNext()),m_pStateMsgSetBUCpwr);
    m_pStateBUCpwr->addTransition(this,SIGNAL(sigStateTransitionRepaint()),m_pStateBUCpwr);
    ketBUC10m *pKetBUC10m = new ketBUC10m(this);
    m_pStateBUC10m->addTransition(pKetBUC10m);
    m_pStateBUC10m->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateMenuBUC10m);
    m_pStateBUC10m->addTransition(this,SIGNAL(sigStateTransitionRight()),m_pStateBUC10m);
    m_pStateBUC10m->addTransition(this,SIGNAL(sigStateTransitionNext()),m_pStateMsgSetBUC10m);
    m_pStateBUC10m->addTransition(this,SIGNAL(sigStateTransitionRepaint()),m_pStateBUC10m);
    ketLNBpwr *pKetLNBpwr = new ketLNBpwr(this);
    m_pStateLNBpwr->addTransition(pKetLNBpwr);
    m_pStateLNBpwr->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateMenuLNBpwr);
    m_pStateLNBpwr->addTransition(this,SIGNAL(sigStateTransitionRight()),m_pStateLNBpwr);
    m_pStateLNBpwr->addTransition(this,SIGNAL(sigStateTransitionNext()),m_pStateMsgSetLNBpwr);
    m_pStateLNBpwr->addTransition(this,SIGNAL(sigStateTransitionRepaint()),m_pStateLNBpwr);
    ketLNB10m *pKetLNB10m = new ketLNB10m(this);
    m_pStateLNB10m->addTransition(pKetLNB10m);
    m_pStateLNB10m->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateMenuLNB10m);
    m_pStateLNB10m->addTransition(this,SIGNAL(sigStateTransitionRight()),m_pStateLNB10m);
    m_pStateLNB10m->addTransition(this,SIGNAL(sigStateTransitionNext()),m_pStateMsgSetLNB10m);
    m_pStateLNB10m->addTransition(this,SIGNAL(sigStateTransitionRepaint()),m_pStateLNB10m);

    ketStatus1 *pKetStatus1 = new ketStatus1(this);
    m_pStateStatusPage1->addTransition(pKetStatus1);
    m_pStateStatusPage1->addTransition(this,SIGNAL(sigStateTransitionDown()),m_pStateStatusPage2);
    m_pStateStatusPage1->addTransition(this,SIGNAL(sigStateTransitionNext()),m_pStateMenuCtrl);
    m_pStateStatusPage1->addTransition(this,SIGNAL(sigStateTransitionUp()),m_pStateStatusPage3);
    m_pStateStatusPage1->addTransition(this,SIGNAL(sigStateTransition1s()),m_pStateStatusPage1);
    ketStatus2 *pKetStatus2 = new ketStatus2(this);
    m_pStateStatusPage2->addTransition(pKetStatus2);
    m_pStateStatusPage2->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateStatusPage1);
    m_pStateStatusPage2->addTransition(this,SIGNAL(sigStateTransitionDown()),m_pStateStatusPage3);
    m_pStateStatusPage2->addTransition(this,SIGNAL(sigStateTransitionNext()),m_pStateMenuCtrl);
    m_pStateStatusPage2->addTransition(this,SIGNAL(sigStateTransitionRepaint()),m_pStateStatusPage2);
    ketStatus3 *pKetStatus3 = new ketStatus3(this);
    m_pStateStatusPage3->addTransition(pKetStatus3);
    m_pStateStatusPage3->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateStatusPage1);
    m_pStateStatusPage3->addTransition(this,SIGNAL(sigStateTransitionUp()),m_pStateStatusPage2);
    m_pStateStatusPage3->addTransition(this,SIGNAL(sigStateTransitionNext()),m_pStateMenuCtrl);
    m_pStateStatusPage3->addTransition(this,SIGNAL(sigStateTransitionRepaint()),m_pStateStatusPage3);

    ketAbout *pKetAbout = new ketAbout(this);
    m_pStateAbout->addTransition(pKetAbout);
    m_pStateAbout->addTransition(this,SIGNAL(sigStateTransitionNext()),m_pStateMenuCtrl);

    m_pStateMsgSetBUCpwr->addTransition(this,SIGNAL(sigStateTransitionNext()),m_pStateBUCpwr);
    m_pStateMsgSetBUC10m->addTransition(this,SIGNAL(sigStateTransitionNext()),m_pStateBUC10m);
    m_pStateMsgSetLNBpwr->addTransition(this,SIGNAL(sigStateTransitionNext()),m_pStateLNBpwr);
    m_pStateMsgSetLNB10m->addTransition(this,SIGNAL(sigStateTransitionNext()),m_pStateLNB10m);


    ketBackspace *pKetMsgZZHJ = new ketBackspace(this);
    m_pStateMsgZZHJ->addTransition(pKetMsgZZHJ);
    m_pStateMsgZZHJ->addTransition(this,SIGNAL(sigEVzzhjPaint()),m_pStateMsgZZHJ);
    m_pStateMsgZZHJ->addTransition(this,SIGNAL(sigEVzzhjBackM()),m_pStateStatusPage1);
    m_pStateMsgZZHJ->addTransition(this,SIGNAL(sigEVzzhjBack()),m_pStateMenuCall);

    m_pStateMsgHJCG->addTransition(this,SIGNAL(sigStateTransitionNext()),m_pStateStatusPage1);
    m_pStateMsgZZGD->addTransition(this,SIGNAL(sigStateTransitionNext()),m_pStateMsgGDCG);
    m_pStateMsgGDCG->addTransition(this,SIGNAL(sigStateTransitionNext()),m_pStateStatusPage1);

    m_pStateMsgZZRW->addTransition(this,SIGNAL(sigStateTransitionNext()),m_pStateStatusPage1);
    m_pStateMsgZZTW->addTransition(this,SIGNAL(sigStateTransitionNext()),m_pStateStatusPage1);

    m_pStateLogo->addTransition(this,SIGNAL(sigStateTransitionNext()),m_pStateStatusPage1);


    ketMenuCtrl *pKetMenuCtrl = new ketMenuCtrl(this);
    m_pStateMenuCtrl->addTransition(pKetMenuCtrl);
    m_pStateMenuCtrl->addTransition(this,SIGNAL(sigStateTransitionDown()),m_pStateMenuPara);
    m_pStateMenuCtrl->addTransition(this,SIGNAL(sigStateTransitionUp()),m_pStateMenuRadioSetting);
    m_pStateMenuCtrl->addTransition(this,SIGNAL(sigStateTransitionEnter()),m_pStateMenuCall);
    m_pStateMenuCtrl->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateStatusPage1);
    ketMenuPara *pKetMenuPara = new ketMenuPara(this);
    m_pStateMenuPara->addTransition(pKetMenuPara);
    m_pStateMenuPara->addTransition(this,SIGNAL(sigStateTransitionDown()),m_pStateMenuWorkMode);
    m_pStateMenuPara->addTransition(this,SIGNAL(sigStateTransitionUp()),m_pStateMenuCtrl);
    m_pStateMenuPara->addTransition(this,SIGNAL(sigStateTransitionP2P()),m_pStateParaPage1a);
    m_pStateMenuPara->addTransition(this,SIGNAL(sigStateTransitionCentral()),m_pStateParaP11c);
    //m_pStateMenuPara->addTransition(this,SIGNAL(sigStateTransitionCentral()),m_pStateParaPage1c);
    m_pStateMenuPara->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateStatusPage1);
#if 0
    ketParaPage1 *pKetParaPage1 = new ketParaPage1(this);
    m_pStateParaPage1->addTransition(pKetParaPage1);
    m_pStateParaPage1->addTransition(this,SIGNAL(sigStateTransitionDown()),m_pStateParaPage11);
    m_pStateParaPage1->addTransition(this,SIGNAL(sigStateTransitionUp()),m_pStateParaPage23);
    m_pStateParaPage1->addTransition(this,SIGNAL(sigStateTransitionLeft()),m_pStateParaPage30);
    m_pStateParaPage1->addTransition(this,SIGNAL(sigStateTransitionRight()),m_pStateParaPage30);
    m_pStateParaPage1->addTransition(this,SIGNAL(sigStateTransitionNext()),m_pStateEditorTxFreq);
    m_pStateParaPage1->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateMenuPara);

    ketParaPage1 *pKetParaPage11 = new ketParaPage1(this);
    m_pStateParaPage11->addTransition(pKetParaPage11);
    m_pStateParaPage11->addTransition(this,SIGNAL(sigStateTransitionDown()),m_pStateParaPage30);
    m_pStateParaPage11->addTransition(this,SIGNAL(sigStateTransitionUp()),m_pStateParaPage1);
    m_pStateParaPage11->addTransition(this,SIGNAL(sigStateTransitionLeft()),m_pStateParaPage30);
    m_pStateParaPage11->addTransition(this,SIGNAL(sigStateTransitionRight()),m_pStateParaPage30);
    m_pStateParaPage11->addTransition(this,SIGNAL(sigStateTransitionNext()),m_pStateEditorRxFreq);
    m_pStateParaPage11->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateMenuPara);
#endif
    ketParaPage1 *pKetParaPage1a = new ketParaPage1(this);
    m_pStateParaPage1a->addTransition(pKetParaPage1a);
    m_pStateParaPage1a->addTransition(this,SIGNAL(sigStateTransitionDown()),m_pStateParaPage11a);
    m_pStateParaPage1a->addTransition(this,SIGNAL(sigStateTransitionUp()),m_pStateParaPage21);
    m_pStateParaPage1a->addTransition(this,SIGNAL(sigStateTransitionLeft()),m_pStateParaPage2);
    m_pStateParaPage1a->addTransition(this,SIGNAL(sigStateTransitionRight()),m_pStateParaPage30a);
    m_pStateParaPage1a->addTransition(this,SIGNAL(sigStateTransitionNext()),m_pStateEditorTxFreq);
    m_pStateParaPage1a->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateMenuPara);

    ketParaPage1 *pKetParaPage11a = new ketParaPage1(this);
    m_pStateParaPage11a->addTransition(pKetParaPage11a);
    m_pStateParaPage11a->addTransition(this,SIGNAL(sigStateTransitionDown()),m_pStateParaPage30a);
    m_pStateParaPage11a->addTransition(this,SIGNAL(sigStateTransitionUp()),m_pStateParaPage1a);
    m_pStateParaPage11a->addTransition(this,SIGNAL(sigStateTransitionLeft()),m_pStateParaPage2);
    m_pStateParaPage11a->addTransition(this,SIGNAL(sigStateTransitionRight()),m_pStateParaPage30a);
    m_pStateParaPage11a->addTransition(this,SIGNAL(sigStateTransitionNext()),m_pStateEditorRxFreq);
    m_pStateParaPage11a->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateMenuPara);

    ketParaPage1 *pKetParaPage12a = new ketParaPage1(this);
    m_pStateParaPage12a->addTransition(pKetParaPage12a);
    m_pStateParaPage12a->addTransition(this,SIGNAL(sigStateTransitionDown()),m_pStateParaPage13a);
    m_pStateParaPage12a->addTransition(this,SIGNAL(sigStateTransitionUp()),m_pStateParaPage11a);
    m_pStateParaPage12a->addTransition(this,SIGNAL(sigStateTransitionLeft()),m_pStateParaPage2);
    m_pStateParaPage12a->addTransition(this,SIGNAL(sigStateTransitionRight()),m_pStateParaPage30a);
    m_pStateParaPage12a->addTransition(this,SIGNAL(sigStateTransitionNext()),m_pStateEditorBUCfreq);
    m_pStateParaPage12a->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateMenuPara);

    ketParaPage1 *pKetParaPage13a = new ketParaPage1(this);
    m_pStateParaPage13a->addTransition(pKetParaPage13a);
    m_pStateParaPage13a->addTransition(this,SIGNAL(sigStateTransitionDown()),m_pStateParaPage30a);
    m_pStateParaPage13a->addTransition(this,SIGNAL(sigStateTransitionUp()),m_pStateParaPage12a);
    m_pStateParaPage13a->addTransition(this,SIGNAL(sigStateTransitionLeft()),m_pStateParaPage2);
    m_pStateParaPage13a->addTransition(this,SIGNAL(sigStateTransitionRight()),m_pStateParaPage30a);
    m_pStateParaPage13a->addTransition(this,SIGNAL(sigStateTransitionNext()),m_pStateEditorLNBfreq);
    m_pStateParaPage13a->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateMenuPara);

    ketRxPSKeditor *pKetEditRxPSK = new ketRxPSKeditor(this);
    m_pStateEditRxPSK->addTransition(pKetEditRxPSK);
    m_pStateEditRxPSK->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateParaPage30a);
    ketTxPSKeditor *pKetEditTxPSK = new ketTxPSKeditor(this);
    m_pStateEditTxPSK->addTransition(pKetEditTxPSK);
    m_pStateEditTxPSK->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateParaPage40a);

    ketParaPage1 *pKetParaPage30a = new ketParaPage1(this);
    m_pStateParaPage30a->addTransition(pKetParaPage30a);
    m_pStateParaPage30a->addTransition(this,SIGNAL(sigStateTransitionDown()),m_pStateParaPage31a);
    m_pStateParaPage30a->addTransition(this,SIGNAL(sigStateTransitionUp()),m_pStateParaPage11a);
    m_pStateParaPage30a->addTransition(this,SIGNAL(sigStateTransitionLeft()),m_pStateParaPage1a);
    m_pStateParaPage30a->addTransition(this,SIGNAL(sigStateTransitionRight()),m_pStateParaPage2);
    m_pStateParaPage30a->addTransition(this,SIGNAL(sigStateTransitionNext()),m_pStateEditRxPSK);
    m_pStateParaPage30a->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateMenuPara);

    ketParaPage1 *pKetParaPage31a = new ketParaPage1(this);
    m_pStateParaPage31a->addTransition(pKetParaPage31a);
    m_pStateParaPage31a->addTransition(this,SIGNAL(sigStateTransitionDown()),m_pStateParaPage40a);
    m_pStateParaPage31a->addTransition(this,SIGNAL(sigStateTransitionUp()),m_pStateParaPage30a);
    m_pStateParaPage31a->addTransition(this,SIGNAL(sigStateTransitionLeft()),m_pStateParaPage1a);
    m_pStateParaPage31a->addTransition(this,SIGNAL(sigStateTransitionRight()),m_pStateParaPage2);
    m_pStateParaPage31a->addTransition(this,SIGNAL(sigStateTransitionNext()),m_pStateEditorRxRatePSK);
    m_pStateParaPage31a->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateMenuPara);
#if 0
    ketParaPage1 *pKetParaPage32 = new ketParaPage1(this);
    m_pStateParaPage32->addTransition(pKetParaPage32);
    m_pStateParaPage32->addTransition(this,SIGNAL(sigStateTransitionDown()),m_pStateParaPage40);
    m_pStateParaPage32->addTransition(this,SIGNAL(sigStateTransitionUp()),m_pStateParaPage31);
    m_pStateParaPage32->addTransition(this,SIGNAL(sigStateTransitionLeft()),m_pStateParaPage40);
    m_pStateParaPage32->addTransition(this,SIGNAL(sigStateTransitionRight()),m_pStateParaPage40);
    m_pStateParaPage32->addTransition(this,SIGNAL(sigStateTransitionNext()),m_pStateParaPage32);
    m_pStateParaPage32->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateMenuPara);
#endif
    ketParaPage1 *pKetParaPage40a = new ketParaPage1(this);
    m_pStateParaPage40a->addTransition(pKetParaPage40a);
    m_pStateParaPage40a->addTransition(this,SIGNAL(sigStateTransitionDown()),m_pStateParaPage41a);
    m_pStateParaPage40a->addTransition(this,SIGNAL(sigStateTransitionUp()),m_pStateParaPage31a);
    m_pStateParaPage40a->addTransition(this,SIGNAL(sigStateTransitionLeft()),m_pStateParaPage1a);
    m_pStateParaPage40a->addTransition(this,SIGNAL(sigStateTransitionRight()),m_pStateParaPage2);
    m_pStateParaPage40a->addTransition(this,SIGNAL(sigStateTransitionNext()),m_pStateEditTxPSK);
    m_pStateParaPage40a->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateMenuPara);

    ketParaPage1 *pKetParaPage41a = new ketParaPage1(this);
    m_pStateParaPage41a->addTransition(pKetParaPage41a);
    m_pStateParaPage41a->addTransition(this,SIGNAL(sigStateTransitionDown()),m_pStateParaPage2);
    m_pStateParaPage41a->addTransition(this,SIGNAL(sigStateTransitionUp()),m_pStateParaPage40a);
    m_pStateParaPage41a->addTransition(this,SIGNAL(sigStateTransitionLeft()),m_pStateParaPage1a);
    m_pStateParaPage41a->addTransition(this,SIGNAL(sigStateTransitionRight()),m_pStateParaPage2);
    m_pStateParaPage41a->addTransition(this,SIGNAL(sigStateTransitionNext()),m_pStateEditorTxRatePSK);
    m_pStateParaPage41a->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateMenuPara);
#if 0
    ketParaPage1 *pKetParaPage42 = new ketParaPage1(this);
    m_pStateParaPage42->addTransition(pKetParaPage42);
    m_pStateParaPage42->addTransition(this,SIGNAL(sigStateTransitionDown()),m_pStateParaPage2);
    m_pStateParaPage42->addTransition(this,SIGNAL(sigStateTransitionUp()),m_pStateParaPage41);
    m_pStateParaPage42->addTransition(this,SIGNAL(sigStateTransitionLeft()),m_pStateParaPage2);
    m_pStateParaPage42->addTransition(this,SIGNAL(sigStateTransitionRight()),m_pStateParaPage2);
    m_pStateParaPage42->addTransition(this,SIGNAL(sigStateTransitionNext()),m_pStateParaPage42);
    m_pStateParaPage42->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateMenuPara);
#endif
#if 0
    ketParaPage1 *pKetParaPage12 = new ketParaPage1(this);
    m_pStateParaPage12->addTransition(pKetParaPage12);
    m_pStateParaPage12->addTransition(this,SIGNAL(sigStateTransitionDown()),m_pStateParaPage13);
    m_pStateParaPage12->addTransition(this,SIGNAL(sigStateTransitionUp()),m_pStateParaPage11);
    m_pStateParaPage12->addTransition(this,SIGNAL(sigStateTransitionLeft()),m_pStateParaPage2);
    m_pStateParaPage12->addTransition(this,SIGNAL(sigStateTransitionRight()),m_pStateParaPage2);
    m_pStateParaPage12->addTransition(this,SIGNAL(sigStateTransitionNext()),m_pStateEditorTxRate);
    m_pStateParaPage12->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateMenuPara);
#endif
#if 0
    ketParaPage1 *pKetParaPage13 = new ketParaPage1(this);
    m_pStateParaPage13->addTransition(pKetParaPage13);
    m_pStateParaPage13->addTransition(this,SIGNAL(sigStateTransitionDown()),m_pStateParaPage2);
    m_pStateParaPage13->addTransition(this,SIGNAL(sigStateTransitionUp()),m_pStateParaPage12);
    m_pStateParaPage13->addTransition(this,SIGNAL(sigStateTransitionLeft()),m_pStateParaPage2);
    m_pStateParaPage13->addTransition(this,SIGNAL(sigStateTransitionRight()),m_pStateParaPage2);
    m_pStateParaPage13->addTransition(this,SIGNAL(sigStateTransitionNext()),m_pStateEditorRxRate);
    m_pStateParaPage13->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateMenuPara);
#endif
    ketParaPage1 *pKetParaPage2 = new ketParaPage1(this);
    m_pStateParaPage2->addTransition(pKetParaPage2);
    m_pStateParaPage2->addTransition(this,SIGNAL(sigStateTransitionDown()),m_pStateParaPage21);
    m_pStateParaPage2->addTransition(this,SIGNAL(sigStateTransitionUp()),m_pStateParaPage41a);
    m_pStateParaPage2->addTransition(this,SIGNAL(sigStateTransitionLeft()),m_pStateParaPage30a);
    m_pStateParaPage2->addTransition(this,SIGNAL(sigStateTransitionRight()),m_pStateParaPage1a);
    m_pStateParaPage2->addTransition(this,SIGNAL(sigStateTransitionNext()),m_pStateEditorPower);
    m_pStateParaPage2->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateMenuPara);

    ketParaPage1 *pKetParaPage21 = new ketParaPage1(this);
    m_pStateParaPage21->addTransition(pKetParaPage21);
    m_pStateParaPage21->addTransition(this,SIGNAL(sigStateTransitionDown()),m_pStateParaPage1a);
    m_pStateParaPage21->addTransition(this,SIGNAL(sigStateTransitionUp()),m_pStateParaPage2);
    m_pStateParaPage21->addTransition(this,SIGNAL(sigStateTransitionLeft()),m_pStateParaPage30a);
    m_pStateParaPage21->addTransition(this,SIGNAL(sigStateTransitionRight()),m_pStateParaPage1a);
    m_pStateParaPage21->addTransition(this,SIGNAL(sigStateTransitionNext()),m_pStateDevMode1);
    m_pStateParaPage21->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateMenuPara);
#if 0
    ketParaPage1 *pKetParaPage22 = new ketParaPage1(this);
    m_pStateParaPage22->addTransition(pKetParaPage22);
    m_pStateParaPage22->addTransition(this,SIGNAL(sigStateTransitionDown()),m_pStateParaPage23);
    m_pStateParaPage22->addTransition(this,SIGNAL(sigStateTransitionUp()),m_pStateParaPage21);
    m_pStateParaPage22->addTransition(this,SIGNAL(sigStateTransitionLeft()),m_pStateParaPage41);
    m_pStateParaPage22->addTransition(this,SIGNAL(sigStateTransitionRight()),m_pStateParaPage1a);
    m_pStateParaPage22->addTransition(this,SIGNAL(sigStateTransitionNext()),m_pStateEditorBUCfreq);
    m_pStateParaPage22->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateMenuPara);

    ketParaPage1 *pKetParaPage23 = new ketParaPage1(this);
    m_pStateParaPage23->addTransition(pKetParaPage23);
    m_pStateParaPage23->addTransition(this,SIGNAL(sigStateTransitionDown()),m_pStateParaPage1a);
    m_pStateParaPage23->addTransition(this,SIGNAL(sigStateTransitionUp()),m_pStateParaPage22);
    m_pStateParaPage23->addTransition(this,SIGNAL(sigStateTransitionLeft()),m_pStateParaPage41);
    m_pStateParaPage23->addTransition(this,SIGNAL(sigStateTransitionRight()),m_pStateParaPage1a);
    m_pStateParaPage23->addTransition(this,SIGNAL(sigStateTransitionNext()),m_pStateEditorLNBfreq);
    m_pStateParaPage23->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateMenuPara);
#endif
    ketParaPage1 *pKetParaP11c = new ketParaPage1(this);
    m_pStateParaP11c->addTransition(pKetParaP11c);
    m_pStateParaP11c->addTransition(this,SIGNAL(sigStateTransitionDown()),m_pStateParaP12c);
    m_pStateParaP11c->addTransition(this,SIGNAL(sigStateTransitionUp()),m_pStateParaP23c);
    m_pStateParaP11c->addTransition(this,SIGNAL(sigStateTransitionLeft()),m_pStateParaP21c);
    m_pStateParaP11c->addTransition(this,SIGNAL(sigStateTransitionRight()),m_pStateParaP21c);
    m_pStateParaP11c->addTransition(this,SIGNAL(sigStateTransitionNext()),m_pStateEditorTDM);
    m_pStateParaP11c->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateMenuPara);

    ketParaPage1 *pKetParaP12c = new ketParaPage1(this);
    m_pStateParaP12c->addTransition(pKetParaP12c);
    m_pStateParaP12c->addTransition(this,SIGNAL(sigStateTransitionDown()),m_pStateParaP13c);
    m_pStateParaP12c->addTransition(this,SIGNAL(sigStateTransitionUp()),m_pStateParaP11c);
    m_pStateParaP12c->addTransition(this,SIGNAL(sigStateTransitionLeft()),m_pStateParaP21c);
    m_pStateParaP12c->addTransition(this,SIGNAL(sigStateTransitionRight()),m_pStateParaP21c);
    m_pStateParaP12c->addTransition(this,SIGNAL(sigStateTransitionNext()),m_pStateEditorTDM2);
    m_pStateParaP12c->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateMenuPara);

    ketParaPage1 *pKetParaP13c = new ketParaPage1(this);
    m_pStateParaP13c->addTransition(pKetParaP13c);
    m_pStateParaP13c->addTransition(this,SIGNAL(sigStateTransitionDown()),m_pStateParaP14c);
    m_pStateParaP13c->addTransition(this,SIGNAL(sigStateTransitionUp()),m_pStateParaP12c);
    m_pStateParaP13c->addTransition(this,SIGNAL(sigStateTransitionLeft()),m_pStateParaP21c);
    m_pStateParaP13c->addTransition(this,SIGNAL(sigStateTransitionRight()),m_pStateParaP21c);
    m_pStateParaP13c->addTransition(this,SIGNAL(sigStateTransitionNext()),m_pStateEditorPowerCentral);
    m_pStateParaP13c->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateMenuPara);

    ketParaPage1 *pKetParaP14c = new ketParaPage1(this);
    m_pStateParaP14c->addTransition(pKetParaP14c);
    m_pStateParaP14c->addTransition(this,SIGNAL(sigStateTransitionDown()),m_pStateParaP21c);
    m_pStateParaP14c->addTransition(this,SIGNAL(sigStateTransitionUp()),m_pStateParaP13c);
    m_pStateParaP14c->addTransition(this,SIGNAL(sigStateTransitionLeft()),m_pStateParaP21c);
    m_pStateParaP14c->addTransition(this,SIGNAL(sigStateTransitionRight()),m_pStateParaP21c);
    m_pStateParaP14c->addTransition(this,SIGNAL(sigStateTransitionNext()),m_pStateEditCallID);
    m_pStateParaP14c->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateMenuPara);

    ketEditorCallID *pKetEditorCallID = new ketEditorCallID(this);
    m_pStateEditCallID->addTransition(pKetEditorCallID);
    m_pStateEditCallID->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateParaP14c);

    ketParaPage1 *pKetParaP21c = new ketParaPage1(this);
    m_pStateParaP21c->addTransition(pKetParaP21c);
    m_pStateParaP21c->addTransition(this,SIGNAL(sigStateTransitionDown()),m_pStateParaP22c);
    m_pStateParaP21c->addTransition(this,SIGNAL(sigStateTransitionUp()),m_pStateParaP14c);
    m_pStateParaP21c->addTransition(this,SIGNAL(sigStateTransitionLeft()),m_pStateParaP11c);
    m_pStateParaP21c->addTransition(this,SIGNAL(sigStateTransitionRight()),m_pStateParaP11c);
    m_pStateParaP21c->addTransition(this,SIGNAL(sigStateTransitionNext()),m_pStateEditorTxRateC);
    m_pStateParaP21c->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateMenuPara);

    ketParaPage1 *pKetParaP22c = new ketParaPage1(this);
    m_pStateParaP22c->addTransition(pKetParaP22c);
    m_pStateParaP22c->addTransition(this,SIGNAL(sigStateTransitionDown()),m_pStateParaP23c);
    m_pStateParaP22c->addTransition(this,SIGNAL(sigStateTransitionUp()),m_pStateParaP21c);
    m_pStateParaP22c->addTransition(this,SIGNAL(sigStateTransitionLeft()),m_pStateParaP11c);
    m_pStateParaP22c->addTransition(this,SIGNAL(sigStateTransitionRight()),m_pStateParaP11c);
    m_pStateParaP22c->addTransition(this,SIGNAL(sigStateTransitionNext()),m_pStateEditorRxRateC);
    m_pStateParaP22c->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateMenuPara);

    ketParaPage1 *pKetParaP23c = new ketParaPage1(this);
    m_pStateParaP23c->addTransition(pKetParaP23c);
    m_pStateParaP23c->addTransition(this,SIGNAL(sigStateTransitionDown()),m_pStateParaP11c);
    m_pStateParaP23c->addTransition(this,SIGNAL(sigStateTransitionUp()),m_pStateParaP22c);
    m_pStateParaP23c->addTransition(this,SIGNAL(sigStateTransitionLeft()),m_pStateParaP11c);
    m_pStateParaP23c->addTransition(this,SIGNAL(sigStateTransitionRight()),m_pStateParaP11c);
    m_pStateParaP23c->addTransition(this,SIGNAL(sigStateTransitionNext()),m_pStateDevMode1);
    m_pStateParaP23c->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateMenuPara);




#if 0
    ketParaPage1 *pKetParaPage1c = new ketParaPage1(this);
    m_pStateParaPage1c->addTransition(pKetParaPage1c);
    m_pStateParaPage1c->addTransition(this,SIGNAL(sigStateTransitionDown()),m_pStateParaPage11c);
    m_pStateParaPage1c->addTransition(this,SIGNAL(sigStateTransitionUp()),m_pStateParaPage12c);
    m_pStateParaPage1c->addTransition(this,SIGNAL(sigStateTransitionLeft()),m_pStateParaPage12c);
    m_pStateParaPage1c->addTransition(this,SIGNAL(sigStateTransitionRight()),m_pStateParaPage11c);
    m_pStateParaPage1c->addTransition(this,SIGNAL(sigStateTransitionNext()),m_pStateEditorTDM);
    m_pStateParaPage1c->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateMenuPara);

    ketParaPage1 *pKetParaPage11c = new ketParaPage1(this);
    m_pStateParaPage11c->addTransition(pKetParaPage11c);
    m_pStateParaPage11c->addTransition(this,SIGNAL(sigStateTransitionDown()),m_pStateParaPage12c);
    m_pStateParaPage11c->addTransition(this,SIGNAL(sigStateTransitionUp()),m_pStateParaPage1c);
    m_pStateParaPage11c->addTransition(this,SIGNAL(sigStateTransitionLeft()),m_pStateParaPage1c);
    m_pStateParaPage11c->addTransition(this,SIGNAL(sigStateTransitionRight()),m_pStateParaPage12c);
    m_pStateParaPage11c->addTransition(this,SIGNAL(sigStateTransitionNext()),m_pStateEditorTDM2);
    m_pStateParaPage11c->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateMenuPara);

    ketParaPage1 *pKetParaPage12c = new ketParaPage1(this);
    m_pStateParaPage12c->addTransition(pKetParaPage12c);
    m_pStateParaPage12c->addTransition(this,SIGNAL(sigStateTransitionDown()),m_pStateParaPage1c);
    m_pStateParaPage12c->addTransition(this,SIGNAL(sigStateTransitionUp()),m_pStateParaPage11c);
    m_pStateParaPage12c->addTransition(this,SIGNAL(sigStateTransitionLeft()),m_pStateParaPage11c);
    m_pStateParaPage12c->addTransition(this,SIGNAL(sigStateTransitionRight()),m_pStateParaPage1c);
    m_pStateParaPage12c->addTransition(this,SIGNAL(sigStateTransitionNext()),m_pStateEditorPowerCentral);
    m_pStateParaPage12c->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateMenuPara);

    ketParaPage1 *pKetParaPage13c = new ketParaPage1(this);
    m_pStateParaPage13c->addTransition(pKetParaPage13c);
    m_pStateParaPage13c->addTransition(this,SIGNAL(sigStateTransitionDown()),m_pStateParaPage2c);
    m_pStateParaPage13c->addTransition(this,SIGNAL(sigStateTransitionUp()),m_pStateParaPage12c);
    m_pStateParaPage13c->addTransition(this,SIGNAL(sigStateTransitionLeft()),m_pStateParaPage2c);
    m_pStateParaPage13c->addTransition(this,SIGNAL(sigStateTransitionRight()),m_pStateParaPage2c);
    m_pStateParaPage13c->addTransition(this,SIGNAL(sigStateTransitionNext()),m_pStateEditorRxRateCentral);
    m_pStateParaPage13c->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateMenuPara);
#endif
#if 0
    ketParaPage1 *pKetParaPage2c = new ketParaPage1(this);
    m_pStateParaPage2c->addTransition(pKetParaPage2c);
    m_pStateParaPage2c->addTransition(this,SIGNAL(sigStateTransitionDown()),m_pStateParaPage21c);
    m_pStateParaPage2c->addTransition(this,SIGNAL(sigStateTransitionUp()),m_pStateParaPage13c);
    m_pStateParaPage2c->addTransition(this,SIGNAL(sigStateTransitionLeft()),m_pStateParaPage1c);
    m_pStateParaPage2c->addTransition(this,SIGNAL(sigStateTransitionRight()),m_pStateParaPage1c);
    m_pStateParaPage2c->addTransition(this,SIGNAL(sigStateTransitionNext()),m_pStateEditorPowerCentral);
    m_pStateParaPage2c->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateMenuPara);

    ketParaPage1 *pKetParaPage21c = new ketParaPage1(this);
    m_pStateParaPage21c->addTransition(pKetParaPage21c);
    m_pStateParaPage21c->addTransition(this,SIGNAL(sigStateTransitionDown()),m_pStateParaPage2c);
    m_pStateParaPage21c->addTransition(this,SIGNAL(sigStateTransitionUp()),m_pStateParaPage2c);
    m_pStateParaPage21c->addTransition(this,SIGNAL(sigStateTransitionLeft()),m_pStateParaPage1c);
    m_pStateParaPage21c->addTransition(this,SIGNAL(sigStateTransitionRight()),m_pStateParaPage1c);
    m_pStateParaPage21c->addTransition(this,SIGNAL(sigStateTransitionNext()),m_pStateDevMode1);
    m_pStateParaPage21c->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateMenuPara);

    ketParaPage1 *pKetParaPage22c = new ketParaPage1(this);
    m_pStateParaPage22c->addTransition(pKetParaPage22c);
    m_pStateParaPage22c->addTransition(this,SIGNAL(sigStateTransitionDown()),m_pStateParaPage1c);
    m_pStateParaPage22c->addTransition(this,SIGNAL(sigStateTransitionUp()),m_pStateParaPage21c);
    m_pStateParaPage22c->addTransition(this,SIGNAL(sigStateTransitionLeft()),m_pStateParaPage1c);
    m_pStateParaPage22c->addTransition(this,SIGNAL(sigStateTransitionRight()),m_pStateParaPage1c);
    m_pStateParaPage22c->addTransition(this,SIGNAL(sigStateTransitionNext()),m_pStateDevMode1);
    m_pStateParaPage22c->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateMenuPara);
#endif

    ketBUCfreqEditor *pKetBUCfreqEditor = new ketBUCfreqEditor(this);
    m_pStateEditorBUCfreq->addTransition(pKetBUCfreqEditor);
    m_pStateEditorBUCfreq->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateMenuBUCfreq);
    ketLNBfreqEditor *pKetLNBfreqEditor = new ketLNBfreqEditor(this);
    m_pStateEditorLNBfreq->addTransition(pKetLNBfreqEditor);
    m_pStateEditorLNBfreq->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateMenuLNBfreq);


    ketTestFreqEditor *pKetTestFreqEditor = new ketTestFreqEditor(this);
    m_pStateEditorTestFreq->addTransition(pKetTestFreqEditor);
    m_pStateEditorTestFreq->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateMenuTestFreq);

    ketTxFreqEditor *pKetTxFreqEditor = new ketTxFreqEditor(this);
    m_pStateEditorTxFreq->addTransition(pKetTxFreqEditor);
    m_pStateEditorTxFreq->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateParaPage1a);
    ketRxFreqEditor *pKetRxFreqEditor = new ketRxFreqEditor(this);
    m_pStateEditorRxFreq->addTransition(pKetRxFreqEditor);
    m_pStateEditorRxFreq->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateParaPage11a);
#if 1
    ketTxRateEditorPSK *pKetTxRateEditor = new ketTxRateEditorPSK(this);
    m_pStateEditorTxRatePSK->addTransition(pKetTxRateEditor);
    m_pStateEditorTxRatePSK->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateParaPage41a);
    ketRxRateEditorPSK *pKetRxRateEditor = new ketRxRateEditorPSK(this);
    m_pStateEditorRxRatePSK->addTransition(pKetRxRateEditor);
    m_pStateEditorRxRatePSK->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateParaPage31a);
#endif
    ketPowerEditor *pKetPowerEditor = new ketPowerEditor(this);
    m_pStateEditorPower->addTransition(pKetPowerEditor);
    m_pStateEditorPower->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateParaPage2);

    ketTestPwrEditor *pKetTestPwrEditor = new ketTestPwrEditor(this);
    m_pStateEditorTestPwr->addTransition(pKetTestPwrEditor);
    m_pStateEditorTestPwr->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateMenuTestPwr);


    ketTDMeditor *pKetTDMeditor = new ketTDMeditor(this);
    m_pStateEditorTDM->addTransition(pKetTDMeditor);
    m_pStateEditorTDM->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateParaP11c);

    ketTDM2editor *pKetTDM2editor = new ketTDM2editor(this);
    m_pStateEditorTDM2->addTransition(pKetTDM2editor);
    m_pStateEditorTDM2->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateParaP12c);
    ketTxRateCentralEditor *pKetTxRateCentralEditor = new ketTxRateCentralEditor(this);
    m_pStateEditorTxRateC->addTransition(pKetTxRateCentralEditor);
    m_pStateEditorTxRateC->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateParaP21c);
    ketRxRateCentralEditor *pKetRxRateCentralEditor = new ketRxRateCentralEditor(this);
    m_pStateEditorRxRateC->addTransition(pKetRxRateCentralEditor);
    m_pStateEditorRxRateC->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateParaP22c);
    ketPowerCentralEditor *pKetPowerCentralEditor = new ketPowerCentralEditor(this);
    m_pStateEditorPowerCentral->addTransition(pKetPowerCentralEditor);
    m_pStateEditorPowerCentral->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateParaP13c);


    ketMenuWorkMode *pKetMenuBUCpwr = new ketMenuWorkMode(this);
    m_pStateMenuBUCpwr->addTransition(pKetMenuBUCpwr);
    m_pStateMenuBUCpwr->addTransition(this,SIGNAL(sigStateTransitionLeft()),m_pStateMenuLNBpwr);
    m_pStateMenuBUCpwr->addTransition(this,SIGNAL(sigStateTransitionRight()),m_pStateMenuLNBpwr);
    m_pStateMenuBUCpwr->addTransition(this,SIGNAL(sigStateTransitionDown()),m_pStateMenuBUC10m);
    m_pStateMenuBUCpwr->addTransition(this,SIGNAL(sigStateTransitionUp()),m_pStateMenuLNBfreq);
    m_pStateMenuBUCpwr->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateMenuRadioSetting);
    m_pStateMenuBUCpwr->addTransition(this,SIGNAL(sigStateTransitionNext()),m_pStateBUCpwr);
    ketMenuWorkMode *pKetMenuBUC10m = new ketMenuWorkMode(this);
    m_pStateMenuBUC10m->addTransition(pKetMenuBUC10m);
    m_pStateMenuBUC10m->addTransition(this,SIGNAL(sigStateTransitionLeft()),m_pStateMenuLNB10m);
    m_pStateMenuBUC10m->addTransition(this,SIGNAL(sigStateTransitionRight()),m_pStateMenuLNB10m);
    m_pStateMenuBUC10m->addTransition(this,SIGNAL(sigStateTransitionDown()),m_pStateMenuBUCfreq);
    m_pStateMenuBUC10m->addTransition(this,SIGNAL(sigStateTransitionUp()),m_pStateMenuBUCpwr);
    m_pStateMenuBUC10m->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateMenuRadioSetting);
    m_pStateMenuBUC10m->addTransition(this,SIGNAL(sigStateTransitionNext()),m_pStateBUC10m);
    ketMenuWorkMode *pKetMenuBUCfreq = new ketMenuWorkMode(this);
    m_pStateMenuBUCfreq->addTransition(pKetMenuBUCfreq);
    m_pStateMenuBUCfreq->addTransition(this,SIGNAL(sigStateTransitionLeft()),m_pStateMenuBUCfreq);
    m_pStateMenuBUCfreq->addTransition(this,SIGNAL(sigStateTransitionRight()),m_pStateMenuBUCfreq);
    m_pStateMenuBUCfreq->addTransition(this,SIGNAL(sigStateTransitionDown()),m_pStateMenuLNBfreq);
    m_pStateMenuBUCfreq->addTransition(this,SIGNAL(sigStateTransitionUp()),m_pStateMenuBUC10m);
    m_pStateMenuBUCfreq->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateMenuRadioSetting);
    m_pStateMenuBUCfreq->addTransition(this,SIGNAL(sigStateTransitionNext()),m_pStateEditorBUCfreq);
    ketMenuWorkMode *pKetMenuLNBpwr = new ketMenuWorkMode(this);
    m_pStateMenuLNBpwr->addTransition(pKetMenuLNBpwr);
    m_pStateMenuLNBpwr->addTransition(this,SIGNAL(sigStateTransitionLeft()),m_pStateMenuBUCpwr);
    m_pStateMenuLNBpwr->addTransition(this,SIGNAL(sigStateTransitionRight()),m_pStateMenuBUCpwr);
    m_pStateMenuLNBpwr->addTransition(this,SIGNAL(sigStateTransitionDown()),m_pStateMenuLNB10m);
    m_pStateMenuLNBpwr->addTransition(this,SIGNAL(sigStateTransitionUp()),m_pStateMenuLNBfreq);
    m_pStateMenuLNBpwr->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateMenuRadioSetting);
    m_pStateMenuLNBpwr->addTransition(this,SIGNAL(sigStateTransitionNext()),m_pStateLNBpwr);
    ketMenuWorkMode *pKetMenuLNB10m = new ketMenuWorkMode(this);
    m_pStateMenuLNB10m->addTransition(pKetMenuLNB10m);
    m_pStateMenuLNB10m->addTransition(this,SIGNAL(sigStateTransitionLeft()),m_pStateMenuBUC10m);
    m_pStateMenuLNB10m->addTransition(this,SIGNAL(sigStateTransitionRight()),m_pStateMenuBUC10m);
    m_pStateMenuLNB10m->addTransition(this,SIGNAL(sigStateTransitionDown()),m_pStateMenuBUCfreq);
    m_pStateMenuLNB10m->addTransition(this,SIGNAL(sigStateTransitionUp()),m_pStateMenuLNBpwr);
    m_pStateMenuLNB10m->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateMenuRadioSetting);
    m_pStateMenuLNB10m->addTransition(this,SIGNAL(sigStateTransitionNext()),m_pStateLNB10m);
    ketMenuWorkMode *pKetMenuLNBfreq = new ketMenuWorkMode(this);
    m_pStateMenuLNBfreq->addTransition(pKetMenuLNBfreq);
    m_pStateMenuLNBfreq->addTransition(this,SIGNAL(sigStateTransitionLeft()),m_pStateMenuLNBfreq);
    m_pStateMenuLNBfreq->addTransition(this,SIGNAL(sigStateTransitionRight()),m_pStateMenuLNBfreq);
    m_pStateMenuLNBfreq->addTransition(this,SIGNAL(sigStateTransitionDown()),m_pStateMenuTestCtrl);
    m_pStateMenuLNBfreq->addTransition(this,SIGNAL(sigStateTransitionUp()),m_pStateMenuBUCfreq);
    m_pStateMenuLNBfreq->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateMenuRadioSetting);
    m_pStateMenuLNBfreq->addTransition(this,SIGNAL(sigStateTransitionNext()),m_pStateEditorLNBfreq);



    ketMenuTestCtrl *pKetMenuTestCtrl = new ketMenuTestCtrl(this);
    m_pStateMenuTestCtrl->addTransition(pKetMenuTestCtrl);
    m_pStateMenuTestCtrl->addTransition(this,SIGNAL(sigStateTransitionLeft()),m_pStateMenuTestCtrl);
    m_pStateMenuTestCtrl->addTransition(this,SIGNAL(sigStateTransitionRight()),m_pStateMenuTestCtrl);
    m_pStateMenuTestCtrl->addTransition(this,SIGNAL(sigStateTransitionDown()),m_pStateMenuTestFreq);
    m_pStateMenuTestCtrl->addTransition(this,SIGNAL(sigStateTransitionUp()),m_pStateMenuLNBfreq);
    m_pStateMenuTestCtrl->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateMenuRadioSetting);
    m_pStateMenuTestCtrl->addTransition(this,SIGNAL(sigStateTransitionNext()),m_pStateEditorLNBfreq);
    ketMenuTestFreq *pKetMenuTestFreq = new ketMenuTestFreq(this);
    m_pStateMenuTestFreq->addTransition(pKetMenuTestFreq);
    m_pStateMenuTestFreq->addTransition(this,SIGNAL(sigStateTransitionLeft()),m_pStateMenuTestFreq);
    m_pStateMenuTestFreq->addTransition(this,SIGNAL(sigStateTransitionRight()),m_pStateMenuTestFreq);
    m_pStateMenuTestFreq->addTransition(this,SIGNAL(sigStateTransitionDown()),m_pStateMenuTestPwr);
    m_pStateMenuTestFreq->addTransition(this,SIGNAL(sigStateTransitionUp()),m_pStateMenuTestCtrl);
    m_pStateMenuTestFreq->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateMenuRadioSetting);
    m_pStateMenuTestFreq->addTransition(this,SIGNAL(sigStateTransitionNext()),m_pStateEditorTestFreq);
    ketMenuTestFreq *pKetMenuTestPwr = new ketMenuTestFreq(this);
    m_pStateMenuTestPwr->addTransition(pKetMenuTestPwr);
    m_pStateMenuTestPwr->addTransition(this,SIGNAL(sigStateTransitionLeft()),m_pStateMenuTestPwr);
    m_pStateMenuTestPwr->addTransition(this,SIGNAL(sigStateTransitionRight()),m_pStateMenuTestPwr);
    m_pStateMenuTestPwr->addTransition(this,SIGNAL(sigStateTransitionDown()),m_pStateMenuTestCtrl);
    m_pStateMenuTestPwr->addTransition(this,SIGNAL(sigStateTransitionUp()),m_pStateMenuTestFreq);
    m_pStateMenuTestPwr->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateMenuRadioSetting);
    m_pStateMenuTestPwr->addTransition(this,SIGNAL(sigStateTransitionNext()),m_pStateEditorTestPwr);




    ketMenuWorkMode *pKetMenuRadioSetting = new ketMenuWorkMode(this);
    m_pStateMenuRadioSetting->addTransition(pKetMenuRadioSetting);
    m_pStateMenuRadioSetting->addTransition(this,SIGNAL(sigStateTransitionDown()),m_pStateMenuCtrl);
    m_pStateMenuRadioSetting->addTransition(this,SIGNAL(sigStateTransitionUp()),m_pStateMenuWorkMode);
    m_pStateMenuRadioSetting->addTransition(this,SIGNAL(sigStateTransitionRight()),m_pStateMenuCtrl);
    m_pStateMenuRadioSetting->addTransition(this,SIGNAL(sigStateTransitionLeft()),m_pStateMenuWorkMode);
    m_pStateMenuRadioSetting->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateStatusPage1);
    m_pStateMenuRadioSetting->addTransition(this,SIGNAL(sigStateTransitionNext()),m_pStateMenuBUCpwr);

    ketMenuWorkMode *pKetMenuWorkMode = new ketMenuWorkMode(this);
    m_pStateMenuWorkMode->addTransition(pKetMenuWorkMode);
    m_pStateMenuWorkMode->addTransition(this,SIGNAL(sigStateTransitionLeft()),m_pStateAbout);
    m_pStateMenuWorkMode->addTransition(this,SIGNAL(sigStateTransitionRight()),m_pStateMenuRadioSetting);
    m_pStateMenuWorkMode->addTransition(this,SIGNAL(sigStateTransitionDown()),m_pStateMenuRadioSetting);
    m_pStateMenuWorkMode->addTransition(this,SIGNAL(sigStateTransitionUp()),m_pStateMenuPara);
    m_pStateMenuWorkMode->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateStatusPage1);
    m_pStateMenuWorkMode->addTransition(this,SIGNAL(sigStateTransitionNext()),m_pStateWorkMode1);
    ketWorkMode1 *pKetWorkMode1 = new ketWorkMode1(this);
    m_pStateWorkMode1->addTransition(pKetWorkMode1);
    m_pStateWorkMode1->addTransition(this,SIGNAL(sigStateTransitionDown()),m_pStateWorkMode2);
    m_pStateWorkMode1->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateMenuWorkMode);
    m_pStateWorkMode1->addTransition(this,SIGNAL(sigStateTransitionEnter()),m_pStateWorkMode1);
    ketWorkMode2 *pKetWorkMode2 = new ketWorkMode2(this);
    m_pStateWorkMode2->addTransition(pKetWorkMode2);
    m_pStateWorkMode2->addTransition(this,SIGNAL(sigStateTransitionDown()),m_pStateWorkMode1);
    m_pStateWorkMode2->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateMenuWorkMode);
    m_pStateWorkMode2->addTransition(this,SIGNAL(sigStateTransitionEnter()),m_pStateWorkMode2);

    ketDevMode1 *pKetDevMode1 = new ketDevMode1(this);
    m_pStateDevMode1->addTransition(pKetDevMode1);
    m_pStateDevMode1->addTransition(this,SIGNAL(sigStateTransitionDown()),m_pStateDevMode2);
    m_pStateDevMode1->addTransition(this,SIGNAL(sigStateTransitionP2P()),m_pStateParaPage21);
    m_pStateDevMode1->addTransition(this,SIGNAL(sigStateTransitionCentral()),m_pStateParaP23c);
    m_pStateDevMode1->addTransition(this,SIGNAL(sigStateTransitionEnter()),m_pStateDevMode1);
    ketDevMode2 *pKetDevMode2 = new ketDevMode2(this);
    m_pStateDevMode2->addTransition(pKetDevMode2);
    m_pStateDevMode2->addTransition(this,SIGNAL(sigStateTransitionDown()),m_pStateDevMode1);
    m_pStateDevMode2->addTransition(this,SIGNAL(sigStateTransitionP2P()),m_pStateParaPage21);
    m_pStateDevMode2->addTransition(this,SIGNAL(sigStateTransitionCentral()),m_pStateParaP23c);
    m_pStateDevMode2->addTransition(this,SIGNAL(sigStateTransitionEnter()),m_pStateDevMode2);

    // p2p call menu
    ketMenuCall *pKetMenuCall = new ketMenuCall(this);
    m_pStateMenuCall->addTransition(pKetMenuCall);
    m_pStateMenuCall->addTransition(this,SIGNAL(sigStateTransitionBackspace()),m_pStateMenuCtrl);
    m_pStateMenuCall->addTransition(this,SIGNAL(sigStateTransitionNext()),m_pStateMsgZZHJ);
    m_pStateMenuCall->addTransition(this,SIGNAL(sigStateTransitionRepaint()),m_pStateMenuCall);




    m_pMachine->start();

    m_pTimer1s->start();

    QTimer::singleShot(1000,this,SLOT(slotGetCUstateInit()));

    qDebug("     end objui.initMachine");
}
void objui::slotGetCUstate()
{
    std::string stdstr;

    stdstr=m_cu.getCUState();
    writeTcp(QString::fromStdString(stdstr).toUtf8());
    stdstr=m_cu.getSessionState();
    writeTcp(QString::fromStdString(stdstr).toUtf8());

    stdstr=m_cu.getRadioLinkState();
    writeTcp(QString::fromStdString(stdstr).toUtf8());

    if(m_status.m_cuMode==objPara::CUState_mode_OFFLINE_P2P){
        stdstr=m_cu.getP2PModeParam();
        writeTcp(QString::fromStdString(stdstr).toUtf8());
    }
}
void objui::slotGetCUstateInit()
{
    std::string stdstr;

    if(m_status.m_cuMode==objPara::CUState_mode_unknown){
        stdstr=m_cu.getCUState();
        writeTcp(QString::fromStdString(stdstr).toUtf8());
        stdstr=m_cu.getSessionState();
        writeTcp(QString::fromStdString(stdstr).toUtf8());
        stdstr=m_cu.getRadioLinkParams();
        writeTcp(QString::fromStdString(stdstr).toUtf8());
        QTimer::singleShot(1000,this,SLOT(slotGetCUstateInit()));
    }
    else{
        if(m_para.m_workMode==objPara::Mode_p2p){
            stdstr=m_cu.logoutNet();
            writeTcp(QString::fromStdString(stdstr).toUtf8());
            QTimer::singleShot(500,this,SLOT(slotGetCUstate()));
            QTimer::singleShot(1000,this,SLOT(slotGetCUstateInitLogout()));
        }
        else{
            m_bInitDone = true;
        }
    }
}
void objui::slotGetCUstateInitLogout()
{
    std::string stdstr;

    if(m_status.m_cuMode==objPara::CUState_mode_NCC_CTRL
            && m_status.m_cuNetState==objPara::CUState_netState_OFFLINE){
        m_bInitDone = true;
    }
    else{
        stdstr=m_cu.logoutNet();
        writeTcp(QString::fromStdString(stdstr).toUtf8());
        QTimer::singleShot(500,this,SLOT(slotGetCUstate()));
        QTimer::singleShot(1000,this,SLOT(slotGetCUstateInitLogout()));
    }
}
void objui::getCUstate()
{
    std::string stdstr;

    // test
    //stdstr=m_cu.logoutNet();
    //writeTcp(QString::fromStdString(stdstr).toUtf8());

    stdstr=m_cu.getCUState();
    writeTcp(QString::fromStdString(stdstr).toUtf8());
    stdstr=m_cu.getSessionState();
    writeTcp(QString::fromStdString(stdstr).toUtf8());

}

void objui::doMenuPara()
{
    emit sigStateTransitionNext();
}

void objui::slotEVzzhjBack()
{
    emit sigEVzzhjBack();
}
void objui::slotEVzzhjBackM()
{
    emit sigEVzzhjBackM();
}
void objui::slotEVzzhjPaint()
{
    emit sigEVzzhjPaint();
}

void objui::slotStateTransitionRepaint()
{
    emit sigStateTransitionRepaint();
}

void objui::slotStateTransitionNext()
{
    emit sigStateTransitionNext();
}
void objui::slotStateTransitionBack()
{
    emit sigStateTransitionBack();
}

void objui::slotStateTransitionUp()
{
    emit sigStateTransitionUp();
}
void objui::slotStateTransitionDown()
{
    emit sigStateTransitionDown();
}
void objui::slotStateTransitionLeft()
{
    emit sigStateTransitionLeft();
}
void objui::slotStateTransitionRight()
{
    emit sigStateTransitionRight();
}
void objui::slotStateTransitionEnter()
{
    emit sigStateTransitionEnter();
}
void objui::slotStateTransitionBackspace()
{
    emit sigStateTransitionBackspace();
}
void objui::slotStateTransitionP2P()
{
    emit sigStateTransitionP2P();
}
void objui::slotStateTransitionCentral()
{
    emit sigStateTransitionCentral();
}

#if 0
void objui::buildMachine()
{
    m_pMachine = new QStateMachine;

    QState *p0000 = new QState();
    QState *p0100 = new QState();
    QState *p0200 = new QState();

#if 0
    //QState *quitState = new QState(machine);
    //quitState->assignProperty(this, "status", "Really quit(y/n)?");

    QKeyEventTransition *yesTransition = new
        QKeyEventTransition(this, QEvent::KeyPress, Qt::Key_Y);
    yesTransition->setTargetState(new QFinalState(machine));
    quitState->addTransition(yesTransition);

    QKeyEventTransition *noTransition =
        new QKeyEventTransition(this, QEvent::KeyPress, Qt::Key_N);
    noTransition->setTargetState(inputState);
    quitState->addTransition(noTransition);

    QKeyEventTransition *quitTransition =
        new QKeyEventTransition(this, QEvent::KeyPress, Qt::Key_Q);
    quitTransition->setTargetState(quitState);
    inputState->addTransition(quitTransition);

    QKeyEventTransition *kt = new QKeyEventTransition(this,QEvent::KeyPress,Qt::Key_Enter);
    kt->setTargetState(p0100);
    p0000->addTransition(kt);

    QKeyEventTransition *kt1 = new QKeyEventTransition(this,QEvent::KeyPress,Qt::Key_Enter);
    kt1->setTargetState(p0000);
    p0001->addTransition(kt1);

    //connect(p0000,SIGNAL(en))

#endif
    m_pMachine->addState(p0000);
    m_pMachine->addState(p0100);
    m_pMachine->setInitialState(p0000);

    QKeyEventTransition *ket00to01 = new QKeyEventTransition(0, QEvent::KeyPress, Qt::Key_Down);
    ket00to01->setTargetState(p0100);
    p0000->addTransition(ket00to01);

    kt0000 *k0000=new kt0000(this);
    //k0000->setTargetState(p0100);
    p0000->addTransition(k0000);

    p0000->addTransition(this,SIGNAL(sig0000()),p0100);

    //kt0000 *k0001=new kt0000(this);
    //p0100->addTransition(k0001);

    //connect(ket00to01,SIGNAL(triggered()),this,SLOT(slotKET()));


    connect(m_pMachine,SIGNAL(started()),this,SLOT(slotTest()));
    connect(m_pMachine,SIGNAL(stopped()),this,SLOT(slotTestStop()));

    connect(p0000,SIGNAL(entered()),this,SLOT(slot00enter()));
    connect(p0000,SIGNAL(exited()),this,SLOT(slot00exit()));

    connect(p0100,SIGNAL(entered()),this,SLOT(slot01enter()));


    m_pMachine->start();

    QKeyEvent *ev = new QKeyEvent(QEvent::KeyPress,Qt::Key_Enter,Qt::NoModifier);

    //while (!m_pMachine->isRunning()) {

    //}
    //m_pMachine->postDelayedEvent(ev,5000);
}
#endif
void objui::slotTest()
{
    qDebug(" machine start");
}

void objui::slotTestStop()
{
    qDebug(" machine stop");
}
void objui::initUDPkey()
{
    m_pUDPkey = new QUdpSocket(this);
    //QHostAddress gAddr=QHostAddress("239.255.43.21");

    m_pUDPkey->bind(QHostAddress::Any,57700,QUdpSocket::ShareAddress);
    //m_pus->joinMulticastGroup(QHostAddress("239.255.43.21"));

    connect(m_pUDPkey, SIGNAL(readyRead()),this, SLOT(slotUDPkey()));

}
void objui::slotUDPkey()
{
    int key;
    int buf32[100];

    while (m_pUDPkey->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(m_pUDPkey->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        m_pUDPkey->readDatagram(datagram.data(), datagram.size(),&sender, &senderPort);

        if(datagram.size()>0){
            m_hostVOLED.setAddress(sender.toIPv4Address());
            //memcpy(&key,datagram.data(),4);
        }
        if(datagram.size()==8){
            memcpy((char*)buf32,datagram.data(),8);
            switch (buf32[0]) {
            case 0x55abab55:
                key = htonl(buf32[1]);
                //qDebug(" udp key ================== %x",key);
                slotKey(key);
                break;
            default:
                break;
            }
        }
    }

}

void objui::slotKey(int key)
{
    //qDebug(" slotkey : 0x%x",key);
    QKeyEvent *ev;// = new QKeyEvent(QEvent::KeyPress,Qt::Key_4,Qt::NoModifier);
    m_bEnable1s = false;
    m_pTimer2s->start();
    if(!m_semKey.tryAcquire()){
        writeLog(QString("keyboard block : 0x%1\n").arg(key,0,16).toLatin1());
        return;
    }
    //qDebug(" slotKey(............ func.objui");
    switch (key) {
    case KEY_DOWN:
        ev = new QKeyEvent(QEvent::KeyPress,Qt::Key_Down,Qt::NoModifier);
        m_pTimer60->start();
        m_pMachine->postEvent(ev);
        break;
    case KEY_UP:
        ev = new QKeyEvent(QEvent::KeyPress,Qt::Key_Up,Qt::NoModifier);
        m_pTimer60->start();
        m_pMachine->postEvent(ev);
        break;
    case KEY_LEFT:
        ev = new QKeyEvent(QEvent::KeyPress,Qt::Key_Left,Qt::NoModifier);
        m_pTimer60->start();
        m_pMachine->postEvent(ev);
        break;
    case KEY_RIGHT:
        ev = new QKeyEvent(QEvent::KeyPress,Qt::Key_Right,Qt::NoModifier);
        m_pTimer60->start();
        m_pMachine->postEvent(ev);
        break;
    case KEY_ENTER:
        ev = new QKeyEvent(QEvent::KeyPress,Qt::Key_Enter,Qt::NoModifier);
        m_pTimer60->start();
        m_pMachine->postEvent(ev);
        break;
    case KEY_BACKSPACE:
        ev = new QKeyEvent(QEvent::KeyPress,Qt::Key_Backspace,Qt::NoModifier);
        m_pTimer60->start();
        m_pMachine->postEvent(ev);
        break;
    default:
        break;
    }
    m_pTimerKey->start();

}

void objui::slot00enter()
{
    qDebug(" state 00 enter");
}
void objui::slot00exit()
{
    qDebug(" state 00 exit.exit");
}
void objui::slot01enter()
{
    qDebug(" state 01..01 enter");
}

void objui::slotKET()
{
    qDebug("     ====== KET");
}

void objui::slot0000()
{
    emit sig0000();
}
void objui::getColorMenuCall(int n, int *pc, int *pbg)
{
    if(n==m_nSelectMenu10){
        *pc=0;*pbg=0x0f;
    }
    else{
        if(m_bEnableMenu10[n]){
            *pc=0x0f,*pbg=0;
        }
        else{
            *pc=1,*pbg=0;
        }
    }
}
void objui::showMenuCallgray()
{
    zeroFB(0);

    centerXY(QString("入    网"),0,0,256,16,1,1,1,0);
    centerXY(QString("退    网"),0,16,256,16,1,1,1,0);
    centerXY(QString("呼    叫"),0,32,256,16,1,1,1,0);
    centerXY(QString("挂    断"),0,48,256,16,1,1,1,0);

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();
    slotKeyEnable();
}

// p2p.menu.call
void objui::showMenuCallP2P()
{
    zeroFB(0);

    if(m_status.m_cuMode==objPara::CUState_mode_OFFLINE_P2P ){
        centerXY(QString("呼    叫"),0,12,256,16,1,1,1);// gray
        centerXY(QString("挂    断"),0,48-10,256,16,1,1,0,0x0f);
        m_status.m_callCMD = objPara::CMD_P2P_HANGUP;
    }
    else{
        centerXY(QString("呼    叫"),0,12,256,16,1,1,0,0x0f);
        centerXY(QString("挂    断"),0,48-10,256,16,1,1,1);
        m_status.m_callCMD = objPara::CMD_P2P_CALL;
    }

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();
    slotKeyEnable();
}

void objui::showMenuCallNCC()
{

}
void objui::showMenuCallNCClogout()
{

}
void objui::setEnableMenuCall()
{
    switch(m_status.m_cuNetState){
    case objPara::CUState_netState_WAITING_TDM:
    case objPara::CUState_netState_LOGINING:
    case objPara::CUState_netState_LOGOUTING:
        m_bMenuCall_login=false;
        m_bMenuCall_logout=true;
        m_bMenuCall_call=false;
        m_bMenuCall_hangup=false;
        break;
    case objPara::CUState_netState_OFFLINE:
        m_bMenuCall_login=true;
        m_bMenuCall_logout=false;
        m_bMenuCall_call=false;
        m_bMenuCall_hangup=false;
        break;
    case objPara::CUState_netState_ONLINE:
        switch(m_status.m_sessionState){
        case objPara::SESSIONstate_dialing:
        case objPara::SESSIONstate_alerting:
        case objPara::SESSIONstate_closing:
        case objPara::SESSIONstate_connected:
            m_bMenuCall_login=false;
            m_bMenuCall_logout=true;
            m_bMenuCall_call=false;
            m_bMenuCall_hangup=true;
            break;
        case objPara::SESSIONstate_closed:
            m_bMenuCall_login=false;
            m_bMenuCall_logout=true;
            m_bMenuCall_call=true;
            m_bMenuCall_hangup=false;
            break;
        case objPara::SESSIONstate_incoming:
            m_bMenuCall_login=false;
            m_bMenuCall_logout=true;
            m_bMenuCall_call=false;
            m_bMenuCall_hangup=false;
            break;
        default:
            m_bMenuCall_login=false;
            m_bMenuCall_logout=true;
            m_bMenuCall_call=false;
            m_bMenuCall_hangup=false;
            break;
        }
        break;
    default:
        m_bMenuCall_login=false;
        m_bMenuCall_logout=false;
        m_bMenuCall_call=false;
        m_bMenuCall_hangup=false;
        break;
    }

}
bool objui::setCMD(int n)
{
    bool b=false;

    m_status.m_callCMD = objPara::CMD_DONOTHING;
    switch(n){
    case 0:
        if(m_bMenuCall_login){
            m_status.m_callCMD = objPara::CMD_NCC_LOGIN;
            b= true;
        }
        break;
    case 1:
        if(m_bMenuCall_logout){
            m_status.m_callCMD = objPara::CMD_NCC_LOGOUT;
            b= true;
        }
        break;
    case 2:
        if(m_bMenuCall_call){
            m_status.m_callCMD = objPara::CMD_NCC_CALL;
            b= true;
        }
        break;
    case 3:
        if(m_bMenuCall_hangup){
            m_status.m_callCMD = objPara::CMD_NCC_HANGUP;
            b= true;
        }
        break;
    default:
        break;
    }
    return b;
}

void objui::moveCursorMenuCall(int inc)
{
    int i;

    m_nCursorMenuCall = (m_nCursorMenuCall+inc) % 4;
    if(inc >= 0){
        for(i=0;i<4;i++){
            if(setCMD(m_nCursorMenuCall)) break;
            m_nCursorMenuCall = (m_nCursorMenuCall+1) % 4;
        }
    }
    else{
        for(i=0;i<4;i++){
            if(setCMD(m_nCursorMenuCall)) break;
            m_nCursorMenuCall = (m_nCursorMenuCall-1) % 4;
        }
    }

}

void objui::slotShowMenuCall()
{
    m_nCountTimerCall = 0;// reset

    if(m_status.m_cuUsageState==objPara::CUState_usageState_NCC_PLAN){ // all gray
        showMenuCallgray();
        return;
    }
    if(m_status.m_cuMode==objPara::CUState_mode_OFFLINE_P2P ){
        qDebug("       slotShowMenuCall , m_status.m_cuMode(%d) == offline_p2p(%d) ",
               m_status.m_cuMode,objPara::CUState_mode_OFFLINE_P2P);
        qDebug("      2 slotShowMenuCall , m_status.m_cuMode(%d) ?? ncc_ctrl(%d) ",
               m_status.m_cuMode,objPara::CUState_mode_NCC_CTRL);
        showMenuCallP2P();
        return;
    }
    else if(m_status.m_cuNetState==objPara::CUState_netState_OFFLINE && m_para.m_workMode==objPara::Mode_p2p){
        qDebug("     *****  slotShowMenuCall , m_status.m_cuMode(%d) == offline_p2p(%d) ",
               m_status.m_cuMode,objPara::CUState_mode_OFFLINE_P2P);
        qDebug("     ****** 2 slotShowMenuCall , m_status.m_cuMode(%d) ?? ncc_ctrl(%d) ",
               m_status.m_cuMode,objPara::CUState_mode_NCC_CTRL);
        qDebug("       slotShowMenuCall , m_para.m_workMode(%d) == mode_p2p(%d) ",
               m_para.m_workMode, objPara::Mode_p2p);
        showMenuCallP2P();
        return;
    }
    // NCC_CTRL
    qDebug("          slotshowMenuCall    NCC_CTRL ");
    setEnableMenuCall();// central
    moveCursorMenuCall(0);

    zeroFB(0);

    if(m_bMenuCall_login) centerXY(QString("入    网"),0,0,256,16,1,1);
    else centerXY(QString("入    网"),0,0,256,16,1,1,1);
    if(m_bMenuCall_logout) centerXY(QString("退    网"),0,16,256,16,1,1);
    else centerXY(QString("退    网"),0,16,256,16,1,1,1);// gray
    if(m_bMenuCall_call) centerXY(QString("呼    叫"),0,32,256,16,1,1);
    else centerXY(QString("呼    叫"),0,32,256,16,1,1,1);
    if(m_bMenuCall_hangup) centerXY(QString("挂    断"),0,48,256,16,1,1);
    else centerXY(QString("挂    断"),0,48,256,16,1,1,1);// gray

    switch(m_status.m_callCMD){
    case objPara::CMD_NCC_LOGIN: centerXY(QString("入    网"),0,0,256,16,1,1,0,0x0f);break;
    case objPara::CMD_NCC_LOGOUT: centerXY(QString("退    网"),0,16,256,16,1,1,0,0x0f);break;
    case objPara::CMD_NCC_CALL: centerXY(QString("呼    叫"),0,32,256,16,1,1,0,0x0f);break;
    case objPara::CMD_NCC_HANGUP: centerXY(QString("挂    断"),0,48,256,16,1,1,0,0x0f);break;
    default: break;
    }

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();
    //QTimer::singleShot(200,this,SLOT(slotKeyEnable()));
    slotKeyEnable();

}
void objui::changeSelectMenu10(int step)
{
    if(m_nSelectMenu10<0 || m_nSelectMenu10>3)m_nSelectMenu10=0;
    for(;;){
        m_nSelectMenu10 = (m_nSelectMenu10+step) & 0x3;
        if(m_bEnableMenu10[m_nSelectMenu10]) break;
    }
    slotShowMenuCall();
}
void objui::slotP2PmodeParam(QByteArray ba)
{
    struct P2PMode *pMode;
    pMode = (struct P2PMode *)ba.data();
    memcpy(&m_P2PMode,ba.data(),sizeof(struct P2PMode));
    qDebug("  == p2p.param------------tx.freq: %lld",pMode->txFrequence);
    qDebug("  p2p.param------------rx.freq: %lld",pMode->rxFrequence);
    qDebug("  p2p.param------------tx.rate: %lld",pMode->txBitrate);
    qDebug("  p2p.param------------rx.rate: %lld",pMode->rxBitrate);
    qDebug("  p2p.param------------power: %.2f",pMode->txIFPower);
    m_status.m_RxFreq = pMode->rxFrequence;
    m_status.m_TxFreq = pMode->txFrequence;
    m_status.m_rxRate = pMode->rxBitrate/1000;
    m_status.m_txRate = pMode->txBitrate/1000;
    m_status.m_power100 = pMode->txIFPower * 100.;
    QString str=QString::fromStdString(pMode->dataCommMode);
    if(str.contains("bridge")){
        m_status.m_devMode = objPara::DevMode_bridge;
    }
    else{
        m_status.m_devMode = objPara::DevMode_router;
    }
}
void objui::setBUCfreq(qint64 freq)
{
    std::string stdstr;

    struct RadioLinkParamsChanged para;
    para.dataSendLink.lo_frequency = freq;

    stdstr=m_cu.setRadioLinkParams(para);// setConfigurationModem(modem);
    writeTcp(QString::fromStdString(stdstr).toUtf8());

    QTimer::singleShot(500,this,SLOT(slotGetRadioParams()));
    QTimer::singleShot(1500,this,SLOT(slotGetRadioParams()));

}
void objui::setLNBfreq(qint64 freq)
{
    std::string stdstr;

    struct RadioLinkParamsChanged para;
    para.dataRecvLink.lo_frequency = freq;

    stdstr=m_cu.setRadioLinkParams(para);// setConfigurationModem(modem);
    writeTcp(QString::fromStdString(stdstr).toUtf8());

    QTimer::singleShot(500,this,SLOT(slotGetRadioParams()));
    QTimer::singleShot(1500,this,SLOT(slotGetRadioParams()));

}

void objui::slotRadioLinkParams(QByteArray ba)
{
    struct RadioLinkParamsChanged *pRadio;
    pRadio = (struct RadioLinkParamsChanged *)ba.data();

    memcpy(&m_RadioLinkParamsChanged,pRadio,sizeof(struct RadioLinkParamsChanged));
    m_para.m_BUCfreq = m_RadioLinkParamsChanged.dataSendLink.lo_frequency;
    m_para.m_LNBfreq = m_RadioLinkParamsChanged.dataRecvLink.lo_frequency;
}

void objui::slotRadioLinkState(QByteArray ba)
{
    qint64 i64;
    struct RadioLinkStateChanged *pRadio;
    pRadio = (struct RadioLinkStateChanged *)ba.data();
    memcpy(&m_RadioLinkStateChanged,ba.data(),sizeof(struct RadioLinkStateChanged));
    //qDebug("  --- %s ---------snr: %.2f",QTime::currentTime().toString("h:m:s").toLatin1().data(),pRadio->dataRecvLink.snr);
    m_status.m_fSNR = pRadio->dataRecvLink.snr;
    m_status.m_freqOffset = pRadio->dataRecvLink.freqOffset;
    i64 = pRadio->dataRecvLink.frequency;
    if(i64>0){
        m_status.m_RxFreq = i64;
        m_status.m_rxRateC = pRadio->dataRecvLink.datarate/1000;
    }
    i64 = pRadio->dataSendLink.frequency;
    if(i64>0){
        m_status.m_TxFreq = i64;
        m_status.m_txRateC = pRadio->dataSendLink.datarate/1000;
    }
    m_status.m_recvSync= pRadio->dataRecvLink.sync;
    m_status.m_power100Central = 100. * pRadio->dataSendLink.if_power;
    emit sigStateTransitionRepaint();
}
void objui::slotTDMConfig(QByteArray ba)
{
    struct TDMStruct *p;
    p = (struct TDMStruct *)ba.data();
    if(p->initTDMFrequency.size()>1){
        qDebug(" ==  get.tdm.freq2: %lld",p->initTDMFrequency[1]);
        m_para.m_TDMfreq2 = p->initTDMFrequency[1];
    }
    if(p->initTDMFrequency.size()>0){
        qDebug(" ==  get.tdm.freq1: %lld",p->initTDMFrequency[0]);
        m_para.m_TDMfreq1 = p->initTDMFrequency[0];
    }
}
void objui::slotModemConfig(QByteArray ba)
{
    qDebug("====================================================================================slot modem status");
    struct ModemOutputStruct *p;
    p = (struct ModemOutputStruct *)ba.data();

    if(p->bucPowerSupply>0) m_status.m_swBUCpwr = 1;
    else if(p->bucPowerSupply==0) m_status.m_swBUCpwr = -1;
    else m_status.m_swBUCpwr = 0;

    if(p->buc10MRef>0) m_status.m_swBUC10m = 1;
    else if(p->buc10MRef==0) m_status.m_swBUC10m = -1;
    else m_status.m_swBUC10m = 0;

    if(p->lnbPowerSupply>0) m_status.m_swLNBpwr = 1;
    else if(p->lnbPowerSupply==0) m_status.m_swLNBpwr = -1;
    else m_status.m_swLNBpwr = 0;

    if(p->lnb10MRef>0) m_status.m_swLNB10m = 1;
    else if(p->buc10MRef==0) m_status.m_swLNB10m = -1;
    else m_status.m_swLNB10m = 0;

    emit sigStateTransitionRepaint();
}
void objui::slotPowerConfig(QByteArray ba)
{
    struct PowerStruct *p;
    p = (struct PowerStruct *)ba.data();
    m_para.m_power100Central = p->powerAdjust;
    qDebug(" =*****=  get.poweradj: %.2f  %lld",p->powerAdjust,m_para.m_power100Central);
}
void objui::slotSessionState(QByteArray ba)
{
    qDebug("   xxxxxxxxxxxxxxxxxx === xxxxxxxxx == slot.session.state");
    struct Session *p;
    p = (struct Session*)ba.data();
    QString str=QString::fromStdString(p->state);

    if(0==str.compare(QString("dialing"),Qt::CaseInsensitive)){
        m_status.m_sessionState = objPara::SESSIONstate_dialing;
    }
    else if(0==str.compare(QString("alerting"),Qt::CaseInsensitive)){
        m_status.m_sessionState = objPara::SESSIONstate_alerting;
    }
    else if(0==str.compare(QString("incoming"),Qt::CaseInsensitive)){
        m_status.m_sessionState = objPara::SESSIONstate_incoming;
    }
    else if(0==str.compare(QString("connected"),Qt::CaseInsensitive)){
        m_status.m_sessionState = objPara::SESSIONstate_connected;
    }
    else if(0==str.compare(QString("closing"),Qt::CaseInsensitive)){
        m_status.m_sessionState = objPara::SESSIONstate_closing;
    }
    else{// closed
    //if(0==str.compare(QString("dialing"),Qt::CaseInsensitive)){
        m_status.m_sessionState = objPara::SESSIONstate_closed;
    }

}
bool objui::isStatusP2P()
{

    return m_boolStatusPageP2P;
}

void objui::slotCUState(QByteArray ba)
{
    struct CUState *p;
    p = (struct CUState*)ba.data();
    QString str=QString::fromStdString(p->mode);

    memcpy(&m_CUState,ba.data(),sizeof(struct CUState));

    if(0==str.compare(QString("offline_p2p"),Qt::CaseInsensitive)){
        qDebug("             ===========custate , p2p  ");
        if( m_status.m_workMode != objPara::Mode_p2p){
            qDebug("             ===========custate , p2p  111111111111111111");
            m_status.m_workMode=objPara::Mode_p2p;
            emit sigStateTransitionTimeout();// back to status page
        }
        if(m_para.m_status != objPara::Status_connected){
            qDebug("             ===========custate , p2p  start count");
            m_para.m_startSecs = QDateTime::currentDateTime().toTime_t();
            m_para.m_status = objPara::Status_connected;
        }

        m_status.m_cuMode = objPara::CUState_mode_OFFLINE_P2P;
    }
    else{// "ncc_ctrl"
        m_status.m_cuMode = objPara::CUState_mode_NCC_CTRL;

        QString str1=QString::fromStdString(p->state);
        if(0==str1.compare(QString("offline"),Qt::CaseInsensitive)){
            m_para.m_status=objPara::Status_idle;
            m_status.m_workMode = objPara::Mode_unknown;

            m_status.m_cuNetState = objPara::CUState_netState_OFFLINE;
        }
        else if(0==str1.compare(QString("waiting_tdm"),Qt::CaseInsensitive)){
            m_para.m_status=objPara::Status_waiting_tdm;
            m_status.m_workMode = objPara::Mode_central;

            m_status.m_cuNetState = objPara::CUState_netState_WAITING_TDM;
        }
        else if(0==str1.compare(QString("logining"),Qt::CaseInsensitive)){
            m_para.m_status=objPara::Status_logining;
            m_status.m_workMode = objPara::Mode_central;

            m_status.m_cuNetState = objPara::CUState_netState_LOGINING;
        }
        else if(0==str1.compare(QString("logouting"),Qt::CaseInsensitive)){
            m_para.m_status=objPara::Status_logouting;
            m_status.m_workMode = objPara::Mode_central;

            m_status.m_cuNetState = objPara::CUState_netState_LOGOUTING;
        }
        else if(0==str1.compare(QString("online"),Qt::CaseInsensitive)){
            m_status.m_cuNetState = objPara::CUState_netState_ONLINE;

            m_status.m_workMode = objPara::Mode_central;
            QString str2=QString::fromStdString(p->usageState);
            if(0==str2.compare(QString("idle"),Qt::CaseInsensitive)){
                m_para.m_status=objPara::Status_online_idle;

                m_status.m_cuUsageState = objPara::CUState_usageState_IDLE;
            }
            else if(0==str2.compare(QString("p2p_call"),Qt::CaseInsensitive)){
                m_para.m_status=objPara::Status_online_p2p_call;

                m_status.m_cuUsageState = objPara::CUState_usageState_P2P_CALL;
            }
            else{
                m_status.m_cuUsageState = objPara::CUState_usageState_NCC_PLAN;

                m_para.m_status=objPara::Status_online_ncc_plan;
            }

        }

    }
    qDebug("mode : ********************************************  %s ",QString::fromStdString(p->mode).toLatin1().data());
    qDebug("!!!! isdn : --------------- %s",QString::fromStdString(p->isdn).toLatin1().data());
    qDebug(" name: -------------- %s",QString::fromStdString(p->name).toLatin1().data());
    qDebug("      -----------------  state ----------- %s",QString::fromStdString(p->state).toLatin1().data());
    qDebug("=============*******************=========== usageState : ----------- %s",QString::fromStdString(p->usageState).toLatin1().data());

}
void objui::slotGetP2Pstatus()
{
    std::string stdstr;

    stdstr=m_cu.getCUState();
    writeTcp(QString::fromStdString(stdstr).toUtf8());

    stdstr=m_cu.getRadioLinkState();
    writeTcp(QString::fromStdString(stdstr).toUtf8());

    stdstr=m_cu.getP2PModeParam();
    writeTcp(QString::fromStdString(stdstr).toUtf8());

}
void objui::cmdNCCcall()
{
    struct Session s;
    std::string stdstr;

    s.sessionType=2;// data
    s.calleeISDN=m_para.m_strCallID.toLatin1().data();
    s.sendDataRate = m_para.m_txRateC * 1000;
    s.recvDataRate = m_para.m_rxRateC * 1000;
    if(m_para.m_devMode==objPara::DevMode_bridge){
        s.dataCommMode = "bridge";
    }
    else{
        s.dataCommMode = "route";
    }
    stdstr=m_cu.createSession(s);
    writeTcp(QString::fromStdString(stdstr).toUtf8());

}
void objui::cmdNCChangup()
{
    std::string stdstr;

    stdstr=m_cu.closeSession();
    writeTcp(QString::fromStdString(stdstr).toUtf8());
}

void objui::doCallP2P()
{
    qDebug(" docallp2p, +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
    struct P2PMode p2pmode;
    std::string stdstr;

    switch(m_para.m_txPSK){
    case objPara::Mod_qpsk12:
        p2pmode.txModType="QPSK";
        p2pmode.txFECRate="1/2";
        break;
    case objPara::Mod_qpsk34:
        p2pmode.txModType="QPSK";
        p2pmode.txFECRate="3/4";
        break;
    case objPara::Mod_8psk12:
        p2pmode.txModType="8PSK";
        p2pmode.txFECRate="1/2";
        break;
    default:// 8psk34
        p2pmode.txModType="8PSK";
        p2pmode.txFECRate="3/4";
        break;
    }
    switch(m_para.m_rxPSK){
    case objPara::Mod_qpsk12:
        p2pmode.rxModType="QPSK";
        p2pmode.rxFECRate="1/2";
        break;
    case objPara::Mod_qpsk34:
        p2pmode.rxModType="QPSK";
        p2pmode.rxFECRate="3/4";
        break;
    case objPara::Mod_8psk12:
        p2pmode.rxModType="8PSK";
        p2pmode.rxFECRate="1/2";
        break;
    default:// 8psk34
        p2pmode.rxModType="8PSK";
        p2pmode.rxFECRate="3/4";
        break;
    }

    p2pmode.txFrequence = m_para.m_TxFreq;// + m_para.m_BUCfreq;
    p2pmode.txBitrate = m_para.m_txRate*1000;
    p2pmode.txIFPower = 0.01 * m_para.m_power100;
    p2pmode.rxFrequence = m_para.m_RxFreq;// + m_para.m_LNBfreq;
    p2pmode.rxBitrate = m_para.m_rxRate*1000;
    if(m_para.m_devMode==objPara::DevMode_bridge){
        p2pmode.dataCommMode="bridge";
    }
    else{
        p2pmode.dataCommMode="route";
    }
    stdstr=m_cu.enterP2PMode(p2pmode);
    writeTcp(QString::fromStdString(stdstr).toUtf8());

    QTimer::singleShot(500,this,SLOT(slotGetP2Pstatus()));

#if 0
    stdstr=m_cu.getCUState();
    writeTcp(QString::fromStdString(stdstr).toUtf8());

    stdstr=m_cu.getRadioLinkState();
    writeTcp(QString::fromStdString(stdstr).toUtf8());

    stdstr=m_cu.getP2PModeParam();
    writeTcp(QString::fromStdString(stdstr).toUtf8());
#endif
}
void objui::doCallP2Pagain()
{
    if(m_para.m_status!=objPara::Status_connected) return;
    struct P2PMode p2pmode;
    std::string stdstr;

    switch(m_para.m_txPSK){
    case objPara::Mod_qpsk12:
        p2pmode.txModType="QPSK";
        p2pmode.txFECRate="1/2";
        break;
    case objPara::Mod_qpsk34:
        p2pmode.txModType="QPSK";
        p2pmode.txFECRate="3/4";
        break;
    case objPara::Mod_8psk12:
        p2pmode.txModType="8PSK";
        p2pmode.txFECRate="1/2";
        break;
    default:// 8psk34
        p2pmode.txModType="8PSK";
        p2pmode.txFECRate="3/4";
        break;
    }
    switch(m_para.m_rxPSK){
    case objPara::Mod_qpsk12:
        p2pmode.rxModType="QPSK";
        p2pmode.rxFECRate="1/2";
        break;
    case objPara::Mod_qpsk34:
        p2pmode.rxModType="QPSK";
        p2pmode.rxFECRate="3/4";
        break;
    case objPara::Mod_8psk12:
        p2pmode.rxModType="8PSK";
        p2pmode.rxFECRate="1/2";
        break;
    default:// 8psk34
        p2pmode.rxModType="8PSK";
        p2pmode.rxFECRate="3/4";
        break;
    }
    p2pmode.txFrequence = m_para.m_TxFreq;// + m_para.m_BUCfreq;
    p2pmode.txBitrate = m_para.m_txRate*1000;
    p2pmode.txIFPower = 0.01 * m_para.m_power100;
    p2pmode.rxFrequence = m_para.m_RxFreq;// + m_para.m_LNBfreq;
    p2pmode.rxBitrate = m_para.m_rxRate*1000;
    if(m_para.m_devMode==objPara::DevMode_bridge){
        p2pmode.dataCommMode="bridge";
    }
    else{
        p2pmode.dataCommMode="route";
    }
    stdstr=m_cu.enterP2PMode(p2pmode);
    writeTcp(QString::fromStdString(stdstr).toUtf8());

    QTimer::singleShot(500,this,SLOT(slotGetP2Pstatus()));
}
void objui::doDisconnectP2P()
{
    std::string stdstr;
    stdstr=m_cu.exitP2PMode();
    writeTcp(QString::fromStdString(stdstr).toUtf8());
}
void objui::doLogin()
{
    std::string stdstr;
    stdstr=m_cu.loginNet();
    writeTcp(QString::fromStdString(stdstr).toUtf8());
    slotStateTransitionLeft();
}
void objui::doLogout()
{
    std::string stdstr;
    stdstr=m_cu.logoutNet();
    writeTcp(QString::fromStdString(stdstr).toUtf8());
    slotStateTransitionRight();
}

void objui::doMenuCall()
{
    if(m_para.m_status==objPara::Status_idle){
        if(m_para.m_workMode==objPara::Mode_p2p){
            doCallP2P();
            slotStateTransitionUp();// ==> calling
        }
        else{// central   login
            doLogin();
        }

    }
    else{//  connected ...
        if(m_status.m_workMode == objPara::Mode_p2p){
            doDisconnectP2P();
            //m_para.m_status = objPara::Status_disconnecting;
            slotStateTransitionDown();  // ==> call end
        }
        else{
            doLogout();
        }
    }

#if 0
    switch(m_para.m_workMode){
    case objPara::Mode_p2p:
        switch (m_para.m_status) {
        case objPara::Status_connected:
            doDisconnectP2P();
            //m_para.m_status = objPara::Status_disconnecting;
            slotStateTransitionDown();  // ==> call end
            break;
        case objPara::Status_idle:
            //m_para.m_status = objPara::Status_connecting;
            doCallP2P();
            slotStateTransitionUp();// ==> calling
            break;
        default:
            break;
        }
        break;
    case objPara::Mode_central:
        break;
    default:
        break;
    }
#endif
#if 0
    if(work_mode_central == m_workMode){
    switch(m_nSelectMenu10){
    case 0:
        m_bStatConnect=true;
        break;
    case 1:
        m_bStatConnect = false;
        break;
    case 2:
        if(!m_bStatLogin){
            qDebug(" do login");
            stdstr=m_cu.loginNet();
            writeTcp(QString::fromStdString(stdstr).toUtf8());
            m_bStatLogin=true;break;
        }
    case 3:
        if(m_bStatLogin){
            qDebug(" do logout");
            stdstr=m_cu.logoutNet();
            writeTcp(QString::fromStdString(stdstr).toUtf8());
            m_bStatLogin=false;
            m_bStatConnect=false;
        }
        break;
    default:
        break;
    }
    setSelectMenu10();
    slotShowMenuCall();
    }
#endif
}

//
void objui::slotShowMenu11()
{
    zeroFB(0);

    //centerXY(QString("hujiao/guaduan   挂断    呼叫"),0,0,256,16,1,1);
    centerXY(QString("呼    叫"),0,0,256,64,1,1);
    //centerXY(QString("挂    断"),0,0,256,64,1,1);

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();
    QTimer::singleShot(200,this,SLOT(slotKeyEnable()));
    //m_bEnableKeyboard = true;

}
//
void objui::slotShowMenu12()
{
    zeroFB(0);

    //centerXY(QString("hujiao/guaduan   挂断    呼叫"),0,0,256,16,1,1);
    centerXY(QString("呼    叫"),0,0,256,64,1,1);
    //centerXY(QString("挂    断"),0,0,256,64,1,1);

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();
    QTimer::singleShot(200,this,SLOT(slotKeyEnable()));
    //m_bEnableKeyboard = true;

}
void objui::showDataParaPage1()
{
    char buf[40];
    //const QLocale & locale = QLocale::c();
    //QString s;//=locale.toString(m_numEditor.m_i64);

    //s=locale.toString(m_para.m_TxFreq).replace(',',' ') + " Hz";
    sprintf(buf,"%.4f MHz",0.000001*m_para.m_TxFreq);
    centerXY(buf,16*4,11,256-16*4,16,1,1,0x0f,0);
    sprintf(buf,"%.4f MHz",0.000001*m_para.m_RxFreq);
    centerXY(buf,16*4,38,256-16*4,16,1,1,0x0f,0);
#if 0
    sprintf(buf,"%d kbps",m_para.m_TxRate);
    centerXY(buf,16*4,32,256-16*4,16,1,1,0x0f,0);
    sprintf(buf,"%d kbps",m_para.m_RxRate);
    centerXY(buf,16*4,48,256-16*4,16,1,1,0x0f,0);
#endif
    strXY("1/4",256-8*3,48,0x0f,0);

    strXY("中频发送",0,11);// fasong pindian 发送频率  频率。频点
    strXY("中频接收",0,38);// jieshou pindian 发送频率  频率。频点 接收

}
void objui::showDataParaPage1a()
{
    char buf[40];
    //const QLocale & locale = QLocale::c();
    //QString s;//=locale.toString(m_numEditor.m_i64);

    //s=locale.toString(m_para.m_TxFreq).replace(',',' ') + " Hz";
    sprintf(buf,"%.4f MHz",0.000001*m_para.m_TxFreq);
    centerXY(buf,16*4,0+11,256-16*4,16,1,1,0x0f,0);
    sprintf(buf,"%.4f MHz",0.000001*m_para.m_RxFreq);
    centerXY(buf,16*4,48-11,256-16*4,16,1,1,0x0f,0);
#if 0
    sprintf(buf,"%d kbps",m_para.m_TxRate);
    centerXY(buf,16*4,32,256-16*4,16,1,1,0x0f,0);
    sprintf(buf,"%d kbps",m_para.m_RxRate);
    centerXY(buf,16*4,48,256-16*4,16,1,1,0x0f,0);
#endif
    strXY("1/3",256-8*3,48,0x0f,0);

    strXY("中频发送",0,0+11);// fasong pindian 发送频率  频率。频点
    strXY("中频接收",0,48-11);// jieshou pindian 发送频率  频率。频点 接收
    //strXY("BUC本振",0,32);//
    //strXY("LNB本振",0,48);//

    //sprintf(buf,"%.4f MHz",0.000001*m_para.m_BUCfreq);
    //centerXY(buf,16*4,32,256-16*4,16,1,1,0x0f,0);
    //sprintf(buf,"%.4f MHz",0.000001*m_para.m_LNBfreq);
    //centerXY(buf,16*4,48,256-16*4,16,1,1,0x0f,0);

}

void objui::slotShowParaPage1a()
{
    zeroFB(0);

    m_numEditor.setNum64(m_para.m_TxFreq,m_para.m_maxTxFreq,m_para.m_minTxFreq,-1,2,-1);

    showDataParaPage1a();
    strXY("中频发送",0,0+11,0,0x0f);// fasong pindian 发送频率  频率。频点
    //strXY("中频接收",0,38,0x0f,0);// jieshou pindian 发送频率  频率。频点 接收

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();
    QTimer::singleShot(200,this,SLOT(slotKeyEnable()));

}
void objui::slotShowParaPage11a()
{
    zeroFB(0);

    m_numEditor.setNum64(m_para.m_RxFreq,m_para.m_maxRxFreq,m_para.m_minRxFreq,-1,2,-1);

    showDataParaPage1a();
    //strXY("中频发送",0,0,0x0f,0);// fasong pindian 发送频率  频率。频点
    strXY("中频接收",0,48-11,0,0x0f);// jieshou pindian 发送频率  频率。频点 接收

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();
    QTimer::singleShot(200,this,SLOT(slotKeyEnable()));

}
// 12a ==> menuBUCfreq
void objui::slotShowParaPage12a()
{
    zeroFB(0);

    m_numEditor.setNum64(m_para.m_BUCfreq,m_para.m_maxTDMfreq,m_para.m_minTDMfreq,-1,2,-2);

    showDataParaPage1a();
    strXY("BUC本振",0,32,0,0x0f);//

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();
    QTimer::singleShot(200,this,SLOT(slotKeyEnable()));

}
void objui::slotShowParaPage13a()
{
    zeroFB(0);

    m_numEditor.setNum64(m_para.m_LNBfreq,m_para.m_maxTDMfreq,m_para.m_minTDMfreq,-1,2,-2);

    showDataParaPage1a();
    strXY("LNB本振",0,48,0,0x0f);//

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();
    QTimer::singleShot(200,this,SLOT(slotKeyEnable()));

}

void objui::slotShowParaPage1()
{
    //qDebug("     show menu.page.1");

    zeroFB(0);

    m_numEditor.setNum64(m_para.m_TxFreq,m_para.m_maxTxFreq,m_para.m_minTxFreq,-1,2,-1);

    showDataParaPage1();
    strXY("中频发送",0,11,0,0x0f);// fasong pindian 发送频率  频率。频点
    //strXY("中频接收",0,38,0x0f,0);// jieshou pindian 发送频率  频率。频点 接收
    //strXY("发送速率",0,32,0x0f,0);// fasong sulv 发送频率  频率。频点  速率
    //strXY("接收速率",0,48,0x0f,0);// jieshou sulv 发送频率  频率。频点

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();
    //m_bEnableKeyboard = true;
    QTimer::singleShot(200,this,SLOT(slotKeyEnable()));


}
void objui::slotShowParaPage11()
{
    //qDebug("     show menu.page.11");

    zeroFB(0);

    m_numEditor.setNum64(m_para.m_RxFreq,m_para.m_maxRxFreq,m_para.m_minRxFreq,-1,2,-1);

    showDataParaPage1();
    //strXY("中频发送",0,0,0x0f,0);// fasong pindian 发送频率  频率。频点
    strXY("中频接收",0,38,0,0x0f);// jieshou pindian 发送频率  频率。频点 接收
    //strXY("发送速率",0,32,0x0f,0);// fasong sulv 发送频率  频率。频点  速率
    //strXY("接收速率",0,48,0x0f,0);// jieshou sulv 发送频率  频率。频点

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();
    //m_bEnableKeyboard = true;
    QTimer::singleShot(200,this,SLOT(slotKeyEnable()));


}
#if 0
void objui::slotShowParaPage12()
{
    //qDebug("     show menu.page.12");
    zeroFB(0);

    m_numEditor.setIdxRate(m_para.m_TxRate);

    strXY("中频发送",0,0,0x0f,0);// fasong pindian 发送频率  频率。频点
    strXY("中频接收",0,16,0x0f,0);// jieshou pindian 发送频率  频率。频点 接收
    strXY("发送速率",0,32,0,0x0f);// fasong sulv 发送频率  频率。频点  速率
    strXY("接收速率",0,48,0x0f,0);// jieshou sulv 发送频率  频率。频点

    showDataParaPage1();

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();
    //m_bEnableKeyboard = true;
    QTimer::singleShot(200,this,SLOT(slotKeyEnable()));


}

void objui::slotShowParaPage13()
{
    //qDebug("     show menu.page.13");
    zeroFB(0);

    m_numEditor.setIdxRate(m_para.m_RxRate);

    strXY("中频发送",0,0,0x0f,0);// fasong pindian 发送频率  频率。频点
    strXY("中频接收",0,16,0x0f,0);// jieshou pindian 发送频率  频率。频点 接收
    strXY("发送速率",0,32,0x0f,0);// fasong sulv 发送频率  频率。频点  速率
    strXY("接收速率",0,48,0,0x0f);// jieshou sulv 发送频率  频率。频点

    showDataParaPage1();

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();
    //m_bEnableKeyboard = true;
    QTimer::singleShot(200,this,SLOT(slotKeyEnable()));


}
#endif
void objui::showDataParaPage2()
{
    strXY("发送功率",0,0+11);// fasong gonglv 发送 功率
    strXY("业务类型",0,48-11);// yewu leixing 业务类型
    //strXY("BUC本振",0,32);//
    //strXY("LNB本振",0,48);// bianma fangshi 编码方式

    char buf[40];
    //const QLocale & locale = QLocale::c();
    //QString s;//=locale.toString(m_numEditor.m_i64);

    //s=locale.toString(0.01*m_para.m_power100) + " dBm";
    sprintf(buf,"%.2f dBm",0.01*m_para.m_power100);
    centerXY(buf,4*16,0+11,256-16*4,16,1,1,0x0f,0);
    switch(m_para.m_devMode){
    case objPara::DevMode_bridge:
        centerXY("网桥",4*16,48-11,256-16*4,16,1,1,0x0f,0);// wangqiao  网桥
        break;
    case objPara::DevMode_router:
        centerXY("路由",4*16,48-11,256-16*4,16,1,1,0x0f,0);// wangqiao  网桥
        break;
    default:
        break;
    }
    //sprintf(buf,"%.4f MHz",0.000001*m_para.m_BUCfreq);
    //centerXY(buf,16*4,32,256-16*4,16,1,1,0x0f,0);
    //sprintf(buf,"%.4f MHz",0.000001*m_para.m_LNBfreq);
    //centerXY(buf,16*4,48,256-16*4,16,1,1,0x0f,0);

    //centerXY("QPSK1/2",4*16,32,256-16*4,16,1,1,0x0f,0);

    strXY("3/3",256-8*3,48,0x0f,0);

}
void objui::showDataParaPage2c()
{
    char buf[20];

    //const QLocale & locale = QLocale::c();
    //QString s;//=locale.toString(m_numEditor.m_i64);

    //s=locale.toString(0.01*m_para.m_power100Central) + " dBm";
    sprintf(buf,"%.2f dBm",0.01*m_para.m_power100Central);
    centerXY(buf,4*16,0,256-16*4,16,1,1,0x0f,0);
    switch(m_para.m_devMode){
    case objPara::DevMode_bridge:
        centerXY("网桥",4*16,16,256-16*4,16,1,1,0x0f,0);// wangqiao  网桥
        break;
    case objPara::DevMode_router:
        centerXY("路由",4*16,16,256-16*4,16,1,1,0x0f,0);// wangqiao  网桥
        break;
    default:
        break;
    }
    centerXY("QPSK1/2",4*16,32,256-16*4,16,1,1,0x0f,0);

    strXY("2/2",256-8*3,48,0x0f,0);

}

void objui::slotShowParaPage2()
{
    zeroFB(0);

    m_numEditor.setNum64(m_para.m_power100,m_para.m_maxPower,m_para.m_minPower,-1,0,0);

    showDataParaPage2();
    strXY("发送功率",0,0+11,0,0x0f);// fasong gonglv 发送 功率

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();
    //m_bEnableKeyboard = true;
    QTimer::singleShot(200,this,SLOT(slotKeyEnable()));


}
void objui::slotShowParaPage21()
{
    zeroFB(0);

    showDataParaPage2();
    strXY("业务类型",0,48-11,0,0x0f);// yewu leixing 业务类型

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();
    //m_bEnableKeyboard = true;
    QTimer::singleShot(200,this,SLOT(slotKeyEnable()));


}
void objui::slotShowParaPage22()
{
    zeroFB(0);

    m_numEditor.setNum64(m_para.m_BUCfreq,m_para.m_maxTDMfreq,m_para.m_minTDMfreq,-1,2,-2);

    showDataParaPage2();
    strXY("BUC本振",0,32,0,0x0f);//

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();
    //m_bEnableKeyboard = true;
    QTimer::singleShot(200,this,SLOT(slotKeyEnable()));


}
void objui::slotShowParaPage23()
{
    zeroFB(0);

    m_numEditor.setNum64(m_para.m_LNBfreq,m_para.m_maxTDMfreq,m_para.m_minTDMfreq,-1,2,-2);

    showDataParaPage2();
    strXY("LNB本振",0,48,0,0x0f);//

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();
    //m_bEnableKeyboard = true;
    QTimer::singleShot(200,this,SLOT(slotKeyEnable()));


}
void objui::slotShowEditBUCfreq()
{
    char buf[40];
    zeroFB(0);

    centerXY("BUC本振频率",0,10,256,16,1,1,0x0f,0);
    m_numEditor.getStrFreq(buf);
    centerXY(buf,0,64-12-16,(9+9)<<3,16,2,1,0x0f,0);
    strXY("  MHz",(9+9)<<3,64-12-16,0x0f,0);
    underLine(0,64-12-16,9+9-1-m_numEditor.getCursorMHz(),0x0f);

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();
    //m_bEnableKeyboard = true;
    QTimer::singleShot(200,this,SLOT(slotKeyEnable()));


}
void objui::slotShowEditLNBfreq()
{
    char buf[40];
    zeroFB(0);

    centerXY("LNB本振频率",0,10,256,16,1,1,0x0f,0);
    m_numEditor.getStrFreq(buf);
    centerXY(buf,0,64-12-16,(9+9)<<3,16,2,1,0x0f,0);
    strXY("  MHz",(9+9)<<3,64-12-16,0x0f,0);
    underLine(0,64-12-16,9+9-1-m_numEditor.getCursorMHz(),0x0f);

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();
    //m_bEnableKeyboard = true;
    QTimer::singleShot(200,this,SLOT(slotKeyEnable()));


}

void objui::slotShowEditTxFreq()
{
    char buf[40];
    zeroFB(0);

    centerXY("中 频 发 送 频 点",0,10,256,16,1,1,0x0f,0);
    m_numEditor.getStrFreq(buf);
    centerXY(buf,0,64-12-16,(9+9)<<3,16,2,1,0x0f,0);
    strXY("  MHz",(9+9)<<3,64-12-16,0x0f,0);
    underLine(0,64-12-16,9+9-1-m_numEditor.getCursorMHz(),0x0f);

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();
    //m_bEnableKeyboard = true;
    QTimer::singleShot(200,this,SLOT(slotKeyEnable()));


}
void objui::slotShowEditRxFreq()
{
    char buf[40];
    zeroFB(0);

    centerXY("中 频 接 收 频 点",0,10,256,16,1,1,0x0f,0);
    m_numEditor.getStrFreq(buf);
    centerXY(buf,0,64-12-16,(9+9)<<3,16,2,1,0x0f,0);
    strXY("  MHz",(9+9)<<3,64-12-16,0x0f,0);
    underLine(0,64-12-16,9+9-1-m_numEditor.getCursorMHz(),0x0f);

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();
    //m_bEnableKeyboard = true;
    QTimer::singleShot(200,this,SLOT(slotKeyEnable()));


}
// fix array
void objui::slotShowEditTxRateC()
{
    char buf[40];
    zeroFB(0);

    centerXY("发 送 速 率",0,10,256,16,1,1,0x0f,0);
    sprintf(buf,"%d kbps",m_editorRate.getRate());
    centerXY(buf,0,64-12-16,256,16,1,1,0x0f,0);

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();
    slotKeyEnable();
}

void objui::slotShowEditRxRateC()
{
    char buf[40];
    zeroFB(0);

    centerXY("接 收 速 率",0,10,256,16,1,1,0x0f,0);
    sprintf(buf,"%d kbps",m_editorRate.getRate());
    centerXY(buf,0,64-12-16,256,16,1,1,0x0f,0);

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();
    slotKeyEnable();
}
void objui::slotShowEditTxRate()
{
    char buf[40];
    zeroFB(0);

    centerXY("发 送 速 率",0,10,256,16,1,1,0x0f,0);
    sprintf(buf,"%d kbps",m_numEditor.m_ArrayRate[m_numEditor.m_nIdxRate]);
    centerXY(buf,0,64-12-16,256,16,1,1,0x0f,0);

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();
    //m_bEnableKeyboard = true;
    QTimer::singleShot(200,this,SLOT(slotKeyEnable()));


}
void objui::slotShowEditRxRate()
{
    char buf[40];
    zeroFB(0);

    centerXY("接 收 速 率",0,10,256,16,1,1,0x0f,0);
    sprintf(buf,"%d kbps",m_numEditor.m_ArrayRate[m_numEditor.m_nIdxRate]);
    centerXY(buf,0,64-12-16,256,16,1,1,0x0f,0);

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();
    //m_bEnableKeyboard = true;
    QTimer::singleShot(200,this,SLOT(slotKeyEnable()));


}
void objui::slotShowEditTxRatePSK()
{
    char buf[40];
    zeroFB(0);

    centerXY("发 送 速 率",0,10,256,16,1,1,0x0f,0);
    sprintf(buf,"%d kbps",m_numEditor.m_num);
    centerXY(buf,0,64-12-16,256,16,1,1,0x0f,0);

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();
    QTimer::singleShot(200,this,SLOT(slotKeyEnable()));


}
void objui::slotShowEditRxRatePSK()
{
    char buf[40];
    zeroFB(0);

    centerXY("接 收 速 率",0,10,256,16,1,1,0x0f,0);
    sprintf(buf,"%d kbps",m_numEditor.m_num);
    centerXY(buf,0,64-12-16,256,16,1,1,0x0f,0);

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();
    QTimer::singleShot(200,this,SLOT(slotKeyEnable()));


}
void objui::slotShowEditPower()
{
    zeroFB(0);
    char buf[20];

    const QLocale & locale = QLocale::c();
    QString s=locale.toString(0.01 * m_numEditor.m_i64);
    sprintf(buf,"%.2f",0.01*m_numEditor.m_i64);

    centerXY("发 送 功 率",0,10,256,16,1,1,0x0f,0);
    centerXY(buf,0,64-12-16,(11+6)<<3,16,2,1,0x0f,0);
    strXY(" dBm",(11+6)<<3,64-12-16,0x0f,0);
    underLine(0,64-12-16,11+6-1-m_numEditor.getCursor(),0x0f);

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();
    //m_bEnableKeyboard = true;
    QTimer::singleShot(200,this,SLOT(slotKeyEnable()));


}

void objui::slotShowEditTDM()
{
    char buf[40];
    zeroFB(0);

    centerXY("TDM频点1",0,10,256,16,1,1,0x0f,0);
    m_numEditor.getStrTDM(buf);
    centerXY(buf,0,64-12-16,(9+10)<<3,16,2,1,0x0f,0);
    strXY(" MHz",(9+10)<<3,64-12-16,0x0f,0);
    underLine(0,64-12-16,9+10-1-m_numEditor.getCursorMHz(),0x0f);

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();
    //m_bEnableKeyboard = true;
    QTimer::singleShot(200,this,SLOT(slotKeyEnable()));


}
void objui::slotShowEditTDM2()
{
    char buf[40];
    zeroFB(0);

    centerXY("TDM频点2",0,10,256,16,1,1,0x0f,0);
    m_numEditor.getStrTDM(buf);
    centerXY(buf,0,64-12-16,(9+10)<<3,16,2,1,0x0f,0);
    strXY(" MHz",(9+10)<<3,64-12-16,0x0f,0);
    underLine(0,64-12-16,9+10-1-m_numEditor.getCursorMHz(),0x0f);

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();
    //m_bEnableKeyboard = true;
    QTimer::singleShot(200,this,SLOT(slotKeyEnable()));


}
void objui::slotShowEditNumber()
{
    zeroFB(0);
    char buf[20];
    sprintf(buf,"%08lld",m_numEditor.m_i64);

    centerXY("被 叫 号 码",0,10,256,16,1,1,0x0f,0);
    centerXY(QString(buf).insert(4,'-').toLatin1().data(),0,64-12-16,256,16,1,1,0x0f,0);
    underLine(88+4+64,64-12-16,0-m_numEditor.strGetCursor(),0x0f);

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();
    //m_bEnableKeyboard = true;
    QTimer::singleShot(200,this,SLOT(slotKeyEnable()));


}
#if 0
void objui::slotShowEditCallID()
{
    zeroFB(0);
    char buf[20];
    sprintf(buf,"%08lld",m_editorCallID.m_i64);

    centerXY("呼 叫 号 码",0,10,256,16,1,1,0x0f,0);
    centerXY(QString(buf).insert(4,'-').toLatin1().data(),0,64-12-16,256,16,1,1,0x0f,0);
    underLine(11*8+4,64-12-16,m_editorCallID.getCursor(),0x0f);

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();
    //m_bEnableKeyboard = true;
    QTimer::singleShot(200,this,SLOT(slotKeyEnable()));


}
#endif
void objui::slotShowEditIDstr()
{
    QString str;
    zeroFB(0);
    char buf[20];
    str = m_editorIDstr.getStrFormat();
    sprintf(buf,"%s",str.toLatin1().data());
    int x = (256-(str.size()<<3))>>1;

    centerXY("呼 叫 号 码",0,10,256,16,1,1,0x0f,0);
    centerXY(str.toLatin1().data(),0,64-12-16,256,16,1,1,0x0f,0);
    underLine(x,64-12-16,m_editorIDstr.getCursor(),0x0f);

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();
    //m_bEnableKeyboard = true;
    QTimer::singleShot(200,this,SLOT(slotKeyEnable()));


}
#if 0
void objui::slotShowEditTxRateCentral()
{
    zeroFB(0);

    const QLocale & locale = QLocale::c();
    QString s=locale.toString(m_numEditor.m_i64);

    centerXY("发 送 速 率",0,10,256,16,1,1,0x0f,0);
    centerXY(s.replace(',',' ').toLatin1().data(),0,64-12-16,(11+5)<<3,16,2,1,0x0f,0);  // 32-8=24 -10 7    (8+7+6)
    strXY("  kHz",(11+5)<<3,64-12-16,0x0f,0);
    underLine(0,64-12-16,(11+5)-1-m_numEditor.getCursor(),0x0f);

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();
    //m_bEnableKeyboard = true;
    QTimer::singleShot(200,this,SLOT(slotKeyEnable()));


}
void objui::slotShowEditRxRateCentral()
{
    zeroFB(0);

    const QLocale & locale = QLocale::c();
    QString s=locale.toString(m_numEditor.m_i64);

    centerXY("接 收 速 率",0,10,256,16,1,1,0x0f,0);
    centerXY(s.replace(',',' ').toLatin1().data(),0,64-12-16,(11+5)<<3,16,2,1,0x0f,0);
    strXY("  kHz",(11+5)<<3,64-12-16,0x0f,0);
    underLine(0,64-12-16,11+5-1-m_numEditor.getCursor(),0x0f);

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();
    //m_bEnableKeyboard = true;
    QTimer::singleShot(200,this,SLOT(slotKeyEnable()));


}
#endif
void objui::slotShowEditPowerCentral()
{
    zeroFB(0);
    char buf[20];

    sprintf(buf,"%lld dB",m_numEditor.m_i64);

    centerXY("功 率 补 偿",0,10,256,16,1,1,0x0f,0);
    centerXY(buf,0,64-12-16,256,16,1,1,0x0f,0);

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();
    //m_bEnableKeyboard = true;
    QTimer::singleShot(200,this,SLOT(slotKeyEnable()));


}
void objui::showDataParaP1c()
{
    char buf[20];

    strXY("TDM频点1",0,0,0x0f,0);// fasong pindian 发送频率  频率。频点
    strXY("TDM频点2",0,16,0x0f,0);// jieshou pindian 发送频率  频率。频点 接收 被叫号码
    strXY("功率补偿",0,32,0x0f,0);// fasong sulv 发送频率  频率。频点  速率
    strXY("呼叫号码",0,48,0x0f,0);//

    sprintf(buf,"%.4f MHz",0.000001 * m_para.m_TDMfreq1);
    centerXY(buf,16*4,0,256-16*4,16,1,1,0x0f,0);
    sprintf(buf,"%.4f MHz",0.000001 * m_para.m_TDMfreq2);
    centerXY(buf,16*4,16,256-16*4,16,1,1,0x0f,0);

    sprintf(buf,"%lld dB",m_para.m_power100Central);
    centerXY(buf,16*4,32,256-16*4,16,1,1,0x0f,0);

    sprintf(buf,"%s",m_para.m_strCallID.toLatin1().data());
    centerXY(buf,16*4,48,256-16*4,16,1,1,0x0f,0);

    strXY("1/2",256-8*3,48,0x0f,0);
}

void objui::showDataParaPage1c()
{
    char buf[20];

    sprintf(buf,"%.4f MHz",0.000001 * m_para.m_TDMfreq1);
    centerXY(buf,16*4,4,256-16*4,16,1,1,0x0f,0);
    sprintf(buf,"%.4f MHz",0.000001 * m_para.m_TDMfreq2);
    centerXY(buf,16*4,16+8,256-16*4,16,1,1,0x0f,0);

    sprintf(buf,"%lld dB",m_para.m_power100Central);
    centerXY(buf,16*4,32+12,256-16*4,16,1,1,0x0f,0);

    //strXY("1/2",256-8*3,48,0x0f,0);
}
// TDM1
void objui::slotShowParaP11c()
{
    zeroFB(0);

    m_numEditor.setNum64(m_para.m_TDMfreq1,m_para.m_maxTDMfreq,m_para.m_minTDMfreq,-1,2,-1);

    showDataParaP1c();
    strXY("TDM频点1",0,0,0,0x0f);// fasong pindian 发送频率  频率。频点
    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);
    emit sigFlush();
    QTimer::singleShot(200,this,SLOT(slotKeyEnable()));
}

void objui::slotShowParaPage1c()
{
    //qDebug("     show menu.page.1c");

    zeroFB(0);

    m_numEditor.setNum64(m_para.m_TDMfreq1,m_para.m_maxTDMfreq,m_para.m_minTDMfreq,-1,2,-1);

    strXY("TDM频点1",0,4,0,0x0f);// fasong pindian 发送频率  频率。频点
    strXY("TDM频点2",0,16+8,0x0f,0);// jieshou pindian 发送频率  频率。频点 接收 被叫号码
    strXY("功率补偿",0,32+12,0x0f,0);// fasong sulv 发送频率  频率。频点  速率

    showDataParaPage1c();

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();
    //m_bEnableKeyboard = true;
    QTimer::singleShot(200,this,SLOT(slotKeyEnable()));


}
void objui::slotShowParaP12c()
{
    zeroFB(0);

    m_numEditor.setNum64(m_para.m_TDMfreq2,m_para.m_maxTDMfreq,m_para.m_minTDMfreq,-1,2,-1);

    showDataParaP1c();
    strXY("TDM频点2",0,16,0,0x0f);//

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);
    emit sigFlush();
    QTimer::singleShot(200,this,SLOT(slotKeyEnable()));
}
void objui::slotShowParaPage11c()
{
    //qDebug("     show menu.page.11");

    zeroFB(0);

    m_numEditor.setNum64(m_para.m_TDMfreq2,m_para.m_maxTDMfreq,m_para.m_minTDMfreq,-1,2,-1);

    strXY("TDM频点1",0,4,0x0f,0);// fasong pindian 发送频率  频率。频点
    strXY("TDM频点2",0,16+8,0,0x0f);// jieshou pindian 发送频率  频率。频点 接收 被叫号码
    strXY("功率补偿",0,32+12,0x0f,0);// fasong sulv 发送频率  频率。频点  速率

    showDataParaPage1c();

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();
    //m_bEnableKeyboard = true;
    QTimer::singleShot(200,this,SLOT(slotKeyEnable()));


}
// power   <==  slotShowParaPage12c
void objui::slotShowParaP13c()
{
    zeroFB(0);

    m_numEditor.setNum64(m_para.m_power100Central,m_para.m_maxPowerCentral,m_para.m_minPowerCentral,-1,0,0);

    showDataParaP1c();
    strXY("功率补偿",0,32,0,0x0f);// fasong sulv 发送频率  频率。频点  速率

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);
    emit sigFlush();
    QTimer::singleShot(200,this,SLOT(slotKeyEnable()));
}

// callID ,
void objui::slotShowParaP14c()
{
    zeroFB(0);

    //m_editorCallID.setNumber8(123456);
    m_editorIDstr.setStr(m_para.m_strCallID);

    showDataParaP1c();
    strXY("呼叫号码",0,48,0,0x0f);//

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);
    emit sigFlush();
    QTimer::singleShot(200,this,SLOT(slotKeyEnable()));
}


void objui::slotShowParaPage12c()
{
    zeroFB(0);

    m_numEditor.setNum64(m_para.m_power100Central,m_para.m_maxPowerCentral,m_para.m_minPowerCentral,-1,0,0);

    strXY("TDM频点1",0,4,0x0f,0);// fasong pindian 发送频率  频率。频点
    strXY("TDM频点2",0,16+8,0x0f,0);// jieshou pindian 发送频率  频率。频点 接收 被叫号码
    strXY("功率补偿",0,32+12,0,0x0f);// fasong sulv 发送频率  频率。频点  速率

    showDataParaPage1c();

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();
    //m_bEnableKeyboard = true;
    QTimer::singleShot(200,this,SLOT(slotKeyEnable()));


}
#if 0
void objui::slotShowParaPage13c()
{
    //qDebug("     show menu.page.13");
    zeroFB(0);

    m_numEditor.setNum64(m_para.m_RxRateCentral,m_para.m_maxRxRateCentral,m_para.m_minRxRateCentral,-1,0,0);

    strXY("TDM 频点",0,0,0x0f,0);// fasong pindian 发送频率  频率。频点
    strXY("被叫号码",0,16,0x0f,0);// jieshou pindian 发送频率  频率。频点 接收
    strXY("发送速率",0,32,0x0f,0);// fasong sulv 发送频率  频率。频点  速率
    strXY("接收速率",0,48,0,0x0f);// jieshou sulv 发送频率  频率。频点

    showDataParaPage1c();

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();
    //m_bEnableKeyboard = true;
    QTimer::singleShot(200,this,SLOT(slotKeyEnable()));


}
#endif
void objui::showDataParaP2c()
{
    char buf[40];
    zeroFB(0);

    strXY("发送速率",0,0);// fasong sulv 发送频率  频率。频点  速率
    strXY("接收速率",0,16);// jieshou sulv 发送频率  频率。频点
    strXY("业务类型",0,32);// yewu leixing 业务类型

    sprintf(buf,"%d kbps",m_para.m_txRateC);
    centerXY(buf,16*4,0,256-16*4,16,1,1);
    sprintf(buf,"%d kbps",m_para.m_rxRateC);
    centerXY(buf,16*4,16,256-16*4,16,1,1);

    switch(m_para.m_devMode){
    case objPara::DevMode_bridge:
        centerXY("网桥",4*16,32,256-16*4,16,1,1,0x0f,0);// wangqiao  网桥
        break;
    case objPara::DevMode_router:
        centerXY("路由",4*16,32,256-16*4,16,1,1,0x0f,0);// wangqiao  网桥
        break;
    default:
        break;
    }

    strXY("2/2",256-8*3,48,0x0f,0);

}

void objui::slotShowParaP21c()
{
    //qDebug("   tx.rate:%d",m_para.m_TxRate);
    m_para.m_txRateC = m_editorRate.setRate(m_para.m_txRateC);
    //qDebug("    after.set  tx.rate:%d",m_para.m_TxRate);

    showDataParaP2c();
    strXY("发送速率",0,0,0,0x0f);// fasong sulv 发送频率  频率。频点  速率
    //strXY("接收速率",0,16,0,0x0f);// jieshou sulv 发送频率  频率。频点
    //strXY("业务类型",0,32,0x0f,0);// yewu leixing 业务类型

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();
    QTimer::singleShot(200,this,SLOT(slotKeyEnable()));

}
void objui::slotShowParaP22c()
{
    m_para.m_rxRateC = m_editorRate.setRate(m_para.m_rxRateC);

    showDataParaP2c();
    //strXY("发送速率",0,0,0x0f,0);// fasong sulv 发送频率  频率。频点  速率
    strXY("接收速率",0,16,0,0x0f);// jieshou sulv 发送频率  频率。频点
    //strXY("业务类型",0,32,0x0f,0);// yewu leixing 业务类型

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();
    QTimer::singleShot(200,this,SLOT(slotKeyEnable()));

}
void objui::slotShowParaP23c()
{
    m_numEditor.setNum64(m_para.m_power100Central,m_para.m_maxPowerCentral,m_para.m_minPowerCentral,-1,0,0);

    showDataParaP2c();
    //strXY("发送速率",0,0,0x0f,0);// fasong sulv 发送频率  频率。频点  速率
    //strXY("接收速率",0,16,0,0x0f);// jieshou sulv 发送频率  频率。频点
    strXY("业务类型",0,32,0,0x0f);// yewu leixing 业务类型

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();
    QTimer::singleShot(200,this,SLOT(slotKeyEnable()));

}

void objui::slotShowParaPage2c()
{
    zeroFB(0);

    m_numEditor.setNum64(m_para.m_power100Central,m_para.m_maxPowerCentral,m_para.m_minPowerCentral,-1,0,0);

    strXY("发送功率",0,0,0,0x0f);// fasong gonglv 发送 功率
    strXY("业务类型",0,16,0x0f,0);// yewu leixing 业务类型
    strXY("编码方式",0,32,0x0f,0);// bianma fangshi 编码方式

    showDataParaPage2c();

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();
    //m_bEnableKeyboard = true;
    QTimer::singleShot(200,this,SLOT(slotKeyEnable()));


}
void objui::slotShowParaPage21c()
{
    zeroFB(0);

    strXY("发送功率",0,0,0x0f,0);// fasong gonglv 发送 功率
    strXY("业务类型",0,16,0,0x0f);// yewu leixing 业务类型
    strXY("编码方式",0,32,0x0f,0);// bianma fangshi 编码方式

    showDataParaPage2c();

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();
    //m_bEnableKeyboard = true;
    QTimer::singleShot(200,this,SLOT(slotKeyEnable()));


}
void objui::slotShowParaPage22c()
{
    zeroFB(0);

    strXY("发送功率",0,0,0x0f,0);// fasong gonglv 发送 功率
    strXY("业务类型",0,16,0x0f,0);// yewu leixing 业务类型
    strXY("编码方式",0,32,0,0x0f);// bianma fangshi 编码方式

    showDataParaPage2c();

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();
    //m_bEnableKeyboard = true;
    QTimer::singleShot(200,this,SLOT(slotKeyEnable()));


}
QString objui::getTimeSpan()
{
    qint64 secs=QDateTime::currentDateTime().toTime_t() - m_para.m_startSecs;
    int days=secs/(24*60*60);
    QTime t=QTime(0,0).addSecs(secs%(24*60*60));
    if(days>0) return QString("%1 天" ).arg(days) + t.toString(" hh : mm : ss");//.arg(t.hour()).arg((t.minute())).arg(t.second());
    else return t.toString("hh : mm : ss");//QString("%1:%2:%3").arg(t.hour()).arg((t.minute())).arg(t.second());

}
QString objui::getTimeRunning()
{
    qint64 secs=QDateTime::currentDateTime().toTime_t() - m_secs0;
    int days=secs/(24*60*60);
    QTime t=QTime(0,0).addSecs(secs%(24*60*60));
    if(days>0) return QString("%1天" ).arg(days) + t.toString("hh:mm:ss");//.arg(t.hour()).arg((t.minute())).arg(t.second());
    else return t.toString("hh:mm:ss");//QString("%1:%2:%3").arg(t.hour()).arg((t.minute())).arg(t.second());

}

void objui::showStatusPage1c()
{
    char buf[40];
    strXY("发送速率:  -- k",128,32);
    strXY("接收速率:  -- k",128,48);
    QString str="--";
    switch(m_para.m_status){
    case objPara::Status_waiting_tdm:
        str="等待网控广播";
        break;
    case objPara::Status_logining:
        str="正在入网";
        break;
    case objPara::Status_logouting:
        str="正在退网";
        break;
    case objPara::Status_online_idle:
        str="在线等待";
        break;
    case objPara::Status_online_p2p_call:
        strXY("发送速率:       ",128,32);
        strXY("接收速率:       ",128,48);
        strXY(m_status.strTXrateC().toLatin1().data(),128,32);
        strXY(m_status.strRXrateC().toLatin1().data(),128,48);
        str="正在通信";
        break;
    case objPara::Status_online_ncc_plan:
        str="预案通信";
        break;
    default:
        break;
    }
    centerXY(str.toLatin1().data(),0,0,256,32,1,1);

    strXY(QString("集中控制"),0,32);
    sprintf(buf,"S/N: %.2f",m_status.m_fSNR);
    strXY(buf,0,48);

}

// calling
// success
// offline
// p2p
// tdm_offline ,
void objui::slotShowStatusPage1()
{
    slotGetCUstate();
    QString str;
    char buf[40];
    zeroFB(0);

    if(!m_bInitDone){
        centerXY("正在初始化......",0,0,256,32,1,1);
    }
    else if(m_para.m_status==objPara::Status_idle){// idle
        switch(m_para.m_workMode){
        case objPara::Mode_p2p:
            m_boolStatusPageP2P = true;
            switch(m_para.m_devMode){
            case objPara::DevMode_bridge:
                strXY(QString("(网桥)"),48,32);
                break;
            case objPara::DevMode_router:
                strXY(QString("(路由)"),48,32);
                break;
            default: break;
            }
            centerXY("空    闲",0,0,256,32,1,1);

            strXY(QString("点对点"),0,32);
            //sprintf(buf,"S/N: 0.00",m_status.m_fSNR);
            strXY("S/N: 0.00",0,48);
            strXY("发送速率:  -- k",128,32);
            strXY("接收速率:  -- k",128,48);
            //strXY(m_para.strTxRate().toLatin1().data(),128,32);
            //strXY(m_para.strRxRate().toLatin1().data(),128,48);
            break;
        case objPara::Mode_central:
            m_boolStatusPageP2P = false;
            centerXY("空    闲",0,0,256,32,1,1);
            strXY(QString("集中控制"),0,32);
            strXY("S/N: 0.00",0,48);
            strXY("发送速率:  -- k",128,32);
            strXY("接收速率:  -- k",128,48);
            //strXY(m_para.strTxRate().toLatin1().data(),128,32);
            //strXY(m_para.strRxRate().toLatin1().data(),128,48);
            break;
        default: break;
        }
    }
    else{// if(m_para.m_status==objPara::Status_connected){
        switch(m_status.m_workMode){
        case objPara::Mode_p2p:
            m_boolStatusPageP2P = true;
            switch(m_status.m_devMode){
            case objPara::DevMode_bridge:
                strXY(QString("(网桥)"),48,32);
                break;
            case objPara::DevMode_router:
                strXY(QString("(路由)"),48,32);
                break;
            default: break;
            }

            centerXY("正在通信",0,0,256,16,1,1);
            str=getTimeSpan();
            centerXY(str.toLatin1().data(),0,16,256,16,1,1);

            strXY(QString("点对点"),0,32);
            //strXY(QString("集中控制"),0,32);
            sprintf(buf,"S/N: %.2f",m_status.m_fSNR);
            //if(m_status.m_recvSync) sprintf(buf,"S/N: %.2f",m_status.m_fSNR);
            //else sprintf(buf,"S/N: 0.00");
            strXY(buf,0,48);
            strXY("发送速率:       ",128,32);
            strXY("接收速率:       ",128,48);
            strXY(m_status.strTXrate().toLatin1().data(),128,32);
            strXY(m_status.strRXrate().toLatin1().data(),128,48);
            break;
        case objPara::Mode_central:
            m_boolStatusPageP2P = false;
            showStatusPage1c();
            break;
        default: break;
        }
    }

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();
    //if(1>m_nShowStatusPage1++) qDebug("       end.objui.func.showStatusPage1");
    QTimer::singleShot(200,this,SLOT(slotKeyEnable()));
}
void objui::slotShowStatusPage2()
{
    char buf[40];
    //const QLocale & locale = QLocale::c();
    QString s;//=locale.toString(m_numEditor.m_i64);
    zeroFB(0);

    switch(m_para.m_workMode){
    case objPara::Mode_p2p:
        strXY("发送功率:",0,0,0x0f,0);// fasong gonglv 发送 功率
        strXY("发送频点:",0,16,0x0f,0);// fasong pindian 发送频率  频率。频点
        strXY("接收频点:",0,32,0x0f,0);// jieshou pindian 发送频率  频率。频点 接收
        strXY("业务接收:",0,48,0x0f,0);//

        //s=locale.toString(0.01*m_para.m_power100) + " dBm";
        //sprintf(buf,"%.2f dBm",0.01*m_status.m_power100);
        //centerXY(buf,9*8, 0, 256-9*8,16,1,1,0x0f,0);

        //centerXY(m_status.strTxFreq(m_para.m_BUCfreq).toLatin1().data(),9*8, 16, 256-9*8,16,1,1,0x0f,0);
        //centerXY(m_status.strRxFreq(m_para.m_LNBfreq).toLatin1().data(),9*8, 32, 256-9*8,16,1,1,0x0f,0);

        if(m_para.m_status==objPara::Status_idle){// idle
            strXY("发送功率:",0,0,0x0f,0);// fasong gonglv 发送 功率
            strXY("发送频点:",0,16,0x0f,0);// fasong pindian 发送频率  频率。频点
            strXY("接收频点:",0,32,0x0f,0);// jieshou pindian 发送频率  频率。频点 接收
            strXY("业务接收:",0,48,0x0f,0);//

            //s=locale.toString(0.01*m_para.m_power100) + " dBm";
            sprintf(buf,"%.2f dBm",0.01*m_para.m_power100);
            centerXY(buf,9*8, 0, 256-9*8,16,1,1,0x0f,0);

            centerXY(m_para.strTxFreq(m_para.m_BUCfreq).toLatin1().data(),9*8, 16, 256-9*8,16,1,1,0x0f,0);
            centerXY(m_para.strRxFreq(m_para.m_LNBfreq).toLatin1().data(),9*8, 32, 256-9*8,16,1,1,0x0f,0);
        }
        else{// if(m_para.m_status==objPara::Status_connected){
            strXY("发送功率:",0,0,0x0f,0);// fasong gonglv 发送 功率
            strXY("发送频点:",0,16,0x0f,0);// fasong pindian 发送频率  频率。频点
            strXY("接收频点:",0,32,0x0f,0);// jieshou pindian 发送频率  频率。频点 接收
            strXY("业务接收:",0,48,0x0f,0);//

            //s=locale.toString(0.01*m_para.m_power100) + " dBm";
            sprintf(buf,"%.2f dBm",0.01*m_status.m_power100);
            centerXY(buf,9*8, 0, 256-9*8,16,1,1,0x0f,0);

            centerXY(m_status.strTxFreq(m_para.m_BUCfreq).toLatin1().data(),9*8, 16, 256-9*8,16,1,1,0x0f,0);
            centerXY(m_status.strRxFreq(m_para.m_LNBfreq).toLatin1().data(),9*8, 32, 256-9*8,16,1,1,0x0f,0);
        }

        break;
    case objPara::Mode_central:
        if(m_para.m_status==objPara::Status_idle){// idle
            strXY("发送功率:",0,0,0x0f,0);// fasong gonglv 发送 功率
            strXY("发送频点:",0,16,0x0f,0);// fasong pindian 发送频率  频率。频点
            strXY("接收频点:",0,32,0x0f,0);// jieshou pindian 发送频率  频率。频点 接收
            strXY("业务接收:",0,48,0x0f,0);//

            //s=locale.toString(0.01*m_para.m_power100) + " dBm";
            sprintf(buf,"--- dBm");
            centerXY(buf,9*8, 0, 256-9*8,16,1,1,0x0f,0);

            centerXY(m_para.strTxFreq(m_para.m_BUCfreq).toLatin1().data(),9*8, 16, 256-9*8,16,1,1,0x0f,0);
            centerXY(m_para.strRxFreq(m_para.m_LNBfreq).toLatin1().data(),9*8, 32, 256-9*8,16,1,1,0x0f,0);
        }
        else{// if(m_para.m_status==objPara::Status_connected){
            strXY("发送功率:",0,0,0x0f,0);// fasong gonglv 发送 功率
            strXY("发送频点:",0,16,0x0f,0);// fasong pindian 发送频率  频率。频点
            strXY("接收频点:",0,32,0x0f,0);// jieshou pindian 发送频率  频率。频点 接收
            strXY("业务接收:",0,48,0x0f,0);//

            //s=locale.toString(0.01*m_para.m_power100) + " dBm";
            sprintf(buf,"%.2f dBm",0.01*m_status.m_power100Central);
            centerXY(buf,9*8, 0, 256-9*8,16,1,1,0x0f,0);

            centerXY(m_status.strTxFreq(m_para.m_BUCfreq).toLatin1().data(),9*8, 16, 256-9*8,16,1,1,0x0f,0);
            centerXY(m_status.strRxFreq(m_para.m_LNBfreq).toLatin1().data(),9*8, 32, 256-9*8,16,1,1,0x0f,0);
        }
        break;
    default: break;
    }

    if(m_status.m_recvSync) centerXY("同步",9*8,48,256-9*8,16,1,1);
    else centerXY("未同步",9*8,48,256-9*8,16,1,1);

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();

    QTimer::singleShot(200,this,SLOT(slotKeyEnable()));

}
void objui::slotShowStatusPage3()
{
    char buf[40];
    //const QLocale & locale = QLocale::c();
    QString s;//=locale.toString(m_numEditor.m_i64);
    zeroFB(0);

    strXY("业务频偏:",0,0,0x0f,0);//

    sprintf(buf,"%.1f kHz",0.001*m_status.m_freqOffset);
    centerXY(buf,9*8, 0, 256-9*8,16,1,1,0x0f,0);

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();

    QTimer::singleShot(200,this,SLOT(slotKeyEnable()));

}


void objui::slotShowMode1()
{
    int c=0x0f;
    if(m_bErrChangeWorkMode) c=1;
    zeroFB(0);

    switch(m_para.m_workMode){
    case objPara::Mode_p2p:
        centerXY("[*] 点 对 点",0,10,256,16,1,1,0,c);
        centerXY("[ ] 集中控制",0,48-10,256,16,1,1,c);
        break;
    case objPara::Mode_central:
        centerXY("[ ] 点 对 点",0,10,256,16,1,1,0,c);
        centerXY("[*] 集中控制",0,48-10,256,16,1,1,c);
        break;
    default: break;
    }
    if(m_bErrChangeWorkMode){
        centerXY("-------------------------",0,21,256,16,1,1);
        centerXY("-------------------------",0,27,256,16,1,1);
        centerXY("正在通信,不能切换工作模式",0,24,256,16,1,1);
    }
    //if(m_bErrChangeWorkMode) centerXY("正在通信,不能切换工作模式",0,24,256,16,1,1);
    //if(m_bErrChangeWorkMode) strXY("正在通信,不能切换工作模式",0,48);

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();
    QTimer::singleShot(200,this,SLOT(slotKeyEnable()));
    //m_bEnableKeyboard = true;

}
void objui::slotShowMode2()
{
    int c=0x0f;
    if(m_bErrChangeWorkMode) c=1;
    zeroFB(0);

    switch(m_para.m_workMode){
    case objPara::Mode_p2p:
        centerXY("[*] 点 对 点",0,10,256,16,1,1,c);
        centerXY("[ ] 集中控制",0,48-10,256,16,1,1,0,c);
        break;
    case objPara::Mode_central:
        centerXY("[ ] 点 对 点",0,10,256,16,1,1,c);
        centerXY("[*] 集中控制",0,48-10,256,16,1,1,0,c);
        break;
    default: break;
    }
    if(m_bErrChangeWorkMode){
        centerXY("-------------------------",0,21,256,16,1,1);
        centerXY("-------------------------",0,27,256,16,1,1);
        centerXY("正在通信,不能切换工作模式",0,24,256,16,1,1);
    }

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();
    QTimer::singleShot(200,this,SLOT(slotKeyEnable()));
    //m_bEnableKeyboard = true;

}
void objui::slotShowDevMode1()
{
    zeroFB(0);

    switch(m_para.m_devMode){
    case objPara::DevMode_bridge:
        centerXY("[*] 网  桥",0,12,256,16,1,1,0,0x0f);
        centerXY("[ ] 路  由",0,48-10,256,16,1,1);
        break;
    case objPara::DevMode_router:
        centerXY("[ ] 网  桥",0,12,256,16,1,1,0,0x0f);
        centerXY("[*] 路  由",0,48-10,256,16,1,1);
        break;
    default: break;
    }

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();
    QTimer::singleShot(200,this,SLOT(slotKeyEnable()));
    //m_bEnableKeyboard = true;

}
void objui::slotShowDevMode2()
{
    zeroFB(0);

    switch(m_para.m_devMode){
    case objPara::DevMode_bridge:
        centerXY("[*] 网  桥",0,12,256,16,1,1);
        centerXY("[ ] 路  由",0,48-10,256,16,1,1,0,0x0f);
        break;
    case objPara::DevMode_router:
        centerXY("[ ] 网  桥",0,12,256,16,1,1);
        centerXY("[*] 路  由",0,48-10,256,16,1,1,0,0x0f);
        break;
    default: break;
    }

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();
    QTimer::singleShot(200,this,SLOT(slotKeyEnable()));
    //m_bEnableKeyboard = true;

}
// ver1.0 spi.freq:16MHz
// ver1.1 spi.freq:4MHz timeout:120s
// ver1.2 recall ,
// ver1.3 power +100
// ver1.4 status.page1.dobackspace()  => doBackspace()          3.19
// ver1.5 status2.freq.format
// ver1.6 state : msgZZHJ.HJCG.....
// ver1.7 4MHz
// ver1.8 m_status.m_workmode
// ver1.9 flush per dot
// ver1.10 demo.logoutnet
// ver1.11 login , logout
// ver1.12 para.central  tdm , powerADJ
// ver1.13 fix tdm.power
// ver1.14 draw line /4
// ver1.15 32align
// ver1.16(4.18 /dev/shm/log6q.txt
// ver1.17(4.18 add m_semKey
//         4.23 voled
// ver1.18(4.29 q.char->q.32 better , seg fault => abort
//        (4.30 myq32
// ver1.19(5.19 36M
//        (5.19a add ParaPage1a
//        (5.19b
// ver1.20(5.21 logout
//    1.20a , key<- ->
// ver1.21(5.31 BUC,LNB ctrl
//    1.21a(6.3 menu page BUC.setting
// ver1.22a(6.9 add callID , StateParaP11c , 12c , 13c,14c
//    1.22b(6.11 callID ==> IDstr
// ver1.23a(6.12 BUCpwr
// ver1.24a(6.14 sw4
// ver1.25(6.14 add sync  status.page2
// ver1.26(6.15 status.page2 add repaint)    t2:pwr -10 -45
// ver1.27a(6.17 para.default.workmode=central , donot logout on start
// ver1.27b(6.18 ..ParaP21c,..
//    1.28(6.25
// ver1.28a(6.27 save p2p.workmode ,
// ver1.28b(6.27 init.ing
// ver1.29(6.27 csdev_id.about
// ver1.30(6.28   bugfix , app.central.display.p2p
// ver1.31(7.6 power.c +-20 ==> +-40
// ver1.32(7.12 RxTxFreq: 950-2150
// ver1.32a(7.22 add testSignal ,
// ver1.32d(7.23 recvSync.snr
// ver1.33(7.31   if_power
// ver1.34(8.3
// ver1.35(8.3 4m,36m

void objui::slotShowAbout()
{
    zeroFB(0);

    strXY("ver: 1.35",0,0);
    centerXY("8.3",0,48,256,16,2,1);// data 19.3.10

    const QHostAddress &localaddress = QHostAddress::LocalHost;
    foreach(const QHostAddress &addr, QNetworkInterface::allAddresses()){
        if(addr.protocol()==QAbstractSocket::IPv4Protocol && addr!=localaddress){
            strXY(addr.toString().toLatin1().data(),0,16);
            break;
        }
            //qDebug(" ======================= ipaddress : %s",addr.toString().toLatin1().data());
    }
    strXY(getTimeRunning().toLatin1().data(),0,32);
    strXY(m_para.m_cwDevID.toLatin1().data(),0,48);

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();
    QTimer::singleShot(200,this,SLOT(slotKeyEnable()));
    //m_bEnableKeyboard = true;

}
void objui::slotShowMsgSetBUCpwr()
{
    m_status.m_swBUCpwr=0;
    setBUCpwr();
    getParaModem();

    zeroFB(0);

    if(m_para.m_swBUCpwr>0) centerXY(QString("打开BUC馈电......"),0,24,256,16,1,1);
    else centerXY(QString("关闭BUC馈电......"),0,24,256,16,1,1);

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();
    QTimer::singleShot(1500,this,SLOT(slotStateTransitionNext()));
}
void objui::slotShowMsgSetBUC10m()
{
    m_status.m_swBUC10m=0;
    setBUC10m();
    getParaModem();

    zeroFB(0);

    if(m_para.m_swBUC10m>0) centerXY(QString("打开BUC馈钟......"),0,24,256,16,1,1);
    else centerXY(QString("关闭BUC馈钟......"),0,24,256,16,1,1);

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();
    QTimer::singleShot(1500,this,SLOT(slotStateTransitionNext()));

}
void objui::slotShowMsgSetLNB10m()
{
    m_status.m_swLNB10m=0;
    setLNB10m();
    getParaModem();

    zeroFB(0);

    if(m_para.m_swLNB10m>0) centerXY(QString("打开LNB馈钟......"),0,24,256,16,1,1);
    else centerXY(QString("关闭LNB馈钟......"),0,24,256,16,1,1);

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();
    QTimer::singleShot(1500,this,SLOT(slotStateTransitionNext()));

}
void objui::slotShowMsgSetLNBpwr()
{
    m_status.m_swLNBpwr=0;
    setLNBpwr();
    getParaModem();

    zeroFB(0);

    if(m_para.m_swLNBpwr>0) centerXY(QString("打开LNB馈电......"),0,24,256,16,1,1);
    else centerXY(QString("关闭LNB馈电......"),0,24,256,16,1,1);

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();
    QTimer::singleShot(1500,this,SLOT(slotStateTransitionNext()));

}

void objui::slotShowMsgZZRW()
{
    qDebug(" == slotShowMsgZZRW");
    zeroFB(0);

    centerXY(QString("正 在 入 网 ......"),0,24,256,16,1,1);

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();
    QTimer::singleShot(1500,this,SLOT(slotStateTransitionNext()));
    qDebug(" == end slotShowMsgZZRW");
}
void objui::slotShowMsgZZTW()
{
    zeroFB(0);

    centerXY(QString("正 在 退 网 ......"),0,24,256,16,1,1);

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();
    QTimer::singleShot(1500,this,SLOT(slotStateTransitionNext()));
}
void objui::statusP2Pcall()
{
    switch(m_nCountTimerCall){
    case 0:
        doCallP2P();
        zeroFB(0);
        centerXY(QString("正 在 呼 叫 ......"),0,24,256,16,1,1);
        Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);
        emit sigFlush();
        m_nCountTimerCall++;
        QTimer::singleShot(1500,this,SLOT(slotEVzzhjPaint()));
        break;
    case 1:
    case 2:
        if(m_status.m_cuMode==objPara::CUState_mode_OFFLINE_P2P){
            zeroFB(0);
            centerXY(QString("呼 叫 成 功 !"),0,24,256,16,1,1);// chenggong
            Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);
            emit sigFlush();
            QTimer::singleShot(1500,this,SLOT(slotEVzzhjBackM()));// go to main.status
        }
        else{
            m_nCountTimerCall++;
            QTimer::singleShot(1500,this,SLOT(slotEVzzhjPaint()));
        }
        break;
    case 3:
        zeroFB(0);
        centerXY(QString("呼 叫 失 败 !"),0,24,256,16,1,1);// chenggong
        Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);
        emit sigFlush();
        QTimer::singleShot(1500,this,SLOT(slotEVzzhjBack()));
        break;
    default:
        break;
    }

}
void objui::statusP2Phangup()
{
    switch(m_nCountTimerCall){
    case 0:
        doDisconnectP2P();
        zeroFB(0);
        centerXY(QString("正 在 挂 断 ......"),0,24,256,16,1,1);
        Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);
        emit sigFlush();
        m_nCountTimerCall++;
        QTimer::singleShot(1500,this,SLOT(slotEVzzhjPaint()));
        break;
    case 1:
    case 2:
        if(m_status.m_cuMode!=objPara::CUState_mode_OFFLINE_P2P){
            zeroFB(0);
            centerXY(QString("挂 断 成 功 !"),0,24,256,16,1,1);// chenggong
            Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);
            emit sigFlush();
            QTimer::singleShot(1500,this,SLOT(slotEVzzhjBackM()));// go to main.status
        }
        else{
            m_nCountTimerCall++;
            QTimer::singleShot(1500,this,SLOT(slotEVzzhjPaint()));
        }
        break;
    case 3:
        zeroFB(0);
        centerXY(QString("挂 断 失 败 !"),0,24,256,16,1,1);//
        Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);
        emit sigFlush();
        QTimer::singleShot(1500,this,SLOT(slotEVzzhjBack()));// go to main.status
        break;
    default:
        break;
    }

}
void objui::cmdLogin()
{
    std::string stdstr;
    stdstr=m_cu.loginNet();
    writeTcp(QString::fromStdString(stdstr).toUtf8());
}
void objui::cmdLogout()
{
    std::string stdstr;
    stdstr=m_cu.logoutNet();
    writeTcp(QString::fromStdString(stdstr).toUtf8());
}

void objui::statusNCClogout()
{
    switch(m_nCountTimerCall){
    case 0:
        cmdLogout();
        zeroFB(0);
        centerXY(QString("正 在 退 网 ......"),0,24,256,16,1,1);
        Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);
        emit sigFlush();
        m_nCountTimerCall++;
        QTimer::singleShot(1500,this,SLOT(slotEVzzhjPaint()));
        break;
    case 1:
    case 2:
        if(m_status.m_cuNetState==objPara::CUState_netState_OFFLINE){
            zeroFB(0);
            centerXY(QString("退 网 成 功 !"),0,24,256,16,1,1);// chenggong
            Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);
            emit sigFlush();
            QTimer::singleShot(1500,this,SLOT(slotEVzzhjBack()));// go to main.status
        }
        else{
            m_nCountTimerCall++;
            QTimer::singleShot(1500,this,SLOT(slotEVzzhjPaint()));
        }
        break;
    case 3:
        zeroFB(0);
        centerXY(QString("退 网 失 败 !"),0,24,256,16,1,1);//
        Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);
        emit sigFlush();
        QTimer::singleShot(1500,this,SLOT(slotEVzzhjBack()));
        break;
    default:
        break;
    }

}
void objui::statusNCClogin()
{
    switch(m_nCountTimerCall){
    case 0:
        cmdLogin();
        zeroFB(0);
        centerXY(QString("正 在 入 网 ......"),0,24,256,16,1,1);
        Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);
        emit sigFlush();
        m_nCountTimerCall++;
        QTimer::singleShot(1500,this,SLOT(slotEVzzhjPaint()));
        break;
    default:
        if(m_status.m_cuNetState==objPara::CUState_netState_ONLINE){
            zeroFB(0);
            centerXY(QString("入 网 成 功 !"),0,24,256,16,1,1);// chenggong
            Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);
            emit sigFlush();
            QTimer::singleShot(1500,this,SLOT(slotEVzzhjBack()));// go to main.status
        }
        else{
            switch(m_status.m_cuNetState){
            case objPara::CUState_netState_WAITING_TDM:
                zeroFB(0);
                centerXY(QString("等待网控广播"),0,24,256,16,1,1);
                Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);
                emit sigFlush();
                m_nCountTimerCall++;
                QTimer::singleShot(1500,this,SLOT(slotEVzzhjPaint()));
                break;
            case objPara::CUState_netState_LOGINING:
                zeroFB(0);
                centerXY(QString("正 在 入 网 ......"),0,24,256,16,1,1);
                Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);
                emit sigFlush();
                m_nCountTimerCall++;
                QTimer::singleShot(1500,this,SLOT(slotEVzzhjPaint()));
                break;
            default:
                m_nCountTimerCall++;
                QTimer::singleShot(1500,this,SLOT(slotEVzzhjPaint()));
                break;
            }
        }
        slotKeyEnable();
        break;
    }

}
void objui::statusNCCcall()
{
    switch(m_nCountTimerCall){
    case 0:
        cmdNCCcall();
        zeroFB(0);
        centerXY(QString("正 在 呼 叫 ......"),0,24,256,16,1,1);
        Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);
        emit sigFlush();
        m_nCountTimerCall++;
        QTimer::singleShot(1500,this,SLOT(slotEVzzhjPaint()));
        break;
    case 1:
    case 2:
    case 3:
        if(m_status.m_sessionState==objPara::SESSIONstate_connected){
            zeroFB(0);
            centerXY(QString("呼 叫 成 功 !"),0,24,256,16,1,1);// chenggong
            Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);
            emit sigFlush();
            m_nCountTimerCall++;
            QTimer::singleShot(1500,this,SLOT(slotEVzzhjBackM()));// go to main.status
        }
        else{
            m_nCountTimerCall++;
            QTimer::singleShot(1500,this,SLOT(slotEVzzhjPaint()));
        }
        slotKeyEnable();
        break;
    default:
        if(m_status.m_sessionState==objPara::SESSIONstate_closed){
            qDebug("                            ncc.call             ============= closed closed ");
            zeroFB(0);
            centerXY(QString("呼 叫 失 败 !"),0,24,256,16,1,1);
            Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);
            emit sigFlush();
            m_nCountTimerCall++;
            QTimer::singleShot(1500,this,SLOT(slotEVzzhjBack()));
        }
        else if(m_status.m_sessionState==objPara::SESSIONstate_connected){
            zeroFB(0);
            centerXY(QString("呼 叫 成 功 !"),0,24,256,16,1,1);// chenggong
            Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);
            emit sigFlush();
            m_nCountTimerCall++;
            QTimer::singleShot(1500,this,SLOT(slotEVzzhjBackM()));
        }
        else{
            m_nCountTimerCall++;
            QTimer::singleShot(1500,this,SLOT(slotEVzzhjPaint()));
        }
        slotKeyEnable();
        break;
    }


}
void objui::statusNCChangup()
{
    switch(m_nCountTimerCall){
    case 0:
        cmdNCChangup();
        zeroFB(0);
        centerXY(QString("正 在 挂 断 ......"),0,24,256,16,1,1);
        Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);
        emit sigFlush();
        m_nCountTimerCall++;
        QTimer::singleShot(1500,this,SLOT(slotEVzzhjPaint()));
        break;
    default:
        if(m_status.m_sessionState==objPara::SESSIONstate_closed){
            zeroFB(0);
            centerXY(QString("挂 断 成 功 !"),0,24,256,16,1,1);// chenggong
            Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);
            emit sigFlush();
            QTimer::singleShot(1500,this,SLOT(slotEVzzhjBack()));// go to main.status
        }
        else{
            m_nCountTimerCall++;
            QTimer::singleShot(1500,this,SLOT(slotEVzzhjPaint()));
        }
        slotKeyEnable();
        break;
    }


}

void objui::slotShowMsgZZHJ()
{
    getCUstate();
    switch(m_status.m_callCMD){
    case objPara::CMD_P2P_CALL:
        statusP2Pcall();
        break;
    case objPara::CMD_P2P_HANGUP:
        statusP2Phangup();
        break;
    case objPara::CMD_NCC_LOGOUT:
        statusNCClogout();
        break;
    case objPara::CMD_NCC_LOGIN:
        statusNCClogin();
        break;
    case objPara::CMD_NCC_CALL:
        statusNCCcall();
        break;
    case objPara::CMD_NCC_HANGUP:
        statusNCChangup();
        break;
    default:
        break;
    }

    return;
}
void objui::slotShowMsgHJCG()
{
    zeroFB(0);

    if(m_para.m_status == objPara::Status_connected){
        centerXY(QString("呼 叫 成 功 !"),0,24,256,16,1,1);// chenggong
    }
    else{
        centerXY(QString("呼 叫 失 败 !"),0,24,256,16,1,1);// chenggong
    }

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();
#if 0
    if(m_para.m_status != objPara::Status_connected){
        m_para.m_status = objPara::Status_connected;
        m_para.m_startSecs = QDateTime::currentDateTime().toTime_t();
    }
#endif
    QTimer::singleShot(1000,this,SLOT(slotStateTransitionNext()));
}
void objui::slotShowMsgZZGD()
{
    zeroFB(0);

    centerXY(QString("正 在 挂 断 ......"),0,24,256,16,1,1);

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();
    QTimer::singleShot(1000,this,SLOT(slotStateTransitionNext()));
}
void objui::slotShowMsgGDCG()
{
    m_para.m_status = objPara::Status_idle;
    zeroFB(0);

    centerXY(QString("挂 断 成 功 !"),0,24,256,16,1,1);// chenggong

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();
    QTimer::singleShot(1000,this,SLOT(slotStateTransitionNext()));
}

void objui::slotShowMenu00()
{
    qDebug(" func slotShow.menu.00000000000000000000000000000000000000");
    getPara();
    zeroFB(0);

    centerXY(QString("通  信  控  制"),0,0,256,16,1,1);
    centerXY(QString("通  信  参  数"),0,16,256,16,1,1);
    centerXY(QString("工  作  模  式"),0,32,256,16,1,1);
    centerXY(QString("射  频  设  置"),0,48,256,16,1,1);
    centerXY(QString("通  信  控  制"),0,0,256,16,1,1,0,0x0f);

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();
    QTimer::singleShot(200,this,SLOT(slotKeyEnable()));
    //m_bEnableKeyboard = true;

}
void objui::slotGetRadioParams()
{
    std::string stdstr;

    // get bufFreq LNBfreq
    stdstr=m_cu.getRadioLinkParams();// getConfiguration(QString("modem").toStdString());
    writeTcp(QString::fromStdString(stdstr).toUtf8());

}

void objui::getPara()
{
    qDebug(" ========================================================================================= get para");
    std::string stdstr;

    getCUstate();

    stdstr=m_cu.getConfiguration(QString("tdm").toStdString());
    qDebug("=====tdm %s",QString::fromStdString(stdstr).toLatin1().data());
    writeTcp(QString::fromStdString(stdstr).toUtf8());
    stdstr=m_cu.getConfiguration(QString("power").toStdString());
    qDebug(" =======   power  %s",QString::fromStdString(stdstr).toLatin1().data());
    writeTcp(QString::fromStdString(stdstr).toUtf8());

    stdstr=m_cu.getConfiguration(QString("modem").toStdString());
    writeTcp(QString::fromStdString(stdstr).toUtf8());
    // get bufFreq LNBfreq
    stdstr=m_cu.getRadioLinkParams();// getConfiguration(QString("modem").toStdString());
    writeTcp(QString::fromStdString(stdstr).toUtf8());
}
void objui::getParaModem()
{
    std::string stdstr;

    stdstr=m_cu.getConfiguration(QString("modem").toStdString());
    writeTcp(QString::fromStdString(stdstr).toUtf8());
}

void objui::slotShowMenu01()
{
    //qDebug("  ***** func slotShow.menu.01");
    getPara();
    zeroFB(0);

    centerXY(QString("通  信  控  制"),0,0,256,16,1,1);
    centerXY(QString("通  信  参  数"),0,16,256,16,1,1);
    centerXY(QString("工  作  模  式"),0,32,256,16,1,1);
    centerXY(QString("射  频  设  置"),0,48,256,16,1,1);
    centerXY(QString("通  信  参  数"),0,16,256,16,1,1,0,0x0f);

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();
    QTimer::singleShot(300,this,SLOT(slotKeyEnable()));
    //m_bEnableKeyboard = true;

}
void objui::slotShowMenu02()
{
    //qDebug(" func slotShow.menu.02");
    zeroFB(0);

    centerXY(QString("通  信  控  制"),0,0,256,16,1,1);
    centerXY(QString("通  信  参  数"),0,16,256,16,1,1);
    centerXY(QString("工  作  模  式"),0,32,256,16,1,1);
    centerXY(QString("射  频  设  置"),0,48,256,16,1,1);
    centerXY(QString("工  作  模  式"),0,32,256,16,1,1,0,0x0f);

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();
    QTimer::singleShot(200,this,SLOT(slotKeyEnable()));
    //m_bEnableKeyboard = true;

}
void objui::slotGetSWStatus()
{

}
// radio setting
void objui::slotShowMenu03()
{

    getPara();
    //slotGetSWstatus();
    zeroFB(0);

    centerXY(QString("通  信  控  制"),0,0,256,16,1,1);
    centerXY(QString("通  信  参  数"),0,16,256,16,1,1);
    centerXY(QString("工  作  模  式"),0,32,256,16,1,1);
    centerXY(QString("射  频  设  置"),0,48,256,16,1,1);
    centerXY(QString("射  频  设  置"),0,48,256,16,1,1,0,0x0f);

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();
    QTimer::singleShot(200,this,SLOT(slotKeyEnable()));

}
// BUC pwr
void objui::slotShow_BUCpwr()
{
    zeroFB(0);

    centerXY(QString("BUC 馈 电 设 置"),0,4,256,16,1,1);
    if(m_status.m_swBUCpwr>0) centerXY(QString("当前状态:开"),0,16+8,256,16,1,1);
    else if(m_status.m_swBUCpwr<0) centerXY(QString("当前状态:关"),0,16+8,256,16,1,1);
    else  centerXY(QString("当前状态:未知"),0,16+8,256,16,1,1);

    if(m_para.m_swBUCpwr>0){
        strXY(QString("打 开"),7*8,32+12,0,0x0f);
        strXY(QString("关 闭"),20*8,32+12);
    }
    else{
        strXY(QString("打 开"),7*8,32+12);
        strXY(QString("关 闭"),20*8,32+12,0,0x0f);
    }

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();
    QTimer::singleShot(200,this,SLOT(slotKeyEnable()));
}
// BUC 10m
void objui::slotShow_BUC10m()
{
    zeroFB(0);

    centerXY(QString("BUC 馈 钟 设 置"),0,4,256,16,1,1);
    if(m_status.m_swBUC10m>0) centerXY(QString("当前状态:开"),0,16+8,256,16,1,1);
    else if(m_status.m_swBUC10m<0) centerXY(QString("当前状态:关"),0,16+8,256,16,1,1);
    else  centerXY(QString("当前状态:未知"),0,16+8,256,16,1,1);

    if(m_para.m_swBUC10m>0){
        strXY(QString("打 开"),7*8,32+12,0,0x0f);
        strXY(QString("关 闭"),20*8,32+12);
    }
    else{
        strXY(QString("打 开"),7*8,32+12);
        strXY(QString("关 闭"),20*8,32+12,0,0x0f);
    }

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();
    QTimer::singleShot(200,this,SLOT(slotKeyEnable()));
}
// LNB pwr
void objui::slotShow_LNBpwr()
{
    zeroFB(0);

    centerXY(QString("LNB 馈 电 设 置"),0,4,256,16,1,1);
    if(m_status.m_swLNBpwr>0) centerXY(QString("当前状态:开"),0,16+8,256,16,1,1);
    else if(m_status.m_swLNBpwr<0) centerXY(QString("当前状态:关"),0,16+8,256,16,1,1);
    else  centerXY(QString("当前状态:未知"),0,16+8,256,16,1,1);

    if(m_para.m_swLNBpwr>0){
        strXY(QString("打 开"),7*8,32+12,0,0x0f);
        strXY(QString("关 闭"),20*8,32+12);
    }
    else{
        strXY(QString("打 开"),7*8,32+12);
        strXY(QString("关 闭"),20*8,32+12,0,0x0f);
    }

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();
    QTimer::singleShot(200,this,SLOT(slotKeyEnable()));
}
// LNB 10m
void objui::slotShow_LNB10m()
{
    zeroFB(0);

    centerXY(QString("LNB 馈 钟 设 置"),0,4,256,16,1,1);
    if(m_status.m_swLNB10m>0) centerXY(QString("当前状态:开"),0,16+8,256,16,1,1);
    else if(m_status.m_swLNB10m<0) centerXY(QString("当前状态:关"),0,16+8,256,16,1,1);
    else  centerXY(QString("当前状态:未知"),0,16+8,256,16,1,1);

    if(m_para.m_swLNB10m>0){
        strXY(QString("打 开"),7*8,32+12,0,0x0f);
        strXY(QString("关 闭"),20*8,32+12);
    }
    else{
        strXY(QString("打 开"),7*8,32+12);
        strXY(QString("关 闭"),20*8,32+12,0,0x0f);
    }

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();
    QTimer::singleShot(200,this,SLOT(slotKeyEnable()));
}
QString objui::getSWstr(QString str, int sw)
{
    QString ret;
    if(sw>0) ret=str+" 开";
    else if(sw<0)ret = str+" 关";
    else ret=str +" 未知";
    return ret;
}

void objui::showBUCLNB()
{
    QString str;
    char buf[40];
#if 0
    strXY(QString("BUC馈电设置(开)"),0,0);
    strXY(QString("BUC馈钟设置(关)"),0,16);
    strXY(QString("BUC本振频率"),0,32);
    strXY(QString("LNB馈电设置(关)"),128,0);
    strXY(QString("LNB馈钟设置(开)"),128,16);
    strXY(QString("LNB本振频率"),0,48);
    strXY(QString("BUC馈电设置"),0,0,0,0x0f);
#endif
    str = getSWstr(QString("BUC馈电"),m_status.m_swBUCpwr);
    strXY(str,0,0);
    str = getSWstr(QString("BUC馈钟"),m_status.m_swBUC10m);
    strXY(str,0,16);
    strXY(QString("BUC本振频率"),0,32);
    str = getSWstr(QString("LNB馈电"),m_status.m_swLNBpwr);
    strXY(str,128+32,0);
    str = getSWstr(QString("LNB馈钟"),m_status.m_swLNB10m);
    strXY(str,128+32,16);
    strXY(QString("LNB本振频率"),0,48);

    sprintf(buf,"%.4f MHz",0.000001*m_para.m_BUCfreq);
    strXY(buf,16*6,32);
    sprintf(buf,"%.4f MHz",0.000001*m_para.m_LNBfreq);
    strXY(buf,16*6,48);
}

void objui::slotShowBUCpwr()
{
    zeroFB(0);

    showBUCLNB();
    strXY(QString("BUC馈电"),0,0,0,0x0f);

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();
    QTimer::singleShot(200,this,SLOT(slotKeyEnable()));

}
void objui::slotShowBUC10m()
{
    zeroFB(0);
    showBUCLNB();
    strXY(QString("BUC馈钟"),0,16,0,0x0f);

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();
    QTimer::singleShot(200,this,SLOT(slotKeyEnable()));

}
void objui::slotShowBUCfreq()
{
    m_numEditor.setNum64(m_para.m_BUCfreq,m_para.m_maxTDMfreq,m_para.m_minTDMfreq,-1,2,-2);

    zeroFB(0);
    showBUCLNB();
    strXY(QString("BUC本振频率"),0,32,0,0x0f);

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();
    QTimer::singleShot(200,this,SLOT(slotKeyEnable()));

}
void objui::slotShowLNBpwr()
{
    zeroFB(0);
    showBUCLNB();
    strXY(QString("LNB馈电"),128+32,0,0,0x0f);

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();
    QTimer::singleShot(200,this,SLOT(slotKeyEnable()));

}
void objui::slotShowLNB10m()
{
    zeroFB(0);
    showBUCLNB();
    strXY(QString("LNB馈钟"),128+32,16,0,0x0f);

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();
    QTimer::singleShot(200,this,SLOT(slotKeyEnable()));

}
void objui::slotShowLNBfreq()
{
    m_numEditor.setNum64(m_para.m_LNBfreq,m_para.m_maxTDMfreq,m_para.m_minTDMfreq,-1,2,-2);
    zeroFB(0);
    showBUCLNB();
    strXY(QString("LNB本振频率"),0,48,0,0x0f);

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();
    QTimer::singleShot(200,this,SLOT(slotKeyEnable()));

}

void objui::slotShowLogo()
{
#if 0
    QFile f("/root/qt/logo16.dat");
    if(f.open(QIODevice::ReadOnly)){
        QByteArray ba=f.readAll();
        Fill_BlockP((unsigned char*)ba.data(),0,63,0,63);
        emit sigFlush();
        f.close();
        qDebug(" show logo");
    }
    else qDebug(" show logo error");
#endif
    QTimer::singleShot(1000,this,SIGNAL(sigStateTransitionNext()));
}
void objui::showP3a()
{
    int y0=0,y1=16,y2=32,y3=48;
    char buf[40];

    strXY("接收调制方式",0,y0);
    strXY("接 收 速 率",4,y1);
    switch(m_para.m_rxPSK){
    case objPara::Mod_qpsk12:
        centerXY("1/2 QPSK",6*16,y0,10*16,16,1,1);
        break;
    case objPara::Mod_8psk12:
        centerXY("1/2 8PSK",6*16,y0,10*16,16,1,1);
        break;
    case objPara::Mod_qpsk34:
        centerXY("3/4 QPSK",6*16,y0,10*16,16,1,1);
        break;
    default:
        centerXY("3/4 8PSK",6*16,y0,10*16,16,1,1);
        break;
    }
    //sprintf(buf,"%d kbps",m_para.m_TxRate);
    //centerXY(buf,16*4,32,256-16*4,16,1,1,0x0f,0);
    sprintf(buf,"%d kbps",m_para.m_rxRate);
    centerXY(buf,16*6,y1,256-16*6,16,1,1);

    strXY("发送调制方式",0,y2);
    strXY("发 送 速 率",4,y3);
    switch(m_para.m_txPSK){
    case objPara::Mod_qpsk12:
        centerXY("1/2 QPSK",6*16,y2,10*16,16,1,1);
        break;
    case objPara::Mod_8psk12:
        centerXY("1/2 8PSK",6*16,y2,10*16,16,1,1);
        break;
    case objPara::Mod_qpsk34:
        centerXY("3/4 QPSK",6*16,y2,10*16,16,1,1);
        break;
    default:
        centerXY("3/4 8PSK",6*16,y2,10*16,16,1,1);
        break;
    }
    sprintf(buf,"%d kbps",m_para.m_txRate);
    centerXY(buf,16*6,y3,256-16*6,16,1,1);

    strXY("2/3",256-8*3,48,0x0f,0);
}
void objui::showP3()
{
    char buf[40];

    strXY("接收调制方式",0,11);
    strXY("接 收 速 率",4,38);
    switch(m_para.m_rxPSK){
    case objPara::Mod_qpsk12:
        centerXY("1/2 QPSK",6*16,11,10*16,16,1,1);
        break;
    case objPara::Mod_8psk12:
        centerXY("1/2 8PSK",6*16,11,10*16,16,1,1);
        break;
    case objPara::Mod_qpsk34:
        centerXY("3/4 QPSK",6*16,11,10*16,16,1,1);
        break;
    default:
        centerXY("3/4 8PSK",6*16,11,10*16,16,1,1);
        break;
    }
    //sprintf(buf,"%d kbps",m_para.m_TxRate);
    //centerXY(buf,16*4,32,256-16*4,16,1,1,0x0f,0);
    sprintf(buf,"%d kbps",m_para.m_rxRate);
    centerXY(buf,16*6,38,256-16*6,16,1,1);

    strXY("2/4",256-8*3,48,0x0f,0);

}
void objui::showP4()
{
    char buf[40];

    strXY("发送调制方式",0,11);
    strXY("发 送 速 率",4,38);
    switch(m_para.m_txPSK){
    case objPara::Mod_qpsk12:
        centerXY("1/2 QPSK",6*16,11,10*16,16,1,1);
        break;
    case objPara::Mod_8psk12:
        centerXY("1/2 8PSK",6*16,11,10*16,16,1,1);
        break;
    case objPara::Mod_qpsk34:
        centerXY("3/4 QPSK",6*16,11,10*16,16,1,1);
        break;
    default:
        centerXY("3/4 8PSK",6*16,11,10*16,16,1,1);
        break;
    }
    sprintf(buf,"%d kbps",m_para.m_txRate);
    centerXY(buf,16*6,38,256-16*6,16,1,1);
    //sprintf(buf,"%d kbps",m_para.m_RxRate);
    //centerXY(buf,16*6,38,256-16*6,16,1,1);

    strXY("3/4",256-8*3,48,0x0f,0);

}
void objui::slotShowP30a()
{
    m_numEditor.setPSK(m_para.m_rxPSK);
    zeroFB(0);
    showP3a();
    strXY("接收调制方式",0,0,0,0x0f);

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);
    emit sigFlush();
    slotKeyEnable();

}
void objui::slotShowP31a()
{
    m_numEditor.setArrayRate(m_para.m_rxPSK);
    m_numEditor.m_num = m_para.m_rxRate;
    zeroFB(0);
    showP3a();
    strXY("接 收 速 率",4,16,0,0x0f);

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);
    emit sigFlush();

    slotKeyEnable();

}
void objui::slotShowP40a()
{
    m_numEditor.setPSK(m_para.m_txPSK);
    zeroFB(0);
    showP3a();
    strXY("发送调制方式",0,32,0,0x0f);

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);
    emit sigFlush();

    slotKeyEnable();

}
void objui::slotShowP41a()
{
    m_numEditor.setArrayRate(m_para.m_txPSK);
    m_numEditor.m_num = m_para.m_txRate;
    zeroFB(0);
    showP3a();
    strXY("发 送 速 率",4,48,0,0x0f);

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);
    emit sigFlush();

    slotKeyEnable();

}

void objui::slotShowP30()
{
    m_numEditor.setPSK(m_para.m_rxPSK);
    zeroFB(0);
    showP3();
    strXY("接收调制方式",0,11,0,0x0f);

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);
    emit sigFlush();
    slotKeyEnable();
}
void objui::slotShowP31()
{
    zeroFB(0);
    showP3();
    strXY("接 收 速 率",4,38,0,0x0f);

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);
    emit sigFlush();

    slotKeyEnable();
}
void objui::slotShowP32()
{
    zeroFB(0);
    showP3();
    strXY("32",0,0,0,0x0f);

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);
    emit sigFlush();

    slotKeyEnable();
}
void objui::slotShowP40()
{
    m_numEditor.setPSK(m_para.m_txPSK);
    zeroFB(0);
    showP4();
    strXY("发送调制方式",0,11,0,0x0f);

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);
    emit sigFlush();

    slotKeyEnable();
}
void objui::slotShowP41()
{
    zeroFB(0);
    showP4();
    strXY("发 送 速 率",4,38,0,0x0f);

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);
    emit sigFlush();

    slotKeyEnable();
}
void objui::slotShowP42()
{
    zeroFB(0);
    showP4();
    strXY("42",0,0,0,0x0f);

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);
    emit sigFlush();

    slotKeyEnable();
}

void objui::showPSK(int psk)
{
    strXY("[ ] 1/2 QPSK",2*8,24);
    strXY("[ ] 1/2 8PSK",18*8,24);
    strXY("[ ] 3/4 QPSK",2*8,44);
    strXY("[ ] 3/4 8PSK",18*8,44);

    switch(psk){
    case objPara::Mod_qpsk12:
        strXY("[*]",2*8,24);
        break;
    case objPara::Mod_8psk12:
        strXY("[*]",18*8,24);
        break;
    case objPara::Mod_qpsk34:
        strXY("[*]",2*8,44);
        break;
    case objPara::Mod_8psk34:
        strXY("[*]",18*8,44);
        break;
    default:
        break;
    }
}

void objui::slotShowEditRxPSK()
{
    zeroFB(0);
    showPSK(m_numEditor.m_nPSK);
    centerXY("接 收 调 制 方 式",0,4,256,16,1,1);

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);
    emit sigFlush();

    slotKeyEnable();
}
void objui::slotShowEditTxPSK()
{
    zeroFB(0);
    showPSK(m_numEditor.m_nPSK);
    centerXY("发 送 调 制 方 式",0,4,256,16,1,1);

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);
    emit sigFlush();

    slotKeyEnable();
}
void objui::showTestCtrl()
{
    int c=0x0f;
    char buf[40];
    if(m_para.m_testON){
        centerXY("关闭单载波模式",0,0,256,32,1,1);
        c = 0x1;
    }
    else{
        centerXY("打开单载波模式",0,0,256,32,1,1);
        c = 0x0f;
    }
    strXY("发射频点",32,32,c);
    strXY("发射功率",32,48,c);
    sprintf(buf,"%.4f MHz",0.000001*m_para.m_testFreq);
    centerXY(buf,64+32,32,256-64-64,16,2,1,c);
    sprintf(buf,"%.2f dBm",0.01*m_para.m_testPwr);
    centerXY(buf,64+32,48,256-64-64,16,2,1,c);
}

void objui::slotShowMenuTestCtrl()
{
    //m_numEditor.setPSK(m_para.m_rxPSK);
    zeroFB(0);
    showTestCtrl();
    if(m_para.m_testON){
        centerXY("关闭单载波模式",0,0,256,32,1,1,0,0x0f);
        //centerXY("关  闭",80,0,256-80,16,1,1,0,0x0f);
    }
    else{
        centerXY("打开单载波模式",0,0,256,32,1,1,0,0x0f);
        //centerXY("打  开",80,0,256-80,16,1,1,0,0x0f);
    }

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);
    emit sigFlush();
    slotKeyEnable();

}
void objui::slotShowMenuTestFreq()
{
    //m_numEditor.setPSK(m_para.m_rxPSK);
    m_numEditor.setNum64(m_para.m_testFreq,m_para.m_maxTxFreq,m_para.m_minTxFreq,-1,2,-1);
    zeroFB(0);
    showTestCtrl();
    strXY("发射频点",32,32,0,0x0f);

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);
    emit sigFlush();
    slotKeyEnable();

}
void objui::slotShowMenuTestPwr()
{
    m_numEditor.setNum64(m_para.m_testPwr,m_para.m_maxPower,m_para.m_minPower,-1,0,0);
    zeroFB(0);
    showTestCtrl();
    strXY("发射功率",32,48,0,0x0f);

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);
    emit sigFlush();
    slotKeyEnable();

}

void objui::slotShowEditTestPwr()
{
    zeroFB(0);
    char buf[20];

    const QLocale & locale = QLocale::c();
    QString s=locale.toString(0.01 * m_numEditor.m_i64);
    sprintf(buf,"%.2f",0.01*m_numEditor.m_i64);

    centerXY("单载波发射功率",0,10,256,16,1,1,0x0f,0);
    centerXY(buf,0,64-12-16,(11+6)<<3,16,2,1,0x0f,0);
    strXY(" dBm",(11+6)<<3,64-12-16,0x0f,0);
    underLine(0,64-12-16,11+6-1-m_numEditor.getCursor(),0x0f);

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();
    //m_bEnableKeyboard = true;
    QTimer::singleShot(200,this,SLOT(slotKeyEnable()));

}
void objui::slotShowEditTestFreq()
{
    char buf[40];
    zeroFB(0);

    centerXY("单载波发射频点",0,10,256,16,1,1,0x0f,0);
    m_numEditor.getStrFreq(buf);
    centerXY(buf,0,64-12-16,(9+9)<<3,16,2,1,0x0f,0);
    strXY("  MHz",(9+9)<<3,64-12-16,0x0f,0);
    underLine(0,64-12-16,9+9-1-m_numEditor.getCursorMHz(),0x0f);

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();
    //m_bEnableKeyboard = true;
    QTimer::singleShot(200,this,SLOT(slotKeyEnable()));

}
void objui::doStartTest()
{
    struct TestSignalStruct s;
    std::string stdstr;

    s.txFrequence = m_para.m_testFreq;
    s.txIFPower = m_para.m_testPwr * 0.01;
    stdstr=m_cu.startTestSignal(s);
    writeTcp(QString::fromStdString(stdstr).toUtf8());

}
void objui::doStopTest()
{
    std::string stdstr;

    stdstr=m_cu.stopTestSignal();
    writeTcp(QString::fromStdString(stdstr).toUtf8());

}
