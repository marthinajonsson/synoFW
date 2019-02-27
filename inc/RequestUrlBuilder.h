//
// Created by mjonsson on 2/9/19.
//

#ifndef SYNOFW_REQUESTURLBUILDER_H
#define SYNOFW_REQUESTURLBUILDER_H

#include "Utilities.h"

class RequestUrlBuilder {
public:

    RequestUrlBuilder(std::string& _url, std::string& _path, std::string& _api, std::string& _version,
            std::string& _method, std::string& _params) : urlString(_url), path(_path), api(_api),
        version(_version), method(_method), params(_params)
    {
        urlString+="/webapi/"+path;
        urlString+="?api="+api;
        urlString+="&version="+version;
        urlString+="&method="+method;
        urlString+=params;
        urlString+="&_sid=";
        removeEndOfLines(urlString);
    }
    ~RequestUrlBuilder() = default;

    std::string getResult() { return urlString; }

private:

    std::string urlString;
    std::string path;
    std::string api;
    std::string version;
    std::string method;
    std::string params;
};

#endif //SYNOFW_REQUESTURLBUILDER_H
