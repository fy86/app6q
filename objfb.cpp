#include "objfb.h"

objFB::objFB(QObject *parent) :
    objOled(parent)
{
    m_baFB.resize(10000);
    loaddot();
}

void objFB::loaddot()
{
    int fd;

    m_ba16.resize(300000);

#if 0
    fd=open("song12.dot",O_RDONLY);
    if(fd!=-1){
    read(fd,gp12,300000);
    close(fd);
    }
#endif
    fd=open("/root/qt/song16.dot",O_RDONLY);
    if(fd!=-1){
        read(fd,m_ba16.data(),290000);
        close(fd);
    }
#if 0
    fd=open("song24.dot",O_RDONLY);
    if(fd!=-1){
    read(fd,gp24,650000);
    close(fd);
    }
#endif
}
void objFB::zeroFB(char f)
{
    int i;
    char *p;
    p=m_baFB.data();
    for(i=0;i<(256*64/2);i++){
        *p++=f;
    }

}

void objFB::demo16()
{
    //qDebug("func demo16");

    OLED_Init();
    emit sigFlush();

    zeroFB(0x0);
    //lineXY();

    //col0();
    //Fill_BlockP(gfb,0,64-1,0,63);

    showHZdemo();

}
void objFB::showHZ16(char *p, int x, int y)
{
    char ch;
    int c1;
    int i,j,k,m;

    //qDebug("func showHZ16");

    for(i=0;i<16;i++){
        for(j=0;j<2;j++){
            ch = *p++;
            for(k=0;k<8;k++){
                m = 7-k;
                c1 = (1<<m)&ch;
                if(c1)
                   draw1(x+(j<<3)+k,y+i,0x0f);
                else
                   draw1(x+(j<<3)+k,y+i,0x0);
            }
        }
    }

}

void objFB::showHZdemo()
{
    char *pdot;
    int i,j;
    pdot = m_ba16.data()+(15*94*32);
    //pdot = gp16;
    for(i=0;i<4;i++){
        for(j=0;j<16;j++){
            showHZ16(pdot,j<<4,i<<4);
            pdot+=32;
        }
    }
    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();
}

void objFB::draw1(int x, int y, int c)
{
    int offset=0;
    int s;
    char *p;
    char ch;

    offset=y*128;
    offset+=(x/4)*2;
    switch(x%4){
#if 0
        case 0:
          offset+=1;
          s=0;
          break;
        case 1:
          offset+=1;
          s=4;
          break;
        case 2:
          s=0;
          break;
        default:
          s=4;
          break;
#endif
        case 0:
          s=4;
          break;
        case 1:
          s=0;
          break;
        case 2:
          offset+=1;
          s=4;
          break;
        default:
          offset+=1;
          s=0;
          break;
    }
    p=m_baFB.data() + offset;
    ch = *p;
    switch(s){
        case 0:
          c=0x0f & c;
          ch = (ch & 0x0ff)|c;
          break;
        case 4:
          c =(c<<4)&0x0f0;
          ch = (ch & 0x0f0)| c;
          break;
        default:
          break;
    }
    *p=ch;

}
