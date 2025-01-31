#pragma once
#include <iostream>
#include <string>
#include "limits"
#include <fmt/core.h>
#include <drogon/HttpClient.h>
#include <drogon/HttpController.h>
#include "RouteInfo.h"
using namespace std;
using namespace drogon;
using namespace drogon::orm;
class DbChecker{
private:
    shared_ptr<DbClient> dbClient;
    DbChecker():dbClient{app().getDbClient("default")}{
        LOG_DEBUG<<"Create singleton object DbChecker";
    }
    ~DbChecker(){
        LOG_DEBUG<<"Delete singleton object DbChecker";
    }
    static DbChecker* instance;
    class MemGuard{
    public:
        MemGuard(){
        }
        ~MemGuard(){
            delete instance;
            instance=nullptr;
        }
    };
    static string constructQueryStr(const vector<int>&);
    static string getVecStr(const vector<int>& vec);
public:
    DbChecker(const DbChecker&) = delete;
    DbChecker& operator=(const DbChecker&) = delete;
    static DbChecker& getInstance()
    {
        static MemGuard g;
        if (!instance) {
            instance = new DbChecker();
        }
        return *instance;
    }
    vector<shared_ptr<RouteInfo>> getInfoUpdatingRoutes();
};
