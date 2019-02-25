#ifndef KT0000_H
#define KT0000_H

#include <QObject>
#include <QEvent>
#include <QEventTransition>
#include <QtGui/QKeyEvent>

#include "objui.h"

class kt0000 : public QEventTransition
{
    Q_OBJECT
public:
    //explicit kt0000(QObject *parent = 0);

    kt0000(objui *obj):QEventTransition(obj,QEvent::KeyPress){
        m_pui = obj;

    }

protected:
    bool eventTest(QEvent *event) {
        //qDebug("  kt0000 event all %d",event->type());
        if (event->type() == QEvent::StateMachineWrapped &&
            static_cast<QStateMachine::WrappedEvent *>(event)->event()->type() == QEvent::KeyPress) {
            QEvent *wrappedEvent = static_cast<QStateMachine::WrappedEvent *>(event)->event();

            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(wrappedEvent);
            int key = keyEvent->key();

            qDebug(" kt0000 keypress");
            return key == Qt::Key_2 || key == Qt::Key_8 || key == Qt::Key_6 ||
                   key == Qt::Key_4 || key == Qt::Key_Down || key == Qt::Key_Up ||
                   key == Qt::Key_Right || key == Qt::Key_Left;
        }
        else if (event->type() == QEvent::KeyPress) {

            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            int key = keyEvent->key();

            qDebug(" kt0000 keypress  2222  key:%d",key);
            //return true;
#if 1
            return key == Qt::Key_Up;// || key == Qt::Key_8 || key == Qt::Key_6 ||
                   //key == Qt::Key_4 || key == Qt::Key_Down || key == Qt::Key_Up ||
                   //key == Qt::Key_Right || key == Qt::Key_Left;
#endif
        }
        return false;
    }

    void onTransition(QEvent *event) {
        //QKeyEvent *keyEvent = static_cast<QKeyEvent *>(
            //static_cast<QStateMachine::WrappedEvent *>(event)->event());
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

        int key = keyEvent->key();
        qDebug("  state key event");
        switch (key) {
            case Qt::Key_Left:
            case Qt::Key_4:
                qDebug(" key 4 press");
                //window->movePlayer(Window::Left);
                break;
            case Qt::Key_Up:
            case Qt::Key_8:
                qDebug(" key up_qt press");
                m_pui->slot0000();
                //window->movePlayer(Window::Up);
                break;
            case Qt::Key_Right:
            case Qt::Key_6:
                //window->movePlayer(Window::Right);
                break;
            case Qt::Key_Down:
            case Qt::Key_2:
                //window->movePlayer(Window::Down);
                break;
            default:
                qDebug(" unknown key press");
                break;
        }
    }


signals:

public slots:

private:
    objui *m_pui;
};

#endif // KT0000_H
