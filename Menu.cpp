#include "Menu.hpp"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <cmath>

using namespace std;

// ============================================================================
// FUNÇÕES AUXILIARES
// ============================================================================

string Menu::lerLinha(const string& prompt) {
    string entrada;
    cout << prompt;
    getline(cin, entrada);
    return entrada;
}

int Menu::lerInteiro(const string& prompt) {
    int valor;
    while (true) {
        cout << prompt;
        if (cin >> valor) {
            cin.ignore(); // Limpar buffer
            return valor;
        }
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Entrada invalida. Tente novamente.\n";
    }
}

double Menu::lerDouble(const string& prompt) {
    double valor;
    while (true) {
        cout << prompt;
        if (cin >> valor) {
            cin.ignore();
            return valor;
        }
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Entrada invalida. Tente novamente.\n";
    }
}

vector<string> Menu::lerMultiplasOpcoes(const string& prompt) {
    string linha = lerLinha(prompt);
    vector<string> opcoes;
    string opcao;
    
    auto trim = [](string& s) {
        while (!s.empty() && isspace((unsigned char)s.front())) s.erase(s.begin());
        while (!s.empty() && isspace((unsigned char)s.back())) s.pop_back();
    };

    for (char c : linha) {
        if (c == ',' || c == ';') {
            trim(opcao);
            if (!opcao.empty()) {
                opcoes.push_back(opcao);
                opcao.clear();
            }
        } else {
            opcao += c;
        }
    }
    trim(opcao);
    if (!opcao.empty()) {
        opcoes.push_back(opcao);
    }
    
    return opcoes;
}

int Menu::lerOpcao(int minimo, int maximo) {
    while (true) {
        int opcao = lerInteiro("Escolha uma opcao: ");
        if (opcao >= minimo && opcao <= maximo) {
            return opcao;
        }
        cout << "Opção inválida. Escolha entre " << minimo << " e " << maximo << ".\n";
    }
}

void Menu::exibirFilmes(const vector<Filme*>& filmes, int limite) {
    if (filmes.empty()) {
        cout << "\n>>> Nenhum filme encontrado.\n" << endl;
        return;
    }
    
    int quantidade = min((int)filmes.size(), limite);
    cout << "\n>>> Encontrados " << filmes.size() << " filme(s). "
         << "Exibindo " << quantidade << ":\n" << endl;
    
    for (int i = 0; i < quantidade; i++) {
        Filme* f = filmes[i];
        cout << (i + 1) << ". " << f->getTituloPrimario()
             << " | Ano: " << f->getAnoInicio();
        if (f->getAnoFim() != 0 && f->getAnoFim() != f->getAnoInicio()) {
            cout << " - " << f->getAnoFim();
        }
        cout << " | Tipo: " << f->getTipo()
             << " | Duracao: " << f->getTempo() << " min"
             << " | Generos: ";
        const vector<string>& generos = f->getGenero();
        for (size_t j = 0; j < generos.size(); ++j) {
            cout << generos[j];
            if (j < generos.size() - 1) cout << ", ";
        }
        cout << endl;
    }
    cout << endl;
}

void Menu::exibirCinemas(const vector<Cinema*>& cinemas, const vector<Filme>& filmes, int limite) {
    if (cinemas.empty()) {
        cout << "\n>>> Nenhum cinema encontrado.\n" << endl;
        return;
    }
    
    int quantidade = min((int)cinemas.size(), limite);
    cout << "\n>>> Encontrados " << cinemas.size() << " cinema(s). "
         << "Exibindo " << quantidade << ":\n" << endl;
    
    for (int i = 0; i < quantidade; i++) {
        Cinema* c = cinemas[i];
        const int SEDE_X = 551867;
        const int SEDE_Y = 416560;
        double distancia = sqrt(pow(c->getCoordenadaX() - SEDE_X, 2) + pow(c->getCoordenadaY() - SEDE_Y, 2));
        cout << (i + 1) << ". " << c->getNome()
             << " | Preco: " << fixed << setprecision(2) << c->getPrecoIngresso()
             << " | Distancia: " << fixed << setprecision(2) << distancia << " metros"
             << "\n";
        const vector<unsigned int>& filmesEmCartaz = c->getFilmesEmCartaz();
        if (filmesEmCartaz.empty()) {
            cout << "    Nenhum filme em cartaz." << endl;
        } else {
            cout << "    Filmes em cartaz:\n";
            for (unsigned int id_filme : filmesEmCartaz) {
                Filme* f = buscarFilmePorId(const_cast<vector<Filme>&>(filmes), id_filme);
                if (f != nullptr) {
                    cout << "      [" << f->getId() << "] " << f->getTituloPrimario() << "\n";
                } else {
                    cout << "      [" << id_filme << "] (filme desconhecido)\n";
                }
            }
        }
        cout << endl;
    }
}

// ============================================================================
// MENU PRINCIPAL
// ============================================================================

void Menu::exibirMenuPrincipal() {
    cout << "\n";
    cout << "-----   SISTEMA DE BUSCA DE FILMES E CINEMAS    -------\n";
    cout << "\n1. Buscar Filmes\n";
    cout << "2. Buscar Cinemas\n";
    cout << "0. Sair\n";
}

// ============================================================================
// MENUS DE BUSCA DE FILMES
// ============================================================================

void Menu::buscaFilmePorTipo(const vector<Filme>& filmes,
                               const indicetipo& indicetipo) {
    cout << "\n=== BUSCA DE FILMES POR TIPO ===\n";
    cout << "Buscar por 1 ou mais tipos?\n";
    cout << "Tipos disponiveis: Movie, TV Series, Short\n";
    vector<string> tipos = lerMultiplasOpcoes(
        "Digite os tipos (separados por virgula): ");
    
    vector<Filme*> resultado = buscarFilmesPorTipos(indicetipo, tipos);
    exibirFilmes(resultado);
}

void Menu::buscaFilmePorGenero(const vector<Filme>& filmes,
                                const indicegenero& indicegenero) {
    cout << "\n=== BUSCA DE FILMES POR GENERO ===\n";
    int op;
    cout << "1. Filmes que contenham TODOS os generos (E)\n";
    cout << "2. Filmes que contenham ALGUM genero (OU)\n";
    op = lerOpcao(1, 2);
    
    vector<string> generos = lerMultiplasOpcoes(
        "Digite os generos (separados por virgula): ");
    
    FiltroLogico operador = (op == 1) ? FiltroLogico::E : FiltroLogico::OU;
    vector<Filme*> resultado = buscarFilmesPorGeneros(indicegenero, generos, operador);
    exibirFilmes(resultado);
}

void Menu::buscaFilmePorDuracao(const vector<Filme>& filmes,
                                 const ListaOrdenadaFilmes& listaOrdenadaDuracao) {
    cout << "\n=== BUSCA DE FILMES POR DURACAO ===\n";
    int durMin = lerInteiro("Duracao minima (minutos): ");
    int durMax = lerInteiro("Duracao maxima (minutos): ");
    
    FiltroFaixa faixa{durMin, durMax};
    vector<Filme*> resultado = buscarFilmesPorDuracao(listaOrdenadaDuracao.lista, faixa.minimo, faixa.maximo);
    exibirFilmes(resultado);
}

void Menu::buscaFilmePorAno(const vector<Filme>& filmes,
                             const ListaOrdenadaFilmes& listaOrdenadaAno) {
    cout << "\n=== BUSCA DE FILMES POR ANO ===\n";
    int anoMin = lerInteiro("Ano minimo: ");
    int anoMax = lerInteiro("Ano maximo: ");
    
    FiltroFaixa faixa{anoMin, anoMax};
    vector<Filme*> resultado = buscarFilmesPorAno(listaOrdenadaAno.lista, faixa.minimo, faixa.maximo);
    exibirFilmes(resultado);
}

void Menu::buscaFilmeCombinada(const vector<Filme>& filmes,
                                const vector<Cinema>& cinemas,
                                const indicetipo& indicetipo,
                                const indicegenero& indicegenero,
                                const ListaOrdenadaFilmes& listaOrdenadaDuracao,
                                const ListaOrdenadaFilmes& listaOrdenadaAno) {
    cout << "\n=== BUSCA COMBINADA DE FILMES ===\n";
    
    ParametrosBuscaFilmes params;
    
    // Tipo
    int usarTipo = lerInteiro("\nAdicionar filtro de TIPO? (1=Sim, 0=Nao): ");
    if (usarTipo == 1) {
        string tipo = lerLinha("Digite o tipo (Movie/TV Series/Short): ");
        params.tipos.valores.push_back(tipo);
    }
    
    // Gêneros
    int usarGenero = lerInteiro("Adicionar filtro de GENERO? (1=Sim, 0=Nao): ");
    if (usarGenero == 1) {
        cout << "1. Filmes com TODOS os generos (E)\n";
        cout << "2. Filmes com ALGUM genero (OU)\n";
        int opGen = lerOpcao(1, 2);
        params.generos.valores = lerMultiplasOpcoes("Digite os generos (separados por virgula): ");
        params.generos.operador = (opGen == 1) ? FiltroLogico::E : FiltroLogico::OU;
    }
    
    // Duração
    int usarDuracao = lerInteiro("Adicionar filtro de DURACAO? (1=Sim, 0=Nao): ");
    if (usarDuracao == 1) {
        params.duracao.minimo = lerInteiro("Duracao minima (minutos): ");
        params.duracao.maximo = lerInteiro("Duracao maxima (minutos): ");
    }
    
    // Ano
    int usarAno = lerInteiro("Adicionar filtro de ANO? (1=Sim, 0=Nao): ");
    if (usarAno == 1) {
        params.ano.minimo = lerInteiro("Ano minimo: ");
        params.ano.maximo = lerInteiro("Ano maximo: ");
    }
    
    vector<Filme*> resultado = buscarFilmesCombinados(indicetipo, indicegenero,
                                                       listaOrdenadaDuracao.lista, listaOrdenadaAno.lista, params);
    exibirFilmes(resultado);
}

void Menu::menuBuscaFilmes(const vector<Filme>& filmes,
                            const vector<Cinema>& cinemas,
                            const indicetipo& indicetipo,
                            const indicegenero& indicegenero,
                            const ListaOrdenadaFilmes& listaOrdenadaDuracao,
                            const ListaOrdenadaFilmes& listaOrdenadaAno) {
    while (true) {
        cout << "\n ---- BUSCA DE FILMES ----\n";
        cout << "1. Por Tipo                             \n";
        cout << "2. Por Genero                           \n";
        cout << "3. Por Duracao                          \n";
        cout << "4. Por Ano                              \n";
        cout << "5. Busca Combinada                      \n";
        cout << "0. Voltar ao Menu Principal             \n";
        
        int opcao = lerOpcao(0, 5);
        
        switch (opcao) {
            case 1:
                buscaFilmePorTipo(filmes, indicetipo);
                break;
            case 2:
                buscaFilmePorGenero(filmes, indicegenero);
                break;
            case 3:
                buscaFilmePorDuracao(filmes, listaOrdenadaDuracao);
                break;
            case 4:
                buscaFilmePorAno(filmes, listaOrdenadaAno);
                break;
            case 5:
                buscaFilmeCombinada(filmes, cinemas, indicetipo, indicegenero, listaOrdenadaDuracao, listaOrdenadaAno);
                break;
            case 0:
                return;
        }
    }
}

// ============================================================================
// MENUS DE BUSCA DE CINEMAS
// ============================================================================

void Menu::buscaCinemaPorDistancia(const vector<Filme>& filmes,
                                    const vector<Cinema>& cinemas,
                                    const vector<IndiceDistancia>& listaBuscaDistancia) {
    cout << "\n=== BUSCA DE CINEMAS POR DISTANCIA ===\n";
    double distMax = lerDouble("Distancia maxima (metros): ");
    
    vector<Cinema*> resultado = buscarCinemasPorDistancia(listaBuscaDistancia, distMax);
    exibirCinemas(resultado, filmes);
}

void Menu::buscaCinemaPorTipo(const vector<Filme>& filmes,
                               const vector<Cinema>& cinemas,
                               const indicetipo& indicetipo) {
    cout << "\n=== BUSCA DE CINEMAS POR TIPO DE FILME ===\n";
    string tipo = lerLinha("Digite o tipo (Movie/TV Series/Short): ");
    
    vector<string> tipos = {tipo};
    vector<Cinema*> resultado = buscarCinemasPorMultiplosTipos(cinemas, filmes, tipos);
    exibirCinemas(resultado, filmes);
}

void Menu::buscaCinemaPorGenero(const vector<Filme>& filmes,
                                 const vector<Cinema>& cinemas,
                                 const indicegenero& indicegenero) {
    cout << "\n=== BUSCA DE CINEMAS POR GENERO ===\n";
    int op;
    cout << "1. Filmes que contenham TODOS os generos (E)\n";
    cout << "2. Filmes que contenham ALGUM genero (OU)\n";
    op = lerOpcao(1, 2);
    
    vector<string> generos = lerMultiplasOpcoes(
        "Digite os generos (separados por virgula): ");
    
    FiltroLogico operador = (op == 1) ? FiltroLogico::E : FiltroLogico::OU;
    vector<Cinema*> resultado = buscarCinemasPorMultiplosGeneros(cinemas, filmes, generos);
    exibirCinemas(resultado, filmes);
}

void Menu::buscaCinemaPorDuracao(const vector<Filme>& filmes,
                                  const vector<Cinema>& cinemas,
                                  const ListaOrdenadaFilmes& listaOrdenada) {
    cout << "\n=== BUSCA DE CINEMAS POR DURACAO DO FILME ===\n";
    int durMin = lerInteiro("Duração minima (minutos): ");
    int durMax = lerInteiro("Duração maxima (minutos): ");
    
    FiltroFaixa faixa{durMin, durMax};
    vector<Cinema*> resultado = buscarCinemasPorDuracao(cinemas, filmes, durMin, durMax);
    exibirCinemas(resultado, filmes);
}

void Menu::buscaCinemaPorAno(const vector<Filme>& filmes,
                              const vector<Cinema>& cinemas,
                              const ListaOrdenadaFilmes& listaOrdenada) {
    cout << "\n=== BUSCA DE CINEMAS POR ANO DO FILME ===\n";
    int anoMin = lerInteiro("Ano minimo: ");
    int anoMax = lerInteiro("Ano maximo: ");
    
    FiltroFaixa faixa{anoMin, anoMax};
    vector<Cinema*> resultado = buscarCinemasPorAno(cinemas, filmes, anoMin, anoMax);
    exibirCinemas(resultado, filmes);
}

void Menu::buscaCinemaPorTitulo(const vector<Filme>& filmes,
                                 const vector<Cinema>& cinemas) {
    cout << "\n=== BUSCA DE CINEMAS POR TITULO DO FILME ===\n";
    string titulo = lerLinha("Digite o titulo do filme: ");
    
    vector<Cinema*> resultado = buscarCinemasPorTitulo(cinemas, filmes, titulo);
    exibirCinemas(resultado, filmes);
}

void Menu::buscaCinemaCombinada(const vector<Filme>& filmes,
                                 const vector<Cinema>& cinemas,
                                 const vector<IndiceDistancia>& listaBuscaDistancia,
                                 const indicetipo& indicetipo,
                                 const indicegenero& indicegenero,
                                 const ListaOrdenadaFilmes& listaOrdenada) {
    cout << "\n=== BUSCA COMBINADA DE CINEMAS ===\n";
    
    ParametrosBuscaCinemas params;
    
    // Distância
    int usarDist = lerInteiro("\nAdicionar filtro de DISTANCIA? (1=Sim, 0=Nao): ");
    if (usarDist == 1) {
        params.distanciaMax = lerDouble("Distancia maxima (metros): ");
    }
    
    // Tipos de filme
    int usarTipo = lerInteiro("Adicionar filtro de TIPO? (1=Sim, 0=Nao): ");
    if (usarTipo == 1) {
        params.tipos = lerMultiplasOpcoes("Digite os tipos (Movie/TV Series/Short): ");
    }
    
    // Gêneros
    int usarGenero = lerInteiro("Adicionar filtro de GENERO? (1=Sim, 0=Nao): ");
    if (usarGenero == 1) {
        params.generos = lerMultiplasOpcoes("Digite os generos (separados por virgula): ");
    }
    
    // Duração
    int usarDuracao = lerInteiro("Adicionar filtro de DURACAO? (1=Sim, 0=Nao): ");
    if (usarDuracao == 1) {
        params.duracao.minimo = lerInteiro("Duracao minima (minutos): ");
        params.duracao.maximo = lerInteiro("Duracao maxima (minutos): ");
    }
    
    // Ano
    int usarAno = lerInteiro("Adicionar filtro de ANO? (1=Sim, 0=Nao): ");
    if (usarAno == 1) {
        params.ano.minimo = lerInteiro("Ano minimo: ");
        params.ano.maximo = lerInteiro("Ano maximo: ");
    }
    
    // Título
    int usarTitulo = lerInteiro("Adicionar filtro de TITULO? (1=Sim, 0=Nao): ");
    if (usarTitulo == 1) {
        params.titulo = lerLinha("Digite o titulo: ");
    }
    
    vector<Cinema*> resultado = buscarCinemasCombinados(cinemas, filmes,
                                                         listaBuscaDistancia,
                                                         indicetipo, indicegenero,
                                                         params);
    exibirCinemas(resultado, filmes);
}

void Menu::menuBuscaCinemas(const vector<Filme>& filmes,
                             const vector<Cinema>& cinemas,
                             const vector<IndiceDistancia>& listaBuscaDistancia,
                             const indicetipo& indicetipo,
                             const indicegenero& indicegenero,
                             const ListaOrdenadaFilmes& listaOrdenadaDuracao,
                             const ListaOrdenadaFilmes& listaOrdenadaAno) {
    while (true) {
        cout << "\n---- BUSCA DE CINEMAS ----\n";
        cout << "1. Por Distancia                        \n";
        cout << "2. Por Tipo de Filme                    \n";
        cout << "3. Por Genero                           \n";
        cout << "4. Por Duracao de Filme                 \n";
        cout << "5. Por Ano de Filme                     \n";
        cout << "6. Por Titulo de Filme                  \n";
        cout << "7. Busca Combinada                      \n";
        cout << "0. Voltar ao Menu Principal             \n";
        
        int opcao = lerOpcao(0, 7);
        
        switch (opcao) {
            case 1:
                buscaCinemaPorDistancia(filmes, cinemas, listaBuscaDistancia);
                break;
            case 2:
                buscaCinemaPorTipo(filmes, cinemas, indicetipo);
                break;
            case 3:
                buscaCinemaPorGenero(filmes, cinemas, indicegenero);
                break;
            case 4:
                buscaCinemaPorDuracao(filmes, cinemas, listaOrdenadaDuracao);
                break;
            case 5:
                buscaCinemaPorAno(filmes, cinemas, listaOrdenadaDuracao);
                break;
            case 6:
                buscaCinemaPorTitulo(filmes, cinemas);
                break;
            case 7:
                buscaCinemaCombinada(filmes, cinemas, listaBuscaDistancia,
                                     indicetipo, indicegenero, listaOrdenadaDuracao);
                break;
            case 0:
                return;
        }
    }
}
