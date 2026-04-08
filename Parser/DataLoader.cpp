#include "DataLoader.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_set>
using namespace std;

typedef enum { None, Krasnale, Kopalnie, Straznicy } Section;


InputData DataLoader::loadFromFile(const std::string& filePath) const
{
    ifstream file(filePath);
    if (!file.is_open())
    {
        throw runtime_error("Failed to open file");
    }

    Section currentSection = None;
    InputData data;

    string line;
    while (getline(file, line))
    {
        if (line.empty()) continue;

        if (line.find("KRASNALE") == 0)
        {
            currentSection = Krasnale;
            continue;
        }
        if (line.find("KOPALNIE") == 0)
        {
            currentSection = Kopalnie;
            continue;
        }
        if (line.find("STRAZNICY") == 0)
        {
            currentSection = Straznicy;
            continue;
        }

        //WCZYTYWANIE DANYCH KRASNALA
        if (currentSection == Krasnale)
        {
            istringstream iss(line);
            int id;
            float x, y;
            string ulubionySurowiec;
            int liczbaKompetencji;

            if (!(iss >> id >> x >> y >> ulubionySurowiec >> liczbaKompetencji))
            {
                throw runtime_error("Failed to parse line");
            }


            if (liczbaKompetencji < 0)
            {
                throw runtime_error("Negative liczbaKompetencji");
            }

            vector<string> kompetencje;
            for (int i = 0; i < liczbaKompetencji; i++)
            {
                string temp_kompetencje;
                if (!(iss >> temp_kompetencje))
                {
                    throw runtime_error("Failed to parse line");
                }

                    kompetencje.push_back(temp_kompetencje);
            }

            data.krasnale.emplace_back(id, Point(x,y), kompetencje, ulubionySurowiec, false);

        }

        //WCZYTYWANIE DANYCH KOPALNI
        if (currentSection == Kopalnie)
        {
            istringstream iss(line);
            int id;
            float x, y;
            string typSurowca;
            int pojemnosc;

            if (!(iss >> id >> x >> y >> typSurowca >> pojemnosc))
            {
                throw runtime_error("Failed to parse line");
            }

            if (pojemnosc < 0)
            {
                throw runtime_error("Negative pojemnosc");
            }

            data.kopalnie.emplace_back(id, Point(x,y), typSurowca, pojemnosc, vector<int>{});

        }
        //WCZYTYWANIE DANYCH STRAZNIKOW
        if (currentSection == Straznicy)
        {
            istringstream iss(line);
            int id;
            int glosnosc;

            if (!(iss >> id  >> glosnosc))
            {
                throw runtime_error("Failed to parse line");
            }

            if (glosnosc < 0)
            {
                throw runtime_error("Negative glosnosc");
            }

            data.straznicy.emplace_back(id, glosnosc);
        }

    }

    return data;

}