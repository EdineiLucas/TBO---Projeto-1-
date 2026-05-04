#ifndef BUSCA2_HPP
#define BUSCA2_HPP

#include <vector>
#include <string>
#include "Filme.hpp"
#include "Cinema.hpp"

// Forward declarations
class indicetipo;
class indicegenero;
struct IndiceDistancia;

// ========================================================================
// TIPOS DE FILTRO
// ========================================================================

enum class FiltroLogico {
    OU,
    E
};

struct FiltroLista {
    std::vector<std::string> valores;
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
    std::vector<std::string> tipos;
    std::vector<std::string> generos;
    FiltroFaixa duracao;
    FiltroFaixa ano;
    std::string titulo;
};

// ========================================================================
// FUNÇÕES DE ORDENAÇÃO POR ID
// ========================================================================

std::vector<Filme*> ordenarFilmesPorId(const std::vector<Filme*>& filmes);
std::vector<Cinema*> ordenarCinemasPorId(const std::vector<Cinema*>& cinemas);

// ========================================================================
// FUNÇÕES DE BUSCA POR ID (BINÁRIA)
// ========================================================================

Filme* buscarFilmePorIdOrdenado(const std::vector<Filme*>& filmesOrdenados, unsigned int id_procurado);
Cinema* buscarCinemaPorIdOrdenado(const std::vector<Cinema*>& cinemasOrdenados, unsigned int id_procurado);

// ========================================================================
// FUNÇÕES DE INTERSEÇÃO / UNIÃO (SEM unordered_set)
// ========================================================================

std::vector<Filme*> intersecaoFilmesOrdenada(const std::vector<Filme*>& listaA, const std::vector<Filme*>& listaB);
std::vector<Cinema*> intersecaoCinemasOrdenada(const std::vector<Cinema*>& listaA, const std::vector<Cinema*>& listaB);

std::vector<Filme*> uniaoFilmesOrdenada(const std::vector<Filme*>& listaA, const std::vector<Filme*>& listaB);
std::vector<Cinema*> uniaoCinemasOrdenada(const std::vector<Cinema*>& listaA, const std::vector<Cinema*>& listaB);

// ========================================================================
// FUNÇÕES DE BUSCA POR FILMES (SEM unordered_set)
// ========================================================================

std::vector<Filme*> buscarFilmesPorTipos_Otimizado(const indicetipo& listaTipos, const std::vector<std::string>& tipos);
std::vector<Filme*> buscarFilmesPorGeneros_Otimizado(const indicegenero& listaGeneros, const std::vector<std::string>& generos, bool operadorE = false);
std::vector<Filme*> buscarFilmesPorDuracao_Otimizado(const std::vector<Filme*>& filmesOrdenadosDuracao, int dur_min, int dur_max);
std::vector<Filme*> buscarFilmesPorAno_Otimizado(const std::vector<Filme*>& filmesOrdenadosAno, int ano_min, int ano_max);
std::vector<Filme*> buscarFilmesCombinados_Otimizado(const indicetipo& listaTipos, const indicegenero& listaGeneros,
    const std::vector<Filme*>& filmesOrdenadosDuracao, const std::vector<Filme*>& filmesOrdenadosAno,
    const ParametrosBuscaFilmes& parametros);

// ========================================================================
// FUNÇÕES DE BUSCA POR CINEMAS (SEM unordered_set)
// ========================================================================

std::vector<Cinema*> buscarCinemasPorDistancia_Otimizado(const std::vector<IndiceDistancia>& listaBuscaDistancia, double distancia_max);
std::vector<Cinema*> buscarCinemasPorMultiplosTipos_Otimizado(const std::vector<Cinema>& cinemas, const std::vector<Filme*>& filmesOrdenadosPorId, const std::vector<std::string>& tipos);
std::vector<Cinema*> buscarCinemasPorMultiplosGeneros_Otimizado(const std::vector<Cinema>& cinemas, const std::vector<Filme*>& filmesOrdenadosPorId, const std::vector<std::string>& generos, bool operadorE = false);
std::vector<Cinema*> buscarCinemasPorDuracao_Otimizado(const std::vector<Cinema>& cinemas, const std::vector<Filme*>& filmesOrdenadosPorId, int dur_min, int dur_max);
std::vector<Cinema*> buscarCinemasPorAno_Otimizado(const std::vector<Cinema>& cinemas, const std::vector<Filme*>& filmesOrdenadosPorId, int ano_min, int ano_max);
std::vector<Cinema*> buscarCinemasPorTitulo_Otimizado(const std::vector<Cinema>& cinemas, const std::vector<Filme*>& filmesOrdenadosPorId, const std::string& titulo);
std::vector<Cinema*> buscarCinemasCombinados_Otimizado(const std::vector<Cinema>& cinemas, const std::vector<Filme*>& filmesOrdenadosPorId,
    const std::vector<IndiceDistancia>& listaBuscaDistancia,
    const indicetipo& listaTipos, const indicegenero& listaGeneros,
    const ParametrosBuscaCinemas& parametros);

#endif // BUSCA2_HPP
