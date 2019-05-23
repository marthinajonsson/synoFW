//
// Created by mjonsson on 5/20/19.
//

#include "JsonStreamer.h"


void JsonStreamer::writeStream(DatabaseObject &obj) {
    Json::Value rootItem;
    Json::Value addItem;
    std::ifstream inFile(m_path, std::ifstream::binary);
    inFile >> rootItem;
    inFile.close();

    addItem["title"] = obj.m_title;
    addItem["titleId"] = obj.m_titleId;
    addItem["titleType"] = obj.m_titleType;
    addItem["genre"] = obj.m_genre;
    addItem["startYear"] = obj.m_startYear;
    addItem["endYear"] = !obj.m_endYear.empty() ? obj.m_endYear : "N";
    addItem["region"] = !obj.m_region.empty() ? obj.m_region : "N";
    addItem["language"] = !obj.m_language.empty() ? obj.m_language : "N";
    addItem["runtimeMinutes"] = !obj.m_runtimeMinutes.empty() ? obj.m_runtimeMinutes : "0";
    addItem["season"] = !obj.m_season.empty() ? obj.m_season : "0";
    addItem["episode"] = !obj.m_episode.empty() ? obj.m_episode : "0";
    addItem["parentTconst"] = !obj.m_parentTconst.empty() ? obj.m_parentTconst : "0";
    addItem["writers"] = obj.m_writers;
    addItem["directors"] = obj.m_directors;
    rootItem.append(addItem);

    std::ofstream outFile(m_path, std::ios::trunc);
    outFile << rootItem;
    outFile.close();
}

Json::Value JsonStreamer::readStream(std::string& searchTitle) {
    Json::Value root;
    std::ifstream inFile(m_path, std::ifstream::binary);
    inFile >> root;
    inFile.close();

    for (const Json::Value& cacheItem : root)
    {
        std::string cachedTitle = cacheItem["title"].asString();
        if (cachedTitle == searchTitle)
        {
            return cacheItem;
        }
    }

    return Json::Value(Json::arrayValue);
}


bool JsonStreamer::checkForNull(std::string &searchTitle) {
    auto item = readStream(searchTitle);
    auto needUpdate = item.empty();
    if(needUpdate) { return true; }

    for (auto const& keys : item.getMemberNames()) {
        auto val = item[keys].asString();
        if(val.empty()) { return true; }
    }
    return false;
}

void JsonStreamer::update(DatabaseObject &obj) {
    writeStream(obj);
}

DatabaseObject JsonStreamer::find(std::string &searchTitle) {
    auto item = readStream(searchTitle);
    DatabaseObject obj;
    obj.m_titleId = item["titleId"].asString();
    obj.m_titleType = item["titleType"].asString();
    if(obj.m_titleType == "series") {
        obj.m_episode = item["episode"].asString();
        obj.m_season = item["season"].asString();
        obj.m_parentTconst = item["parentTconst"].asString();
    }
    obj.m_title = item["title"].asString();
    obj.m_genre = item["genre"].asString();
    obj.m_region = item["region"].asString();
    obj.m_language = item["language"].asString();
    obj.m_directors = item["directors"].asString();
    obj.m_writers = item["writers"].asString();
    obj.m_runtimeMinutes = item["runtimeMinutes"].asString();
    obj.m_startYear = item["startYear"].asString();
    obj.m_endYear = item["endYear"].asString();
    return obj;
}