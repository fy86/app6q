#include "objui.h"

#include "kt0000.h"

#include "ketmenu00.h"

objui::objui(QObject *parent) :
    objPage(parent)
{
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
    connect(m_cu.getRecvResponseObject(),SIGNAL(sigRadioLinkState(QByteArray)),this,SLOT(slotRadioLinkState(QByteArray)));
    connect(m_cu.getRecvResponseObject(),SIGNAL(sigP2PModeParam(QByteArray)),this,SLOT(slotP2PmodeParam(QByteArray)));
    connect(m_cu.getRecvResponseObject(),SIGNAL(sigTDMConfig(QByteArray)),this,SLOT(slotTDMConfig(QByteArray)));
    connect(m_cu.getRecvResponseObject(),SIGNAL(sigPowerConfig(QByteArray)),this,SLOT(slotPowerConfig(QByteArray)));

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

    m_pStateAbout = new QState(m_pStateGroupTimeout);
    m_pStateLogo = new QState(m_pMachine);

    m_pStateMsgZZHJ = new QState(m_pMachine);
    m_pStateMsgHJCG = new QState(m_pMachine);
    m_pStateMsgZZGD = new QState(m_pMachine);
    m_pStateMsgGDCG = new QState(m_pMachine);

    m_pStateMsgZZRW = new QState(m_pMachine);
    m_pStateMsgZZTW = new QState(m_pMachine);

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
    m_pStateEditorPower = new QState(m_pStateGroupTimeout);

    m_pStateEditorTDM = new QState(m_pStateGroupTimeout);
    m_pStateEditorTDM2 = new QState(m_pStateGroupTimeout);
    m_pStateEditorTxRateCentral = new QState(m_pStateGroupTimeout);
    m_pStateEditorRxRateCentral = new QState(m_pStateGroupTimeout);
    m_pStateEditorPowerCentral = new QState(m_pStateGroupTimeout);

    m_pStateParaPage1 = new QState(m_pStateGroupTimeout);
    m_pStateParaPage11 = new QState(m_pStateGroupTimeout);
    //m_pStateParaPage12 = new QState(m_pStateGroupTimeout);
    //m_pStateParaPage13 = new QState(m_pStateGroupTimeout);
    m_pStateParaPage2 = new QState(m_pStateGroupTimeout);
    m_pStateParaPage21 = new QState(m_pStateGroupTimeout);
    m_pStateParaPage22 = new QState(m_pStateGroupTimeout);
    m_pStateParaPage23 = new QState(m_pStateGroupTimeout);

    m_pStateParaPage30 = new QState(m_pStateGroupTimeout);
    m_pStateParaPage31 = new QState(m_pStateGroupTimeout);
    //m_pStateParaPage32 = new QState(m_pStateGroupTimeout);
    m_pStateParaPage40 = new QState(m_pStateGroupTimeout);
    m_pStateParaPage41 = new QState(m_pStateGroupTimeout);
    //m_pStateParaPage42 = new QState(m_pStateGroupTimeout);

    m_pStateEditRxPSK = new QState(m_pStateGroupTimeout);
    m_pStateEditTxPSK = new QState(m_pStateGroupTimeout);


    m_pStateParaPage1c = new QState(m_pStateGroupTimeout);
    m_pStateParaPage11c = new QState(m_pStateGroupTimeout);
    m_pStateParaPage12c = new QState(m_pStateGroupTimeout);
    m_pStateParaPage13c = new QState(m_pStateGroupTimeout);
    m_pStateParaPage2c = new QState(m_pStateGroupTimeout);
    m_pStateParaPage21c = new QState(m_pStateGroupTimeout);
    m_pStateParaPage22c = new QState(m_pStateGroupTimeout);

    m_pStateGroupTimeout->setInitialState(m_pStateMenuCtrl);
    m_pMachine->setInitialState(m_pStateLogo);//m_pStateStatusPage1);

    connect(m_pStateStatusPage1,SIGNAL(entered()),this,SLOT(slotShowStatusPage1()));
    connect(m_pStateStatusPage2,SIGNAL(entered()),this,SLOT(slotShowStatusPage2()));

    connect(m_pStateAbout,SIGNAL(entered()),this,SLOT(slotShowAbout()));
    connect(m_pStateLogo,SIGNAL(entered()),this,SLOT(slotShowLogo()));

    connect(m_pStateMsgZZHJ,SIGNAL(entered()),this,SLOT(slotShowMsgZZHJ()));
    connect(m_pStateMsgHJCG,SIGNAL(entered()),this,SLOT(slotShowMsgHJCG()));
    connect(m_pStateMsgZZGD,SIGNAL(entered()),this,SLOT(slotShowMsgZZGD()));
    connect(m_pStateMsgGDCG,SIGNAL(entered()),this,SLOT(slotShowMsgGDCG()));

    connect(m_pStateMsgZZRW,SIGNAL(entered()),this,SLOT(slotShowMsgZZRW()));
    connect(m_pStateMsgZZTW,SIGNAL(entered()),this,SLOT(slotShowMsgZZTW()));

    connect(m_pStateMenuCtrl,SIGNAL(entered()),this,SLOT(slotShowMenu00()));
    connect(m_pStateMenuPara,SIGNAL(entered()),this,SLOT(slotShowMenu01()));
    connect(m_pStateMenuWorkMode,SIGNAL(entered()),this,SLOT(slotShowMenu02()));

    connect(m_pStateWorkMode1,SIGNAL(entered()),this,SLOT(slotShowMode1()));
    connect(m_pStateWorkMode2,SIGNAL(entered()),this,SLOT(slotShowMode2()));

    connect(m_pStateDevMode1,SIGNAL(entered()),this,SLOT(slotShowDevMode1()));
    connect(m_pStateDevMode2,SIGNAL(entered()),this,SLOT(slotShowDevMode2()));

    connect(m_pStateMenuCall,SIGNAL(entered()),this,SLOT(slotShowMenuCall()));

    connect(m_pStateParaPage30,SIGNAL(entered()),this,SLOT(slotShowP30()));
    connect(m_pStateParaPage31,SIGNAL(entered()),this,SLOT(slotShowP31()));
    //connect(m_pStateParaPage32,SIGNAL(entered()),this,SLOT(slotShowP32()));
    connect(m_pStateParaPage40,SIGNAL(entered()),this,SLOT(slotShowP40()));
    connect(m_pStateParaPage41,SIGNAL(entered()),this,SLOT(slotShowP41()));
    //connect(m_pStateParaPage42,SIGNAL(entered()),this,SLOT(slotShowP42()));

    connect(m_pStateEditRxPSK,SIGNAL(entered()),this,SLOT(slotShowEditRxPSK()));
    connect(m_pStateEditTxPSK,SIGNAL(entered()),this,SLOT(slotShowEditTxPSK()));


    connect(m_pStateParaPage1,SIGNAL(entered()),this,SLOT(slotShowParaPage1()));
    connect(m_pStateParaPage11,SIGNAL(entered()),this,SLOT(slotShowParaPage11()));
    //connect(m_pStateParaPage12,SIGNAL(entered()),this,SLOT(slotShowParaPage12()));
    //connect(m_pStateParaPage13,SIGNAL(entered()),this,SLOT(slotShowParaPage13()));
    connect(m_pStateParaPage2,SIGNAL(entered()),this,SLOT(slotShowParaPage2()));
    connect(m_pStateParaPage21,SIGNAL(entered()),this,SLOT(slotShowParaPage21()));
    connect(m_pStateParaPage22,SIGNAL(entered()),this,SLOT(slotShowParaPage22()));
    connect(m_pStateParaPage23,SIGNAL(entered()),this,SLOT(slotShowParaPage23()));

    connect(m_pStateParaPage1c,SIGNAL(entered()),this,SLOT(slotShowParaPage1c()));
    connect(m_pStateParaPage11c,SIGNAL(entered()),this,SLOT(slotShowParaPage11c()));
    connect(m_pStateParaPage12c,SIGNAL(entered()),this,SLOT(slotShowParaPage12c()));
    connect(m_pStateParaPage13c,SIGNAL(entered()),this,SLOT(slotShowParaPage13c()));
    connect(m_pStateParaPage2c,SIGNAL(entered()),this,SLOT(slotShowParaPage2c()));
    connect(m_pStateParaPage21c,SIGNAL(entered()),this,SLOT(slotShowParaPage21c()));
    connect(m_pStateParaPage22c,SIGNAL(entered()),this,SLOT(slotShowParaPage22c()));

    connect(m_pStateEditorBUCfreq,SIGNAL(entered()),this,SLOT(slotShowEditBUCfreq()));
    connect(m_pStateEditorLNBfreq,SIGNAL(entered()),this,SLOT(slotShowEditLNBfreq()));

    connect(m_pStateEditorTxFreq,SIGNAL(entered()),this,SLOT(slotShowEditTxFreq()));
    connect(m_pStateEditorRxFreq,SIGNAL(entered()),this,SLOT(slotShowEditRxFreq()));
    connect(m_pStateEditorTxRate,SIGNAL(entered()),this,SLOT(slotShowEditTxRate()));
    connect(m_pStateEditorRxRate,SIGNAL(entered()),this,SLOT(slotShowEditRxRate()));
    connect(m_pStateEditorPower,SIGNAL(entered()),this,SLOT(slotShowEditPower()));

    connect(m_pStateEditorTDM,SIGNAL(entered()),this,SLOT(slotShowEditTDM()));
    connect(m_pStateEditorTDM2,SIGNAL(entered()),this,SLOT(slotShowEditTDM2()));
    connect(m_pStateEditorTxRateCentral,SIGNAL(entered()),this,SLOT(slotShowEditTxRateCentral()));
    connect(m_pStateEditorRxRateCentral,SIGNAL(entered()),this,SLOT(slotShowEditRxRateCentral()));
    connect(m_pStateEditorPowerCentral,SIGNAL(entered()),this,SLOT(slotShowEditPowerCentral()));

    m_pStateGroupTimeout->addTransition(this,SIGNAL(sigStateTransitionTimeout()),m_pStateStatusPage1);

    ketStatus1 *pKetStatus1 = new ketStatus1(this);
    m_pStateStatusPage1->addTransition(pKetStatus1);
    m_pStateStatusPage1->addTransition(this,SIGNAL(sigStateTransitionDown()),m_pStateStatusPage2);
    m_pStateStatusPage1->addTransition(this,SIGNAL(sigStateTransitionNext()),m_pStateMenuCtrl);
    m_pStateStatusPage1->addTransition(this,SIGNAL(sigStateTransition1s()),m_pStateStatusPage1);
    ketStatus2 *pKetStatus2 = new ketStatus2(this);
    m_pStateStatusPage2->addTransition(pKetStatus2);
    m_pStateStatusPage2->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateStatusPage1);
    m_pStateStatusPage2->addTransition(this,SIGNAL(sigStateTransitionNext()),m_pStateMenuCtrl);

    ketAbout *pKetAbout = new ketAbout(this);
    m_pStateAbout->addTransition(pKetAbout);
    m_pStateAbout->addTransition(this,SIGNAL(sigStateTransitionNext()),m_pStateMenuCtrl);

    m_pStateMsgZZHJ->addTransition(this,SIGNAL(sigStateTransitionNext()),m_pStateMsgHJCG);
    m_pStateMsgHJCG->addTransition(this,SIGNAL(sigStateTransitionNext()),m_pStateStatusPage1);
    m_pStateMsgZZGD->addTransition(this,SIGNAL(sigStateTransitionNext()),m_pStateMsgGDCG);
    m_pStateMsgGDCG->addTransition(this,SIGNAL(sigStateTransitionNext()),m_pStateStatusPage1);

    m_pStateMsgZZRW->addTransition(this,SIGNAL(sigStateTransitionNext()),m_pStateStatusPage1);
    m_pStateMsgZZTW->addTransition(this,SIGNAL(sigStateTransitionNext()),m_pStateStatusPage1);

    m_pStateLogo->addTransition(this,SIGNAL(sigStateTransitionNext()),m_pStateStatusPage1);


    ketMenuCtrl *pKetMenuCtrl = new ketMenuCtrl(this);
    m_pStateMenuCtrl->addTransition(pKetMenuCtrl);
    m_pStateMenuCtrl->addTransition(this,SIGNAL(sigStateTransitionDown()),m_pStateMenuPara);
    m_pStateMenuCtrl->addTransition(this,SIGNAL(sigStateTransitionUp()),m_pStateMenuWorkMode);
    m_pStateMenuCtrl->addTransition(this,SIGNAL(sigStateTransitionEnter()),m_pStateMenuCall);
    m_pStateMenuCtrl->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateStatusPage1);
    ketMenuPara *pKetMenuPara = new ketMenuPara(this);
    m_pStateMenuPara->addTransition(pKetMenuPara);
    m_pStateMenuPara->addTransition(this,SIGNAL(sigStateTransitionDown()),m_pStateMenuWorkMode);
    m_pStateMenuPara->addTransition(this,SIGNAL(sigStateTransitionUp()),m_pStateMenuCtrl);
    m_pStateMenuPara->addTransition(this,SIGNAL(sigStateTransitionP2P()),m_pStateParaPage1);
    m_pStateMenuPara->addTransition(this,SIGNAL(sigStateTransitionCentral()),m_pStateParaPage1c);
    m_pStateMenuPara->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateStatusPage1);

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

    ketRxPSKeditor *pKetEditRxPSK = new ketRxPSKeditor(this);
    m_pStateEditRxPSK->addTransition(pKetEditRxPSK);
    m_pStateEditRxPSK->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateParaPage30);
    ketTxPSKeditor *pKetEditTxPSK = new ketTxPSKeditor(this);
    m_pStateEditTxPSK->addTransition(pKetEditTxPSK);
    m_pStateEditTxPSK->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateParaPage40);


    ketParaPage1 *pKetParaPage30 = new ketParaPage1(this);
    m_pStateParaPage30->addTransition(pKetParaPage30);
    m_pStateParaPage30->addTransition(this,SIGNAL(sigStateTransitionDown()),m_pStateParaPage31);
    m_pStateParaPage30->addTransition(this,SIGNAL(sigStateTransitionUp()),m_pStateParaPage11);
    m_pStateParaPage30->addTransition(this,SIGNAL(sigStateTransitionLeft()),m_pStateParaPage40);
    m_pStateParaPage30->addTransition(this,SIGNAL(sigStateTransitionRight()),m_pStateParaPage40);
    m_pStateParaPage30->addTransition(this,SIGNAL(sigStateTransitionNext()),m_pStateEditRxPSK);
    m_pStateParaPage30->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateMenuPara);

    ketParaPage1 *pKetParaPage31 = new ketParaPage1(this);
    m_pStateParaPage31->addTransition(pKetParaPage31);
    m_pStateParaPage31->addTransition(this,SIGNAL(sigStateTransitionDown()),m_pStateParaPage40);
    m_pStateParaPage31->addTransition(this,SIGNAL(sigStateTransitionUp()),m_pStateParaPage30);
    m_pStateParaPage31->addTransition(this,SIGNAL(sigStateTransitionLeft()),m_pStateParaPage40);
    m_pStateParaPage31->addTransition(this,SIGNAL(sigStateTransitionRight()),m_pStateParaPage40);
    m_pStateParaPage31->addTransition(this,SIGNAL(sigStateTransitionNext()),m_pStateParaPage31);
    m_pStateParaPage31->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateMenuPara);
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
    ketParaPage1 *pKetParaPage40 = new ketParaPage1(this);
    m_pStateParaPage40->addTransition(pKetParaPage40);
    m_pStateParaPage40->addTransition(this,SIGNAL(sigStateTransitionDown()),m_pStateParaPage41);
    m_pStateParaPage40->addTransition(this,SIGNAL(sigStateTransitionUp()),m_pStateParaPage31);
    m_pStateParaPage40->addTransition(this,SIGNAL(sigStateTransitionLeft()),m_pStateParaPage2);
    m_pStateParaPage40->addTransition(this,SIGNAL(sigStateTransitionRight()),m_pStateParaPage2);
    m_pStateParaPage40->addTransition(this,SIGNAL(sigStateTransitionNext()),m_pStateEditTxPSK);
    m_pStateParaPage40->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateMenuPara);

    ketParaPage1 *pKetParaPage41 = new ketParaPage1(this);
    m_pStateParaPage41->addTransition(pKetParaPage41);
    m_pStateParaPage41->addTransition(this,SIGNAL(sigStateTransitionDown()),m_pStateParaPage2);
    m_pStateParaPage41->addTransition(this,SIGNAL(sigStateTransitionUp()),m_pStateParaPage40);
    m_pStateParaPage41->addTransition(this,SIGNAL(sigStateTransitionLeft()),m_pStateParaPage2);
    m_pStateParaPage41->addTransition(this,SIGNAL(sigStateTransitionRight()),m_pStateParaPage2);
    m_pStateParaPage41->addTransition(this,SIGNAL(sigStateTransitionNext()),m_pStateParaPage41);
    m_pStateParaPage41->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateMenuPara);
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
    m_pStateParaPage2->addTransition(this,SIGNAL(sigStateTransitionUp()),m_pStateParaPage41);
    m_pStateParaPage2->addTransition(this,SIGNAL(sigStateTransitionLeft()),m_pStateParaPage41);
    m_pStateParaPage2->addTransition(this,SIGNAL(sigStateTransitionRight()),m_pStateParaPage1);
    m_pStateParaPage2->addTransition(this,SIGNAL(sigStateTransitionNext()),m_pStateEditorPower);
    m_pStateParaPage2->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateMenuPara);

    ketParaPage1 *pKetParaPage21 = new ketParaPage1(this);
    m_pStateParaPage21->addTransition(pKetParaPage21);
    m_pStateParaPage21->addTransition(this,SIGNAL(sigStateTransitionDown()),m_pStateParaPage22);
    m_pStateParaPage21->addTransition(this,SIGNAL(sigStateTransitionUp()),m_pStateParaPage2);
    m_pStateParaPage21->addTransition(this,SIGNAL(sigStateTransitionLeft()),m_pStateParaPage41);
    m_pStateParaPage21->addTransition(this,SIGNAL(sigStateTransitionRight()),m_pStateParaPage1);
    m_pStateParaPage21->addTransition(this,SIGNAL(sigStateTransitionNext()),m_pStateDevMode1);
    m_pStateParaPage21->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateMenuPara);

    ketParaPage1 *pKetParaPage22 = new ketParaPage1(this);
    m_pStateParaPage22->addTransition(pKetParaPage22);
    m_pStateParaPage22->addTransition(this,SIGNAL(sigStateTransitionDown()),m_pStateParaPage23);
    m_pStateParaPage22->addTransition(this,SIGNAL(sigStateTransitionUp()),m_pStateParaPage21);
    m_pStateParaPage22->addTransition(this,SIGNAL(sigStateTransitionLeft()),m_pStateParaPage41);
    m_pStateParaPage22->addTransition(this,SIGNAL(sigStateTransitionRight()),m_pStateParaPage1);
    m_pStateParaPage22->addTransition(this,SIGNAL(sigStateTransitionNext()),m_pStateEditorBUCfreq);
    m_pStateParaPage22->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateMenuPara);

    ketParaPage1 *pKetParaPage23 = new ketParaPage1(this);
    m_pStateParaPage23->addTransition(pKetParaPage23);
    m_pStateParaPage23->addTransition(this,SIGNAL(sigStateTransitionDown()),m_pStateParaPage1);
    m_pStateParaPage23->addTransition(this,SIGNAL(sigStateTransitionUp()),m_pStateParaPage22);
    m_pStateParaPage23->addTransition(this,SIGNAL(sigStateTransitionLeft()),m_pStateParaPage41);
    m_pStateParaPage23->addTransition(this,SIGNAL(sigStateTransitionRight()),m_pStateParaPage1);
    m_pStateParaPage23->addTransition(this,SIGNAL(sigStateTransitionNext()),m_pStateEditorLNBfreq);
    m_pStateParaPage23->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateMenuPara);

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

    ketBUCfreqEditor *pKetBUCfreqEditor = new ketBUCfreqEditor(this);
    m_pStateEditorBUCfreq->addTransition(pKetBUCfreqEditor);
    m_pStateEditorBUCfreq->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateParaPage22);
    ketLNBfreqEditor *pKetLNBfreqEditor = new ketLNBfreqEditor(this);
    m_pStateEditorLNBfreq->addTransition(pKetLNBfreqEditor);
    m_pStateEditorLNBfreq->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateParaPage23);



    ketTxFreqEditor *pKetTxFreqEditor = new ketTxFreqEditor(this);
    m_pStateEditorTxFreq->addTransition(pKetTxFreqEditor);
    m_pStateEditorTxFreq->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateParaPage1);
    ketRxFreqEditor *pKetRxFreqEditor = new ketRxFreqEditor(this);
    m_pStateEditorRxFreq->addTransition(pKetRxFreqEditor);
    m_pStateEditorRxFreq->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateParaPage11);
#if 0
    ketTxRateEditor *pKetTxRateEditor = new ketTxRateEditor(this);
    m_pStateEditorTxRate->addTransition(pKetTxRateEditor);
    m_pStateEditorTxRate->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateParaPage12);
    ketRxRateEditor *pKetRxRateEditor = new ketRxRateEditor(this);
    m_pStateEditorRxRate->addTransition(pKetRxRateEditor);
    m_pStateEditorRxRate->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateParaPage13);
#endif
    ketPowerEditor *pKetPowerEditor = new ketPowerEditor(this);
    m_pStateEditorPower->addTransition(pKetPowerEditor);
    m_pStateEditorPower->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateParaPage2);


    ketTDMeditor *pKetTDMeditor = new ketTDMeditor(this);
    m_pStateEditorTDM->addTransition(pKetTDMeditor);
    m_pStateEditorTDM->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateParaPage1c);

    ketTDM2editor *pKetTDM2editor = new ketTDM2editor(this);
    m_pStateEditorTDM2->addTransition(pKetTDM2editor);
    m_pStateEditorTDM2->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateParaPage11c);
    ketTxRateCentralEditor *pKetTxRateCentralEditor = new ketTxRateCentralEditor(this);
    m_pStateEditorTxRateCentral->addTransition(pKetTxRateCentralEditor);
    m_pStateEditorTxRateCentral->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateParaPage12c);
    ketRxRateCentralEditor *pKetRxRateCentralEditor = new ketRxRateCentralEditor(this);
    m_pStateEditorRxRateCentral->addTransition(pKetRxRateCentralEditor);
    m_pStateEditorRxRateCentral->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateParaPage13c);
    ketPowerCentralEditor *pKetPowerCentralEditor = new ketPowerCentralEditor(this);
    m_pStateEditorPowerCentral->addTransition(pKetPowerCentralEditor);
    m_pStateEditorPowerCentral->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateParaPage12c);




    ketMenuWorkMode *pKetMenuWorkMode = new ketMenuWorkMode(this);
    m_pStateMenuWorkMode->addTransition(pKetMenuWorkMode);
    m_pStateMenuWorkMode->addTransition(this,SIGNAL(sigStateTransitionRight()),m_pStateAbout);
    m_pStateMenuWorkMode->addTransition(this,SIGNAL(sigStateTransitionDown()),m_pStateMenuCtrl);
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
    m_pStateDevMode1->addTransition(this,SIGNAL(sigStateTransitionCentral()),m_pStateParaPage21c);
    m_pStateDevMode1->addTransition(this,SIGNAL(sigStateTransitionEnter()),m_pStateDevMode1);
    ketDevMode2 *pKetDevMode2 = new ketDevMode2(this);
    m_pStateDevMode2->addTransition(pKetDevMode2);
    m_pStateDevMode2->addTransition(this,SIGNAL(sigStateTransitionDown()),m_pStateDevMode1);
    m_pStateDevMode2->addTransition(this,SIGNAL(sigStateTransitionP2P()),m_pStateParaPage21);
    m_pStateDevMode2->addTransition(this,SIGNAL(sigStateTransitionCentral()),m_pStateParaPage21c);
    m_pStateDevMode2->addTransition(this,SIGNAL(sigStateTransitionEnter()),m_pStateDevMode2);

    // p2p call menu
    ketMenuCall *pKetMenuCall = new ketMenuCall(this);
    m_pStateMenuCall->addTransition(pKetMenuCall);
    m_pStateMenuCall->addTransition(this,SIGNAL(sigStateTransitionBackspace()),m_pStateMenuCtrl);
    m_pStateMenuCall->addTransition(this,SIGNAL(sigStateTransitionUp()),m_pStateMsgZZHJ);
    m_pStateMenuCall->addTransition(this,SIGNAL(sigStateTransitionDown()),m_pStateMsgZZGD);
    m_pStateMenuCall->addTransition(this,SIGNAL(sigStateTransitionLeft()),m_pStateMsgZZRW);
    m_pStateMenuCall->addTransition(this,SIGNAL(sigStateTransitionRight()),m_pStateMsgZZTW);




    m_pMachine->start();

    m_pTimer1s->start();

    QTimer::singleShot(500,this,SLOT(slotGetCUstate()));

    qDebug("     end objui.initMachine");
}
void objui::slotGetCUstate()
{
    std::string stdstr;

    // test
    //stdstr=m_cu.logoutNet();
    //writeTcp(QString::fromStdString(stdstr).toUtf8());

    stdstr=m_cu.getCUState();
    writeTcp(QString::fromStdString(stdstr).toUtf8());

    stdstr=m_cu.getRadioLinkState();
    writeTcp(QString::fromStdString(stdstr).toUtf8());

    stdstr=m_cu.getP2PModeParam();
    writeTcp(QString::fromStdString(stdstr).toUtf8());

}

void objui::doMenuPara()
{
    emit sigStateTransitionNext();
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

void objui::slotShowMenuCall()
{
    zeroFB(0);

    if(m_para.m_status==objPara::Status_idle){
        if(m_para.m_workMode==objPara::Mode_p2p){
            centerXY(QString("呼    叫"),0,12,256,16,1,1,0,0x0f);
            centerXY(QString("挂    断"),0,48-10,256,16,1,1,1);// gray
        }
        else{// central
            centerXY(QString("入    网"),0,12,256,16,1,1,0,0x0f);
            centerXY(QString("退    网"),0,48-10,256,16,1,1,1);// gray
        }

    }
    else{//  connected ...
        if(m_status.m_workMode == objPara::Mode_p2p){
            centerXY(QString("呼    叫"),0,12,256,16,1,1,1);// gray
            centerXY(QString("挂    断"),0,48-10,256,16,1,1,0,0x0f);
        }
        else{
            centerXY(QString("入    网"),0,12,256,16,1,1,1);// gray
            centerXY(QString("退    网"),0,48-10,256,16,1,1,0,0x0f);
        }
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
    qDebug("  == p2p.param------------tx.freq: %lld",pMode->txFrequence);
    qDebug("  p2p.param------------rx.freq: %lld",pMode->rxFrequence);
    qDebug("  p2p.param------------tx.rate: %lld",pMode->txBitrate);
    qDebug("  p2p.param------------rx.rate: %lld",pMode->rxBitrate);
    qDebug("  p2p.param------------power: %.2f",pMode->txIFPower);
    m_status.m_RxFreq = pMode->rxFrequence;
    m_status.m_TxFreq = pMode->txFrequence;
    m_status.m_RxRate = pMode->rxBitrate/1000;
    m_status.m_TxRate = pMode->txBitrate/1000;
    m_status.m_power100 = pMode->txIFPower * 100.;
    QString str=QString::fromStdString(pMode->dataCommMode);
    if(str.contains("bridge")){
        m_status.m_devMode = objPara::DevMode_bridge;
    }
    else{
        m_status.m_devMode = objPara::DevMode_router;
    }
}

void objui::slotRadioLinkState(QByteArray ba)
{
    qint64 i64;
    struct RadioLinkStateChanged *pRadio;
    pRadio = (struct RadioLinkStateChanged *)ba.data();
    qDebug("  --- %s ---------snr: %.2f",QTime::currentTime().toString("h:m:s").toLatin1().data(),pRadio->dataRecvLink.snr);
    m_status.m_fSNR = pRadio->dataRecvLink.snr;
    i64 = pRadio->dataRecvLink.frequency;
    if(i64>0){
        m_status.m_RxFreq = i64;
        m_status.m_RxRate = pRadio->dataRecvLink.datarate/1000;
    }
    i64 = pRadio->dataSendLink.frequency;
    if(i64>0){
        m_status.m_TxFreq = i64;
        m_status.m_TxRate = pRadio->dataSendLink.datarate/1000;
    }
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
void objui::slotPowerConfig(QByteArray ba)
{
    struct PowerStruct *p;
    p = (struct PowerStruct *)ba.data();
    m_para.m_power100Central = p->powerAdjust;
    qDebug(" =*****=  get.poweradj: %.2f  %lld",p->powerAdjust,m_para.m_power100Central);
}

void objui::slotCUState(QByteArray ba)
{
    struct CUState *p;
    p = (struct CUState*)ba.data();
    QString str=QString::fromStdString(p->mode);
#if 0
    if(0==str.compare(QString("offline"),Qt::CaseInsensitive)){
        qDebug("             ===========custate , idle");
        m_para.m_status = objPara::Status_idle;
        m_para.m_workMode = objPara::Mode_unknown;
    }
#endif
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
    }
    else{// "ncc_ctrl"
        QString str1=QString::fromStdString(p->state);
        if(0==str1.compare(QString("offline"),Qt::CaseInsensitive)){
            m_para.m_status=objPara::Status_idle;
            m_status.m_workMode = objPara::Mode_unknown;
        }
        else if(0==str1.compare(QString("waiting_tdm"),Qt::CaseInsensitive)){
            m_para.m_status=objPara::Status_waiting_tdm;
            m_status.m_workMode = objPara::Mode_central;
        }
        else if(0==str1.compare(QString("logining"),Qt::CaseInsensitive)){
            m_para.m_status=objPara::Status_logining;
            m_status.m_workMode = objPara::Mode_central;
        }
        else if(0==str1.compare(QString("logouting"),Qt::CaseInsensitive)){
            m_para.m_status=objPara::Status_logouting;
            m_status.m_workMode = objPara::Mode_central;
        }
        else if(0==str1.compare(QString("online"),Qt::CaseInsensitive)){
            m_status.m_workMode = objPara::Mode_central;
            QString str2=QString::fromStdString(p->usageState);
            if(0==str2.compare(QString("idle"),Qt::CaseInsensitive)){
                m_para.m_status=objPara::Status_online_idle;
            }
            else if(0==str2.compare(QString("p2p_call"),Qt::CaseInsensitive)){
                m_para.m_status=objPara::Status_online_p2p_call;
            }
            else m_para.m_status=objPara::Status_online_ncc_plan;

        }

    }
    qDebug() << QString::fromStdString(p->mode);
    qDebug("!!!! --------------- %s",QString::fromStdString(p->isdn).toLatin1().data());
    qDebug("-------------- %s",QString::fromStdString(p->name).toLatin1().data());
    qDebug("----------- %s",QString::fromStdString(p->state).toLatin1().data());
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

void objui::doCallP2P()
{
    struct P2PMode p2pmode;
    std::string stdstr;

    p2pmode.txFrequence = m_para.m_TxFreq;// + m_para.m_BUCfreq;
    p2pmode.txBitrate = m_para.m_TxRate*1000;
    p2pmode.txIFPower = 0.01 * m_para.m_power100;
    p2pmode.rxFrequence = m_para.m_RxFreq;// + m_para.m_LNBfreq;
    p2pmode.rxBitrate = m_para.m_RxRate*1000;
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

    p2pmode.txFrequence = m_para.m_TxFreq;// + m_para.m_BUCfreq;
    p2pmode.txBitrate = m_para.m_TxRate*1000;
    p2pmode.txIFPower = 0.01 * m_para.m_power100;
    p2pmode.rxFrequence = m_para.m_RxFreq;// + m_para.m_LNBfreq;
    p2pmode.rxBitrate = m_para.m_RxRate*1000;
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
void objui::showDataParaPage2()
{
    strXY("发送功率",0,0);// fasong gonglv 发送 功率
    strXY("业务类型",0,16);// yewu leixing 业务类型
    strXY("BUC本振",0,32);//
    strXY("LNB本振",0,48);// bianma fangshi 编码方式

    char buf[40];
    //const QLocale & locale = QLocale::c();
    //QString s;//=locale.toString(m_numEditor.m_i64);

    //s=locale.toString(0.01*m_para.m_power100) + " dBm";
    sprintf(buf,"%.2f dBm",0.01*m_para.m_power100);
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
    sprintf(buf,"%.4f MHz",0.000001*m_para.m_BUCfreq);
    centerXY(buf,16*4,32,256-16*4,16,1,1,0x0f,0);
    sprintf(buf,"%.4f MHz",0.000001*m_para.m_LNBfreq);
    centerXY(buf,16*4,48,256-16*4,16,1,1,0x0f,0);
    //centerXY("QPSK1/2",4*16,32,256-16*4,16,1,1,0x0f,0);

    strXY("4/4",256-8*3,48,0x0f,0);

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
    strXY("发送功率",0,0,0,0x0f);// fasong gonglv 发送 功率

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();
    //m_bEnableKeyboard = true;
    QTimer::singleShot(200,this,SLOT(slotKeyEnable()));


}
void objui::slotShowParaPage21()
{
    zeroFB(0);

    showDataParaPage2();
    strXY("业务类型",0,16,0,0x0f);// yewu leixing 业务类型

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
void objui::slotShowParaPage12c()
{
    //qDebug("     show menu.page.12");
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
// del
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
        str="呼叫通信";
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
    strXY(m_status.strTxRate().toLatin1().data(),128,32);
    strXY(m_status.strRxRate().toLatin1().data(),128,48);

}

// calling
// success
// offline
// p2p
// tdm_offline ,
void objui::slotShowStatusPage1()
{
    QString str;
    char buf[40];
    zeroFB(0);

    if(m_para.m_status==objPara::Status_idle){// idle
        switch(m_para.m_workMode){
        case objPara::Mode_p2p:
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
            strXY(m_para.strTxRate().toLatin1().data(),128,32);
            strXY(m_para.strRxRate().toLatin1().data(),128,48);
            break;
        case objPara::Mode_central:
            centerXY("空    闲",0,0,256,32,1,1);
            strXY(QString("集中控制"),0,32);
            strXY("S/N: 0.00",0,48);
            strXY(m_para.strTxRate().toLatin1().data(),128,32);
            strXY(m_para.strRxRate().toLatin1().data(),128,48);
            break;
        default: break;
        }
    }
    else{// if(m_para.m_status==objPara::Status_connected){
        switch(m_status.m_workMode){
        case objPara::Mode_p2p:
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
            strXY(buf,0,48);
            strXY(m_status.strTxRate().toLatin1().data(),128,32);
            strXY(m_status.strRxRate().toLatin1().data(),128,48);
            break;
        case objPara::Mode_central:
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
        strXY("发送功率:",0,6,0x0f,0);// fasong gonglv 发送 功率
        strXY("发送频点:",0,6+16+2,0x0f,0);// fasong pindian 发送频率  频率。频点
        strXY("接收频点:",0,48-6,0x0f,0);// jieshou pindian 发送频率  频率。频点 接收

        //s=locale.toString(0.01*m_para.m_power100) + " dBm";
        sprintf(buf,"%.2f dBm",0.01*m_status.m_power100);
        centerXY(buf,9*8, 6, 256-9*8,16,1,1,0x0f,0);

        centerXY(m_status.strTxFreq(m_para.m_BUCfreq).toLatin1().data(),9*8, 6+16+2, 256-9*8,16,1,1,0x0f,0);
        centerXY(m_status.strRxFreq(m_para.m_LNBfreq).toLatin1().data(),9*8, 48-6, 256-9*8,16,1,1,0x0f,0);
        break;
    case objPara::Mode_central:
        break;
    default: break;
    }
    if(m_para.m_status==objPara::Status_idle){// idle
        strXY("发送功率:",0,6,0x0f,0);// fasong gonglv 发送 功率
        strXY("发送频点:",0,6+16+2,0x0f,0);// fasong pindian 发送频率  频率。频点
        strXY("接收频点:",0,48-6,0x0f,0);// jieshou pindian 发送频率  频率。频点 接收

        //s=locale.toString(0.01*m_para.m_power100) + " dBm";
        sprintf(buf,"%.2f dBm",0.01*m_para.m_power100);
        centerXY(buf,9*8, 6, 256-9*8,16,1,1,0x0f,0);

        centerXY(m_para.strTxFreq(m_para.m_BUCfreq).toLatin1().data(),9*8, 6+16+2, 256-9*8,16,1,1,0x0f,0);
        centerXY(m_para.strRxFreq(m_para.m_LNBfreq).toLatin1().data(),9*8, 48-6, 256-9*8,16,1,1,0x0f,0);
    }
    else{// if(m_para.m_status==objPara::Status_connected){
        strXY("发送功率:",0,6,0x0f,0);// fasong gonglv 发送 功率
        strXY("发送频点:",0,6+16+2,0x0f,0);// fasong pindian 发送频率  频率。频点
        strXY("接收频点:",0,48-6,0x0f,0);// jieshou pindian 发送频率  频率。频点 接收

        //s=locale.toString(0.01*m_para.m_power100) + " dBm";
        sprintf(buf,"%.2f dBm",0.01*m_status.m_power100);
        centerXY(buf,9*8, 6, 256-9*8,16,1,1,0x0f,0);

        centerXY(m_status.strTxFreq(m_para.m_BUCfreq).toLatin1().data(),9*8, 6+16+2, 256-9*8,16,1,1,0x0f,0);
        centerXY(m_status.strRxFreq(m_para.m_LNBfreq).toLatin1().data(),9*8, 48-6, 256-9*8,16,1,1,0x0f,0);
    }

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
void objui::slotShowAbout()
{
    zeroFB(0);

    strXY("ver: 1.17",0,0);
    centerXY("4.23",0,48,256,16,2,1);// data 19.3.10

    const QHostAddress &localaddress = QHostAddress::LocalHost;
    foreach(const QHostAddress &addr, QNetworkInterface::allAddresses()){
        if(addr.protocol()==QAbstractSocket::IPv4Protocol && addr!=localaddress){
            strXY(addr.toString().toLatin1().data(),0,16);
            break;
        }
            //qDebug(" ======================= ipaddress : %s",addr.toString().toLatin1().data());
    }
    strXY(getTimeRunning().toLatin1().data(),0,32);

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();
    QTimer::singleShot(200,this,SLOT(slotKeyEnable()));
    //m_bEnableKeyboard = true;

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
void objui::slotShowMsgZZHJ()
{
    zeroFB(0);

    centerXY(QString("正 在 呼 叫 ......"),0,24,256,16,1,1);

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();
    QTimer::singleShot(1500,this,SLOT(slotStateTransitionNext()));
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
    //qDebug(" func slotShow.menu.00");
    zeroFB(0);

    centerXY(QString("通  信  控  制"),0,0,256,16,1,1,0,0x0f);
    centerXY(QString("通  信  参  数"),0,24,256,16,1,1);
    centerXY(QString("工  作  模  式"),0,48,256,16,1,1);

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();
    QTimer::singleShot(200,this,SLOT(slotKeyEnable()));
    //m_bEnableKeyboard = true;

}
void objui::getPara()
{
    std::string stdstr;

    stdstr=m_cu.getConfiguration(QString("tdm").toStdString());
    qDebug("=====tdm %s",QString::fromStdString(stdstr).toLatin1().data());
    writeTcp(QString::fromStdString(stdstr).toUtf8());
    stdstr=m_cu.getConfiguration(QString("power").toStdString());
    qDebug(" =======   power  %s",QString::fromStdString(stdstr).toLatin1().data());
    writeTcp(QString::fromStdString(stdstr).toUtf8());
}

void objui::slotShowMenu01()
{
    //qDebug("  ***** func slotShow.menu.01");
    getPara();
    zeroFB(0);

    centerXY(QString("通  信  控  制"),0,0,256,16,1,1);
    centerXY(QString("通  信  参  数"),0,24,256,16,1,1,0,0x0f);
    centerXY(QString("工  作  模  式"),0,48,256,16,1,1);

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
    centerXY(QString("通  信  参  数"),0,24,256,16,1,1);
    centerXY(QString("工  作  模  式"),0,48,256,16,1,1,0,0x0f);

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();
    QTimer::singleShot(200,this,SLOT(slotKeyEnable()));
    //m_bEnableKeyboard = true;

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
    sprintf(buf,"%d kbps",m_para.m_RxRate);
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
    sprintf(buf,"%d kbps",m_para.m_TxRate);
    centerXY(buf,16*6,38,256-16*6,16,1,1);
    //sprintf(buf,"%d kbps",m_para.m_RxRate);
    //centerXY(buf,16*6,38,256-16*6,16,1,1);

    strXY("3/4",256-8*3,48,0x0f,0);

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


