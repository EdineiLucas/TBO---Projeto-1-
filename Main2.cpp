#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <chrono>
#include <cmath>
#include <set>

#include "Filme.hpp"   
#include "Cinema.hpp"
#include "Uteis.hpp"
#include "Indicegenero.hpp"
#include "Indicetipo.hpp"
#include "listaordenadacinema.hpp"
#include "listaordenadafilmes.hpp"
#include "Menu.hpp"

using namespace std;

int main() {

    auto inicio = chrono::high_resolution_clock::now();
    
    // =========================================================================
    // ETAPA 1: ABERTURA DOS ARQUIVOS E CRIAÇÃO DOS VETORES
    // =========================================================================
    cout << "=== ETAPA 1: Carregando dados dos arquivos ===" << endl;
    
    ifstream arquivoCinema("Fonte de dados/cinemas.txt"), arquivoFilmes("Fonte de dados/filmesCrop.txt");
    stringstream baseDadosCinema, baseDadosFilmes;
    vector<Cinema> cinemas;
    vector<Filme> filmes;

    cinemas.reserve(401);
    filmes.reserve(600000);

    // Verifica abertura do arquivo de cinemas
    if (!arquivoCinema.is_open()) {
        cout << "Erro! Arquivo de cinemas nao aberto" << endl;
        return 1;
    }
    baseDadosCinema << arquivoCinema.rdbuf();
    arquivoCinema.close();
    cout << "Arquivo de cinemas carregado" << endl;

    // Verifica abertura do arquivo de filmes
    if (!arquivoFilmes.is_open()) {
        cout << "Erro! Arquivo de filmes nao aberto" << endl;
        return 1;
    }
    baseDadosFilmes << arquivoFilmes.rdbuf();
    arquivoFilmes.close();
    cout << "Arquivo de filmes carregado" << endl;

    // Preenche os vetores
    string linha;
    getline(baseDadosCinema, linha);
    cinemas = criaVectorCinema(baseDadosCinema, cinemas);
    cout << "- " << cinemas.size() << " cinemas carregados" << endl;

    getline(baseDadosFilmes, linha);
    filmes = criaVectorFilme(baseDadosFilmes, filmes);
    cout << "- " << filmes.size() << " filmes carregados" << endl;

    // =========================================================================
    // ETAPA 2: INICIALIZAÇÃO DAS LISTAS AUXILIARES
    // =========================================================================
    cout << "\n=== ETAPA 3: Inicializando listas auxiliares ===" << endl;

    // 3.1 - Índice de Tipos
    cout << "Criando indice de tipos..." << endl;
    indicetipo listaFilmesTipo(10 * 20);
    for (unsigned int i = 0; i < filmes.size(); i++) {
        listaFilmesTipo.inserir(&filmes.at(i));
    }
    cout << "- Indice de tipos criado" << endl;

    // 3.2 - Índice de Gêneros
    cout << "Criando indice de generos..." << endl;
    indicegenero listaFIlmesGenero(10 * 20);
    for (unsigned int i = 0; i < filmes.size(); i++) {
        listaFIlmesGenero.inserir(&filmes.at(i));
    }
    cout << "- Indice de generos criado" << endl;

    // 3.3 - Lista Ordenada por Duração
    cout << "Criando lista de filmes ordenada por duracao..." << endl;
    ListaOrdenadaFilmes filmesOrdenadosDuracao(filmes.size());
    for (unsigned int i = 0; i < filmes.size(); i++) {
        filmesOrdenadosDuracao.inserir(&filmes.at(i));
    }
    filmesOrdenadosDuracao.ordenar("duracao");
    cout << "- Lista de filmes por duracao criada" << endl;

    // 3.4 - Lista Ordenada por Ano
    cout << "Criando lista de filmes ordenada por ano..." << endl;
    ListaOrdenadaFilmes filmesOrdenadosAno(filmes.size());
    for (unsigned int i = 0; i < filmes.size(); i++) {
        filmesOrdenadosAno.inserir(&filmes.at(i));
    }
    filmesOrdenadosAno.ordenar("ano");
    cout << "- Lista de filmes por ano criada" << endl;

    // 3.5 - Lista Ordenada de Cinemas por Preço
    cout << "Criando lista de cinemas ordenada por preco..." << endl;
    ListaOrdenadaCinemas cinemasOdenadosPorPreco(cinemas.size());
    for (unsigned int i = 0; i < cinemas.size(); i++) {
        cinemasOdenadosPorPreco.inserir(&cinemas.at(i));
    }
    cinemasOdenadosPorPreco.ordenar("preco");
    cout << "- Lista de cinemas por preco criada" << endl;

    // 3.6 - Lista de Cinemas Ordenada por Distância
    cout << "Criando lista de cinemas ordenada por distancia..." << endl;
    const int SEDE_X = 551867;
    const int SEDE_Y = 416560;
    vector<IndiceDistancia> listaBuscaDistancia;
    listaBuscaDistancia.reserve(cinemas.size());
    
    for (unsigned int i = 0; i < cinemas.size(); i++) {
        IndiceDistancia novo_indice;
        novo_indice.distancia = sqrt(
            pow(cinemas.at(i).getCoordenadaX() - SEDE_X, 2) + 
            pow(cinemas.at(i).getCoordenadaY() - SEDE_Y, 2)
        );
        novo_indice.cinema = &cinemas.at(i);
        listaBuscaDistancia.push_back(novo_indice);
    }
    mergeSortDistancia(listaBuscaDistancia, 0, listaBuscaDistancia.size() - 1);
    cout << "- Lista de cinemas por distancia criada" << endl;

    // 3.7 - Lista de Filmes Ordenada por ID
    cout << "Criando lista de filmes ordenada por ID..." << endl;
    vector<Filme*> filmesOrdenadosPorId;
    filmesOrdenadosPorId.reserve(filmes.size());
    for (unsigned int i = 0; i < filmes.size(); i++) {
        filmesOrdenadosPorId.push_back(&filmes.at(i));
    }
    cout << "- Lista de filmes por ID criada" << endl;

    // =========================================================================
    // RESUMO DO CARREGAMENTO
    // =========================================================================
    auto fim = chrono::high_resolution_clock::now();
    chrono::duration<double> tempoEmS = fim - inicio;
    cout << "Total de cinemas: " << cinemas.size() << endl;
    cout << "Total de filmes: " << filmes.size() << endl;
    cout << "Tempo total de carregamento: " << tempoEmS.count() << " segundos" << endl;

    cout << "\n========================================" << endl;
    cout << "      SISTEMA CARREGADO COM SUCESSO!" << endl;
    cout << "========================================\n" << endl;

    // --- DEBUG 2: listaFIlmesGenero ---
    cout << "\n\n[2] LISTA DE FILMES POR GENERO" << endl;
    cout << "===============================" << endl;
    {
        const vector<list<Filme*>>& listaInterna = listaFIlmesGenero.getListaGenero();
        int bucket_com_dados = 0;
        int total_filmes_genero = 0;
        set<string> generos_processados;
        
        for (size_t i = 0; i < listaInterna.size(); i++) {
            if (!listaInterna[i].empty()) {
                // Pega o gênero do primeiro filme deste bucket
                string genero = listaInterna[i].front()->getGenero()[0];
                
                // Evita imprimir o mesmo gênero múltiplas vezes (pode estar em vários buckets por colisão)
                if (generos_processados.find(genero) == generos_processados.end()) {
                    generos_processados.insert(genero);
                    bucket_com_dados++;
                    
                    // Conta quantos filmes têm este gênero
                    int contador_genero = 0;
                    for (size_t j = 0; j < listaInterna.size(); j++) {
                        for (Filme* f : listaInterna[j]) {
                            vector<string> generos_filme = f->getGenero();
                            for (const string& g : generos_filme) {
                                if (g == genero) {
                                    contador_genero++;
                                    break;
                                }
                            }
                        }
                    }
                    
                    cout << "\nGenero: " << genero << " (" << contador_genero << " filmes no total)" << endl;
                    cout << "Primeiros 5 filmes:" << endl;
                    
                    int contador = 0;
                    for (size_t j = 0; j < listaInterna.size(); j++) {
                        for (Filme* filme : listaInterna[j]) {
                            vector<string> generos_filme = filme->getGenero();
                            bool tem_genero = false;
                            for (const string& g : generos_filme) {
                                if (g == genero) {
                                    tem_genero = true;
                                    break;
                                }
                            }
                            
                            if (tem_genero) {
                                if (contador >= 200) break;
                                cout << "  [" << filme->getId() << "] " 
                                     << filme->getTituloPrimario() 
                                     << " | Generos: ";
                                for (size_t k = 0; k < generos_filme.size(); k++) {
                                    cout << generos_filme[k];
                                    if (k < generos_filme.size() - 1) cout << ", ";
                                }
                                cout << endl;
                                contador++;
                            }
                        }
                        if (contador >= 5) break;
                    }
                    total_filmes_genero++;
                }
            }
        }
        cout << "\nResumo: " << bucket_com_dados << " generos unicos encontrados" << endl;
    }
    
    // =========================================================================
    // INTERFACE INTERATIVA
    // =========================================================================
    
    int opcaoPrincipal;
    bool continuar = true;
    
    while (continuar) {
        Menu::exibirMenuPrincipal();
        opcaoPrincipal = Menu::lerOpcao(0, 2);
        
        switch (opcaoPrincipal) {
            case 1:
                Menu::menuBuscaFilmes(filmes, cinemas, listaFilmesTipo, listaFIlmesGenero, filmesOrdenadosDuracao, filmesOrdenadosAno);
                break;
            case 2:
                Menu::menuBuscaCinemas(filmesOrdenadosPorId, cinemas, listaBuscaDistancia, 
                                       listaFilmesTipo, listaFIlmesGenero, filmesOrdenadosDuracao, filmesOrdenadosAno);
                break;
            case 0:
                cout << "\nAte logo!\n" << endl;
                continuar = false;
                break;
        }
    }

    // =========================================================================
    // FINALIZAÇÃO
    // =========================================================================
    auto fimExecucao = chrono::high_resolution_clock::now();
    auto duracao = chrono::duration_cast<chrono::milliseconds>(fimExecucao - inicio);
    cout << "Tempo total de execucao: " << duracao.count() << " ms\n" << endl;

    return 0;
}
