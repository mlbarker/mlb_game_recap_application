#include <iostream>
#include <sstream>
#include <fstream>
#include "MlbGameRecap.h"

MlbGameRecap::MlbGameRecap(std::string filename)
    : m_filename(filename)
    , m_loadedRecapData()
    , m_jsonContents()
    , m_games()
    , m_totalGames(0)
{
}

MlbGameRecap::~MlbGameRecap()
{
}

void MlbGameRecap::RetrieveGameRecapData()
{
    LoadFileContentsIntoBuffer();
    StoreRecapData();
    ParseTotalGamesToRecap();
    ParseRecapData();
}

void MlbGameRecap::GetGameRecapDataByIndex(uint32_t index, Game& game) const
{
    if (index < m_games.size())
    {
        game = m_games[index];
    }
}

void MlbGameRecap::GetGameRecapAllGames(std::vector<Game>& games) const
{
    games = m_games;
}

uint32_t MlbGameRecap::GetNumberOfGames() const
{
    return m_totalGames;
}

void MlbGameRecap::Log() const
{
    std::cout << '\n';

    for (auto game : m_games)
    {
        std::cout << game.date.c_str() << std::endl;
        std::cout << game.venue.c_str() << std::endl;
        std::cout << game.away.name.c_str() << " " << game.away.score << std::endl;
        std::cout << game.home.name.c_str() << " " << game.home.score << std::endl;
        std::cout << "Win: " << game.winner.c_str() << std::endl;
        std::cout << "Lose: " << game.loser.c_str() << std::endl;

        if (!game.save.empty())
        {
            std::cout << "Save: " << game.save.c_str() << std::endl;
        }

        std::cout << '\n';
    }
}

void MlbGameRecap::LoadFileContentsIntoBuffer()
{
    // filename was provided
    if (!m_filename.empty())
    {
        m_jsonContents.clear();

        std::ifstream file(m_filename);
        std::string inputLine;

        // load the file into the stringstream buffer
        while (std::getline(file, inputLine))
        {
            m_jsonContents << inputLine << "\n";
        }

        file.close();
    }

    else
    {
        std::cerr << "ERROR - no filename provided" << std::endl;
    }
}

void MlbGameRecap::StoreRecapData()
{
    m_loadedRecapData.clear();
    m_loadedRecapData = nlohmann::json::parse(m_jsonContents.str().c_str());
}

void MlbGameRecap::ParseTotalGamesToRecap()
{
    // total games to recap
    if (!m_loadedRecapData["totalGames"].is_number())
    {
        std::cerr << "ERROR: total games is not a number" << std::endl;
    }
    else
    {
        m_totalGames = m_loadedRecapData["totalGames"].get<uint32_t>();
    }
}

void MlbGameRecap::ParseRecapData()
{
    for (size_t index = 0; index < m_totalGames; ++index)
    {
        Game game;

        // game info
        // date
        if (!m_loadedRecapData["dates"][0]["date"].is_string())
        {
            std::cerr << "unable to retrieve game " << index + 1 << " data : date" << std::endl;
        }
        else
        {
            game.date = m_loadedRecapData["dates"][0]["date"].get<std::string>();
        }

        // winner (pitcher)
        if (!m_loadedRecapData["dates"][0]["games"][index]["decisions"]["winner"]["fullName"].is_string())
        {
            std::cerr << "unable to retrieve game " << index + 1 << " data : winning pitcher" << std::endl;
        }
        else
        {
            game.winner = m_loadedRecapData["dates"][0]["games"][index]["decisions"]["winner"]["fullName"].get<std::string>();
        }

        // loser (pitcher)
        if (!m_loadedRecapData["dates"][0]["games"][index]["decisions"]["loser"]["fullName"].is_string())
        {
            std::cerr << "unable to retrieve game " << index + 1 << " data : losing pitcher" << std::endl;
        }
        else
        {
            game.loser = m_loadedRecapData["dates"][0]["games"][index]["decisions"]["loser"]["fullName"].get<std::string>();
        }

        // save (pitcher)
        if (!m_loadedRecapData["dates"][0]["games"][index]["decisions"]["save"]["fullName"].is_string())
        {
            std::cerr << "unable to retrieve game " << index + 1 << " data : save pitcher" << std::endl;
            std::cerr << "no saves recorded" << std::endl;
        }
        else
        {
            game.save = m_loadedRecapData["dates"][0]["games"][index]["decisions"]["save"]["fullName"].get<std::string>();
        }

        // venue
        if (!m_loadedRecapData["dates"][0]["games"][index]["venue"]["name"].is_string())
        {
            std::cerr << "unable to retrieve game " << index + 1 << " data : losing pitcher" << std::endl;
        }
        else
        {
            game.venue = m_loadedRecapData["dates"][0]["games"][index]["venue"]["name"].get<std::string>();
        }

        // away team
        // wins
        if (!m_loadedRecapData["dates"][0]["games"][index]["teams"]["away"]["leagueRecord"]["wins"].is_number())
        {
            std::cerr << "unable to retrieve game " << index + 1 << " data : wins" << std::endl;
        }
        else
        {
            game.away.wins = m_loadedRecapData["dates"][0]["games"][index]["teams"]["away"]["leagueRecord"]["wins"].get<uint32_t>();
        }

        // losses
        if (!m_loadedRecapData["dates"][0]["games"][index]["teams"]["away"]["leagueRecord"]["losses"].is_number())
        {
            std::cerr << "unable to retrieve game " << index + 1 << " data : losses" << std::endl;
        }
        else
        {
            game.away.losses = m_loadedRecapData["dates"][0]["games"][index]["teams"]["away"]["leagueRecord"]["losses"].get<uint32_t>();
        }

        // percentage
        if (!m_loadedRecapData["dates"][0]["games"][index]["teams"]["away"]["leagueRecord"]["pct"].is_string())
        {
            std::cerr << "unable to retrieve game " << index + 1 << " data : pct" << std::endl;
        }
        else
        {
            game.away.percentage = m_loadedRecapData["dates"][0]["games"][index]["teams"]["away"]["leagueRecord"]["pct"].get<std::string>();
        }

        // score
        if (!m_loadedRecapData["dates"][0]["games"][index]["teams"]["away"]["score"].is_number())
        {
            std::cerr << "unable to retrieve game " << index + 1 << " data : score" << std::endl;
        }
        else
        {
            game.away.score = m_loadedRecapData["dates"][0]["games"][index]["teams"]["away"]["score"].get<uint32_t>();
        }

        // team name
        if (!m_loadedRecapData["dates"][0]["games"][index]["teams"]["away"]["team"]["name"].is_string())
        {
            std::cerr << "unable to retrieve game " << index + 1 << " data : team name" << std::endl;
        }
        else
        {
            game.away.name = m_loadedRecapData["dates"][0]["games"][index]["teams"]["away"]["team"]["name"].get<std::string>();
        }

        // home team
        // wins
        if (!m_loadedRecapData["dates"][0]["games"][index]["teams"]["home"]["leagueRecord"]["wins"].is_number())
        {
            std::cerr << "unable to retrieve game " << index + 1 << " data : home wins" << std::endl;
        }
        else
        {
            game.home.wins = m_loadedRecapData["dates"][0]["games"][index]["teams"]["home"]["leagueRecord"]["wins"].get<uint32_t>();
        }

        // losses
        if (!m_loadedRecapData["dates"][0]["games"][index]["teams"]["home"]["leagueRecord"]["losses"].is_number())
        {
            std::cerr << "unable to retrieve game " << index + 1 << " data : home losses" << std::endl;
        }
        else
        {
            game.home.losses = m_loadedRecapData["dates"][0]["games"][index]["teams"]["home"]["leagueRecord"]["losses"].get<uint32_t>();
        }

        // percentage
        if (!m_loadedRecapData["dates"][0]["games"][index]["teams"]["home"]["leagueRecord"]["pct"].is_string())
        {
            std::cerr << "unable to retrieve game " << index + 1 << " data : home pct" << std::endl;
        }
        else
        {
            game.home.percentage = m_loadedRecapData["dates"][0]["games"][index]["teams"]["home"]["leagueRecord"]["pct"].get<std::string>();
        }

        // score
        if (!m_loadedRecapData["dates"][0]["games"][index]["teams"]["home"]["score"].is_number())
        {
            std::cerr << "unable to retrieve game " << index + 1 << " data : home score" << std::endl;
        }
        else
        {
            game.home.score = m_loadedRecapData["dates"][0]["games"][index]["teams"]["home"]["score"].get<uint32_t>();
        }

        // team name
        if (!m_loadedRecapData["dates"][0]["games"][index]["teams"]["home"]["team"]["name"].is_string())
        {
            std::cerr << "unable to retrieve game " << index + 1 << " data : home team name" << std::endl;
        }
        else
        {
            game.home.name = m_loadedRecapData["dates"][0]["games"][index]["teams"]["home"]["team"]["name"].get<std::string>();
        }

        // store it!
        m_games.push_back(game);
    }
}
