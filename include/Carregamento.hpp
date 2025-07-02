#ifndef CARREGAMENTO_HPP
#define CARREGAMENTO_HPP

#include <iostream>
#include <string>
#include "Evento.hpp"
#include "Lista.hpp"
#include "Mapa.hpp"

class Carregamento {
private:
    std::string filename;
    Lista<Evento*> eventos; // Continua sendo a fonte da verdade para todos os eventos

    // Índices para acesso rápido
    Mapa<int, Lista<Evento*>*>* mapaPacotes;
    Mapa<std::string, Lista<int>*>* mapaClientes;

    void processarLinha(const std::string& linha);
    TipoEvento stringParaTipoEvento(const std::string& s);

public:
    Carregamento(const std::string& filename);
    ~Carregamento();
    void executar();
};

#endif // CARREGAMENTO_HPP