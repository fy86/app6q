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

signals:
    void sig0000();
    void sigKeyEnter();
    void sigKeyBack();
    void sigKeyUp();
    void sigKeyDown();
    void sigKeyLeft();
    void sigKeyRight();

    void sigMenu00to01();
    void sigMenu00to02();
    void sigMenu01to02();
    void sigMenu01to00();
    void sigMenu02to00();
    void sigMenu02to01();

public slots:
    void slotTest();
    void slotTestStop();
    void slot00enter();
    void slot00exit();
    void slot01enter();

    void slotKET();
    void slot0000();

    virtual void slotKey(int);

    void keMenu00to01();
    void keMenu00to02();
    void keMenu01to02();
    void keMenu01to00();
    void keMenu02to00();
    void keMenu02to01();

    void slotShowMenu10();
    void slotShowMenu11();
    void slotShowMenu12();
};

#endif // OBJUI_H
