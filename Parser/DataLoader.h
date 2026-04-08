#ifndef DATA_LOADER_H
#define DATA_LOADER_H

#include "InputData.h"
#include <string>

class DataLoader {
public:
    InputData loadFromFile(const std::string& filePath) const;
};

#endif
