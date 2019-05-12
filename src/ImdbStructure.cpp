//
// Created by mjonsson on 5/2/19.
//

#include <iostream>
#include <any>
#include <cstring>

#include "ImdbStructure.h"

std::mutex imdbLock;

static ImdbStructure *instance;
ImdbStructure& ImdbStructure::getInstance()
{
    std::lock_guard<std::mutex> lock(imdbLock);
    if(!instance) {
        instance = new ImdbStructure();
    }
    return *instance;
}

size_t ImdbStructure::write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
    size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
    return written;
}

std::string ImdbStructure::unpackFile(std::string&& file) {

    std::string errStr;
    std::string chmodCmd = "chmod a+x " + file;
    if(0 != system(chmodCmd.c_str()) ) {
        errStr = "Unable to run chmod on " + file;
    }
    std::string unpackCmd = "gzip -d -f " + file;
    if(0 != system(unpackCmd.c_str()) ) {
        errStr = "Unable to decompess file " + file;
    }
    return errStr;
}

std::string ImdbStructure::getDataFiles(std::string&& file) {

    CURL *curl = NULL;
    FILE *fp;
    std::string errStr;
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    CURLcode res;
    int retCode;

    if (curl) {
        std::string url = "https://datasets.imdbws.com/" + file;
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        fp = fopen(file.c_str(), "wb");


        curl_easy_setopt(curl, CURLOPT_NOPROGRESS , 1L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);

        if(fp) {
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
            curl_easy_perform(curl);
            fclose(fp);
        }

        long http_code = 0;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

        retCode = static_cast<int>(http_code);
        if(retCode != 200) {
            errStr = "HTTP code ";
            errStr.append(std::to_string(retCode));
        }

        curl_easy_cleanup(curl);

    }
    curl_global_cleanup();
    return errStr;
}

std::map<std::string, std::string> ImdbStructure::parse(const std::string &&filename, std::pair<unsigned short, std::string> &&match, std::vector<std::pair<unsigned short, std::string>> &&add)
{
    std::map<std::string, std::string> metadata;
    std::vector<std::string> row;
    std::string line, word;

    std::fstream file;
    file.open(filename, std::ios::in);

    if(!file.is_open()) {
        std::cerr << typeid(this).name() << " - File is not open and cannot be parsed" << std::endl;
        return metadata;
    }

    std::sort(add.begin(), add.end());
    getline(file, line); // ignore header
    while(getline(file, line))
    {
        std::stringstream ss(line);
        while (getline(ss, word, '\t'))
        {
            row.emplace_back(word);
        }

        if(!match.second.empty())
        {
            auto found = std::find(row.begin(), row.end(), match.second);
            if(found != row.end()) {
                std::string matching = *found;

                metadata.insert ( std::pair<std::string,std::string>(metadataMapper.at(match.first), matching) );

                for(auto &filterAdd : add) {
                    metadata.insert ( std::pair<std::string, std::string> (metadataMapper.at(filterAdd.first), row.at(filterAdd.first)) );
                }
            }
        }
        row.resize(0);
    }
    file.close();

    return metadata;
}
