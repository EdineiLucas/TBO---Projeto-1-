#include "Uteis.hpp"
#include <algorithm>

using namespace std;

int converterInt(const string &texto){
    if(texto.empty() || texto == "\\N"){
        return 0;
    }
    return stoi(texto);
}

void mergeDistancia(vector<IndiceDistancia> & arr, int esquerda, int meio, int direita){
    int n1 = meio - esquerda + 1;
    int n2 =  direita - meio - 1;
    vector<IndiceDistancia> L(n1);
    vector<IndiceDistancia> R(n2);

    for (int i = 0; i < n1; i++){
        L[i] = arr[esquerda + i];
    } 
    for (int j = 0; j < n2; j++){
        R[j] = arr[meio + 1 + j];
    } 

    int i = 0, j = 0, k = esquerda;
    while (i < n1 && j < n2) {
        if (L[i].distancia <= R[j].distancia){
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) { 
        arr[k] = L[i];
        i++;
        k++;
     }
    while (j < n2) {
        arr[k] = R[j];
        j++; 
        k++; 
    }
}

void mergeSortDistancia(vector<IndiceDistancia>& arr, int esquerda, int direita) {
    if (esquerda < direita) {
        int meio = esquerda + (direita - esquerda) / 2;
        mergeSortDistancia(arr, esquerda, meio);
        mergeSortDistancia(arr, meio + 1, direita);
        mergeDistancia(arr, esquerda, meio, direita);
    }
}

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