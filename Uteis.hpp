#ifndef UTEIS_HPP
#define UTEIS_HPP

#include <string>
#include <vector>
#include <sstream>
#include "Filme.hpp"
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

// Funções para criação de vetores a partir dos arquivos
vector<Filme> criaVectorFilme(stringstream &dadosfilmes, vector<Filme> &filmes);
vector<Cinema> criaVectorCinema(stringstream &dadosCinemas, vector<Cinema> &cinemas);

#endif 