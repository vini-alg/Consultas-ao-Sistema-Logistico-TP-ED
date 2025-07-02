#include "../include/Evento.hpp"
#include <iostream>

// Implementação do construtor padrão
Evento::Evento() {
    dataHora = 0;
    tipo = INDEFINIDO;
    idPacote = -1;
    remetente = "";
    destinatario = "";
    armazemOrigem = -1;
    armazemDestino = -1;
    secaoDestino = -1;
}

#include <iomanip> // Para std::setw e std::setfill

std::string tipoEventoParaString(TipoEvento tipo) {
    switch (tipo) {
        case RG: return "RG";
        case AR: return "AR";
        case RM: return "RM";
        case UR: return "UR";
        case TR: return "TR";
        case EN: return "EN";
        default: return "";
    }
}

// Implementação do método para imprimir o evento no formato de saída
void Evento::imprimir() {
    std::cout << std::setw(7) << std::setfill('0') << dataHora << " EV " << tipoEventoParaString(tipo) << " "
              << std::setw(3) << std::setfill('0') << idPacote;

    switch (tipo) {
        case RG:
            std::cout << " " << remetente << " " << destinatario << " "
                      << std::setw(3) << std::setfill('0') << armazemOrigem << " "
                      << std::setw(3) << std::setfill('0') << armazemDestino;
            break;
        case AR:
            std::cout << " " << std::setw(3) << std::setfill('0') << armazemOrigem << " "
                      << std::setw(3) << std::setfill('0') << secaoDestino;
            break;
        case RM:
            std::cout << " " << std::setw(3) << std::setfill('0') << armazemOrigem << " "
                      << std::setw(3) << std::setfill('0') << secaoDestino;
            break;
        case UR:
            std::cout << " " << std::setw(3) << std::setfill('0') << armazemOrigem << " "
                      << std::setw(3) << std::setfill('0') << secaoDestino;
            break;
        case TR:
            std::cout << " " << std::setw(3) << std::setfill('0') << armazemOrigem << " "
                      << std::setw(3) << std::setfill('0') << armazemDestino;
            break;
        case EN:
            std::cout << " " << std::setw(3) << std::setfill('0') << armazemDestino;
            break;
        case INDEFINIDO:
            break;
    }
    std::cout << std::endl;
}
