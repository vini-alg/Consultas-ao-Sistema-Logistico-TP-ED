#include "../include/Evento.hpp"

/**
 * @brief Converte um valor do enum TipoEvento para sua representação em string.
 * @param tipo O tipo de evento a ser convertido.
 * @return A string correspondente ao tipo de evento (e.g., "RG", "AR").
 */
static std::string tipoEventoParaString(TipoEvento tipo) {
    switch (tipo) {
        case RG: return "RG";
        case AR: return "AR";
        case RM: return "RM";
        case UR: return "UR";
        case TR: return "TR";
        case EN: return "EN";
        default: return "INDEFINIDO";
    }
}

/**
 * @brief Construtor padrão da classe Evento.
 * 
 * Inicializa todos os membros com valores padrão para garantir um estado inicial limpo e previsível.
 * `dataHora`, `idPacote`, `armazemOrigem`, `armazemDestino`, e `secaoDestino` são inicializados com -1 ou 0.
 * `remetente` e `destinatario` são inicializados como strings vazias.
 * `tipo` é inicializado como `INDEFINIDO`.
 */
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

/**
 * @brief Imprime os detalhes do evento para o console.
 * 
 * Formata a saída de acordo com o tipo de evento, exibindo informações relevantes
 * como timestamp, tipo de evento, ID do pacote e outros dados específicos do evento.
 * Utiliza `std::setw` e `std::setfill` para garantir a formatação correta dos números.
 */
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
        case RM:
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
            // Não imprime informações adicionais para tipos indefinidos.
            break;
    }
    std::cout << std::endl;
}

/**
 * @brief Retorna o timestamp do evento.
 * 
 * Permite o acesso ao tempo do evento de forma encapsulada.
 * 
 * @return O valor de `dataHora` do evento.
 */
int Evento::getTempo() const {
    return dataHora;
}
