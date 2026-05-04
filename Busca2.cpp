#include "Busca2.hpp"
#include "Indicegenero.hpp"
#include "Indicetipo.hpp"
#include "Uteis.hpp"
#include <list>
#include <utility>

namespace {
    // =====================================================================
    // FUNÇÕES AUXILIARES PARA MERGE SORT
    // =====================================================================

    void mergeFilmesPorId(std::vector<Filme*>& vetor, int esquerda, int meio, int direita) {
        int n1 = meio - esquerda + 1;
        int n2 = direita - meio;

        std::vector<Filme*> L(n1);
        std::vector<Filme*> R(n2);

        for (int i = 0; i < n1; i++) {
            L[i] = vetor[esquerda + i];
        }
        for (int j = 0; j < n2; j++) {
            R[j] = vetor[meio + 1 + j];
        }

        int i = 0, j = 0, k = esquerda;
        while (i < n1 && j < n2) {
            if (L[i]->getId() <= R[j]->getId()) {
                vetor[k] = L[i];
                i++;
            } else {
                vetor[k] = R[j];
                j++;
            }
            k++;
        }

        while (i < n1) {
            vetor[k] = L[i];
            i++;
            k++;
        }

        while (j < n2) {
            vetor[k] = R[j];
            j++;
            k++;
        }
    }

    void mergeSortFilmesPorId(std::vector<Filme*>& vetor, int esquerda, int direita) {
        if (esquerda < direita) {
            int meio = esquerda + (direita - esquerda) / 2;
            mergeSortFilmesPorId(vetor, esquerda, meio);
            mergeSortFilmesPorId(vetor, meio + 1, direita);
            mergeFilmesPorId(vetor, esquerda, meio, direita);
        }
    }

    void mergeCinemasPorId(std::vector<Cinema*>& vetor, int esquerda, int meio, int direita) {
        int n1 = meio - esquerda + 1;
        int n2 = direita - meio;

        std::vector<Cinema*> L(n1);
        std::vector<Cinema*> R(n2);

        for (int i = 0; i < n1; i++) {
            L[i] = vetor[esquerda + i];
        }
        for (int j = 0; j < n2; j++) {
            R[j] = vetor[meio + 1 + j];
        }

        int i = 0, j = 0, k = esquerda;
        while (i < n1 && j < n2) {
            if (L[i]->getId() <= R[j]->getId()) {
                vetor[k] = L[i];
                i++;
            } else {
                vetor[k] = R[j];
                j++;
            }
            k++;
        }

        while (i < n1) {
            vetor[k] = L[i];
            i++;
            k++;
        }

        while (j < n2) {
            vetor[k] = R[j];
            j++;
            k++;
        }
    }

    void mergeSortCinemasPorId(std::vector<Cinema*>& vetor, int esquerda, int direita) {
        if (esquerda < direita) {
            int meio = esquerda + (direita - esquerda) / 2;
            mergeSortCinemasPorId(vetor, esquerda, meio);
            mergeSortCinemasPorId(vetor, meio + 1, direita);
            mergeCinemasPorId(vetor, esquerda, meio, direita);
        }
    }
}

std::vector<Filme*> ordenarFilmesPorId(const std::vector<Filme*>& filmes) {
    std::vector<Filme*> ordenado = filmes;
    if (ordenado.size() > 1) {
        mergeSortFilmesPorId(ordenado, 0, ordenado.size() - 1);
    }
    return ordenado;
}

std::vector<Cinema*> ordenarCinemasPorId(const std::vector<Cinema*>& cinemas) {
    std::vector<Cinema*> ordenado = cinemas;
    if (ordenado.size() > 1) {
        mergeSortCinemasPorId(ordenado, 0, ordenado.size() - 1);
    }
    return ordenado;
}

Filme* buscarFilmePorIdOrdenado(const std::vector<Filme*>& filmesOrdenados, unsigned int id_procurado) {
    int esquerda = 0;
    int direita = filmesOrdenados.size() - 1;

    while (esquerda <= direita) {
        int meio = esquerda + (direita - esquerda) / 2;
        unsigned int idMeio = filmesOrdenados[meio]->getId();

        if (idMeio == id_procurado) {
            return filmesOrdenados[meio];
        } else if (idMeio < id_procurado) {
            esquerda = meio + 1;
        } else {
            direita = meio - 1;
        }
    }

    return nullptr;
}

Cinema* buscarCinemaPorIdOrdenado(const std::vector<Cinema*>& cinemasOrdenados, unsigned int id_procurado) {
    int esquerda = 0;
    int direita = cinemasOrdenados.size() - 1;

    while (esquerda <= direita) {
        int meio = esquerda + (direita - esquerda) / 2;
        unsigned int idMeio = cinemasOrdenados[meio]->getId();

        if (idMeio == id_procurado) {
            return cinemasOrdenados[meio];
        } else if (idMeio < id_procurado) {
            esquerda = meio + 1;
        } else {
            direita = meio - 1;
        }
    }

    return nullptr;
}

std::vector<Filme*> intersecaoFilmesOrdenada(const std::vector<Filme*>& listaA, const std::vector<Filme*>& listaB) {
    if (listaA.empty() || listaB.empty()) {
        return {};
    }

    std::vector<Filme*> a = ordenarFilmesPorId(listaA);
    std::vector<Filme*> b = ordenarFilmesPorId(listaB);
    std::vector<Filme*> resultado;

    size_t i = 0;
    size_t j = 0;
    while (i < a.size() && j < b.size()) {
        unsigned int idA = a[i]->getId();
        unsigned int idB = b[j]->getId();

        if (idA < idB) {
            ++i;
        } else if (idB < idA) {
            ++j;
        } else {
            if (resultado.empty() || resultado.back()->getId() != idA) {
                resultado.push_back(a[i]);
            }
            while (i < a.size() && a[i]->getId() == idA) {
                ++i;
            }
            while (j < b.size() && b[j]->getId() == idB) {
                ++j;
            }
        }
    }

    return resultado;
}

std::vector<Cinema*> intersecaoCinemasOrdenada(const std::vector<Cinema*>& listaA, const std::vector<Cinema*>& listaB) {
    if (listaA.empty() || listaB.empty()) {
        return {};
    }

    std::vector<Cinema*> a = ordenarCinemasPorId(listaA);
    std::vector<Cinema*> b = ordenarCinemasPorId(listaB);
    std::vector<Cinema*> resultado;

    size_t i = 0;
    size_t j = 0;
    while (i < a.size() && j < b.size()) {
        unsigned int idA = a[i]->getId();
        unsigned int idB = b[j]->getId();

        if (idA < idB) {
            ++i;
        } else if (idB < idA) {
            ++j;
        } else {
            if (resultado.empty() || resultado.back()->getId() != idA) {
                resultado.push_back(a[i]);
            }
            while (i < a.size() && a[i]->getId() == idA) {
                ++i;
            }
            while (j < b.size() && b[j]->getId() == idB) {
                ++j;
            }
        }
    }

    return resultado;
}

std::vector<Filme*> uniaoFilmesOrdenada(const std::vector<Filme*>& listaA, const std::vector<Filme*>& listaB) {
    std::vector<Filme*> a = ordenarFilmesPorId(listaA);
    std::vector<Filme*> b = ordenarFilmesPorId(listaB);
    std::vector<Filme*> resultado;

    size_t i = 0;
    size_t j = 0;
    while (i < a.size() && j < b.size()) {
        unsigned int idA = a[i]->getId();
        unsigned int idB = b[j]->getId();

        if (idA < idB) {
            if (resultado.empty() || resultado.back()->getId() != idA) {
                resultado.push_back(a[i]);
            }
            while (i < a.size() && a[i]->getId() == idA) {
                ++i;
            }
        } else if (idB < idA) {
            if (resultado.empty() || resultado.back()->getId() != idB) {
                resultado.push_back(b[j]);
            }
            while (j < b.size() && b[j]->getId() == idB) {
                ++j;
            }
        } else {
            if (resultado.empty() || resultado.back()->getId() != idA) {
                resultado.push_back(a[i]);
            }
            while (i < a.size() && a[i]->getId() == idA) {
                ++i;
            }
            while (j < b.size() && b[j]->getId() == idB) {
                ++j;
            }
        }
    }

    while (i < a.size()) {
        unsigned int idA = a[i]->getId();
        if (resultado.empty() || resultado.back()->getId() != idA) {
            resultado.push_back(a[i]);
        }
        while (i < a.size() && a[i]->getId() == idA) {
            ++i;
        }
    }

    while (j < b.size()) {
        unsigned int idB = b[j]->getId();
        if (resultado.empty() || resultado.back()->getId() != idB) {
            resultado.push_back(b[j]);
        }
        while (j < b.size() && b[j]->getId() == idB) {
            ++j;
        }
    }

    return resultado;
}

std::vector<Cinema*> uniaoCinemasOrdenada(const std::vector<Cinema*>& listaA, const std::vector<Cinema*>& listaB) {
    std::vector<Cinema*> a = ordenarCinemasPorId(listaA);
    std::vector<Cinema*> b = ordenarCinemasPorId(listaB);
    std::vector<Cinema*> resultado;

    size_t i = 0;
    size_t j = 0;
    while (i < a.size() && j < b.size()) {
        unsigned int idA = a[i]->getId();
        unsigned int idB = b[j]->getId();

        if (idA < idB) {
            if (resultado.empty() || resultado.back()->getId() != idA) {
                resultado.push_back(a[i]);
            }
            while (i < a.size() && a[i]->getId() == idA) {
                ++i;
            }
        } else if (idB < idA) {
            if (resultado.empty() || resultado.back()->getId() != idB) {
                resultado.push_back(b[j]);
            }
            while (j < b.size() && b[j]->getId() == idB) {
                ++j;
            }
        } else {
            if (resultado.empty() || resultado.back()->getId() != idA) {
                resultado.push_back(a[i]);
            }
            while (i < a.size() && a[i]->getId() == idA) {
                ++i;
            }
            while (j < b.size() && b[j]->getId() == idB) {
                ++j;
            }
        }
    }

    while (i < a.size()) {
        unsigned int idA = a[i]->getId();
        if (resultado.empty() || resultado.back()->getId() != idA) {
            resultado.push_back(a[i]);
        }
        while (i < a.size() && a[i]->getId() == idA) {
            ++i;
        }
    }

    while (j < b.size()) {
        unsigned int idB = b[j]->getId();
        if (resultado.empty() || resultado.back()->getId() != idB) {
            resultado.push_back(b[j]);
        }
        while (j < b.size() && b[j]->getId() == idB) {
            ++j;
        }
    }

    return resultado;
}

// =========================================================================
// FUNÇÕES AUXILIARES PARA REMOVER DUPLICATAS
// =========================================================================

namespace {
    bool jaAdicionado(const std::vector<Filme*>& resultado, unsigned int id) {
        for (Filme* f : resultado) {
            if (f->getId() == id) {
                return true;
            }
        }
        return false;
    }

    bool jaAdicionadoCinema(const std::vector<Cinema*>& resultado, unsigned int id) {
        for (Cinema* c : resultado) {
            if (c->getId() == id) {
                return true;
            }
        }
        return false;
    }
}

// =========================================================================
// FUNÇÕES DE BUSCA POR FILMES (SEM unordered_set)
// =========================================================================

std::vector<Filme*> buscarFilmesPorTipos_Otimizado(const indicetipo& listaTipos, const std::vector<std::string>& tipos) {
    std::vector<Filme*> resultado;

    for (const std::string& tipo : tipos) {
        unsigned int hash_t = listaTipos.calculaHash(tipo);
        std::list<Filme*> listaHash = listaTipos.busca(hash_t);

        for (Filme* f : listaHash) {
            if (f->getTipo() == tipo && !jaAdicionado(resultado, f->getId())) {
                resultado.push_back(f);
            }
        }
    }

    // Ordenar resultado por ID para garantir consistência
    if (resultado.size() > 1) {
        resultado = ordenarFilmesPorId(resultado);
    }

    return resultado;
}

std::vector<Filme*> buscarFilmesPorGeneros_Otimizado(const indicegenero& listaGeneros, const std::vector<std::string>& generos, bool operadorE) {
    std::vector<Filme*> resultado;

    if (generos.empty()) {
        return resultado;
    }

    if (!operadorE) {
        // Operador OU
        for (const std::string& genero : generos) {
            unsigned int hash_g = listaGeneros.calculaHash(genero);
            std::list<Filme*> listaHash = listaGeneros.busca(hash_g);

            for (Filme* f : listaHash) {
                bool tem_genero = false;
                for (const std::string& g_filme : f->getGenero()) {
                    if (g_filme == genero) {
                        tem_genero = true;
                        break;
                    }
                }

                if (tem_genero && !jaAdicionado(resultado, f->getId())) {
                    resultado.push_back(f);
                }
            }
        }
    } else {
        // Operador E (intersecção)
        std::vector<std::vector<Filme*>> listas;

        for (const std::string& genero : generos) {
            unsigned int hash_g = listaGeneros.calculaHash(genero);
            std::list<Filme*> listaHash = listaGeneros.busca(hash_g);
            std::vector<Filme*> listaTemp;

            for (Filme* f : listaHash) {
                bool tem_genero = false;
                for (const std::string& g_filme : f->getGenero()) {
                    if (g_filme == genero) {
                        tem_genero = true;
                        break;
                    }
                }
                if (tem_genero) {
                    listaTemp.push_back(f);
                }
            }

            if (!listaTemp.empty()) {
                listas.push_back(ordenarFilmesPorId(listaTemp));
            }
        }

        if (!listas.empty()) {
            resultado = listas[0];
            for (size_t i = 1; i < listas.size(); ++i) {
                resultado = intersecaoFilmesOrdenada(resultado, listas[i]);
                if (resultado.empty()) {
                    break;
                }
            }
        }
    }

    // Ordenar resultado por ID
    if (resultado.size() > 1) {
        resultado = ordenarFilmesPorId(resultado);
    }

    return resultado;
}

// =========================================================================
// FUNÇÕES DE BUSCA POR CINEMAS (SEM unordered_set)
// =========================================================================

static void ordenarListasPorTamanho(std::vector<std::vector<Filme*>>& listas) {
    for (size_t i = 0; i + 1 < listas.size(); ++i) {
        size_t menor = i;
        for (size_t j = i + 1; j < listas.size(); ++j) {
            if (listas[j].size() < listas[menor].size()) {
                menor = j;
            }
        }
        if (menor != i) {
            std::vector<Filme*> temp = std::move(listas[i]);
            listas[i] = std::move(listas[menor]);
            listas[menor] = std::move(temp);
        }
    }
}

static void ordenarListasPorTamanho(std::vector<std::vector<Cinema*>>& listas) {
    for (size_t i = 0; i + 1 < listas.size(); ++i) {
        size_t menor = i;
        for (size_t j = i + 1; j < listas.size(); ++j) {
            if (listas[j].size() < listas[menor].size()) {
                menor = j;
            }
        }
        if (menor != i) {
            std::vector<Cinema*> temp = std::move(listas[i]);
            listas[i] = std::move(listas[menor]);
            listas[menor] = std::move(temp);
        }
    }
}

static bool filmeTemTodosGeneros(Filme* filme, const std::vector<std::string>& generos) {
    for (const std::string& genero : generos) {
        bool encontrado = false;
        for (const std::string& gen_filme : filme->getGenero()) {
            if (gen_filme == genero) {
                encontrado = true;
                break;
            }
        }
        if (!encontrado) {
            return false;
        }
    }
    return true;
}

static int buscarPrimeiroIndicePorValor(const std::vector<Filme*>& filmesOrdenados, int valor, bool usarDuracao) {
    int esquerda = 0;
    int direita = (int)filmesOrdenados.size() - 1;
    int resultado = (int)filmesOrdenados.size();

    while (esquerda <= direita) {
        int meio = esquerda + (direita - esquerda) / 2;
        int comparacao = usarDuracao ? filmesOrdenados[meio]->getTempo() : filmesOrdenados[meio]->getAnoInicio();
        if (comparacao >= valor) {
            resultado = meio;
            direita = meio - 1;
        } else {
            esquerda = meio + 1;
        }
    }

    return resultado;
}

static int buscarPrimeiroIndiceMaior(const std::vector<Filme*>& filmesOrdenados, int valor, bool usarDuracao) {
    int esquerda = 0;
    int direita = (int)filmesOrdenados.size() - 1;
    int resultado = (int)filmesOrdenados.size();

    while (esquerda <= direita) {
        int meio = esquerda + (direita - esquerda) / 2;
        int comparacao = usarDuracao ? filmesOrdenados[meio]->getTempo() : filmesOrdenados[meio]->getAnoInicio();
        if (comparacao > valor) {
            resultado = meio;
            direita = meio - 1;
        } else {
            esquerda = meio + 1;
        }
    }

    return resultado;
}

static int buscarPrimeiroIndiceDistancia(const std::vector<IndiceDistancia>& listaBuscaDistancia, double distancia_max) {
    int esquerda = 0;
    int direita = (int)listaBuscaDistancia.size() - 1;
    int resultado = (int)listaBuscaDistancia.size();

    while (esquerda <= direita) {
        int meio = esquerda + (direita - esquerda) / 2;
        if (listaBuscaDistancia[meio].distancia > distancia_max) {
            resultado = meio;
            direita = meio - 1;
        } else {
            esquerda = meio + 1;
        }
    }

    return resultado;
}

std::vector<Filme*> buscarFilmesPorDuracao_Otimizado(const std::vector<Filme*>& filmesOrdenadosDuracao, int dur_min, int dur_max) {
    if (filmesOrdenadosDuracao.empty()) {
        return {};
    }

    int inicio = buscarPrimeiroIndicePorValor(filmesOrdenadosDuracao, dur_min, true);
    int fim = buscarPrimeiroIndiceMaior(filmesOrdenadosDuracao, dur_max, true);

    if (inicio >= (int)filmesOrdenadosDuracao.size() || inicio >= fim) {
        return {};
    }

    return std::vector<Filme*>(filmesOrdenadosDuracao.begin() + inicio, filmesOrdenadosDuracao.begin() + fim);
}

std::vector<Filme*> buscarFilmesPorAno_Otimizado(const std::vector<Filme*>& filmesOrdenadosAno, int ano_min, int ano_max) {
    if (filmesOrdenadosAno.empty()) {
        return {};
    }

    int inicio = buscarPrimeiroIndicePorValor(filmesOrdenadosAno, ano_min, false);
    int fim = buscarPrimeiroIndiceMaior(filmesOrdenadosAno, ano_max, false);

    if (inicio >= (int)filmesOrdenadosAno.size() || inicio >= fim) {
        return {};
    }

    return std::vector<Filme*>(filmesOrdenadosAno.begin() + inicio, filmesOrdenadosAno.begin() + fim);
}

std::vector<Filme*> buscarFilmesCombinados_Otimizado(const indicetipo& listaTipos, const indicegenero& listaGeneros,
    const std::vector<Filme*>& filmesOrdenadosDuracao, const std::vector<Filme*>& filmesOrdenadosAno,
    const ParametrosBuscaFilmes& parametros) {
    std::vector<std::vector<Filme*>> listas;

    if (!parametros.tipos.valores.empty()) {
        listas.push_back(buscarFilmesPorTipos_Otimizado(listaTipos, parametros.tipos.valores));
    }
    if (!parametros.generos.valores.empty()) {
        listas.push_back(buscarFilmesPorGeneros_Otimizado(listaGeneros, parametros.generos.valores, parametros.generos.operador == FiltroLogico::E));
    }
    if (parametros.duracao.minimo != 0 || parametros.duracao.maximo != 0) {
        int max_val = parametros.duracao.maximo > 0 ? parametros.duracao.maximo : 9999;
        listas.push_back(buscarFilmesPorDuracao_Otimizado(filmesOrdenadosDuracao, parametros.duracao.minimo, max_val));
    }
    if (parametros.ano.minimo != 0 || parametros.ano.maximo != 0) {
        int max_val = parametros.ano.maximo > 0 ? parametros.ano.maximo : 2026;
        listas.push_back(buscarFilmesPorAno_Otimizado(filmesOrdenadosAno, parametros.ano.minimo, max_val));
    }

    if (listas.empty()) {
        return {};
    }

    ordenarListasPorTamanho(listas);

    std::vector<Filme*> resultado = listas[0];
    for (size_t i = 1; i < listas.size(); ++i) {
        resultado = intersecaoFilmesOrdenada(resultado, listas[i]);
        if (resultado.empty()) {
            break;
        }
    }

    return resultado;
}

//Busca Por Cinemas

std::vector<Cinema*> buscarCinemasPorDistancia_Otimizado(const std::vector<IndiceDistancia>& listaBuscaDistancia, double distancia_max) {
    std::vector<Cinema*> resultado;
    int fim = buscarPrimeiroIndiceDistancia(listaBuscaDistancia, distancia_max);

    for (int i = 0; i < fim; ++i) {
        resultado.push_back(listaBuscaDistancia[i].cinema);
    }

    return resultado;
}

std::vector<Cinema*> buscarCinemasPorMultiplosTipos_Otimizado(const std::vector<Cinema>& cinemas, const std::vector<Filme*>& filmesOrdenadosPorId, const std::vector<std::string>& tipos) {
    std::vector<Cinema*> resultado;

    for (const Cinema& c : cinemas) {
        bool tem_algum_tipo = false;

        for (unsigned int id_filme : c.getFilmesEmCartaz()) {
            Filme* f = buscarFilmePorIdOrdenado(filmesOrdenadosPorId, id_filme);
            if (f != nullptr) {
                for (const std::string& tipo : tipos) {
                    if (f->getTipo() == tipo) {
                        tem_algum_tipo = true;
                        break;
                    }
                }
            }
            if (tem_algum_tipo) {
                break;
            }
        }

        if (tem_algum_tipo) {
            resultado.push_back(const_cast<Cinema*>(&c));
        }
    }

    return resultado;
}

std::vector<Cinema*> buscarCinemasPorMultiplosGeneros_Otimizado(const std::vector<Cinema>& cinemas, const std::vector<Filme*>& filmesOrdenadosPorId, const std::vector<std::string>& generos, bool operadorE) {
    std::vector<Cinema*> resultado;

    for (const Cinema& c : cinemas) {
        bool cinemaValido = false;

        for (unsigned int id_filme : c.getFilmesEmCartaz()) {
            Filme* f = buscarFilmePorIdOrdenado(filmesOrdenadosPorId, id_filme);
            if (f == nullptr) {
                continue;
            }

            if (!operadorE) {
                for (const std::string& gen_filme : f->getGenero()) {
                    for (const std::string& gen_buscado : generos) {
                        if (gen_filme == gen_buscado) {
                            cinemaValido = true;
                            break;
                        }
                    }
                    if (cinemaValido) {
                        break;
                    }
                }
            } else {
                if (filmeTemTodosGeneros(f, generos)) {
                    cinemaValido = true;
                }
            }

            if (cinemaValido) {
                break;
            }
        }

        if (cinemaValido) {
            resultado.push_back(const_cast<Cinema*>(&c));
        }
    }

    return resultado;
}

std::vector<Cinema*> buscarCinemasPorDuracao_Otimizado(const std::vector<Cinema>& cinemas, const std::vector<Filme*>& filmesOrdenadosPorId, int dur_min, int dur_max) {
    std::vector<Cinema*> resultado;

    for (const Cinema& c : cinemas) {
        bool tem_filme_na_duracao = false;
        for (unsigned int id_filme : c.getFilmesEmCartaz()) {
            Filme* f = buscarFilmePorIdOrdenado(filmesOrdenadosPorId, id_filme);
            if (f != nullptr && f->getTempo() >= dur_min && f->getTempo() <= dur_max) {
                tem_filme_na_duracao = true;
                break;
            }
        }
        if (tem_filme_na_duracao) {
            resultado.push_back(const_cast<Cinema*>(&c));
        }
    }

    return resultado;
}

std::vector<Cinema*> buscarCinemasPorAno_Otimizado(const std::vector<Cinema>& cinemas, const std::vector<Filme*>& filmesOrdenadosPorId, int ano_min, int ano_max) {
    std::vector<Cinema*> resultado;

    for (const Cinema& c : cinemas) {
        bool tem_filme_no_ano = false;
        for (unsigned int id_filme : c.getFilmesEmCartaz()) {
            Filme* f = buscarFilmePorIdOrdenado(filmesOrdenadosPorId, id_filme);
            if (f != nullptr && f->getAnoInicio() >= ano_min && f->getAnoInicio() <= ano_max) {
                tem_filme_no_ano = true;
                break;
            }
        }
        if (tem_filme_no_ano) {
            resultado.push_back(const_cast<Cinema*>(&c));
        }
    }

    return resultado;
}

std::vector<Cinema*> buscarCinemasPorTitulo_Otimizado(const std::vector<Cinema>& cinemas, const std::vector<Filme*>& filmesOrdenadosPorId, const std::string& titulo) {
    std::vector<Cinema*> resultado;

    for (const Cinema& c : cinemas) {
        bool tem_o_filme = false;
        for (unsigned int id_filme : c.getFilmesEmCartaz()) {
            Filme* f = buscarFilmePorIdOrdenado(filmesOrdenadosPorId, id_filme);
            if (f != nullptr && f->getTituloPrimario() == titulo) {
                tem_o_filme = true;
                break;
            }
        }
        if (tem_o_filme) {
            resultado.push_back(const_cast<Cinema*>(&c));
        }
    }

    return resultado;
}

std::vector<Cinema*> buscarCinemasCombinados_Otimizado(const std::vector<Cinema>& cinemas, const std::vector<Filme*>& filmesOrdenadosPorId, const std::vector<IndiceDistancia>& listaBuscaDistancia,
    const indicetipo& listaTipos, const indicegenero& listaGeneros,
    const ParametrosBuscaCinemas& parametros) {
    std::vector<std::vector<Cinema*>> listas;

    if (parametros.distanciaMax >= 0.0) {
        listas.push_back(buscarCinemasPorDistancia_Otimizado(listaBuscaDistancia, parametros.distanciaMax));
    }
    if (!parametros.tipos.empty()) {
        listas.push_back(buscarCinemasPorMultiplosTipos_Otimizado(cinemas, filmesOrdenadosPorId, parametros.tipos));
    }
    if (!parametros.generos.empty()) {
        listas.push_back(buscarCinemasPorMultiplosGeneros_Otimizado(cinemas, filmesOrdenadosPorId, parametros.generos, false));
    }
    if (parametros.duracao.minimo != 0 || parametros.duracao.maximo != 0) {
        int max_val = parametros.duracao.maximo > 0 ? parametros.duracao.maximo : 9999;
        listas.push_back(buscarCinemasPorDuracao_Otimizado(cinemas, filmesOrdenadosPorId, parametros.duracao.minimo, max_val));
    }
    if (parametros.ano.minimo != 0 || parametros.ano.maximo != 0) {
        int max_val = parametros.ano.maximo > 0 ? parametros.ano.maximo : 2026;
        listas.push_back(buscarCinemasPorAno_Otimizado(cinemas, filmesOrdenadosPorId, parametros.ano.minimo, max_val));
    }
    if (!parametros.titulo.empty()) {
        listas.push_back(buscarCinemasPorTitulo_Otimizado(cinemas, filmesOrdenadosPorId, parametros.titulo));
    }

    if (listas.empty()) {
        return {};
    }

    // Ordenar por tamanho das listas para reduzir trabalho de interseção
    for (size_t i = 0; i + 1 < listas.size(); ++i) {
        size_t menor = i;
        for (size_t j = i + 1; j < listas.size(); ++j) {
            if (listas[j].size() < listas[menor].size()) {
                menor = j;
            }
        }
        if (menor != i) {
            std::vector<Cinema*> temp = std::move(listas[i]);
            listas[i] = std::move(listas[menor]);
            listas[menor] = std::move(temp);
        }
    }

    std::vector<Cinema*> resultado = listas[0];
    for (size_t i = 1; i < listas.size(); ++i) {
        resultado = intersecaoCinemasOrdenada(resultado, listas[i]);
        if (resultado.empty()) {
            break;
        }
    }

    return resultado;
}

