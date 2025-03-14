#include <iostream>
#include <queue>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <cstdlib>
#include <ctime>

using namespace std;
#define NUM_ANDARES 5 

//variáveis 
struct ChamadaElevador {
    int usuario;
    int origem;
    int destino;
};
queue<ChamadaElevador> filaChamadas;
sem_t semChamadas; //sémaforo das chamadas 
sem_t semFila; //sémaforo da fila
int andarAtual = 0;

//deslocamento elevador
void moverPara(int destino) {
    while (andarAtual != destino) {
        sleep(2); 
        if (andarAtual < destino) andarAtual++;
        else andarAtual--;
        cout << "[Elevador] Movendo-se para o andar " << andarAtual << "..." << endl;
    }
}

//thread elevador 
void* elevador(void*) {
    cout << "[Elevador] Inicializado no andar 0. Aguardando chamadas..." << endl;
    while (true) {
        sem_wait(&semChamadas); // espera chamadas

        while (true) { // enquanto houver chamadas atende qnd acbar sai do loop
            sem_wait(&semFila);
            if (filaChamadas.empty()) {
                sem_post(&semFila);
                break; 
            }



            ChamadaElevador chamada = filaChamadas.front(); //manutenção da fila 
            filaChamadas.pop();
            sem_post(&semFila); 

            cout << "[Elevador] Nova chamada recebida de Usuário " << chamada.usuario
                 << ": Andar " << chamada.origem << " -> " << chamada.destino << "." << endl;
            moverPara(chamada.origem);
            cout << "[Elevador] Chegou ao andar " << chamada.origem << ". Usuário " << chamada.usuario << " embarcou." << endl;
            sleep(1);
            moverPara(chamada.destino);
            cout << "[Elevador] Chegou ao andar " << chamada.destino << ". Usuário " << chamada.usuario << " desembarcou." << endl;
            sleep(1); 
        }

        cout << "[Elevador] Nenhuma chamada pendente. Retornando ao modo de espera." << endl;
    }
    return NULL;
}

//   thread usuário
void* usuario(void* arg) {




    int id = *((int*)arg);
    delete (int*)arg;
    //randomiza o andar de origem e pra onde ele vai 
    int origem = rand() % NUM_ANDARES;
    int destino;
    do {
        destino = rand() % NUM_ANDARES;
    } while (destino == origem);

    cout << "[Usuário " << id << "] Chamando o elevador do andar " << origem << " para " << destino << "." << endl;
    sem_wait(&semFila);
    filaChamadas.push({id, origem, destino});
    sem_post(&semFila);
    

    sem_post(&semChamadas); 
    return NULL;
}

// continua criando usuários de forma indefinida 

void* gerarUsuarios(void*) {
    int id = 1;
    while (true) {
        sleep(rand() % 4 + 1); 
        int* novoId = new int(id++);
        pthread_t tUsuario;
        pthread_create(&tUsuario, NULL, usuario, novoId);
        pthread_detach(tUsuario); 
    }
    return NULL;
}

int main() {

    srand(time(NULL));
    pthread_t tElevador, tGeradorUsuarios; //gera as threads do delegador e do usuário

    sem_init(&semChamadas, 0, 0);
    sem_init(&semFila, 0, 1);

    pthread_create(&tElevador, NULL, elevador, NULL);
    pthread_create(&tGeradorUsuarios, NULL, gerarUsuarios, NULL);
    pthread_join(tElevador, NULL); 
    return 0;
}
