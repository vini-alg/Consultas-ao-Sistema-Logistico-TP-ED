#ifndef MAPA_HPP
#define MAPA_HPP

#include <string>
#include "Lista.hpp"

/**
 * @brief Estrutura de um nó para a Árvore de Busca Binária.
 * @tparam Chave O tipo da chave.
 * @tparam Valor O tipo do valor.
 */
template <typename Chave, typename Valor>
struct No {
    Chave chave;
    Valor valor;
    No *esquerda, *direita;

    No(Chave c, Valor v) : chave(c), valor(v), esquerda(nullptr), direita(nullptr) {}
};

/**
 * @brief Implementação de um Mapa usando uma Árvore de Busca Binária (BST).
 * 
 * Esta classe mapeia chaves a valores. A implementação usa uma BST não balanceada,
 * o que resulta em um desempenho de O(log n) em média, mas O(n) no pior caso.
 * 
 * @tparam Chave O tipo da chave, que deve suportar operadores de comparação.
 * @tparam Valor O tipo do valor associado à chave.
 */
template <typename Chave, typename Valor>
class Mapa {
private:
    No<Chave, Valor>* raiz;

    No<Chave, Valor>* inserirRecursivo(No<Chave, Valor>* no, Chave chave, Valor valor) {
        if (no == nullptr) return new No<Chave, Valor>(chave, valor);
        if (chave < no->chave) no->esquerda = inserirRecursivo(no->esquerda, chave, valor);
        else if (chave > no->chave) no->direita = inserirRecursivo(no->direita, chave, valor);
        else no->valor = valor; // Chave já existe, atualiza o valor
        return no;
    }

    Valor buscarRecursivo(No<Chave, Valor>* no, Chave chave) const {
        if (no == nullptr) return Valor(); // Retorna valor padrão (nullptr para ponteiros)
        if (chave == no->chave) return no->valor;
        if (chave < no->chave) return buscarRecursivo(no->esquerda, chave);
        return buscarRecursivo(no->direita, chave);
    }

    void destruirRecursivo(No<Chave, Valor>* no) {
        if (no != nullptr) {
            destruirRecursivo(no->esquerda);
            destruirRecursivo(no->direita);
            delete no;
        }
    }

    void coletarValoresRecursivo(No<Chave, Valor>* no, Lista<Valor>& lista) const {
        if (no != nullptr) {
            coletarValoresRecursivo(no->esquerda, lista);
            lista.adicionar(no->valor);
            coletarValoresRecursivo(no->direita, lista);
        }
    }

public:
    /** @brief Construtor padrão. Cria um mapa vazio. */
    Mapa() : raiz(nullptr) {}

    /** @brief Destrutor. Libera todos os nós da árvore. */
    ~Mapa() { destruirRecursivo(raiz); }

    /**
     * @brief Insere um par chave-valor no mapa.
     * Se a chave já existir, o valor associado é atualizado.
     * @param chave A chave para inserir.
     * @param valor O valor associado à chave.
     */
    void inserir(Chave chave, Valor valor) {
        raiz = inserirRecursivo(raiz, chave, valor);
    }

    /**
     * @brief Busca um valor associado a uma chave.
     * @param chave A chave a ser buscada.
     * @return O valor associado à chave. Se a chave não for encontrada, retorna
     *         um valor padrão para o tipo `Valor` (e.g., `nullptr` para ponteiros).
     */
    Valor buscar(Chave chave) const {
        return buscarRecursivo(raiz, chave);
    }

    /**
     * @brief Retorna uma lista com todos os valores contidos no mapa.
     * Os valores são retornados em ordem (in-order traversal da BST).
     * @return Uma `Lista<Valor>` com todos os valores.
     */
    Lista<Valor> obterValores() const {
        Lista<Valor> listaDeValores;
        coletarValoresRecursivo(raiz, listaDeValores);
        return listaDeValores;
    }
};

#endif // MAPA_HPP
