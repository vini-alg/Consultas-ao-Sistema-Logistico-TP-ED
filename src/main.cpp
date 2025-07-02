#include <iostream>
#include "../include/Carregamento.hpp"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Uso: " << argv[0] << " <arquivo_de_entrada>" << std::endl;
        return 1;
    }

    Carregamento carregamento(argv[1]);
    carregamento.executar();

    return 0;
}