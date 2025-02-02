#pragma once
#include "string"
using std::string;
struct RouteInfo{
    int usersId;
    string userName;
    string telegram;
    string email;
    int routeId;
    string ticketData;
    RouteInfo(){
        usersId=0;
        userName="";
        telegram="";
        email="";
        routeId=0;
        ticketData="";
    }
    string toString(){
        string str = fmt::format("usersId = {}\n"
                                 "userName = {}\n"
                                 "telegram ={}\n"
                                 "email = {}\n"
                                 "routeId = {}\n"
                                 "ticketData = {}\n\n",usersId,userName,telegram,email,routeId,ticketData);
        return str;
    }
    string toJsonString() {
        string json = fmt::format(
                "{{\n"
                "  \"usersId\": \"{}\",\n"
                "  \"userName\": \"{}\",\n"
                "  \"telegram\": \"{}\",\n"
                "  \"email\": \"{}\",\n"
                "  \"routeId\": \"{}\",\n"
                "  \"ticketData\": {}}"
                "}",
                usersId, userName, telegram, email, routeId, ticketData);
        return json;
    }
};