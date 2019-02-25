#ifndef OBJBASE_H
#define OBJBASE_H

#include <QObject>
#include <QTextCodec>
#include <QtGui/QKeyEvent>

class objBase : public QObject
{
    Q_OBJECT
public:
    explicit objBase(QObject *parent = 0);


    QTextCodec *m_pUTF8;
    QTextCodec *m_pGBK;

    void str2GBK(QString str,QByteArray &ba);
    void testbase();

signals:

public slots:

};

#endif // OBJBASE_H
