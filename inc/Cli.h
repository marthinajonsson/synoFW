//
// Created by mjonsson on 9/30/18.
//

#ifndef MEDIAFW_CLI_H
#define MEDIAFW_CLI_H

#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

#include "Metadata.h"
#include "Request.h"
#include "JsonParser.h"
#include "ifc/CommandLineParser.h"

/*! \class Cli Cli.h "inc/Cli.h"
 * @brief Module handling everything related to our Command line interface.
 */
class Cli : CommandLineParser
{
private:

    /*! \var FILENAME "filename
     *  @brief used to refer to @property filename
     */
    const std::string FILENAME = "filename";


    /*! \var MOVIE "movie"
     * @brief used to refer to @enum Category
     */
    std::string MOVIE = "movie";
    /*! \var SERIES "series"
     * @brief used to refer to @enum Category
     */
    std::string SERIES = "series";

    /*! \var DOWNLOAD "download"
     * @brief used to refer to @enum Event
     */
    const std::string DOWNLOAD = "download";
    /*! \var UPLOAD "upload"
     * @brief used to refer to @enum Event
     */
    const std::string UPLOAD = "upload";
    /*! \var SEARCH "search"
     * @brief used to refer to @enum Event
     */
    const std::string SEARCH = "search";
    /*! \var DELETE "delete"
     * @brief used to refer to @enum Event
     */
    const std::string DELETE = "delete";
    /*! \var HELP "help"
     * @brief used to refer to @enum Event
     */
    const std::string HELP = "help";
    /*! \var EXIT "exit"
     * @brief used to refer to @enum Event
     */
    const std::string EXIT = "exit";

    /*! \var SSH "ssh"
     * @brief used to refer to @enum Event
     */
    const std::string STREAM = "stream";

    /*! \var EVENT_ARGS
     * @brief contain all valid @enum Event using @property
     */
    const std::vector<std::string> EVENT_ARGS = {UPLOAD, DOWNLOAD, SEARCH, DELETE, EXIT, HELP, STREAM};

public:
    Cli() = default;
    ~Cli() = default;

    /*! \public Client::process()
     * @implements the interface of CommandLineParser
     * @brief Public method processing input from stdin.
     * @
     * @return Vector of strings containing output from stdin.
     */
    Request process() override;

    /*! \public Client::interprete
     * @brief translate incoming user request and save in request object
     * @implements the interface of CommandLineParser
     * @param std::vector<std::string>& vector of string arguments to be translated
     * @return the translated request object
     */
    int interprete(Vec<std::string> &, Request&) override;

    /*! \public Client::process()
     * @implements the interface of CommandLineParser
     * @brief Public method processing input from stdin.
     * @param std::string& fixed string used for unit testing mocking stdin
     * @return Vector of strings containing output from stdin.
     */
    Request process(std::string &) override;

    void verifyUploadTest(Request &request, Metadata &meta) {
        setProperties(request, meta);
    }

private:

    /*! \private Cli::split
     *
     * @param input incoming unprocessed string
     * @param delim delimiter used to split @param input
     * @return @param input splitted into multiple strings saved in a std::vector
     */
    Vec<std::string> split(std::string &input, char delim) {
        std::stringstream m_stream(input);
        Vec<std::string> seglist;
        std::string segment;
        seglist.reserve(50);
        seglist.clear();

        while(std::getline(m_stream, segment, delim))
        {
            seglist.push_back(std::move(segment));
        }
        return seglist;
    }


    /*! \private Cli::checkValidEvent
     * @brief check for valid event and cast to @enum Event
     * @param choice - event in string
     * @param event - interpreted event from cli input
     * @return - result of operation with @enum RET::OK or @enum RET::ERROR
     */
    int checkValidEvent(Vec<std::string> &input, Event &event){
        auto result = RET::ERROR;
        auto eventStr = input.front();
        std::transform(eventStr.begin(), eventStr.end(), eventStr.begin(), ::tolower);

        auto it = std::find(EVENT_ARGS.begin(), EVENT_ARGS.end(), eventStr);
        if(it != EVENT_ARGS.end())
        {
            long val = std::distance(EVENT_ARGS.begin(),it);
            event = castTypeToEvent(val);
            pop_front(input);
            result = RET::OK;
        }
        return result;
    }

    /*! \private Cli::checkValidCategory
     *  @brief check for valid category and cast to @enum Category
     * @param input stdin result
     * @param category undefined category that is being updated accordingly
     * @return result of operation with @enum RET::OK or @enum RET::ERROR
     */
    int checkValidCategory(Vec<std::string> &input, Category &category){
        auto result = RET::ERROR;
        auto vec = {MOVIE, SERIES};
        auto str = input.front();
        std::transform(str.begin(), str.end(), str.begin(), ::tolower);
        auto it = std::find(vec.begin(), vec.end(), str);
        if(it != vec.end())
        {
            long val = std::distance(vec.begin(),it);

            category = castTypeToCategory(val);
            pop_front(input);
            result = RET::OK;
        }
        return result;
    }

    /*! \private Cli::checkValidType
     * @brief check for each event @enum Event that corresponding type can be found
     * @param request object to update
     * @param type type to validate
     * @return result of operation with @enum RET::OK or @enum RET::ERROR
     */
    int checkValidType(Request &request, std::string &type) {
        Event e = request.getEvent();

        if(e == Event::UPLOAD) {
            auto found = type.find(FILENAME);
            if(found == std::string::npos) {
                return RET::ERROR;
            }
        }
        else if (e == Event::DOWNLOAD || e == Event::DELETE) {
            auto found = type.find(TITLE);
            if(found == std::string::npos) {
                return RET::ERROR;
            }
        }
        else if (e == Event::SEARCH) {
            auto vec = {TITLE, GENRE, DIRECTOR, ACTOR};
            auto found = std::find(vec.begin(), vec.end(), type);
            if(found == vec.end()) {
                return RET::ERROR;
            }
        }
        else {
            return RET::ERROR;
        }
        return RET::OK;
    }

    /*! \private Cli::verifyObjectExists
     * @brief verify that the object in question is located in the database for processing
     * @param category used to filter search
     * @param value value to search for
     * @return result of operation with @enum RET::OK or @enum RET::ERROR
     */
    int verifyObjectExists(Category &category, const std::string &value) {
        auto found =  JsonParser::getInstance().find(category, value);

        if(!found) { return RET::ERROR; }

        return RET::OK;
    }

    /*! \private Cli::getType
     * @brief get valid type and strip from input vector @param input
     * @param input stdin input
     * @param type type to process
     * @return result of operation with @enum RET::OK or @enum RET::ERROR
     */
    int getTypeOfValue(Vec<std::string> &input, std::string &type) {
        type = input.front();

        std::transform(type.begin(), type.end(), type.begin(), ::tolower);
        if(type == TITLE || type == GENRE || type == ACTOR || type == DIRECTOR || type == FILENAME) {
            pop_front(input);
            return RET::OK;
        }
        type = "";
        return RET::ERROR;
    }

    /*! \private Cli::getValueOfType
     * @brief fetch value from stdin and quickly evaluate it
     * @param input stdin
     * @param val value fetched
     * @return result of operation with @enum RET::OK or @enum RET::ERROR
     */
    int getValueOfType(Vec<std::string> &input, std::string &val) {

        if(input.size() > 1) {
            return RET::ERROR;
        }else {
            val = input.front();
        }

        if(val.empty()) {
            return RET::ERROR;
        }
        pop_front(input);
        return RET::OK;
    }

    /*! \private Cli::setFileName
     * @brief set @property filename in request object
     * @param request object to update
     * @param val value to save to @property filename
     */
    void setFileName(Request &request, std::string& val) {
        std::string path = "../data/";
        val = path + val;
        request.set(FILENAME, val);
    }

    /*! \private Cli::setProperties
     * @brief set @property title @property genre
     * @param request
     * @param item
     */
    void setProperties(Request &request, Metadata meta) {

        for(auto &a : meta.m_actors) {
            request.set(ACTOR, a);
        }
        request.set(DIRECTOR, meta.m_director);
        request.set(GENRE, meta.m_genre);
        request.set(TITLE, meta.m_title);
    }

    /*! \private Cli::verifyUpload
     * @brief receive and verify extra info for upload
     * @param request object to save info in
     * @return result of operation with @enum RET::OK or @enum RET::ERROR
     */
    int verifyUpload(Request &request) {

        const auto filename = request.get(FILENAME);
        if (filename.empty()) { return false; }
        if(access( filename.c_str(), F_OK ) == RET::ERROR) {
            request.setError(RET::ERROR);
            request.setErrorDesc("File not found");
            return RET::ERROR;
        }
        std::cout << "To confirm your upload add the following info: \n"
                     "<title> <genre> <director> {<actor> <actor>.. } " << std::endl;

        std::string info;
        std::string answer;

        std::getline(std::cin, info);
        auto parsedInfo = split(info, ' ');
        std::cout << "Please confirm <title> " << parsedInfo.front() <<
                  " <genre> " << parsedInfo.at(1) << " <director> " << parsedInfo.at(2) << " and the following actors: " << std::endl;

        for (auto it = parsedInfo.begin() + 3; it != parsedInfo.end(); ++it){
            std::cout << *it << " " << std::endl;
        }

        std::cout << "\n y or n? ";
        std::getline(std::cin, answer);
        std::transform(answer.begin(), answer.end(), answer.begin(), ::tolower);
        if(answer == "n"){
            return RET::ERROR;
        }

        request.set(TITLE, parsedInfo.front());
        request.set(GENRE, parsedInfo.at(1));
        request.set(DIRECTOR, parsedInfo.at(2));
        pop_front(parsedInfo);
        pop_front(parsedInfo);
        pop_front(parsedInfo);
        for(auto &a : parsedInfo) {
            request.set(ACTOR, a);
        }
        return RET::OK;
    }

    /*! \private Cli::printOptions
    *   @brief Test A private method that prints all valid options for stdin.
    */
    void printOptions() {
        std::cout << "\n" << std::endl;

        auto choice {"<choice> = upload, download, search, delete\n"};

        auto upload = "<upload>:<movie/series>:<filename> \n\t*<filename> - absolute path to filename. \n";
        auto cmfupload = "\t To confirm your upload add the following info: \n\t <title> <genre> {<actor> <actor>.. } <director>\n\n";

        auto download = "<download>:<movie/series>:title:<title> \n";
        auto cfmdownload = "\t Title exists or did you mean 'abc'? \n\t Download completed\n\n";

        auto search {"<search>:<movie/series>:actor:Sandra Bullock. \n \t "};
        auto cfmsearch = "\t Found the following matches..\n\n";
        auto search2 {"<search>:<movie/series>:genre:Romance. \n \t "};

        auto deleted {"\n<delete>:<movie/series><title>\n"};
        auto cfmdelete = "\t Sure you want to delete, Y or n? \n\n";

        auto disconnect = "Write 'exit' to disconnect\n";

        std::cout << choice << "\nExample input..\n\n" << upload << cmfupload << download << cfmdownload << search
                  << cfmsearch << search2 << cfmsearch << deleted << cfmdelete << std::endl;
        std::cout << disconnect << std::endl;
    }
};

#endif //MEDIAFW_CLI_H
