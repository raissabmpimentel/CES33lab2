#include <semaphore.h> //Semaforos
#include <vector>
#include <string>
#include <unordered_map>
#include <pthread.h>
#include <fstream>
#include <iostream>
#include <sstream> // stringstream
#include <sys/types.h> // Para leitura de arquivos
#include <dirent.h> // Para leitura de arquivos
#include <time.h> // Para medir tempo

#define NUM_MRG_THREADS 2 // Numero de threads para o merge sort
#define NUM_TESTS 100 // Numero de testes

using namespace std;

typedef struct{
string day; // Dia do preco
string hour; // Hora do preco
float price; // Preco
} Row;

int items_per_thread; //Numero de items do subvetor em que se fazer o mergesort
int offset; //Numero de itens que nao estao numa metade completa do vetor
int finished_threads; //Numero de threads finalizadas (util para saber quando fazer merge das metades ordenadas)
vector<Row> current_arr; //Vetor sendo ordenado

sem_t full; //Semaforo para identificar se as threads ja ordenaram cada metade do vetor
sem_t mutex; //Semaforo para identificar regiao critica (atualizar variavel finished_threads)

vector<vector<double>> result_tests_days; // Resultados para os vetores dos precos diarios ordenados por hora
vector<double> result_tests_month; // Resultados para os vetores dos precos durante um mes ordenados por dia
