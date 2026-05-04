#include "Menu.hpp"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <cmath>

using namespace std;

// ============================================================================
// FUNÇÕES AUXILIARES
// ============================================================================

//Leitura da linha para multiplos filtros
string Menu::lerLinha(const string& prompt) {
    string entrada;
    cout << prompt;
    getline(cin, entrada);
    return entrada;
}

//Utilizado para escolha de opções - retorna o valor do input
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

//Utilizado para ler entrada double da busca por distancia
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

//Utilizado para busca de tipo(s) e genero(s) / lê a linha de tipos ou generos e separa os tipo em um vetor de strings (cada posição um tipo)
vector<string> Menu::lerMultiplasOpcoes(const string& prompt) {
    string linha = lerLinha(prompt);
    vector<string> opcoes;
    string opcao;
    
    //Função lambda de auxilio para limpar espaços em braço do começo e do final
    auto trim = [](string& s) {
        while (!s.empty() && isspace((unsigned char)s.front())) s.erase(s.begin());
        while (!s.empty() && isspace((unsigned char)s.back())) s.pop_back();
    };

    //percorre a linha e separa os tipos no vetor opcoes
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

//Utilizado para selecionar uma opção, navegação pelos paineis de filtros
int Menu::lerOpcao(int minimo, int maximo) {
    while (true) {
        int opcao = lerInteiro("Escolha uma opcao: ");
        if (opcao >= minimo && opcao <= maximo) {
            return opcao;
        }
        cout << "Opção inválida. Escolha entre " << minimo << " e " << maximo << ".\n";
    }
}

//Imprime os filmes da busca, utilizada nas funções de busca simples e na função de busca combinada
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

//Imprime os cinemas da busca, segue os mesmos conceitos de exibir filmes, tem um loop final para exibir os filmes em cartaz daquele cinema
void Menu::exibirCinemas(const vector<Cinema*>& cinemas, const vector<Filme*>& filmesOrdenadosPorId, int limite) {
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
                Filme* f = buscarFilmePorIdOrdenado(filmesOrdenadosPorId, id_filme);
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

//Texto do menu inicial
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

//utiliza das funções de busca para buscar por tipo
void Menu::buscaFilmePorTipo(const vector<Filme>& filmes, const indicetipo& indicetipo) {
    cout << "\n=== BUSCA DE FILMES POR TIPO ===\n";
    cout << "Buscar por 1 ou mais tipos?\n";
    cout << "Tipos disponiveis: Movie, TV Series, Short\n";
    vector<string> tipos = lerMultiplasOpcoes("Digite os tipos (separados por virgula): ");
    
    vector<Filme*> resultado = buscarFilmesPorTipos_Otimizado(indicetipo, tipos);
    exibirFilmes(resultado);
}

//Chamada da busca por gênero
void Menu::buscaFilmePorGenero(const vector<Filme>& filmes, const indicegenero& indicegenero) {
    cout << "\n=== BUSCA DE FILMES POR GENERO ===\n";
    int op;
    cout << "1. Filmes que contenham TODOS os generos (E)\n";
    cout << "2. Filmes que contenham ALGUM genero (OU)\n";
    op = lerOpcao(1, 2);
    
    vector<string> generos = lerMultiplasOpcoes(
        "Digite os generos (separados por virgula): ");
    
    bool operadorE = (op == 1);
    vector<Filme*> resultado = buscarFilmesPorGeneros_Otimizado(indicegenero, generos, operadorE);
    exibirFilmes(resultado);
}

//Chamada da busca por duração
void Menu::buscaFilmePorDuracao(const vector<Filme>& filmes, const ListaOrdenadaFilmes& listaOrdenadaDuracao) {
    cout << "\n=== BUSCA DE FILMES POR DURACAO ===\n";
    int durMin = lerInteiro("Duracao minima (minutos): ");
    int durMax = lerInteiro("Duracao maxima (minutos): ");
    
    FiltroFaixa faixa{durMin, durMax};
    vector<Filme*> resultado = buscarFilmesPorDuracao_Otimizado(listaOrdenadaDuracao.lista, faixa.minimo, faixa.maximo);
    exibirFilmes(resultado);
}

//Chamada da busca por ano
void Menu::buscaFilmePorAno(const vector<Filme>& filmes, const ListaOrdenadaFilmes& listaOrdenadaAno) {
    cout << "\n=== BUSCA DE FILMES POR ANO ===\n";
    int anoMin = lerInteiro("Ano minimo: ");
    int anoMax = lerInteiro("Ano maximo: ");
    
    FiltroFaixa faixa{anoMin, anoMax};
    vector<Filme*> resultado = buscarFilmesPorAno_Otimizado(listaOrdenadaAno.lista, faixa.minimo, faixa.maximo);
    exibirFilmes(resultado);
}

//Combinação dos filtros na busca por filmes, utiliza a estrutura de busca para armazenar os dados a serem buscados e direciona a lista certa para cada tipo de busca
void Menu::buscaFilmeCombinada(const vector<Filme>& filmes, const vector<Cinema>& cinemas, const indicetipo& indicetipo, const indicegenero& indicegenero,
                                const ListaOrdenadaFilmes& listaOrdenadaDuracao,
                                const ListaOrdenadaFilmes& listaOrdenadaAno) {
    cout << "\n=== BUSCA COMBINADA DE FILMES ===\n";
    
    ParametrosBuscaFilmes params;
    
    // Tipo
    int usarTipo = lerInteiro("\nAdicionar filtro de TIPO? (1=Sim, 0=Nao): ");
    if (usarTipo == 1) {
        params.tipos.valores = lerMultiplasOpcoes("Digite os tipos (separados por virgula): ");
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
    
    vector<Filme*> resultado = buscarFilmesCombinados_Otimizado(indicetipo, indicegenero,
                                                       listaOrdenadaDuracao.lista, listaOrdenadaAno.lista, params);
    exibirFilmes(resultado);
}

//Menu de busca por filmes chama as funções de buscas individuais ou faz busca combinada
void Menu::menuBuscaFilmes(const vector<Filme>& filmes, const vector<Cinema>& cinemas, const indicetipo& indicetipo, const indicegenero& indicegenero,
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

//busca cinemas com distância menor ou igual a distância de input - Interface de busca
void Menu::buscaCinemaPorDistancia(const vector<Filme*>& filmesOrdenadosPorId, const vector<Cinema>& cinemas, const vector<IndiceDistancia>& listaBuscaDistancia) {
    cout << "\n=== BUSCA DE CINEMAS POR DISTANCIA ===\n";
    double distMax = lerDouble("Distancia maxima (metros): ");
    
    vector<Cinema*> resultado = buscarCinemasPorDistancia_Otimizado(listaBuscaDistancia, distMax);
    exibirCinemas(resultado, filmesOrdenadosPorId);
}

//Interface de buscar cinema por tipo
void Menu::buscaCinemaPorTipo(const vector<Filme*>& filmesOrdenadosPorId, const vector<Cinema>& cinemas, const indicetipo& indicetipo) {
    cout << "\n=== BUSCA DE CINEMAS POR TIPO DE FILME ===\n";
    string tipo = lerLinha("Digite o tipo (Movie/TV Series/Short): ");
    
    vector<string> tipos = {tipo};
    vector<Cinema*> resultado = buscarCinemasPorMultiplosTipos_Otimizado(cinemas, filmesOrdenadosPorId, tipos);
    exibirCinemas(resultado, filmesOrdenadosPorId);
}

void Menu::buscaCinemaPorGenero(const vector<Filme*>& filmesOrdenadosPorId, const vector<Cinema>& cinemas, const indicegenero& indicegenero, bool operadorE) {
    cout << "\n=== BUSCA DE CINEMAS POR GENERO ===\n";
    vector<string> generos = lerMultiplasOpcoes(
        "Digite os generos (separados por virgula): ");
    
    vector<Cinema*> resultado = buscarCinemasPorMultiplosGeneros_Otimizado(cinemas, filmesOrdenadosPorId, generos, operadorE);
    exibirCinemas(resultado, filmesOrdenadosPorId);
}

void Menu::buscaCinemaPorDuracao(const vector<Filme*>& filmesOrdenadosPorId, const vector<Cinema>& cinemas, const ListaOrdenadaFilmes& listaOrdenada) {
    cout << "\n=== BUSCA DE CINEMAS POR DURACAO DO FILME ===\n";
    int durMin = lerInteiro("Duração minima (minutos): ");
    int durMax = lerInteiro("Duração maxima (minutos): ");
    
    vector<Cinema*> resultado = buscarCinemasPorDuracao_Otimizado(cinemas, filmesOrdenadosPorId, durMin, durMax);
    exibirCinemas(resultado, filmesOrdenadosPorId);
}

void Menu::buscaCinemaPorAno(const vector<Filme*>& filmesOrdenadosPorId, const vector<Cinema>& cinemas, const ListaOrdenadaFilmes& listaOrdenada) {
    cout << "\n=== BUSCA DE CINEMAS POR ANO DO FILME ===\n";
    int anoMin = lerInteiro("Ano minimo: ");
    int anoMax = lerInteiro("Ano maximo: ");
    
    vector<Cinema*> resultado = buscarCinemasPorAno_Otimizado(cinemas, filmesOrdenadosPorId, anoMin, anoMax);
    exibirCinemas(resultado, filmesOrdenadosPorId);
}

void Menu::buscaCinemaPorTitulo(const vector<Filme*>& filmesOrdenadosPorId, const vector<Cinema>& cinemas) {
    cout << "\n=== BUSCA DE CINEMAS POR TITULO DO FILME ===\n";
    string titulo = lerLinha("Digite o titulo do filme: ");
    
    vector<Cinema*> resultado = buscarCinemasPorTitulo_Otimizado(cinemas, filmesOrdenadosPorId, titulo);
    exibirCinemas(resultado, filmesOrdenadosPorId);
}

void Menu::buscaCinemaCombinada(const vector<Filme*>& filmesOrdenadosPorId, const vector<Cinema>& cinemas, const vector<IndiceDistancia>& listaBuscaDistancia, const indicetipo& indicetipo,
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
    
    vector<Cinema*> resultado = buscarCinemasCombinados_Otimizado(cinemas, filmesOrdenadosPorId, listaBuscaDistancia, indicetipo, indicegenero, params);
    exibirCinemas(resultado, filmesOrdenadosPorId);
}

//Interface da busca por cinemas onde direciona para cara tipo de busca.
void Menu::menuBuscaCinemas(const vector<Filme*>& filmesOrdenadosPorId, const vector<Cinema>& cinemas, const vector<IndiceDistancia>& listaBuscaDistancia, const indicetipo& indicetipo,
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
                buscaCinemaPorDistancia(filmesOrdenadosPorId, cinemas, listaBuscaDistancia);
                break;
            case 2:
                buscaCinemaPorTipo(filmesOrdenadosPorId, cinemas, indicetipo);
                break;
            case 3: {
                cout << "1. Filmes que contenham TODOS os generos (E)\n";
                cout << "2. Filmes que contenham ALGUM genero (OU)\n";
                int operacao = lerOpcao(1, 2);
                buscaCinemaPorGenero(filmesOrdenadosPorId, cinemas, indicegenero, operacao == 1);
                break;
            }
            case 4:
                buscaCinemaPorDuracao(filmesOrdenadosPorId, cinemas, listaOrdenadaDuracao);
                break;
            case 5:
                buscaCinemaPorAno(filmesOrdenadosPorId, cinemas, listaOrdenadaDuracao);
                break;
            case 6:
                buscaCinemaPorTitulo(filmesOrdenadosPorId, cinemas);
                break;
            case 7:
                buscaCinemaCombinada(filmesOrdenadosPorId, cinemas, listaBuscaDistancia,
                                     indicetipo, indicegenero, listaOrdenadaDuracao);
                break;
            case 0:
                return;
        }
    }
}
