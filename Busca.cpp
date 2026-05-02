#include "Busca.hpp"
#include "Indicegenero.hpp"
#include "Indicetipo.hpp"
#include "Uteis.hpp"
#include <algorithm>
#include <unordered_set>

using namespace std;

// HashMap global para armazenar mapeamento ID -> Filme*
static std::unordered_map<unsigned int, Filme*> mapaFilmesId;

// =========================================================================
// INICIALIZAÇÃO DO MAPA DE FILMES (ESSENCIAL)
// =========================================================================

void inicializarMapaFilmes(const std::vector<Filme>& filmes) {
    mapaFilmesId.clear();
    for (const Filme& f : filmes) {
        mapaFilmesId[f.getId()] = const_cast<Filme*>(&f);
    }
}

Filme* buscarFilmePorId(vector<Filme>& filmes, unsigned int id_procurado) {
    auto it = mapaFilmesId.find(id_procurado);
    if (it != mapaFilmesId.end()) {
        return it->second;
    }
    return nullptr;
}

// =========================================================================
// INTERSEÇÃO / UNIÃO
// =========================================================================

vector<Filme*> intersecaoFilmes_Otimizada(const vector<Filme*>& listaA, const vector<Filme*>& listaB) {
    vector<Filme*> resultado;
    if (listaA.empty() || listaB.empty()) {
        return resultado;
    }

    const vector<Filme*> &menor = (listaA.size() <= listaB.size()) ? listaA : listaB;
    const vector<Filme*> &maior = (listaA.size() <= listaB.size()) ? listaB : listaA;

    unordered_set<unsigned int> idsMenor;
    idsMenor.reserve(menor.size());
    for (Filme* f : menor) {
        idsMenor.insert(f->getId());
    }

    for (Filme* f : maior) {
        if (idsMenor.count(f->getId())) {
            resultado.push_back(f);
        }
    }

    return resultado;
}

vector<Filme*> uniaoFilmes(const vector<Filme*>& listaA, const vector<Filme*>& listaB) {
    vector<Filme*> resultado;
    unordered_set<unsigned int> ids;
    ids.reserve(listaA.size() + listaB.size());

    for (Filme* f : listaA) {
        if (ids.insert(f->getId()).second) {
            resultado.push_back(f);
        }
    }
    for (Filme* f : listaB) {
        if (ids.insert(f->getId()).second) {
            resultado.push_back(f);
        }
    }

    return resultado;
}

vector<Cinema*> intersecaoCinemas_Otimizada(const vector<Cinema*>& listaA, const vector<Cinema*>& listaB) {
    vector<Cinema*> resultado;
    if (listaA.empty() || listaB.empty()) {
        return resultado;
    }

    const vector<Cinema*> &menor = (listaA.size() <= listaB.size()) ? listaA : listaB;
    const vector<Cinema*> &maior = (listaA.size() <= listaB.size()) ? listaB : listaA;

    unordered_set<Cinema*> setMenor;
    setMenor.reserve(menor.size());
    for (Cinema* c : menor) {
        setMenor.insert(c);
    }

    for (Cinema* c : maior) {
        if (setMenor.count(c)) {
            resultado.push_back(c);
        }
    }

    return resultado;
}

vector<Cinema*> uniaoCinemas(const vector<Cinema*>& listaA, const vector<Cinema*>& listaB) {
    vector<Cinema*> resultado;
    unordered_set<Cinema*> ids;
    ids.reserve(listaA.size() + listaB.size());

    for (Cinema* c : listaA) {
        if (ids.insert(c).second) {
            resultado.push_back(c);
        }
    }
    for (Cinema* c : listaB) {
        if (ids.insert(c).second) {
            resultado.push_back(c);
        }
    }

    return resultado;
}

// =========================================================================
// AUXILIARES
// =========================================================================

vector<string> dividirString(const string& str, char delimitador) {
    vector<string> resultado;
    stringstream ss(str);
    string token;

    while (getline(ss, token, delimitador)) {
        token.erase(token.begin(), find_if(token.begin(), token.end(), [](unsigned char ch) {
            return !isspace(ch);
        }));
        token.erase(find_if(token.rbegin(), token.rend(), [](unsigned char ch) {
            return !isspace(ch);
        }).base(), token.end());
        if (!token.empty()) {
            resultado.push_back(token);
        }
    }

    return resultado;
}

// =========================================================================
// BUSCA POR FILMES
// =========================================================================

vector<Filme*> buscarFilmesPorTipos(const indicetipo& listaTipos, const vector<string>& tipos) {
    vector<Filme*> resultado;
    unordered_set<unsigned int> ids;
    ids.reserve(tipos.size() * 4);

    for (const string& tipo : tipos) {
        unsigned int hash_t = listaTipos.calculaHash(tipo);
        for (Filme* f : listaTipos.busca(hash_t)) {
            if (f->getTipo() == tipo && ids.insert(f->getId()).second) {
                resultado.push_back(f);
            }
        }
    }

    return resultado;
}

vector<Filme*> buscarFilmesPorGeneros(const indicegenero& listaGeneros, const vector<string>& generos, FiltroLogico operador) {
    vector<Filme*> resultado;
    if (generos.empty()) {
        return resultado;
    }

    if (operador == FiltroLogico::OU) {
        unordered_set<unsigned int> ids;
        ids.reserve(generos.size() * 4);

        for (const string& genero : generos) {
            unsigned int hash_g = listaGeneros.calculaHash(genero);
            for (Filme* f : listaGeneros.busca(hash_g)) {
                if (ids.count(f->getId()) == 0) {
                    for (const string& g_filme : f->getGenero()) {
                        if (g_filme == genero) {
                            ids.insert(f->getId());
                            resultado.push_back(f);
                            break;
                        }
                    }
                }
            }
        }
        return resultado;
    }

    unordered_map<unsigned int, int> contador;
    contador.reserve(generos.size() * 4);
    int alvo = generos.size();

    for (const string& genero : generos) {
        unsigned int hash_g = listaGeneros.calculaHash(genero);
        for (Filme* f : listaGeneros.busca(hash_g)) {
            bool tem = false;
            for (const string& g_filme : f->getGenero()) {
                if (g_filme == genero) {
                    tem = true;
                    break;
                }
            }
            if (tem) {
                contador[f->getId()]++;
            }
        }
    }

    for (const string& genero : generos) {
        unsigned int hash_g = listaGeneros.calculaHash(genero);
        for (Filme* f : listaGeneros.busca(hash_g)) {
            if (contador[f->getId()] == alvo) {
                bool ja_adicionado = false;
                for (Filme* existente : resultado) {
                    if (existente->getId() == f->getId()) {
                        ja_adicionado = true;
                        break;
                    }
                }
                if (!ja_adicionado) {
                    resultado.push_back(f);
                }
            }
        }
    }

    return resultado;
}

static int valorPorCriterio(Filme* filme, bool porAno) {
    return porAno ? filme->getAnoInicio() : filme->getTempo();
}

vector<Filme*> buscarFilmesPorDuracao(const vector<Filme*>& filmesOrdenadosDuracao, int dur_min, int dur_max) {
    if (filmesOrdenadosDuracao.empty()) {
        return {};
    }

    auto it_min = std::lower_bound(filmesOrdenadosDuracao.begin(), filmesOrdenadosDuracao.end(), dur_min,
        [](Filme* f, int valor) {
            return f->getTempo() < valor;
        });

    auto it_max = std::upper_bound(it_min, filmesOrdenadosDuracao.end(), dur_max,
        [](int valor, Filme* f) {
            return valor < f->getTempo();
        });

    return vector<Filme*>(it_min, it_max);
}

vector<Filme*> buscarFilmesPorAno(const vector<Filme*>& filmesOrdenadosAno, int ano_min, int ano_max) {
    if (filmesOrdenadosAno.empty()) {
        return {};
    }

    auto it_min = std::lower_bound(filmesOrdenadosAno.begin(), filmesOrdenadosAno.end(), ano_min,
        [](Filme* f, int valor) {
            return f->getAnoInicio() < valor;
        });

    auto it_max = std::upper_bound(it_min, filmesOrdenadosAno.end(), ano_max,
        [](int valor, Filme* f) {
            return valor < f->getAnoInicio();
        });

    return vector<Filme*>(it_min, it_max);
}

vector<Filme*> buscarFilmesCombinados(const indicetipo& listaTipos, const indicegenero& listaGeneros,
    const vector<Filme*>& filmesOrdenadosDuracao, const vector<Filme*>& filmesOrdenadosAno,
    const ParametrosBuscaFilmes& parametros) {
    vector<vector<Filme*>> listas;

    if (!parametros.tipos.valores.empty()) {
        listas.push_back(buscarFilmesPorTipos(listaTipos, parametros.tipos.valores));
    }
    if (!parametros.generos.valores.empty()) {
        listas.push_back(buscarFilmesPorGeneros(listaGeneros, parametros.generos.valores, parametros.generos.operador));
    }
    if (parametros.duracao.minimo != 0 || parametros.duracao.maximo != 0) {
        int max_val = parametros.duracao.maximo > 0 ? parametros.duracao.maximo : 9999;
        listas.push_back(buscarFilmesPorDuracao(filmesOrdenadosDuracao, parametros.duracao.minimo, max_val));
    }
    if (parametros.ano.minimo != 0 || parametros.ano.maximo != 0) {
        int max_val = parametros.ano.maximo > 0 ? parametros.ano.maximo : 2026;
        listas.push_back(buscarFilmesPorAno(filmesOrdenadosAno, parametros.ano.minimo, max_val));
    }

    if (listas.empty()) {
        return {};
    }

    sort(listas.begin(), listas.end(), [](const vector<Filme*>& a, const vector<Filme*>& b) {
        return a.size() < b.size();
    });

    vector<Filme*> resultado = listas[0];
    for (size_t i = 1; i < listas.size(); ++i) {
        resultado = intersecaoFilmes_Otimizada(resultado, listas[i]);
        if (resultado.empty()) {
            break;
        }
    }

    return resultado;
}

vector<Cinema*> buscarCinemasPorDistancia(const vector<IndiceDistancia>& listaBuscaDistancia, double distancia_max) {
    vector<Cinema*> resultado;
    for (const IndiceDistancia& item : listaBuscaDistancia) {
        if (item.distancia <= distancia_max) {
            resultado.push_back(item.cinema);
        } else {
            break;
        }
    }
    return resultado;
}

vector<Cinema*> buscarCinemasPorMultiplosTipos(const vector<Cinema>& cinemas, const vector<Filme>& filmes, const vector<string>& tipos) {
    vector<Cinema*> resultado;
    
    for (const Cinema& c : cinemas) {
        bool tem_algum_tipo = false;
        
        for (unsigned int id_filme : c.getFilmesEmCartaz()) {
            Filme* f = buscarFilmePorId(const_cast<vector<Filme>&>(filmes), id_filme);
            if (f != nullptr) {
                for (const string& tipo : tipos) {
                    if (f->getTipo() == tipo) {
                        tem_algum_tipo = true;
                        break;
                    }
                }
                if (tem_algum_tipo) break;
            }
        }
        
        if (tem_algum_tipo) {
            resultado.push_back(const_cast<Cinema*>(&c));
        }
    }
    
    return resultado;
}

vector<Cinema*> buscarCinemasPorMultiplosGeneros(const vector<Cinema>& cinemas, const vector<Filme>& filmes, const vector<string>& generos) {
    vector<Cinema*> resultado;
    
    for (const Cinema& c : cinemas) {
        bool tem_algum_genero = false;
        
        for (unsigned int id_filme : c.getFilmesEmCartaz()) {
            Filme* f = buscarFilmePorId(const_cast<vector<Filme>&>(filmes), id_filme);
            if (f != nullptr) {
                for (const string& gen_filme : f->getGenero()) {
                    for (const string& gen_buscado : generos) {
                        if (gen_filme == gen_buscado) {
                            tem_algum_genero = true;
                            break;
                        }
                    }
                    if (tem_algum_genero) break;
                }
                if (tem_algum_genero) break;
            }
        }
        
        if (tem_algum_genero) {
            resultado.push_back(const_cast<Cinema*>(&c));
        }
    }
    
    return resultado;
}

vector<Cinema*> buscarCinemasPorDuracao(const vector<Cinema>& cinemas, const vector<Filme>& filmes, int dur_min, int dur_max) {
    vector<Cinema*> resultado;
    
    for (const Cinema& c : cinemas) {
        bool tem_filme_na_duracao = false;
        
        for (unsigned int id_filme : c.getFilmesEmCartaz()) {
            Filme* f = buscarFilmePorId(const_cast<vector<Filme>&>(filmes), id_filme);
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

vector<Cinema*> buscarCinemasPorAno(const vector<Cinema>& cinemas, const vector<Filme>& filmes, int ano_min, int ano_max) {
    vector<Cinema*> resultado;
    
    for (const Cinema& c : cinemas) {
        bool tem_filme_no_ano = false;
        
        for (unsigned int id_filme : c.getFilmesEmCartaz()) {
            Filme* f = buscarFilmePorId(const_cast<vector<Filme>&>(filmes), id_filme);
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

vector<Cinema*> buscarCinemasPorTitulo(const vector<Cinema>& cinemas, const vector<Filme>& filmes, const string& titulo) {
    vector<Cinema*> resultado;
    
    for (const Cinema& c : cinemas) {
        bool tem_o_filme = false;
        
        for (unsigned int id_filme : c.getFilmesEmCartaz()) {
            Filme* f = buscarFilmePorId(const_cast<vector<Filme>&>(filmes), id_filme);
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


vector<Cinema*> buscarCinemasCombinados(const vector<Cinema>& cinemas, const vector<Filme>& filmes,
    const vector<IndiceDistancia>& listaBuscaDistancia,
    const indicetipo& listaTipos, const indicegenero& listaGeneros,
    const ParametrosBuscaCinemas& parametros) {
    vector<vector<Cinema*>> listas;

    if (parametros.distanciaMax >= 0.0) {
        listas.push_back(buscarCinemasPorDistancia(listaBuscaDistancia, parametros.distanciaMax));
    }
    if (!parametros.tipos.empty()) {
        listas.push_back(buscarCinemasPorMultiplosTipos(cinemas, filmes, parametros.tipos));
    }
    if (!parametros.generos.empty()) {
        listas.push_back(buscarCinemasPorMultiplosGeneros(cinemas, filmes, parametros.generos));
    }
    if (parametros.duracao.minimo != 0 || parametros.duracao.maximo != 0) {
        int max_val = parametros.duracao.maximo > 0 ? parametros.duracao.maximo : 9999;
        listas.push_back(buscarCinemasPorDuracao(cinemas, filmes, parametros.duracao.minimo, max_val));
    }
    if (parametros.ano.minimo != 0 || parametros.ano.maximo != 0) {
        int max_val = parametros.ano.maximo > 0 ? parametros.ano.maximo : 2026;
        listas.push_back(buscarCinemasPorAno(cinemas, filmes, parametros.ano.minimo, max_val));
    }
    if (!parametros.titulo.empty()) {
        listas.push_back(buscarCinemasPorTitulo(cinemas, filmes, parametros.titulo));
    }

    if (listas.empty()) {
        return {};
    }

    sort(listas.begin(), listas.end(), [](const vector<Cinema*>& a, const vector<Cinema*>& b) {
        return a.size() < b.size();
    });

    vector<Cinema*> resultado = listas[0];
    for (size_t i = 1; i < listas.size(); ++i) {
        resultado = intersecaoCinemas_Otimizada(resultado, listas[i]);
        if (resultado.empty()) {
            break;
        }
    }

    return resultado;
}
