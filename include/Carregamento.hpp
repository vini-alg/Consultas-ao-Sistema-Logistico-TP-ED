#ifndef CARREGAMENTO_HPP
#define CARREGAMENTO_HPP

#include <iostream>
#include <string>

class Carregamento {
public:
    Carregamento(const std::string& filename);
    void executar();
private:
    std::string filename;
};

#endif // CARREGAMENTO_HPP