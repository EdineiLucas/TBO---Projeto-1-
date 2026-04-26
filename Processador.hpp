/* // verificação do hashlist da lista auxiliar de busca por tipos
    unsigned int hashX = listaFilmesTipo.calculaHash(filmes.at(1).getTipo());
    list<Filme*> listaRetorno = listaFilmesTipo.busca(hashX);
    for(Filme* f : listaRetorno){
        cout << f->getId() << " " << f->getTipo() << endl; 
    } 
    */

    /*  //Verificar se armazenou os filmes
    for (Filme f1 : filmes){
        cout << f1.getId() << "nome: " << f1.getTituloPrimario() << endl;   
    }
*/

/* // Conferindo se armazenou corretamente os cinemas no meu vetor de cinemas
    for (Cinema c1 : cinemas){
        cout << c1.getId() << "," << c1.getNome() << ", Lista de filmes" << endl;
        vector<unsigned int> f1 = c1.getFilmesEmCartaz();
        for (unsigned int f2 : f1){
            cout << f2 << ",";
        }
        cout << endl;

    }
    */