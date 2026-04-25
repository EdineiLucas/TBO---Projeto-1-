#include "Filme.hpp"

// Implementação do Construtor
Filme::Filme(unsigned int id, std::string tipo, std::string titulo_primario, std::string titulo_original, 
             bool classificacao, int ano_inicio, int ano_fim, int tempo, std::vector<std::string> genero) {
    this->id = id;
    this->tipo = tipo;
    this->titulo_primario = titulo_primario;
    this->titulo_original = titulo_original;
    this->classificacao = classificacao;
    this->ano_inicio = ano_inicio;
    this->ano_fim = ano_fim;
    this->tempo = tempo;
    this->genero = genero;
}

// Implementação dos Getters
unsigned int Filme::getId() const {
    return this->id;
}

std::string Filme::getTipo() const {
    return tipo;
}

std::string Filme::getTituloPrimario() const {
    return titulo_primario;
}

std::string Filme::getTituloOriginal() const {
    return titulo_original;
}

bool Filme::getClassificacao() const {
    return classificacao;
}

unsigned short Filme::getAnoInicio() const {
    return ano_inicio;
}

unsigned short Filme::getAnoFim() const {
    return ano_fim;
}

unsigned short Filme::getTempo() const {
    return tempo;
}

std::vector<std::string> Filme::getGenero() const {
    return genero;
}

// Implementação dos Métodos de Ação
void Filme::adicionarGenero(const std::string& novo_genero) {
    genero.push_back(novo_genero);
}