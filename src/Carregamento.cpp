#include "../include/Carregamento.hpp"
#include <fstream>
#include <sstream>
#include <iomanip> // Para std::setw e std::setfill
#include <chrono>  // Para medição de tempo

/**
 * @file Carregamento.cpp
 * @brief Implementação da classe Carregamento.
 */

Carregamento::Carregamento(const std::string& filename) : filename(filename) {
    mapaPacotes = new Mapa<int, Lista<Evento*>*>();
    mapaClientes = new Mapa<std::string, PacotesCliente*>();
}

Carregamento::~Carregamento() {
    // 1. Libera a memória de todos os objetos Evento.
    for (int i = 0; i < eventos.getTamanho(); ++i) {
        delete eventos.obter(i);
    }

    // 2. Libera as listas de eventos dentro do mapa de pacotes.
    Lista<Lista<Evento*>*> listasDeEventos = mapaPacotes->obterValores();
    for (int i = 0; i < listasDeEventos.getTamanho(); ++i) {
        delete listasDeEventos.obter(i);
    }

    // 3. Libera os objetos PacotesCliente dentro do mapa de clientes.
    Lista<PacotesCliente*> listasDePacotes = mapaClientes->obterValores();
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

    // Carrega todas as linhas para a memória primeiro
    Lista<std::string> linhas;
    std::string linha;
    while (std::getline(inputFile, linha)) {
        if (!linha.empty()) {
            linhas.adicionar(linha);
        }
    }
    inputFile.close();

    // Agora, processa as linhas a partir da memória
    for (int i = 0; i < linhas.getTamanho(); ++i) {
        processarLinha(linhas.obter(i));
    }
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
                
                // Processa remetente
                PacotesCliente* pacotesRemetente = mapaClientes->buscar(ev->remetente);
                if (pacotesRemetente == nullptr) {
                    pacotesRemetente = new PacotesCliente();
                    mapaClientes->inserir(ev->remetente, pacotesRemetente);
                }
                if (!pacotesRemetente->enviados.contem(ev->idPacote)) {
                    pacotesRemetente->enviados.adicionar(ev->idPacote);
                }

                // Processa destinatário
                PacotesCliente* pacotesDestinatario = mapaClientes->buscar(ev->destinatario);
                if (pacotesDestinatario == nullptr) {
                    pacotesDestinatario = new PacotesCliente();
                    mapaClientes->inserir(ev->destinatario, pacotesDestinatario);
                }
                if (!pacotesDestinatario->aReceber.contem(ev->idPacote)) {
                    pacotesDestinatario->aReceber.adicionar(ev->idPacote);
                }
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
    std::cout << std::setw(6) << std::setfill('0') << tempo << " PC " << std::setw(3) << std::setfill('0') << idPacote << std::endl;

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
    std::cout << std::setw(6) << std::setfill('0') << tempo << " CL " << nomeCliente << std::endl;

    PacotesCliente* pacotesCliente = mapaClientes->buscar(nomeCliente);
    if (pacotesCliente == nullptr) {
        std::cout << 0 << std::endl;
        return;
    }

    Lista<Evento*> eventosResultantes;
    Lista<int> todosOsIds;
    // Coleta IDs de pacotes enviados
    for(int i = 0; i < pacotesCliente->enviados.getTamanho(); ++i) {
        todosOsIds.adicionar(pacotesCliente->enviados.obter(i));
    }
    // Coleta IDs de pacotes a receber
    for(int i = 0; i < pacotesCliente->aReceber.getTamanho(); ++i) {
        int idPacote = pacotesCliente->aReceber.obter(i);
        if (!todosOsIds.contem(idPacote)) { // Evita duplicatas
            todosOsIds.adicionar(idPacote);
        }
    }

    for (int i = 0; i < todosOsIds.getTamanho(); ++i) {
        int idPacote = todosOsIds.obter(i);
        Lista<Evento*>* eventosPacote = mapaPacotes->buscar(idPacote);
        if (eventosPacote == nullptr) continue;

        Evento* ultimoEvento = nullptr;
        Evento* eventoRegistro = nullptr;

        for (int j = 0; j < eventosPacote->getTamanho(); ++j) {
            Evento* ev = eventosPacote->obter(j);
            if (ev->getTempo() <= tempo) {
                // Encontra o evento de registro (RG)
                if (ev->tipo == RG) {
                    eventoRegistro = ev;
                }
                // Encontra o último evento no tempo, com desempate pelo tipo de evento
                if (ultimoEvento == nullptr || ev->getTempo() > ultimoEvento->getTempo() ||
                   (ev->getTempo() == ultimoEvento->getTempo() && ev->tipo > ultimoEvento->tipo)) {
                    ultimoEvento = ev;
                }
            }
        }

        // Adiciona o evento de registro, se encontrado e ainda não estiver na lista
        if (eventoRegistro != nullptr && !eventosResultantes.contem(eventoRegistro)) {
            eventosResultantes.adicionar(eventoRegistro);
        }
        // Adiciona o último evento, se encontrado e ainda não estiver na lista
        if (ultimoEvento != nullptr && !eventosResultantes.contem(ultimoEvento)) {
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

void Carregamento::ordenarEventos(Lista<Evento*>& lista) {
    lista.ordenar(compararEventos);
}

void Carregamento::executarComTiming() {
    // 1. Ler todas as linhas para a memória primeiro
    std::ifstream inputFile(this->filename);
    if (!inputFile.is_open()) {
        std::cerr << "Erro ao abrir o arquivo: " << this->filename << std::endl;
        return;
    }

    Lista<std::string> linhas;
    std::string linha;
    while (std::getline(inputFile, linha)) {
        if (!linha.empty()) {
            linhas.adicionar(linha);
        }
    }
    inputFile.close();

    // 2. Medir o tempo de processamento
    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < linhas.getTamanho(); ++i) {
        processarLinha(linhas.obter(i));
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;

    // Imprime o tempo de processamento em stderr para não interferir com a saída padrão
    std::cerr << "Processing time: " << duration.count() << " ms" << std::endl;
}
