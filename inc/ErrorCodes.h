//
// Created by mjonsson on 1/25/19.
//

#ifndef SYNOFW_ERRORCODES_H
#define SYNOFW_ERRORCODES_H

#include <string>
#include <map>
#include <algorithm>

namespace ERROR {

    std::pair<int, std::string> UNKNOWN = {100, "Unknown error"};

    std::pair<int, std::string> MISSING_API_METHOD_VERSION = {101, "No parameter of API, method or version"};

    std::pair<int, std::string> API = {102, "The requested API does not exist"};

    std::pair<int, std::string> METHOD = {103, "The requested method does not exist"};

    std::pair<int, std::string> VERSION = {104, "The requested version does not support the functionality"};

    std::pair<int, std::string> PERMISSION = {105, "The logged in session does not have permission"};

    std::pair<int, std::string> SESSION_TIMEOUT = {106, "Session timeout"};

    std::pair<int, std::string> DUP_LOGIN = {107, "Session interrupted by duplicate login"};


}
#endif //SYNOFW_ERRORCODES_H
