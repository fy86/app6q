#ifndef OBJUI_H
#define OBJUI_H

#include <QObject>
#include <QStateMachine>
#include <QtGui/QKeyEvent>
#include <QtGui/QKeyEventTransition>
#include <QTcpSocket>
#include <QLocale>

#include <iostream>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/input.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

#include "objpara.h"
#include "numeditor.h"
#include "objpage.h"
//#include "kt0000.h"
#include "jsonrpc/cu_rpc_control.h"

class objui : public objPage
{
    Q_OBJECT
public:
    explicit objui(QObject *parent = 0);
    CuRpcControl m_cu;

    QTcpSocket *m_pTcp;
    void initTcp();

    objPara m_para;
    numEditor m_numEditor;

    QStateMachine *m_pMachine;
    QState *m_pStateMenuCtrl;
    QState *m_pStateMenuPara;
    QState *m_pStateMenuWorkMode;

    QState *m_pStateMenu10;
    QState *m_pStateMenu11;

    QState *m_pStateEditorTxFreq;
    QState *m_pStateEditorRxFreq;

    QState *m_pStateParaPage1;// txFreq
    QState *m_pStateParaPage11;// rxFreq
    QState *m_pStateParaPage12;//
    QState *m_pStateParaPage13;
    QState *m_pStateParaPage2;
    QState *m_pStateParaPage21;
    QState *m_pStateParaPage22;
    QState *m_pStateParaPage1p2p;
    QState *m_pStateParaPage2p2p;

    void buildMachine();
    void initMachine();

    void getColorMenu10(int n,int *pc,int *pbg);

    void doMenu10();
    void doMenuPara();// enter paraPage1
    void changeSelectMenu10(int step);
signals:

    void sigStateTransitionUp();
    void sigStateTransitionDown();
    void sigStateTransitionLeft();
    void sigStateTransitionRight();
    void sigStateTransitionEnter();
    void sigStateTransitionBackspace();
    void sigStateTransitionNext();
    void sigStateTransitionBack();

    void sig0000();
    void sigKeyEnter();
    void sigKeyBack();
    void sigKeyUp();
    void sigKeyDown();
    void sigKeyLeft();
    void sigKeyRight();

public slots:
    void slotCUState(QByteArray ba);
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

    void slotStateTransitionUp();
    void slotStateTransitionDown();
    void slotStateTransitionLeft();
    void slotStateTransitionRight();
    void slotStateTransitionEnter();
    void slotStateTransitionBackspace();
    void slotStateTransitionNext();
    void slotStateTransitionBack();

    void slotShowMenu10();
    void slotShowMenu11();
    void slotShowMenu12();

    void slotShowParaPage1();
    void slotShowParaPage11();
    void slotShowParaPage12();
    void slotShowParaPage13();
    void slotShowParaPage2();
    void slotShowParaPage21();
    void slotShowParaPage22();
    void slotShowParaPage1p2p();
    void slotShowParaPage2p2p();

    void slotShowEditTxFreq();
    void slotShowEditRxFreq();
};

#endif // OBJUI_H
