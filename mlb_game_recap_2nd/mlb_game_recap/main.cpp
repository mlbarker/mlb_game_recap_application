// mlb_game_recap.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <memory>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include "json.hpp"
#include "SdlWrapper.h"
#include "MlbGameRecap.h"
#include "Constants.h"

int main(int argc, char* argv[])
{
    // load json
    MlbGameRecap gameRecap("schedule.json");
    gameRecap.RetrieveGameRecapData();
    gameRecap.Log();

    // get the games
    std::vector<Game> games;
    gameRecap.GetGameRecapAllGames(games);

    // SDL stuff for later
    SdlWrapper sdl;
    int32_t result{ 0 };

    result = sdl.Initialize("MLB Game Recap");
    if (result)
    {
        // something went wrong
        return result;
    }

    sdl.LoadMedia("background.jpg", false);

    for (size_t index = 0; index < games.size(); ++index)
    {
        //std::string imagePath = "game_images\\";
        char buffer[Constants::BUFFER_SIZE];
        sprintf_s<Constants::BUFFER_SIZE>(buffer, "game_images\\%d\\cut.jpg", index);

        if (index == 0)
        {
            result = sdl.LoadMedia(buffer, true);
        }
        else
        {
            result = sdl.LoadMedia(buffer, true, 100, 0);
        }
        if (result)
        {
            // something went wrong
            return result;
        }
    }

    result = sdl.Run();
    if (result)
    {
        // something went wrong
        return result;
    }

    result = sdl.Shutdown();
    return result;
}
