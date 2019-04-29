#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QtNetwork/QTcpSocket>
#include <QString>
#include "cu_rpc_control.h"

class TcpClient : public QObject
{
    Q_OBJECT
public:
    explicit TcpClient(CuRpcControl *p_cuRpcControl, QObject *parent = 0);

    /**
	*  @brief                 连接TCP服务器
	*  @param[in] 	ip        服务器ip
	*  @param[in]   port      服务器端口
	*  @return			      void
	*/
    void Connect(QString ip, unsigned short port);

    /**
	*  @brief                 向服务器发送消息
	*  @param[in] 	data_str  发送的消息内容
	*  @return			      void
	*/
    void Send(QString data_str);

signals:

public slots:
    /**
	*  @brief                 接收服务器发来的消息
	*  @param[in] 	          void
	*  @param[out]            void
	*  @return			      void
	*/
    void ReadDataSlot();

private:
    QTcpSocket *m_tcp_client;           ///< tcp socket
    CuRpcControl *m_p_cuRpcControl;     ///< jsonrpc解析类
};

#endif // TCPCLIENT_H
