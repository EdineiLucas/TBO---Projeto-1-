#ifndef BUSCA_HPP
#define BUSCA_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include "Filme.hpp"
#include "Cinema.hpp"

using namespace std;

// Forward declarations
class indicetipo;
class indicegenero;
struct IndiceDistancia;

// -------------------------------------------------------------------------
// TIPOS DE FILTRO
// -------------------------------------------------------------------------

enum class FiltroLogico {
    OU,
    E
};

struct FiltroLista {
    vector<string> valores;
    FiltroLogico operador = FiltroLogico::OU;
};

struct FiltroFaixa {
    int minimo = 0;
    int maximo = 0;
};

struct ParametrosBuscaFilmes {
    FiltroLista tipos;
    FiltroLista generos;
    FiltroFaixa duracao;
    FiltroFaixa ano;
};

struct ParametrosBuscaCinemas {
    double distanciaMax = -1.0;
    vector<string> tipos;
    vector<string> generos;
    FiltroFaixa duracao;
    FiltroFaixa ano;
    string titulo;
};

// ========================================================================
// FUNÇÕES DE BUSCA POR ID
// ========================================================================

void inicializarMapaFilmes(const std::vector<Filme>& filmes);
Filme* buscarFilmePorId(std::vector<Filme>& filmes, unsigned int id_procurado);

// ========================================================================
// FUNÇÕES DE INTERSEÇÃO / UNIÃO
// ========================================================================

vector<Filme*> intersecaoFilmes_Otimizada(const vector<Filme*>& listaA, const vector<Filme*>& listaB);
vector<Filme*> uniaoFilmes(const vector<Filme*>& listaA, const vector<Filme*>& listaB);
vector<Cinema*> intersecaoCinemas_Otimizada(const vector<Cinema*>& listaA, const vector<Cinema*>& listaB);
vector<Cinema*> uniaoCinemas(const vector<Cinema*>& listaA, const vector<Cinema*>& listaB);

// ========================================================================
// FUNÇÕES AUXILIARES
// ========================================================================

vector<string> dividirString(const string& str, char delimitador);

// ========================================================================
// FUNÇÕES DE BUSCA POR FILMES
// ========================================================================

vector<Filme*> buscarFilmesPorTipos(const indicetipo& listaTipos, const vector<string>& tipos);
vector<Filme*> buscarFilmesPorGeneros(const indicegenero& listaGeneros, const vector<string>& generos, FiltroLogico operador = FiltroLogico::OU);
vector<Filme*> buscarFilmesPorDuracao(const vector<Filme*>& filmesOrdenadosDuracao, int dur_min, int dur_max);
vector<Filme*> buscarFilmesPorAno(const vector<Filme*>& filmesOrdenadosAno, int ano_min, int ano_max);
vector<Filme*> buscarFilmesCombinados(const indicetipo& listaTipos, const indicegenero& listaGeneros,
    const vector<Filme*>& filmesOrdenadosDuracao, const vector<Filme*>& filmesOrdenadosAno,
    const ParametrosBuscaFilmes& parametros);

// ========================================================================
// FUNÇÕES DE BUSCA POR CINEMAS (COM MÚLTIPLOS FILTROS)
// ========================================================================

vector<Cinema*> buscarCinemasPorDistancia(const vector<IndiceDistancia>& listaBuscaDistancia, double distancia_max);
vector<Cinema*> buscarCinemasPorMultiplosTipos(const vector<Cinema>& cinemas, const vector<Filme>& filmes, const vector<string>& tipos);
vector<Cinema*> buscarCinemasPorMultiplosGeneros(const vector<Cinema>& cinemas, const vector<Filme>& filmes, const vector<string>& generos);
vector<Cinema*> buscarCinemasPorDuracao(const vector<Cinema>& cinemas, const vector<Filme>& filmes, int dur_min, int dur_max);
vector<Cinema*> buscarCinemasPorAno(const vector<Cinema>& cinemas, const vector<Filme>& filmes, int ano_min, int ano_max);
vector<Cinema*> buscarCinemasPorTitulo(const vector<Cinema>& cinemas, const vector<Filme>& filmes, const string& titulo);
vector<Cinema*> buscarCinemasCombinados(const vector<Cinema>& cinemas, const vector<Filme>& filmes,
    const vector<IndiceDistancia>& listaBuscaDistancia,
    const indicetipo& listaTipos, const indicegenero& listaGeneros,
    const ParametrosBuscaCinemas& parametros);

#endif
