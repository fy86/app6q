#ifndef STATUSPARA_H
#define STATUSPARA_H

#include <QObject>

class statuspara : public QObject
{
    Q_OBJECT
public:
    explicit statuspara(QObject *parent = 0);

    bool m_enableUpdate;//
    int m_v32;
    qint64 m_v64;
    float m_vf;
    double m_vd;

    void update64(qint64 i64);
    void enableUpdate(bool en);

signals:

public slots:

};

#endif // STATUSPARA_H
