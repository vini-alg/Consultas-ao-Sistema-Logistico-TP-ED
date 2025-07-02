#include "../include/Carregamento.hpp"
#include "Evento.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <iomanip> // Para std::setw e std::setfill

// Função de comparação para ordenar eventos por ID do pacote
bool compararEventosPorPacote(Evento* a, Evento* b) {
    return a->idPacote < b->idPacote;
}

Carregamento::Carregamento(const std::string& filename) {
    this->filename = filename;
    mapaPacotes = new Mapa<int, Lista<Evento*>*>();
    mapaClientes = new Mapa<std::string, Lista<int>*>();
}

Carregamento::~Carregamento() {
    // Libera a memória de todos os eventos criados
    for (int i = 0; i < eventos.tamanho(); ++i) {
        delete eventos.obter(i);
    }

    // Libera a memória das listas dentro do mapa de pacotes
    Lista<Lista<Evento*>*> listasDeEventos = mapaPacotes->obterValores();
    for (int i = 0; i < listasDeEventos.tamanho(); ++i) {
        delete listasDeEventos.obter(i);
    }

    // Libera a memória das listas dentro do mapa de clientes
    Lista<Lista<int>*> listasDePacotes = mapaClientes->obterValores();
    for (int i = 0; i < listasDePacotes.tamanho(); ++i) {
        delete listasDePacotes.obter(i);
    }

    // Libera a memória dos próprios mapas
    delete mapaPacotes;
    delete mapaClientes;
}

void Carregamento::executar() {
    std::ifstream inputFile(this->filename);
    std::string line;

    if (!inputFile.is_open()) {
        std::cerr << "Erro ao abrir o arquivo: " << this->filename << std::endl;
        return;
    }

    while (std::getline(inputFile, line)) {
        processarLinha(line);
    }

    inputFile.close();
}

TipoEvento Carregamento::stringParaTipoEvento(const std::string& s) {
    if (s == "RG") return RG;
    if (s == "AR") return AR;
    if (s == "RM") return RM;
    if (s == "UR") return UR;
    if (s == "TR") return TR;
    if (s == "EN") return EN;
    return INDEFINIDO;
}

void Carregamento::processarLinha(const std::string& linha) {
    std::stringstream ss(linha);
    int dataHora;
    std::string tipoComando;

    ss >> dataHora >> tipoComando;

    if (tipoComando == "EV") {
        Evento* ev = new Evento();
        ev->dataHora = dataHora;
        std::string tipoEventoStr;
        ss >> tipoEventoStr;
        ev->tipo = stringParaTipoEvento(tipoEventoStr);
        
        ss >> ev->idPacote;

        switch (ev->tipo) {
            case RG: {
                ss >> ev->remetente >> ev->destinatario >> ev->armazemOrigem >> ev->armazemDestino;
                
                // Atualiza o índice de clientes para o remetente
                Lista<int>* pacotesRemetente = mapaClientes->buscar(ev->remetente);
                if (pacotesRemetente == nullptr) {
                    pacotesRemetente = new Lista<int>();
                    mapaClientes->inserir(ev->remetente, pacotesRemetente);
                }
                if (!pacotesRemetente->contem(ev->idPacote)) {
                    pacotesRemetente->adicionar(ev->idPacote);
                }

                // Atualiza o índice de clientes para o destinatário
                Lista<int>* pacotesDestinatario = mapaClientes->buscar(ev->destinatario);
                if (pacotesDestinatario == nullptr) {
                    pacotesDestinatario = new Lista<int>();
                    mapaClientes->inserir(ev->destinatario, pacotesDestinatario);
                }
                if (!pacotesDestinatario->contem(ev->idPacote)) {
                    pacotesDestinatario->adicionar(ev->idPacote);
                }
                break;
            }
            case AR:
                ss >> ev->armazemOrigem >> ev->secaoDestino;
                break;
            case RM:
                ss >> ev->armazemOrigem >> ev->secaoDestino;
                break;
            case UR:
                ss >> ev->armazemOrigem >> ev->secaoDestino;
                break;
            case TR:
                ss >> ev->armazemOrigem >> ev->armazemDestino;
                break;
            case EN:
                ss >> ev->armazemDestino;
                break;
            case INDEFINIDO:
                // Tratar erro ou ignorar
                break;
        }

        eventos.adicionar(ev);

        // Atualiza o índice de pacotes
        Lista<Evento*>* listaEventosPacote = mapaPacotes->buscar(ev->idPacote);
        if (listaEventosPacote == nullptr) { // Pacote não está no mapa ainda
            listaEventosPacote = new Lista<Evento*>();
            mapaPacotes->inserir(ev->idPacote, listaEventosPacote);
        }
        listaEventosPacote->adicionar(ev);

    } else if (tipoComando == "CL") {
        std::string nomeCliente;
        ss >> nomeCliente;
        // Lógica da consulta de cliente (CL) usando índices
        std::cout << std::setw(7) << std::setfill('0') << dataHora << " CL " << nomeCliente << std::endl;

        Lista<Evento*> ultimosEventos;
        Lista<int>* pacotesDoCliente = mapaClientes->buscar(nomeCliente);

        if (pacotesDoCliente != nullptr) {
            for (int i = 0; i < pacotesDoCliente->tamanho(); ++i) {
                int idPacote = pacotesDoCliente->obter(i);
                Evento* ultimoEvento = nullptr;
                
                Lista<Evento*>* todosEventosDoPacote = mapaPacotes->buscar(idPacote);
                if (todosEventosDoPacote != nullptr) {
                    for (int j = 0; j < todosEventosDoPacote->tamanho(); ++j) {
                        Evento* ev = todosEventosDoPacote->obter(j);
                        if (ev->dataHora <= dataHora) {
                            if (ultimoEvento == nullptr || ev->dataHora > ultimoEvento->dataHora) {
                                ultimoEvento = ev;
                            }
                        }
                    }
                }
                
                if (ultimoEvento != nullptr) {
                    ultimosEventos.adicionar(ultimoEvento);
                }
            }
        }

        // Ordenar e imprimir a saída
        ultimosEventos.ordenar(compararEventosPorPacote);
        std::cout << ultimosEventos.tamanho() << std::endl;
        for (int i = 0; i < ultimosEventos.tamanho(); ++i) {
            ultimosEventos.obter(i)->imprimir();
        }

    } else if (tipoComando == "PC") {
        int idPacote;
        ss >> idPacote;
        // Lógica da consulta de pacote (PC) usando índices
        std::cout << std::setw(7) << std::setfill('0') << dataHora << " PC " << idPacote << std::endl;
        
        Lista<Evento*> eventosFiltrados;
        Lista<Evento*>* todosEventosDoPacote = mapaPacotes->buscar(idPacote);

        if (todosEventosDoPacote != nullptr) {
            for (int i = 0; i < todosEventosDoPacote->tamanho(); ++i) {
                Evento* ev = todosEventosDoPacote->obter(i);
                if (ev->dataHora <= dataHora) {
                    eventosFiltrados.adicionar(ev);
                }
            }
        }

        std::cout << eventosFiltrados.tamanho() << std::endl;
        for (int i = 0; i < eventosFiltrados.tamanho(); ++i) {
            eventosFiltrados.obter(i)->imprimir();
        }
    }
}
