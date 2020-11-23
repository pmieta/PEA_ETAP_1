#include<iostream>
#include<fstream>
#include<sstream>
#include<string.h>
#include<windows.h>
#include <vector>
#include <algorithm>
#include <chrono>

using namespace std;
void readFromFile(string& instanceName, int& numberOfCities, int*** matrix, int& optimalValue);
void showMatrix(int** matrix, int numberOfCities);
void createOrder(int** matrix, int** order, int numberOfCities, int** tempOrder);
void bruteForce(int** matrix, int numberOfCities, int* order, int optimalValue, int krok, int* tempOrder, int& tempCycleLenght, int cycleLenght);
void swap(int v[], int i, int j);
void brute(int numberOfCities, int tempCycleLenght, int** matrix, int* order, int optimalValue, int* tempOrder, int cycleLenght);
void boundAndBranchAlgorithm(int** matrix, int numberOfCities, int optimalValue);
void boundAndBranch(int** matrix, int numberOfCities, int& upperBound, int* cityOrder, int level, int* tempCityOrder, int tempValue, int optimalValue);



int main() {
    string instanceName = "";
    int numberOfCities = 0;
    int** matrix = nullptr;
    int optimalValue = 0;
    int* order = nullptr;
    int* tempOrder = nullptr;
    int tempCycleLenght = 0;
    int cycleLenght = 0;
    readFromFile(instanceName, numberOfCities, &matrix, optimalValue);
    showMatrix(matrix, numberOfCities);
    cout << "wybierz algorytm" << endl << "1: brute force" << endl << "2: bound and branch" << endl << "3: zakoncz" << endl;
    int option;    
    cin >> option;
    std::chrono::high_resolution_clock::time_point begin;
    std::chrono::high_resolution_clock::time_point end;
    do {
        switch (option) {
        case 1:
            begin = chrono::high_resolution_clock::now();
            brute(numberOfCities, tempCycleLenght, matrix, order, optimalValue, tempOrder, cycleLenght);
            end = chrono::high_resolution_clock::now();
            cout << "Time difference = " << chrono::duration_cast<chrono::milliseconds> (end - begin).count() << "[ms]" << std::endl;
            break;
        case 2:
            begin = chrono::high_resolution_clock::now();
            boundAndBranchAlgorithm(matrix, numberOfCities, optimalValue);
            end = chrono::high_resolution_clock::now();
            cout << "Time difference = " << chrono::duration_cast<chrono::milliseconds> (end - begin).count() << "[ms]" << std::endl;
            break;
        case 3:
            break;
        default:
            cout << "niepoprawny wybór" << endl;


        }
    } while (option==3);
    
    system("PAUSE");
    return 0;
}
void readFromFile(string& instanceName, int& numberOfCities, int*** matrix, int& optimalValue) {
    ifstream file;
    string fileName;
    cout << "podaj nazwe pliku" << endl;
    cin >> fileName;
    fileName = "C:/Users/mateu/OneDrive/Desktop/pea/" +fileName;
    file.open(fileName, ios::in);
    if (file.good()) {
        cout << "udalo sie otworzyc plik" << endl;
        string word;
        file >> word;
        instanceName = word;
        
        file >> word;
        numberOfCities = stoi(word);
        
        (*matrix) = new int* [numberOfCities]; //tworze tablice
        for (int i = 0; i < numberOfCities; i++)
        {
            (*matrix)[i] = new int[numberOfCities];
            for (int j = 0; j < numberOfCities; j++)
            {
                file >> word;
                (*matrix)[i][j] = stoi(word);
                
            }
            
        }
        file >> word;
        optimalValue = stoi(word);
        
        file.close();
    }
    else {
        cout << "nie udalo siê otworzyc pliku" << endl;

    }
}
void showMatrix(int** matrix, int numberOfCities) {
    if (matrix != nullptr) {
        for (int i = 0; i < numberOfCities; i++)
        {
            for (int j = 0; j < numberOfCities; j++)
            {
                cout << matrix[i][j] << " ";
            }
            cout << endl;
        }
    }
    cout << endl;
}
void createOrder(int** matrix, int** order, int numberOfCities, int** tempOrder) {
    (*order) = new int[numberOfCities];
    if (*order != nullptr) {
        for (int i = 0; i < numberOfCities; i++) {
            (*order)[i] = i;
        }
    }
    (*tempOrder) = new int[numberOfCities];
    if (*tempOrder != nullptr) {
        for (int i = 0; i < numberOfCities; i++) {
            (*tempOrder)[i] = i;
        }
    }


}
void swap(int v[], int i, int j) {
    int	t;

    t = v[i];
    v[i] = v[j];
    v[j] = t;
}
void bruteForce(int** matrix, int numberOfCities, int* order, int optimalValue, int i, int* tempOrder, int& tempCycleLenght, int cycleLenght) {
    int j;
    
    if (i == numberOfCities) {

        for (int i = 0; i < numberOfCities - 1; i++) {
            cycleLenght += matrix[order[i]][order[i + 1]];
        }
        cycleLenght += matrix[order[numberOfCities - 1]][order[0]];
        if (cycleLenght < tempCycleLenght) {
            tempCycleLenght = cycleLenght;
            for (int i = 0; i < numberOfCities; i++) {
                tempOrder[i] = order[i];
            }
            cout << endl << tempCycleLenght << "  " << 100 * tempCycleLenght / optimalValue << "%";
        }


    }
    else

        for (j = i; j < numberOfCities; j++) {
            swap(order, i, j);
            
            bruteForce(matrix, numberOfCities, order, optimalValue, i + 1, tempOrder, tempCycleLenght, cycleLenght);

            swap(order, i, j);
        }
}
void brute(int numberOfCities, int tempCycleLenght, int** matrix, int* order, int optimalValue, int* tempOrder, int cycleLenght) {
    createOrder(matrix, &order, numberOfCities, &tempOrder);
    for (int i = 0; i < numberOfCities - 1; i++) {
        tempCycleLenght += matrix[order[i]][order[i + 1]];
    }
    tempCycleLenght += matrix[order[numberOfCities - 1]][order[0]];
    cout << endl << tempCycleLenght << "  " << 100 * tempCycleLenght / optimalValue << "%";
    if (matrix != nullptr)
        bruteForce(matrix, numberOfCities, order, optimalValue, 1, tempOrder, tempCycleLenght, cycleLenght);
    cout << endl;
    for (int j = 0; j < numberOfCities; j++) cout << tempOrder[j] << "->";
    cout << tempOrder[0] << endl;
    cout << endl;
}
void boundAndBranchAlgorithm(int** matrix, int numberOfCities, int optimalValue) {
    int upperBound = INT_MAX;
    int* cityOrder = new int[numberOfCities];
    int* tempCityOrder = new int[numberOfCities];
    int tempValue = 0;
    for (int i = 0; i < numberOfCities; i++)
    {
        tempCityOrder[i] = i;
    }
    
    cout << endl;
    boundAndBranch(matrix, numberOfCities, upperBound, cityOrder, 1, tempCityOrder, tempValue, optimalValue);
    cout << endl;
    for (int j = 0; j < numberOfCities; j++) cout << cityOrder[j] << "->";
    cout << cityOrder[0] << endl;
    cout << endl;


}
void boundAndBranch(int** matrix, int numberOfCities, int& upperBound, int* cityOrder, int level, int* tempCityOrder, int tempValue, int optimalValue) {

    if (level < numberOfCities - 2) {
        
        for (int j = level; j < numberOfCities; j++) {
            int nodeValue = tempValue;
            int lowerBound = 0;
            
            swap(tempCityOrder, level, j);
            
            int minRow = INT_MAX;
            for (int i = level + 1; i < numberOfCities; i++) {
                if (matrix[tempCityOrder[level]][tempCityOrder[i]] < minRow) {
                    minRow = matrix[tempCityOrder[level]][tempCityOrder[i]];
                }
            }
            
            lowerBound += minRow;
            swap(tempCityOrder, level, 0);
            for (int i = level + 1; i < numberOfCities; i++) {
                minRow = INT_MAX;
                for (int k = level; k < numberOfCities; k++) {
                    if (i != k && matrix[tempCityOrder[i]][tempCityOrder[k]] < minRow) {
                        minRow = matrix[tempCityOrder[i]][tempCityOrder[k]];
                    }
                }
                lowerBound += minRow;
            }
            
            swap(tempCityOrder, level, 0);
            nodeValue += matrix[tempCityOrder[level - 1]][tempCityOrder[level]];
            lowerBound += nodeValue;
            if (lowerBound < upperBound)
                boundAndBranch(matrix, numberOfCities, upperBound, cityOrder, level + 1, tempCityOrder, nodeValue, optimalValue);
        

            swap(tempCityOrder, level, j);
            
        }
    }
    else if (level == numberOfCities - 2) {
        int pomValue = tempValue;
        tempValue += matrix[tempCityOrder[level - 1]][tempCityOrder[level]];
        tempValue += matrix[tempCityOrder[level]][tempCityOrder[level + 1]] + matrix[tempCityOrder[level + 1]][tempCityOrder[0]];
        if (tempValue < upperBound) {
            upperBound = tempValue;
            cout << endl << tempValue << "  " << 100 * tempValue / optimalValue << "%";
            for (int i = 0; i < numberOfCities; i++) {
                cityOrder[i] = tempCityOrder[i];
            }
        }
        swap(tempCityOrder, numberOfCities - 2, numberOfCities - 1);
        pomValue += matrix[tempCityOrder[level - 1]][tempCityOrder[level]];
        pomValue += matrix[tempCityOrder[level]][tempCityOrder[level + 1]] + matrix[tempCityOrder[level + 1]][tempCityOrder[0]];
        if (pomValue < upperBound) {
            upperBound = pomValue;
            cout << endl << pomValue << "  " << 100 * pomValue / optimalValue << "%";
            for (int i = 0; i < numberOfCities; i++) {
                cityOrder[i] = tempCityOrder[i];
            }
        }
        swap(tempCityOrder, numberOfCities - 2, numberOfCities - 1);
    }
}


