#ifndef DATA_STRUCT_H
#define DATA_STRUCT_H
#include <iostream>
#include <string>
#include <vector>
#include "json/json.h"

/**
 * @brief CU状态结构
*/
struct CUState
{
    std::string mode;
    std::string state;
    std::string isdn;
    std::string name;
    std::string usageState;

    CUState()
    {
        mode = "";
        state = "";
        isdn = "";
        name = "";
        usageState = "";
    }
};

/**
 * @brief 链路状态，包括tdm、aloha、data send、data recv
*/
struct RadioLinkStateChanged
{
    struct RadioLinkState
    {
        int64_t frequency;            ///< 工作频点
        int64_t datarate;             ///< Data rate
        float if_attenuation;         ///< 中频衰减
        float if_power;               ///< 中频发送功率
        float snr;                    ///< 接收信噪比
        int64_t pduCount;             ///< 累计收/发数据包
        int64_t byteCount;            ///< 累计收/发字节数
        int64_t errorCount;           ///< 累计错误数

        RadioLinkState()
        {
            frequency = -1;
            datarate = -1;
            if_attenuation = -1;
            if_power = -1;
            snr = -1;
            pduCount = -1;
            byteCount = -1;
            errorCount = -1;
        }

    }tdmLink, alohaLink, dataSendLink, dataRecvLink;

    // 将链路参数转化成结构体
    void JsonToRadioLinkState(RadioLinkState &radio_state_struct, Json::Value const &json_value)
    {
        // 频点
        if (json_value.isMember("frequency") && json_value["frequency"].isInt64())
        {
            radio_state_struct.frequency = json_value["frequency"].asInt64();
        }

        // Data rate
        if (json_value.isMember("datarate") && json_value["datarate"].isInt64())
        {
            radio_state_struct.datarate = json_value["datarate"].asInt64();
        }

        // 中频衰减
        if (json_value.isMember("if_attenuation") && json_value["if_attenuation"].isDouble())
        {
            radio_state_struct.if_attenuation = json_value["if_attenuation"].asDouble();
        }

        // 中频发送功率
        if (json_value.isMember("if_power") && json_value["if_power"].isDouble())
        {
            radio_state_struct.if_power = json_value["if_power"].asDouble();
        }

        // 信噪比
        if (json_value.isMember("snr") && json_value["snr"].isDouble())
        {
            radio_state_struct.snr = json_value["snr"].asDouble();
        }

        // 累计收/发数据包
        if (json_value.isMember("pduCount") && json_value["pduCount"].isInt64())
        {
            radio_state_struct.pduCount = json_value["pduCount"].asInt64();
        }

        // 累计收/发字节数
        if (json_value.isMember("byteCount") && json_value["byteCount"].isInt64())
        {
            radio_state_struct.byteCount = json_value["byteCount"].asInt64();
        }

        // 累计错误数
        if (json_value.isMember("errorCount") && json_value["errorCount"].isInt64())
        {
            radio_state_struct.errorCount = json_value["errorCount"].asInt64();
        }
    }
};

/**
 * @brief 会话状态
*/
struct Session
{
    int64_t sessionType;        ///< 会话类型
    std::string state;          ///< 会话状态
    std::string calleeISDN;     ///< 被叫号码
    int64_t sendDataRate;       ///< 本终端发送业务信道速率（bps）
    int64_t recvDataRate;       ///< 本终端接收业务信道速率（bps）
    struct AudioCodec
    {
        std::string codec;      ///< 编码类型
    }audioCodec;                ///< 音频编码信息

    struct VideoCodec
    {
        std::string codec;      ///< 视频编码类型
    }videoCodec;                ///< 视频编码信息

    std::string dataCommMode;   ///< 通信模式(桥接、nat、路由)

    Session()
    {
        sessionType = -1;
        state = "";
        calleeISDN = "";
        sendDataRate = -1;
        recvDataRate = -1;
        audioCodec.codec = "";
        videoCodec.codec = "";
        dataCommMode = "";
    }

    // 将会话参数转化成结构体
    void JsonToSession(Json::Value const &json_value)
    {
        if (json_value.isMember("sessionType") && json_value["sessionType"].isInt64())
        {
            sessionType = json_value["sessionType"].asInt64();
        }

        if (json_value.isMember("state") && json_value["state"].isString())
        {
            state = json_value["state"].asString();
        }

        if (json_value.isMember("calleeISDN") && json_value["calleeISDN"].isString())
        {
            calleeISDN = json_value["calleeISDN"].asString();
        }

        if (json_value.isMember("sendDataRate") && json_value["sendDataRate"].isInt64())
        {
            sendDataRate = json_value["sendDataRate"].asInt64();
        }

        if (json_value.isMember("recvDataRate") && json_value["recvDataRate"].isInt64())
        {
            recvDataRate = json_value["recvDataRate"].asInt64();
        }

        if (json_value.isMember("dataCommMode") && json_value["dataCommMode"].isString())
        {
            dataCommMode = json_value["dataCommMode"].asString();
        }
    }
};

/**
 * @brief 点对点模式
*/
struct P2PMode
{
    int64_t txFrequence;       ///< 发射频频点（单位Hz）
    int64_t txBitrate;         ///< 发速率(单位bps)
    float txIFPower;           ///< 发中频功率（单位dBm）
    int64_t rxFrequence;       ///< 收射频频点（单位Hz）
    int64_t rxBitrate;         ///< 收速率(单位bps)
    std::string dataCommMode;  ///< 数据通信模式(default、bridge、nat、route)

    P2PMode()
    {
        txFrequence = -1;
        txBitrate = -1;
        txIFPower = -1;
        rxFrequence = -1;
        rxBitrate = -1;
        dataCommMode = "";
    }

    // 将点对点参数转化成结构体
    void JsonToP2PMode(Json::Value const &json_value)
    {
        if (json_value.isMember("txFrequence") && json_value["txFrequence"].isInt64())
        {
            txFrequence = json_value["txFrequence"].asInt64();
        }

        if (json_value.isMember("txBitrate") && json_value["txBitrate"].isInt64())
        {
            txBitrate = json_value["txBitrate"].asInt64();
        }

        if (json_value.isMember("txIFPower") && json_value["txIFPower"].isDouble())
        {
            txIFPower = json_value["txIFPower"].asDouble();
        }

        if (json_value.isMember("rxFrequence") && json_value["rxFrequence"].isInt64())
        {
            rxFrequence = json_value["rxFrequence"].asInt64();
        }

        if (json_value.isMember("rxBitrate") && json_value["rxBitrate"].isInt64())
        {
            rxBitrate = json_value["rxBitrate"].asInt64();
        }

        if (json_value.isMember("dataCommMode") && json_value["dataCommMode"].isString())
        {
            dataCommMode = json_value["dataCommMode"].asString();
        }
    }
};

/**
 * @brief TDM相关参数
*/
struct TDMStruct
{
    std::string version;                    ///< 终端软件版本号
    std::string imei;                       ///< 设备标识
    std::vector<int64_t> initTDMFrequency;   ///< TDM频点

    TDMStruct()
    {
        version = "";
        imei = "";
    }

    // 将TDM参数转化成结构体
    void JsonToP2PMode(Json::Value const &json_value)
    {
        if (json_value.isMember("version") && json_value["version"].isString())
        {
            version = json_value["version"].asString();
        }

        if (json_value.isMember("imei") && json_value["imei"].isString())
        {
            imei = json_value["imei"].asString();
        }

        if (json_value.isMember("initTDMFrequency") && json_value["initTDMFrequency"].isArray())
        {
            int sz = json_value["initTDMFrequency"].size();
            for(int i = 0; i < sz; ++i)
            {
                initTDMFrequency.push_back(json_value["initTDMFrequency"][i].asInt64());
            }
        }
    }
};

/**
 * @brief 功率相关参数
*/
struct PowerStruct
{
    float bucPower;              ///< BUC饱和发送功率，单位W
    float bucGain;               ///< BUC线性增益，单位dB
    float antennaGain;           ///< 天线增益(dBi)，单位dB
    float antennaGT;             ///< 天线G/T值

    /// 功率调节，单位dB,用户根据实际环境设置此参数
    /// 此值为正，则在理论功率基础上，增加发送功率
    float powerAdjust;

    PowerStruct()
    {
        bucPower = -99999.999;
        bucGain = -99999.999;
        antennaGain = -99999.999;
        antennaGT = -99999.999;
        powerAdjust = -99999.999;
    }

    // 将功率参数转化成结构体
    void JsonToP2PMode(Json::Value const &json_value)
    {
        if (json_value.isMember("bucPower") && json_value["bucPower"].isDouble())
        {
            bucPower = json_value["bucPower"].asDouble();
        }

        if (json_value.isMember("bucGain") && json_value["bucGain"].isDouble())
        {
            bucGain = json_value["bucGain"].asDouble();
        }

        if (json_value.isMember("antennaGain") && json_value["antennaGain"].isDouble())
        {
            antennaGain = json_value["antennaGain"].asDouble();
        }

        if (json_value.isMember("antennaGT") && json_value["antennaGT"].isDouble())
        {
            antennaGT = json_value["antennaGT"].asDouble();
        }

        if (json_value.isMember("powerAdjust") && json_value["powerAdjust"].isDouble())
        {
            powerAdjust = json_value["powerAdjust"].asDouble();
        }
    }
};

/**
 * @brief 信道板参数，包括tdm、aloha、data send、data recv、全局参数
*/
struct RadioLinkParamsChanged
{
    struct RadioLinkParams
    {
        int64_t i_dc_bias;            ///< I路直流偏置。取值范围:[-8192,8191]
        int64_t q_dc_bias;            ///< Q路直流偏置
        int64_t frequency_offset;     ///< 频偏(HZ)
        int64_t lo_frequency;         ///< BUC/LNB本震动频率(HZ)

        RadioLinkParams()
        {
            i_dc_bias = -99999;
            q_dc_bias = -99999;
            frequency_offset = -99999;
            lo_frequency = -99999;
        }

    }tdmLink, alohaLink, dataSendLink, dataRecvLink;

    struct GlobalLinkParam
    {
        int64_t agc1;
        int64_t agc2;

        GlobalLinkParam()
        {
            agc1 = -99999;
            agc2 = -99999;
        }

    }globalParam;

    // 将链路参数转化成结构体
    void JsonToRadioLinkParams(RadioLinkParams &radio_params_struct, Json::Value const &json_value)
    {
        // I路直流偏置
        if (json_value.isMember("i_dc_bias") && json_value["i_dc_bias"].isInt64())
        {
            radio_params_struct.i_dc_bias = json_value["i_dc_bias"].asInt64();
        }

        // Q路直流偏置
        if (json_value.isMember("q_dc_bias") && json_value["q_dc_bias"].isInt64())
        {
            radio_params_struct.q_dc_bias = json_value["q_dc_bias"].asInt64();
        }

        // 频偏
        if (json_value.isMember("frequency_offset") && json_value["frequency_offset"].isInt64())
        {
            radio_params_struct.frequency_offset = json_value["frequency_offset"].asInt64();
        }

        // lnb或buc
        if (json_value.isMember("lo_frequency") && json_value["lo_frequency"].isInt64())
        {
            radio_params_struct.lo_frequency = json_value["lo_frequency"].asInt64();
        }
    }
};

#endif // !DATA_STRUCT_H

