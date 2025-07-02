#ifndef LISTA_H
#define LISTA_H

#include <stdexcept>

// Um TAD de Lista/Vetor Dinâmico implementado do zero.
// Usa templates para poder ser reutilizado com qualquer tipo de dado.
template <typename T>
class Lista {
private:
    T* elementos;
    int capacidade;
    int quantidade;

    void redimensionar() {
        int novaCapacidade = (capacidade == 0) ? 1 : capacidade * 2;
        T* novosElementos = new T[novaCapacidade];
        for (int i = 0; i < quantidade; ++i) {
            novosElementos[i] = elementos[i];
        }
        delete[] elementos;
        elementos = novosElementos;
        capacidade = novaCapacidade;
    }

public:
    Lista() {
        capacidade = 10;
        quantidade = 0;
        elementos = new T[capacidade];
    }

    // Construtor de cópia (Deep Copy)
    Lista(const Lista<T>& outra) {
        capacidade = outra.capacidade;
        quantidade = outra.quantidade;
        elementos = new T[capacidade];
        for (int i = 0; i < quantidade; ++i) {
            elementos[i] = outra.elementos[i];
        }
    }

    // Operador de atribuição de cópia (Deep Copy)
    Lista<T>& operator=(const Lista<T>& outra) {
        if (this == &outra) { // Proteção contra auto-atribuição
            return *this;
        }

        delete[] elementos; // Libera a memória antiga

        capacidade = outra.capacidade;
        quantidade = outra.quantidade;
        elementos = new T[capacidade];
        for (int i = 0; i < quantidade; ++i) {
            elementos[i] = outra.elementos[i];
        }
        
        return *this;
    }

    ~Lista() {
        delete[] elementos;
    }

    void adicionar(T elemento) {
        if (quantidade == capacidade) {
            redimensionar();
        }
        elementos[quantidade++] = elemento;
    }

    T& obter(int indice) {
        if (indice < 0 || indice >= quantidade) {
            throw std::out_of_range("Índice fora dos limites da lista.");
        }
        return elementos[indice];
    }

    int tamanho() {
        return quantidade;
    }

    bool contem(T elemento) {
        for (int i = 0; i < quantidade; ++i) {
            if (elementos[i] == elemento) {
                return true;
            }
        }
        return false;
    }

    // Método para ordenar a lista usando insertion sort e uma função de comparação
    void ordenar(bool (*comparar)(T a, T b)) {
        for (int i = 1; i < quantidade; i++) {
            T chave = elementos[i];
            int j = i - 1;
            while (j >= 0 && comparar(chave, elementos[j])) {
                elementos[j + 1] = elementos[j];
                j = j - 1;
            }
            elementos[j + 1] = chave;
        }
    }
};

#endif // LISTA_H
