//
// Created by mjonsson on 2/9/19.
//

#ifndef SYNOFW_REQUESTURLBUILDER_H
#define SYNOFW_REQUESTURLBUILDER_H

#include "Utilities.h"

class RequestUrlBuilder {
public:

    RequestUrlBuilder(std::string& url, std::string& path, std::string& api, std::string& version,
            std::string& method, std::string& params) : _urlString(url), _path(path), _api(api),
        _version(version), _method(method), _params(params)
    {
        _urlString+="/webapi/"+path;
        _urlString+="?api="+api;
        _urlString+="&version="+version;
        _urlString+="&method="+method;
        _urlString+=params;
        _urlString+="&_sid=";
        removeEndOfLines(_urlString);
    }
    ~RequestUrlBuilder() = default;

    std::string getResult() { return _urlString; }

private:

    std::string _urlString;
    std::string _path;
    std::string _api;
    std::string _version;
    std::string _method;
    std::string _params;
};

#endif //SYNOFW_REQUESTURLBUILDER_H
