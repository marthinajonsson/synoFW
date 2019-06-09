//
// Created by mjonsson on 6/9/19.
//

#ifndef SYNOFW_TABLEPRINTER_H
#define SYNOFW_TABLEPRINTER_H

#include "DatabaseObject.h"
#include "Utilities.h"
#include <sstream>
#include <iostream>

class TablePrinter {
private:
    int index = 0;
    int _tableSize = 0;
    int _sumColumnSize = 0;
    int _columnSize = 30;
    int _columnSizeMedium = 20;
    int _columnSizeSmall = 10;
    int _columnSizeXL = 50;
    std::stringstream _stream;
    const std::string _rowSeparator = "|";
    std::vector<std::pair<int, int>> _columnSizes;
    void horisontal();
public:
    TablePrinter(int&& size) :_tableSize(size) {
        index = 1;
    }
    ~TablePrinter() {}

    void addCell(std::string&&, int size = 30);
    void addHeader(std::vector<std::string>&&);
    void addRow(std::vector<std::string>&&);
    void setRowColor(std::string&&);
    void write();
};

#endif //SYNOFW_TABLEPRINTER_H

