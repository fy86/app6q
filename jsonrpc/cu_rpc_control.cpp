#include "cu_rpc_control.h"

CuRpcControl::CuRpcControl()
{
    Json::StreamWriterBuilder buildwriter;
    buildwriter.settings_["indentation"] = ""; // 不使用列表格式
    m_writer = std::unique_ptr<Json::StreamWriter>(buildwriter.newStreamWriter());

	m_json_buf = "";

	// 添加通知方法
	m_json_handler.AddMethod(new Json::Rpc::RpcMethod<RecvNotifyRPC>(m_recv_notify_rpc,
		&RecvNotifyRPC::notifyRadioLinkStateChanged, std::string("notifyRadioLinkStateChanged")));

	m_json_handler.AddMethod(new Json::Rpc::RpcMethod<RecvNotifyRPC>(m_recv_notify_rpc,
		&RecvNotifyRPC::notifyCUStateChanged, std::string("notifyCUStateChanged")));

	m_json_handler.AddMethod(new Json::Rpc::RpcMethod<RecvNotifyRPC>(m_recv_notify_rpc,
		&RecvNotifyRPC::notifySessionStateChanged, std::string("notifySessionStateChanged")));

	// 添加响应方法
	m_json_handler.AddMethod(new Json::Rpc::RpcMethod<RecvResponseRpc>(m_recv_response_rpc,
		&RecvResponseRpc::getCUState, std::string("getCUState")));

	m_json_handler.AddMethod(new Json::Rpc::RpcMethod<RecvResponseRpc>(m_recv_response_rpc,
		&RecvResponseRpc::getRadioLinkState, std::string("getRadioLinkState")));

	m_json_handler.AddMethod(new Json::Rpc::RpcMethod<RecvResponseRpc>(m_recv_response_rpc,
		&RecvResponseRpc::getSessionState, std::string("getSessionState")));

	m_json_handler.AddMethod(new Json::Rpc::RpcMethod<RecvResponseRpc>(m_recv_response_rpc,
		&RecvResponseRpc::getP2PModeParam, std::string("getP2PModeParam")));

    m_json_handler.AddMethod(new Json::Rpc::RpcMethod<RecvResponseRpc>(m_recv_response_rpc,
        &RecvResponseRpc::defaultProcess, std::string("defaultProcess")));

    m_json_handler.AddMethod(new Json::Rpc::RpcMethod<RecvResponseRpc>(m_recv_response_rpc,
        &RecvResponseRpc::enterP2PMode, std::string("enterP2PMode")));

    m_json_handler.AddMethod(new Json::Rpc::RpcMethod<RecvResponseRpc>(m_recv_response_rpc,
        &RecvResponseRpc::getConfigurationTDM, std::string("getConfigurationTDM")));

    m_json_handler.AddMethod(new Json::Rpc::RpcMethod<RecvResponseRpc>(m_recv_response_rpc,
        &RecvResponseRpc::getConfigurationPower, std::string("getConfigurationPower")));

    m_json_handler.AddMethod(new Json::Rpc::RpcMethod<RecvResponseRpc>(m_recv_response_rpc,
        &RecvResponseRpc::getConfigurationModem, std::string("getConfigurationModem")));

    m_json_handler.AddMethod(new Json::Rpc::RpcMethod<RecvResponseRpc>(m_recv_response_rpc,
        &RecvResponseRpc::getRadioLinkParams, std::string("getRadioLinkParams")));
}

CuRpcControl::~CuRpcControl()
{
}

// 增加缓冲区数据
void CuRpcControl::recv(std::string data_str)
{
	m_json_buf += data_str;
	parse(); // 解析json数据
}

// 重置
void CuRpcControl::reset()
{
	m_json_buf = "";
}

// 解析json
void CuRpcControl::parse()
{
	int last_index = -1;  // 缓冲区处理结束的位置
	int start_index = -1;
	int end_index = -1;

	while (true)
	{
        if (last_index + 1 > (int)m_json_buf.length())
			break;
		start_index = m_json_buf.find_first_of(0x1E, last_index + 1);
		end_index = m_json_buf.find_first_of(0x0A, last_index + 1);
		while (end_index < start_index && -1 != end_index) // 如果结束位置在开始位置前面，继续查找结束位置
		{
			end_index = m_json_buf.find_first_of(0x0A, start_index);
		}

		if (-1 != start_index && -1 != end_index)
		{
			std::string json_str = m_json_buf.substr(start_index + 1, end_index - start_index - 1);
			last_index = end_index;

			// 处理收到的通知
			m_json_handler.Process(json_str, m_notify_response);
		}
		else // 没有找到一个完整的json串，跳出循环
		{
			break;
		}
	}

	if (-1 != last_index)
	{
		m_json_buf = m_json_buf.substr(last_index + 1);
	}
}

// 查询CU状态
std::string CuRpcControl::getCUState()
{
	/* build JSON-RPC CUState query */
	Json::Value query;
	query["jsonrpc"] = "2.0";
	query["id"] = "getCUState_id";
	query["method"] = "getCUState";
    std::ostringstream writer_os;
    m_writer->write(query, &writer_os);
    std::string request_str = writer_os.str();
	request_str = (char)0x1E + request_str + (char)0x0A;
	return request_str;
}

// 入网请求
std::string CuRpcControl::loginNet()
{
	/* build JSON-RPC loginNet query */
	Json::Value query;
	query["jsonrpc"] = "2.0";
	query["id"] = "loginNet_id";
	query["method"] = "loginNet";
    std::ostringstream writer_os;
    m_writer->write(query, &writer_os);
    std::string request_str = writer_os.str();
	request_str = (char)0x1E + request_str + (char)0x0A;
	return request_str;
}

// 退网请求
std::string CuRpcControl::logoutNet()
{
	/* build JSON-RPC logoutNet query */
	Json::Value query;
	query["jsonrpc"] = "2.0";
	query["id"] = "logoutNet_id";
	query["method"] = "logoutNet";
    std::ostringstream writer_os;
    m_writer->write(query, &writer_os);
    std::string request_str = writer_os.str();
	request_str = (char)0x1E + request_str + (char)0x0A;
	return request_str;
}

// 获取会话状态
std::string CuRpcControl::getSessionState()
{
	/* build JSON-RPC SessionState query */
	Json::Value query;
	query["jsonrpc"] = "2.0";
	query["id"] = "getSessionState_id";
	query["method"] = "getSessionState";
    std::ostringstream writer_os;
    m_writer->write(query, &writer_os);
    std::string request_str = writer_os.str();
	request_str = (char)0x1E + request_str + (char)0x0A;
	return request_str;
}

// 获取无线链路状态
std::string CuRpcControl::getRadioLinkState()
{
	/* build JSON-RPC RadioLinkState query */
	Json::Value query;
	query["jsonrpc"] = "2.0";
	query["id"] = "getRadioLinkState_id";
	query["method"] = "getRadioLinkState";
    std::ostringstream writer_os;
    m_writer->write(query, &writer_os);
    std::string request_str = writer_os.str();
	request_str = (char)0x1E + request_str + (char)0x0A;
	return request_str;
}

// 创建会话
std::string CuRpcControl::createSession(Session &session)
{
	/* build JSON-RPC create session */
	Json::Value query;
	query["jsonrpc"] = "2.0";
	query["id"] = "createSession_id";
	query["method"] = "createSession";
	query["params"]["peerISDN"] = session.calleeISDN;
	query["params"]["sendDataRate"] = session.sendDataRate;
	query["params"]["recvDataRate"] = session.recvDataRate;
	query["params"]["sessionType"] = session.sessionType;
	query["params"]["audioCodec"]["codec"] = session.audioCodec.codec;
	query["params"]["videoCodec"]["codec"] = session.videoCodec.codec;
	query["params"]["dataCommMode"] = session.dataCommMode;
    std::ostringstream writer_os;
    m_writer->write(query, &writer_os);
    std::string request_str = writer_os.str();
	request_str = (char)0x1E + request_str + (char)0x0A;
	return request_str;
}

// 结束会话
std::string CuRpcControl::closeSession()
{
	/* build JSON-RPC close session */
	Json::Value query;
	query["jsonrpc"] = "2.0";
	query["id"] = "closeSession_id";
	query["method"] = "closeSession";
    std::ostringstream writer_os;
    m_writer->write(query, &writer_os);
    std::string request_str = writer_os.str();
	request_str = (char)0x1E + request_str + (char)0x0A;
	return request_str;
}

// 进入点对点通信模式
std::string CuRpcControl::enterP2PMode(P2PMode &p2p_mode)
{
	/* build JSON-RPC enter P2PMode */
	Json::Value query;
	query["jsonrpc"] = "2.0";
	query["id"] = "enterP2PMode_id";
	query["method"] = "enterP2PMode";
	query["params"]["txFrequence"] = p2p_mode.txFrequence;
	query["params"]["txBitrate"] = p2p_mode.txBitrate;
	query["params"]["txIFPower"] = p2p_mode.txIFPower;
	query["params"]["rxFrequence"] = p2p_mode.rxFrequence;
	query["params"]["rxBitrate"] = p2p_mode.rxBitrate;
	query["params"]["dataCommMode"] = p2p_mode.dataCommMode;
    query["params"]["txModType"] = p2p_mode.txModType;
    query["params"]["txFECRate"] = p2p_mode.txFECRate;
    query["params"]["rxModType"] = p2p_mode.rxModType;
    query["params"]["rxFECRate"] = p2p_mode.rxFECRate;
    std::ostringstream writer_os;
    m_writer->write(query, &writer_os);
    std::string request_str = writer_os.str();
	request_str = (char)0x1E + request_str + (char)0x0A;
	return request_str;
}

// 点对点参数查询
std::string CuRpcControl::getP2PModeParam()
{
	/* build JSON-RPC getP2PModeParam query */
	Json::Value query;
	query["jsonrpc"] = "2.0";
	query["id"] = "getP2PModeParam_id";
	query["method"] = "getP2PModeParam";
    std::ostringstream writer_os;
    m_writer->write(query, &writer_os);
    std::string request_str = writer_os.str();
	request_str = (char)0x1E + request_str + (char)0x0A;
	return request_str;
}

// 退出点对点工作模式
std::string CuRpcControl::exitP2PMode()
{
	/* build JSON-RPC exit P2PMode */
	Json::Value query;
	query["jsonrpc"] = "2.0";
	query["id"] = "exitP2PMode_id";
	query["method"] = "exitP2PMode";
    std::ostringstream writer_os;
    m_writer->write(query, &writer_os);
    std::string request_str = writer_os.str();
	request_str = (char)0x1E + request_str + (char)0x0A;
    return request_str;
}

std::string CuRpcControl::getConfiguration(std::string type_str)
{
    /* build JSON-RPC getConfiguration*/
    Json::Value query;
    query["jsonrpc"] = "2.0";
    query["method"] = "getConfiguration";
    if("power" == type_str)
    {
        query["id"] = "getConfigurationPower_id";
        query["params"]["configGroup"] = "powerControl";
    }
    else if("tdm" == type_str)
    {

        query["id"] = "getConfigurationTDM_id";
        query["params"]["configGroup"] = "basic";
    }
    else
    {
        query["id"] = "getConfigurationModem_id";
        query["params"]["configGroup"] = "modemOutput";
    }
    std::ostringstream writer_os;
    m_writer->write(query, &writer_os);
    std::string request_str = writer_os.str();
    request_str = (char)0x1E + request_str + (char)0x0A;
    return request_str;
}

std::string CuRpcControl::setConfigurationTDM(TDMStruct &tdm_struct)
{
    /* build JSON-RPC setConfigurationTDM*/
    Json::Value query;
    query["jsonrpc"] = "2.0";
    query["method"] = "setConfiguration";
    query["id"] = "setConfigurationTDM_id";
    query["params"]["configGroup"] = "basic";
    if(!tdm_struct.imei.empty())
        query["params"]["configValue"]["imei"] = tdm_struct.imei;

    if(!tdm_struct.version.empty())
        query["params"]["configValue"]["version"] = tdm_struct.version;

    for(unsigned int i = 0; i < tdm_struct.initTDMFrequency.size(); ++i)
    {
        query["params"]["configValue"]["initTDMFrequency"][i] = tdm_struct.initTDMFrequency[i];
    }

    std::ostringstream writer_os;
    m_writer->write(query, &writer_os);
    std::string request_str = writer_os.str();
    request_str = (char)0x1E + request_str + (char)0x0A;
    return request_str;
}

std::string CuRpcControl::setConfigurationPower(PowerStruct &power_struct)
{
    /* build JSON-RPC setPowerConfiguration*/
    Json::Value query;
    query["jsonrpc"] = "2.0";
    query["method"] = "setConfiguration";
    query["id"] = "setConfigurationPower_id";
    query["params"]["configGroup"] = "powerControl";
    if(-90000 < power_struct.bucPower)
        query["params"]["configValue"]["bucPower"] = power_struct.bucPower;

    if(-99999.999 < power_struct.bucGain)
        query["params"]["configValue"]["bucGain"] = power_struct.bucGain;

    if(-99999.999 < power_struct.antennaGain)
        query["params"]["configValue"]["antennaGain"] = power_struct.antennaGain;

    if(-99999.999 < power_struct.antennaGT)
        query["params"]["configValue"]["antennaGT"] = power_struct.antennaGT;

    if(-99999.999 < power_struct.powerAdjust)
        query["params"]["configValue"]["powerAdjust"] = power_struct.powerAdjust;

    std::ostringstream writer_os;
    m_writer->write(query, &writer_os);
    std::string request_str = writer_os.str();
    request_str = (char)0x1E + request_str + (char)0x0A;
    return request_str;
}

std::string CuRpcControl::setConfigurationModem(ModemOutputStruct &modem_struct)
{
    /* build JSON-RPC setConfigurationModem*/
    Json::Value query;
    query["jsonrpc"] = "2.0";
    query["method"] = "setConfiguration";
    query["id"] = "setConfigurationModem_id";
    query["params"]["configGroup"] = "modemOutput";
    if(-1 != modem_struct.bucPowerSupply)
        query["params"]["configValue"]["bucPowerSupply"] = (1 == modem_struct.bucPowerSupply)?true:false;

    if(-1 != modem_struct.buc10MRef)
        query["params"]["configValue"]["buc10MRef"] = (1 == modem_struct.buc10MRef)?true:false;

    if(-1 != modem_struct.lnbPowerSupply)
        query["params"]["configValue"]["lnbPowerSupply"] = (1 == modem_struct.lnbPowerSupply)?true:false;

    if(-1 != modem_struct.lnb10MRef)
        query["params"]["configValue"]["lnb10MRef"] = (1 == modem_struct.lnb10MRef)?true:false;

    std::ostringstream writer_os;
    m_writer->write(query, &writer_os);
    std::string request_str = writer_os.str();
    request_str = (char)0x1E + request_str + (char)0x0A;
    return request_str;
}

std::string CuRpcControl::getRadioLinkParams()
{
    /* build JSON-RPC RadioLinkParams query */
    Json::Value query;
    query["jsonrpc"] = "2.0";
    query["id"] = "getRadioLinkParams_id";
    query["method"] = "getRadioLinkParams";
    std::ostringstream writer_os;
    m_writer->write(query, &writer_os);
    std::string request_str = writer_os.str();
    request_str = (char)0x1E + request_str + (char)0x0A;
    return request_str;
}

std::string CuRpcControl::setRadioLinkParams(RadioLinkParamsChanged &radio_link_params)
{
    /* build JSON-RPC setRadioLinkParams*/
    Json::Value query;
    query["jsonrpc"] = "2.0";
    query["method"] = "setRadioLinkParams";
    query["id"] = "setRadioLinkParams_id";
    if(-99999 < radio_link_params.tdmLink.lo_frequency)
        query["params"]["tdmLink"]["lo_frequency"] = radio_link_params.tdmLink.lo_frequency;

    if(-99999 < radio_link_params.alohaLink.lo_frequency)
        query["params"]["alohaLink"]["lo_frequency"] = radio_link_params.alohaLink.lo_frequency;

    if(-99999 < radio_link_params.dataSendLink.lo_frequency)
    {
        query["params"]["dataSendLink"]["lo_frequency"] = radio_link_params.dataSendLink.lo_frequency;
        query["params"]["alohaLink"]["lo_frequency"] = radio_link_params.dataSendLink.lo_frequency;
    }

    if(-99999 < radio_link_params.dataRecvLink.lo_frequency)
    {
        query["params"]["dataRecvLink"]["lo_frequency"] = radio_link_params.dataRecvLink.lo_frequency;
        query["params"]["tdmLink"]["lo_frequency"] = radio_link_params.dataRecvLink.lo_frequency;
    }

    std::ostringstream writer_os;
    m_writer->write(query, &writer_os);
    std::string request_str = writer_os.str();
    request_str = (char)0x1E + request_str + (char)0x0A;
    return request_str;
}

std::string CuRpcControl::stopTestSignal()
{
    /* build JSON-RPC stopTestSignal*/
    Json::Value query;
    query["jsonrpc"] = "2.0";
    query["method"] = "stopTestSignal";
    query["id"] = "stopTestSignal_id";

    std::ostringstream writer_os;
    m_writer->write(query, &writer_os);
    std::string request_str = writer_os.str();
    request_str = (char)0x1E + request_str + (char)0x0A;
    return request_str;
}

std::string CuRpcControl::startTestSignal(TestSignalStruct &test_signal_params)
{
    /* build JSON-RPC startTestSignal*/
    Json::Value query;
    query["jsonrpc"] = "2.0";
    query["method"] = "startTestSignal";
    query["id"] = "startTestSignal_id";
    if(-1 != test_signal_params.txFrequence)
        query["params"]["txFrequence"] = test_signal_params.txFrequence;

    if(-1 != test_signal_params.txIFPower)
        query["params"]["txIFPower"] = test_signal_params.txIFPower;

    if(-1 != test_signal_params.rxFrequency)
        query["params"]["rxFrequency"] = test_signal_params.rxFrequency;

    std::ostringstream writer_os;
    m_writer->write(query, &writer_os);
    std::string request_str = writer_os.str();
    request_str = (char)0x1E + request_str + (char)0x0A;
    return request_str;
}

RecvNotifyRPC *CuRpcControl::getRecvNotifyObject()
{
    return &m_recv_notify_rpc;
}

RecvResponseRpc *CuRpcControl::getRecvResponseObject()
{
    return &m_recv_response_rpc;
}
