#ifndef MESSAGEHANDLE_H
#define MESSAGEHANDLE_H

#include <QObject>
#include "recv_notify_rpc.h"
#include "recv_response_rpc.h"

class MessageHandle : public QObject
{
    Q_OBJECT
public:
    explicit MessageHandle(RecvNotifyRPC *p_recvNotify, RecvResponseRpc *p_recvResponse, QObject *parent = 0);

signals:

public slots:
    void radioLinkStateChangedSlot(QByteArray);
};

#endif // MESSAGEHANDLE_H
