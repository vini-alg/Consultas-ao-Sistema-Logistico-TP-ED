#ifndef CARREGAMENTO_HPP
#define CARREGAMENTO_HPP

#include <iostream>
#include <string>
#include "Evento.hpp"
#include "Lista.hpp"
#include "Mapa.hpp"

/**
 * @brief Estrutura para armazenar os pacotes associados a um cliente.
 *
 * Separa os pacotes em duas listas: os que foram enviados pelo cliente e os que
 * ele deve receber.
 */
struct PacotesCliente {
    Lista<int> enviados;
    Lista<int> aReceber;
};

/**
 * @brief Orquestra o carregamento de dados e o processamento de simulações logísticas.
 *
 * Esta classe é responsável por ler um arquivo de entrada contendo eventos e consultas,
 * processá-los e gerar as saídas correspondentes. Ela gerencia a memória de todos
 * os objetos de evento e das estruturas de dados auxiliares.
 */
class Carregamento {
public:
    /**
     * @brief Construtor.
     * @param filename O caminho para o arquivo de entrada a ser processado.
     */
    Carregamento(const std::string& filename);

    /**
     * @brief Destrutor.
     * Libera toda a memória alocada dinamicamente, incluindo eventos e os mapas.
     */
    ~Carregamento();

    /**
     * @brief Inicia o processo de leitura e execução da simulação.
     */
    void executar();

    /**
     * @brief Executa a simulação com medição de tempo, separando I/O de processamento.
     *
     * Primeiro, todas as linhas são lidas para a memória. Em seguida, o tempo de
     * processamento dessas linhas é medido e impresso em stderr.
     */
    void executarComTiming();

private:
    std::string filename;
    Lista<Evento*> eventos; ///< Fonte da verdade, armazena todos os eventos criados.

    // Os mapas abaixo são índices para acesso rápido aos dados.
    // Eles armazenam ponteiros para listas/objetos, que são alocados dinamicamente
    // e devem ser liberadas manualmente no destrutor.
    Mapa<int, Lista<Evento*>*>* mapaPacotes;         ///< Mapeia ID do pacote para sua lista de eventos.
    Mapa<std::string, PacotesCliente*>* mapaClientes; ///< Mapeia nome do cliente para seus pacotes (enviados/a receber).

    /**
     * @brief Processa uma única linha do arquivo de entrada.
     * @param linha A linha a ser processada.
     */
    void processarLinha(const std::string& linha);

    /**
     * @brief Processa uma consulta do tipo 'PC' (Pacote-Cliente).
     * @param tempo O timestamp da consulta.
     * @param idPacote O ID do pacote a ser consultado.
     */
    void processarConsultaPacote(int tempo, int idPacote);

    /**
     * @brief Processa uma consulta do tipo 'CL' (Cliente).
     * @param tempo O timestamp da consulta.
     * @param nomeCliente O nome do cliente a ser consultado.
     */
    void processarConsultaCliente(int tempo, const std::string& nomeCliente);

    /**
     * @brief Converte uma string que representa um tipo de evento para o enum TipoEvento.
     * @param s A string a ser convertida.
     * @return O valor do enum correspondente.
     */
    static TipoEvento stringParaTipoEvento(const std::string& s);

    /**
     * @brief Ordena uma lista de eventos por tempo e, como critério de desempate, por ID do pacote.
     * @param eventos A lista de eventos a ser ordenada.
     */
    static void ordenarEventos(Lista<Evento*>& eventos);
};

#endif // CARREGAMENTO_HPP