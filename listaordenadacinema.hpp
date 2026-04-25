#ifndef LISTAORDENADACINEMAS_HPP
#define LISTAORDENADACINEMAS_HPP
#include <vector>
#include <iostream>
#include <string>
#include "Cinema.hpp"


class ListaOrdenadaCinemas{
    public:
    std::vector<Cinema*> lista;
    ListaOrdenadaCinemas();
    ListaOrdenadaCinemas(unsigned int tamanho);

    void inserir(Cinema* cinema);
    void ordenar(std::string criterio);
};


#endif