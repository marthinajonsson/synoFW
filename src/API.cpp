//
// Created by mjonsson on 5/30/19.
//

#include <ErrorCodes.h>
#include "API.h"
#include <cstring>

std::string API::loadAPI(const std::string &file, std::string &api)
{
    boost::property_tree::ptree root;
    boost::property_tree::read_json(file, root);

    for(auto it : root) {
        auto key = it.first;
        auto tmp = boost::algorithm::to_lower_copy(key);
        if(tmp.find(api) != std::string::npos) {
            return key;
        }
    }
    throw GENERIC::BadRequestException(GENERIC::ERROR_CODE_API_DOES_NOT_EXISTS, "No API found");
}

std::string API::loadMethod(const std::string &file, std::string& api, int&val)
{
    boost::property_tree::ptree root;
    boost::property_tree::read_json(file, root);
    boost::property_tree::ptree node;
    val = 0;

    auto nodeIt = root.find(api);
    if(root.not_found() != nodeIt) {
        node = (*nodeIt).second;
    }
    auto methods = node.get_child("method");
    std::vector<std::string> vec;

    BOOST_FOREACH( boost::property_tree::ptree::value_type& v, methods) {
        auto n = v.second.get<std::string>("name");
        vec.push_back(n);
    }
    if(vec.size() == 1 || testing ) {
        return vec.front();
    }

    for(auto &s : vec) {
        std::cout << val << ": " << s << std::endl;
    }

    std::cout << "Choose method: " << std::endl;
    std::cin >> val;

    if(!vec.at(val).empty()) {
        return vec.at(val);
    }

    throw GENERIC::BadRequestException(GENERIC::ERROR_CODE_METHOD_DOES_NOT_EXISTS, "No API methods found");

}

std::string API::loadParams(const std::string &file, std::string &api, int &val) {
    boost::property_tree::ptree root;
    boost::property_tree::read_json(file, root);
    boost::property_tree::ptree node;
    val = 0;

    auto nodeIt = root.find(api);
    if(root.not_found() != nodeIt) {
        node = (*nodeIt).second;
    }
    auto methods = node.get_child("method");
    std::string result;

    BOOST_FOREACH( boost::property_tree::ptree::value_type& v, methods) {
                    auto p = v.second.get<std::string>("param");
                    auto o = v.second.get<std::string>("optional");
                    result.append(p.append(":").append(o).append(":"));
                }

    if(result.empty()) {
        throw GENERIC::BadRequestException(GENERIC::ERROR_CODE_NO_PARAMETER, "No parameter of API method");
    }
    return result;
}


std::string API::loadPath(const std::string &file, std::string& api) {
    boost::property_tree::ptree root;
    boost::property_tree::read_json(file, root);
    boost::property_tree::ptree node;

    auto nodeIt = root.find(api);
    if(root.not_found() != nodeIt) {
        node = (*nodeIt).second;
    }
    auto result = node.get<std::string>("path");
    if(result.empty()) {
        throw GENERIC::BadRequestException(GENERIC::ERROR_CODE_NO_PARAMETER, "No parameter API path");
    }
    return result;
}

std::string API::loadVersion(const std::string &file, std::string& api) {
    boost::property_tree::ptree root;
    boost::property_tree::read_json(file, root);
    boost::property_tree::ptree node;

    auto nodeIt = root.find(api);
    if(root.not_found() != nodeIt) {
        node = (*nodeIt).second;
    }
    auto result = node.get<std::string>("maxVersion");
    if(result.empty()) {
        throw GENERIC::BadRequestException(GENERIC::ERROR_CODE_NO_PARAMETER, "No parameter Version for this API");
    }
    return result;
}

std::string API::loadResponse(const std::string &file, std::string &api, int &val) {
    boost::property_tree::ptree root;
    boost::property_tree::read_json(file, root);
    boost::property_tree::ptree node;
    val = 0;

    auto nodeIt = root.find(api);
    if(root.not_found() != nodeIt) {
        node = (*nodeIt).second;
    }
    auto methods = node.get_child("method");
    std::string result;

    BOOST_FOREACH(boost::property_tree::ptree::value_type& v, methods) {
        auto p = v.second.get<std::string>("response");
        result.append(p.append(":"));
    }
    return result;
}
