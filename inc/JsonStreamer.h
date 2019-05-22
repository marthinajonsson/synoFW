//
// Created by mjonsson on 5/20/19.
//

#ifndef SYNOFW_JSONREADWRITE_H
#define SYNOFW_JSONREADWRITE_H

#include <string>
#include <json/json.h>
#include <fstream>
#include "DatabaseObject.h"

class JsonStreamer {
private:
    std::string m_filename;
    std::string m_path;

    void writeStream(DatabaseObject &obj);
    Json::Value readStream(std::string&);
public:

    JsonStreamer() = default;

    explicit JsonStreamer(const std::string&f) : m_filename(f) {
        m_path = "../data/";
        m_path.append(f);
    }

    void setFile(std::string&&f) {
        m_path = "../data/";
        m_path.append(f);
    }
    ~JsonStreamer() = default;

    DatabaseObject find(std::string&);
    void update(DatabaseObject &obj);
    bool checkForNull(std::string&);
};


#endif //SYNOFW_JSONREADWRITE_H
