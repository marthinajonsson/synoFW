//
// Created by mjonsson on 5/2/19.
//

#include <iostream>
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


std::vector<std::string> ImdbStructure::parse(std::string &&filename, std::string &&findStr, const short &column, const short &compareColumn, std::string&& compareStr)
{
    std::fstream file;
    file.open(filename, std::ios::in);

    std::vector<std::string> row;

    std::string line, word, temp;

    std::string header;
    getline(file, header);

    if(column < 99) {
        //std::string firstWord = header.substr(0, line.find('\t'));
        std::stringstream s(header);
        for(int i = 0; i <= column; i++) {
            getline(s, word, '\t');
        }
        row.emplace_back(word);
    }else {
        std::stringstream s(header);
        while (getline(s, word, '\t')) {
            row.emplace_back(word);
        }

    }

    while (!file.eof()) {

        getline(file, line);

        if( std::search(line.begin(), line.end(),
                        findStr.begin(), findStr.end()) == line.end()){
            continue;
        }

        std::stringstream s(line);

        if(column < 99) {
//            std::string firstWord = line.substr(0, line.find('\t'));
//            row.emplace_back(firstWord);
            for(int i = 0; i <= column; i++) {
                getline(s, word, '\t');
            }
            if(column == 2 && word.compare(findStr) != 0) { //title
                continue;
            }

            if(compareStr != "" && word != compareStr) {
                continue;
            }

            row.emplace_back(word);
        }else {
            while (getline(s, word, '\t')) {
                row.emplace_back(word);
            }
        }

    }
    file.close();

    return row;
}
