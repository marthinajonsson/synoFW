//
// Created by mjonsson on 1/25/19.
//

#ifndef SYNOFW_METHODS_H
#define SYNOFW_METHODS_H

#include <string>

namespace VS {

    std::string getinfo = "getinfo";

    namespace movie {
        std::string list = "list";
        std::string search = "search";
        std::string getinfo = "getinfo";
        std::string edit = "edit";
        std::string deletemovie = "delete";
        std::string setwatched = "set_watched";
        std::string setrating = "set_rating";
    }

    namespace series {
        std::string list = "list";
        std::string search = "search";
        std::string getinfo = "getinfo";
        std::string edit = "edit";
        namespace episode {
            std::string deletemovie = "delete";
            std::string setwatched = "set_watched";
            std::string setrating = "set_rating";
        }
    }

    namespace metadata {
        std::string list = "list";
    }

    namespace streaming {
        std::string open = "open";
        std::string stream = "stream";
        std::string close = "close";
    }

    namespace subtitle {
        std::string list = "list";
        std::string get = "get";
        std::string search = "search";
        std::string download = "download";
        std::string discover = "discover";
    }

    namespace folder {
        std::string list = "list"
    }
}

#endif //SYNOFW_METHODS_H
