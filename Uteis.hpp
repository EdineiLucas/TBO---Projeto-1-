#ifndef UTEIS_HPP
#define UTEIS_HPP

#include <string>
#include <vector>
#include <sstream>
#include "Filme.hpp"
#include "Indicegenero.hpp"
#include "Cinema.hpp"

using namespace std;

struct IndiceDistancia
{
    double distancia;
    Cinema * cinema;
};

void mergeDistancia(vector<IndiceDistancia> & arr, int esquerda, int meio, int direita);
void mergeSortDistancia(vector<IndiceDistancia>& arr, int esquerda, int direita);

int converterInt(const string &text);

#endif 