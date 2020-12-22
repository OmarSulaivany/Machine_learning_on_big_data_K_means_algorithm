// Machine_learning_on_big_data.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Kmeans.h"
using namespace std;

int main()
{
    Kmeans a;
    a.dataset("Covid-19_17_dec_2020.csv");
    a.kmeans(5, 100);
  
}

//a.dataset("CSV or TXT file location");
//a.Kmeans("number of clusters", "number of iterations")
