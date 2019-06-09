//
// Created by mjonsson on 6/9/19.
//

#include <iomanip>
#include "TablePrinter.h"

void TablePrinter::write() {
    std::flush(std::cout);
    std::cout << _stream.str();
}

void TablePrinter::horisontal() {
    _stream << "\n";

    std::string str;
    for(auto i = 0; i<400; i++){
        str += "=";
    }
    str.resize(_sumColumnSize);
    _stream << str << "\n";
}

void TablePrinter::addCell(std::string&& value, int colSize) {
    value.resize(colSize,' ');
    _stream << _rowSeparator
    << value;
}

void TablePrinter::addHeader(std::vector<std::string>&&header) {
    horisontal();
    for(auto& h : header) {
        if(h.find("Title") != std::string::npos ) {
            addCell(std::move(h), _columnSize);
            _sumColumnSize+= _columnSize;
            _columnSizes.emplace_back(std::make_pair(_columnSizes.size(), _columnSize));
        }
        else if(h.find("Actors") != std::string::npos) {
            addCell(std::move(h), _columnSizeXL);
            _sumColumnSize+= _columnSizeXL;
            _columnSizes.emplace_back(std::make_pair(_columnSizes.size(), _columnSizeXL));
        }
        else if(h.find("Genre") != std::string::npos ||
            h.find("Directors") != std::string::npos) {
            addCell(std::move(h), _columnSizeMedium);
            _sumColumnSize+= _columnSizeMedium;
            _columnSizes.emplace_back(std::make_pair(_columnSizes.size(), _columnSizeMedium));
        }
        else {
            addCell(std::move(h), _columnSizeSmall);
            _sumColumnSize+= _columnSizeSmall;
            _columnSizes.emplace_back(std::make_pair(_columnSizes.size(), _columnSizeSmall));
        }
    }
}

void TablePrinter::setRowColor(std::string&& color)
{
    if (color.find("green") != std::string::npos) {
        _stream << green_on;
    } else if (color.find("red") != std::string::npos) {
        _stream << red_on;
    } else if (color.find("bold") != std::string::npos) {
        _stream << bold_on;
    } else {
        _stream << reset;
    }
}

void TablePrinter::addRow(std::vector<std::string> && row)
{
    horisontal();

    auto indexS = std::to_string(index);
    indexS.resize(_columnSizeSmall, ' ');
    _stream  << _rowSeparator << indexS;;
    index++;
    for(int i = 0; i < row.size(); i++) {
        auto sizeOfColumn = _columnSizes.at(i+1).second;
        std::string value = row.at(i);
        addCell(std::move(value), sizeOfColumn);
    }
}

