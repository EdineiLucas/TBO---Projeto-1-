#include "Indicegenero.hpp"

    indicegenero::indicegenero(){}

    indicegenero::indicegenero(int tamanho){
        listaGenero.resize(tamanho);
    }

    unsigned int indicegenero::calculaHash(string generoDoFIlme){
        unsigned int hashcalculado = 0;
        for(char c : generoDoFIlme){
            hashcalculado += c;
        }
        return hashcalculado % listaGenero.size();
    }

    bool indicegenero::inserir(Filme* filme){
         bool check = false;
        for (string genero : filme->getGenero()){
            unsigned int hash = calculaHash(genero);
            if (hash >= 0 && (hash<listaGenero.size())){
                listaGenero[hash].push_back(filme);
                check = true;
            }else{
                cout << "Erro hash genero inserir " << hash << endl; 
            }
        }
        return check;
    }

    list<Filme*> indicegenero::busca(unsigned int hash){
        return listaGenero.at(hash);
    }
