# Projeto-elevador---STR-2024.2
# Projeto de Controle de Elevador Concorrente

## Descrição

Este projeto implementa um **simulador de elevador concorrente** em C++, utilizando threads e semáforos para gerenciar múltiplos usuários em um prédio de 5 andares. O sistema garante o atendimento de chamadas em ordem FIFO, evitando condições de corrida e garantindo sincronização entre as threads. O objetivo principal é simular o movimento do elevador de forma eficiente, seguindo regras pré-definidas.


## Funcionalidades

- **Controle concorrente de usuários** através de threads;
- **Uso de semáforos** para sincronização e exclusão mútua;
- **Fila FIFO** para gerenciamento de chamadas;
- Simulação de movimento entre andares com tempos de embarque/desembarque;
- Logs detalhados para acompanhamento em tempo real.

## Regras do Elevador

- Inicia no **andar 0** e só se move quando há chamadas pendentes.
- Atende chamadas na ordem de chegada (FIFO).
- Aceita novas chamadas durante o deslocamento.
- Semáforos garantem acesso seguro à fila compartilhada.

## Exemplo de Execução

```plaintext
[Elevador] Inicializado no andar 0. Aguardando chamadas...
[Usuário 1] Chamando o elevador do andar 1 para 3.
[Elevador] Nova chamada recebida de Usuário 1: Andar 1 -> 3.
[Elevador] Movendo-se para o andar 1...
[Elevador] Chegou ao andar 1. Usuário 1 embarcou.
[Usuário 2] Chamando o elevador do andar 0 para 3.
[Usuário 3] Chamando o elevador do andar 3 para 2.
[Elevador] Movendo-se para o andar 2...
[Elevador] Movendo-se para o andar 3...
[Elevador] Chegou ao andar 3. Usuário 1 desembarcou.
[Elevador] Nova chamada recebida de Usuário 2: Andar 0 -> 3.
[Elevador] Movendo-se para o andar 2...
[Elevador] Movendo-se para o andar 1...
[Elevador] Movendo-se para o andar 0...
[Elevador] Chegou ao andar 0. Usuário 2 embarcou.
[Elevador] Movendo-se para o andar 1...
[Elevador] Movendo-se para o andar 2...
[Elevador] Movendo-se para o andar 3...
[Elevador] Chegou ao andar 3. Usuário 2 desembarcou.
[Elevador] Nova chamada recebida de Usuário 3: Andar 3 -> 2.
[Elevador] Chegou ao andar 3. Usuário 3 embarcou.
[Elevador] Movendo-se para o andar 2...
[Elevador] Chegou ao andar 2. Usuário 3 desembarcou.
[Elevador] Todas as chamadas foram atendidas. Encerrando operação...
```
Vídeo de Demonstração <!-- Substitua pelo link do YouTube -->

## Como Executar
Pré-requisitos:
-Compilador C++(ex: g++).
-Biblioteca pthreads.

Compilação e Execução
# Clone o repositório
```plaintext
git clone https://github.com/seu-usuario/projeto-elevador.git
```
# Compile o código
```plaintext
g++ -pthread Main_Elevador.cpp -o elevador
```
# Execute o programa
```plaintext
./elevador
```
## Análise de Sincronização
O sistema utiliza dois semáforos:

- semFila (binário): Controla acesso à fila de chamadas.
- semChamadas (contador): Notifica o elevador sobre novas requisições.


## Contribuições
Contribuições são bem-vindas! Siga os passos:

Faça um fork do projeto.

Crie uma branch: git checkout -b feature/nova-funcionalidade.

Commit suas mudanças: git commit -m 'Adicione uma funcionalidade'.

Push para a branch: git push origin feature/nova-funcionalidade.

Abra um Pull Request.



## Autores
Ádson Vital Correia (adson.correia@ee.ufcg.edu.br); 

Arthur de Queiroz Tavares Borges Mesquisa (arthur.mesquita@ee.ufcg.edu.br)

Ivan da Silva Filho (ivan.filho@ee.ufcg.edu.br)

## Licença
Distribuído sob a licença MIT. Veja LICENSE para detalhes.
