#include "recv_response_rpc.h"
#include <iostream>

RecvResponseRpc::RecvResponseRpc()
{
}


RecvResponseRpc::~RecvResponseRpc()
{
}

// CU状态的响应处理
bool RecvResponseRpc::getCUState(const Json::Value& root, Json::Value& response)
{
	//std::cout << "Receive response: " << root << std::endl;
	std::cout << std::endl << "getCUState response: " << std::endl;
	response["jsonrpc"] = "2.0";

    if(root.isMember("error"))
    {
        std::cout << root["id"].asString() << ": " << root["error"]["message"].asString() << std::endl;
        response["result"] = "failed";
        return false;
    }

	if (root.isMember("result")) // 查询状态成功
	{
		response["result"] = "success";

		CUState cu_state;
		cu_state.mode = root["result"].isMember("mode") ? root["result"]["mode"].asString() : "";
		cu_state.state = root["result"].isMember("state") ? root["result"]["state"].asString() : "";
		cu_state.isdn = root["result"].isMember("isdn") ? root["result"]["isdn"].asString() : "";
		cu_state.name = root["result"].isMember("name") ? root["result"]["name"].asString() : "";
        cu_state.usageState = root["result"].isMember("usageState") ? root["result"]["usageState"].asString() : "";

        QByteArray ba;
        ba.append((char *)(&cu_state), sizeof(CUState));
        emit sigCUState(ba);

		// 控制台输出
		std::cout << "cu mode: " << cu_state.mode << std::endl;
		std::cout << "cu state: " << cu_state.state << std::endl;
		std::cout << "cu isdn: " << cu_state.isdn << std::endl;
		std::cout << "cu name: " << cu_state.name << std::endl;
        std::cout << "cu usageState: " << cu_state.usageState << std::endl;
	}
	else
	{
		response["result"] = "failed";
		return false;
	}

	return true;
}

// 会话状态的响应处理
bool RecvResponseRpc::getSessionState(const Json::Value& root, Json::Value& response)
{
	//std::cout << "Receive response: " << root << std::endl;
	std::cout << std::endl << "getSessionState response: " << std::endl;
	response["jsonrpc"] = "2.0";

    if(root.isMember("error"))
    {
        std::cout << root["id"].asString() << ": " << root["error"]["message"].asString() << std::endl;
        response["result"] = "failed";
        return false;
    }

	if (root.isMember("result")) // 查询状态成功
	{
		response["result"] = "success";

		Session session;
		session.JsonToSession(root["result"]);

        QByteArray ba;
        ba.append((char *)(&session), sizeof(Session));
        emit sigSessionState(ba);

		// 控制台输出
		std::cout << "session type: " << session.sessionType << std::endl;
		std::cout << "session state: " << session.state << std::endl;
		std::cout << "callee ISDN: " << session.calleeISDN << std::endl;
		std::cout << "send data rate: " << session.sendDataRate << std::endl;
		std::cout << "recv data date: " << session.recvDataRate << std::endl;
		std::cout << "data communicate mode: " << session.dataCommMode << std::endl;
	}
	else
	{
		response["result"] = "failed";
		return false;
	}

	return true;
}

// 无线链路响应处理
bool RecvResponseRpc::getRadioLinkState(const Json::Value& root, Json::Value& response)
{
//    std::cout << "Receive notify: " << root << std::endl;
	std::cout << std::endl << "getRadioLinkState response: " << std::endl;
	response["jsonrpc"] = "2.0";
	response["result"] = "success";

    if(root.isMember("error"))
    {
        std::cout << root["id"].asString() << ": " << root["error"]["message"].asString() << std::endl;
        response["result"] = "failed";
        return false;
    }

    if (!root.isMember("result"))
	{
		response["result"] = "failed";
		return false;
	}

	Json::Value alohaLink = root["result"].isMember("alohaLink") ? root["result"]["alohaLink"] : Json::Value::null;
	Json::Value dataRecvLink = root["result"].isMember("dataRecvLink") ? root["result"]["dataRecvLink"] : Json::Value::null;
	Json::Value dataSendLink = root["result"].isMember("dataSendLink") ? root["result"]["dataSendLink"] : Json::Value::null;
	Json::Value tdmLink = root["result"].isMember("tdmLink") ? root["result"]["tdmLink"] : Json::Value::null;

	RadioLinkStateChanged radio_link_state;

	if (Json::Value::null != alohaLink)
		radio_link_state.JsonToRadioLinkState(radio_link_state.alohaLink, alohaLink);

	if (Json::Value::null != dataRecvLink)
		radio_link_state.JsonToRadioLinkState(radio_link_state.dataRecvLink, dataRecvLink);

	if (Json::Value::null != dataSendLink)
		radio_link_state.JsonToRadioLinkState(radio_link_state.dataSendLink, dataSendLink);

	if (Json::Value::null != tdmLink)
		radio_link_state.JsonToRadioLinkState(radio_link_state.tdmLink, tdmLink);

    QByteArray ba;
    ba.append((char *)(&radio_link_state), sizeof(RadioLinkStateChanged));
    emit sigRadioLinkState(ba);

    // 控制台输出
    std::cout << "alohaLink: frequency = " << radio_link_state.alohaLink.frequency
        << " alohaLink: datarate = " << radio_link_state.alohaLink.datarate << " if_attenuation = "
        << radio_link_state.alohaLink.if_attenuation << " if_power = " << radio_link_state.alohaLink.if_power
        << " snr = " << radio_link_state.alohaLink.snr << " pduCount = " << radio_link_state.alohaLink.pduCount << " byteCount = "
        << radio_link_state.alohaLink.byteCount << " errorCount = " << radio_link_state.alohaLink.errorCount << std::endl;

    std::cout << "dataRecvLink: frequency = " << radio_link_state.dataRecvLink.frequency
        << " dataRecvLink: datarate = " << radio_link_state.dataRecvLink.datarate <<  " if_attenuation = "
        << radio_link_state.dataRecvLink.if_attenuation << " if_power = "	<< radio_link_state.dataRecvLink.if_power
        << " snr = " << radio_link_state.dataRecvLink.snr << " pduCount = " << radio_link_state.dataRecvLink.pduCount << " byteCount = "
        << radio_link_state.dataRecvLink.byteCount << " errorCount = " << radio_link_state.dataRecvLink.errorCount << std::endl;

    std::cout << "dataSendLink: frequency = " << radio_link_state.dataSendLink.frequency
        << " dataSendLink: datarate = " << radio_link_state.dataSendLink.datarate << " if_attenuation = "
        << radio_link_state.dataSendLink.if_attenuation << " if_power = "	<< radio_link_state.dataSendLink.if_power
        << " snr = " << radio_link_state.dataSendLink.snr << " pduCount = " << radio_link_state.dataSendLink.pduCount << " byteCount = "
        << radio_link_state.dataSendLink.byteCount << " errorCount = " << radio_link_state.dataSendLink.errorCount << std::endl;

    std::cout << "tdmLink: frequency = " << radio_link_state.tdmLink.frequency
        << " tdmLink: datarate = " << radio_link_state.tdmLink.datarate << " if_attenuation = "
        << radio_link_state.tdmLink.if_attenuation << " if_power = " << radio_link_state.tdmLink.if_power
        << " snr = " << radio_link_state.tdmLink.snr << " pduCount = " << radio_link_state.tdmLink.pduCount << " byteCount = "
        << radio_link_state.tdmLink.byteCount << " errorCount = " << radio_link_state.tdmLink.errorCount << std::endl;

	return true;
}

// 点对点参数响应处理
bool RecvResponseRpc::getP2PModeParam(const Json::Value& root, Json::Value& response)
{
//    std::cout << "Receive response: " << root << std::endl;
	std::cout << std::endl << "getP2PModeParam response: " << std::endl;
	response["jsonrpc"] = "2.0";

    if(root.isMember("error"))
    {
        std::cout << root["id"].asString() << ": " << root["error"]["message"].asString() << std::endl;
        response["result"] = "failed";
        return false;
    }

	if (root.isMember("result")) // 查询状态成功
	{
		response["result"] = "success";

		P2PMode p2p_mode;
		p2p_mode.JsonToP2PMode(root["result"]);

        QByteArray ba;
        ba.append((char *)(&p2p_mode), sizeof(P2PMode));
        emit sigP2PModeParam(ba);

		// 控制台输出
		std::cout << "tx freq: " << p2p_mode.txFrequence << std::endl;
		std::cout << "tx bitrate: " << p2p_mode.txBitrate << std::endl;
		std::cout << "tx if_power: " << p2p_mode.txIFPower << std::endl;
		std::cout << "rx freq: " << p2p_mode.rxFrequence << std::endl;
		std::cout << "rx bitrate: " << p2p_mode.rxBitrate << std::endl;
		std::cout << "data communicate mode: " << p2p_mode.dataCommMode << std::endl;
	}
	else
	{
		response["result"] = "failed";
		return false;
	}

    return true;
}

bool RecvResponseRpc::defaultProcess(const Json::Value &root, Json::Value &response)
{
    std::cout << std::endl << "defaultProcess response: " << std::endl;
    response["jsonrpc"] = "2.0";

    if(root.isMember("error"))
        std::cout << root["id"].asString() << ": " << root["error"]["message"].asString() << std::endl;
    else
        std::cout << root << std::endl;

    return true;
}

bool RecvResponseRpc::enterP2PMode(const Json::Value &root, Json::Value &response)
{
    std::cout << std::endl << "enterP2PMode response: " << std::endl;
    response["jsonrpc"] = "2.0";

    if(root.isMember("error"))
    {
        std::cout << root["id"].asString() << ": " << root["error"]["message"].asString() << std::endl;
        response["result"] = "failed";
        emit sigEnterP2PMode(false);
        return false;
    }

    std::cout << "Enter P2P mode successful!" << std::endl;
    emit sigEnterP2PMode(true);

    return true;
}

bool RecvResponseRpc::getConfigurationTDM(const Json::Value &root, Json::Value &response)
{
    std::cout << std::endl << "getConfigurationTDM response: " << std::endl;

    response["jsonrpc"] = "2.0";

    if(root.isMember("error"))
    {
        std::cout << root["id"].asString() << ": " << root["error"]["message"].asString() << std::endl;
        response["result"] = "failed";
        return false;
    }

    if (root.isMember("result")) // 查询状态成功
    {
        response["result"] = "success";

        TDMStruct tdm_struct;
        tdm_struct.JsonToP2PMode(root["result"]);

        QByteArray ba;
        ba.append((char *)(&tdm_struct), sizeof(TDMStruct));
        emit sigTDMConfig(ba);

        // 控制台输出
        std::cout << "version: " << tdm_struct.version << std::endl;
        std::cout << "imei: " << tdm_struct.imei << std::endl;
        for(unsigned int i = 0; i < tdm_struct.initTDMFrequency.size(); ++i)
        {
            std::cout << "TDM_" << i << ": " << tdm_struct.initTDMFrequency[i] << std::endl;
        }
    }
    else
    {
        response["result"] = "failed";
        return false;
    }


    return true;
}

bool RecvResponseRpc::getConfigurationPower(const Json::Value &root, Json::Value &response)
{
    std::cout << std::endl << "getConfigurationPower response: " << std::endl;

    response["jsonrpc"] = "2.0";

    if(root.isMember("error"))
    {
        std::cout << root["id"].asString() << ": " << root["error"]["message"].asString() << std::endl;
        response["result"] = "failed";
        return false;
    }

    if (root.isMember("result")) // 查询状态成功
    {
        response["result"] = "success";

        PowerStruct power_struct;
        power_struct.JsonToP2PMode(root["result"]);

        QByteArray ba;
        ba.append((char *)(&power_struct), sizeof(PowerStruct));
        emit sigPowerConfig(ba);

        // 控制台输出
        std::cout << "bucPower: " << power_struct.bucPower << std::endl;
        std::cout << "bucGain: " << power_struct.bucGain << std::endl;
        std::cout << "antennaGain: " << power_struct.antennaGain << std::endl;
        std::cout << "antennaGT: " << power_struct.antennaGT << std::endl;
        std::cout << "powerAdjust: " << power_struct.powerAdjust << std::endl;
    }
    else
    {
        response["result"] = "failed";
        return false;
    }


    return true;
}

bool RecvResponseRpc::getRadioLinkParams(const Json::Value &root, Json::Value &response)
{
        std::cout << "Receive getRadioLinkParams response: " << root << std::endl;
        std::cout << std::endl << "getRadioLinkState response: " << std::endl;
        response["jsonrpc"] = "2.0";
        response["result"] = "success";

        if(root.isMember("error"))
        {
            std::cout << root["id"].asString() << ": " << root["error"]["message"].asString() << std::endl;
            response["result"] = "failed";
            return false;
        }

        if (!root.isMember("result"))
        {
            response["result"] = "failed";
            return false;
        }

        Json::Value alohaLink = root["result"].isMember("alohaLink") ? root["result"]["alohaLink"] : Json::Value::null;
        Json::Value dataRecvLink = root["result"].isMember("dataRecvLink") ? root["result"]["dataRecvLink"] : Json::Value::null;
        Json::Value dataSendLink = root["result"].isMember("dataSendLink") ? root["result"]["dataSendLink"] : Json::Value::null;
        Json::Value tdmLink = root["result"].isMember("tdmLink") ? root["result"]["tdmLink"] : Json::Value::null;

        RadioLinkParamsChanged radio_link_params;

        if (Json::Value::null != alohaLink)
            radio_link_params.JsonToRadioLinkParams(radio_link_params.alohaLink, alohaLink);

        if (Json::Value::null != dataRecvLink)
            radio_link_params.JsonToRadioLinkParams(radio_link_params.dataRecvLink, dataRecvLink);

        if (Json::Value::null != dataSendLink)
            radio_link_params.JsonToRadioLinkParams(radio_link_params.dataSendLink, dataSendLink);

        if (Json::Value::null != tdmLink)
            radio_link_params.JsonToRadioLinkParams(radio_link_params.tdmLink, tdmLink);

        QByteArray ba;
        ba.append((char *)(&radio_link_params), sizeof(RadioLinkParamsChanged));
        emit sigRadioLinkParams(ba);

        // 控制台输出
        std::cout << "alohaLink: i_dc_bias = " << radio_link_params.alohaLink.i_dc_bias << " q_dc_bias = "
            << radio_link_params.alohaLink.q_dc_bias << " frequency_offset = " << radio_link_params.alohaLink.frequency_offset
            << " lo_frequency = " << radio_link_params.alohaLink.lo_frequency << std::endl;

        std::cout << "dataRecvLink: i_dc_bias = " << radio_link_params.dataRecvLink.i_dc_bias << " q_dc_bias = "
            << radio_link_params.dataRecvLink.q_dc_bias << " frequency_offset = " << radio_link_params.dataRecvLink.frequency_offset
            << " lo_frequency = " << radio_link_params.dataRecvLink.lo_frequency << std::endl;

        std::cout << "dataSendLink: i_dc_bias = " << radio_link_params.dataSendLink.i_dc_bias << " q_dc_bias = "
            << radio_link_params.dataSendLink.q_dc_bias << " frequency_offset = " << radio_link_params.dataSendLink.frequency_offset
            << " lo_frequency = " << radio_link_params.dataSendLink.lo_frequency << std::endl;

        std::cout << "tdmLink: i_dc_bias = " << radio_link_params.tdmLink.i_dc_bias << " q_dc_bias = "
            << radio_link_params.tdmLink.q_dc_bias << " frequency_offset = " << radio_link_params.tdmLink.frequency_offset
            << " lo_frequency = " << radio_link_params.tdmLink.lo_frequency << std::endl;

        return true;
}
