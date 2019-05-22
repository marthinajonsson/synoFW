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

    //rootItem = rootItem["cache"];
    addItem["title"] = obj.m_title;
    addItem["titleId"] = obj.m_titleId;
    addItem["genre"] = obj.m_genre;
    addItem["startYear"] = obj.m_startYear;
    addItem["endYear"] = obj.m_endYear;
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
        if (cacheItem["title"].asString() == searchTitle)
        {
            return cacheItem;
        }
    }
    return root;
}


bool JsonStreamer::checkForNull(std::string &searchTitle) {
    auto item = readStream(searchTitle);
    auto isNull = item.isNull();
    if(isNull) { return true; }

    for (auto const& keys : item.getMemberNames()) {
        auto val = item[keys].asString();
        isNull = val.empty();
        if(isNull) { break; }
    }
    return isNull;
}

void JsonStreamer::update(DatabaseObject &obj) {
    writeStream(obj);
}

DatabaseObject JsonStreamer::find(std::string &searchTitle) {
    auto item = readStream(searchTitle);
    DatabaseObject obj;
    obj.m_titleId = item["titleId"].asString();
    obj.m_title = item["title"].asString();
    obj.m_genre = item["genre"].asString();
    obj.m_directors = item["directors"].asString();
    obj.m_writers = item["writers"].asString();
    obj.m_crew = item["crew"].asString();
    obj.m_startYear = item["startYear"].asString();
    obj.m_endYear = item["endYear"].asString();
    return obj;
}