//
// Created by mjonsson on 5/23/19.
//

#ifndef SYNOFW_FILEMGR_H
#define SYNOFW_FILEMGR_H

#include <map>
#include <curl/curl.h>
#include <string>
#include <iostream>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <chrono>
#include <ctime>


static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
    size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
    return written;
}

class FileMgr {
public:

    struct tm *modifiedS;
    struct stat statS;

    std::map<std::string, bool> needUpdate ()
    {
        std::map<std::string, bool> checkList;
        auto files = {"title.akas.tsv", "title.basics.tsv", "title.crew.tsv", "title.episode.tsv", "name.basics.tsv"};
        std::string modifiedYear;
        std::string modifiedMonth;

        auto now = std::chrono::system_clock::now();
        std::time_t now_time = std::chrono::system_clock::to_time_t(now);
        char currentDateNum[10];
        if (std::strftime(currentDateNum, sizeof(currentDateNum), "%Y %m", std::localtime(&now_time))) {
            for(int i = 0; i < 4; i++) {
                modifiedYear += currentDateNum[i];
            }
            for(int i = 5; i < 7; i++) {
                modifiedMonth += currentDateNum[i];
            }
        }

        int todaysYear = stoi(modifiedYear);
        int todaysMonth = stoi(modifiedMonth);

        for(auto &f : files)
        {
            stat(f, &statS);
            modifiedS = gmtime(&(statS.st_mtime));
            auto year = modifiedS->tm_year;
            auto month = modifiedS->tm_mon;
            if(todaysYear >= year && todaysMonth > month + 2) {
                checkList.insert(std::make_pair(f, true));
            }
            checkList.insert(std::make_pair(f, false));
        }
        return checkList;
    }

    std::string unpackFile( std::string&& file) {

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

    std::string getDataFiles (std::string&& file) {

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


    void fetch ()
    {
        auto files = {"title.akas.tsv.gz", "title.basics.tsv.gz", "title.crew.tsv.gz", "title.episode.tsv.gz", "name.basics.tsv.gz"};

        auto require = needUpdate();

        for(auto f : files) {
            if(!require.at(f)) {
                continue;
            }
            std::string err = getDataFiles(f);
            if(!err.empty()) {
                std::string info = "Compressed file ";
                info.append(f);
                info.append(" failed to download due to ");
                info.append(err);
            }
        }

        for(auto f : files) {
            if(!require.at(f)) {
                continue;
            }
            std::string err = unpackFile(f);
            if(!err.empty()) {
                std::string info = "Decompressing file ";
                info.append(f);
                info.append(" failed due to ");
                info.append(err);
            }
        }
    }
};
#endif //SYNOFW_FILEMGR_H
