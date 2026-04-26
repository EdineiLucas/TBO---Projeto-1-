#ifndef UTEIS_HPP
#define UTEIS_HPP

#include <string>
#include <vector>
#include <sstream>
#include "Filme.hpp"
#include "Indicegenero.hpp"
#include "Indicetipo.hpp"
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

vector<Filme*> intersecaoFilmes(const vector<Filme*> &listaA, vector<Filme*> &listaB);

vector<Filme*> uniaoFilme(const vector<Filme*> &listaA, vector<Filme*> &listaB);

// Função de Busca Binária para encontrar um filme específico pelo ID
Filme* buscarFilmePorId(std::vector<Filme>& filmes, unsigned int id_procurado);

vector<Cinema*> intersecaoCinemas(const vector<Cinema*>& listaA, const vector<Cinema*>& listaB);

// Funções auxiliares para busca aprimorada
vector<string> dividirString(const string& str, char delimitador);
vector<Filme*> buscarPorMultiplosTipos(indicetipo& listaTipos, const vector<string>& tipos);
vector<Filme*> buscarPorMultiplosGeneros(indicegenero& listaGeneros, const vector<string>& generos);

// Funções auxiliares para busca de cinemas aprimorada
vector<Cinema*> buscarCinemasPorMultiplosTipos(const vector<Cinema>& cinemas, const vector<Filme>& filmes, const vector<string>& tipos);
vector<Cinema*> buscarCinemasPorMultiplosGeneros(const vector<Cinema>& cinemas, const vector<Filme>& filmes, const vector<string>& generos);
vector<Cinema*> buscarCinemasPorDuracao(const vector<Cinema>& cinemas, const vector<Filme>& filmes, int dur_min, int dur_max);
vector<Cinema*> buscarCinemasPorAno(const vector<Cinema>& cinemas, const vector<Filme>& filmes, int ano_min, int ano_max);
vector<Cinema*> buscarCinemasPorTitulo(const vector<Cinema>& cinemas, const vector<Filme>& filmes, const string& titulo);

// Funções para criação de vetores a partir dos arquivos
vector<Filme> criaVectorFilme(stringstream &dadosfilmes, vector<Filme> &filmes);
vector<Cinema> criaVectorCinema(stringstream &dadosCinemas, vector<Cinema> &cinemas);

#endif 