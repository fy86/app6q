#include "threadkey.h"

#include <linux/input.h>

threadkey::threadkey(QObject *parent) :
    QThread(parent)
{
    m_fd=-1;
}
void threadkey::openDev(char *p)
{
    m_fd = open(p, O_RDWR);
    //qDebug(" open input dev");
}

void threadkey::run()
{
    struct input_event ev;

    int len=sizeof(struct input_event);
    int ret;
    if(m_fd<0)return;
    for(;;){
        ret = read(m_fd,&ev,len);
        if(ret!=len)continue;
        if(ev.type==EV_KEY && ev.value==1){   // value.1.press   value.0.release  value.2.repeat
#if 0
            switch(ev.code){
            case KEY_LEFT:
                qDebug("key left : %d",ev.code);
                break;
            case KEY_RIGHT:
                qDebug("key right : %d ",ev.code);
                break;
            case KEY_UP:
                qDebug("key up : %d",ev.code);
                break;
            case KEY_DOWN:
                qDebug("key down : %d",ev.code);
                break;
            case KEY_ENTER:
                qDebug("key ENTER : %d",ev.code);
                break;
            case KEY_BACKSPACE:
                qDebug("key backspace : %d",ev.code);
                break;
            default:
                qDebug("key unknown : %d",ev.code);
                break;
            }
#endif
            emit sigKey(ev.code);
        }
    }
}
