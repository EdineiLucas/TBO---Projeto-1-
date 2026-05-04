🎬 Sistema de Busca de Filmes e Cinemas

Um sistema de busca otimizado desenvolvido em C++ para consultar um vasto catálogo de filmes e cinemas. O projeto processa grandes volumes de dados de arquivos de texto e utiliza estruturas de dados eficientes, como Tabelas Hash e algoritmos de ordenação, para garantir buscas rápidas em uma interface interativa de linha de comando (CLI).  
✨ Funcionalidades
🎞️ Busca de Filmes

Permite consultar o catálogo de filmes utilizando diversos critérios:

    Por Tipo: Filtragem por categorias exatas, como Movie, TV Series ou Short.  

    Por Gênero: Busca por um ou múltiplos gêneros, suportando operadores lógicos restritivos (E) ou inclusivos (OU).  

    Por Duração e Ano: Busca parametrizada por faixas de tempo e ano de lançamento.  

    Busca Combinada: Aplicação de múltiplos filtros simultaneamente para buscas altamente específicas.  

🍿 Busca de Cinemas

Encontre o cinema ideal cruzando dados de localização e catálogo:

    Distância: Lista cinemas dentro de um raio de distância a partir de uma coordenada sede.    

    Busca Combinada: Integração total dos filtros de filmes com o filtro de distância dos cinemas.  

🏗️ Arquitetura e Estruturas de Dados

O sistema foi projetado com foco em performance computacional:

    Tabelas Hash customizadas: Implementadas nas classes indicegenero e indicetipo para mapeamento e recuperação instantânea O(1) de filmes baseados em strings de texto.  

    Merge Sort: Algoritmo de divisão e conquista implementado do zero para ordenar os vetores de filmes (por ano/duração) e cinemas (por preço/distância) com complexidade O(n log n).  

    Busca Binária: Utilização de iteradores para capturar faixas de dados de forma otimizada em vetores previamente ordenados.  

🚀 Como Compilar e Executar (CLI)

O projeto não requer IDEs complexas e pode ser facilmente executado pelo terminal utilizando o compilador GCC.
1. Preparando o Ambiente

Antes de executar, certifique-se de que os arquivos de dados estão posicionados corretamente. O sistema espera que exista uma pasta chamada Fonte de dados/ na raiz do projeto contendo os arquivos:  

    cinemas.txt

    filmesCrop.txt

2. Compilação

Abra o seu terminal, navegue até o diretório onde os arquivos .cpp e .hpp estão localizados e execute o comando g++ abaixo. Isso fará a compilação do main.cpp junto com as dependências do projeto:
Bash

g++ -Wall -std=c++17 *.cpp -o sistema_cinemas

O argumento *.cpp garante que classes auxiliares como Menu, Indicegenero, Indicetipo e as Listas Ordenadas sejam linkadas corretamente no executável final.
3. Execução

Após a compilação, inicie o sistema gerado pelo compilador:

No Linux / macOS:
Bash

./sistema_cinemas

No Windows:
DOS

sistema_cinemas.exe
