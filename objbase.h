#ifndef OBJBASE_H
#define OBJBASE_H

#include <QObject>
#include <QTextCodec>
#include <QtGui/QKeyEvent>
#include <QList>
#include <QFile>
#include <QMutex>
#include <QTime>
#include <QSemaphore>
#include <QDateTime>
#include <QUdpSocket>

class objBase : public QObject
{
    Q_OBJECT
public:
    explicit objBase(QObject *parent = 0);

    qint64 m_secs0;

    int m_nDebug;
    QSemaphore m_semKey;
    QSemaphore m_semLog;

    QList<QByteArray> m_listLog;

    enum myEnum{
        enumP2Pmode,

    };


    QTextCodec *m_pUTF8;
    QTextCodec *m_pGBK;

    void str2GBK(QString str,QByteArray &ba);
    void testbase();

    void writeLog(QByteArray ba);

signals:

public slots:

};


#endif // OBJBASE_H
