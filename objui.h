#ifndef OBJUI_H
#define OBJUI_H

#include <QObject>
#include <QStateMachine>
#include <QtGui/QKeyEvent>
#include <QtGui/QKeyEventTransition>


#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/input.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>


#include "objpage.h"
//#include "kt0000.h"

class objui : public objPage
{
    Q_OBJECT
public:
    explicit objui(QObject *parent = 0);

    QStateMachine *m_pMachine;
    QState *m_pStateMenu00;
    QState *m_pStateMenu01;
    QState *m_pStateMenu02;

    QState *m_pStateMenu10;
    QState *m_pStateMenu11;

    void buildMachine();
    void initMachine();

    void getColorMenu10(int n,int *pc,int *pbg);

    void doMenu10();
    void changeSelectMenu10(int step);
signals:

    void sigStateTransitionUp();
    void sigStateTransitionDown();
    void sigStateTransitionLeft();
    void sigStateTransitionRight();
    void sigStateTransitionEnter();
    void sigStateTransitionBackspace();
    //void sigStateTransitionUp();
    //void sigStateTransitionUp();

    void sig0000();
    void sigKeyEnter();
    void sigKeyBack();
    void sigKeyUp();
    void sigKeyDown();
    void sigKeyLeft();
    void sigKeyRight();

public slots:
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

    void slotShowMenu10();
    void slotShowMenu11();
    void slotShowMenu12();

};

#endif // OBJUI_H
