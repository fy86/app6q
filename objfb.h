#ifndef OBJFB_H
#define OBJFB_H

#include <QObject>
#include <QByteArray>

#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

#include <objoled.h>

#define OLED_WIDTH 256
#define OLED_HEIGHT 64

class objFB : public objOled
{
    Q_OBJECT
public:
    explicit objFB(QObject *parent = 0);

    static char font8x12[256][12];

    int m_nPage;

    QByteArray m_ba16;
    QByteArray m_baFB;

    void loaddot();
    void draw1(int x,int y,int c);

    void demo16();
    void zeroFB(char f);
    void showHZdemo();
    void showHZ16(char *p,int x,int y);
    void show8x12(char ch,int x,int y);
    void show8x16(char ch,int x,int y);

    void centerXY(QString str,int left,int top,int w,int h,int hc,int vc);
    void strXY(QString str,int x,int y);
    void show0000();
    void show0001();


signals:

public slots:
    virtual void slotKey(int);

};

#endif // OBJFB_H
