#include "NotificationServiceRequester.h"

NotificationServiceRequester* NotificationServiceRequester::instance=nullptr;
const string NotificationServiceRequester::NOTIFICATION_SERVER_HOST="http://localhost:8083";


RdKafka::Producer* NotificationServiceRequester::createProducer(){
    std::string brokers = "localhost:9092";
    std::string errstr;
    RdKafka::Conf *conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL); // Объект конфигурации kafka
    if (conf->set("bootstrap.servers", brokers, errstr) != RdKafka::Conf::CONF_OK) { // добавляем параметры конфигурации
        LOG_ERROR << "Error creating kafka configuration: " << errstr;
        delete conf;
        return nullptr;
    }

    // Создание Producer
    RdKafka::Producer *producer = RdKafka::Producer::create(conf, errstr); // создаем продюсера
    if (!producer) {
        LOG_ERROR <<"Error creating kafka producer: " << errstr;
        delete conf;
        return nullptr;
    }
    delete conf; // Конфигурация больше не нужна
    return producer;
}
void NotificationServiceRequester::requestAndSendDataTicket(shared_ptr<RouteInfo> routeInfo){
    auto httpRequest = HttpRequest::newHttpRequest();
    httpRequest->setPath(fmt::format("/users/{}/routes/{}/current",routeInfo->usersId,routeInfo->routeId));
    httpRequest->setMethod(Get);
    HttpClient::newHttpClient(NOTIFICATION_SERVER_HOST)->
        sendRequest(httpRequest,[this,routeInfo](ReqResult result, const HttpResponsePtr &response){
        if(result==ReqResult::Ok){
            LOG_INFO<<"Correct get ticket data from NOTIFICATION_SERVER_HOST";
            routeInfo->ticketData = response->getJsonObject()->toStyledString();
            sendMessage(routeInfo);
        }else{
            LOG_ERROR<<"Error request ticket data from NOTIFICATION_SERVER_HOST";
        }
    });
}
void NotificationServiceRequester::sendMessage(shared_ptr<RouteInfo> routeInfo){
    lock_guard<mutex> lock(mtxSendingMessage);
    RdKafka::ErrorCode resp = producer->produce(
            topic,
            RdKafka::Topic::PARTITION_UA, // Автоматический выбор партиции
            RdKafka::Producer::RK_MSG_COPY, //копирование сообщения (Kafka сама выделит память)
            routeInfo->toString().data(), routeInfo->toString().size(),
            nullptr, 0, 0, nullptr);
    if (resp != RdKafka::ERR_NO_ERROR) {
        std::cerr << "Error sending message kafka: " << RdKafka::err2str(resp) << std::endl;
    }
    cout<<routeInfo->toJsonString()<<endl;
}