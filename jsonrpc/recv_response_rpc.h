//////////////////////////////////////////////////////             
///                                                                            
///  @file     recv_response_rpc.h                                                       
///  @brief    处理接收到的来自服务器的响应                                                                          
///  @author   wly                                                                                                        
///  @version  1.0.0.1                                                
///  @date     2019.01.17                                                                               
///                                                                          
//////////////////////////////////////////////////////		

#ifndef RECV_RESPONSE_RPC_H
#define RECV_RESPONSE_RPC_H

#include <QObject>
#include <json/json.h>
#include "data_struct.h"

class RecvResponseRpc : public QObject
{
    Q_OBJECT
public:
	RecvResponseRpc();
	~RecvResponseRpc();

	/**
	*  @brief                CU状态的响应处理
	*  @param[in] 	root     从服务端接收到的json结构
	*  @param[in]   response 错误信息
	*  @return			     处理结果
	*/
	bool getCUState(const Json::Value& root, Json::Value& response);

	/**
	*  @brief                会话状态的响应处理
	*  @param[in] 	root     从服务端接收到的json结构
	*  @param[in]   response 错误信息
	*  @return			     处理结果
	*/
	bool getSessionState(const Json::Value& root, Json::Value& response);

	/**
	*  @brief                无线链路状态的响应处理
	*  @param[in] 	root     从服务端接收到的json结构
	*  @param[in]   response 错误信息
	*  @return			     处理结果
	*/
	bool getRadioLinkState(const Json::Value& root, Json::Value& response);

	/**
	*  @brief                点对点参数响应处理
	*  @param[in] 	root     从服务端接收到的json结构
	*  @param[in]   response 错误信息
	*  @return			     处理结果
	*/
	bool getP2PModeParam(const Json::Value& root, Json::Value& response);

    /**
     * @brief defaultProcess
     * @param root
     * @param response
     * @return
     */
    bool defaultProcess(const Json::Value& root, Json::Value& response);

    /**
     * @brief enterP2PMode
     * @param root
     * @param response
     * @return
     */
    bool enterP2PMode(const Json::Value& root, Json::Value& response);

	    /**
     * @brief getConfigurationTDM
     * @param root
     * @param response
     * @return
     */
    bool getConfigurationTDM(const Json::Value& root, Json::Value& response);

     /**
     * @brief getConfigurationPower
     * @param root
     * @param response
     * @return
     */
    bool getConfigurationPower(const Json::Value& root, Json::Value& response);

    /**
    * @brief getConfigurationModem
    * @param root
    * @param response
    * @return
    */
    bool getConfigurationModem(const Json::Value& root, Json::Value& response);

    /**
    * @brief getRadioLinkParams
    * @param root
    * @param response
    * @return
    */
    bool getRadioLinkParams(const Json::Value& root, Json::Value& response);

signals:
    void sigCUState(QByteArray);
    void sigSessionState(QByteArray);
    void sigRadioLinkState(QByteArray);
    void sigP2PModeParam(QByteArray);
    void sigEnterP2PMode(bool flag);
    void sigTDMConfig(QByteArray);
    void sigPowerConfig(QByteArray);
    void sigRadioLinkParams(QByteArray);
    void sigModemConfig(QByteArray);
};

#endif // !RECV_RESPONSE_RPC_H
