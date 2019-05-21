//
// Created by mjonsson on 5/21/19.
//

#ifndef SYNOFW_IMDBPARSER_H
#define SYNOFW_IMDBPARSER_H


#include "Logger.h"
#include "DatabaseObject.h"

#include <curl/curl.h>
#include <string>
#include <mutex>

static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
    size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
    return written;
}

class Imdb {
public:
    Imdb() = default;
    ~Imdb() = default;
    virtual void init() = 0;
    virtual const std::map<unsigned short, std::string>& getMetaMapping() = 0;
    virtual std::map<std::string, std::string> parse(std::pair<unsigned short, std::string> &&, std::vector<std::pair<unsigned short, std::string>> &&) = 0;

    std::mutex akasLck;
    std::mutex basicsLck;
    std::mutex crewLck;
    std::mutex episodeLck;
    std::mutex nameLck;

    static std::map<std::string, std::map<unsigned short, std::string>> metaMapper;
//
//    std::map<unsigned short, std::string> mapCrew {
//            {0, "titleId"}, {1, "directors"}, {2, "writers"}
//    };
//
//    std::map<unsigned short, std::string> mapEpisode {
//            {0, "episodeId"}, {2, "season"}, {3, "episode"}
//    };
//
//    std::map<unsigned short, std::string> mapNames {
//            {0, "nameId"}, {1, "primaryName"}
//    };

    std::string unpackFile(std::string&& file) {

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

    std::string getDataFiles(std::string&& file) {

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


    void fetch(std::shared_ptr<Logger> &pLog) {
        pLog->writeLog(SeverityType::GENERAL, "Downloading compressed files.. ");
        auto files = {"title.akas.tsv.gz", "title.basics.tsv.gz", "title.crew.tsv.gz", "title.episode.tsv.gz", "name.basics.tsv.gz"};

        for(auto f : files) {
            std::string err = getDataFiles(f);
            if(!err.empty()) {
                std::string info = "Compressed file ";
                info.append(f);
                info.append(" failed to download due to ");
                info.append(err);
                pLog->writeLog(SeverityType::WARNING, info);
            }
        }
        pLog->writeLog(SeverityType::GENERAL, "Downloading complete");


        pLog->writeLog(SeverityType::GENERAL, "Decompress files.. ");
        for(auto f : files) {
            std::string err = unpackFile(f);
            if(!err.empty()) {
                std::string info = "Decompressing file ";
                info.append(f);
                info.append(" failed due to ");
                info.append(err);
                pLog->writeLog(SeverityType::WARNING, info);
            }
        }
        pLog->writeLog(SeverityType::GENERAL, "IMDB files are decompressed");
    }
};


#endif //SYNOFW_IMDBPARSER_H
