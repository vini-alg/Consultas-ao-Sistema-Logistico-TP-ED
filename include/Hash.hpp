#ifndef HASH_HPP
#define HASH_HPP

#include <string>
#include <cstddef> // Para a definição de size_t

/**
 * @brief Estrutura de template para funções de hash customizadas.
 * 
 * Fornece uma interface genérica para hashing que pode ser especializada
 * para diferentes tipos de chave.
 * 
 * @tparam K O tipo da chave a ser hashada.
 */
template <typename K>
struct Hash;

/**
 * @brief Especialização do Hash para chaves do tipo `int`.
 */
template <>
struct Hash<int> {
    /**
     * @brief Calcula o hash para uma chave `int`.
     * @param key A chave inteira.
     * @return O valor de hash, que é a própria chave.
     */
    size_t operator()(int key) const {
        return static_cast<size_t>(key);
    }
};

/**
 * @brief Especialização do Hash para chaves do tipo `std::string`.
 */
template <>
struct Hash<std::string> {
    /**
     * @brief Calcula o hash para uma `std::string` usando o algoritmo djb2.
     * @param key A string a ser hashada.
     * @return O valor de hash calculado.
     */
    size_t operator()(const std::string& key) const {
        size_t hash = 5381;
        for (char c : key) {
            hash = ((hash << 5) + hash) + c; // hash * 33 + c
        }
        return hash;
    }
};

#endif // HASH_HPP
