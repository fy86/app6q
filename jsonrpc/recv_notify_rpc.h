 /**
  * \file recv_notify_rpc.h
  * \brief RPC example.
  * \author Sebastien Vincent
  */

#ifndef TEST_RPC_H
#define TEST_RPC_H 

#include <QObject>
#include <QByteArray>
#include <json/json.h>
#include "data_struct.h"

  /**
   * \class TestRpc
   * \brief RPC example.
   */
class RecvNotifyRPC : public QObject
{
    Q_OBJECT
public:
	/**
	 * \brief Reply with success.
	 * \param root JSON-RPC request
	 * \param response JSON-RPC response
	 * \return true if correctly processed, false otherwise
	 */
	bool notifyRadioLinkStateChanged(const Json::Value& root, Json::Value& response);

	/**
	*  @brief                 CU状态变化通知
	*  @param[in] 	root      通知消息json结构
	*  @param[in]   response  收到的通知的解析情况
	*  @return			      解析通知情况
	*  @retval      true      有效的通知
	*  @retval      false     无效的通知
	*/
	bool notifyCUStateChanged(const Json::Value& root, Json::Value& response);

	/**
	*  @brief                 会话状态变化通知
	*  @param[in] 	root      通知消息json结构
	*  @param[in]   response  收到的通知的解析情况
	*  @return			      解析通知情况
	*  @retval      true      有效的通知
	*  @retval      false     无效的通知
	*/
	bool notifySessionStateChanged(const Json::Value& root, Json::Value& response);

signals:
    void sigRadioLinkState(QByteArray);
    void sigCUState(QByteArray);
    void sigSessionState(QByteArray);
};

#endif /* TEST_RPC_H */

