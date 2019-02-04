//
// Created by mjonsson on 1/25/19.
//

#ifndef SYNOFW_ERRORCODES_H
#define SYNOFW_ERRORCODES_H

#include <vector>
#include <string>
#include <map>
#include <mutex>
#include <algorithm>

#include <Utilities.h>


namespace ERROR {

    namespace CODE {

        const std::pair<int, std::string> UNKNOWN = {100, "Unknown error"};

        const std::pair<int, std::string> MISSING_API_METHOD_VERSION = {101, "No parameter of API, method or version"};

        const std::pair<int, std::string> API = {102, "The requested API does not exist"};

        const std::pair<int, std::string> METHOD = {103, "The requested method does not exist"};

        const std::pair<int, std::string> VERSION = {104, "The requested version does not support the functionality"};

        const std::pair<int, std::string> PERMISSION = {105, "The logged in session does not have permission"};

        const std::pair<int, std::string> SESSION_TIMEOUT = {106, "Session timeout"};

        const std::pair<int, std::string> DUP_LOGIN = {107, "Session interrupted by duplicate login"};

    }

    namespace COMMON{

        static std::vector<PAIR> commonPairs = {CODE::UNKNOWN, CODE::MISSING_API_METHOD_VERSION, CODE::API, CODE::METHOD, CODE::VERSION, CODE::PERMISSION, CODE::SESSION_TIMEOUT, CODE::DUP_LOGIN};

        static std::string get(const int &code) {
            std::unique_lock<std::mutex>  m_lock;

            for(auto p:commonPairs){
                if(p.first == code) {
                    return p.second;
                }
            }
            return "...";
        }
    }
}
#endif //SYNOFW_ERRORCODES_H
