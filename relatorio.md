---
title: "Trabalho Prático de Estruturas de Dados: Consultas a um Sistema Logístico"
author: "Vinicius de Alcantara Garrido"
date: "Junho de 2025"
---

# 1. Introdução

Esta documentação aborda a implementação de um sistema de consultas para um grande volume de dados de uma empresa de logística. O desafio central é processar um arquivo de entrada contendo milhares de eventos logísticos (registro de pacote, armazenamento, transporte, entrega) e, simultaneamente, responder a consultas sobre o histórico de pacotes e o estado dos pacotes de clientes específicos em determinados instantes de tempo.

O objetivo deste trabalho é desenvolver um sistema em C++ capaz de processar o arquivo de entrada de forma eficiente, sem o uso de contêineres da biblioteca padrão (STL), e responder às consultas em tempo hábil. A solução deve ler o fluxo de eventos e construir estruturas de dados em memória que permitam a recuperação rápida das informações solicitadas, evitando a varredura completa do log a cada consulta.

Para resolver o problema, foi desenvolvida uma solução que utiliza estruturas de dados customizadas, incluindo um vetor dinâmico e uma tabela de hash como mapa. A abordagem central é processar o arquivo de entrada sequencialmente, construindo índices em tempo real que associam pacotes e clientes aos seus respectivos eventos.

# 2. Método

## 2.1. Estruturas de Dados e Classes

-   **`Carregamento`**: Classe principal que orquestra todo o processo. É responsável por ler o arquivo de entrada linha por linha, interpretar se é um evento ou uma consulta, e delegar o processamento.
-   **`Evento`**: Estrutura que armazena os dados de um único evento logístico, como tipo, timestamp, ID do pacote e outras informações pertinentes.
-   **`Lista<T>`**: Implementação de um vetor dinâmico genérico, utilizado para armazenar a lista completa de eventos e as listas de eventos/pacotes dentro dos índices.
-   **`Mapa<Chave, Valor>`**: Implementação de um mapa genérico utilizando uma **Tabela de Hash** com encadeamento separado. Esta estrutura foi totalmente customizada, sem depender de bibliotecas da STL.
-   **`Hash<K>`**: Estrutura de template que fornece funções de hash customizadas para diferentes tipos de chave (e.g., `int`, `std::string`), permitindo que o `Mapa` opere de forma genérica e eficiente.
-   A indexação é feita com dois mapas principais:
    -   `mapaPacotes`: Mapeia um ID de pacote (`int`) para uma lista (`Lista<Evento*>`) de todos os seus eventos.
    -   `mapaClientes`: Mapeia um nome de cliente (`std::string`) para uma lista (`Lista<int>`) de todos os IDs de pacotes associados a ele.

## 2.2. Lógica de Processamento

O núcleo do sistema é um laço que lê o arquivo de entrada e processa cada linha:

1.  **Leitura e Parsing**: A classe `Carregamento` lê uma linha. Um `stringstream` é usado para extrair os campos.
2.  **Identificação**: O sistema verifica se a linha é um evento (ex: `RG`, `AR`) ou uma consulta (`PC`, `CL`).
3.  **Processamento de Evento**:
    -   Um novo objeto `Evento` é alocado dinamicamente.
    -   O ponteiro para o evento é adicionado à lista principal de eventos.
    -   **Indexação**: O evento é indexado nos mapas. O `mapaPacotes` e o `mapaClientes` são atualizados com as informações do novo evento, garantindo que as consultas futuras sejam rápidas.
4.  **Processamento de Consulta**:
    -   **Consulta de Pacote (PC)**: O ID do pacote é buscado no `mapaPacotes`. A lista de eventos retornada é percorrida, e todos os eventos ocorridos até o timestamp da consulta são impressos.
    -   **Consulta de Cliente (CL)**: O nome do cliente é buscado no `mapaClientes` para obter a lista de seus pacotes. Para cada pacote, o sistema busca seu último evento no `mapaPacotes` (anterior ao timestamp da consulta). Os resultados são ordenados por ID de pacote e impressos.

## 2.3. Diagrama de Fluxo de Dados

![Diagrama de Fluxo de Dados](image.png)

*Figura 1: Fluxograma do processamento de dados no sistema logístico*

**Observação:** Seguindo a proposta do trabalho prático, as linhas do arquivo são inicialmente armazenadas em um vetor de eventos (memória primária) e posteriormente processadas sequencialmente pelo algoritmo principal.

# 3. Análise de Complexidade

A eficiência do sistema depende diretamente da complexidade das estruturas de dados customizadas e da lógica de indexação. Seja *I* o número total de linhas no arquivo de entrada, *E* o número de eventos, *P* o número de pacotes únicos e *C* o número de clientes únicos.

## 3.1. Estruturas de Dados e Algoritmos

-   **`Lista<T>`**: Operações de inserção no final (`adicionar`) têm custo **O(1)** amortizado.
-   **`Mapa<Chave, Valor>` (Tabela de Hash)**: As operações de inserção (`inserir`) e busca (`buscar`) têm complexidade de tempo média de **O(1)**. No pior caso, quando múltiplas chaves colidem no mesmo balde (bucket), a complexidade pode degradar para **O(N)**, onde N é o número de chaves no mapa.
-   **Ordenação (Insertion Sort)**: A ordenação dos resultados da consulta `CL` tem complexidade **O(L²)**, onde L é o número de pacotes do cliente. Dado que L é geralmente pequeno, o impacto é limitado.

## 3.2. Lógica de Processamento

-   **Indexação de Evento**: Para cada evento, o sistema realiza buscas e/ou inserções nos dois mapas. A complexidade é, em média, **O(log P + log C)**.
-   **Consulta PC**: Envolve uma busca no `mapaPacotes` (**O(log P)**) e uma varredura na lista de eventos do pacote (tamanho *K*), resultando em **O(log P + K)**.
-   **Consulta CL**: Envolve uma busca no `mapaClientes` (**O(log C)**), seguida por *L* buscas no `mapaPacotes` (**L \* O(log P)**) e uma ordenação, resultando em **O(log C + L \* log P + L²)**.

## 3.3. Complexidade Geral

-   **Tempo**: A complexidade de tempo total para processar o arquivo é aproximadamente **O(I)** no caso médio já que os acessos são praticamente **O(1)**, então para *I* linhas, o tempo de execução é **O(I)**.
-   **Espaço**: A memória é consumida pelo armazenamento de todos os eventos e pelos índices.
    -   Lista de eventos: **O(E)**
    -   Índices (`mapaPacotes` e `mapaClientes`): **O(P + C + E)** (armazenam chaves e ponteiros para todos os eventos).
    -   A complexidade de espaço total é **O(E + P + C)**.

# 4. Estratégias de Robustez

-   **Gerenciamento de Memória**: Todas as estruturas de dados foram implementadas com gerenciamento eficiente de memória. O conteúdo dos eventos é armazenado apenas uma vez no vetor principal de eventos, enquanto as demais estruturas utilizam ponteiros para acessar esses dados, minimizando o consumo de memória.
-   **Tratamento de Erros**: O sistema inclui verificações para garantir a integridade dos dados e o correto gerenciamento de recursos, mesmo em situações de erro.
-   **Geração de Entradas Flexível**: Para garantir testes abrangentes, foi desenvolvido um módulo de configuração (`configure_gen.py`) integrado ao `Makefile`. Isso permite a geração de cargas de teste aleatórias e personalizadas diretamente pela linha de comando, facilitando a validação do sistema sob diferentes cenários e escalas de dados.

# 5. Análise Experimental

A análise experimental foi feita com os seguintes passos:

1.  **Geração de entradas**: Dentro do `Makefile` foi criada uma regra para geração de entradas aleatórias, que pode ser usada para gerar arquivos de entrada com diferentes parâmetros através do script `configure_gen.py` pelos arquivos do diretorio `gerador_de_entradas`.

2. **Simulação de entradas**: Com a função pronta, foi-se gerado um script jupyter (`analise_experimental.ipynb`) que mede o tempo de execução do programa para diferentes parâmetros de entrada. Para cada teste, foi alterado apenas um parâmetro de entrada, mantendo os outros fixos.

3. **Medição de métricas**: As métricas avaliadas durante a análise experimental foram:
    - Tempo de execução
    - Custo de memoria

### Parâmetros Base Utilizados

Os experimentos foram executados utilizando um conjunto de parâmetros base, que foram mantidos constantes enquanto um dos parâmetros era variado.

| Parâmetro | Valor Base |
|-----------|------------|
| `packets` | 1000       |
| `clients` | 500        |
| `num_cl`  | 100        |
| `num_pc`  | 100        |

### Experimento 1: Variação do Número de Pacotes

| Número de Pacotes | Tempo (ms) | Memória (KB) |
|-------------------|------------|--------------|
| 1000              | 6.56       | 3968         |
| 1473              | 9.31       | 4224         |
| 1947              | 11.86      | 4352         |
| 2421              | 12.32      | 4608         |
| 2894              | 14.90      | 4736         |
| 3368              | 16.48      | 4864         |
| 3842              | 21.32      | 4736         |
| 4315              | 24.13      | 5376         |
| 4789              | 27.69      | 5248         |
| 5263              | 36.11      | 5632         |
| 5736              | 40.02      | 5632         |
| 6210              | 43.06      | 5632         |
| 6684              | 43.92      | 6144         |
| 7157              | 41.44      | 6144         |
| 7631              | 45.65      | 6400         |
| 8105              | 66.84      | 6656         |
| 8578              | 53.95      | 6912         |
| 9052              | 62.96      | 7040         |
| 9526              | 56.88      | 7168         |
| 10000             | 58.70      | 7296         |

![](resultados/memoria_vs_pacotes.png)

![](resultados/tempo_vs_pacotes.png)

**Conclusão:** Tanto o tempo de execução quanto o consumo de memória aumentam de forma aproximadamente linear em relação ao número de pacotes. Este comportamento era esperado, pois o número de operações de inserção no vetor de eventos é diretamente proporcional à quantidade de pacotes processados.

### Experimento 2: Variação do Número de Clientes

| Número de Clientes | Tempo (ms) | Memória (KB) |
|--------------------|------------|--------------|
| 500                | 6.91       | 3968         |
| 736                | 6.09       | 4096         |
| 973                | 6.06       | 4096         |
| 1210               | 8.14       | 4096         |
| 1447               | 6.34       | 4096         |
| 1684               | 7.50       | 4096         |
| 1921               | 8.08       | 4224         |
| 2157               | 8.30       | 4096         |
| 2394               | 7.54       | 4224         |
| 2631               | 7.45       | 4096         |
| 2868               | 7.79       | 3840         |
| 3105               | 7.55       | 4224         |
| 3342               | 10.22      | 4224         |
| 3578               | 8.07       | 4096         |
| 3815               | 8.34       | 4224         |
| 4052               | 7.21       | 4096         |
| 4289               | 9.46       | 4224         |
| 4526               | 8.71       | 4224         |
| 4763               | 7.53       | 4224         |
| 5000               | 8.24       | 4224         |

![](resultados/memoria_vs_clientes.png)

![](resultados/tempo_vs_clientes.png)

**Conclusão:** Tanto o tempo quanto a memoria não são afetados diretamente com o aumento do número de clientes. Provavelmente pelo motivo de que o acesso ao dados já é feito por meio de um mapa, que possui complexidade de acesso próxima a O(1) e os outros paramétros que involvem em número de eventos não se alteram como por exemplo o número total de pacotes.

### Experimento 3: Variação do Número de Consultas CL

| Número de Consultas CL | Tempo (ms) | Memória (KB) |
|------------------------|------------|--------------|
| 100                    | 6.71       | 3968         |
| 147                    | 6.67       | 3968         |
| 194                    | 8.80       | 4096         |
| 242                    | 3.57       | 3968         |
| 289                    | 9.75       | 3968         |
| 336                    | 9.90       | 4096         |
| 384                    | 10.79      | 4096         |
| 431                    | 11.57      | 4096         |
| 478                    | 13.47      | 4096         |
| 526                    | 13.23      | 4096         |
| 573                    | 13.86      | 4096         |
| 621                    | 17.84      | 4096         |
| 668                    | 17.13      | 4096         |
| 715                    | 17.17      | 4096         |
| 763                    | 17.08      | 4224         |
| 810                    | 18.75      | 4096         |
| 857                    | 17.89      | 4224         |
| 905                    | 19.82      | 4096         |
| 952                    | 19.82      | 4096         |
| 1000                   | 22.78      | 4096         |

![](resultados/memoria_vs_consultas_cl.png)

![](resultados/tempo_vs_consultas_cl.png)

**Conclusão:** Neste caso, o número de consultas por cliente não influencia no custo de memoria. Porém, o tempo de execução aumenta de forma linear por ter cada acesso individual é O(1), se são feitos n acessos, o tempo de execução é O(n).

### Experimento 4: Variação do Número de Consultas PC
Neste experimento, o número de consultas de pacote (PC) foi variado, mantendo 1.000 pacotes, 500 clientes e 100 consultas de cliente (CL).

| Número de Consultas PC | Tempo (ms) | Memória (KB) |
|------------------------|------------|--------------|
| 100                    | 6.43       | 4224         |
| 147                    | 6.39       | 4224         |
| 194                    | 8.76       | 4096         |
| 242                    | 7.31       | 3968         |
| 289                    | 12.05      | 4096         |
| 336                    | 8.13       | 4096         |
| 384                    | 9.26       | 3968         |
| 431                    | 8.65       | 4224         |
| 478                    | 9.40       | 4096         |
| 526                    | 9.67       | 4096         |
| 573                    | 11.65      | 3968         |
| 621                    | 11.24      | 4096         |
| 668                    | 10.59      | 4096         |
| 715                    | 10.63      | 3840         |
| 763                    | 12.40      | 3968         |
| 810                    | 12.34      | 3968         |
| 857                    | 12.52      | 4096         |
| 905                    | 11.73      | 4096         |
| 952                    | 11.90      | 4096         |
| 1000                   | 14.74      | 4224         |

![](resultados/memoria_vs_consultas_pc.png)

![](resultados/tempo_vs_consultas_pc.png)

**Conclusão:** Assim como no experimento anterior, o número de consultas por pacote não afeta significativamente o consumo de memória, e o tempo de execução mantém um comportamento aproximadamente linear. No entanto, observa-se que a inclinação da curva de tempo é menos acentuada neste caso. Isso ocorre porque as consultas por cliente envolvem a recuperação de múltiplos pacotes associados, enquanto as consultas por pacote acessam apenas os eventos relacionados a um único pacote específico.

# 6. Conclusões

Durante o desenvolvimento deste trabalho, foram aplicados e consolidados conceitos fundamentais de estruturas de dados, com destaque para:

1. **Estruturas de Dados Eficientes**: A implementação de uma tabela de hash personalizada permitiu operações de busca e inserção em tempo médio constante (O(1)), garantindo um desempenho escalável mesmo com grandes volumes de dados.

2. **Gerenciamento de Memória**: A estratégia de armazenar os dados apenas uma vez e utilizar ponteiros nas estruturas de índice mostrou-se eficaz na otimização do uso de memória.

3. **Análise de Complexidade**: A análise teórica foi validada pelos resultados experimentais, que demonstraram o comportamento linear esperado para as operações principais do sistema.

4. **Validação Experimental**: A metodologia de testes sistemáticos, com variação controlada de parâmetros, permitiu uma avaliação abrangente do desempenho do sistema em diferentes cenários.

Os resultados obtidos demonstram a importância da escolha adequada de estruturas de dados e algoritmos para o processamento eficiente de grandes volumes de informações, um requisito crítico em sistemas logísticos do mundo real.

# 7. Referências

- Chaimowicz, L. and Prates, R. (2020). *Slides virtuais da disciplina de Estruturas de Dados*. Departamento de Ciência da Computação, Universidade Federal de Minas Gerais. Belo Horizonte. Disponível em: Moodle/DCC/UFMG.
- Cormen, T. H., Leiserson, C. E., Rivest, R. L., & Stein, C. (2009). *Introduction to Algorithms* (3rd ed.). MIT Press.
- Sedgewick, R., & Wayne, K. (2011). *Algorithms* (4th ed.). Addison-Wesley Professional.
- Knuth, D. E. (1997). *The Art of Computer Programming, Volume 3: Sorting and Searching* (3rd ed.). Addison-Wesley Professional.
