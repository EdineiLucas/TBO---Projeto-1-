#ifndef FILME_HPP
#define FILME_HPP

#include <string>
#include <vector>

class Filme {
private:
    unsigned int id;
    std::string tipo; // short / tvEpisode / video / tvSeries / movie /
    std::string titulo_primario;
    std::string titulo_original;
    bool classificacao; // 0 pra falso (livre), 1 para verdadeiro (+18)
    unsigned short ano_inicio; 
    unsigned short ano_fim; 
    unsigned short tempo; // em minutos
    std::vector<std::string> genero; // genero(s) do filme

public:
    // Construtor
    Filme(unsigned int id, std::string tipo, std::string titulo_primario, std::string titulo_original, bool classificacao, int ano_inicio, int ano_fim, int tempo, std::vector<std::string> genero);

    // Getters
    unsigned int getId() const;
    std::string getTipo() const;
    std::string getTituloPrimario() const;
    std::string getTituloOriginal() const;
    bool getClassificacao() const;
    unsigned short getAnoInicio() const;
    unsigned short getAnoFim() const;
    unsigned short getTempo() const;
    std::vector<std::string> getGenero() const;

    // Métodos de Ação
    void adicionarGenero(const std::string& novo_genero);
};

#endif