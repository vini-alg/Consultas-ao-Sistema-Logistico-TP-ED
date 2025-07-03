#ifndef EVENTO_HPP
#define EVENTO_HPP

#include <string>
#include <iostream>
#include <iomanip>

/**
 * @brief Enumeração dos tipos de eventos logísticos.
 *
 * Define os diferentes tipos de eventos que podem ocorrer no sistema,
 * como registro de pacote, armazenamento, remoção, etc.
 */
enum TipoEvento {
    RG,         // Registro de um novo pacote
    AR,         // Armazenamento em um armazém
    RM,         // Remoção de um armazém para transporte
    UR,         // Descarregamento (Unload) em um armazém intermediário
    TR,         // Trânsito entre armazéns
    EN,         // Entrega final ao destinatário
    INDEFINIDO  // Tipo de evento desconhecido ou inválido
};

/**
 * @brief Representa um único evento no sistema logístico.
 *
 * Contém todas as informações associadas a um evento, como o momento em que
 * ocorreu, o tipo de evento, o pacote envolvido e outros dados relevantes
 * que variam conforme o tipo de evento.
 */
class Evento {
public:
    // Atributos principais do evento
    int dataHora;       ///< Timestamp do evento (formato AAAAMMDDHHMMSS).
    TipoEvento tipo;    ///< O tipo do evento (e.g., RG, AR, EN).
    int idPacote;       ///< Identificador único do pacote associado ao evento.

    // Atributos contextuais (podem não ser usados por todos os tipos de evento)
    std::string remetente;      ///< Nome do cliente remetente (usado em RG).
    std::string destinatario;   ///< Nome do cliente destinatário (usado em RG).
    int armazemOrigem;          ///< ID do armazém de origem.
    int armazemDestino;         ///< ID do armazém de destino.
    int secaoDestino;           ///< ID da seção de destino dentro de um armazém.

    /**
     * @brief Construtor padrão para um objeto Evento.
     *
     * Inicializa os membros com valores padrão para garantir um estado consistente.
     * Valores numéricos são zerados e strings são esvaziadas.
     */
    Evento();

    /**
     * @brief Imprime os detalhes do evento no formato de saída padrão.
     *
     * Formata e exibe as informações do evento, ajustando a saída
     * de acordo com o tipo de evento.
     */
    void imprimir();

    /**
     * @brief Retorna o timestamp do evento.
     * @return O tempo em que o evento ocorreu.
     */
    int getTempo() const;
};

#endif // EVENTO_HPP
