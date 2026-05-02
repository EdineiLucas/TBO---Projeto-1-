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
        if(lista.size()>1){
            mergeSort(0, lista.size() - 1, criterio);
        }
    }

    void ListaOrdenadaFilmes::mergeSort(int esquerda, int direita, std::string criterio){
        if(esquerda<direita){
            int meio = esquerda + (direita - esquerda) / 2;
            mergeSort(esquerda, meio, criterio);
            mergeSort(meio + 1, direita, criterio);
            merge(esquerda, meio, direita, criterio);
        }
    }

    void ListaOrdenadaFilmes::merge(int esquerda, int meio, int direita, std::string criterio){
        int n1 = meio - esquerda + 1;
        int n2 = direita - meio;

        std::vector<Filme*> L(n1);
        std::vector<Filme*> R(n2);

        for (int i = 0; i < n1; i++){
            L[i] = lista[esquerda + i]; 
        }
        for (int j = 0; j < n2; j++){
            R[j] = lista[meio + 1 + j];
        }

        int i = 0, j = 0, k = esquerda;
        
        while(i < n1 && j < n2){
            bool condicaoDeTroca = false;

            if(criterio == "ano"){
                condicaoDeTroca = L[i]->getAnoInicio() <= R[j]->getAnoInicio();
            }else if(criterio == "duracao"){
                condicaoDeTroca = L[i]->getTempo() <= R[j]->getTempo();
            }

            if(condicaoDeTroca){
                lista[k] = L[i];
                i++;
            }else{
                lista[k] = R[j];
                j++;
            }
            k++;
        }
        while(i < n1){
            lista[k] = L[i];
            i++;
            k++;
        }
        while (j < n2)
        {
            lista[k] = R[j];
            j++;
            k++;
        }
    }

    std::vector<Filme*> ListaOrdenadaFilmes::buscaPorFaixa(int min, int max, std::string criterio){
        std::vector<Filme*> resultados;

        // Usar busca binária para encontrar os limites da faixa
        auto getValor = [criterio](Filme* f) -> int {
            if(criterio == "ano"){
                return f->getAnoInicio();
            }else if (criterio == "duracao"){
                return f->getTempo();
            }
            return 0;
        };

        // Encontrar o primeiro elemento >= min
        auto it_min = std::lower_bound(lista.begin(), lista.end(), min,
            [getValor](Filme* f, int val) { return getValor(f) < val; });

        // Encontrar o primeiro elemento > max
        auto it_max = std::upper_bound(it_min, lista.end(), max,
            [getValor](int val, Filme* f) { return val < getValor(f); });

        // Copiar os elementos da faixa para o resultado
        for (auto it = it_min; it != it_max; ++it) {
            resultados.push_back(*it);
        }

        return resultados;
    }