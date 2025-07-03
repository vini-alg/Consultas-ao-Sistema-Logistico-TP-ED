#include "../include/Carregamento.hpp"
#include <fstream>
#include <sstream>
#include <iomanip> // Para std::setw e std::setfill

/**
 * @file Carregamento.cpp
 * @brief Implementação da classe Carregamento.
 */

Carregamento::Carregamento(const std::string& filename) : filename(filename) {
    mapaPacotes = new Mapa<int, Lista<Evento*>*>();
    mapaClientes = new Mapa<std::string, Lista<int>*>();
}

Carregamento::~Carregamento() {
    // 1. Libera a memória de todos os objetos Evento.
    // A lista `eventos` é a fonte da verdade e detém a posse dos ponteiros.
    for (int i = 0; i < eventos.getTamanho(); ++i) {
        delete eventos.obter(i);
    }

    // 2. Libera as listas de eventos dentro do mapa de pacotes.
    Lista<Lista<Evento*>*> listasDeEventos = mapaPacotes->obterValores();
    for (int i = 0; i < listasDeEventos.getTamanho(); ++i) {
        delete listasDeEventos.obter(i);
    }

    // 3. Libera as listas de IDs de pacotes dentro do mapa de clientes.
    Lista<Lista<int>*> listasDePacotes = mapaClientes->obterValores();
    for (int i = 0; i < listasDePacotes.getTamanho(); ++i) {
        delete listasDePacotes.obter(i);
    }

    // 4. Libera os próprios mapas.
    delete mapaPacotes;
    delete mapaClientes;
}

void Carregamento::executar() {
    std::ifstream inputFile(this->filename);
    if (!inputFile.is_open()) {
        std::cerr << "Erro ao abrir o arquivo: " << this->filename << std::endl;
        return;
    }

    std::string linha;
    while (std::getline(inputFile, linha)) {
        if (!linha.empty()) {
            processarLinha(linha);
        }
    }

    inputFile.close();
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
        ss >> tipoEventoStr >> ev->idPacote;
        ev->tipo = stringParaTipoEvento(tipoEventoStr);

        switch (ev->tipo) {
            case RG: {
                ss >> ev->remetente >> ev->destinatario >> ev->armazemOrigem >> ev->armazemDestino;
                Lista<int>* pacotesRemetente = mapaClientes->buscar(ev->remetente);
                if (pacotesRemetente == nullptr) {
                    pacotesRemetente = new Lista<int>();
                    mapaClientes->inserir(ev->remetente, pacotesRemetente);
                }
                if (!pacotesRemetente->contem(ev->idPacote)) pacotesRemetente->adicionar(ev->idPacote);

                Lista<int>* pacotesDestinatario = mapaClientes->buscar(ev->destinatario);
                if (pacotesDestinatario == nullptr) {
                    pacotesDestinatario = new Lista<int>();
                    mapaClientes->inserir(ev->destinatario, pacotesDestinatario);
                }
                if (!pacotesDestinatario->contem(ev->idPacote)) pacotesDestinatario->adicionar(ev->idPacote);
                break;
            }
            case AR: ss >> ev->armazemOrigem >> ev->secaoDestino; break;
            case RM: ss >> ev->armazemOrigem >> ev->secaoDestino; break;
            case UR: ss >> ev->armazemOrigem >> ev->secaoDestino; break;
            case TR: ss >> ev->armazemOrigem >> ev->armazemDestino; break;
            case EN: ss >> ev->armazemDestino; break;
            case INDEFINIDO: delete ev; return; // Ignora evento inválido e libera memória
        }

        eventos.adicionar(ev);

        Lista<Evento*>* listaEventosPacote = mapaPacotes->buscar(ev->idPacote);
        if (listaEventosPacote == nullptr) {
            listaEventosPacote = new Lista<Evento*>();
            mapaPacotes->inserir(ev->idPacote, listaEventosPacote);
        }
        listaEventosPacote->adicionar(ev);

    } else if (tipoComando == "CL") {
        std::string nomeCliente;
        ss >> nomeCliente;
        processarConsultaCliente(dataHora, nomeCliente);
    } else if (tipoComando == "PC") {
        int idPacote;
        ss >> idPacote;
        processarConsultaPacote(dataHora, idPacote);
    }
}

void Carregamento::processarConsultaPacote(int tempo, int idPacote) {
    std::cout << std::setw(7) << std::setfill('0') << tempo << " PC " << std::setw(3) << std::setfill('0') << idPacote << std::endl;

    Lista<Evento*>* todosEventos = mapaPacotes->buscar(idPacote);
    if (todosEventos == nullptr) {
        std::cout << 0 << std::endl;
        return;
    }

    Lista<Evento*> eventosFiltrados;
    for (int i = 0; i < todosEventos->getTamanho(); ++i) {
        Evento* ev = todosEventos->obter(i);
        if (ev->getTempo() <= tempo) {
            eventosFiltrados.adicionar(ev);
        }
    }

    std::cout << eventosFiltrados.getTamanho() << std::endl;
    for (int i = 0; i < eventosFiltrados.getTamanho(); ++i) {
        eventosFiltrados.obter(i)->imprimir();
    }
}

void Carregamento::processarConsultaCliente(int tempo, const std::string& nomeCliente) {
    std::cout << std::setw(7) << std::setfill('0') << tempo << " CL " << nomeCliente << std::endl;

    Lista<int>* idPacotes = mapaClientes->buscar(nomeCliente);
    if (idPacotes == nullptr || idPacotes->getTamanho() == 0) {
        std::cout << 0 << std::endl;
        return;
    }

    Lista<Evento*> eventosResultantes;
    for (int i = 0; i < idPacotes->getTamanho(); ++i) {
        int idPacote = idPacotes->obter(i);
        Lista<Evento*>* eventosPacote = mapaPacotes->buscar(idPacote);
        if (eventosPacote == nullptr) continue;

        Evento* ultimoEvento = nullptr;
        for (int j = 0; j < eventosPacote->getTamanho(); ++j) {
            Evento* ev = eventosPacote->obter(j);
            if (ev->getTempo() <= tempo) {
                if (ultimoEvento == nullptr || ev->getTempo() > ultimoEvento->getTempo()) {
                    ultimoEvento = ev;
                }
            }
        }
        if (ultimoEvento != nullptr) {
            eventosResultantes.adicionar(ultimoEvento);
        }
    }

    ordenarEventos(eventosResultantes);

    std::cout << eventosResultantes.getTamanho() << std::endl;
    for (int i = 0; i < eventosResultantes.getTamanho(); ++i) {
        eventosResultantes.obter(i)->imprimir();
    }
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

// Função de comparação estática para o método de ordenação.
static bool compararEventos(Evento* a, Evento* b) {
    if (a->getTempo() != b->getTempo()) {
        return a->getTempo() < b->getTempo();
    }
    return a->idPacote < b->idPacote;
}

void Carregamento::ordenarEventos(Lista<Evento*>& eventos) {
    eventos.ordenar(compararEventos);
}
