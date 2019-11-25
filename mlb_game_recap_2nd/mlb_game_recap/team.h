#ifndef _TEAM_H
#define _TEAM_H

#include <string>

struct Team
{
    uint32_t score;
    uint32_t wins;
    uint32_t losses;
    std::string percentage;
    std::string name;
    bool winner;

    void clear()
    {
        score = wins = losses = 0;
        percentage.clear();
        name.clear();
        winner = false;
    }
};

#endif
