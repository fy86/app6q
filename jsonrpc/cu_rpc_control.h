//////////////////////////////////////////////////////             
///                                                                            
///  @file     cu_rpc_control.h                                                       
///  @brief    面板与CU之间的操作类                                                                              
///  @author   wly                                                                                                        
///  @version  1.0.0.1                                                
///  @date     2019.01.16                                                                                
///                                                                          
//////////////////////////////////////////////////////		

#ifndef CU_RPC_CONTROL_H
#define CU_RPC_CONTROL_H

#include <QObject>

#include <string.h>
#include <json/json.h>
#include "jsonrpc_handler.h"
#include "recv_notify_rpc.h"
#include "recv_response_rpc.h"
#include "data_struct.h"

class CuRpcControl : public QObject
{
    Q_OBJECT
public:
	CuRpcControl();
	~CuRpcControl();

	/**
	*  @brief                  获取CU状态
	*  @param[in] 	           void
	*  @return			       查询内容
	*/
	std::string getCUState();

	/**
	*  @brief                  入网
	*  @param[in] 	           void
	*  @return			       入网内容
	*/
	std::string loginNet();

	/**
	*  @brief                  退网
	*  @param[in] 	           void
	*  @return			       退网内容
	*/
	std::string logoutNet();

	/**
	*  @brief                  增加缓冲区数据
	*  @param[in] 	data_str   增加的数据 
	*  @return			       void
	*/
	void recv(std::string data_str);

	/**
	*  @brief                  重置操作
	*  @param[in] 	           void
	*  @return			       void
	*/
	void reset();

	/**
	*  @brief                  获取会话状态
	*  @param[in] 	           void
	*  @return			       查询内容
	*/
	std::string getSessionState();

	/**
	*  @brief                  获取无线链路状态
	*  @param[in] 	           void
	*  @return			       查询内容
	*/
	std::string getRadioLinkState();

	/**
	*  @brief                  创建会话
	*  @param[in] 	session    会话参数
	*  @return			       创建会话命令内容
	*/
	std::string createSession(Session &session);

	/**
	*  @brief                  结束会话
	*  @param[in] 	session    会话参数
	*  @return			       结束会话命令内容
	*/
	std::string closeSession();

	/**
	*  @brief                  进入点对点通信模式
	*  @param[in] 	P2PMode    点对点参数
	*  @return			       进入点对点命令内容
	*/
	std::string enterP2PMode(P2PMode &p2p_mode);

	/**
	*  @brief                  查询点对点工作参数
	*  @param[in] 	           void
	*  @return			       点对点参数查询命令内容
	*/
	std::string getP2PModeParam();

	/**
	*  @brief                  退出点对点工作模式
	*  @param[in] 	           void
	*  @return			       退出点对点命令内容
	*/
	std::string exitP2PMode();

    /**
    *  @brief                  获取终端配置参数
    *  @param[in] 	type_str   终端参数类型：'tdm' or 'power' or 'modem'
    *  @return			       获取终端配置参数命令内容
    */
    std::string getConfiguration(std::string type_str);

    /**
    *  @brief                  设置TDM相关参数
    *  @param[in] 	tdm_struct 要设置的参数
    *  @return			       设置命令内容
    */
    std::string setConfigurationTDM(TDMStruct &tdm_struct);

    /**
    *  @brief                      设置功率相关参数
    *  @param[in] 	power_struct   要设置的参数
    *  @return			           设置命令内容
    */
    std::string setConfigurationPower(PowerStruct &power_struct);

    /**
    *  @brief                      设置buc/lnb的10M相关参数
    *  @param[in] 	modem_struct   要设置的参数
    *  @return			           设置命令内容
    */
    std::string setConfigurationModem(ModemOutputStruct &modem_struct);

    /**
    *  @brief                      获取信道板参数
    *  @param[in] 	   			   void
    *  @return			           命令内容
    */
    std::string getRadioLinkParams();

    /**
    *  @brief                        设置信道板参数
    *  @param[in] radio_link_params  要设置的参数值
    *  @return			             命令内容
    */
    std::string setRadioLinkParams(RadioLinkParamsChanged &radio_link_params);

	/**
    *  @brief                        发送测试信号，退网模式下有效
    *  @param[in] test_signal_params 要设置的参数值
    *  @return			             命令内容
    */
    std::string startTestSignal(TestSignalStruct &test_signal_params);

	/**
    *  @brief                        停止发送测试信号
    *  @param[in] void               void
    *  @return			             命令内容
    */
    std::string stopTestSignal();

    RecvNotifyRPC *getRecvNotifyObject();

    RecvResponseRpc *getRecvResponseObject();
private:
	/**
	*  @brief                  解析json
	*  @param[in] 	           void
	*  @return			       void
	*/
	void parse();
private:
	/// json写对象json-->字符串
	std::unique_ptr<Json::StreamWriter> m_writer;

	std::string m_json_buf;              ///< 存放收到的json数据
	Json::Value m_notify_response;       ///< 处理通知时的响应
	Json::Rpc::Handler m_json_handler;   ///< rpc相关操作类
	RecvNotifyRPC m_recv_notify_rpc;     ///< 通知函数类
	RecvResponseRpc m_recv_response_rpc; ///< 响应函数类
};

#endif // !CU_RPC_CONTROL_H


