#include "Uteis.hpp"
#include <algorithm>
#include <unordered_set>

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

Filme* buscarFilmePorId(vector<Filme>& filmes, unsigned int id_procurado) {
    for (Filme& f : filmes) {
        if (f.getId() == id_procurado) {
            return &f;
        }
    }
    return nullptr;
}

vector<Filme*> intersecaoFilmes(const vector<Filme*>& listaA, vector<Filme*>& listaB) {
    vector<Filme*> resultado;
    
    if (listaA.empty() || listaB.empty()) {
        return resultado;
    }

    for (Filme* fA : listaA) {
        for (Filme* fB : listaB) {
            if (fA->getId() == fB->getId()) {
                resultado.push_back(fA);
                break;
            }
        }
    }
    return resultado;
}

vector<Filme*> intersecaoFilmes_Otimizada(const vector<Filme*>& listaA, const vector<Filme*>& listaB) {
    vector<Filme*> resultado;
    
    if (listaA.empty() || listaB.empty()) {
        return resultado;
    }

    unordered_set<unsigned int> idsA;
    for (Filme* f : listaA) {
        idsA.insert(f->getId());
    }

    for (Filme* f : listaB) {
        if (idsA.count(f->getId())) {
            resultado.push_back(f);
        }
    }
    
    return resultado;
}

vector<string> dividirString(const string& str, char delimitador) {
    vector<string> resultado;
    stringstream ss(str);
    string token;
    
    while (getline(ss, token, delimitador)) {
        // Remove espaços em branco do início e fim
        token.erase(token.begin(), find_if(token.begin(), token.end(), [](unsigned char ch) {
            return !isspace(ch);
        }));
        token.erase(find_if(token.rbegin(), token.rend(), [](unsigned char ch) {
            return !isspace(ch);
        }).base(), token.end());
        
        if (!token.empty()) {
            resultado.push_back(token);
        }
    }
    
    return resultado;
}

vector<Filme*> buscarPorMultiplosTipos(indicetipo& listaTipos, const vector<string>& tipos) {
    vector<Filme*> resultado;
    
    for (const string& tipo : tipos) {
        unsigned int hash_t = listaTipos.calculaHash(tipo);
        for (Filme* f : listaTipos.busca(hash_t)) {
            // Confirmação contra colisão de Hash
            if (f->getTipo() == tipo) {
                // Verifica se já não foi adicionado (evita duplicatas)
                bool ja_adicionado = false;
                for (Filme* existente : resultado) {
                    if (existente->getId() == f->getId()) {
                        ja_adicionado = true;
                        break;
                    }
                }
                if (!ja_adicionado) {
                    resultado.push_back(f);
                }
            }
        }
    }
    
    return resultado;
}

vector<Filme*> buscarPorMultiplosGeneros(indicegenero& listaGeneros, const vector<string>& generos) {
    vector<Filme*> resultado;
    
    for (const string& gen_buscado : generos) {
        unsigned int hash_g = listaGeneros.calculaHash(gen_buscado);
        for (Filme* f : listaGeneros.busca(hash_g)) {
            // Confirmação contra colisão de Hash
            bool tem_genero = false;
            for (const string& g_filme : f->getGenero()) {
                if (g_filme == gen_buscado) {
                    tem_genero = true;
                    break;
                }
            }
            
            if (tem_genero) {
                // Verifica se já não foi adicionado (evita duplicatas)
                bool ja_adicionado = false;
                for (Filme* existente : resultado) {
                    if (existente->getId() == f->getId()) {
                        ja_adicionado = true;
                        break;
                    }
                }
                if (!ja_adicionado) {
                    resultado.push_back(f);
                }
            }
        }
    }
    
    return resultado;
}

vector<Cinema*> buscarCinemasPorMultiplosTipos(const vector<Cinema>& cinemas, const vector<Filme>& filmes, const vector<string>& tipos) {
    vector<Cinema*> resultado;
    
    for (const Cinema& c : cinemas) {
        bool tem_algum_tipo = false;
        
        for (unsigned int id_filme : c.getFilmesEmCartaz()) {
            Filme* f = buscarFilmePorId(const_cast<vector<Filme>&>(filmes), id_filme);
            if (f != nullptr) {
                for (const string& tipo : tipos) {
                    if (f->getTipo() == tipo) {
                        tem_algum_tipo = true;
                        break;
                    }
                }
                if (tem_algum_tipo) break;
            }
        }
        
        if (tem_algum_tipo) {
            // Verifica se já não foi adicionado (evita duplicatas)
            bool ja_adicionado = false;
            for (Cinema* existente : resultado) {
                if (existente == &c) {
                    ja_adicionado = true;
                    break;
                }
            }
            if (!ja_adicionado) {
                resultado.push_back(const_cast<Cinema*>(&c));
            }
        }
    }
    
    return resultado;
}

vector<Cinema*> buscarCinemasPorMultiplosGeneros(const vector<Cinema>& cinemas, const vector<Filme>& filmes, const vector<string>& generos) {
    vector<Cinema*> resultado;
    
    for (const Cinema& c : cinemas) {
        bool tem_algum_genero = false;
        
        for (unsigned int id_filme : c.getFilmesEmCartaz()) {
            Filme* f = buscarFilmePorId(const_cast<vector<Filme>&>(filmes), id_filme);
            if (f != nullptr) {
                for (const string& gen_filme : f->getGenero()) {
                    for (const string& gen_buscado : generos) {
                        if (gen_filme == gen_buscado) {
                            tem_algum_genero = true;
                            break;
                        }
                    }
                    if (tem_algum_genero) break;
                }
                if (tem_algum_genero) break;
            }
        }
        
        if (tem_algum_genero) {
            // Verifica se já não foi adicionado (evita duplicatas)
            bool ja_adicionado = false;
            for (Cinema* existente : resultado) {
                if (existente == &c) {
                    ja_adicionado = true;
                    break;
                }
            }
            if (!ja_adicionado) {
                resultado.push_back(const_cast<Cinema*>(&c));
            }
        }
    }
    
    return resultado;
}

vector<Cinema*> buscarCinemasPorDuracao(const vector<Cinema>& cinemas, const vector<Filme>& filmes, int dur_min, int dur_max) {
    vector<Cinema*> resultado;
    
    for (const Cinema& c : cinemas) {
        bool tem_filme_na_duracao = false;
        
        for (unsigned int id_filme : c.getFilmesEmCartaz()) {
            Filme* f = buscarFilmePorId(const_cast<vector<Filme>&>(filmes), id_filme);
            if (f != nullptr && f->getTempo() >= dur_min && f->getTempo() <= dur_max) {
                tem_filme_na_duracao = true;
                break;
            }
        }
        
        if (tem_filme_na_duracao) {
            resultado.push_back(const_cast<Cinema*>(&c));
        }
    }
    
    return resultado;
}

vector<Cinema*> buscarCinemasPorAno(const vector<Cinema>& cinemas, const vector<Filme>& filmes, int ano_min, int ano_max) {
    vector<Cinema*> resultado;
    
    for (const Cinema& c : cinemas) {
        bool tem_filme_no_ano = false;
        
        for (unsigned int id_filme : c.getFilmesEmCartaz()) {
            Filme* f = buscarFilmePorId(const_cast<vector<Filme>&>(filmes), id_filme);
            if (f != nullptr && f->getAnoInicio() >= ano_min && f->getAnoInicio() <= ano_max) {
                tem_filme_no_ano = true;
                break;
            }
        }
        
        if (tem_filme_no_ano) {
            resultado.push_back(const_cast<Cinema*>(&c));
        }
    }
    
    return resultado;
}

vector<Cinema*> buscarCinemasPorTitulo(const vector<Cinema>& cinemas, const vector<Filme>& filmes, const string& titulo) {
    vector<Cinema*> resultado;
    
    for (const Cinema& c : cinemas) {
        bool tem_o_filme = false;
        
        for (unsigned int id_filme : c.getFilmesEmCartaz()) {
            Filme* f = buscarFilmePorId(const_cast<vector<Filme>&>(filmes), id_filme);
            if (f != nullptr && f->getTituloPrimario() == titulo) {
                tem_o_filme = true;
                break;
            }
        }
        
        if (tem_o_filme) {
            resultado.push_back(const_cast<Cinema*>(&c));
        }
    }
    
    return resultado;
}

vector<Cinema*> intersecaoCinemas(const vector<Cinema*>& listaA, const vector<Cinema*>& listaB) {
    std::vector<Cinema*> resultado;
    
    if (listaA.empty() || listaB.empty()){
        return resultado;
    } 

    for (Cinema* cA : listaA) {
        for (Cinema* cB : listaB) {
            if (cA == cB) { // Compara os endereços de memória!
                resultado.push_back(cA);
                break;
            }
        }
    }
    return resultado;
}

// ====== VERSÃO OTIMIZADA COM UNORDERED_SET ======
vector<Cinema*> intersecaoCinemas_Otimizada(const vector<Cinema*>& listaA, const vector<Cinema*>& listaB) {
    vector<Cinema*> resultado;
    
    if (listaA.empty() || listaB.empty()) {
        return resultado;
    }

    // Passo 1: Cria um hash set com os ponteiros da primeira lista
    unordered_set<Cinema*> cinemasA;
    for (Cinema* c : listaA) {
        cinemasA.insert(c);
    }

    // Passo 2: Percorre a segunda lista e verifica se cada cinema existe no set
    for (Cinema* c : listaB) {
        // count() retorna 1 se encontrado, 0 se não encontrado
        if (cinemasA.count(c)) {
            resultado.push_back(c);
        }
    }
    
    return resultado;
}