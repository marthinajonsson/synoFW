//
// Created by mjonsson on 1/25/19.
//

#ifndef SYNOFW_ERRORCODES_H
#define SYNOFW_ERRORCODES_H

#include <string>
#include <map>
#include <algorithm>

namespace ERROR {
    const int UNKNOWN_CODE = 100;
    const int PARAM_CODE = 101;
    const int API_CODE = 102;
    const int METHOD_CODE = 103;
    const int VERSION_CODE = 104;
    const int PERMISSION_CODE = 105;
    const int SESSION_TIMEOUT_CODE = 106;
    const int DUP_LOGIN_CODE = 107;


    std::string UNKNOWN_STR = "Unknown error";
    std::string NO_PARA_STR = "No parameter of API, method or version";
    std::string NO_API_STR = "The requested API does not exist";
    std::string NO_METHOD_STR = "The requested method does not exist";
    std::string VERSION_NOT_SUPP_STR = "The requested version does not support the functionality";
    std::string PERMISSION_STR = "The logged in session does not have permission";
    std::string SESSION_TIMEOUT_STR = "Session timeout";
    std::string SESSION_DUP_STR = "Session interrupted by duplicate login";

    std::map<std::string, int> mapper {{UNKNOWN_STR, UNKNOWN_CODE}, {NO_PARA_STR, PARAM_CODE}, {NO_API_STR, API_CODE}, {NO_METHOD_STR, METHOD_CODE}, {VERSION_NOT_SUPP_STR, VERSION_CODE},
                                       {PERMISSION_STR,PERMISSION_CODE}, {SESSION_TIMEOUT_STR, SESSION_TIMEOUT_CODE}, {SESSION_DUP_STR, DUP_LOGIN_CODE}};

    void get(const int &err, std::string &desc) {

        auto findings = std::find_if(std::begin(mapper), std::end(mapper), [&](const std::pair<std::string, int> &pair)
        {
            return pair.second == err;
        });

        std::string key = "...";
        if (findings != std::end(mapper))
        {
            key = findings->first;
        }
        desc = key;
    }
}
#endif //SYNOFW_ERRORCODES_H
