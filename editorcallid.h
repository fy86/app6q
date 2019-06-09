#ifndef EDITORCALLID_H
#define EDITORCALLID_H

#include <QObject>

class editorCallID : public QObject
{
    Q_OBJECT
public:
    explicit editorCallID(QObject *parent = 0);

    qint64 m_i64;
    qint64 m_i64old;
    int m_nLen;
    int m_pos70;
    qint64 m_step64;

    void setNumber8(qint64 i64);
    void inc(int one);
    void setStep();
    void movePos(int one);
    int getCursor();

signals:

public slots:

};

#endif // EDITORCALLID_H
