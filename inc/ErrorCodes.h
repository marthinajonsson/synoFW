//
// Created by mjonsson on 1/25/19.
//

#ifndef SYNOFW_ERRORCODES_H
#define SYNOFW_ERRORCODES_H


#include <algorithm>
#include <json/json.h>
#include <iostream>
#include <vector>
#include <string>
#include <mutex>
#include <map>

#include <Utilities.h>


namespace GENERIC {

    static int ERROR_CODE_UNKNOWN_ERROR = 100;
    static int ERROR_CODE_NO_PARAMETER = 101;
    static int ERROR_CODE_API_DOES_NOT_EXISTS = 102;
    static int ERROR_CODE_METHOD_DOES_NOT_EXISTS = 103;
    static int ERROR_CODE_VERSION_DOES_NOT_SUPPORT_FEATURE = 104;
    static int ERROR_CODE_PERMISSION_DENIED = 105;
    static int ERROR_CODE_SESSION_TIMEOUT = 106;
    static int ERROR_CODE_DUPLICATE_LOGIN = 107;

    class UnknownException : public std::logic_error
    {
    public:
        std::string code;
        UnknownException() : code(std::to_string(ERROR_CODE_UNKNOWN_ERROR)), std::logic_error("Unknown error - " + code) {
        }
    };

    class BadRequestException : public std::logic_error
    {
    public:
        std::string code;
        BadRequestException() :  code(std::to_string(ERROR_CODE_NO_PARAMETER)), std::logic_error("No parameter of API, method or version - " + code) { };
        BadRequestException(int &error, std::string &&desc) :  code(std::to_string(error)), std::logic_error(desc + " - " + code) { };
    };

    static void printError(Json::Value &response) {

        int code = response["error"]["code"].asInt();
        std::cout << "Request failed - " << code;
        auto errArray = response["error"]["errors"];
        for(auto a : errArray) {
            for (Json::Value::const_iterator it=a.begin(); it!=a.end(); ++it) {
                auto fault = it->asString();
                std::cout << "\n" << fault << std::endl;
            }
        }
    }
}
#endif //SYNOFW_ERRORCODES_H
