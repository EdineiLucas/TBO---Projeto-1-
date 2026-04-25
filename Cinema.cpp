#include "Cinema.hpp"

Cinema::Cinema(unsigned int id, std::string nome, int coordenadaX, int coordenadaY, float precoingresso, std::vector<unsigned int> vectorListCinema){
    this->id = id;
    this->nome = nome;
    this->coordenadaX = coordenadaX;
    this->coordenadaY = coordenadaY;
    this->preco_ingresso = precoingresso;
    this->idFilmes = vectorListCinema;
}

int Cinema::getId() const{
    return this->id;
}

std::string Cinema::getNome()const{
    return this->nome;
}

int Cinema::getCoordenadaX()const{
    return this->coordenadaX;
}

int Cinema::getCoordenadaY()const{
    return this->coordenadaY;
}

float Cinema::getPrecoIngresso()const{
    return this->preco_ingresso;
}

std::vector<unsigned int> Cinema::getFilmesEmCartaz()const{
    return this->idFilmes;
}

void Cinema::setPrecoIngresso(float preco){
    this->preco_ingresso = preco;
}

void Cinema::setListaCinema(std::vector<unsigned int> listaFilmes){
    this->idFilmes = listaFilmes;
}