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

    void buildMachine();

signals:
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

};

#endif // OBJUI_H
