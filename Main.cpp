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

vector<Filme> criaVectorFilme(stringstream &dadosfilmes, vector<Filme> &filmes){ //Armazena vetor filmes

    string linha;
    int contadorFilmes = 0;

    while(getline(dadosfilmes, linha)){

        //variaveis de auxilio
        stringstream ss(linha); 
        unsigned int idFilme;
        string tipo, tituloPrimario, tituloOriginal, idCompletoFilme, dadoTrab;
        bool classificacao;
        unsigned short anoInicio, anoFim, tempo;
        vector<string> genero;

        //pegando cada dado
        getline(ss, idCompletoFilme, '\t'); //Id
        // Verifica se a string tem pelo menos 3 caracteres antes de dar o substr evita linha em branco
        if (idCompletoFilme.length() >= 3) {
            idFilme = stoul(idCompletoFilme.substr(2));
        } else {
            // Caso a linha esteja vazia ou inválida, pula para a próxima iteração
            continue; 
        }
        
        getline(ss, tipo, '\t'); //Tipo
        getline(ss, tituloPrimario, '\t'); // titulo primario
        getline(ss, tituloOriginal, '\t'); // titulo original
        
        getline(ss, dadoTrab, '\t'); // classificação
        classificacao = (converterInt(dadoTrab) == 1);
        
        getline(ss, dadoTrab, '\t'); // ano incio
        anoInicio = converterInt(dadoTrab);
        
        getline(ss, dadoTrab, '\t'); // ano fim
        anoFim = converterInt(dadoTrab);
        
        getline(ss, dadoTrab, '\t'); // tempo
        tempo = converterInt(dadoTrab); 

        getline(ss, dadoTrab, '\t'); //Vai analisar o caso de genero(os)

        //passando pelos casos de genero
        if(dadoTrab != "\\N" && !dadoTrab.empty()){
            stringstream ssGeneros(dadoTrab);
            string genenoIndividual;
            
            while(getline(ssGeneros, genenoIndividual, ',')){
                genero.push_back(genenoIndividual);
            }    
        }

        //criando e salvando os filmes
        Filme novoFilme(idFilme, tipo, tituloPrimario, tituloOriginal, classificacao, anoInicio, anoFim, tempo, genero);
        contadorFilmes ++;
        filmes.push_back(novoFilme);
    }

    cout << "Numero de filmes" << contadorFilmes << endl;
    return filmes;
}

vector<Cinema> criaVectorCinema(stringstream &dadosCinemas, vector<Cinema> &cinemas){
    
    string linha;
    while(getline(dadosCinemas, linha)){

        //variaveis de auxilio
        stringstream ss(linha);
        string dadoTrab, dadoTrab2;
        string idCinema, nomeCinema;
        int coordX, coordY;
        float preco;
        vector<unsigned int> listaIdFilmes;

        //pegando cada informação
        getline(ss, idCinema, ','); //ID
        idCinema = idCinema.substr(2);
        ss>>ws;
        
        getline(ss, nomeCinema, ','); //Nome
        ss>>ws;
        
        getline(ss, dadoTrab, ','); //Coordenadax
        coordX = stoi(dadoTrab);
        ss>>ws;
        
        getline(ss, dadoTrab, ','); //CoordenadaY
        coordY = stoi(dadoTrab);
        ss>>ws;
        
        getline(ss, dadoTrab, ','); //Preco
        preco = stof(dadoTrab);
        ss>>ws;

        //pegando os filmes
        while(getline(ss, dadoTrab, ',')){ //Lista de filmes em cartaz
            dadoTrab2 = dadoTrab.substr(2);
            listaIdFilmes.push_back(stoul(dadoTrab2));
            ss>>ws;
        }

        //cria novo cinema e adiciona ao vetor de cinemas
        Cinema novoCinema(stoul(idCinema), nomeCinema, coordX, coordY, preco, listaIdFilmes);
        cinemas.push_back(novoCinema);
    }
    return cinemas;
}


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
    cout << "DEBUG 4: Lista Filme DURACAO ORDENADA criada" << endl;

    //Cria lista filmes ordenada para buscar por ano
    ListaOrdenadaFilmes filmesOrdenadosAno = (filmes.size());
    for(unsigned int i = 0; i < filmes.size(); i++){
        filmesOrdenadosAno.inserir(&filmes.at(i));
    }
    filmesOrdenadosAno.ordenar("ano");
    cout << "DEBUG 5: Lista Filme ANO ORDENADA criada" << endl;

    //Cria lista de Cinemas ornadada para busca por preco
    ListaOrdenadaCinemas cinemasOdenadosPorPreco(cinemas.size());
    for(unsigned int i=0; i<cinemas.size(); i++){
        cinemasOdenadosPorPreco.inserir(&cinemas.at(i));
    }
    cinemasOdenadosPorPreco.ordenar("preco");
    cout << "DEBUG 6: Lista Cinema PRECO ORDENADA criada" << endl;

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

    cout << "DEBUG 7: Lista DISTANCIA ORDENADA criada" << endl;
    cout << "DEBUG 8: Chegou perto do fim" << endl;

    auto fim = chrono::high_resolution_clock::now();
    chrono::duration<double> tempoEmS = fim - inicio;

    cout << "Tempo em segundos: " << tempoEmS.count() << endl;
    
    return 0;
}