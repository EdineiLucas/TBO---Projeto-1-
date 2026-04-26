#include "Uteis.hpp"

using namespace std;

int converterInt(const string &texto){
    if(texto.empty() || texto == "\\N"){
        return 0;
    }
    return stoi(texto);
}

void mergeDistancia(vector<IndiceDistancia> & arr, int esquerda, int meio, int direita){
    int n1 = meio - esquerda + 1;
    int n2 =  direita - meio - 1;
    vector<IndiceDistancia> L(n1);
    vector<IndiceDistancia> R(n2);

    for (int i = 0; i < n1; i++){
        L[i] = arr[esquerda + i];
    } 
    for (int j = 0; j < n2; j++){
        R[j] = arr[meio + 1 + j];
    } 

    int i = 0, j = 0, k = esquerda;
    while (i < n1 && j < n2) {
        if (L[i].distancia <= R[j].distancia){
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) { 
        arr[k] = L[i];
        i++;
        k++;
     }
    while (j < n2) {
        arr[k] = R[j];
        j++; 
        k++; 
    }
}

void mergeSortDistancia(vector<IndiceDistancia>& arr, int esquerda, int direita) {
    if (esquerda < direita) {
        int meio = esquerda + (direita - esquerda) / 2;
        mergeSortDistancia(arr, esquerda, meio);
        mergeSortDistancia(arr, meio + 1, direita);
        mergeDistancia(arr, esquerda, meio, direita);
    }
}