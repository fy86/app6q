#ifndef THREADKEY_H
#define THREADKEY_H

#include <QObject>
#include <QThread>

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include <unistd.h>


class threadkey : public QThread
{
    Q_OBJECT
public:
    explicit threadkey(QObject *parent = 0);

    int m_fd;

    void openDev(char *p);

protected:
    virtual void run();

signals:
    void sigKey(int);

public slots:

};

#endif // THREADKEY_H
