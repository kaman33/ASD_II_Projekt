#include "DataLoader.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_set>
using namespace std;

enum class Section { None, Dwarves, Mines, Guards };


InputData DataLoader::loadFromFile(const std::string& filePath) const
{
    ifstream file(filePath);
    if (!file.is_open())
    {
        throw runtime_error("Failed to open file");
    }

    Section currentSection = Section::None;
    InputData data;

    string line;
    while (getline(file, line))
    {
        if (line.empty()) continue;

        if (line.find("KRASNALE") == 0)
        {
            currentSection = Section::Dwarves;
            continue;
        }
        if (line.find("KOPALNIE") == 0)
        {
            currentSection = Section::Mines;
            continue;
        }
        if (line.find("STRAZNICY") == 0)
        {
            currentSection = Section::Guards;
            continue;
        }

        //WCZYTYWANIE DANYCH KRASNALA
        if (currentSection == Section::Dwarves)
        {
            istringstream iss(line);
            int id;
            double x, y;
            string preferredResource;
            int skillCount;

            if (!(iss >> id >> x >> y >> preferredResource >> skillCount))
            {
                throw runtime_error("Failed to parse line");
            }


            if (skillCount < 0)
            {
                throw runtime_error("Negative skill count");
            }

            vector<string> skills;
            for (int i = 0; i < skillCount; i++)
            {
                string skill;
                if (!(iss >> skill))
                {
                    throw runtime_error("Failed to parse line");
                }

                skills.push_back(skill);
            }

            data.dwarves.emplace_back(id, Point(x, y), skills, preferredResource, false);

        }

        //WCZYTYWANIE DANYCH KOPALNI
        if (currentSection == Section::Mines)
        {
            istringstream iss(line);
            int id;
            double x, y;
            string resourceType;
            int capacity;

            if (!(iss >> id >> x >> y >> resourceType >> capacity))
            {
                throw runtime_error("Failed to parse line");
            }

            if (capacity < 0)
            {
                throw runtime_error("Negative capacity");
            }

            data.mines.emplace_back(id, Point(x, y), resourceType, capacity, vector<int>{});

        }
        //WCZYTYWANIE DANYCH STRAZNIKOW
        if (currentSection == Section::Guards)
        {
            istringstream iss(line);
            int id;
            int loudness;

            if (!(iss >> id >> loudness))
            {
                throw runtime_error("Failed to parse line");
            }

            if (loudness < 0)
            {
                throw runtime_error("Negative loudness");
            }

            data.guards.emplace_back(id, loudness);
        }

    }

    return data;

}
