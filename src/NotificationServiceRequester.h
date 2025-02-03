#pragma once
#include <iostream>
#include <string>
#include "limits"
#include <fmt/core.h>
#include <drogon/HttpClient.h>
#include <drogon/HttpController.h>
#include <librdkafka/rdkafkacpp.h>
#include "inner_libraries/Singleton.h"
#include "inner_libraries/ConfigReader.h"
#include "RouteInfo.h"
using namespace std;
using namespace drogon;
using namespace drogon::orm;

class NotificationServiceRequester:public Singleton<NotificationServiceRequester>{
private:
    ConfigReader& configReader;
    string NOTIFICATION_SERVER_HOST;
    mutex mtxSendingMessage;
    string topic;
    RdKafka::Producer* producer;
    RdKafka::Producer* createProducer();
    void sendMessage(shared_ptr<RouteInfo> routeInfo);
public:
    NotificationServiceRequester()
    :configReader(ConfigReader::getInstance()){
        LOG_DEBUG<<"Create singleton object NotificationServiceRequester";
        topic = (*configReader.getJsonValue())["kafka"]["topic"].asString();
        producer = createProducer();
        NOTIFICATION_SERVER_HOST = fmt::format("http://{}:{}",
                                               (*configReader.getJsonValue())["notification_server"]["host"].asString(),
                                               (*configReader.getJsonValue())["notification_server"]["port"].asInt());
    }
    ~NotificationServiceRequester(){
        LOG_DEBUG<<"Delete singleton object NotificationServiceRequester";
        delete producer;
    }
    void requestAndSendDataTicket(shared_ptr<RouteInfo> routeInfo);
};
