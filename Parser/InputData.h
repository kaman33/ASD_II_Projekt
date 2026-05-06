#ifndef INPUT_DATA_H
#define INPUT_DATA_H

#include "../Struktury/Kopalnia.h"
#include "../Struktury/Krasnal.h"
#include "../Struktury/Straznik.h"

#include <vector>

struct InputData {
    std::vector<Krasnal> dwarves;
    std::vector<Kopalnia> mines;
    std::vector<Straznik> guards;
};

#endif
