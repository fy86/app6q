#include "objui.h"

#include "kt0000.h"

objui::objui(QObject *parent) :
    objPage(parent)
{
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

    //QKeyEventTransition *ket00to01 = new QKeyEventTransition(0, QEvent::KeyPress, Qt::Key_Down);
    //ket00to01->setTargetState(p0100);
    //p0000->addTransition(ket00to01);

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
    //qDebug(" ui slotkey");
    QKeyEvent *ev;// = new QKeyEvent(QEvent::KeyPress,Qt::Key_4,Qt::NoModifier);
    switch (key) {
    case KEY_DOWN:
        ev = new QKeyEvent(QEvent::KeyPress,Qt::Key_Down,Qt::NoModifier);
        break;
    default:
        ev = new QKeyEvent(QEvent::KeyPress,Qt::Key_Up,Qt::NoModifier);
        break;
    }

    m_pMachine->postEvent(ev);

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
