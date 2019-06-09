#-------------------------------------------------
#
# Project created by QtCreator 2019-01-24T20:01:04
#
#-------------------------------------------------

QT       += core network

#QT       -= gui
#QMAKE_CXXFLAGS += -std=c++0x
TARGET = app6q
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    threadspi.cpp \
    objbase.cpp \
    objoled.cpp \
    objfb.cpp \
    threadkey.cpp \
    objpage.cpp \
    objui.cpp \
    keytransition.cpp \
    kt0000.cpp \
    ketmenu00.cpp \
    jsoncpp/json_reader.cpp \
    jsoncpp/json_value.cpp \
    jsoncpp/json_valueiterator.inl \
    jsoncpp/json_writer.cpp \
    jsonrpc/cu_rpc_control.cpp \
    jsonrpc/jsonrpc_handler.cpp \
    jsonrpc/message_handle.cpp \
    jsonrpc/recv_notify_rpc.cpp \
    jsonrpc/recv_response_rpc.cpp \
    jsonrpc/tcp_client.cpp \
    myqt.cpp \
    numeditor.cpp \
    objpara.cpp \
    objstatus.cpp \
    myq32.cpp \
    editorcallid.cpp

HEADERS += \
    threadspi.h \
    objbase.h \
    objoled.h \
    objfb.h \
    threadkey.h \
    objpage.h \
    objui.h \
    keytransition.h \
    kt0000.h \
    ketmenu00.h \
    json/allocator.h \
    json/assertions.h \
    json/autolink.h \
    json/config.h \
    json/features.h \
    json/forwards.h \
    json/json.h \
    json/reader.h \
    json/value.h \
    json/version.h \
    json/writer.h \
    jsoncpp/json_tool.h \
    jsoncpp/version.h.in \
    jsonrpc/cu_rpc_control.h \
    jsonrpc/data_struct.h \
    jsonrpc/jsonrpc_common.h \
    jsonrpc/jsonrpc_handler.h \
    jsonrpc/message_handle.h \
    jsonrpc/recv_notify_rpc.h \
    jsonrpc/recv_response_rpc.h \
    jsonrpc/tcp_client.h \
    myqt.h \
    numeditor.h \
    objpara.h \
    objstatus.h \
    myq32.h \
    editorcallid.h
