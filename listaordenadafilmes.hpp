#ifndef LISTAORDENADAFILMES_HPP
#define LISTAORDENADAFILMES_HPP
#include <vector>
#include <iostream>
#include <string>
#include "Filme.hpp"


class ListaOrdenadaFilmes{
    public:
    std::vector<Filme*> lista;
    ListaOrdenadaFilmes();
    ListaOrdenadaFilmes(unsigned int tamanho);

    void inserir(Filme* filme);
    void ordenar(std::string criterio);
};


#endif