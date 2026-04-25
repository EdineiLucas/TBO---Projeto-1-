#ifndef INDICEGENERO_HPP
#define INDICEGENERO_HPP

#include <string>
#include "Filme.hpp"
#include "Cinema.hpp"
#include <sstream>
#include <iostream>
#include <vector>
#include <list>

using namespace std;

class indicegenero{
    private:
    vector<list<Filme*>> listaGenero;

    public:

    indicegenero();
    indicegenero(int tamanho);
    unsigned int calculaHash(string generoDoFilme);
    bool inserir(Filme* filme);
    list<Filme*> busca(unsigned int hash);

};



#endif 