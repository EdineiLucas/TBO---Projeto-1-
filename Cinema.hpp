#ifndef CINEMA_HPP
#define CINEMA_HPP

#include <string>
#include <vector>
#include <sstream>
#include "Filme.hpp"

class Cinema {
private:
    unsigned int id; // id do cinema
    std::string nome; // nome do cinema
    int coordenadaX; // coordenada x do cinema
    int coordenadaY; // coordenada y do cinema
    float preco_ingresso; // preço do ingresso nesse cinema
    std::vector<unsigned int> idFilmes; // Lista de filmes em cartaz nesse cinema

public:
    // Construtor
    Cinema(unsigned int id, std::string nome, int coordenadaX, int coordenadaY, float preco_ingresso, std::vector<unsigned int> vectorIdFilmes);

    // Getters
    int getId() const;
    std::string getNome() const;
    int getCoordenadaX() const;
    int getCoordenadaY() const;
    float getPrecoIngresso() const;
    
    // Retorna a lista completa de filmes
    std::vector<unsigned int> getFilmesEmCartaz() const;

    // Setters (Métodos para alterar atributos)
    void setPrecoIngresso(float preco);
    void setListaCinema(std::vector<unsigned int> listaDeFilmes);
    
};

#endif // CINEMA_HPP