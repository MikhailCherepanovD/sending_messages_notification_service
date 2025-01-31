#include "NotificationServiceRequester.h"

NotificationServiceRequester* NotificationServiceRequester::instance=nullptr;
const string NotificationServiceRequester::NOTIFICATION_SERVER_HOST="http://localhost:8083";

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
    cout<<routeInfo->toString()<<endl;
}