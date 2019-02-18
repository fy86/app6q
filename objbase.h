#ifndef OBJBASE_H
#define OBJBASE_H

#include <QObject>

class objBase : public QObject
{
    Q_OBJECT
public:
    explicit objBase(QObject *parent = 0);

signals:

public slots:

};

#endif // OBJBASE_H
