#ifndef INPUT_DATA_H
#define INPUT_DATA_H

#include "../Struktury/Kopalnia.h"
#include "../Struktury/Krasnal.h"
#include "../Struktury/Straznik.h"

#include <vector>

struct InputData {
    std::vector<Krasnal> krasnale;
    std::vector<Kopalnia> kopalnie;
    std::vector<Straznik> straznicy;
};

#endif
