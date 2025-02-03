#pragma once
#include <iostream>
#include <string>
#include "limits"
#include <fmt/core.h>
#include <drogon/HttpClient.h>
#include <drogon/HttpController.h>
#include "RouteInfo.h"
#include "inner_libraries/Singleton.h"
using namespace std;
using namespace drogon;
using namespace drogon::orm;
class DbChecker:public Singleton<DbChecker>{
private:
    shared_ptr<DbClient> dbClient;
    string constructQueryStr(const vector<int>&);
    string getVecStr(const vector<int>& vec);
public:
    DbChecker():dbClient{app().getDbClient("default")}{
        LOG_DEBUG<<"Create singleton object DbChecker";
    }
    ~DbChecker(){
        LOG_DEBUG<<"Delete singleton object DbChecker";
    }
    vector<shared_ptr<RouteInfo>> getInfoUpdatingRoutes();
};
