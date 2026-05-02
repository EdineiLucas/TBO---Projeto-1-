#ifndef LISTAORDENADAFILMES_HPP
#define LISTAORDENADAFILMES_HPP
#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include "Filme.hpp"


class ListaOrdenadaFilmes{
    private:
    void mergeSort(int esquerda, int direita, std::string criterio);
    void merge(int esquerda, int meio, int direita, std::string criterio);
    
    public:
    std::vector<Filme*> lista;
    ListaOrdenadaFilmes();
    ListaOrdenadaFilmes(unsigned int tamanho);

    void inserir(Filme* filme);
    void ordenar(std::string criterio);

    std::vector<Filme*> buscaPorFaixa(int min, int max, std::string criterio);
};


#endif