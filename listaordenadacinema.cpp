#include "listaordenadacinema.hpp"

    ListaOrdenadaCinemas::ListaOrdenadaCinemas(){
    }
    ListaOrdenadaCinemas::ListaOrdenadaCinemas(unsigned int tamanho){
        lista.reserve(tamanho);
    }

    void ListaOrdenadaCinemas::inserir(Cinema* cinema){
        lista.push_back(cinema);
    }

    void ListaOrdenadaCinemas::ordenar(std::string criterio){

    }