#ifndef MAPA_H
#define MAPA_H

#include <string>

// Estrutura do Nó da Árvore de Busca Binária
template <typename Chave, typename Valor>
struct No {
    Chave chave;
    Valor valor;
    No *esquerda, *direita;

    No(Chave c, Valor v) : chave(c), valor(v), esquerda(nullptr), direita(nullptr) {}
};

// Template da classe Mapa (implementada como uma Árvore de Busca Binária)
template <typename Chave, typename Valor>
class Mapa {
private:
    No<Chave, Valor>* raiz;

    // Função auxiliar recursiva para inserir
    No<Chave, Valor>* inserir(No<Chave, Valor>* no, Chave chave, Valor valor) {
        if (no == nullptr) {
            return new No<Chave, Valor>(chave, valor);
        }

        if (chave < no->chave) {
            no->esquerda = inserir(no->esquerda, chave, valor);
        } else if (chave > no->chave) {
            no->direita = inserir(no->direita, chave, valor);
        } else {
            // Chave já existe, atualiza o valor
            no->valor = valor;
        }
        return no;
    }

    // Função auxiliar recursiva para buscar
    Valor buscar(No<Chave, Valor>* no, Chave chave) {
        if (no == nullptr) {
            return Valor(); // Retorna valor padrão se não encontrado
        }

        if (chave == no->chave) {
            return no->valor;
        } else if (chave < no->chave) {
            return buscar(no->esquerda, chave);
        } else {
            return buscar(no->direita, chave);
        }
    }

    // Função auxiliar para destruir a árvore
    void destruir(No<Chave, Valor>* no) {
        if (no != nullptr) {
            destruir(no->esquerda);
            destruir(no->direita);
            delete no;
        }
    }

    // Função auxiliar para coletar todos os valores em uma lista
    void coletarValores(No<Chave, Valor>* no, Lista<Valor>& lista) {
        if (no != nullptr) {
            coletarValores(no->esquerda, lista);
            lista.adicionar(no->valor);
            coletarValores(no->direita, lista);
        }
    }

public:
    Mapa() : raiz(nullptr) {}

    ~Mapa() {
        destruir(raiz);
    }

    void inserir(Chave chave, Valor valor) {
        raiz = inserir(raiz, chave, valor);
    }

    Valor buscar(Chave chave) {
        return buscar(raiz, chave);
    }

    Lista<Valor> obterValores() {
        Lista<Valor> listaDeValores;
        coletarValores(raiz, listaDeValores);
        return listaDeValores;
    }
};

#endif // MAPA_H
