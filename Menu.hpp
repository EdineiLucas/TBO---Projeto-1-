#ifndef MENU_HPP
#define MENU_HPP

#include <vector>
#include <string>
#include "Filme.hpp"
#include "Cinema.hpp"
#include "Busca2.hpp"
#include "Indicetipo.hpp"
#include "Indicegenero.hpp"
#include "listaordenadafilmes.hpp"

using namespace std;

class Menu {
public:
    // Menu principal
    static void exibirMenuPrincipal();
    
    // Menus de busca de filmes
    static void menuBuscaFilmes(const vector<Filme>& filmes, 
                                 const vector<Cinema>& cinemas,
                                 const indicetipo& indicetipo,
                                 const indicegenero& indicegenero,
                                 const ListaOrdenadaFilmes& listaOrdenadaDuracao,
                                 const ListaOrdenadaFilmes& listaOrdenadaAno);
    
    static void buscaFilmePorTipo(const vector<Filme>& filmes,
                                   const indicetipo& indicetipo);
    
    static void buscaFilmePorGenero(const vector<Filme>& filmes,
                                     const indicegenero& indicegenero);
    
    static void buscaFilmePorDuracao(const vector<Filme>& filmes,
                                      const ListaOrdenadaFilmes& listaOrdenadaDuracao);
    
    static void buscaFilmePorAno(const vector<Filme>& filmes,
                                  const ListaOrdenadaFilmes& listaOrdenadaAno);
    
    static void buscaFilmeCombinada(const vector<Filme>& filmes,
                                     const vector<Cinema>& cinemas,
                                     const indicetipo& indicetipo,
                                     const indicegenero& indicegenero,
                                     const ListaOrdenadaFilmes& listaOrdenadaDuracao,
                                     const ListaOrdenadaFilmes& listaOrdenadaAno);
    
    // Menus de busca de cinemas
    static void menuBuscaCinemas(const vector<Filme*>& filmesOrdenadosPorId,
                                  const vector<Cinema>& cinemas,
                                  const vector<IndiceDistancia>& listaBuscaDistancia,
                                  const indicetipo& indicetipo,
                                  const indicegenero& indicegenero,
                                  const ListaOrdenadaFilmes& listaOrdenadaDuracao,
                                  const ListaOrdenadaFilmes& listaOrdenadaAno);
    
    static void buscaCinemaPorDistancia(const vector<Filme*>& filmesOrdenadosPorId,
                                         const vector<Cinema>& cinemas,
                                         const vector<IndiceDistancia>& listaBuscaDistancia);
    
    static void buscaCinemaPorTipo(const vector<Filme*>& filmesOrdenadosPorId,
                                    const vector<Cinema>& cinemas,
                                    const indicetipo& indicetipo);
    
    static void buscaCinemaPorGenero(const vector<Filme*>& filmesOrdenadosPorId,
                                      const vector<Cinema>& cinemas,
                                      const indicegenero& indicegenero,
                                      bool operadorE);
    
    static void buscaCinemaPorDuracao(const vector<Filme*>& filmesOrdenadosPorId,
                                       const vector<Cinema>& cinemas,
                                       const ListaOrdenadaFilmes& listaOrdenada);
    
    static void buscaCinemaPorAno(const vector<Filme*>& filmesOrdenadosPorId,
                                   const vector<Cinema>& cinemas,
                                   const ListaOrdenadaFilmes& listaOrdenada);
    
    static void buscaCinemaPorTitulo(const vector<Filme*>& filmesOrdenadosPorId,
                                      const vector<Cinema>& cinemas);
    
    static void buscaCinemaCombinada(const vector<Filme*>& filmesOrdenadosPorId,
                                      const vector<Cinema>& cinemas,
                                      const vector<IndiceDistancia>& listaBuscaDistancia,
                                      const indicetipo& indicetipo,
                                      const indicegenero& indicegenero,
                                      const ListaOrdenadaFilmes& listaOrdenada);
    
    // Funções auxiliares para exibição
    static void exibirFilmes(const vector<Filme*>& filmes, int limite = 20);
    static void exibirCinemas(const vector<Cinema*>& cinemas, const vector<Filme*>& filmesOrdenadosPorId, int limite = 20);
    static string lerLinha(const string& prompt);
    static int lerInteiro(const string& prompt);
    static double lerDouble(const string& prompt);
    static vector<string> lerMultiplasOpcoes(const string& prompt);
    static int lerOpcao(int minimo, int maximo);
};

#endif // MENU_HPP
