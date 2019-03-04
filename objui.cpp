#include "objui.h"

#include "kt0000.h"

#include "ketmenu00.h"

objui::objui(QObject *parent) :
    objPage(parent)
{
    m_pTcp = new QTcpSocket(this);
    connect(m_pTcp,SIGNAL(readyRead()),this,SLOT(slotReadTCP()));
    connect(m_pTcp,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(slotErrTCP()));

    initTcp();

    connect(m_cu.getRecvNotifyObject(),SIGNAL(sigCUState(QByteArray)),this,SLOT(slotCUState(QByteArray)));
}
void objui::initTcp()
{
    m_pTcp->abort();
    m_pTcp->connectToHost("192.168.0.199",6790);
}
void objui::slotErrTCP()
{
    qDebug(" tcp error");
    m_cu.reset();
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
    m_pMachine = new QStateMachine;

    m_pStateMenuCtrl = new QState(m_pMachine);//
    m_pStateMenuPara = new QState(m_pMachine);
    m_pStateMenuWorkMode = new QState(m_pMachine);

    m_pStateMenu10 = new QState(m_pMachine);// p2p and central
    m_pStateMenu11 = new QState(m_pMachine);//

    m_pStateEditorTxFreq = new QState(m_pMachine);
    m_pStateEditorRxFreq = new QState(m_pMachine);
    m_pStateEditorTxRate = new QState(m_pMachine);
    m_pStateEditorRxRate = new QState(m_pMachine);
    m_pStateEditorPower = new QState(m_pMachine);

    m_pStateParaPage1 = new QState(m_pMachine);
    m_pStateParaPage11 = new QState(m_pMachine);
    m_pStateParaPage12 = new QState(m_pMachine);
    m_pStateParaPage13 = new QState(m_pMachine);
    m_pStateParaPage2 = new QState(m_pMachine);
    m_pStateParaPage21 = new QState(m_pMachine);
    m_pStateParaPage22 = new QState(m_pMachine);
    m_pStateParaPage1p2p = new QState(m_pMachine);
    m_pStateParaPage2p2p = new QState(m_pMachine);

    m_pMachine->setInitialState(m_pStateMenuCtrl);

    connect(m_pStateMenuCtrl,SIGNAL(entered()),this,SLOT(slotShowMenu00()));
    connect(m_pStateMenuPara,SIGNAL(entered()),this,SLOT(slotShowMenu01()));
    connect(m_pStateMenuWorkMode,SIGNAL(entered()),this,SLOT(slotShowMenu02()));

    connect(m_pStateMenu10,SIGNAL(entered()),this,SLOT(slotShowMenu10()));

    connect(m_pStateParaPage1,SIGNAL(entered()),this,SLOT(slotShowParaPage1()));
    connect(m_pStateParaPage11,SIGNAL(entered()),this,SLOT(slotShowParaPage11()));
    connect(m_pStateParaPage12,SIGNAL(entered()),this,SLOT(slotShowParaPage12()));
    connect(m_pStateParaPage13,SIGNAL(entered()),this,SLOT(slotShowParaPage13()));
    connect(m_pStateParaPage2,SIGNAL(entered()),this,SLOT(slotShowParaPage2()));
    connect(m_pStateParaPage21,SIGNAL(entered()),this,SLOT(slotShowParaPage21()));
    connect(m_pStateParaPage22,SIGNAL(entered()),this,SLOT(slotShowParaPage22()));

    connect(m_pStateEditorTxFreq,SIGNAL(entered()),this,SLOT(slotShowEditTxFreq()));
    connect(m_pStateEditorRxFreq,SIGNAL(entered()),this,SLOT(slotShowEditRxFreq()));
    connect(m_pStateEditorTxRate,SIGNAL(entered()),this,SLOT(slotShowEditTxRate()));
    connect(m_pStateEditorRxRate,SIGNAL(entered()),this,SLOT(slotShowEditRxRate()));
    connect(m_pStateEditorPower,SIGNAL(entered()),this,SLOT(slotShowEditPower()));

    connect(m_pStateParaPage1p2p,SIGNAL(entered()),this,SLOT(slotShowParaPage1p2p()));
    connect(m_pStateParaPage2p2p,SIGNAL(entered()),this,SLOT(slotShowParaPage2p2p()));

    ketMenu00 *pKetMenu00 = new ketMenu00(this);
    m_pStateMenuCtrl->addTransition(pKetMenu00);
    m_pStateMenuCtrl->addTransition(this,SIGNAL(sigStateTransitionDown()),m_pStateMenuPara);
    m_pStateMenuCtrl->addTransition(this,SIGNAL(sigStateTransitionUp()),m_pStateMenuWorkMode);
    m_pStateMenuCtrl->addTransition(this,SIGNAL(sigStateTransitionEnter()),m_pStateMenu10);
    ketMenuPara *pKetMenuPara = new ketMenuPara(this);
    m_pStateMenuPara->addTransition(pKetMenuPara);
    m_pStateMenuPara->addTransition(this,SIGNAL(sigStateTransitionDown()),m_pStateMenuWorkMode);
    m_pStateMenuPara->addTransition(this,SIGNAL(sigStateTransitionUp()),m_pStateMenuCtrl);
    m_pStateMenuPara->addTransition(this,SIGNAL(sigStateTransitionNext()),m_pStateParaPage1);
    //m_pStateMenuPara->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateNumEditor);

    ketParaPage1 *pKetParaPage1 = new ketParaPage1(this);
    m_pStateParaPage1->addTransition(pKetParaPage1);
    m_pStateParaPage1->addTransition(this,SIGNAL(sigStateTransitionDown()),m_pStateParaPage11);
    m_pStateParaPage1->addTransition(this,SIGNAL(sigStateTransitionUp()),m_pStateParaPage2);
    m_pStateParaPage1->addTransition(this,SIGNAL(sigStateTransitionLeft()),m_pStateParaPage2);
    m_pStateParaPage1->addTransition(this,SIGNAL(sigStateTransitionRight()),m_pStateParaPage2);
    m_pStateParaPage1->addTransition(this,SIGNAL(sigStateTransitionNext()),m_pStateEditorTxFreq);
    m_pStateParaPage1->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateMenuPara);

    ketParaPage1 *pKetParaPage11 = new ketParaPage1(this);
    m_pStateParaPage11->addTransition(pKetParaPage11);
    m_pStateParaPage11->addTransition(this,SIGNAL(sigStateTransitionDown()),m_pStateParaPage12);
    m_pStateParaPage11->addTransition(this,SIGNAL(sigStateTransitionUp()),m_pStateParaPage1);
    m_pStateParaPage11->addTransition(this,SIGNAL(sigStateTransitionLeft()),m_pStateParaPage2);
    m_pStateParaPage11->addTransition(this,SIGNAL(sigStateTransitionRight()),m_pStateParaPage2);
    m_pStateParaPage11->addTransition(this,SIGNAL(sigStateTransitionNext()),m_pStateEditorRxFreq);
    m_pStateParaPage11->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateMenuPara);

    ketParaPage1 *pKetParaPage12 = new ketParaPage1(this);
    m_pStateParaPage12->addTransition(pKetParaPage12);
    m_pStateParaPage12->addTransition(this,SIGNAL(sigStateTransitionDown()),m_pStateParaPage13);
    m_pStateParaPage12->addTransition(this,SIGNAL(sigStateTransitionUp()),m_pStateParaPage11);
    m_pStateParaPage12->addTransition(this,SIGNAL(sigStateTransitionLeft()),m_pStateParaPage2);
    m_pStateParaPage12->addTransition(this,SIGNAL(sigStateTransitionRight()),m_pStateParaPage2);
    m_pStateParaPage12->addTransition(this,SIGNAL(sigStateTransitionNext()),m_pStateEditorTxRate);
    m_pStateParaPage12->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateMenuPara);

    ketParaPage1 *pKetParaPage13 = new ketParaPage1(this);
    m_pStateParaPage13->addTransition(pKetParaPage13);
    m_pStateParaPage13->addTransition(this,SIGNAL(sigStateTransitionDown()),m_pStateParaPage2);
    m_pStateParaPage13->addTransition(this,SIGNAL(sigStateTransitionUp()),m_pStateParaPage12);
    m_pStateParaPage13->addTransition(this,SIGNAL(sigStateTransitionLeft()),m_pStateParaPage2);
    m_pStateParaPage13->addTransition(this,SIGNAL(sigStateTransitionRight()),m_pStateParaPage2);
    m_pStateParaPage13->addTransition(this,SIGNAL(sigStateTransitionNext()),m_pStateEditorRxRate);
    m_pStateParaPage13->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateMenuPara);

    ketParaPage1 *pKetParaPage2 = new ketParaPage1(this);
    m_pStateParaPage2->addTransition(pKetParaPage2);
    m_pStateParaPage2->addTransition(this,SIGNAL(sigStateTransitionDown()),m_pStateParaPage21);
    m_pStateParaPage2->addTransition(this,SIGNAL(sigStateTransitionUp()),m_pStateParaPage1);
    m_pStateParaPage2->addTransition(this,SIGNAL(sigStateTransitionLeft()),m_pStateParaPage1);
    m_pStateParaPage2->addTransition(this,SIGNAL(sigStateTransitionRight()),m_pStateParaPage1);
    m_pStateParaPage2->addTransition(this,SIGNAL(sigStateTransitionNext()),m_pStateEditorPower);
    m_pStateParaPage2->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateMenuPara);

    ketParaPage1 *pKetParaPage21 = new ketParaPage1(this);
    m_pStateParaPage21->addTransition(pKetParaPage21);
    m_pStateParaPage21->addTransition(this,SIGNAL(sigStateTransitionDown()),m_pStateParaPage22);
    m_pStateParaPage21->addTransition(this,SIGNAL(sigStateTransitionUp()),m_pStateParaPage2);
    m_pStateParaPage21->addTransition(this,SIGNAL(sigStateTransitionLeft()),m_pStateParaPage1);
    m_pStateParaPage21->addTransition(this,SIGNAL(sigStateTransitionRight()),m_pStateParaPage1);
    m_pStateParaPage21->addTransition(this,SIGNAL(sigStateTransitionNext()),m_pStateEditorTxFreq);
    m_pStateParaPage21->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateMenuPara);

    ketParaPage1 *pKetParaPage22 = new ketParaPage1(this);
    m_pStateParaPage22->addTransition(pKetParaPage22);
    m_pStateParaPage22->addTransition(this,SIGNAL(sigStateTransitionDown()),m_pStateParaPage1);
    m_pStateParaPage22->addTransition(this,SIGNAL(sigStateTransitionUp()),m_pStateParaPage21);
    m_pStateParaPage22->addTransition(this,SIGNAL(sigStateTransitionLeft()),m_pStateParaPage1);
    m_pStateParaPage22->addTransition(this,SIGNAL(sigStateTransitionRight()),m_pStateParaPage1);
    m_pStateParaPage22->addTransition(this,SIGNAL(sigStateTransitionNext()),m_pStateEditorTxFreq);
    m_pStateParaPage22->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateMenuPara);

    ketTxFreqEditor *pKetTxFreqEditor = new ketTxFreqEditor(this);
    m_pStateEditorTxFreq->addTransition(pKetTxFreqEditor);
    m_pStateEditorTxFreq->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateParaPage1);
    ketRxFreqEditor *pKetRxFreqEditor = new ketRxFreqEditor(this);
    m_pStateEditorRxFreq->addTransition(pKetRxFreqEditor);
    m_pStateEditorRxFreq->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateParaPage11);

    ketTxRateEditor *pKetTxRateEditor = new ketTxRateEditor(this);
    m_pStateEditorTxRate->addTransition(pKetTxRateEditor);
    m_pStateEditorTxRate->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateParaPage12);
    ketRxRateEditor *pKetRxRateEditor = new ketRxRateEditor(this);
    m_pStateEditorRxRate->addTransition(pKetRxRateEditor);
    m_pStateEditorRxRate->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateParaPage13);

    ketPowerEditor *pKetPowerEditor = new ketPowerEditor(this);
    m_pStateEditorPower->addTransition(pKetPowerEditor);
    m_pStateEditorPower->addTransition(this,SIGNAL(sigStateTransitionBack()),m_pStateParaPage2);






    ketMenu02 *pKetMenu02 = new ketMenu02(this);
    m_pStateMenuWorkMode->addTransition(pKetMenu02);
    m_pStateMenuWorkMode->addTransition(this,SIGNAL(sigStateTransitionDown()),m_pStateMenuCtrl);
    m_pStateMenuWorkMode->addTransition(this,SIGNAL(sigStateTransitionUp()),m_pStateMenuPara);
    ketMenu10 *pKetMenu10 = new ketMenu10(this);
    m_pStateMenu10->addTransition(pKetMenu10);
    m_pStateMenu10->addTransition(this,SIGNAL(sigStateTransitionBackspace()),m_pStateMenuCtrl);




    m_pMachine->start();
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

void objui::slotTest()
{
    qDebug(" machine start");
}

void objui::slotTestStop()
{
    qDebug(" machine stop");
}
void objui::slotKey(int key)
{
    QKeyEvent *ev;// = new QKeyEvent(QEvent::KeyPress,Qt::Key_4,Qt::NoModifier);
    switch (key) {
    case KEY_DOWN:
        //qDebug(" slotKey(............ func.objui");
        ev = new QKeyEvent(QEvent::KeyPress,Qt::Key_Down,Qt::NoModifier);
        m_pMachine->postEvent(ev);
        break;
    case KEY_UP:
        ev = new QKeyEvent(QEvent::KeyPress,Qt::Key_Up,Qt::NoModifier);
        m_pMachine->postEvent(ev);
        break;
    case KEY_LEFT:
        ev = new QKeyEvent(QEvent::KeyPress,Qt::Key_Left,Qt::NoModifier);
        m_pMachine->postEvent(ev);
        break;
    case KEY_RIGHT:
        ev = new QKeyEvent(QEvent::KeyPress,Qt::Key_Right,Qt::NoModifier);
        m_pMachine->postEvent(ev);
        break;
    case KEY_ENTER:
        ev = new QKeyEvent(QEvent::KeyPress,Qt::Key_Enter,Qt::NoModifier);
        m_pMachine->postEvent(ev);
        break;
    case KEY_BACKSPACE:
        ev = new QKeyEvent(QEvent::KeyPress,Qt::Key_Backspace,Qt::NoModifier);
        m_pMachine->postEvent(ev);
        break;
    default:
        break;
    }

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
void objui::getColorMenu10(int n, int *pc, int *pbg)
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

void objui::slotShowMenu10()
{
    qint64 freq;
    const QLocale & locale = QLocale::c();
    freq=14000000000;
    QString s=locale.toString(freq);

    int c=0x0f,bg=0;
    zeroFB(0);

    switch(m_workMode){
    case work_mode_central:
        getColorMenu10(0,&c,&bg);
        centerXY(QString("呼    叫"),0,0,256,16,1,1,c,bg);

        getColorMenu10(1,&c,&bg);
        centerXY(QString("挂    断"),0,16,256,16,1,1,c,bg);

        getColorMenu10(2,&c,&bg);
        centerXY(QString("入    网"),0,32,256,16,1,1,c,bg);

        getColorMenu10(3,&c,&bg);
        centerXY(QString("退    网"),0,48,256,16,1,1,c,bg);

        ////////////////////  test qint64
        strXY(s.replace(',',' ').toLatin1().data(),0,32);
        underLine(0,32,4);
        strXY(s.replace(',',' ').toLatin1().data(),0,48);
        strXY("14 0",128,32);
        strXY("0",128+32,32,0,0x0f);
        strXY("0 000",128+32+8,32);
        underLine(0,32,17);


        break;
    case work_mode_p2p:
        if(m_bStatConnect){
            centerXY(QString("呼    叫"),0,8,256,16,1,1,1);
            centerXY(QString("挂    断"),0,32,256,16,1,1,0,0x0f);
        }
        else{
            centerXY(QString("呼    叫"),0,8,256,16,1,1,0,0x0f);
            centerXY(QString("挂    断"),0,32,256,16,1,1,1);
        }
        break;
    default:
        break;
    }

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();

}
void objui::changeSelectMenu10(int step)
{
    if(m_nSelectMenu10<0 || m_nSelectMenu10>3)m_nSelectMenu10=0;
    for(;;){
        m_nSelectMenu10 = (m_nSelectMenu10+step) & 0x3;
        if(m_bEnableMenu10[m_nSelectMenu10]) break;
    }
    slotShowMenu10();
}

void objui::slotCUState(QByteArray ba)
{
    //qDebug("%s",ba.data());

#if 1
    struct CUState *p,cu;
    p = (struct CUState*)ba.data();
    //memcpy(&cu,ba.data(),ba.size());
    //p = &cu;
    qDebug() << QString::fromStdString(p->mode);
    qDebug("--------------- %s",QString::fromStdString(p->isdn).toLatin1().data());
    qDebug("-------------- %s",QString::fromStdString(p->name).toLatin1().data());
    qDebug("----------- %s",QString::fromStdString(p->state).toLatin1().data());
#endif

}

void objui::doMenu10()
{
    std::string stdstr;

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
            m_pTcp->write(QString::fromStdString(stdstr).toUtf8());
            m_bStatLogin=true;break;
        }
    case 3:
        if(m_bStatLogin){
            qDebug(" do logout");
            stdstr=m_cu.logoutNet();
            m_pTcp->write(QString::fromStdString(stdstr).toUtf8());
            m_bStatLogin=false;
            m_bStatConnect=false;
        }
        break;
    default:
        break;
    }
    setSelectMenu10();
    slotShowMenu10();
    }
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

}


void objui::slotShowParaPage1()
{
    //qDebug("     show menu.page.1");

    const QLocale & locale = QLocale::c();
    QString s;//=locale.toString(m_numEditor.m_i64);

    zeroFB(0);

    m_numEditor.setNum64(m_para.m_TxFreq,m_para.m_maxTxFreq,m_para.m_minTxFreq,-1,0,-1);

    strXY("发送频点",0,0,0,0x0f);// fasong pindian 发送频率  频率。频点
    strXY("接收频点",0,16,0x0f,0);// jieshou pindian 发送频率  频率。频点 接收
    strXY("发送速率",0,32,0x0f,0);// fasong sulv 发送频率  频率。频点  速率
    strXY("接收速率",0,48,0x0f,0);// jieshou sulv 发送频率  频率。频点

    s=locale.toString(m_para.m_TxFreq).replace(',',' ') + " Hz";
    centerXY(s.toLatin1().data(),16*4,0,256-16*4,16,1,1,0x0f,0);
    s=locale.toString(m_para.m_RxFreq).replace(',',' ') + " Hz";
    centerXY(s.toLatin1().data(),16*4,16,256-16*4,16,1,1,0x0f,0);
    s=locale.toString(m_para.m_TxRate).replace(',',' ') + " kHz";
    centerXY(s.toLatin1().data(),16*4,32,256-16*4,16,1,1,0x0f,0);
    s=locale.toString(m_para.m_RxRate).replace(',',' ') + " kHz";
    centerXY(s.toLatin1().data(),16*4,48,256-16*4,16,1,1,0x0f,0);

    strXY("1/2",256-8*3,48,0x0f,0);

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();

}
void objui::slotShowParaPage11()
{
    //qDebug("     show menu.page.11");

    zeroFB(0);

    m_numEditor.setNum64(m_para.m_RxFreq,m_para.m_maxRxFreq,m_para.m_minRxFreq,-1,0,-1);

    strXY("发送频点",0,0,0x0f,0);// fasong pindian 发送频率  频率。频点
    strXY("接收频点",0,16,0,0x0f);// jieshou pindian 发送频率  频率。频点 接收
    strXY("发送速率",0,32,0x0f,0);// fasong sulv 发送频率  频率。频点  速率
    strXY("接收速率",0,48,0x0f,0);// jieshou sulv 发送频率  频率。频点

    const QLocale & locale = QLocale::c();
    QString s;//=locale.toString(m_numEditor.m_i64);
    s=locale.toString(m_para.m_TxFreq).replace(',',' ') + " Hz";
    centerXY(s.toLatin1().data(),16*4,0,256-16*4,16,1,1,0x0f,0);
    s=locale.toString(m_para.m_RxFreq).replace(',',' ') + " Hz";
    centerXY(s.toLatin1().data(),16*4,16,256-16*4,16,1,1,0x0f,0);
    s=locale.toString(m_para.m_TxRate).replace(',',' ') + " kHz";
    centerXY(s.toLatin1().data(),16*4,32,256-16*4,16,1,1,0x0f,0);
    s=locale.toString(m_para.m_RxRate).replace(',',' ') + " kHz";
    centerXY(s.toLatin1().data(),16*4,48,256-16*4,16,1,1,0x0f,0);

    strXY("1/2",256-8*3,48,0x0f,0);

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();

}
void objui::slotShowParaPage12()
{
    //qDebug("     show menu.page.12");
    zeroFB(0);

    m_numEditor.setNum64(m_para.m_TxRate,m_para.m_maxTxRate,m_para.m_minTxRate,-1,0,0);

    strXY("发送频点",0,0,0x0f,0);// fasong pindian 发送频率  频率。频点
    strXY("接收频点",0,16,0x0f,0);// jieshou pindian 发送频率  频率。频点 接收
    strXY("发送速率",0,32,0,0x0f);// fasong sulv 发送频率  频率。频点  速率
    strXY("接收速率",0,48,0x0f,0);// jieshou sulv 发送频率  频率。频点

    const QLocale & locale = QLocale::c();
    QString s;//=locale.toString(m_numEditor.m_i64);
    s=locale.toString(m_para.m_TxFreq).replace(',',' ') + " Hz";
    centerXY(s.toLatin1().data(),16*4,0,256-16*4,16,1,1,0x0f,0);
    s=locale.toString(m_para.m_RxFreq).replace(',',' ') + " Hz";
    centerXY(s.toLatin1().data(),16*4,16,256-16*4,16,1,1,0x0f,0);

    s=locale.toString(m_para.m_TxRate).replace(',',' ') + " kHz";
    centerXY(s.toLatin1().data(),16*4,32,256-16*4,16,1,1,0x0f,0);
    s=locale.toString(m_para.m_RxRate).replace(',',' ') + " kHz";
    centerXY(s.toLatin1().data(),16*4,48,256-16*4,16,1,1,0x0f,0);

    strXY("1/2",256-8*3,48,0x0f,0);

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();

}
void objui::slotShowParaPage13()
{
    //qDebug("     show menu.page.13");
    zeroFB(0);

    m_numEditor.setNum64(m_para.m_RxRate,m_para.m_maxRxRate,m_para.m_minRxRate,-1,0,0);

    strXY("发送频点",0,0,0x0f,0);// fasong pindian 发送频率  频率。频点
    strXY("接收频点",0,16,0x0f,0);// jieshou pindian 发送频率  频率。频点 接收
    strXY("发送速率",0,32,0x0f,0);// fasong sulv 发送频率  频率。频点  速率
    strXY("接收速率",0,48,0,0x0f);// jieshou sulv 发送频率  频率。频点

    const QLocale & locale = QLocale::c();
    QString s;//=locale.toString(m_numEditor.m_i64);
    s=locale.toString(m_para.m_TxFreq).replace(',',' ') + " Hz";
    centerXY(s.toLatin1().data(),16*4,0,256-16*4,16,1,1,0x0f,0);
    s=locale.toString(m_para.m_RxFreq).replace(',',' ') + " Hz";
    centerXY(s.toLatin1().data(),16*4,16,256-16*4,16,1,1,0x0f,0);
    s=locale.toString(m_para.m_TxRate).replace(',',' ') + " kHz";
    centerXY(s.toLatin1().data(),16*4,32,256-16*4,16,1,1,0x0f,0);
    s=locale.toString(m_para.m_RxRate).replace(',',' ') + " kHz";
    centerXY(s.toLatin1().data(),16*4,48,256-16*4,16,1,1,0x0f,0);

    strXY("1/2",256-8*3,48,0x0f,0);

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();

}
void objui::slotShowParaPage2()
{
    char buf[20];
    zeroFB(0);

    m_numEditor.setNum64(m_para.m_power100,m_para.m_maxPower,m_para.m_minPower,-1,0,0);

    strXY("发送功率",0,0,0,0x0f);// fasong gonglv 发送 功率
    strXY("业务类型",0,16,0x0f,0);// yewu leixing 业务类型
    strXY("编码方式",0,32,0x0f,0);// bianma fangshi 编码方式

    const QLocale & locale = QLocale::c();
    QString s;//=locale.toString(m_numEditor.m_i64);
    s=locale.toString(0.01*m_para.m_power100) + " dBm";
    sprintf(buf,"%.2f dBm",0.01*m_para.m_power100);
    centerXY(buf,4*16,0,256-16*4,16,1,1,0x0f,0);
    centerXY("网桥",4*16,16,256-16*4,16,1,1,0x0f,0);// wangqiao  网桥
    centerXY("QPSK1/2",4*16,32,256-16*4,16,1,1,0x0f,0);

    strXY("2/2",256-8*3,48,0x0f,0);

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();

}
void objui::slotShowParaPage21()
{
    zeroFB(0);

    strXY("发送功率",0,0,0x0f,0);// fasong gonglv 发送 功率
    strXY("业务类型",0,16,0,0x0f);// yewu leixing 业务类型
    strXY("编码方式",0,32,0x0f,0);// bianma fangshi 编码方式

    centerXY("-37.25 dbm",4*16,0,256-16*4,16,1,1,0x0f,0);
    centerXY("网桥",4*16,16,256-16*4,16,1,1,0x0f,0);// wangqiao  网桥
    centerXY("QPSK1/2",4*16,32,256-16*4,16,1,1,0x0f,0);

    strXY("2/2",256-8*3,48,0x0f,0);

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();

}
void objui::slotShowParaPage22()
{
    zeroFB(0);

    strXY("发送功率",0,0,0x0f,0);// fasong gonglv 发送 功率
    strXY("业务类型",0,16,0x0f,0);// yewu leixing 业务类型
    strXY("编码方式",0,32,0,0x0f);// bianma fangshi 编码方式

    centerXY("-37.25 dbm",4*16,0,256-16*4,16,1,1,0x0f,0);
    centerXY("网桥",4*16,16,256-16*4,16,1,1,0x0f,0);// wangqiao  网桥
    centerXY("QPSK1/2",4*16,32,256-16*4,16,1,1,0x0f,0);

    strXY("2/2",256-8*3,48,0x0f,0);

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();

}

void objui::slotShowEditTxFreq()
{
    zeroFB(0);

    const QLocale & locale = QLocale::c();
    QString s=locale.toString(m_numEditor.m_i64);

    centerXY("发 送 频 点",0,10,256,16,1,1,0x0f,0);
    centerXY(s.replace(',',' ').toLatin1().data(),0,64-12-16,(7+14)<<3,16,2,1,0x0f,0);
    strXY("  Hz",(14+7)<<3,64-12-16,0x0f,0);
    underLine(0,64-12-16,14+7-1-m_numEditor.getCursor(),0x0f);

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();

}
void objui::slotShowEditRxFreq()
{
    zeroFB(0);

    const QLocale & locale = QLocale::c();
    QString s=locale.toString(m_numEditor.m_i64);

    centerXY("接 收 频 点",0,10,256,16,1,1,0x0f,0);
    centerXY(s.replace(',',' ').toLatin1().data(),0,64-12-16,(7+14)<<3,16,2,1,0x0f,0);
    strXY("  Hz",(14+7)<<3,64-12-16,0x0f,0);
    underLine(0,64-12-16,14+7-1-m_numEditor.getCursor(),0x0f);

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();

}

void objui::slotShowEditTxRate()
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

}
void objui::slotShowEditRxRate()
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

}

void objui::slotShowParaPage1p2p()
{

}
void objui::slotShowParaPage2p2p()
{

}


