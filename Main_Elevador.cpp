#include <iostream>
#include <queue>
#include <thread>
#include <chrono>
#include <random>
#include <semaphore>
#include <atomic>

using namespace std;

// =============================================
// Estrutura de dados para representar uma chamada
// =============================================
struct Chamada {
    int id;         // Identificador do usuário
    int origem;     // Andar onde o usuário está
    int destino;    // Andar para onde o usuário quer ir
};

// =============================================
// Variáveis globais de sincronização
// =============================================
queue<Chamada> fila;                        // Fila compartilhada de chamadas
binary_semaphore sem_fila(1);               // Controla acesso exclusivo à fila (substituto do mutex)
counting_semaphore<INT_MAX> sem_chamadas(0); // Contador de chamadas pendentes
atomic<bool> executando(true);              // Flag para controle de execução
int andar_atual = 0;                        // Estado atual do elevador

// =============================================
// Gerador de números aleatórios (andares 0-4)
// =============================================
random_device rd;
mt19937 gen(rd());

// =============================================
// Lógica principal do elevador
// =============================================
void elevador() {
    while (executando) {
        sem_chamadas.acquire(); // Bloqueia até haver chamadas (decrementa o contador)

        // ---- Seção crítica: acesso à fila ----
        sem_fila.acquire(); // Bloqueia acesso à fila
        Chamada chamada = fila.front(); // Lê a primeira chamada
        fila.pop();         // Remove da fila
        sem_fila.release(); // Libera acesso à fila
        // ---------------------------------------

        // Processamento da chamada
        cout << "[Elevador] Nova chamada recebida de Usuário " << chamada.id 
             << ": Andar " << chamada.origem << " -> " << chamada.destino << endl;

        // Movimento para origem do usuário
        while (andar_atual != chamada.origem) {
            this_thread::sleep_for(1s); // Simula tempo de movimento
            andar_atual += (chamada.origem > andar_atual) ? 1 : -1;
            cout << "[Elevador] Movendo-se para o andar " << andar_atual << "..." << endl;
        }

        cout << "[Elevador] Chegou ao andar " << andar_atual << ". Usuário " 
             << chamada.id << " embarcou." << endl;

        this_thread::sleep_for(2s); // Simula tempo de embarque

        // Movimento para destino do usuário
        while (andar_atual != chamada.destino) {
            this_thread::sleep_for(1s);
            andar_atual += (chamada.destino > andar_atual) ? 1 : -1;
            cout << "[Elevador] Movendo-se para o andar " << andar_atual << "..." << endl;
        }

        cout << "[Elevador] Chegou ao andar " << andar_atual << ". Usuário " 
             << chamada.id << " desembarcou." << endl;
    }
}

// =============================================
// Lógica de geração de usuários
// =============================================
void usuario(int id) {
    uniform_int_distribution<> andares(0, 4);
    Chamada chamada;
    chamada.id = id;
    
    // Garante origem e destino diferentes
    do {
        chamada.origem = andares(gen);
        chamada.destino = andares(gen);
    } while (chamada.origem == chamada.destino);

    // ---- Seção crítica: adição à fila ----
    sem_fila.acquire(); // Bloqueia acesso à fila
    cout << "[Usuário " << id << "] Chamando o elevador do andar " 
         << chamada.origem << " para o andar " << chamada.destino << "." << endl;
    fila.push(chamada); // Adiciona chamada à fila
    sem_fila.release(); // Libera acesso à fila
    // ---------------------------------------

    sem_chamadas.release(); // Notifica o elevador (+1 chamada)
}

// =============================================
// Função principal
// =============================================
int main() {
    cout << "[Elevador] Inicializado no andar 0. Aguardando chamadas..." << endl;

    thread elevator_thread(elevador); // Inicia thread do elevador
    vector<thread> usuarios;          // Pool de threads de usuários
    int id_counter = 1;               // Contador de IDs

    // Gera usuários aleatoriamente
    while (executando) {
        this_thread::sleep_for(chrono::seconds(rand() % 3 + 1)); // Intervalo entre 1-3 segundos
        usuarios.emplace_back(usuario, id_counter++); // Cria nova thread de usuário
    }

    // Finalização
    elevator_thread.join();
    for (auto& t : usuarios) {
        if (t.joinable()) t.join();
    }

    return 0;
}