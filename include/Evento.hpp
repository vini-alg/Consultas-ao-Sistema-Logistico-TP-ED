#ifndef EVENTO_H
#define EVENTO_H

#include <string>

// Enum para os tipos de evento, para facilitar a leitura do código
enum TipoEvento {
    RG, AR, RM, UR, TR, EN, INDEFINIDO
};

class Evento {
public:
    int dataHora;
    TipoEvento tipo;
    int idPacote;
    // Atributos que podem ou não estar presentes dependendo do evento
    std::string remetente;
    std::string destinatario;
    int armazemOrigem;
    int armazemDestino;
    int secaoDestino;

    // Construtor padrão
    Evento();

    // Método para imprimir o evento no formato de saída
    void imprimir();
};

#endif // EVENTO_H
