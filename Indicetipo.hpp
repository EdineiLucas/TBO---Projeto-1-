#ifndef INDICETIPO_HPP
#define INDICETIPO_HPP

#include <string>
#include "Filme.hpp"
#include "Cinema.hpp"
#include <iostream>
#include <vector>
#include <list>

using namespace std;

class indicetipo{
    private:
    vector<list<Filme*>> listatipo;

    public:

    indicetipo();
    indicetipo(int tamanho);
    unsigned int calculaHash(string tipoDoFilme) const;
    bool inserir(Filme* filme);
    list<Filme*> busca(unsigned int hash) const;
    
    const vector<list<Filme*>>& getListaTipo() const;

};



#endif 