//
// Created by mjonsson on 5/20/19.
//

#include <iostream>
#include "JsonStreamer.h"

typedef boost::property_tree::ptree ptree;

void JsonStreamer::writeStream(database &obj) {

    ptree root;
    ptree parent;
    ptree child;

    boost::property_tree::read_json(_path, root);
    long size = root.size();
    auto index = std::to_string(size);

    auto parentKey = _propertyMap.at(TITLE_P);
    auto parentValue = *getBegin(&obj);

    parent.put(parentKey, parentValue);
    int propertyInt = TITLE_ID_P;
    for (std::string* p = getBegin(&obj) + 1; p < getEnd(&obj); propertyInt++, p++) {
        auto property = static_cast<PROPERTY >(propertyInt);
        if (propertyInt > FILENAME_P)
            break;
        auto val = *p;
        auto key = _propertyMap.at(property);
        child.put(key, val);
    }

    parent.put_child("additional", child);

    root.push_back(std::make_pair(index, parent));
    boost::property_tree::json_parser::write_json(_path, root);
}

boost::property_tree::ptree JsonStreamer::readStream(std::string& searchTitle) {
    ptree root;
    ptree empty;
    boost::property_tree::read_json(_path, root);

    for(auto &it : root) {
        auto key = it.first;
        auto val = it.second;
        auto found = val.get<std::string>(TITLE);
        if(found.find(searchTitle) != std::string::npos) {
            return val;
        }
    }
    return empty;
}


bool JsonStreamer::checkForNull(std::string &searchTitle) {
    auto item = readStream(searchTitle);
    return item.empty();
}

void JsonStreamer::update(database &obj) {
    writeStream(obj);
}

database JsonStreamer::find(std::string &searchTitle) {
    auto item = readStream(searchTitle);
    database obj;
    std::string* p = getBegin(&obj);
    auto key = _propertyMap.at(TITLE_P);
    *p = item.get<std::string>(key);

    auto test = obj.m_title;
    auto test2 = *p;

    obj.m_title = item.get<std::string>("title");
    auto child = item.get_child("additional");

    obj.m_titleId = child.get<std::string>("titleId");
    obj.m_titleType = child.get<std::string>("titleType");
    obj.m_genre = child.get<std::string>("genre");
    obj.m_startYear = child.get<std::string>("startYear");
    obj.m_endYear = child.get<std::string>("endYear");
    obj.m_runtimeMinutes = child.get<std::string>("runtimeMinutes");
    obj.m_directors = child.get<std::string>("directors");
    obj.m_writers = child.get<std::string>("writers");
    obj.m_path = child.get<std::string>("path");
    obj.m_library_id = child.get<std::string>("filename");
    if(obj.m_titleType == "series") {
        obj.m_episode = child.get<std::string>("episode");
        obj.m_season = child.get<std::string>("season");
        obj.m_parentTconst = child.get<std::string>("parentTconst");
    }
    return obj;
}