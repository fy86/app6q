#include "editorcallid.h"

editorCallID::editorCallID(QObject *parent) :
    QObject(parent)
{
    m_i64 = 0;
}

void editorCallID::inc(int one)
{
    qint64 i,j;
    i = m_i64/(10*m_step64) * (10*m_step64);
    j = m_i64%(10*m_step64);
    if(one>0){
        j = (j+m_step64)%(10*m_step64);
    }
    else{
        j = (j-m_step64+10*m_step64)%(10*m_step64);
    }
    m_i64 = i+j;

}

void editorCallID::setNumber8(qint64 i64)
{
    m_i64 = i64 % 100000000;
    m_i64old = m_i64;

    m_nLen=8;

    m_pos70 = 7;

    setStep();
}

void editorCallID::setStep()
{
    int i;
    m_step64 = 1;
    for(i=0;i<m_pos70;i++){
        m_step64 *= 10;
    }

}
void editorCallID::movePos(int one)
{
    m_pos70 = (m_pos70+one+m_nLen)%m_nLen;
    setStep();
}
int editorCallID::getCursor()
{
    if(m_pos70>3) return 7-m_pos70;
    else return 7-m_pos70 + 1;

}
