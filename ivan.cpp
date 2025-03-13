#include <iostream>
#include <queue>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <cstdlib>
#include <ctime>
#include <csignal>

using namespace std;

// Definições
#define NUM_ANDARES 4

// Estruturas
struct ChamadaElevador {
    int usuario;
    int origem;
    int destino;
};

// Variáveis globais
queue<ChamadaElevador> filaChamadas;
sem_t semChamadas; // Semáforo para sinalizar chamadas pendentes
sem_t semFila; // Semáforo para proteger a fila
int andarAtual = 0;
bool todasChamadasAtendidas = false; // Flag para encerrar o programa
pthread_t tElevador, tGeradorUsuarios; // Threads principais

// Função para movimentar o elevador
void moverPara(int destino) {
    while (andarAtual != destino) {
        sleep(2); // Simula tempo de deslocamento
        if (andarAtual < destino) andarAtual++;
        else andarAtual--;
        cout << "[Elevador] Movendo-se para o andar " << andarAtual << "..." << endl;
    }
}

// Função da thread do elevador
void* elevador(void*) {
    cout << "[Elevador] Inicializado no andar 0. Aguardando chamadas..." << endl;
    while (true) {
        sem_wait(&semChamadas); // Espera pelo menos uma chamada

        while (true) { // Continua atendendo chamadas enquanto houver na fila
            sem_wait(&semFila); // Bloqueia a fila para retirar chamadas
            if (filaChamadas.empty()) {
                sem_post(&semFila); // Libera a fila se não houver chamadas
                break; // Sai do loop e espera nova chamada
            }

            ChamadaElevador chamada = filaChamadas.front();
            filaChamadas.pop(); // Remove a primeira chamada
            sem_post(&semFila); // Libera a fila

            cout << "[Elevador] Nova chamada recebida de Usuário " << chamada.usuario
                 << ": Andar " << chamada.origem << " -> " << chamada.destino << "." << endl;
            moverPara(chamada.origem);

            cout << "[Elevador] Chegou ao andar " << chamada.origem << ". Usuário " << chamada.usuario << " embarcou." << endl;
            moverPara(chamada.destino);
            cout << "[Elevador] Chegou ao andar " << chamada.destino << ". Usuário " << chamada.usuario << " desembarcou." << endl;
        }

        // Se todas as chamadas foram atendidas e o programa foi encerrado, finaliza
        if (todasChamadasAtendidas) {
            cout << "[Elevador] Todas as chamadas foram atendidas. Encerrando operação..." << endl;
            break;
        }
    }
    return NULL;
}

// Função da thread de usuário
void* usuario(void* arg) {
    int id = ((int)arg);
    free(arg);

    int origem = rand() % NUM_ANDARES;
    int destino;
    do {
        destino = rand() % NUM_ANDARES;
    } while (destino == origem);

    cout << "[Usuário " << id << "] Chamando o elevador do andar " << origem << " para " << destino << "." << endl;

    sem_wait(&semFila); // Bloqueia a fila para adicionar um pedido
    filaChamadas.push({id, origem, destino}); // Adiciona à queue
    sem_post(&semFila); // Libera a fila

    sem_post(&semChamadas); // Avisa ao elevador que há uma nova chamada
    return NULL;
}

// Thread geradora de usuários indefinidos
void* geradorUsuarios(void*) {
    int contadorUsuarios = 1;
    while (!todasChamadasAtendidas) {
        sleep(rand() % 3 + 1); // Simula tempo aleatório para chamadas
        int* id = new int(contadorUsuarios++);
        pthread_t tUsuario;
        pthread_create(&tUsuario, NULL, usuario, id);
        pthread_detach(tUsuario); // O usuário se encerra automaticamente após sua execução
    }
    return NULL;
}

// Função para capturar sinais do sistema e encerrar corretamente
void sinalEncerramento(int signo) {
    if (signo == SIGINT) { // Detecta Ctrl+C
        cout << "\n[Sistema] Encerrando operação..." << endl;
        todasChamadasAtendidas = true;
        sem_post(&semChamadas); // Libera o elevador para finalizar
        pthread_join(tElevador, NULL); // Aguarda a finalização do elevador
        cout << "[Sistema] Finalizado com sucesso." << endl;
        sem_destroy(&semChamadas);
        sem_destroy(&semFila);
        exit(0);
    }
}

int main() {
    srand(time(NULL));
    sem_init(&semChamadas, 0, 0); // Inicializa semáforo de chamadas
    sem_init(&semFila, 0, 1); // Inicializa semáforo da fila

    // Captura Ctrl+C para encerrar corretamente
    signal(SIGINT, sinalEncerramento);

    // Cria as threads principais
    pthread_create(&tElevador, NULL, elevador, NULL);
    pthread_create(&tGeradorUsuarios, NULL, geradorUsuarios, NULL);

    // Mantém o programa rodando até ser interrompido pelo usuário
    pthread_join(tGeradorUsuarios, NULL);
    return 0;
}
