#include "Uteis.hpp"


using namespace std;

int converterInt(const string &texto){
    if(texto.empty() || texto == "\\N"){
        return 0;
    }
    return stoi(texto);
}