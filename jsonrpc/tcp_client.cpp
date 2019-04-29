#include "tcp_client.h"
#include <QDebug>
#include <QByteArray>

TcpClient::TcpClient(CuRpcControl *p_cuRpcControl, QObject *parent) : QObject(parent)
{
    m_tcp_client = new QTcpSocket();
    m_p_cuRpcControl = p_cuRpcControl;
}

void TcpClient::ReadDataSlot()
{
    QByteArray qba = m_tcp_client->readAll();
    QString qstr = qba;
    std::string recv_str = qstr.toStdString();
//    qDebug() << QString::fromStdString(recv_str);
    m_p_cuRpcControl->recv(recv_str);
}

void TcpClient::Connect(QString ip, unsigned short port)
{
    m_tcp_client->connectToHost(ip, port);
    connect(m_tcp_client, SIGNAL(readyRead()), this, SLOT(ReadDataSlot()));
}

void TcpClient::Send(QString data_str)
{
    m_tcp_client->write(data_str.toUtf8());
}
