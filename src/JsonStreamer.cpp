//
// Created by mjonsson on 5/20/19.
//

#include <boost/lexical_cast.hpp>
#include <iostream>
#include <fstream>

#include "JsonStreamer.h"
#include "Utilities.h"

/*
 * writes:
 *
 * {
 *   0: {
 *
 *     parent : value
 *     additional
 *     {
 *       child : value,
 *       child : value
 *     }
 *   },
 *   1: {
 *     parent : value
 *     additional
 *     {
 *       child : value,
 *       child : value
 *     }
 *   }
 * }
 * */

template <typename Type, typename Enum, class ValueType = std::string>
void JsonStreamer::writeStream(Type &write_obj) {

    boost::property_tree::ptree root;
    boost::property_tree::ptree parent;
    boost::property_tree::ptree child;

    int propertyInt = 0;

    boost::property_tree::read_json(_path, root);
    long size = root.size();
    auto index = std::to_string(size);
    int sizeEnum = sizeof(Enum) / sizeof(Enum[0]);

    auto property = static_cast<Enum>(propertyInt);
    auto parentKey = _Keys.at(property);
    ValueType parentValue = *getBegin(&write_obj);
    parent.put(parentKey, parentValue);

    for (ValueType* p = getBegin(&write_obj) + 1; p < getEnd(&write_obj) && propertyInt < sizeEnum; propertyInt++, p++) {
        auto property = static_cast<E>(propertyInt);
        auto val = *p;
        auto key = keys.at(property);
        child.put(key, val);
    }

    parent.put_child("additional", child);

    root.push_back(std::make_pair(index, parent));
    boost::property_tree::json_parser::write_json(_path, root);
}


template <typename Type, typename Enum, typename ValueType = std::string>
boost::property_tree::ptree JsonStreamer::readStream(ValueType& search_pattern)
{
    boost::property_tree::ptree root;
    boost::property_tree::ptree empty;
    boost::property_tree::read_json(_path, root);

    auto property = static_cast<Enum>(0);
    auto parentKey = _Keys.at(property);

    for (auto &it : root) {
        auto val = it.second;
        if (val.empty())
            continue;
        auto found = val.get<ValueType>(parentKey);
        if (found.find(search_pattern) != std::string::npos)
            return val;
    }
    return empty;
}

template <typename Type, typename Enum, typename ValueType = std::string>
Type JsonStreamer::find(ValueType &search_pattern)
{
    boost::property_tree::ptree item;
    Type obj;

    item = readStream(search_pattern);

    ValueType* p = getBegin<ValueType(&obj);

    int propertyInt = 0;
    auto property = static_cast<Enum>(propertyInt);
    int sizeEnum = sizeof(Enum) / sizeof(Enum[0]);
    auto parentKey = _Keys.at(property);

    ValueType* parentValue = getBegin(&obj);
    parentValue = item.get<ValueType>(parentKey);

    item = item.get_child("additional");
    for (ValueType* p = getBegin(&obj) + 1; p < getEnd(&obj) && propertyInt < sizeEnum; propertyInt++, p++) {
        auto property = static_cast<Enum>(propertyInt);
        auto key = keys.at(property);
        p = item.get<ValueType>(key);
    }
    return obj;
}