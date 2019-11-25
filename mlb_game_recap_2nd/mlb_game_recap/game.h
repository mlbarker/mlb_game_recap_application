#ifndef _GAME_H
#define _GAME_H_

#include <string>
#include "team.h"

struct Game
{
    Team away;
    Team home;

    std::string date;
    std::string winner;
    std::string loser;
    std::string save;
    std::string venue;

    void clear()
    {
        away.clear();
        home.clear();
        winner.clear();
        loser.clear();
        save.clear();
        venue.clear();
    }
};

#endif
