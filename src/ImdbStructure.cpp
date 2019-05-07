//
// Created by mjonsson on 5/2/19.
//

#include <iostream>
#include <any>
#include <cstring>
#include "ImdbStructure.h"

size_t ImdbStructure::write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
    size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
    return written;
}

void ImdbStructure::unpackFile(std::string&& file) {

    std::string chmodCmd = "chmod +rx " + file;
    system(chmodCmd.c_str());
    std::string unpackCmd = "gzip -d -f " + file;
    system(unpackCmd.c_str());
}

void ImdbStructure::getDataFiles(std::string&& file) {

    CURL *curl = NULL;
    FILE *fp;

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    CURLcode res;
    int retCode;

    if (curl) {
        std::string url = "https://datasets.imdbws.com/" + file;
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        fp = fopen(file.c_str(), "wb");


        curl_easy_setopt(curl, CURLOPT_NOPROGRESS , 0L);
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
            std::cerr << "HTTP code " << retCode << std::endl;
        }

        curl_easy_cleanup(curl);

    }
    curl_global_cleanup();
}

bool sort(const std::pair<const unsigned short,std::string> &a,
               const std::pair<const unsigned short,std::string> &b)
{

    return (a.first < b.first);

}

std::vector<std::string> ImdbStructure::parse(const std::string &&filename, std::pair<unsigned short, std::string> &&match, std::vector<std::pair<unsigned short, std::string>> &&add)
{
    std::fstream file;
    file.open(filename, std::ios::in);

    std::vector<std::string> tmpVec, row;

    std::string line, word;

    std::sort(add.begin(), add.end());
    getline(file, line); // ignore header
    while(getline(file, line))
    {
        std::stringstream ss(line);
        while (getline(ss, word, '\t'))
        {
            tmpVec.emplace_back(word);
        }

        if(!match.second.empty())
        {
            auto found = std::find(tmpVec.begin(), tmpVec.end(), match.second);
            if(found != tmpVec.end()) {
                std::string added;
                for(auto &filterAdd : add) {
                    added = tmpVec.at(filterAdd.first);
                    added.append(":");
                }
                std::string matching = *found;
                row.emplace_back(added+matching);
            }
            tmpVec.resize(0);
        }else {
            for(auto &item : tmpVec) {
                row.emplace_back(item);
            }
            tmpVec.resize(0);
        }

    }
    file.close();

    return row;
}
