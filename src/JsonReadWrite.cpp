//
// Created by mjonsson on 5/20/19.
//

#include "JsonReadWrite.h"


void JsonReadWrite::writeStream() {
    Json::Value rootItem;
    Json::Value addItem;
    std::ifstream inFile(m_path, std::ifstream::binary);
    inFile >> rootItem;
    inFile.close();

    addItem["testItem"] = "value";
    addItem["testItem"] = "value2";
    rootItem.append(addItem);

    addItem.clear();
    addItem["testItem2"] = "value";
    addItem["testItem2"] = "value2";
    rootItem.append(addItem);

    std::ofstream outFile(m_path, std::ios::trunc);
    outFile << rootItem;
    outFile.close();
}

void JsonReadWrite::readStream(std::string&) {
    Json::Value rootItem;
    std::ifstream inFile(m_path, std::ifstream::binary);
    inFile >> rootItem;
    inFile.close();


    for (Json::ArrayIndex i = 0; rootItem.isValidIndex(i); i++) {
        auto t = rootItem[i].type();
    }
}
