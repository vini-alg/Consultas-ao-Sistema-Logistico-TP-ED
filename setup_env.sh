#!/bin/bash

# Verifica se o ambiente virtual já existe
if [ -d ".venv" ]; then
    echo "O ambiente virtual já existe. Pulando criação."
else
    # Cria o ambiente virtual chamado "venv"
    python3 -m venv .venv
    echo "Ambiente virtual criado com sucesso."
fi

# Instala as bibliotecas listadas no requirements.txt
if [ -f "requirements.txt" ]; then
    source .venv/bin/activate && pip install -r requirements.txt
    echo "Bibliotecas instaladas com sucesso."
else
    source .venv/bin/activate
    echo "Arquivo requirements.txt não encontrado."
fi
