#include <semaphore.h>
#include <vector>
#include <string>
#include <sys/time.h>
#include <unordered_map>
#include <pthread.h>
#include <fstream>
#include <iostream>
#include <sstream> // stringstream
#include <sys/types.h> // Para leitura de arquivos
#include <dirent.h> // Para leitura de arquivos

#define NUM_MRG_THREADS 2

using namespace std;

typedef struct{
string day;
string hour;
float price;
} Row;

/* define derived values from the variables */
int numbers_per_thread;
int offset;
int finished_threads;
vector<Row> current_arr;

sem_t full;
//sem_t empty;
sem_t mutex;
