#ifndef MAPA_HPP
#define MAPA_HPP

#include "Lista.hpp"
#include "Hash.hpp"

/**
 * @brief Implementação de um Mapa usando uma tabela de hash totalmente customizada.
 * 
 * @details Esta classe mapeia chaves a valores usando uma tabela de hash com
 * encadeamento separado. A implementação é totalmente customizada, utilizando
 * a classe `Lista` para a tabela e para os baldes (buckets), e uma função de hash
 * própria definida em `Hash.hpp`. Fornece complexidade de tempo média O(1)
 * para inserção e busca.
 *
 * @tparam Chave O tipo da chave. Deve ter uma especialização correspondente em `Hash.hpp`.
 * @tparam Valor O tipo do valor associado à chave.
 */
template <typename Chave, typename Valor>
class Mapa {
private:
    struct Entrada {
        Chave chave;
        Valor valor;
    };

    Lista<Lista<Entrada>> tabela;
    size_t tamanho;
    size_t capacidade;

    size_t hash(Chave chave) const {
        return Hash<Chave>{}(chave) % capacidade;
    }

public:
    /**
     * @brief Construtor padrão. Cria um mapa vazio com capacidade inicial.
     */
    Mapa() : tamanho(0), capacidade(100) {
        tabela.resize(capacidade);
    }

    /**
     * @brief Destrutor. Libera todos os recursos alocados.
     */
    ~Mapa() {}

    /**
     * @brief Insere um par chave-valor no mapa.
     *
     * Se a chave já existir, o valor é atualizado. Caso contrário, um novo
     * par chave-valor é adicionado.
     * 
     * @param chave A chave a ser inserida.
     * @param valor O valor associado à chave.
     */
    void inserir(Chave chave, Valor valor) {
        size_t indice = hash(chave);
        Lista<Entrada>& balde = tabela[indice];

        for (int i = 0; i < balde.getTamanho(); ++i) {
            if (balde.obter(i).chave == chave) {
                balde.obter(i).valor = valor;
                return;
            }
        }

        balde.adicionar({chave, valor});
        tamanho++;
    }

    /**
     * @brief Busca um valor no mapa pela chave.
     * 
     * @param chave A chave a ser buscada.
     * @return O valor associado à chave. Se a chave não for encontrada, retorna
     *         um valor padrão para o tipo `Valor` (e.g., `nullptr` para ponteiros).
     */
    Valor buscar(Chave chave) const {
        size_t indice = hash(chave);
        const Lista<Entrada>& balde = tabela[indice];

        for (int i = 0; i < balde.getTamanho(); ++i) {
            if (balde.obter(i).chave == chave) {
                return balde.obter(i).valor;
            }
        }

        return Valor();
    }

    /**
     * @brief Retorna todos os valores armazenados no mapa.
     * 
     * @return Uma `Lista` contendo todos os valores.
     */
    Lista<Valor> obterValores() const {
        Lista<Valor> listaDeValores;
        for (int j = 0; j < tabela.getTamanho(); ++j) {
            const Lista<Entrada>& balde = tabela.obter(j);
            for (int i = 0; i < balde.getTamanho(); ++i) {
                listaDeValores.adicionar(balde.obter(i).valor);
            }
        }
        return listaDeValores;
    }
};

#endif // MAPA_HPP
