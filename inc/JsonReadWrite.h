//
// Created by mjonsson on 5/20/19.
//

#ifndef SYNOFW_JSONREADWRITE_H
#define SYNOFW_JSONREADWRITE_H

#include <string>
#include <json/json.h>
#include <fstream>

class JsonReadWrite {
private:
    std::string m_filename;
    std::string m_path;

public:

    JsonReadWrite(std::string &f) : m_filename(f){
        m_path = "../data/";
        m_path.append(m_filename);
    }
    ~JsonReadWrite() = default;
    void writeStream();
    void readStream(std::string&);
};


#endif //SYNOFW_JSONREADWRITE_H
