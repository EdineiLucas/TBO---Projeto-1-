#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <chrono>
#include <cmath>

#include "Filme.hpp"   
#include "Cinema.hpp"
#include "Uteis.hpp"
#include "Indicegenero.hpp"
#include "Indicetipo.hpp"
#include "listaordenadacinema.hpp"
#include "listaordenadafilmes.hpp"

using namespace std;

int main() {

    auto inicio = chrono::high_resolution_clock::now();
    ifstream arquivoCinema("Fonte de dados/cinemas.txt"), arquivoFilmes("Fonte de dados/filmesCrop.txt"); //abre os arquivos
    stringstream baseDadosCinema, baseDadosFilmes; // Fluxo de string para facil manipulação
    vector<Cinema> cinemas; // Vetor dos cinemas (lista dos cinemas)
    vector<Filme> filmes;   // Vetor dos filmes (lista dos filmes)

    //***************ABERTURA DOS ARQUIVOS CRIAÇAO DE SS E AMAZENAMENTO NOS VETORES************ */
    if(!arquivoCinema.is_open()){  
        cout << "Erro! Arquivo não aberto" << endl;
        return 1;
    }
    baseDadosCinema << arquivoCinema.rdbuf(); // Adiciona o txt cinemas em um fluxo de streams
    arquivoCinema.close(); // fechar arquivo do cinema

    if(!arquivoFilmes.is_open()){
        cout << "Erro! Arquivo filmes nao abriu" << endl;
        return 1;
    }
    baseDadosFilmes << arquivoFilmes.rdbuf(); //Adiciona o txt filmes no fluxo de streams
    arquivoFilmes.close(); // fechar arquivo do filmescrop

    string linha;
    getline(baseDadosCinema, linha); // Elimina a primeira linha 
    cinemas = criaVectorCinema(baseDadosCinema, cinemas); // preenche o vetor cinemas

    getline(baseDadosFilmes, linha); //Elimina primeira linha do arquivo filmes
    filmes = criaVectorFilme(baseDadosFilmes, filmes); // preenche o vetor filmes

    //*******************INICIALIZAÇÃO DAS LISTAS AUXILIARES**************** */
    cout << "DEBUG 1: Inicializando listas auxiliares" << endl;

    //Cria lista auxilar de busca por tipo
    indicetipo listaFilmesTipo(10*20);
    for(unsigned int i=0; i<filmes.size(); i++){
        listaFilmesTipo.inserir(&filmes.at(i));
    }
    cout << "DEBUG 2: Filme Lista tipo criada" << endl;

    //Cria lista para busca por Genero
    indicegenero listaFIlmesGenero(10*20);
    for(unsigned int i = 0; i < filmes.size(); i++){
        listaFIlmesGenero.inserir(&filmes.at(i));
    }
    cout << "DEBUG 3: Filme Lista GENERO criada" << endl;

    //Cria lista filmes ordenada para buscar por duracao
    ListaOrdenadaFilmes filmesOrdenadosDuracao(filmes.size());
    for(unsigned int i = 0; i < filmes.size(); i++){
        filmesOrdenadosDuracao.inserir(&filmes.at(i));
    }
    filmesOrdenadosDuracao.ordenar("duracao");
    cout << "DEBUG 4: Filme lista DURACAO ORDENADA criada" << endl;

    //Cria lista filmes ordenada para buscar por ano
    ListaOrdenadaFilmes filmesOrdenadosAno = (filmes.size());
    for(unsigned int i = 0; i < filmes.size(); i++){
        filmesOrdenadosAno.inserir(&filmes.at(i));
    }
    filmesOrdenadosAno.ordenar("ano");
    cout << "DEBUG 5: Filme lista ANO ORDENADA criada" << endl;

    //Cria lista de Cinemas ornadada para busca por preco
    ListaOrdenadaCinemas cinemasOdenadosPorPreco(cinemas.size());
    for(unsigned int i=0; i<cinemas.size(); i++){
        cinemasOdenadosPorPreco.inserir(&cinemas.at(i));
    }
    cinemasOdenadosPorPreco.ordenar("preco");
    cout << "DEBUG 6: Cinema lista PRECO ORDENADA criada" << endl;

    //Cria lista de cinemas ordenada para busca por distancia
    const int SEDE_X = 551867;
    const int SEDE_Y = 416560;
    vector<IndiceDistancia> listaBuscaDistancia;
    listaBuscaDistancia.reserve(cinemas.size());
    for(unsigned int i = 0; i < cinemas.size(); i++) {
        IndiceDistancia novo_indice;
        
        // Calcula a distância usando a fórmula Euclidiana
        novo_indice.distancia = sqrt(
            pow(cinemas.at(i).getCoordenadaX() - SEDE_X, 2) + 
            pow(cinemas.at(i).getCoordenadaY() - SEDE_Y, 2)
        );
        novo_indice.cinema = &cinemas.at(i);
        
        listaBuscaDistancia.push_back(novo_indice);
    }
    mergeSortDistancia(listaBuscaDistancia, 0, listaBuscaDistancia.size() - 1);

    cout << "DEBUG 7: Cinema Lista DISTANCIA ORDENADA criada" << endl;

    auto fim = chrono::high_resolution_clock::now();
    chrono::duration<double> tempoEmS = fim - inicio;

    cout << "Tempo em segundos: " << tempoEmS.count() << endl;
    
  // =========================================================================
    // INÍCIO DA INTERFACE DO USUÁRIO (MENU PRINCIPAL)
    // =========================================================================
    
    int opcao = -1;

    do {
        cout << "\n=========================================================" << endl;
        cout << "              SISTEMA DE BUSCA - CINEMAS E FILMES        " << endl;
        cout << "=========================================================" << endl;
        cout << "[1] Buscar Filmes (Filtros Simples ou Combinados)" << endl;
        cout << "[2] Buscar Cinemas (Por Distancia e Preco)" << endl;
        cout << "[0] Sair do Programa" << endl;
        cout << "=========================================================" << endl;
        cout << "Escolha uma opcao: ";
        
        cin >> opcao;

        // Limpa o buffer do teclado para não bugar os próximos 'getlines' ou 'cins'
        cin.ignore(10000, '\n'); 

        switch (opcao) {
            case 1: {
                cout << "\n--- BUSCA DE FILMES APRIMORADA ---" << endl;
                cout << "(Pressione ENTER para deixar em branco e pular o filtro)" << endl;
                cout << "(Para múltiplos valores, separe por vírgula: movie,short,tvSeries)" << endl;

                // Variáveis para receber os textos do terminal
                string tipos_str, generos_str, dur_min_str, dur_max_str, ano_min_str, ano_max_str;

                cout << "Tipos (ex: movie,short,tvSeries): ";
                getline(cin, tipos_str);

                cout << "Generos (ex: Action,Comedy,Drama): ";
                getline(cin, generos_str);

                cout << "Duracao Minima (minutos): ";
                getline(cin, dur_min_str);
                
                cout << "Duracao Maxima (minutos): ";
                getline(cin, dur_max_str);

                cout << "Ano Minimo (ex: 2000): ";
                getline(cin, ano_min_str);
                
                cout << "Ano Maximo (ex: 2024): ";
                getline(cin, ano_max_str);

                cout << "\nBuscando na base de dados..." << endl;

                // ⏱️ INÍCIO DA MEDIÇÃO DO TEMPO DE BUSCA
                auto inicio_busca = chrono::high_resolution_clock::now();

                // Este container vai guardar todas as sublistas que precisaremos cruzar
                vector<vector<Filme*>> listas_para_cruzar;

                // 1. Filtro de Tipos (Usa Tabela Hash) - AGORA SUPORTA MÚLTIPLOS TIPOS
                if (!tipos_str.empty()) {
                    vector<string> tipos = dividirString(tipos_str, ',');
                    vector<Filme*> v_tipos = buscarPorMultiplosTipos(listaFilmesTipo, tipos);
                    if (!v_tipos.empty()) {
                        listas_para_cruzar.push_back(v_tipos);
                    }
                }

                // 2. Filtro de Generos (Usa Tabela Hash) - AGORA SUPORTA MÚLTIPLOS GÊNEROS
                if (!generos_str.empty()) {
                    vector<string> generos = dividirString(generos_str, ',');
                    vector<Filme*> v_generos = buscarPorMultiplosGeneros(listaFIlmesGenero, generos);
                    if (!v_generos.empty()) {
                        listas_para_cruzar.push_back(v_generos);
                    }
                }

                // 3. Filtro de Duracao (Usa Lista Ordenada + Busca Binária/Faixa)
                if (!dur_min_str.empty() || !dur_max_str.empty()) {
                    // Se o usuario pulou o min, assume 0. Se pulou o max, assume 9999.
                    int d_min = dur_min_str.empty() ? 0 : stoi(dur_min_str);
                    int d_max = dur_max_str.empty() ? 9999 : stoi(dur_max_str);
                    
                    vector<Filme*> v_duracao = filmesOrdenadosDuracao.buscaPorFaixa(d_min, d_max, "duracao");
                    listas_para_cruzar.push_back(v_duracao);
                }

                // 4. Filtro de Ano (Usa Lista Ordenada + Busca Binária/Faixa)
                if (!ano_min_str.empty() || !ano_max_str.empty()) {
                    int a_min = ano_min_str.empty() ? 0 : stoi(ano_min_str);
                    int a_max = ano_max_str.empty() ? 3000 : stoi(ano_max_str);
                    
                    vector<Filme*> v_ano = filmesOrdenadosAno.buscaPorFaixa(a_min, a_max, "ano");
                    listas_para_cruzar.push_back(v_ano);
                }

                // -------------------------------------------------------------
                // CRUZAMENTO DE DADOS (INTERSEÇÃO FINAL)
                // -------------------------------------------------------------
                if (listas_para_cruzar.empty()) {
                    cout << "Nenhum filtro foi aplicado. Mostrando os 10 primeiros do catalogo geral:" << endl;
                    for (int i = 0; i < 10 && i < filmes.size(); i++) {
                        cout << "- [" << filmes[i].getId() << "] " << filmes[i].getTituloPrimario() << endl;
                    }
                } else {
                    // Mostra os filtros aplicados
                    cout << "Filtros aplicados:" << endl;
                    if (!tipos_str.empty()) {
                        vector<string> tipos = dividirString(tipos_str, ',');
                        cout << "- Tipos: ";
                        for (size_t i = 0; i < tipos.size(); i++) {
                            cout << tipos[i];
                            if (i < tipos.size() - 1) cout << ", ";
                        }
                        cout << endl;
                    }
                    if (!generos_str.empty()) {
                        vector<string> generos = dividirString(generos_str, ',');
                        cout << "- Generos: ";
                        for (size_t i = 0; i < generos.size(); i++) {
                            cout << generos[i];
                            if (i < generos.size() - 1) cout << ", ";
                        }
                        cout << endl;
                    }
                    if (!dur_min_str.empty() || !dur_max_str.empty()) {
                        int d_min = dur_min_str.empty() ? 0 : stoi(dur_min_str);
                        int d_max = dur_max_str.empty() ? 9999 : stoi(dur_max_str);
                        cout << "- Duracao: " << d_min << " - " << d_max << " minutos" << endl;
                    }
                    if (!ano_min_str.empty() || !ano_max_str.empty()) {
                        int a_min = ano_min_str.empty() ? 0 : stoi(ano_min_str);
                        int a_max = ano_max_str.empty() ? 3000 : stoi(ano_max_str);
                        cout << "- Ano: " << a_min << " - " << a_max << endl;
                    }
                    cout << endl;

                    // Inicializa o resultado com a primeira lista de filtros válidos encontrada
                    vector<Filme*> resultado_final = listas_para_cruzar[0];

                    // Aplica a operação 'E' cruzando com as próximas listas (se o usuário preencheu mais de uma)
                    for (size_t i = 1; i < listas_para_cruzar.size(); i++) {
                        resultado_final = intersecaoFilmes(resultado_final, listas_para_cruzar[i]);
                    }

                    // Impressão limpa dos resultados
                    cout << "Encontrados " << resultado_final.size() << " filmes que atendem a TODOS os criterios:" << endl;
                    
                    int limite = min((int)resultado_final.size(), 20);
                    for (int i = 0; i < limite; i++) {
                        cout << "- [" << resultado_final[i]->getId() << "] " 
                             << resultado_final[i]->getTituloPrimario() 
                             << " (" << resultado_final[i]->getAnoInicio() << " | " 
                             << resultado_final[i]->getTempo() << " min";
                        
                        // Mostra os gêneros do filme
                        vector<string> generos_filme = resultado_final[i]->getGenero();
                        if (!generos_filme.empty()) {
                            cout << " | Generos: ";
                            for (size_t j = 0; j < generos_filme.size(); j++) {
                                cout << generos_filme[j];
                                if (j < generos_filme.size() - 1) cout << ", ";
                            }
                        }
                        cout << ")" << endl;
                    }
                    if (resultado_final.size() > 20) {
                        cout << "... e mais " << resultado_final.size() - 20 << " resultados ocultos (seja mais especifico nos filtros)." << endl;
                    }
                }

                // ⏱️ FIM DA MEDIÇÃO DO TEMPO DE BUSCA
                auto fim_busca = chrono::high_resolution_clock::now();
                chrono::duration<double> tempo_busca = fim_busca - inicio_busca;
                cout << "\nTempo de busca total: " << tempo_busca.count() << " segundos" << endl;

                break;
            } 
            case 2: {
                cout << "\n--- BUSCA DE CINEMAS ---" << endl;
                cout << "(Pressione ENTER para deixar em branco e pular o filtro)" << endl;
                cout << "(Para múltiplos valores, separe por vírgula: movie,short,tvSeries)" << endl;

                string dist_str, preco_str, tipos_str, generos_str, dur_min_str, dur_max_str, ano_min_str, ano_max_str, titulo_str;

                cout << "Distancia Maxima da sede (ex: 150000): ";
                getline(cin, dist_str);

                cout << "Preco Maximo do ingresso (R$) (ex: 15.50): ";
                getline(cin, preco_str);

                cout << "Tipos de filme em exibicao (ex: movie,short,tvSeries): ";
                getline(cin, tipos_str);

                cout << "Generos de filme em exibicao (ex: Action,Comedy,Drama): ";
                getline(cin, generos_str);

                cout << "Duracao Minima dos filmes (minutos): ";
                getline(cin, dur_min_str);
                
                cout << "Duracao Maxima dos filmes (minutos): ";
                getline(cin, dur_max_str);

                cout << "Ano Minimo dos filmes (ex: 2000): ";
                getline(cin, ano_min_str);
                
                cout << "Ano Maximo dos filmes (ex: 2024): ";
                getline(cin, ano_max_str);

                cout << "Titulo especifico de filme em cartaz (ex: The Batman): ";
                getline(cin, titulo_str);

                cout << "\nBuscando na base de dados..." << endl;

                // ⏱️ INÍCIO DA MEDIÇÃO DO TEMPO DE BUSCA
                auto inicio_busca = chrono::high_resolution_clock::now();

                vector<vector<Cinema*>> listas_para_cruzar;

                // 1. Filtro de Distância (Usa a sua lista ordenada!)
                if (!dist_str.empty()) {
                    double d_max = stod(dist_str);
                    vector<Cinema*> v_dist;
                    for (const IndiceDistancia& ind : listaBuscaDistancia) {
                        if (ind.distancia <= d_max) {
                            v_dist.push_back(ind.cinema);
                        } else {
                            break; // Se passou do raio, para a busca imediatamente! (O(logN) feelings)
                        }
                    }
                    listas_para_cruzar.push_back(v_dist);
                }

                // 2. Filtro de Preço (Usa a sua lista ordenada!)
                if (!preco_str.empty()) {
                    float p_max = stof(preco_str);
                    vector<Cinema*> v_preco;
                    for (Cinema* c : cinemasOdenadosPorPreco.lista) {
                        if (c->getPrecoIngresso() <= p_max) {
                            v_preco.push_back(c);
                        } else {
                            break; // Passou do preço máximo, pode parar!
                        }
                    }
                    listas_para_cruzar.push_back(v_preco);
                }

                // 3. Filtro de Tipos de Filme em Cartaz - AGORA SUPORTA MÚLTIPLOS TIPOS
                if (!tipos_str.empty()) {
                    vector<string> tipos = dividirString(tipos_str, ',');
                    vector<Cinema*> v_tipos = buscarCinemasPorMultiplosTipos(cinemas, filmes, tipos);
                    if (!v_tipos.empty()) {
                        listas_para_cruzar.push_back(v_tipos);
                    }
                }

                // 4. Filtro de Gêneros de Filme em Cartaz - AGORA SUPORTA MÚLTIPLOS GÊNEROS
                if (!generos_str.empty()) {
                    vector<string> generos = dividirString(generos_str, ',');
                    vector<Cinema*> v_generos = buscarCinemasPorMultiplosGeneros(cinemas, filmes, generos);
                    if (!v_generos.empty()) {
                        listas_para_cruzar.push_back(v_generos);
                    }
                }

                // 5. Filtro de Duração dos Filmes em Cartaz
                if (!dur_min_str.empty() || !dur_max_str.empty()) {
                    int d_min = dur_min_str.empty() ? 0 : stoi(dur_min_str);
                    int d_max = dur_max_str.empty() ? 9999 : stoi(dur_max_str);
                    vector<Cinema*> v_duracao = buscarCinemasPorDuracao(cinemas, filmes, d_min, d_max);
                    if (!v_duracao.empty()) {
                        listas_para_cruzar.push_back(v_duracao);
                    }
                }

                // 6. Filtro de Ano dos Filmes em Cartaz
                if (!ano_min_str.empty() || !ano_max_str.empty()) {
                    int a_min = ano_min_str.empty() ? 0 : stoi(ano_min_str);
                    int a_max = ano_max_str.empty() ? 3000 : stoi(ano_max_str);
                    vector<Cinema*> v_ano = buscarCinemasPorAno(cinemas, filmes, a_min, a_max);
                    if (!v_ano.empty()) {
                        listas_para_cruzar.push_back(v_ano);
                    }
                }

                // 7. Filtro de Título Específico de Filme em Cartaz
                if (!titulo_str.empty()) {
                    vector<Cinema*> v_titulo = buscarCinemasPorTitulo(cinemas, filmes, titulo_str);
                    if (!v_titulo.empty()) {
                        listas_para_cruzar.push_back(v_titulo);
                    }
                }

                // --- CRUZAMENTO DOS DADOS (INTERSEÇÃO FINAL) ---
                if (listas_para_cruzar.empty()) {
                    cout << "Nenhum filtro foi aplicado. Mostrando alguns cinemas do catalogo:" << endl;
                    for (int i = 0; i < 10 && i < cinemas.size(); i++) {
                        cout << "- " << cinemas[i].getNome() << " (R$ " << cinemas[i].getPrecoIngresso() << ")" << endl;
                    }
                } else {
                    vector<Cinema*> resultado_final = listas_para_cruzar[0];
                    
                    for (size_t i = 1; i < listas_para_cruzar.size(); i++) {
                        resultado_final = intersecaoCinemas(resultado_final, listas_para_cruzar[i]);
                    }

                    // Impressão limpa dos resultados
                    cout << "Encontrados " << resultado_final.size() << " cinemas que atendem a TODOS os criterios exatos:" << endl;
                    
                    int limite = min((int)resultado_final.size(), 15);
                    for (int i = 0; i < limite; i++) {
                        cout << "- " << resultado_final[i]->getNome() 
                             << "\n    [Preco: R$ " << resultado_final[i]->getPrecoIngresso() 
                             << " | Filmes em Cartaz: " << resultado_final[i]->getFilmesEmCartaz().size() << "]" << endl;
                    }
                    if (resultado_final.size() > 15) {
                        cout << "... e mais " << resultado_final.size() - 15 << " resultados ocultos." << endl;
                    }
                }
                break;
            }
            case 0:
                cout << "\nEncerrando o sistema. Obrigado!" << endl;
                break;
            default:
                cout << "\nOpcao invalida. Tente novamente." << endl;
                break;
        }

        if (opcao != 0) {
            cout << "\nPressione ENTER para voltar ao menu...";
            cin.get(); // Pausa a tela até o usuário dar Enter
        }

    } while (opcao != 0);

    return 0; // Fim real do programa
}