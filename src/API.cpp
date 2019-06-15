//
// Created by mjonsson on 5/30/19.
//

#include <ErrorCodes.h>
#include "API.h"
#include <cstring>

std::string API::loadAPI (const std::string &file, const std::string &api)
{
    boost::property_tree::ptree root;
    boost::property_tree::read_json(file, root);

    for (auto node : root) {
        auto key = node.first;
        auto tmp = boost::algorithm::to_lower_copy (key);
        if (tmp.find (api) != std::string::npos)
            return key;
    }
    throw GENERIC::BadRequestException (GENERIC::ERROR_CODE_API_DOES_NOT_EXISTS, "No API found");
}

std::string API::loadMethod (const std::string &file, const std::string& api, int& methodIndex)
{
    boost::property_tree::ptree root;
    boost::property_tree::ptree node;
    std::vector<std::string> result;
    int count = 0;

    boost::property_tree::read_json (file, root);

    auto nodeIterator = root.find (api);
    if (root.not_found() != nodeIterator) {
        node = (*nodeIterator).second;
    }
    auto methods = node.get_child("method");

    if (methods.empty())
        throw GENERIC::BadRequestException (GENERIC::ERROR_CODE_METHOD_DOES_NOT_EXISTS, "No API methods found");

    BOOST_FOREACH (boost::property_tree::ptree::value_type& m, methods) {
        if (count == methodIndex)
            return m.second.get<std::string>("name");

        count++;
    }
    return "";
}

std::string API::loadMethods (const std::string &file, const std::string& api, int& methodIndex)
{
    boost::property_tree::ptree root;
    boost::property_tree::ptree node;
    std::vector<std::string> result;
    int val = 0;

    boost::property_tree::read_json (file, root);

    auto nodeIterator = root.find (api);
    if (root.not_found() != nodeIterator) {
        node = (*nodeIterator).second;
    }
    auto methods = node.get_child("method");

    BOOST_FOREACH (boost::property_tree::ptree::value_type& m, methods) {
        result.emplace_back(m.second.get<std::string>("name"));
    }

    for (auto &s : result) {
        std::cout << val << ": " << s << std::endl;
        val++;
    }

    std::cout << "Choose method: " << std::endl;
    std::cin >> val;

    if (result.at(val).empty())
        throw GENERIC::BadRequestException (GENERIC::ERROR_CODE_METHOD_DOES_NOT_EXISTS, "No API methods found");

    return result.at(val);
}


std::string API::loadParams (const std::string &file, const std::string &api, int &methodIndex) {
    boost::property_tree::ptree root;
    boost::property_tree::ptree node;
    std::string result;
    auto count = 0;

    boost::property_tree::read_json (file, root);

    auto nodeIterator = root.find (api);
    if (root.not_found() != nodeIterator)
        node = (*nodeIterator).second;

    auto methods = node.get_child("method");

    BOOST_FOREACH ( boost::property_tree::ptree::value_type& v, methods) {
        if (count == methodIndex) {
            auto p = v.second.get<std::string>("param");
            auto o = v.second.get<std::string>("optional");
            result.append(p.append(":").append(o));
            break;
        }
        count++;
    }

    if (result.empty())
        throw GENERIC::BadRequestException(GENERIC::ERROR_CODE_NO_PARAMETER, "No parameter of API method");

    return result;
}


std::string API::loadPath (const std::string &file, const std::string& api) {
    boost::property_tree::ptree root;
    boost::property_tree::ptree node;

    boost::property_tree::read_json(file, root);

    auto nodeIterator = root.find (api);
    if (root.not_found() != nodeIterator)
        node = (*nodeIterator).second;

    auto result = node.get<std::string>("path");
    if (result.empty())
        throw GENERIC::BadRequestException(GENERIC::ERROR_CODE_NO_PARAMETER, "No parameter path for this API");

    return result;
}

std::string API::loadVersion (const std::string &file, const std::string& api) {
    boost::property_tree::ptree root;
    boost::property_tree::ptree node;

    boost::property_tree::read_json(file, root);

    auto nodeIterator = root.find(api);
    if (root.not_found() != nodeIterator)
        node = (*nodeIterator).second;

    auto result = node.get<std::string>("maxVersion");
    if (result.empty())
        throw GENERIC::BadRequestException(GENERIC::ERROR_CODE_NO_PARAMETER, "No parameter Version for this API");

    return result;
}

std::string API::loadResponse (const std::string &file, const std::string &api, const int& methodIndex) {
    boost::property_tree::ptree root;
    boost::property_tree::ptree node;
    int count = 0;

    boost::property_tree::read_json(file, root);

    auto nodeIterator = root.find(api);
    if (root.not_found() != nodeIterator)
        node = (*nodeIterator).second;

    auto methods = node.get_child("method");

    BOOST_FOREACH (boost::property_tree::ptree::value_type& m, methods) {
        if (methodIndex == count)
            return m.second.get<std::string>("response");

        count++;
    }
    throw GENERIC::BadRequestException(GENERIC::ERROR_CODE_NO_PARAMETER, "No parameter Response for this API");
}
