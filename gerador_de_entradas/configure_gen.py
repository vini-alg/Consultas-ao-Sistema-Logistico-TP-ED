import re
import argparse
import os

def update_variable(content, var_name, new_value):
    """Atualiza uma variável no conteúdo do código C."""
    # Regex para encontrar "int var_name = value;" e substituir o valor
    pattern = re.compile(r"(int\s+{}\s*=\s*)\d+;".format(re.escape(var_name)))
    replacement = r"\g<1>{};".format(new_value)
    new_content, count = pattern.subn(replacement, content)
    if count == 0:
        print(f"Aviso: Variável '{var_name}' não encontrada ou não atualizada.")
    return new_content

def main():
    parser = argparse.ArgumentParser(description="Configura as variáveis de genwkl3.c.")
    parser.add_argument("--nodes", type=int, help="Número de nós (armazéns).")
    parser.add_argument("--packets", type=int, help="Número de pacotes.")
    parser.add_argument("--clients", type=int, help="Número de clientes.")
    parser.add_argument("--numcl", type=int, help="Número de consultas CL.")
    parser.add_argument("--numpc", type=int, help="Número de consultas PC.")
    parser.add_argument("--seed", type=int, help="Semente aleatória.")
    
    args = parser.parse_args()

    c_file_path = os.path.join(os.path.dirname(__file__), 'genwkl3.c')

    try:
        with open(c_file_path, 'r') as f:
            content = f.read()
    except FileNotFoundError:
        print(f"Erro: {c_file_path} não encontrado.")
        return

    var_map = {
        'nodes': 'nodes',
        'packets': 'numpackets',
        'clients': 'numclients',
        'numcl': 'numcl',
        'numpc': 'numpc',
        'seed': 'seed'
    }

    # Atualiza o conteúdo para cada argumento fornecido
    for arg, c_var in var_map.items():
        value = getattr(args, arg)
        if value is not None:
            content = update_variable(content, c_var, value)

    # Escreve o conteúdo modificado de volta
    with open(c_file_path, 'w') as f:
        f.write(content)

    # print(f"{c_file_path} atualizado com sucesso.")

if __name__ == "__main__":
    main()
