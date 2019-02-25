#ifndef OBJPAGE_H
#define OBJPAGE_H

#include <QObject>

#include <objfb.h>

class objPage : public objFB
{
    Q_OBJECT
public:
    explicit objPage(QObject *parent = 0);

    enum pageNum{
        pageWQkongxian,
        pageWQtongxin,
        pageLYkongxian,
        pageLYtongxin
    };


signals:

public slots:

};

#endif // OBJPAGE_H
