//
// Created by mjonsson on 2/8/19.
//

#ifndef SYNOFW_PARAMHANDLING_H
#define SYNOFW_PARAMHANDLING_H

#include <algorithm>
#include <iostream>
#include <string>

class ParamHandling {
public:
    bool m_testing = false;
    ParamHandling (bool testing) : m_testing(testing) {};
    ParamHandling () = default;
    ~ParamHandling () = default;

    std::string setParam (std::string &&param, std::string &&defaultvalue);

    std::string getPathFolder (std::string& pattern);

    std::string getPath (std::string& pattern);
};


#endif //SYNOFW_PARAMHANDLING_H
