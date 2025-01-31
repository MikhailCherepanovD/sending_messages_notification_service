#include "DbChecker.h"
DbChecker* DbChecker::instance=nullptr;
string DbChecker::getVecStr(const vector<int>& vec){
    string ret;
    for(auto e:vec){
        ret+= to_string(e);
        ret+=" ";
    }
    return ret;
}


string DbChecker::constructQueryStr(const vector<int>&vecUpdatingId){
    string queryString = "SELECT users_id,email,telegram,user_name,route_monitoring_id FROM notification_sending_view WHERE  ";
    for(auto id:vecUpdatingId){
        queryString+= fmt::format("route_monitoring_id = {} OR ",id);
    }
    queryString.pop_back();
    queryString.pop_back();
    queryString.pop_back();
    LOG_INFO<<"query_str: "<<queryString;
    return queryString;
}
vector<shared_ptr<RouteInfo>> DbChecker::getInfoUpdatingRoutes() {
    auto f1 = dbClient->execSqlAsyncFuture("SELECT route_id FROM table_route_checking WHERE time_of_next_checking<CURRENT_TIMESTAMP;");
    vector<int> vecUpdatingId;
    try{
        auto result = f1.get();
        for(auto& row:result){
            vecUpdatingId.push_back(row["route_id"].as<int>());
        }
        LOG_INFO<<" values for updating: "<< getVecStr(vecUpdatingId);
    }catch(...){
        LOG_WARN<<" Unknown DB error getting values updating";
    }
    if(vecUpdatingId.empty()){
        return {};
    }
    auto fNeverUsed = async(launch::async,[&](){
        for(auto route_id: vecUpdatingId){
            try {
                dbClient->execSqlSync(R"(call update_time_of_next_checking($1);)", route_id);
            }catch(...){
                LOG_WARN<<" Unknown DB error calling function";
            }
        }
    });

    string queryString = constructQueryStr(vecUpdatingId);
    vector<shared_ptr<RouteInfo>> vecPtrReturnedInfo;
    vecPtrReturnedInfo.reserve(vecUpdatingId.size());
    auto f2 = dbClient->execSqlAsyncFuture(queryString);
    try{
        auto result = f2.get();
        for(auto& row:result){
            auto routeInfoPtr = make_shared<RouteInfo>();;
            routeInfoPtr->usersId = row["users_id"].as<int>();
            routeInfoPtr->email = row["email"].as<string>();
            routeInfoPtr->telegram = row["telegram"].as<string>();
            routeInfoPtr->userName = row["user_name"].as<string>();
            routeInfoPtr->routeId = row["route_monitoring_id"].as<int>();
            vecPtrReturnedInfo.push_back(routeInfoPtr);
        }
        LOG_INFO<<"Correct read info from view";
    }
    catch(...){
        LOG_WARN<<" Unknown DB error reading view";
    }
    return vecPtrReturnedInfo;
}
