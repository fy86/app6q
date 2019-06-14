/**
 * \file test-rpc.cpp
 * \brief Test RPC example.
 * \author Sebastien Vincent
 */

#include "recv_notify_rpc.h"

#include <iostream>

 // 无线链路状态变化通知
bool RecvNotifyRPC::notifyRadioLinkStateChanged(const Json::Value& root, Json::Value& response)
{
	//std::cout << "Receive notify: " << root << std::endl;
	std::cout << std::endl << "notifyRadioLinkStateChanged notify: " << std::endl;
	response["jsonrpc"] = "2.0";
	response["result"] = "success";

	if (!root.isMember("params"))
	{
		response["result"] = "failed";
		return false;
	}

	Json::Value alohaLink = root["params"].isMember("alohaLink") ? root["params"]["alohaLink"] : Json::Value::null;
	Json::Value dataRecvLink = root["params"].isMember("dataRecvLink") ? root["params"]["dataRecvLink"] : Json::Value::null;
	Json::Value dataSendLink = root["params"].isMember("dataSendLink") ? root["params"]["dataSendLink"] : Json::Value::null;
	Json::Value tdmLink = root["params"].isMember("tdmLink") ? root["params"]["tdmLink"] : Json::Value::null;

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
        << radio_link_state.dataRecvLink.byteCount << " errorCount = " << radio_link_state.dataRecvLink.errorCount << std::endl
        << " sync = " << radio_link_state.dataRecvLink.sync << std::endl;

    std::cout << "dataSendLink: frequency = " << radio_link_state.dataSendLink.frequency
        << " dataSendLink: datarate = " << radio_link_state.dataSendLink.datarate << " if_attenuation = "
		<< radio_link_state.dataSendLink.if_attenuation << " if_power = "	<< radio_link_state.dataSendLink.if_power
		<< " snr = " << radio_link_state.dataSendLink.snr << " pduCount = " << radio_link_state.dataSendLink.pduCount << " byteCount = " 
		<< radio_link_state.dataSendLink.byteCount << " errorCount = " << radio_link_state.dataSendLink.errorCount << std::endl;

    std::cout << "tdmLink: frequency = " << radio_link_state.tdmLink.frequency
        << " tdmLink: datarate = " << radio_link_state.tdmLink.datarate << " if_attenuation = "
		<< radio_link_state.tdmLink.if_attenuation << " if_power = " << radio_link_state.tdmLink.if_power
		<< " snr = " << radio_link_state.tdmLink.snr << " pduCount = " << radio_link_state.tdmLink.pduCount << " byteCount = " 
        << radio_link_state.tdmLink.byteCount << " errorCount = " << radio_link_state.tdmLink.errorCount << std::endl
        << " sync = " << radio_link_state.tdmLink.sync << std::endl;

	return true;
}

// CU状态变化通知
bool RecvNotifyRPC::notifyCUStateChanged(const Json::Value& root, Json::Value& response)
{
	std::cout << std::endl << "notifyCUStateChanged notify: " << std::endl;
	response["jsonrpc"] = "2.0";
	response["result"] = "success";

	if (!root.isMember("params"))
	{
		response["result"] = "failed";
		return false;
	}

	CUState cu_state;
	cu_state.mode = root["params"].isMember("mode") ? root["params"]["mode"].asString() : "";
	cu_state.state = root["params"].isMember("state") ? root["params"]["state"].asString() : "";
	cu_state.isdn = root["params"].isMember("isdn") ? root["params"]["isdn"].asString() : "";
	cu_state.name = root["params"].isMember("name") ? root["params"]["name"].asString() : "";
    cu_state.usageState = root["params"].isMember("usageState") ? root["params"]["usageState"].asString() : "";

    QByteArray ba;
    ba.append((char *)(&cu_state), sizeof(CUState));
    emit sigCUState(ba);

	//// 控制台输出
	std::cout << "cu mode: " << cu_state.mode << std::endl;
	std::cout << "cu state: " << cu_state.state << std::endl;
	std::cout << "cu isdn: " << cu_state.isdn << std::endl;
	std::cout << "cu name: " << cu_state.name << std::endl;
    std::cout << "cu usageState: " << cu_state.name << std::endl;

    return true;
}

// 会话状态变化通知
bool RecvNotifyRPC::notifySessionStateChanged(const Json::Value& root, Json::Value& response)
{
	std::cout << std::endl << "notifySessionStateChanged notify: " << std::endl;
	response["jsonrpc"] = "2.0";
	response["result"] = "success";

	if (!root.isMember("params"))
	{
		response["result"] = "failed";
		return false;
	}

	Session session;
	session.JsonToSession(root["params"]);

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

	return true;
}
