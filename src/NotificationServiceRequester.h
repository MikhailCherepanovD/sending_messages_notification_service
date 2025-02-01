#pragma once
#include <iostream>
#include <string>
#include "limits"
#include <fmt/core.h>
#include <drogon/HttpClient.h>
#include <drogon/HttpController.h>
#include <librdkafka/rdkafkacpp.h>
#include "RouteInfo.h"
using namespace std;
using namespace drogon;
using namespace drogon::orm;
class NotificationServiceRequester{
private:
    NotificationServiceRequester()    {
        LOG_DEBUG<<"Create singleton object NotificationServiceRequester";
        producer = createProducer();
    }
    ~NotificationServiceRequester(){
        //producer->flush(100); // Ожидание доставки сообщений
        LOG_DEBUG<<"Delete singleton object NotificationServiceRequester";
        delete producer;
    }
    static NotificationServiceRequester* instance;
    class MemGuard{
    public:
        MemGuard(){
        }
        ~MemGuard(){
            delete instance;
            instance=nullptr;
        }
    };
    static const string NOTIFICATION_SERVER_HOST;
    mutex mtxSendingMessage;
    string topic = "my_topic1";
    RdKafka::Producer* producer;
    static RdKafka::Producer* createProducer();
    void sendMessage(shared_ptr<RouteInfo> routeInfo);
public:
    NotificationServiceRequester(const NotificationServiceRequester&) = delete;
    NotificationServiceRequester& operator=(const NotificationServiceRequester&) = delete;
    static NotificationServiceRequester& getInstance(){
        static MemGuard g;
        if (!instance) {
            instance = new NotificationServiceRequester();
        }
        return *instance;
    }
    void requestAndSendDataTicket(shared_ptr<RouteInfo> routeInfo);
};
