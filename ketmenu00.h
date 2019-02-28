#ifndef KETMENU00_H
#define KETMENU00_H

#include <QObject>
#include <QEvent>
#include <QEventTransition>
#include <QtGui/QKeyEvent>

#include "objui.h"

class ketMenu00 : public QEventTransition
{
    Q_OBJECT
public:
    //explicit ketMenu00(QObject *parent = 0);
    ketMenu00(objui *obj):QEventTransition(obj,QEvent::KeyPress){
        m_pui = obj;

    }

protected:
    bool eventTest(QEvent *event) {
        //qDebug("  ketMenu00 event all %d",event->type());
        if (event->type() == QEvent::StateMachineWrapped &&
            static_cast<QStateMachine::WrappedEvent *>(event)->event()->type() == QEvent::KeyPress) {
            QEvent *wrappedEvent = static_cast<QStateMachine::WrappedEvent *>(event)->event();

            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(wrappedEvent);
            int key = keyEvent->key();

            //qDebug(" ketMenu00 keypress");
            return key == Qt::Key_Down
                    || key==Qt::Key_Right
                    || key == Qt::Key_Up
                    || key == Qt::Key_Left;
        }
        else if (event->type() == QEvent::KeyPress) {

            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            int key = keyEvent->key();

            //qDebug(" ketMenu00 keypress  2222  key:%d",key);
            //return true;
            return key == Qt::Key_Down
                    || key==Qt::Key_Right
                    || key == Qt::Key_Up
                    || key == Qt::Key_Left;
        }
        return false;
    }

    void onTransition(QEvent *event) {
        //QKeyEvent *keyEvent = static_cast<QKeyEvent *>(
            //static_cast<QStateMachine::WrappedEvent *>(event)->event());
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

        int key = keyEvent->key();
        switch (key) {
            case Qt::Key_Left:
            case Qt::Key_Up:
                m_pui->slotStateTransitionUp();
                break;
            case Qt::Key_Right:
            case Qt::Key_Down:
                //qDebug("   ket00 down");
                m_pui->slotStateTransitionDown();
                break;
            default:
                break;
        }
    }


signals:

public slots:

private:
    objui *m_pui;

};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief The ketMenu01 class
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class ketMenu01 : public QEventTransition
{
    Q_OBJECT
public:
    //explicit ketMenu00(QObject *parent = 0);
    ketMenu01(objui *obj):QEventTransition(obj,QEvent::KeyPress){
        m_pui = obj;

    }

protected:
    bool eventTest(QEvent *event) {
        //qDebug("  ketMenu00 event all %d",event->type());
        if (event->type() == QEvent::StateMachineWrapped &&
            static_cast<QStateMachine::WrappedEvent *>(event)->event()->type() == QEvent::KeyPress) {
            QEvent *wrappedEvent = static_cast<QStateMachine::WrappedEvent *>(event)->event();

            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(wrappedEvent);
            int key = keyEvent->key();

            //qDebug(" ketMenu00 keypress");
            return key == Qt::Key_Down
                    || key==Qt::Key_Right
                    || key == Qt::Key_Up
                    || key == Qt::Key_Left;
        }
        else if (event->type() == QEvent::KeyPress) {

            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            int key = keyEvent->key();

            //qDebug(" ketMenu00 keypress  2222  key:%d",key);
            //return true;
            return key == Qt::Key_Down
                    || key==Qt::Key_Right
                    || key == Qt::Key_Up
                    || key == Qt::Key_Left;
        }
        return false;
    }

    void onTransition(QEvent *event) {
        //QKeyEvent *keyEvent = static_cast<QKeyEvent *>(
            //static_cast<QStateMachine::WrappedEvent *>(event)->event());
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

        int key = keyEvent->key();
        switch (key) {
            case Qt::Key_Left:
            case Qt::Key_Up:
                m_pui->slotStateTransitionUp();
                break;
            case Qt::Key_Right:
            case Qt::Key_Down:
                //qDebug(" ket01 down");
                m_pui->slotStateTransitionDown();
                break;
            default:
                break;
        }
    }


signals:

public slots:

private:
    objui *m_pui;

};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief The ketMenu01 class
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class ketMenu02 : public QEventTransition
{
    Q_OBJECT
public:
    //explicit ketMenu00(QObject *parent = 0);
    ketMenu02(objui *obj):QEventTransition(obj,QEvent::KeyPress){
        m_pui = obj;

    }

protected:
    bool eventTest(QEvent *event) {
        //qDebug("  ketMenu00 event all %d",event->type());
        if (event->type() == QEvent::StateMachineWrapped &&
            static_cast<QStateMachine::WrappedEvent *>(event)->event()->type() == QEvent::KeyPress) {
            QEvent *wrappedEvent = static_cast<QStateMachine::WrappedEvent *>(event)->event();

            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(wrappedEvent);
            int key = keyEvent->key();

            //qDebug(" ketMenu00 keypress");
            return key == Qt::Key_Down
                    || key==Qt::Key_Right
                    || key == Qt::Key_Up
                    || key == Qt::Key_Left;
        }
        else if (event->type() == QEvent::KeyPress) {

            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            int key = keyEvent->key();

            //qDebug(" ketMenu00 keypress  2222  key:%d",key);
            //return true;
            return key == Qt::Key_Down
                    || key==Qt::Key_Right
                    || key == Qt::Key_Up
                    || key == Qt::Key_Left;
        }
        return false;
    }

    void onTransition(QEvent *event) {
        //QKeyEvent *keyEvent = static_cast<QKeyEvent *>(
            //static_cast<QStateMachine::WrappedEvent *>(event)->event());
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

        int key = keyEvent->key();
        switch (key) {
            case Qt::Key_Left:
            case Qt::Key_Up:
                m_pui->slotStateTransitionUp();
                break;
            case Qt::Key_Right:
            case Qt::Key_Down:
                //qDebug(" ket02 down");
                m_pui->slotStateTransitionDown();
                break;
            default:
                break;
        }
    }


signals:

public slots:

private:
    objui *m_pui;

};


#endif // KETMENU00_H
