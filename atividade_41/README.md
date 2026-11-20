# Atividade 41 - Agenda de missões (programa modular)

A agenda foi dividida em três arquivos, cada um com uma responsabilidade:

| Arquivo    | Papel                                                        |
|------------|--------------------------------------------------------------|
| `agenda.h` | Interface pública: tipo `Missao`, constantes e protótipos    |
| `agenda.c` | Implementação: corpo das funções                             |
| `main.c`   | Programa que usa a agenda, conhecendo só o que está no `.h`  |

## Como compilar e executar

Compilar cada módulo separado e depois vincular os objetos:

```bash
gcc -Wall -Wextra -std=c99 -pedantic -c agenda.c
gcc -Wall -Wextra -std=c99 -pedantic -c main.c
gcc main.o agenda.o -o agenda_app
./agenda_app
```

Ou tudo de uma vez: `gcc -Wall -Wextra -std=c99 -pedantic main.c agenda.c -o agenda_app`

## Mapa dos requisitos no código

| Requisito / conceito                         | Onde está                                  |
|----------------------------------------------|--------------------------------------------|
| Include guards (abrem)                       | `agenda.h` linhas 15-16 (`#ifndef` / `#define`) |
| Include guards (fecham)                      | `agenda.h` linha 51 (`#endif`)             |
| Constantes compartilhadas                    | `agenda.h` linhas 19, 22 e 23              |
| Struct `Missao` (tipo no local adequado)     | `agenda.h` linhas 30-35                    |
| Protótipos (declaração no `.h`)              | `agenda.h` linhas 43, 46 e 49              |
| Criação de missão (implementação)            | `agenda.c` linhas 12-31                    |
| Ajuste da prioridade para os limites         | `agenda.c` linhas 20-26                    |
| Exibição de missão (implementação)           | `agenda.c` linhas 33-43                    |
| Conclusão de missão (implementação)          | `agenda.c` linhas 45-51                    |
| `.c` inclui o próprio `.h`                   | `agenda.c` linha 9                         |
| `main` inclui só o cabeçalho, nunca um `.c`  | `main.c` linha 10                          |
| Sem variável global (agenda é local à main)  | `main.c` linha 16                          |
| Uso da interface: criar                      | `main.c` linhas 18-21                      |
| Uso da interface: exibir                     | `main.c` linhas 25 e 33                    |
| Uso da interface: concluir                   | `main.c` linha 29                          |

## Ideia central

O `main.c` só enxerga o `agenda.h`. Se a implementação em `agenda.c` mudar
(por exemplo, trocar o formato do `printf`), o `main.c` não precisa mudar.
Cada `.c` é compilado sozinho em um arquivo objeto (`.o`) e o *linker* junta
os objetos no executável final.
