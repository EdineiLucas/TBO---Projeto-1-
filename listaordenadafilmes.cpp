    #include "listaordenadafilmes.hpp"

    
    ListaOrdenadaFilmes::ListaOrdenadaFilmes(){

    }
    ListaOrdenadaFilmes::ListaOrdenadaFilmes(unsigned int tamanho){
        lista.reserve(tamanho);
    }

    void ListaOrdenadaFilmes::inserir(Filme* filme){
        lista.push_back(filme);
    }
    void ListaOrdenadaFilmes::ordenar(std::string criterio){
        
    }