#include "../DataLoader.h"
#include<iostream>
#include<fstream>
#include<cstdio>
#include<cmath>
#include<vector>

bool createFile(const std::string& fileName, const std::string& content){
    std::ofstream file(fileName.c_str());

    if(!file.is_open()){
        return false;
    }

    file<<content;
    return true;
}

bool compareDouble(double a, double b){
    return std::fabs(a - b) < 0.000000001;
}

bool test1(){
    const std::string fileName = "test_data_loader_1.txt";

    if(!createFile(fileName,
        "KRASNALE\n"
        "1 0 0 zloto 2 zloto miedz\n"
        "2 8 1 wegiel 1 wegiel\n"
        "\n"
        "KOPALNIE\n"
        "10 2 2 zloto 1\n"
        "11 9 2 wegiel 2\n"
        "\n"
        "STRAZNICY\n"
        "100 14\n"
        "101 9\n")){
        return false;
    }

    try{
        DataLoader loader;
        InputData data = loader.loadFromFile(fileName);
        std::remove(fileName.c_str());

        return data.dwarves.size() == 2 &&
               data.mines.size() == 2 &&
               data.guards.size() == 2;
    } catch(...){
        std::remove(fileName.c_str());
        return false;
    }
}

bool test2(){
    const std::string fileName = "test_data_loader_2.txt";

    if(!createFile(fileName,
        "KRASNALE\n"
        "1 -3.5 4.25 zloto 2 zloto miedz\n")){
        return false;
    }

    try{
        DataLoader loader;
        InputData data = loader.loadFromFile(fileName);
        std::remove(fileName.c_str());

        if(data.dwarves.size() != 1){
            return false;
        }

        Krasnal dwarf = data.dwarves[0];
        const std::vector<std::string> expectedSkills = {"zloto", "miedz"};

        return dwarf.getId() == 1 &&
               compareDouble(dwarf.getHomeX(), -3.5) &&
               compareDouble(dwarf.getHomeY(), 4.25) &&
               dwarf.getPreferredResource() == "zloto" &&
               dwarf.getSkills() == expectedSkills &&
               dwarf.isAssignedToPreferredResource() == false;
    } catch(...){
        std::remove(fileName.c_str());
        return false;
    }
}

bool test3(){
    const std::string fileName = "test_data_loader_3.txt";

    if(!createFile(fileName,
        "KOPALNIE\n"
        "10 2.5 -1.25 zloto 3\n")){
        return false;
    }

    try{
        DataLoader loader;
        InputData data = loader.loadFromFile(fileName);
        std::remove(fileName.c_str());

        if(data.mines.size() != 1){
            return false;
        }

        Kopalnia mine = data.mines[0];

        return mine.getId() == 10 &&
               compareDouble(mine.getX(), 2.5) &&
               compareDouble(mine.getY(), -1.25) &&
               mine.getResourceType() == "zloto" &&
               mine.getCapacity() == 3 &&
               mine.getAssignedDwarves().empty();
    } catch(...){
        std::remove(fileName.c_str());
        return false;
    }
}

bool test4(){
    const std::string fileName = "test_data_loader_4.txt";

    if(!createFile(fileName,
        "STRAZNICY\n"
        "100 14\n")){
        return false;
    }

    try{
        DataLoader loader;
        InputData data = loader.loadFromFile(fileName);
        std::remove(fileName.c_str());

        if(data.guards.size() != 1){
            return false;
        }

        Straznik guard = data.guards[0];

        return guard.getId() == 100 &&
               guard.getLoudness() == 14;
    } catch(...){
        std::remove(fileName.c_str());
        return false;
    }
}

bool test5(){
    try{
        DataLoader loader;
        loader.loadFromFile("plik_ktory_nie_istnieje.txt");
    } catch(const std::runtime_error& e){
        return std::string(e.what()) == "Failed to open file";
    } catch(...){
        return false;
    }

    return false;
}

bool test6(){
    const std::string fileName = "test_data_loader_6.txt";

    if(!createFile(fileName,
        "KRASNALE\n"
        "1 0 0 zloto 2 zloto\n")){
        return false;
    }

    try{
        DataLoader loader;
        loader.loadFromFile(fileName);
    } catch(const std::runtime_error& e){
        std::remove(fileName.c_str());
        return std::string(e.what()) == "Failed to parse line";
    } catch(...){
        std::remove(fileName.c_str());
        return false;
    }

    std::remove(fileName.c_str());
    return false;
}

bool test7(){
    const std::string fileName = "test_data_loader_7.txt";

    if(!createFile(fileName,
        "KRASNALE\n"
        "1 0 0 zloto -1\n")){
        return false;
    }

    try{
        DataLoader loader;
        loader.loadFromFile(fileName);
    } catch(const std::runtime_error& e){
        std::remove(fileName.c_str());
        return std::string(e.what()) == "Negative skill count";
    } catch(...){
        std::remove(fileName.c_str());
        return false;
    }

    std::remove(fileName.c_str());
    return false;
}

bool test8(){
    const std::string fileName = "test_data_loader_8.txt";

    if(!createFile(fileName,
        "KOPALNIE\n"
        "10 2 2 zloto -1\n")){
        return false;
    }

    try{
        DataLoader loader;
        loader.loadFromFile(fileName);
    } catch(const std::runtime_error& e){
        std::remove(fileName.c_str());
        return std::string(e.what()) == "Negative capacity";
    } catch(...){
        std::remove(fileName.c_str());
        return false;
    }

    std::remove(fileName.c_str());
    return false;
}

bool test9(){
    const std::string fileName = "test_data_loader_9.txt";

    if(!createFile(fileName,
        "STRAZNICY\n"
        "100 -1\n")){
        return false;
    }

    try{
        DataLoader loader;
        loader.loadFromFile(fileName);
    } catch(const std::runtime_error& e){
        std::remove(fileName.c_str());
        return std::string(e.what()) == "Negative loudness";
    } catch(...){
        std::remove(fileName.c_str());
        return false;
    }

    std::remove(fileName.c_str());
    return false;
}

int main(){
    std::cout<<"Testy dla klasy DataLoader:"<<std::endl;
    std::cout<<"Test 1: "<<(test1()?"OK":"ERROR")<<" (liczba wczytanych danych)"<<std::endl;
    std::cout<<"Test 2: "<<(test2()?"OK":"ERROR")<<" (wczytanie krasnala)"<<std::endl;
    std::cout<<"Test 3: "<<(test3()?"OK":"ERROR")<<" (wczytanie kopalni)"<<std::endl;
    std::cout<<"Test 4: "<<(test4()?"OK":"ERROR")<<" (wczytanie straznika)"<<std::endl;
    std::cout<<"Test 5: "<<(test5()?"OK":"ERROR")<<" (brak pliku)"<<std::endl;
    std::cout<<"Test 6: "<<(test6()?"OK":"ERROR")<<" (bledna linia)"<<std::endl;
    std::cout<<"Test 7: "<<(test7()?"OK":"ERROR")<<" (ujemna liczba kompetencji)"<<std::endl;
    std::cout<<"Test 8: "<<(test8()?"OK":"ERROR")<<" (ujemna pojemnosc)"<<std::endl;
    std::cout<<"Test 9: "<<(test9()?"OK":"ERROR")<<" (ujemna glosnosc)"<<std::endl;

    return 0;
}
