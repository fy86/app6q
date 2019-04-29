#include "message_handle.h"
#include <QDebug>

MessageHandle::MessageHandle(RecvNotifyRPC *p_recvNotify, RecvResponseRpc *p_recvResponse, QObject *parent)
    : QObject(parent)
{
    connect(p_recvNotify, SIGNAL(sigRadioLinkState(QByteArray)), this, SLOT(radioLinkStateChangedSlot(QByteArray)));
}

void MessageHandle::radioLinkStateChangedSlot(QByteArray)
{
    qDebug() << "RadioLink State Changed";
}
