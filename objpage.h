#ifndef OBJPAGE_H
#define OBJPAGE_H

#include <QObject>

#include <objfb.h>

class objPage : public objFB
{
    Q_OBJECT
public:
    explicit objPage(QObject *parent = 0);

    enum MYENUM {
        dev_mode_bridge,
        dev_mode_router,
        work_mode_p2p,
        work_mode_central,

        pageWQkongxian,
        pageWQtongxin,
        pageLYkongxian,
        pageLYtongxin
    };

    enum MYENUM m_devMode;
    enum MYENUM m_workMode;
    bool m_bStatConnect;
    bool m_bStatLogin;
    bool m_bEnableMenu10[4];
    int m_nSelectMenu10;

    void setSelectMenu10();


signals:

public slots:

};

#endif // OBJPAGE_H
