#include <iostream>
#include "../include/Carregamento.hpp"

/**
 * @file main.cpp
 * @brief Ponto de entrada principal para o sistema de simulação logística.
 *
 * Este programa lê um arquivo de entrada especificado via linha de comando,
 * processa os eventos e consultas de um sistema logístico e imprime os
 * resultados na saída padrão.
 */

/**
 * @brief Função principal que inicia a execução do programa.
 * 
 * @param argc O número de argumentos da linha de comando.
 * @param argv Um array de strings contendo os argumentos. Espera-se o nome do
 *             programa e o caminho para o arquivo de entrada.
 * @return 0 em caso de sucesso, 1 em caso de erro (e.g., uso incorreto).
 */
#include <string>

int main(int argc, char* argv[]) {
    // Verifica se o número de argumentos está correto
    if (argc < 2 || argc > 3) {
        std::cerr << "Uso: " << argv[0] << " <arquivo_de_entrada> [--timed]" << std::endl;
        return 1; // Retorna código de erro
    }

    // Cria um objeto Carregamento com o arquivo fornecido
    Carregamento carregamento(argv[1]);

    // Verifica se o modo de medição de tempo foi solicitado
    if (argc == 3 && std::string(argv[2]) == "--timed") {
        carregamento.executarComTiming();
    } else {
        carregamento.executar();
    }

    return 0; // Retorna sucesso
}