//
// Created by mjonsson on 5/20/19.
//

#include <iostream>
#include "JsonStreamer.h"

typedef boost::property_tree::ptree ptree;

void JsonStreamer::writeStream(database &obj) {

    ptree root;
    boost::property_tree::read_json(_path, root);

    long size = root.size();
    std::string index = std::to_string(size);
    ptree parent, child;
    parent.put("title", obj.m_title);
    child.put("titleId", obj.m_titleId);
    child.put("titleType", obj.m_titleType);
    child.put("genre", obj.m_genre);
    child.put("actors", "Ryan Reynolds");
    child.put("startYear", obj.m_startYear);
    child.put("endYear", !obj.m_endYear.empty() ? obj.m_endYear : "0");
    child.put("runtimeMinutes", !obj.m_runtimeMinutes.empty() ? obj.m_runtimeMinutes : "0");
    child.put("directors", obj.m_directors);
    child.put("writers", obj.m_writers);
    child.put("region", !obj.m_region.empty() ? obj.m_language : "N");
    child.put("language", !obj.m_language.empty() ? obj.m_language : "0");
    child.put("season", !obj.m_season.empty() ? obj.m_season : "0");
    child.put("episode", !obj.m_episode.empty() ? obj.m_episode : "0");
    child.put("parentTconst", !obj.m_parentTconst.empty() ? obj.m_parentTconst : "0");
    child.put("libraryId", !obj.m_library_id.empty() ? obj.m_library_id : "0");
    child.put("filename", !obj.m_filename.empty() ? obj.m_filename : "0");
    child.put("path", !obj.m_path.empty() ? obj.m_path : "0");
    parent.put_child("metadata", child);

    root.push_back(std::make_pair(index, parent));
    boost::property_tree::json_parser::write_json(_path, root);
}

boost::property_tree::ptree JsonStreamer::readStream(std::string& searchTitle) {
    ptree root, empty;
    boost::property_tree::read_json(_path, root);

    for(auto it : root) {
        auto key = it.first;
        auto val = it.second;
        auto found = val.get<std::string>("title");
        if(found.find(searchTitle) != std::string::npos) {
            return val;
        }
    }
    return empty;
}


bool JsonStreamer::checkForNull(std::string &searchTitle) {
    auto item = readStream(searchTitle);
    if(item.empty()) { return true; }
    return false;
}

void JsonStreamer::update(database &obj) {
    writeStream(obj);
}

database JsonStreamer::find(std::string &searchTitle) {
    auto item = readStream(searchTitle);
    database obj;
    obj.m_title = item.get<std::string>("title");
    auto child = item.get_child("metadata");

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