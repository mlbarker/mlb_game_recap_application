#ifndef _MLBGAMERECAP_H
#define _MLBGAMERECAP_H

#include <vector>
#include <memory>
#include "json.hpp"
#include "game.h"

class MlbGameRecap
{
public:
    MlbGameRecap(std::string filename);
    ~MlbGameRecap();

    void RetrieveGameRecapData();
    void GetGameRecapDataByIndex(uint32_t index, Game& game) const;
    void GetGameRecapAllGames(std::vector<Game>& games) const;
    uint32_t GetNumberOfGames() const;
    void Log() const;

private:
    MlbGameRecap();

    void LoadFileContentsIntoBuffer();
    void StoreRecapData();
    void ParseTotalGamesToRecap();
    void ParseRecapData();

    std::stringstream m_jsonContents;
    nlohmann::json m_loadedRecapData;
    std::string m_filename;
    std::vector<Game> m_games;
    uint32_t m_totalGames;
};

#endif
