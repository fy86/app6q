#include "editoridstr.h"

editorIDstr::editorIDstr(QObject *parent) :
    QObject(parent)
{
}

void editorIDstr::setStr(QString id)
{
    int i,len;
    m_IDstr = id;

    len = m_IDstr.size();
    for(i=0;i<len;){
        if(m_IDstr.at(i).isDigit()){
            i++;
            continue;
        }
        m_IDstr.remove(i,1);
        len--;
    }
    if(len<1) m_IDstr.append('0');
    m_cursor = 0;
}

void editorIDstr::inc()
{
    if(m_cursor>=0){// 9<==>0
        switch(0x0ff & m_IDstr.at(m_cursor).toAscii()){
        case '0': m_IDstr.replace(m_cursor,1,'1'); break;
        case '1': m_IDstr.replace(m_cursor,1,'2'); break;
        case '2': m_IDstr.replace(m_cursor,1,'3'); break;
        case '3': m_IDstr.replace(m_cursor,1,'4'); break;
        case '4': m_IDstr.replace(m_cursor,1,'5'); break;
        case '5': m_IDstr.replace(m_cursor,1,'6'); break;
        case '6': m_IDstr.replace(m_cursor,1,'7'); break;
        case '7': m_IDstr.replace(m_cursor,1,'8'); break;
        case '8': m_IDstr.replace(m_cursor,1,'9'); break;
        case '9': m_IDstr.replace(m_cursor,1,'0'); break;
        default: break;
        }
    }
    else{
        m_IDstr.insert(0,'0');
        m_cursor = 0;
    }
}
void editorIDstr::dec()
{
    if(m_cursor>0){// 9<==>0
        switch(0x0ff & m_IDstr.at(m_cursor).toAscii()){
        case '0': m_IDstr.replace(m_cursor,1,'9'); break;
        case '1': m_IDstr.replace(m_cursor,1,'0'); break;
        case '2': m_IDstr.replace(m_cursor,1,'1'); break;
        case '3': m_IDstr.replace(m_cursor,1,'2'); break;
        case '4': m_IDstr.replace(m_cursor,1,'3'); break;
        case '5': m_IDstr.replace(m_cursor,1,'4'); break;
        case '6': m_IDstr.replace(m_cursor,1,'5'); break;
        case '7': m_IDstr.replace(m_cursor,1,'6'); break;
        case '8': m_IDstr.replace(m_cursor,1,'7'); break;
        case '9': m_IDstr.replace(m_cursor,1,'8'); break;
        default: break;
        }
    }
    else if(m_cursor==0){
        switch(0x0ff & m_IDstr.at(m_cursor).toAscii()){
        case '0': m_IDstr.remove(m_cursor,1);m_cursor=-1;break;
        case '1': m_IDstr.replace(m_cursor,1,'0'); break;
        case '2': m_IDstr.replace(m_cursor,1,'1'); break;
        case '3': m_IDstr.replace(m_cursor,1,'2'); break;
        case '4': m_IDstr.replace(m_cursor,1,'3'); break;
        case '5': m_IDstr.replace(m_cursor,1,'4'); break;
        case '6': m_IDstr.replace(m_cursor,1,'5'); break;
        case '7': m_IDstr.replace(m_cursor,1,'6'); break;
        case '8': m_IDstr.replace(m_cursor,1,'7'); break;
        case '9': m_IDstr.replace(m_cursor,1,'8'); break;
        default: break;
        }
    }

    if(1>m_IDstr.size()){
        m_IDstr.append('0');
        m_cursor = 0;
    }
}
void editorIDstr::moveRight()
{
    m_cursor=(m_cursor+1)%m_IDstr.size();
}
void editorIDstr::moveLeft()
{
    if(m_cursor<0)return;
    else if(m_cursor==0) m_cursor = -1;
    else{
        m_cursor--;
    }


}
QString editorIDstr::getStrFormat()
{
    QString str;
    if(m_cursor<0) str=" "+m_IDstr;
    else str=m_IDstr;
    return str;
}

int editorIDstr::getCursor()
{
    if(m_cursor<0) return 0;
    else return m_cursor;
}


