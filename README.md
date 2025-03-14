# Projeto-elevador---STR-2024.2
# Projeto de Controle de Elevador Concorrente

## Descrição

Este projeto implementa um **simulador de elevador concorrente** em C++, utilizando threads e semáforos para gerenciar múltiplos usuários em um prédio de 5 andares. O sistema garante o atendimento de chamadas em ordem FIFO, evitando condições de corrida e garantindo sincronização entre as threads. O objetivo principal é simular o movimento do elevador de forma eficiente, seguindo regras pré-definidas.

![Diagrama do Sistema](https://via.placeholder.com/800x400.png?text=Diagrama+do+Fluxo+do+Elevador) <!-- Substitua pelo link da imagem -->

## Funcionalidades

- **Controle concorrente de usuários** através de threads.
- **Uso de semáforos** para sincronização e exclusão mútua.
- **Fila FIFO** para gerenciamento de chamadas.
- Simulação de movimento entre andares com tempos de embarque/desembarque.
- Logs detalhados para acompanhamento em tempo real.

## Regras do Elevador

- Inicia no **andar 0** e só se move quando há chamadas pendentes.
- Atende chamadas na ordem de chegada (FIFO).
- Aceita novas chamadas durante o deslocamento.
- Semáforos garantem acesso seguro à fila compartilhada.

## Exemplo de Execução

```plaintext
[Elevador] Inicializado no andar 0. Aguardando chamadas...
[Usuário 1] Chamando o elevador do andar 2 para o andar 4.
[Usuário 2] Chamando o elevador do andar 0 para o andar 3.
[Elevador] Nova chamada recebida de Usuário 1: Andar 2 -> 4.
[Elevador] Movendo-se para o andar 2...
[Elevador] Chegou ao andar 2. Usuário 1 embarcou.
[Elevador] Movendo-se para o andar 4...
[Elevador] Chegou ao andar 4. Usuário 1 desembarcou.
[Elevador] Nova chamada recebida de Usuário 2: Andar 0 -> 3.
[Elevador] Movendo-se para o andar 3...
[Elevador] Chegou ao andar 3. Usuário 2 desembarcou.
```
Vídeo de Demonstração <!-- Substitua pelo link do YouTube -->

## Como Executar
Pré-requisitos

Compilador C++ compatível com C++20 (ex: g++).
Biblioteca de threads do sistema.

Compilação e Execução
# Clone o repositório
git clone https://github.com/seu-usuario/projeto-elevador.git

# Compile o código
g++ -std=c++20 -pthread src/elevador.cpp -o elevador

# Execute o programa
./elevador

## Análise de Sincronização
O sistema utiliza dois semáforos:

sem_fila (binário): Controla acesso à fila de chamadas.

sem_chamadas (contador): Notifica o elevador sobre novas requisições.

Fluxo de Sincronização <!-- Substitua pelo link da imagem -->

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
