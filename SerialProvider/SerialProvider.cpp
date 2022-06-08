// SerialProvider.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//

#include <iostream>
#include "DatabaseProvider.h"
#include "BinaryAnalyzer.h"
using namespace std;

int main()
{
    // Main thread
    database_provider::DatabaseProvider data;

    // Get file from Github and verify if it present
    bool validGet = data.getFile();
    string fileLocation = data.getFileLocation();
    // Start analyzing binary
    binary_analyzer::SerialCreator serialCreator(fileLocation);
    binary_analyzer::BinaryAnalyzer binaryAnalyzer(fileLocation);

    // Database status
    if (validGet)
    {
        cout << "Succesfully downloaded newest version from database" << endl;
    }
    else
    {
        cout << "Unuccesfully downloaded newest version from database" << endl;
    }

    if (!binaryAnalyzer.getBinaryFile()) { return 0; };

    // Get last serial
    string last = serialCreator.getLastSerial();

    // find pattern
    int index = binaryAnalyzer.findPattern(last, serialCreator.baseList());

    if (index == 0)
    {
        cout << "Cannot find patter" << endl;
        cout << "Terminate process" << endl;
        getchar();
        return 0;
    }

    //Create & Save serial
    string serialLast = serialCreator.createSerial(last);

    //Replace serial number
    binaryAnalyzer.pushSerial(index, serialLast);

    // Commit file to database
    data.pushFile();


    // Debug key only
    //getchar();


    /*auto provider = [this](std::string file_location) {
        system("git pull");
        std::future<bool> fut = std::async(std::launch::async, [this, file_location] {return fileExist(file_location); });
        return fut.get();
    };

    std::future<bool> fut = std::async(std::launch::async, provider, file_location);
    return fut.get();*/

}

// Uruchomienie programu: Ctrl + F5 lub menu Debugowanie > Uruchom bez debugowania
// Debugowanie programu: F5 lub menu Debugowanie > Rozpocznij debugowanie

// Porady dotyczące rozpoczynania pracy:
//   1. Użyj okna Eksploratora rozwiązań, aby dodać pliki i zarządzać nimi
//   2. Użyj okna programu Team Explorer, aby nawiązać połączenie z kontrolą źródła
//   3. Użyj okna Dane wyjściowe, aby sprawdzić dane wyjściowe kompilacji i inne komunikaty
//   4. Użyj okna Lista błędów, aby zobaczyć błędy
//   5. Wybierz pozycję Projekt > Dodaj nowy element, aby utworzyć nowe pliki kodu, lub wybierz pozycję Projekt > Dodaj istniejący element, aby dodać istniejące pliku kodu do projektu
//   6. Aby w przyszłości ponownie otworzyć ten projekt, przejdź do pozycji Plik > Otwórz > Projekt i wybierz plik sln
