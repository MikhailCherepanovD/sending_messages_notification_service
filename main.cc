#include <drogon/drogon.h>
#include <drogon/HttpController.h>
#include <fmt/core.h>
#include "iostream"
#include "src/DbChecker.h"
#include "src/NotificationServiceRequester.h"
#include "inner_libraries/ConfigReader.h"
using namespace std;
using namespace drogon;
using namespace drogon::orm;
void doWork() {
    auto &dbChecker = DbChecker::getInstance();
    auto &notificationServiceRequester = NotificationServiceRequester::getInstance();
    vector<shared_ptr<RouteInfo>> vecRouteInfo =  dbChecker.getInfoUpdatingRoutes();
    if(vecRouteInfo.empty()) {
        return;
    }
    for(auto elem:vecRouteInfo){
        notificationServiceRequester.requestAndSendDataTicket(elem);
    }
}


int main() {
    ConfigReader& configReader = ConfigReader::getInstance();
    int interval_checking = (*configReader.getJsonValue())["interval_checking"].asInt();
    app().loadConfigFile("../config.json");
    trantor::EventLoop* loop = app().getLoop();
    function<void()> mainCircle;
    mainCircle = [loop,&mainCircle,interval_checking](){
        loop->queueInLoop(doWork);
        loop->runAfter(interval_checking, mainCircle);
    };
    loop->queueInLoop(mainCircle);
    app().run();
    return 0;
}
