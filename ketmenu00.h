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

class ketMenuTestCtrl : public ketBase
{
    Q_OBJECT
public:
    //explicit ketMenu00(QObject *parent = 0);
    ketMenuTestCtrl(objui *obj):ketBase(obj){}

    virtual void doKeyBackspace(){
        if( m_pui->m_para.m_testON ){
            m_pui->slotStateTransitionLeft();
        }
        else{
            m_pui->slotStateTransitionBack();
        }
    }
    virtual void doKeyEnter(){
        if(m_pui->m_para.m_testON){
            m_pui->m_para.m_testON = false;
            m_pui->doStopTest();
            m_pui->slotStateTransitionLeft();
        }
        else{
            m_pui->m_para.m_testON = true;
            m_pui->doStartTest();
            m_pui->slotStateTransitionLeft();
        }
    }
    virtual void doKeyUp(){
        if(m_pui->m_para.m_testON){
            m_pui->slotStateTransitionLeft();
        }
        else{
            m_pui->slotStateTransitionUp();
        }
    }
    virtual void doKeyDown(){
        if(m_pui->m_para.m_testON){
            m_pui->slotStateTransitionLeft();
        }
        else{
            m_pui->slotStateTransitionDown();
        }
    }
    virtual void doKeyLeft(){
        m_pui->slotStateTransitionLeft();
    }
    virtual void doKeyRight(){
        m_pui->slotStateTransitionLeft();
    }

};
class ketMenuTestFreq : public ketBase
{
    Q_OBJECT
public:
    //explicit ketMenu00(QObject *parent = 0);
    ketMenuTestFreq(objui *obj):ketBase(obj){}

    virtual void doKeyBackspace(){
        m_pui->slotStateTransitionBack();
    }
    virtual void doKeyEnter(){
        m_pui->slotStateTransitionNext();
    }
    virtual void doKeyUp(){
        m_pui->slotStateTransitionUp();
    }
    virtual void doKeyDown(){
        m_pui->slotStateTransitionDown();
    }
    virtual void doKeyLeft(){
        m_pui->slotStateTransitionLeft();
    }
    virtual void doKeyRight(){
        m_pui->slotStateTransitionRight();
    }

};

class ketAbout : public ketBase
{
    Q_OBJECT
public:
    //explicit ketMenu00(QObject *parent = 0);
    ketAbout(objui *obj):ketBase(obj){}

    virtual void doKeyBackspace(){
        m_pui->slotStateTransitionNext();
    }
    virtual void doKeyEnter(){
        m_pui->slotStateTransitionNext();
    }
    virtual void doKeyUp(){
        m_pui->slotStateTransitionNext();
    }
    virtual void doKeyDown(){
        m_pui->slotStateTransitionNext();
    }
    virtual void doKeyLeft(){
        m_pui->slotStateTransitionNext();
    }
    virtual void doKeyRight(){
        m_pui->slotStateTransitionNext();
    }

};
class ketStatus1 : public ketBase
{
    Q_OBJECT
public:
    //explicit ketMenu00(QObject *parent = 0);
    ketStatus1(objui *obj):ketBase(obj){}

    virtual void doKeyBackspace(){
        m_pui->slotStateTransitionDown();
    }
    virtual void doKeyEnter(){
        m_pui->slotStateTransitionNext();
    }
    virtual void doKeyUp(){
        if(m_pui->isStatusP2P()) m_pui->slotStateTransitionUp();
        else m_pui->slotStateTransitionDown();
    }
    virtual void doKeyDown(){
        m_pui->slotStateTransitionDown();
    }
    virtual void doKeyLeft(){
        m_pui->slotStateTransitionDown();
    }
    virtual void doKeyRight(){
        m_pui->slotStateTransitionDown();
    }

};
class ketStatus2 : public ketBase
{
    Q_OBJECT
public:
    //explicit ketMenu00(QObject *parent = 0);
    ketStatus2(objui *obj):ketBase(obj){}

    virtual void doKeyBackspace(){
        m_pui->slotStateTransitionBack();
    }
    virtual void doKeyEnter(){
        m_pui->slotStateTransitionNext();
    }
    virtual void doKeyUp(){
        m_pui->slotStateTransitionBack();
    }
    virtual void doKeyDown(){
        if(m_pui->isStatusP2P()) m_pui->slotStateTransitionDown();
        else m_pui->slotStateTransitionBack();
    }
    virtual void doKeyLeft(){
        m_pui->slotStateTransitionBack();
    }
    virtual void doKeyRight(){
        m_pui->slotStateTransitionBack();
    }

};
class ketStatus3 : public ketBase
{
    Q_OBJECT
public:
    //explicit ketMenu00(QObject *parent = 0);
    ketStatus3(objui *obj):ketBase(obj){}

    virtual void doKeyBackspace(){
        m_pui->slotStateTransitionBack();
    }
    virtual void doKeyEnter(){
        m_pui->slotStateTransitionNext();
    }
    virtual void doKeyUp(){
        m_pui->slotStateTransitionUp();
    }
    virtual void doKeyDown(){
        m_pui->slotStateTransitionBack();
    }
    virtual void doKeyLeft(){
        m_pui->slotStateTransitionBack();
    }
    virtual void doKeyRight(){
        m_pui->slotStateTransitionBack();
    }

};


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
        m_pui->m_para.m_TDMfreq1 = m_pui->m_numEditor.m_i64;
        m_pui->setTDM();
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
        qDebug(" cursor +1");
        m_pui->m_numEditor.moveCursor(1);
        m_pui->slotShowEditTDM();
    }
    virtual void doKeyRight(){
        qDebug(" cursor -1");
        m_pui->m_numEditor.moveCursor(-1);
        m_pui->slotShowEditTDM();
    }

};
class ketTDM2editor : public ketBase
{
    Q_OBJECT
public:
    //explicit ketMenu00(QObject *parent = 0);
    ketTDM2editor(objui *obj):ketBase(obj){}

    virtual void doKeyBackspace(){
        m_pui->slotStateTransitionBack();
    }
    virtual void doKeyEnter(){
        m_pui->m_para.m_TDMfreq2 = m_pui->m_numEditor.m_i64;
        m_pui->setTDM();
        m_pui->slotStateTransitionBack();
    }
    virtual void doKeyUp(){
        m_pui->m_numEditor.inc();
        m_pui->slotShowEditTDM2();
    }
    virtual void doKeyDown(){
        m_pui->m_numEditor.dec();
        m_pui->slotShowEditTDM2();
    }
    virtual void doKeyLeft(){
        qDebug(" cursor +1");
        m_pui->m_numEditor.moveCursor(1);
        m_pui->slotShowEditTDM2();
    }
    virtual void doKeyRight(){
        qDebug(" cursor -1");
        m_pui->m_numEditor.moveCursor(-1);
        m_pui->slotShowEditTDM2();
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
        m_pui->m_para.m_txRateC = m_pui->m_editorRate.getRate();
        m_pui->slotStateTransitionBack();
    }
    virtual void doKeyUp(){
        m_pui->m_editorRate.inc();
        m_pui->slotShowEditTxRateC();
    }
    virtual void doKeyDown(){
        m_pui->m_editorRate.dec();
        m_pui->slotShowEditTxRateC();
    }
    virtual void doKeyLeft(){
        m_pui->m_editorRate.dec();
        m_pui->slotShowEditTxRateC();
    }
    virtual void doKeyRight(){
        m_pui->m_editorRate.inc();
        m_pui->slotShowEditTxRateC();
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
        m_pui->m_para.m_rxRateC = m_pui->m_editorRate.getRate();
        m_pui->slotStateTransitionBack();
    }
    virtual void doKeyUp(){
        m_pui->m_editorRate.inc();
        m_pui->slotShowEditRxRateC();
    }
    virtual void doKeyDown(){
        m_pui->m_editorRate.dec();
        m_pui->slotShowEditRxRateC();
    }
    virtual void doKeyLeft(){
        m_pui->m_editorRate.dec();
        m_pui->slotShowEditRxRateC();
    }
    virtual void doKeyRight(){
        m_pui->m_editorRate.inc();
        m_pui->slotShowEditRxRateC();
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
        m_pui->setPower100c();
        m_pui->slotStateTransitionBack();
    }
    virtual void doKeyUp(){
        m_pui->m_numEditor.add(1);
        m_pui->slotShowEditPowerCentral();
    }
    virtual void doKeyDown(){
        m_pui->m_numEditor.add(-1);
        m_pui->slotShowEditPowerCentral();
    }
    virtual void doKeyLeft(){
        m_pui->m_numEditor.add(1);
        m_pui->slotShowEditPowerCentral();
    }
    virtual void doKeyRight(){
        m_pui->m_numEditor.add(-1);
        m_pui->slotShowEditPowerCentral();
    }

};
class ketTestPwrEditor : public ketBase
{
    Q_OBJECT
public:
    //explicit ketMenu00(QObject *parent = 0);
    ketTestPwrEditor(objui *obj):ketBase(obj){}

    virtual void doKeyBackspace(){
        m_pui->slotStateTransitionBack();
    }
    virtual void doKeyEnter(){
        if( m_pui->m_para.m_testPwr != m_pui->m_numEditor.m_i64 ){
            m_pui->m_para.m_testPwr = m_pui->m_numEditor.m_i64;
            m_pui->m_para.save();
        }
        m_pui->slotStateTransitionBack();
    }
    virtual void doKeyUp(){
        m_pui->m_numEditor.inc();
        m_pui->slotShowEditTestPwr();
    }
    virtual void doKeyDown(){
        m_pui->m_numEditor.dec();
        m_pui->slotShowEditTestPwr();
    }
    virtual void doKeyLeft(){
        m_pui->m_numEditor.moveCursor(1);
        m_pui->slotShowEditTestPwr();
    }
    virtual void doKeyRight(){
        m_pui->m_numEditor.moveCursor(-1);
        m_pui->slotShowEditTestPwr();
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

class ketEditorCallID : public ketBase
{
    Q_OBJECT
public:
    //explicit ketMenu00(QObject *parent = 0);
    ketEditorCallID(objui *obj):ketBase(obj){}

    virtual void doKeyBackspace(){
        m_pui->slotStateTransitionBack();
    }
    virtual void doKeyEnter(){
        m_pui->m_para.m_strCallID = m_pui->m_editorIDstr.m_IDstr;
        m_pui->m_para.save();
        m_pui->slotStateTransitionBack();
    }
    virtual void doKeyUp(){
        m_pui->m_editorIDstr.inc();
        m_pui->slotShowEditIDstr();
    }
    virtual void doKeyDown(){
        m_pui->m_editorIDstr.dec();
        m_pui->slotShowEditIDstr();
    }
    virtual void doKeyLeft(){
        m_pui->m_editorIDstr.moveLeft();
        m_pui->slotShowEditIDstr();
    }
    virtual void doKeyRight(){
        m_pui->m_editorIDstr.moveRight();
        m_pui->slotShowEditIDstr();
    }

};

class ketMenuCtrl : public ketBase
{
    Q_OBJECT
public:
    ketMenuCtrl(objui *obj):ketBase(obj){}

    virtual void doKeyBackspace(){
        m_pui->slotStateTransitionBack();
    }
    virtual void doKeyEnter(){
        m_pui->slotStateTransitionEnter();
    }
    virtual void doKeyUp(){
        m_pui->slotStateTransitionUp();
    }
    virtual void doKeyDown(){
        m_pui->slotStateTransitionDown();
    }
    virtual void doKeyLeft(){
        m_pui->slotStateTransitionUp();
    }
    virtual void doKeyRight(){
        m_pui->slotStateTransitionDown();
    }

};
class ketMenuPara : public ketBase
{
    Q_OBJECT
public:
    ketMenuPara(objui *obj):ketBase(obj){}

    virtual void doKeyBackspace(){
        m_pui->slotStateTransitionBack();
    }
    virtual void doKeyEnter(){
        if(m_pui->m_para.m_status==objPara::Status_idle){
        switch(m_pui->m_para.m_workMode){
        case objPara::Mode_p2p:
            m_pui->slotStateTransitionP2P();
            break;
        case objPara::Mode_central:
            m_pui->slotStateTransitionCentral();
            break;
        default: break;
        }
        }
        else{// connected
            switch(m_pui->m_status.m_workMode){
            case objPara::Mode_p2p:
                m_pui->slotStateTransitionP2P();
                break;
            case objPara::Mode_central:
                m_pui->slotStateTransitionCentral();
                break;
            default: break;
            }
        }
    }
    virtual void doKeyUp(){
        m_pui->slotStateTransitionUp();
    }
    virtual void doKeyDown(){
        m_pui->slotStateTransitionDown();
    }
    virtual void doKeyLeft(){
        m_pui->slotStateTransitionUp();
    }
    virtual void doKeyRight(){
        m_pui->slotStateTransitionDown();
    }

};
class ketMenuWorkMode : public ketBase
{
    Q_OBJECT
public:
    ketMenuWorkMode(objui *obj):ketBase(obj){}

    virtual void doKeyBackspace(){
        m_pui->slotStateTransitionBack();
    }
    virtual void doKeyEnter(){
        m_pui->slotStateTransitionNext();
    }
    virtual void doKeyUp(){
        m_pui->slotStateTransitionUp();
    }
    virtual void doKeyDown(){
        m_pui->slotStateTransitionDown();
    }
    virtual void doKeyLeft(){
        m_pui->slotStateTransitionLeft();
    }
    virtual void doKeyRight(){
        m_pui->slotStateTransitionRight();
    }

};
class ketBackspace : public ketBase
{
    Q_OBJECT
public:
    ketBackspace(objui *obj):ketBase(obj){}

    virtual void doKeyBackspace(){
        m_pui->slotEVzzhjBack();
    }
    virtual void doKeyEnter(){
        m_pui->slotEVzzhjBack();
    }
    virtual void doKeyUp(){
        m_pui->slotEVzzhjBack();
    }
    virtual void doKeyDown(){
        m_pui->slotEVzzhjBack();
    }
    virtual void doKeyLeft(){
        m_pui->slotEVzzhjBack();
    }
    virtual void doKeyRight(){
        m_pui->slotEVzzhjBack();
    }

};
class ketBUCpwr : public ketBase
{
    Q_OBJECT
public:
    ketBUCpwr(objui *obj):ketBase(obj){}

    virtual void doKeyBackspace(){
        m_pui->slotStateTransitionBack();
    }
    virtual void doKeyEnter(){
        m_pui->slotStateTransitionNext();
    }
    virtual void doKeyUp(){
        if(m_pui->m_para.m_swBUCpwr>0) m_pui->m_para.m_swBUCpwr=-1;
        else m_pui->m_para.m_swBUCpwr = 1;
        m_pui->slotStateTransitionRight();
    }
    virtual void doKeyDown(){
        if(m_pui->m_para.m_swBUCpwr>0) m_pui->m_para.m_swBUCpwr=-1;
        else m_pui->m_para.m_swBUCpwr = 1;
        m_pui->slotStateTransitionRight();
    }
    virtual void doKeyLeft(){
        if(m_pui->m_para.m_swBUCpwr>0) m_pui->m_para.m_swBUCpwr=-1;
        else m_pui->m_para.m_swBUCpwr = 1;
        m_pui->slotStateTransitionRight();
    }
    virtual void doKeyRight(){
        if(m_pui->m_para.m_swBUCpwr>0) m_pui->m_para.m_swBUCpwr=-1;
        else m_pui->m_para.m_swBUCpwr = 1;
        m_pui->slotStateTransitionRight();
    }

};
class ketBUC10m : public ketBase
{
    Q_OBJECT
public:
    ketBUC10m(objui *obj):ketBase(obj){}

    virtual void doKeyBackspace(){
        m_pui->slotStateTransitionBack();
    }
    virtual void doKeyEnter(){
        m_pui->slotStateTransitionNext();
    }
    virtual void doKeyUp(){
        if(m_pui->m_para.m_swBUC10m>0) m_pui->m_para.m_swBUC10m=-1;
        else m_pui->m_para.m_swBUC10m = 1;
        m_pui->slotStateTransitionRight();
    }
    virtual void doKeyDown(){
        if(m_pui->m_para.m_swBUC10m>0) m_pui->m_para.m_swBUC10m=-1;
        else m_pui->m_para.m_swBUC10m = 1;
        m_pui->slotStateTransitionRight();
    }
    virtual void doKeyLeft(){
        if(m_pui->m_para.m_swBUC10m>0) m_pui->m_para.m_swBUC10m=-1;
        else m_pui->m_para.m_swBUC10m = 1;
        m_pui->slotStateTransitionRight();
    }
    virtual void doKeyRight(){
        if(m_pui->m_para.m_swBUC10m>0) m_pui->m_para.m_swBUC10m=-1;
        else m_pui->m_para.m_swBUC10m = 1;
        m_pui->slotStateTransitionRight();
    }

};
class ketLNB10m : public ketBase
{
    Q_OBJECT
public:
    ketLNB10m(objui *obj):ketBase(obj){}

    virtual void doKeyBackspace(){
        m_pui->slotStateTransitionBack();
    }
    virtual void doKeyEnter(){
        m_pui->slotStateTransitionNext();
    }
    virtual void doKeyUp(){
        if(m_pui->m_para.m_swLNB10m>0) m_pui->m_para.m_swLNB10m=-1;
        else m_pui->m_para.m_swLNB10m = 1;
        m_pui->slotStateTransitionRight();
    }
    virtual void doKeyDown(){
        if(m_pui->m_para.m_swLNB10m>0) m_pui->m_para.m_swLNB10m=-1;
        else m_pui->m_para.m_swLNB10m = 1;
        m_pui->slotStateTransitionRight();
    }
    virtual void doKeyLeft(){
        if(m_pui->m_para.m_swLNB10m>0) m_pui->m_para.m_swLNB10m=-1;
        else m_pui->m_para.m_swLNB10m = 1;
        m_pui->slotStateTransitionRight();
    }
    virtual void doKeyRight(){
        if(m_pui->m_para.m_swLNB10m>0) m_pui->m_para.m_swLNB10m=-1;
        else m_pui->m_para.m_swLNB10m = 1;
        m_pui->slotStateTransitionRight();
    }

};
class ketLNBpwr : public ketBase
{
    Q_OBJECT
public:
    ketLNBpwr(objui *obj):ketBase(obj){}

    virtual void doKeyBackspace(){
        m_pui->slotStateTransitionBack();
    }
    virtual void doKeyEnter(){
        m_pui->slotStateTransitionNext();
    }
    virtual void doKeyUp(){
        if(m_pui->m_para.m_swLNBpwr>0) m_pui->m_para.m_swLNBpwr=-1;
        else m_pui->m_para.m_swLNBpwr = 1;
        m_pui->slotStateTransitionRight();
    }
    virtual void doKeyDown(){
        if(m_pui->m_para.m_swLNBpwr>0) m_pui->m_para.m_swLNBpwr=-1;
        else m_pui->m_para.m_swLNBpwr = 1;
        m_pui->slotStateTransitionRight();
    }
    virtual void doKeyLeft(){
        if(m_pui->m_para.m_swLNBpwr>0) m_pui->m_para.m_swLNBpwr=-1;
        else m_pui->m_para.m_swLNBpwr = 1;
        m_pui->slotStateTransitionRight();
    }
    virtual void doKeyRight(){
        if(m_pui->m_para.m_swLNBpwr>0) m_pui->m_para.m_swLNBpwr=-1;
        else m_pui->m_para.m_swLNBpwr = 1;
        m_pui->slotStateTransitionRight();
    }

};

class ketWorkMode1 : public ketBase
{
    Q_OBJECT
public:
    ketWorkMode1(objui *obj):ketBase(obj){}

    virtual void doKeyBackspace(){
        m_pui->m_bErrChangeWorkMode = false;
        m_pui->slotStateTransitionBack();
    }
    virtual void doKeyEnter(){
        if(m_pui->m_bErrChangeWorkMode) m_pui->m_bErrChangeWorkMode=false;
        else if( m_pui->m_para.m_workMode != objPara::Mode_p2p ){
            if(m_pui->m_para.m_status!=objPara::Status_idle){
                m_pui->m_bErrChangeWorkMode = true;
            }
            else{
                m_pui->m_para.m_workMode=objPara::Mode_p2p;
                m_pui->m_para.save();
            }
        }
        m_pui->slotStateTransitionEnter();
    }
    virtual void doKeyUp(){
        m_pui->m_bErrChangeWorkMode = false;
        m_pui->slotStateTransitionDown();
    }
    virtual void doKeyDown(){
        m_pui->m_bErrChangeWorkMode = false;
        m_pui->slotStateTransitionDown();
    }
    virtual void doKeyLeft(){
        m_pui->m_bErrChangeWorkMode = false;
        m_pui->slotStateTransitionDown();
    }
    virtual void doKeyRight(){
        m_pui->m_bErrChangeWorkMode = false;
        m_pui->slotStateTransitionDown();
    }

};
class ketWorkMode2 : public ketBase
{
    Q_OBJECT
public:
    ketWorkMode2(objui *obj):ketBase(obj){}

    virtual void doKeyBackspace(){
        m_pui->m_bErrChangeWorkMode = false;
        m_pui->slotStateTransitionBack();
    }
    virtual void doKeyEnter(){
        if(m_pui->m_bErrChangeWorkMode) m_pui->m_bErrChangeWorkMode=false;
        else if( m_pui->m_para.m_workMode != objPara::Mode_central ){
            if(m_pui->m_para.m_status!=objPara::Status_idle){
                m_pui->m_bErrChangeWorkMode = true;
            }
            else{
                m_pui->m_para.m_workMode=objPara::Mode_central;
                m_pui->m_para.save();
            }
        }
        m_pui->slotStateTransitionEnter();
    }
    virtual void doKeyUp(){
        m_pui->m_bErrChangeWorkMode = false;
        m_pui->slotStateTransitionDown();
    }
    virtual void doKeyDown(){
        m_pui->m_bErrChangeWorkMode = false;
        m_pui->slotStateTransitionDown();
    }
    virtual void doKeyLeft(){
        m_pui->m_bErrChangeWorkMode = false;
        m_pui->slotStateTransitionDown();
    }
    virtual void doKeyRight(){
        m_pui->m_bErrChangeWorkMode = false;
        m_pui->slotStateTransitionDown();
    }

};
class ketDevMode1 : public ketBase
{
    Q_OBJECT
public:
    ketDevMode1(objui *obj):ketBase(obj){}

    virtual void doKeyBackspace(){
        switch(m_pui->m_para.m_workMode){
        case objPara::Mode_p2p:
            m_pui->slotStateTransitionP2P();
            break;
        case objPara::Mode_central:
            m_pui->slotStateTransitionCentral();
            break;
        default:
            break;
        }
    }
    virtual void doKeyEnter(){
        m_pui->m_para.m_devMode=objPara::DevMode_bridge;
        m_pui->m_para.save();
        if(m_pui->m_para.m_workMode==objPara::Mode_p2p) m_pui->doCallP2Pagain();
        m_pui->slotStateTransitionEnter();
    }
    virtual void doKeyUp(){
        m_pui->slotStateTransitionDown();
    }
    virtual void doKeyDown(){
        m_pui->slotStateTransitionDown();
    }
    virtual void doKeyLeft(){
        m_pui->slotStateTransitionDown();
    }
    virtual void doKeyRight(){
        m_pui->slotStateTransitionDown();
    }

};
class ketDevMode2 : public ketBase
{
    Q_OBJECT
public:
    ketDevMode2(objui *obj):ketBase(obj){}

    virtual void doKeyBackspace(){
        switch(m_pui->m_para.m_workMode){
        case objPara::Mode_p2p:
            m_pui->slotStateTransitionP2P();
            break;
        case objPara::Mode_central:
            m_pui->slotStateTransitionCentral();
            break;
        default:
            break;
        }
    }
    virtual void doKeyEnter(){
        m_pui->m_para.m_devMode=objPara::DevMode_router;
        m_pui->m_para.save();
        if(m_pui->m_para.m_workMode==objPara::Mode_p2p) m_pui->doCallP2Pagain();
        m_pui->slotStateTransitionEnter();
    }
    virtual void doKeyUp(){
        m_pui->slotStateTransitionDown();
    }
    virtual void doKeyDown(){
        m_pui->slotStateTransitionDown();
    }
    virtual void doKeyLeft(){
        m_pui->slotStateTransitionDown();
    }
    virtual void doKeyRight(){
        m_pui->slotStateTransitionDown();
    }

};



///////////// menu login    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class ketMenuCall : public QEventTransition
{
    Q_OBJECT
public:
    //explicit ketMenu00(QObject *parent = 0);
    ketMenuCall(objui *obj):QEventTransition(obj,QEvent::KeyPress){
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
            m_pui->slotStateTransitionNext();break;
        case Qt::Key_Up:
            m_pui->moveCursorMenuCall(-1);
            m_pui->slotStateTransitionRepaint();
            break;
        case Qt::Key_Down:
            m_pui->moveCursorMenuCall(1);
            m_pui->slotStateTransitionRepaint();
            break;
        case Qt::Key_Right:
            m_pui->moveCursorMenuCall(1);
            m_pui->slotStateTransitionRepaint();
            break;
        case Qt::Key_Left:
            m_pui->moveCursorMenuCall(-1);
            m_pui->slotStateTransitionRepaint();
            break;
        default:
            m_pui->slotStateTransitionRepaint();
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
            if(m_pui->m_para.m_TxFreq != m_pui->m_numEditor.m_i64){
                m_pui->m_para.m_TxFreq=m_pui->m_numEditor.m_i64;
                m_pui->m_para.save();
                m_pui->doCallP2Pagain();
            }
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
            if(m_pui->m_para.m_RxFreq!=m_pui->m_numEditor.m_i64){
                m_pui->m_para.m_RxFreq=m_pui->m_numEditor.m_i64;
                m_pui->m_para.save();
                m_pui->doCallP2Pagain();
            }
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
class ketTestFreqEditor : public ketBase
{
    Q_OBJECT
public:
    //explicit ketMenu00(QObject *parent = 0);
    ketTestFreqEditor(objui *obj):ketBase(obj){}

    virtual void doKeyBackspace(){
        m_pui->slotStateTransitionBack();
    }
    virtual void doKeyEnter(){
        if( m_pui->m_para.m_testFreq != m_pui->m_numEditor.m_i64 ){
            m_pui->m_para.m_testFreq = m_pui->m_numEditor.m_i64;
            m_pui->m_para.save();
        }
        m_pui->slotStateTransitionBack();
    }
    virtual void doKeyUp(){
        m_pui->m_numEditor.inc();
        m_pui->slotShowEditTestFreq();
    }
    virtual void doKeyDown(){
        m_pui->m_numEditor.dec();
        m_pui->slotShowEditTestFreq();
    }
    virtual void doKeyLeft(){
        m_pui->m_numEditor.moveCursor(1);
        m_pui->slotShowEditTestFreq();
    }
    virtual void doKeyRight(){
        m_pui->m_numEditor.moveCursor(-1);
        m_pui->slotShowEditTestFreq();
    }

};

class ketBUCfreqEditor : public ketBase
{
    Q_OBJECT
public:
    //explicit ketMenu00(QObject *parent = 0);
    ketBUCfreqEditor(objui *obj):ketBase(obj){}

    virtual void doKeyBackspace(){
        m_pui->slotStateTransitionBack();
    }
    virtual void doKeyEnter(){
#if 0
        if( m_pui->m_para.m_BUCfreq != m_pui->m_numEditor.m_i64 ){
            m_pui->m_para.m_BUCfreq = m_pui->m_numEditor.m_i64;
            m_pui->m_para.save();
            m_pui->doCallP2Pagain();
        }
#endif
        m_pui->setBUCfreq(m_pui->m_numEditor.m_i64);
        m_pui->m_para.m_BUCfreq = m_pui->m_numEditor.m_i64;
        m_pui->slotStateTransitionBack();
    }
    virtual void doKeyUp(){
        m_pui->m_numEditor.inc();
        m_pui->slotShowEditBUCfreq();
    }
    virtual void doKeyDown(){
        m_pui->m_numEditor.dec();
        m_pui->slotShowEditBUCfreq();
    }
    virtual void doKeyLeft(){
        m_pui->m_numEditor.moveCursor(1);
        m_pui->slotShowEditBUCfreq();
    }
    virtual void doKeyRight(){
        m_pui->m_numEditor.moveCursor(-1);
        m_pui->slotShowEditBUCfreq();
    }

};

class ketLNBfreqEditor : public ketBase
{
    Q_OBJECT
public:
    //explicit ketMenu00(QObject *parent = 0);
    ketLNBfreqEditor(objui *obj):ketBase(obj){}

    virtual void doKeyBackspace(){
        m_pui->slotStateTransitionBack();
    }
    virtual void doKeyEnter(){
#if 0
        if(m_pui->m_para.m_LNBfreq != m_pui->m_numEditor.m_i64){
            m_pui->m_para.m_LNBfreq = m_pui->m_numEditor.m_i64;
            m_pui->m_para.save();// fixme
            //m_pui->doCallP2Pagain();
            // fixme
        }
#endif
        m_pui->setLNBfreq(m_pui->m_numEditor.m_i64);
        m_pui->m_para.m_LNBfreq = m_pui->m_numEditor.m_i64;
        m_pui->slotStateTransitionBack();
    }
    virtual void doKeyUp(){
        m_pui->m_numEditor.inc();
        m_pui->slotShowEditLNBfreq();
    }
    virtual void doKeyDown(){
        m_pui->m_numEditor.dec();
        m_pui->slotShowEditLNBfreq();
    }
    virtual void doKeyLeft(){
        m_pui->m_numEditor.moveCursor(1);
        m_pui->slotShowEditLNBfreq();
    }
    virtual void doKeyRight(){
        m_pui->m_numEditor.moveCursor(-1);
        m_pui->slotShowEditLNBfreq();
    }

};

// page40 enter
class ketTxPSKeditor : public ketBase
{
    Q_OBJECT
public:
    ketTxPSKeditor(objui *obj):ketBase(obj){}

    virtual void doKeyBackspace(){
        m_pui->slotStateTransitionBack();
    }
    virtual void doKeyEnter(){
        if(m_pui->m_para.m_txPSK!=m_pui->m_numEditor.m_nPSK){
            m_pui->m_para.m_txPSK = m_pui->m_numEditor.m_nPSK;
            // save
        }
        m_pui->slotStateTransitionBack();
    }
    virtual void doKeyUp(){
        m_pui->m_numEditor.pskInc(-1);
        m_pui->slotShowEditTxPSK();
    }
    virtual void doKeyDown(){
        m_pui->m_numEditor.pskInc();
        m_pui->slotShowEditTxPSK();
    }
    virtual void doKeyLeft(){
        m_pui->m_numEditor.pskInc(-1);
        m_pui->slotShowEditTxPSK();
    }
    virtual void doKeyRight(){
        m_pui->m_numEditor.pskInc();
        m_pui->slotShowEditTxPSK();
    }

};
class ketRxPSKeditor : public ketBase
{
    Q_OBJECT
public:
    ketRxPSKeditor(objui *obj):ketBase(obj){}

    virtual void doKeyBackspace(){
        m_pui->slotStateTransitionBack();
    }
    virtual void doKeyEnter(){
        if(m_pui->m_para.m_rxPSK!=m_pui->m_numEditor.m_nPSK){
            m_pui->m_para.m_rxPSK = m_pui->m_numEditor.m_nPSK;
            // save
        }
        m_pui->slotStateTransitionBack();
    }
    virtual void doKeyUp(){
        m_pui->m_numEditor.pskInc(-1);
        m_pui->slotShowEditRxPSK();
    }
    virtual void doKeyDown(){
        m_pui->m_numEditor.pskInc();
        m_pui->slotShowEditRxPSK();
    }
    virtual void doKeyLeft(){
        m_pui->m_numEditor.pskInc(-1);
        m_pui->slotShowEditRxPSK();
    }
    virtual void doKeyRight(){
        m_pui->m_numEditor.pskInc();
        m_pui->slotShowEditRxPSK();
    }

};


class ketTxRateEditor : public ketBase
{
    Q_OBJECT
public:
    //explicit ketMenu00(QObject *parent = 0);
    ketTxRateEditor(objui *obj):ketBase(obj){}

    virtual void doKeyBackspace(){
        m_pui->slotStateTransitionBack();
    }
    virtual void doKeyEnter(){
        if(m_pui->m_para.m_txRate != m_pui->m_numEditor.m_ArrayRate[m_pui->m_numEditor.m_nIdxRate]){
            m_pui->m_para.m_txRate = m_pui->m_numEditor.m_ArrayRate[m_pui->m_numEditor.m_nIdxRate];
            m_pui->m_para.save();
            m_pui->doCallP2Pagain();
        }
        m_pui->slotStateTransitionBack();
    }
    virtual void doKeyUp(){
        m_pui->m_numEditor.incIdx();
        m_pui->slotShowEditTxRate();
    }
    virtual void doKeyDown(){
        m_pui->m_numEditor.decIdx();
        m_pui->slotShowEditTxRate();
    }
    virtual void doKeyLeft(){
        m_pui->m_numEditor.incIdx();
        m_pui->slotShowEditTxRate();
    }
    virtual void doKeyRight(){
        m_pui->m_numEditor.decIdx();
        m_pui->slotShowEditTxRate();
    }

};
class ketTxRateEditorPSK : public ketBase
{
    Q_OBJECT
public:
    //explicit ketMenu00(QObject *parent = 0);
    ketTxRateEditorPSK(objui *obj):ketBase(obj){}

    virtual void doKeyBackspace(){
        m_pui->slotStateTransitionBack();
    }
    virtual void doKeyEnter(){
        if(m_pui->m_para.m_txRate != m_pui->m_numEditor.m_num){
            m_pui->m_para.m_txRate = m_pui->m_numEditor.m_num;
            m_pui->m_para.save();
            m_pui->doCallP2Pagain();
        }
        m_pui->slotStateTransitionBack();
    }
    virtual void doKeyUp(){
        m_pui->m_numEditor.incRatePSK(1);// .incIdx();
        m_pui->slotShowEditTxRatePSK();
    }
    virtual void doKeyDown(){
        m_pui->m_numEditor.incRatePSK(-1);// .decIdx();
        m_pui->slotShowEditTxRatePSK();
    }
    virtual void doKeyLeft(){
        m_pui->m_numEditor.incRatePSK(-1);// .incIdx();
        m_pui->slotShowEditTxRatePSK();
    }
    virtual void doKeyRight(){
        m_pui->m_numEditor.incRatePSK(1);// .decIdx();
        m_pui->slotShowEditTxRatePSK();
    }

};
class ketRxRateEditorPSK : public ketBase
{
    Q_OBJECT
public:
    //explicit ketMenu00(QObject *parent = 0);
    ketRxRateEditorPSK(objui *obj):ketBase(obj){}

    virtual void doKeyBackspace(){
        m_pui->slotStateTransitionBack();
    }
    virtual void doKeyEnter(){
        if(m_pui->m_para.m_rxRate != m_pui->m_numEditor.m_num){
            m_pui->m_para.m_rxRate = m_pui->m_numEditor.m_num;
            m_pui->m_para.save();
            m_pui->doCallP2Pagain();
        }
        m_pui->slotStateTransitionBack();
    }
    virtual void doKeyUp(){
        m_pui->m_numEditor.incRatePSK(1);// .incIdx();
        m_pui->slotShowEditRxRatePSK();
    }
    virtual void doKeyDown(){
        m_pui->m_numEditor.incRatePSK(-1);// .decIdx();
        m_pui->slotShowEditRxRatePSK();
    }
    virtual void doKeyLeft(){
        m_pui->m_numEditor.incRatePSK(-1);// .incIdx();
        m_pui->slotShowEditRxRatePSK();
    }
    virtual void doKeyRight(){
        m_pui->m_numEditor.incRatePSK(1);// .decIdx();
        m_pui->slotShowEditRxRatePSK();
    }

};
class ketRxRateEditor : public ketBase
{
    Q_OBJECT
public:
    //explicit ketMenu00(QObject *parent = 0);
    ketRxRateEditor(objui *obj):ketBase(obj){}

    virtual void doKeyBackspace(){
        m_pui->slotStateTransitionBack();
    }
    virtual void doKeyEnter(){
        if(m_pui->m_para.m_rxRate != m_pui->m_numEditor.m_ArrayRate[m_pui->m_numEditor.m_nIdxRate]){
            m_pui->m_para.m_rxRate = m_pui->m_numEditor.m_ArrayRate[m_pui->m_numEditor.m_nIdxRate];
            m_pui->m_para.save();
            m_pui->doCallP2Pagain();
        }
        m_pui->slotStateTransitionBack();
    }
    virtual void doKeyUp(){
        m_pui->m_numEditor.incIdx();
        m_pui->slotShowEditRxRate();
    }
    virtual void doKeyDown(){
        m_pui->m_numEditor.decIdx();
        m_pui->slotShowEditRxRate();
    }
    virtual void doKeyLeft(){
        m_pui->m_numEditor.incIdx();
        m_pui->slotShowEditRxRate();
    }
    virtual void doKeyRight(){
        m_pui->m_numEditor.decIdx();
        m_pui->slotShowEditRxRate();
    }

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
            if(m_pui->m_para.m_power100 != m_pui->m_numEditor.m_i64){
                m_pui->m_para.m_power100=m_pui->m_numEditor.m_i64;
                m_pui->m_para.save();
                m_pui->doCallP2Pagain();
            }
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
