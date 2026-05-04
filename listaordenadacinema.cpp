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
    mergeSort(0, lista.size(), criterio);
}

void ListaOrdenadaCinemas::mergeSort(int esquerda, int direita, std::string criterio){
    if(esquerda < direita){
        int meio = esquerda + (direita - esquerda) / 2;
        mergeSort(esquerda, meio, criterio);
        mergeSort(meio + 1, direita, criterio);
        merge(esquerda, meio, direita, criterio);
    }
}

void ListaOrdenadaCinemas::merge(int esquerda, int meio, int direita, std::string criterio){
    int n1 = meio - esquerda + 1;
    int n2 = direita - meio - 1;

    std::vector<Cinema*> L(n1);
    std::vector<Cinema*> R(n2);

    for (int i = 0; i < n1; i++){
        L[i] = lista[esquerda + i];
    }
    for (int j = 0; j <n2; j++){
        R[j] = lista[meio + 1 + j]; 
    }

    int i = 0, j = 0, k = esquerda;
    while(i < n1 && j < n2){
        bool condicaoDeTroca = false;

        if(criterio == "preco"){
            condicaoDeTroca = L[i]->getPrecoIngresso() <= R[j]->getPrecoIngresso();
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
    while (j < n2){
        lista[k] = R[j];
        j++;
        k++;
    }
}