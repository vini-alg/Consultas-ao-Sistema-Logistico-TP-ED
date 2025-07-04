#ifndef LISTA_HPP
#define LISTA_HPP

#include <stdexcept>

/**
 * @brief Uma implementação de um vetor dinâmico (lista) genérico.
 * 
 * Esta classe fornece uma estrutura de dados de lista que pode crescer dinamicamente.
 * É implementada usando templates para permitir o armazenamento de qualquer tipo de dado.
 * A classe gerencia a memória internamente e se redimensiona conforme necessário.
 * Segue a "Regra dos Três" para garantir o gerenciamento correto da memória (destrutor,
 * construtor de cópia e operador de atribuição de cópia).
 *
 * @tparam T O tipo de elemento a ser armazenado na lista.
 */
template <typename T>
class Lista {
private:
    T* elementos;       ///< Ponteiro para o array alocado dinamicamente.
    int capacidade;     ///< A capacidade atual do array.
    int quantidade;     ///< O número de elementos atualmente na lista.

    /**
     * @brief Redimensiona o array interno para acomodar mais elementos.
     * 
     * Dobra a capacidade atual do array. Se a capacidade for 0, define como 1.
     * Aloca um novo array, copia os elementos existentes e libera o antigo.
     */
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
    /**
     * @brief Construtor padrão.
     * 
     * Inicializa uma lista vazia com uma capacidade inicial padrão.
     */
    Lista() : capacidade(10), quantidade(0) {
        elementos = new T[capacidade];
    }

    /**
     * @brief Destrutor.
     * 
     * Libera a memória alocada para os elementos da lista.
     */
    ~Lista() {
        delete[] elementos;
    }

    /**
     * @brief Construtor de cópia (Deep Copy).
     * 
     * Cria uma nova lista que é uma cópia profunda de outra lista.
     * Garante que os dados sejam duplicados, não apenas os ponteiros.
     * 
     * @param outra A lista a ser copiada.
     */
    Lista(const Lista<T>& outra) {
        capacidade = outra.capacidade;
        quantidade = outra.quantidade;
        elementos = new T[capacidade];
        for (int i = 0; i < quantidade; ++i) {
            elementos[i] = outra.elementos[i];
        }
    }

    /**
     * @brief Operador de atribuição de cópia (Deep Copy).
     * 
     * Atribui o conteúdo de outra lista a esta, realizando uma cópia profunda.
     * Inclui proteção contra auto-atribuição.
     * 
     * @param outra A lista da qual os dados serão copiados.
     * @return Uma referência para a lista atual (`*this`).
     */
    Lista<T>& operator=(const Lista<T>& outra) {
        if (this == &outra) {
            return *this;
        }

        delete[] elementos;

        capacidade = outra.capacidade;
        quantidade = outra.quantidade;
        elementos = new T[capacidade];
        for (int i = 0; i < quantidade; ++i) {
            elementos[i] = outra.elementos[i];
        }
        
        return *this;
    }

    /**
     * @brief Adiciona um elemento ao final da lista.
     * 
     * Se a lista estiver cheia, ela será redimensionada antes de adicionar o novo elemento.
     * 
     * @param elemento O elemento a ser adicionado.
     */
    void adicionar(T elemento) {
        if (quantidade == capacidade) {
            redimensionar();
        }
        elementos[quantidade++] = elemento;
    }

    /**
     * @brief Redimensiona a lista para conter um novo número de elementos.
     * 
     * @param novoTamanho O novo tamanho da lista.
     */
    void resize(int novoTamanho) {
        if (novoTamanho < 0) return;
        if (novoTamanho > capacidade) {
            int novaCapacidade = novoTamanho;
            T* novosElementos = new T[novaCapacidade];
            for (int i = 0; i < quantidade; ++i) {
                novosElementos[i] = elementos[i];
            }
            delete[] elementos;
            elementos = novosElementos;
            capacidade = novaCapacidade;
        }

        for (int i = quantidade; i < novoTamanho; ++i) {
            elementos[i] = T();
        }
        quantidade = novoTamanho;
    }

    /**
     * @brief Obtém uma referência ao elemento em um índice específico.
     * 
     * @param indice O índice do elemento a ser obtido.
     * @return Uma referência ao elemento no índice especificado.
     * @throws std::out_of_range se o índice for inválido.
     */
    T& obter(int indice) {
        if (indice < 0 || indice >= quantidade) {
            throw std::out_of_range("Índice fora dos limites da lista.");
        }
        return elementos[indice];
    }

    /**
     * @brief Obtém uma referência constante ao elemento em um índice específico.
     * 
     * @param indice O índice do elemento a ser obtido.
     * @return Uma referência constante ao elemento no índice especificado.
     * @throws std::out_of_range se o índice for inválido.
     */
    const T& obter(int indice) const {
        if (indice < 0 || indice >= quantidade) {
            throw std::out_of_range("Índice fora dos limites da lista.");
        }
        return elementos[indice];
    }

    /**
     * @brief Acessa um elemento pelo índice usando o operador [].
     * @param indice O índice do elemento.
     * @return Uma referência ao elemento.
     * @throws std::out_of_range se o índice for inválido.
     */
    T& operator[](int indice) {
        if (indice < 0 || indice >= quantidade) {
            throw std::out_of_range("Índice fora dos limites da lista.");
        }
        return elementos[indice];
    }

    /**
     * @brief Acessa um elemento constante pelo índice usando o operador [].
     * @param indice O índice do elemento.
     * @return Uma referência constante ao elemento.
     * @throws std::out_of_range se o índice for inválido.
     */
    const T& operator[](int indice) const {
        if (indice < 0 || indice >= quantidade) {
            throw std::out_of_range("Índice fora dos limites da lista.");
        }
        return elementos[indice];
    }


    
    /**
     * @brief Define o valor de um elemento em um índice específico.
     *
     * @param indice O índice do elemento a ser modificado.
     * @param valor O novo valor para o elemento.
     * @throws std::out_of_range se o índice for inválido.
     */
    void definir(int indice, T valor) {
        if (indice < 0 || indice >= quantidade) {
            throw std::out_of_range("Índice fora dos limites da lista.");
        }
        elementos[indice] = valor;
    }

    /**
     * @brief Retorna o número de elementos na lista.
     * 
     * @return O tamanho atual da lista.
     */
    int getTamanho() const {
        return quantidade;
    }

    /**
     * @brief Verifica se a lista contém um determinado elemento.
     * 
     * @param elemento O elemento a ser procurado.
     * @return `true` se o elemento for encontrado, `false` caso contrário.
     */
    bool contem(T elemento) const {
        for (int i = 0; i < quantidade; ++i) {
            if (elementos[i] == elemento) {
                return true;
            }
        }
        return false;
    }

    /**
     * @brief Ordena a lista usando o algoritmo Insertion Sort.
     * 
     * @param comparar Um ponteiro de função que recebe dois elementos e retorna `true`
     *                 se o primeiro deve vir antes do segundo.
     */
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

#endif // LISTA_HPP
