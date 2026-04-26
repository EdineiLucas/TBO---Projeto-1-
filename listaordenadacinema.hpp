#ifndef LISTAORDENADACINEMAS_HPP
#define LISTAORDENADACINEMAS_HPP
#include <vector>
#include <iostream>
#include <string>
#include "Cinema.hpp"


class ListaOrdenadaCinemas{
    private:
    void mergeSort(int esquerda, int direita, std::string criterio);
    void merge(int esquerda, int meio, int direita, std::string criterio);
    
    public:
    std::vector<Cinema*> lista;
    ListaOrdenadaCinemas();
    ListaOrdenadaCinemas(unsigned int tamanho);

    void inserir(Cinema* cinema);
    void ordenar(std::string criterio);
};


#endif