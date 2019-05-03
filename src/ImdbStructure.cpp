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

std::vector<std::string> ImdbStructure::parse(const std::string &&filename, const std::string &&matchingTitle, std::vector<std::pair<unsigned short, std::string>> &&filterColumn)
{
    std::fstream file;
    file.open(filename, std::ios::in);

    std::vector<std::string> row;

    std::string line, word;

    std::string header;
    getline(file, header);

    std::stringstream s(header);
    std::sort(filterColumn.begin(), filterColumn.end());
    if(!filterColumn.empty())
    {
        short index = 0;
        while(getline(s, word, '\t'))
        {
            for (auto col : filterColumn) {

                if(col.first == index) {
                    row.emplace_back(word);
                    break;
                }

            }
            index++;
        }

    }else
    {
        while (getline(s, word, '\t')) {
            row.emplace_back(word);
        }
    }

    std::vector<std::string> tmp;
    tmp.reserve(filterColumn.size());
    for(auto f : filterColumn) {
        tmp.emplace_back(f.second);
    }

    while (!file.eof()) {

        getline(file, line);
        std::stringstream s(line);

        if(!filterColumn.empty()) {

            bool found = false;
            for(auto &str : tmp) {
                if(line.find(str) != std::string::npos) {
                    found = true;
                }
            }
            if(!found) { continue;}

            short index = 0;
            while(getline(s, word, '\t'))
            {
                for (auto col : filterColumn) {

                    if(col.first == index) {
                        row.emplace_back(word);
                        break;
                    }

                }
                index++;
            }

        }
        else {
            while (getline(s, word, '\t')) {
                row.emplace_back(word);
            }
        }

    }
    file.close();

    return row;
}
