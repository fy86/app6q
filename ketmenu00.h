#ifndef KETMENU00_H
#define KETMENU00_H

#include <QObject>
#include <QEvent>
#include <QEventTransition>
#include <QtGui/QKeyEvent>

#include "objui.h"
class ketBase : public QEventTransition
{
    Q_OBJECT
public:
    //explicit ketMenu00(QObject *parent = 0);
    ketBase(objui *obj):QEventTransition(obj,QEvent::KeyPress){
        m_pui = obj;
    }

    objui *m_pui;

    virtual void doKeyUp(){}
    virtual void doKeyDown(){}
    virtual void doKeyRight(){}
    virtual void doKeyLeft(){}
    virtual void doKeyEnter(){}
    virtual void doKeyBackspace(){}

protected:
    bool eventTest(QEvent *event) {
        if (event->type() == QEvent::StateMachineWrapped &&
            static_cast<QStateMachine::WrappedEvent *>(event)->event()->type() == QEvent::KeyPress) {
            QEvent *wrappedEvent = static_cast<QStateMachine::WrappedEvent *>(event)->event();

            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(wrappedEvent);
            int key = keyEvent->key();

            return key == Qt::Key_Down
                    || key==Qt::Key_Right
                    || key == Qt::Key_Up
                    || key == Qt::Key_Left
                    || key == Qt::Key_Enter
                    || key == Qt::Key_Backspace;
        }
        else if (event->type() == QEvent::KeyPress) {

            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            int key = keyEvent->key();

            return key == Qt::Key_Down
                    || key==Qt::Key_Right
                    || key == Qt::Key_Up
                    || key == Qt::Key_Left
                    || key == Qt::Key_Enter
                    || key == Qt::Key_Backspace;
        }
        return false;
    }

    void onTransition(QEvent *event) {
        //QKeyEvent *keyEvent = static_cast<QKeyEvent *>(
            //static_cast<QStateMachine::WrappedEvent *>(event)->event());
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

        int key = keyEvent->key();
        switch (key) {
        case Qt::Key_Left: doKeyLeft(); break;
        case Qt::Key_Up: doKeyUp(); break;
        case Qt::Key_Right: doKeyRight(); break;
        case Qt::Key_Down: doKeyDown(); break;
        case Qt::Key_Enter: doKeyEnter(); break;
        case Qt::Key_Backspace: doKeyBackspace(); break;
        default:  break;
        }
    }


signals:

public slots:

};
#if 0
#endif
class ketTDMeditor : public ketBase
{
    Q_OBJECT
public:
    //explicit ketMenu00(QObject *parent = 0);
    ketTDMeditor(objui *obj):ketBase(obj){}

    virtual void doKeyBackspace(){
        m_pui->slotStateTransitionBack();
    }
    virtual void doKeyEnter(){
        m_pui->m_para.m_TDMfreq = m_pui->m_numEditor.m_i64;
        m_pui->slotStateTransitionBack();
    }
    virtual void doKeyUp(){
        m_pui->m_numEditor.inc();
        m_pui->slotShowEditTDM();
    }
    virtual void doKeyDown(){
        m_pui->m_numEditor.dec();
        m_pui->slotShowEditTDM();
    }
    virtual void doKeyLeft(){
        m_pui->m_numEditor.moveCursor(1);
        m_pui->slotShowEditTDM();
    }
    virtual void doKeyRight(){
        m_pui->m_numEditor.moveCursor(-1);
        m_pui->slotShowEditTDM();
    }

};
class ketTxRateCentralEditor : public ketBase
{
    Q_OBJECT
public:
    //explicit ketMenu00(QObject *parent = 0);
    ketTxRateCentralEditor(objui *obj):ketBase(obj){}

    virtual void doKeyBackspace(){
        m_pui->slotStateTransitionBack();
    }
    virtual void doKeyEnter(){
        m_pui->m_para.m_TxRateCentral = m_pui->m_numEditor.m_i64;
        m_pui->slotStateTransitionBack();
    }
    virtual void doKeyUp(){
        m_pui->m_numEditor.inc();
        m_pui->slotShowEditTxRateCentral();
    }
    virtual void doKeyDown(){
        m_pui->m_numEditor.dec();
        m_pui->slotShowEditTxRateCentral();
    }
    virtual void doKeyLeft(){
        m_pui->m_numEditor.moveCursor(1);
        m_pui->slotShowEditTxRateCentral();
    }
    virtual void doKeyRight(){
        m_pui->m_numEditor.moveCursor(-1);
        m_pui->slotShowEditTxRateCentral();
    }

};
class ketRxRateCentralEditor : public ketBase
{
    Q_OBJECT
public:
    //explicit ketMenu00(QObject *parent = 0);
    ketRxRateCentralEditor(objui *obj):ketBase(obj){}

    virtual void doKeyBackspace(){
        m_pui->slotStateTransitionBack();
    }
    virtual void doKeyEnter(){
        m_pui->m_para.m_RxRateCentral = m_pui->m_numEditor.m_i64;
        m_pui->slotStateTransitionBack();
    }
    virtual void doKeyUp(){
        m_pui->m_numEditor.inc();
        m_pui->slotShowEditRxRateCentral();
    }
    virtual void doKeyDown(){
        m_pui->m_numEditor.dec();
        m_pui->slotShowEditRxRateCentral();
    }
    virtual void doKeyLeft(){
        m_pui->m_numEditor.moveCursor(1);
        m_pui->slotShowEditRxRateCentral();
    }
    virtual void doKeyRight(){
        m_pui->m_numEditor.moveCursor(-1);
        m_pui->slotShowEditRxRateCentral();
    }

};
class ketPowerCentralEditor : public ketBase
{
    Q_OBJECT
public:
    //explicit ketMenu00(QObject *parent = 0);
    ketPowerCentralEditor(objui *obj):ketBase(obj){}

    virtual void doKeyBackspace(){
        m_pui->slotStateTransitionBack();
    }
    virtual void doKeyEnter(){
        m_pui->m_para.m_power100Central = m_pui->m_numEditor.m_i64;
        m_pui->slotStateTransitionBack();
    }
    virtual void doKeyUp(){
        m_pui->m_numEditor.inc();
        m_pui->slotShowEditPowerCentral();
    }
    virtual void doKeyDown(){
        m_pui->m_numEditor.dec();
        m_pui->slotShowEditPowerCentral();
    }
    virtual void doKeyLeft(){
        m_pui->m_numEditor.moveCursor(1);
        m_pui->slotShowEditPowerCentral();
    }
    virtual void doKeyRight(){
        m_pui->m_numEditor.moveCursor(-1);
        m_pui->slotShowEditPowerCentral();
    }

};
class ketNumberEditor : public ketBase
{
    Q_OBJECT
public:
    //explicit ketMenu00(QObject *parent = 0);
    ketNumberEditor(objui *obj):ketBase(obj){}

    virtual void doKeyBackspace(){
        m_pui->slotStateTransitionBack();
    }
    virtual void doKeyEnter(){

        m_pui->m_para.m_number = m_pui->m_numEditor.m_i64;
        sprintf(m_pui->m_para.m_pNumber,"%08lld",m_pui->m_para.m_number);
        m_pui->slotStateTransitionBack();
    }
    virtual void doKeyUp(){
        m_pui->m_numEditor.strInc();
        m_pui->slotShowEditNumber();
    }
    virtual void doKeyDown(){
        m_pui->m_numEditor.strDec();
        m_pui->slotShowEditNumber();
    }
    virtual void doKeyLeft(){
        m_pui->m_numEditor.strMoveCursor(1);
        m_pui->slotShowEditNumber();
    }
    virtual void doKeyRight(){
        m_pui->m_numEditor.strMoveCursor(-1);
        m_pui->slotShowEditNumber();
    }

};

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
                    || key == Qt::Key_Left
                    || key == Qt::Key_Enter;
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
            case Qt::Key_Enter:
                m_pui->slotStateTransitionEnter();
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


class ketMenuPara : public QEventTransition
{
    Q_OBJECT
public:
    //explicit ketMenu00(QObject *parent = 0);
    ketMenuPara(objui *obj):QEventTransition(obj,QEvent::KeyPress){
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
                    || key == Qt::Key_Left
                    || key == Qt::Key_Enter;
        }
        else if (event->type() == QEvent::KeyPress) {

            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            int key = keyEvent->key();

            //qDebug(" ketMenu00 keypress  2222  key:%d",key);
            //return true;
            return key == Qt::Key_Down
                    || key==Qt::Key_Right
                    || key == Qt::Key_Up
                    || key == Qt::Key_Left
                    || key == Qt::Key_Enter;
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
        case Qt::Key_Enter:
            if(m_pui->m_para.m_bModeP2P) m_pui->slotStateTransitionP2P();
            else m_pui->slotStateTransitionCentral();
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

///////////// menu login    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class ketMenu10 : public QEventTransition
{
    Q_OBJECT
public:
    //explicit ketMenu00(QObject *parent = 0);
    ketMenu10(objui *obj):QEventTransition(obj,QEvent::KeyPress){
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
                    || key == Qt::Key_Left
                    || key == Qt::Key_Enter
                    || key == Qt::Key_Backspace;
        }
        else if (event->type() == QEvent::KeyPress) {

            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            int key = keyEvent->key();

            //qDebug(" ketMenu00 keypress  2222  key:%d",key);
            //return true;
            return key == Qt::Key_Down
                    || key==Qt::Key_Right
                    || key == Qt::Key_Up
                    || key == Qt::Key_Left
                    || key == Qt::Key_Enter
                    || key == Qt::Key_Backspace;
        }
        return false;
    }

    void onTransition(QEvent *event) {
        //QKeyEvent *keyEvent = static_cast<QKeyEvent *>(
            //static_cast<QStateMachine::WrappedEvent *>(event)->event());
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

        int key = keyEvent->key();
        switch (key) {
        case Qt::Key_Backspace:
            m_pui->slotStateTransitionBackspace(); break;
        case Qt::Key_Enter:
            m_pui->doMenu10();break;
        case Qt::Key_Up:
            m_pui->changeSelectMenu10(-1);break;
        case Qt::Key_Down:
            m_pui->changeSelectMenu10(-1);break;
        default: break;
        }
    }


signals:

public slots:

private:
    objui *m_pui;

};

///////////// number editor    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class ketNumEditor : public QEventTransition
{
    Q_OBJECT
public:
    //explicit ketMenu00(QObject *parent = 0);
    ketNumEditor(objui *obj):QEventTransition(obj,QEvent::KeyPress){
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
                    || key == Qt::Key_Left
                    || key == Qt::Key_Enter
                    || key == Qt::Key_Backspace;
        }
        else if (event->type() == QEvent::KeyPress) {

            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            int key = keyEvent->key();

            //qDebug(" ketMenu00 keypress  2222  key:%d",key);
            //return true;
            return key == Qt::Key_Down
                    || key==Qt::Key_Right
                    || key == Qt::Key_Up
                    || key == Qt::Key_Left
                    || key == Qt::Key_Enter
                    || key == Qt::Key_Backspace;
        }
        return false;
    }

    void onTransition(QEvent *event) {
        //QKeyEvent *keyEvent = static_cast<QKeyEvent *>(
            //static_cast<QStateMachine::WrappedEvent *>(event)->event());
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

        int key = keyEvent->key();
        switch (key) {
        case Qt::Key_Backspace:
            m_pui->slotStateTransitionBackspace();
            break;
        case Qt::Key_Enter:
            m_pui->doMenu10();
            break;
        case Qt::Key_Up:
            m_pui->changeSelectMenu10(-1);
            break;
        case Qt::Key_Down:
            m_pui->changeSelectMenu10(-1);
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


class ketTxFreqEditor : public QEventTransition
{
    Q_OBJECT
public:
    //explicit ketMenu00(QObject *parent = 0);
    ketTxFreqEditor(objui *obj):QEventTransition(obj,QEvent::KeyPress){
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
                    || key == Qt::Key_Left
                    || key == Qt::Key_Enter
                    || key == Qt::Key_Backspace;
        }
        else if (event->type() == QEvent::KeyPress) {

            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            int key = keyEvent->key();

            //qDebug(" ketMenu00 keypress  2222  key:%d",key);
            //return true;
            return key == Qt::Key_Down
                    || key==Qt::Key_Right
                    || key == Qt::Key_Up
                    || key == Qt::Key_Left
                    || key == Qt::Key_Enter
                    || key == Qt::Key_Backspace;
        }
        return false;
    }

    void onTransition(QEvent *event) {
        //QKeyEvent *keyEvent = static_cast<QKeyEvent *>(
            //static_cast<QStateMachine::WrappedEvent *>(event)->event());
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

        int key = keyEvent->key();
        switch (key) {
        case Qt::Key_Backspace:
            m_pui->slotStateTransitionBack();
            break;
        case Qt::Key_Enter:
            m_pui->m_para.m_TxFreq=m_pui->m_numEditor.m_i64;
            m_pui->slotStateTransitionBack();
            break;
        case Qt::Key_Up:
            m_pui->m_numEditor.inc();
            m_pui->slotShowEditTxFreq();
            break;
        case Qt::Key_Down:
            m_pui->m_numEditor.dec();
            m_pui->slotShowEditTxFreq();
            break;
        case Qt::Key_Right:
            m_pui->m_numEditor.moveCursor(-1);
            m_pui->slotShowEditTxFreq();
            break;
        case Qt::Key_Left:
            m_pui->m_numEditor.moveCursor(1);
            m_pui->slotShowEditTxFreq();
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
class ketRxFreqEditor : public QEventTransition
{
    Q_OBJECT
public:
    //explicit ketMenu00(QObject *parent = 0);
    ketRxFreqEditor(objui *obj):QEventTransition(obj,QEvent::KeyPress){
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
                    || key == Qt::Key_Left
                    || key == Qt::Key_Enter
                    || key == Qt::Key_Backspace;
        }
        else if (event->type() == QEvent::KeyPress) {

            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            int key = keyEvent->key();

            //qDebug(" ketMenu00 keypress  2222  key:%d",key);
            //return true;
            return key == Qt::Key_Down
                    || key==Qt::Key_Right
                    || key == Qt::Key_Up
                    || key == Qt::Key_Left
                    || key == Qt::Key_Enter
                    || key == Qt::Key_Backspace;
        }
        return false;
    }

    void onTransition(QEvent *event) {
        //QKeyEvent *keyEvent = static_cast<QKeyEvent *>(
            //static_cast<QStateMachine::WrappedEvent *>(event)->event());
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

        int key = keyEvent->key();
        switch (key) {
        case Qt::Key_Backspace:
            m_pui->slotStateTransitionBack();
            break;
        case Qt::Key_Enter:
            m_pui->m_para.m_RxFreq=m_pui->m_numEditor.m_i64;
            m_pui->slotStateTransitionBack();
            break;
        case Qt::Key_Up:
            m_pui->m_numEditor.inc();
            m_pui->slotShowEditRxFreq();
            break;
        case Qt::Key_Down:
            m_pui->m_numEditor.dec();
            m_pui->slotShowEditRxFreq();
            break;
        case Qt::Key_Right:
            m_pui->m_numEditor.moveCursor(-1);
            m_pui->slotShowEditRxFreq();
            break;
        case Qt::Key_Left:
            m_pui->m_numEditor.moveCursor(1);
            m_pui->slotShowEditRxFreq();
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

class ketTxRateEditor : public QEventTransition
{
    Q_OBJECT
public:
    //explicit ketMenu00(QObject *parent = 0);
    ketTxRateEditor(objui *obj):QEventTransition(obj,QEvent::KeyPress){
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
                    || key == Qt::Key_Left
                    || key == Qt::Key_Enter
                    || key == Qt::Key_Backspace;
        }
        else if (event->type() == QEvent::KeyPress) {

            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            int key = keyEvent->key();

            //qDebug(" ketMenu00 keypress  2222  key:%d",key);
            //return true;
            return key == Qt::Key_Down
                    || key==Qt::Key_Right
                    || key == Qt::Key_Up
                    || key == Qt::Key_Left
                    || key == Qt::Key_Enter
                    || key == Qt::Key_Backspace;
        }
        return false;
    }

    void onTransition(QEvent *event) {
        //QKeyEvent *keyEvent = static_cast<QKeyEvent *>(
            //static_cast<QStateMachine::WrappedEvent *>(event)->event());
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

        int key = keyEvent->key();
        switch (key) {
        case Qt::Key_Backspace:
            m_pui->slotStateTransitionBack();
            break;
        case Qt::Key_Enter:
            m_pui->m_para.m_TxRate=m_pui->m_numEditor.m_i64;
            m_pui->slotStateTransitionBack();
            break;
        case Qt::Key_Up:
            m_pui->m_numEditor.inc();
            m_pui->slotShowEditTxRate();
            break;
        case Qt::Key_Down:
            m_pui->m_numEditor.dec();
            m_pui->slotShowEditTxRate();
            break;
        case Qt::Key_Right:
            m_pui->m_numEditor.moveCursor(-1);
            m_pui->slotShowEditTxRate();
            break;
        case Qt::Key_Left:
            m_pui->m_numEditor.moveCursor(1);
            m_pui->slotShowEditTxRate();
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
class ketRxRateEditor : public QEventTransition
{
    Q_OBJECT
public:
    //explicit ketMenu00(QObject *parent = 0);
    ketRxRateEditor(objui *obj):QEventTransition(obj,QEvent::KeyPress){
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
                    || key == Qt::Key_Left
                    || key == Qt::Key_Enter
                    || key == Qt::Key_Backspace;
        }
        else if (event->type() == QEvent::KeyPress) {

            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            int key = keyEvent->key();

            //qDebug(" ketMenu00 keypress  2222  key:%d",key);
            //return true;
            return key == Qt::Key_Down
                    || key==Qt::Key_Right
                    || key == Qt::Key_Up
                    || key == Qt::Key_Left
                    || key == Qt::Key_Enter
                    || key == Qt::Key_Backspace;
        }
        return false;
    }

    void onTransition(QEvent *event) {
        //QKeyEvent *keyEvent = static_cast<QKeyEvent *>(
            //static_cast<QStateMachine::WrappedEvent *>(event)->event());
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

        int key = keyEvent->key();
        switch (key) {
        case Qt::Key_Backspace:
            m_pui->slotStateTransitionBack();
            break;
        case Qt::Key_Enter:
            m_pui->m_para.m_RxRate=m_pui->m_numEditor.m_i64;
            m_pui->slotStateTransitionBack();
            break;
        case Qt::Key_Up:
            m_pui->m_numEditor.inc();
            m_pui->slotShowEditRxRate();
            break;
        case Qt::Key_Down:
            m_pui->m_numEditor.dec();
            m_pui->slotShowEditRxRate();
            break;
        case Qt::Key_Right:
            m_pui->m_numEditor.moveCursor(-1);
            m_pui->slotShowEditRxRate();
            break;
        case Qt::Key_Left:
            m_pui->m_numEditor.moveCursor(1);
            m_pui->slotShowEditRxRate();
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

class ketPowerEditor : public QEventTransition
{
    Q_OBJECT
public:
    //explicit ketMenu00(QObject *parent = 0);
    ketPowerEditor(objui *obj):QEventTransition(obj,QEvent::KeyPress){
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
                    || key == Qt::Key_Left
                    || key == Qt::Key_Enter
                    || key == Qt::Key_Backspace;
        }
        else if (event->type() == QEvent::KeyPress) {

            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            int key = keyEvent->key();

            //qDebug(" ketMenu00 keypress  2222  key:%d",key);
            //return true;
            return key == Qt::Key_Down
                    || key==Qt::Key_Right
                    || key == Qt::Key_Up
                    || key == Qt::Key_Left
                    || key == Qt::Key_Enter
                    || key == Qt::Key_Backspace;
        }
        return false;
    }

    void onTransition(QEvent *event) {
        //QKeyEvent *keyEvent = static_cast<QKeyEvent *>(
            //static_cast<QStateMachine::WrappedEvent *>(event)->event());
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

        int key = keyEvent->key();
        switch (key) {
        case Qt::Key_Backspace:
            m_pui->slotStateTransitionBack();
            break;
        case Qt::Key_Enter:
            m_pui->m_para.m_power100=m_pui->m_numEditor.m_i64;
            m_pui->slotStateTransitionBack();
            break;
        case Qt::Key_Up:
            m_pui->m_numEditor.inc();
            m_pui->slotShowEditPower();
            break;
        case Qt::Key_Down:
            m_pui->m_numEditor.dec();
            m_pui->slotShowEditPower();
            break;
        case Qt::Key_Right:
            m_pui->m_numEditor.moveCursor(-1);
            m_pui->slotShowEditPower();
            break;
        case Qt::Key_Left:
            m_pui->m_numEditor.moveCursor(1);
            m_pui->slotShowEditPower();
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




class ketParaPage1 : public QEventTransition
{
    Q_OBJECT
public:
    ketParaPage1(objui *obj):QEventTransition(obj,QEvent::KeyPress){
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
                    || key == Qt::Key_Left
                    || key == Qt::Key_Enter
                    || key == Qt::Key_Backspace;
        }
        else if (event->type() == QEvent::KeyPress) {

            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            int key = keyEvent->key();

            //qDebug(" ketMenu00 keypress  2222  key:%d",key);
            //return true;
            return key == Qt::Key_Down
                    || key==Qt::Key_Right
                    || key == Qt::Key_Up
                    || key == Qt::Key_Left
                    || key == Qt::Key_Enter
                    || key == Qt::Key_Backspace;
        }
        return false;
    }

    void onTransition(QEvent *event) {
        //QKeyEvent *keyEvent = static_cast<QKeyEvent *>(
            //static_cast<QStateMachine::WrappedEvent *>(event)->event());
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

        int key = keyEvent->key();
        switch (key) {
        case Qt::Key_Backspace:
            m_pui->slotStateTransitionBack();
            break;
        case Qt::Key_Enter:
            m_pui->slotStateTransitionNext();
            break;
        case Qt::Key_Up:
            m_pui->slotStateTransitionUp();
            break;
        case Qt::Key_Down:
            m_pui->slotStateTransitionDown();
            break;
        case Qt::Key_Right:
            m_pui->slotStateTransitionRight();
            break;
        case Qt::Key_Left:
            m_pui->slotStateTransitionLeft();
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
