#ifndef THREADSPI_H
#define THREADSPI_H

#include <QObject>

class threadSPI : public QObject
{
    Q_OBJECT
public:
    explicit threadSPI(QObject *parent = 0);

signals:

public slots:

};

#endif // THREADSPI_H
