#include "Indicetipo.hpp"

indicetipo::indicetipo(){

}

indicetipo::indicetipo(int tamanho){
    listatipo.resize(tamanho);
}

unsigned int indicetipo::calculaHash(string tipoDoFilme) const {
        
    unsigned int hashcalculado = 0;
    for(char c : tipoDoFilme){
            hashcalculado += c;
    }
    return hashcalculado % listatipo.size();
}

bool indicetipo::inserir(Filme* filme){
    unsigned int hash = calculaHash(filme->getTipo());

    if (hash >= 0 && (hash < listatipo.size())){
        listatipo[hash].push_back(filme);
        return true;
    }
    cout << "Erro hash inserir tipo" << hash << endl;
    return false;
}

list<Filme*> indicetipo::busca(unsigned int hash) const {
    return listatipo.at(hash);
}

const vector<list<Filme*>>& indicetipo::getListaTipo() const {
    return listatipo;
}