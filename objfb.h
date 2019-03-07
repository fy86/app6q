#ifndef OBJFB_H
#define OBJFB_H

#include <QObject>
#include <QByteArray>

#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include <unistd.h>

#include <objoled.h>

#define OLED_WIDTH 256
#define OLED_HEIGHT 64

class objFB : public objOled
{
    Q_OBJECT
public:
    explicit objFB(QObject *parent = 0);

    static char font8x12[256][12];

    QByteArray m_ba16;
    QByteArray m_baFB;

    void loaddot();
    void draw1(int x,int y,int c);
    void underLine(int left,int top,int n=0,int c=0x0f);

    void demo16();
    void zeroFB(char f);
    void showHZdemo();
    void showHZ16(char *p,int x,int y,int color=0x0f,int bg=0);
    void show8x12(char ch,int x,int y,int color=0x0f,int bg=0);
    void show8x16(char ch,int x,int y,int color=0x0f,int bg=0);

    void centerXY(QString str,int left,int top,int w,int h,int hc,int vc,int color=0x0f,int bg=0);
    void strXY(QString str,int x,int y,int color=0x0f,int bg=0);

    void fillFlush();

signals:

public slots:
    virtual void slotKey(int);

    void slotShowMenu00();
    void slotShowMenu01();
    void slotShowMenu02();

};

#endif // OBJFB_H
