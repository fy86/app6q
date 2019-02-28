#include "objui.h"

#include "kt0000.h"

#include "ketmenu00.h"

objui::objui(QObject *parent) :
    objPage(parent)
{
}
void objui::initMachine()
{
    m_pMachine = new QStateMachine;

    m_pStateMenu00 = new QState(m_pMachine);
    m_pStateMenu01 = new QState(m_pMachine);
    m_pStateMenu02 = new QState(m_pMachine);

    m_pStateMenu10 = new QState(m_pMachine);// p2p and central
    m_pStateMenu11 = new QState(m_pMachine);//

    m_pMachine->setInitialState(m_pStateMenu00);

    connect(m_pStateMenu00,SIGNAL(entered()),this,SLOT(slotShowMenu00()));
    connect(m_pStateMenu01,SIGNAL(entered()),this,SLOT(slotShowMenu01()));
    connect(m_pStateMenu02,SIGNAL(entered()),this,SLOT(slotShowMenu02()));

    connect(m_pStateMenu10,SIGNAL(entered()),this,SLOT(slotShowMenu10()));

    ketMenu00 *pKetMenu00 = new ketMenu00(this);
    m_pStateMenu00->addTransition(pKetMenu00);
    m_pStateMenu00->addTransition(this,SIGNAL(sigStateTransitionDown()),m_pStateMenu01);
    m_pStateMenu00->addTransition(this,SIGNAL(sigStateTransitionUp()),m_pStateMenu02);
    m_pStateMenu00->addTransition(this,SIGNAL(sigStateTransitionEnter()),m_pStateMenu10);
    ketMenu01 *pKetMenu01 = new ketMenu01(this);
    m_pStateMenu01->addTransition(pKetMenu01);
    m_pStateMenu01->addTransition(this,SIGNAL(sigStateTransitionDown()),m_pStateMenu02);
    m_pStateMenu01->addTransition(this,SIGNAL(sigStateTransitionUp()),m_pStateMenu00);
    ketMenu02 *pKetMenu02 = new ketMenu02(this);
    m_pStateMenu02->addTransition(pKetMenu02);
    m_pStateMenu02->addTransition(this,SIGNAL(sigStateTransitionDown()),m_pStateMenu00);
    m_pStateMenu02->addTransition(this,SIGNAL(sigStateTransitionUp()),m_pStateMenu01);
    ketMenu10 *pKetMenu10 = new ketMenu10(this);
    m_pStateMenu10->addTransition(pKetMenu10);
    m_pStateMenu10->addTransition(this,SIGNAL(sigStateTransitionBackspace()),m_pStateMenu00);

    m_pMachine->start();
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

void objui::doMenu10()
{
    if(work_mode_central == m_workMode){
    switch(m_nSelectMenu10){
    case 0:
        m_bStatConnect=true;
        break;
    case 1:
        m_bStatConnect = false;
        break;
    case 2:
        m_bStatLogin=true;break;
    case 3:
        m_bStatLogin=false;break;
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


