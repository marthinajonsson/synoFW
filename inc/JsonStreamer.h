//
// Created by mjonsson on 5/20/19.
//

#ifndef SYNOFW_JSONREADWRITE_H
#define SYNOFW_JSONREADWRITE_H

#include <boost/property_tree/json_parser.hpp>
#include <string>
#include <map>


template <class Type, class Enum, class Keys = std::map<std::string, std::string>, class ValueType = std::string>
class JsonStreamer {
private:
    Keys _Keys;
    std::string _path = "../data/";

    JsonStreamer () = default;
    void writeStream(Type &write_obj);
    boost::property_tree::ptree readStream(ValueType& search_pattern);
public:

    JsonStreamer (const std::string &&filename, Keys &keys) : _Keys(keys) {
        _path = _path.append(filename);
    };

    ~JsonStreamer() = default;

    Type find(ValueType &search_pattern);

    void update(Type &obj) {
        writeStream(obj);
    }

    bool exists (ValueType& search_pattern) {
        auto item = readStream(search_pattern);
        return item.empty();
    }
};


#endif //SYNOFW_JSONREADWRITE_H
