// Funcao para executar merge com base no dia do preco da acao
void merge_day(vector<Row> &arr, int l, int m, int r) {
    int i1 = l, i2 = m + 1, i3 = 0;

    vector<Row> aux(r - l + 1);

    while(i1 <= m and i2 <= r){
        if(arr[i1].day < arr[i2].day){
            aux[i3++] = arr[i1++];
        }
        else {
            aux[i3++] = arr[i2++];
        }
    }

    while(i1 <= m){
        aux[i3++] = arr[i1++];
    }

    while(i2 <= r){
        aux[i3++] = arr[i2++];
    }

    for(int i = l, j = 0; i <= r; ++i, ++j){
        arr[i] = aux[j];
    }
}

// Funcao para realizar o merge das duas metades ordenadas do vetor
void* merge_sort_union_day(void * arg) {
    sem_wait(&full); // Aguardar ate que as duas threads tenham terminado de executar
    int length = current_arr.size();

    // Determinar limites do vetor a dar merge
    int left = 0;
    int right = (2* items_per_thread) - 1;
    int middle = left + (items_per_thread) - 1;
    if (right >= length) {
        right = length - 1;
    }
    merge_day(current_arr, left, middle, right);
}

// Relizar merge sort comum
void merge_sort_day(vector<Row> &arr, int left, int right) {
    if (left < right) {
        int middle = left + (right - left) / 2;
        merge_sort_day(arr, left, middle);
        merge_sort_day(arr, middle + 1, right);
        merge_day(arr, left, middle, right);
    }
}

// Realizar merge sort de cada metade do vetor em uma thread
void *thread_merge_sort_day(void* arg)
{
    // Calculo dos indices da parte do vetor a ser ordenado pela thread
    int thread_id = (long) arg;
    int left = thread_id * (items_per_thread);
    int right = (thread_id + 1) * (items_per_thread) - 1;
    if (thread_id == NUM_MRG_THREADS - 1) {
        right += offset; // Offset sobre uma parte do vetor nao analisado
    }
    int middle = left + (right - left) / 2;

    // Realizar merge sort de uma metade do vetor
    if (left < right) {
        merge_sort_day(current_arr, left, right);
        merge_sort_day(current_arr, left + 1, right);
        merge_day(current_arr, left, middle, right);
    }

    // Entrar em regiao critica
    sem_wait(&mutex);
    finished_threads++; //Aumentar numero de threads finalizadas
    //Caso todas as threads estejam finalizadas, liberar para o merge_sort_union_day executar
    if(finished_threads == NUM_MRG_THREADS)
      sem_post(&full);
    sem_post(&mutex); // Sair da regiao critica
    return NULL;
}

// Ordenar por data o vetor dos precos de um mes utilizando threads
void merge_sort_total_day(vector<Row> &arr)
{
  current_arr = arr; // Variavel global com o vetor sendo analisado

  // Calculo de numero de items do subvetor em que se fazer o mergesort e tambem do offset (numero de itens que nao estao numa metade completa do vetor)
  int length = current_arr.size();
  items_per_thread = length / NUM_MRG_THREADS;
  offset = length % NUM_MRG_THREADS;

  // Vetor de threads
  pthread_t threads[NUM_MRG_THREADS + 1];

  // Criar threads para executar merge_sort de metade do vetor
  for (long i = 0; i < NUM_MRG_THREADS; i ++) {
      int rc = pthread_create(&threads[i], NULL, thread_merge_sort_day, (void *)i);
      if (rc){
          cout << "ERRO; Retorno de pthread_create() é " << rc << endl;
          exit(-1);
      }
  }

  // Criar thread para executar o merge das duas metades do vetor
  int rc = pthread_create(&threads[NUM_MRG_THREADS], NULL, merge_sort_union_day, (void *)NULL);
  if (rc){
      cout << "ERRO; Retorno de pthread_create() é " << rc << endl;
      exit(-1);
  }

  // Aguardar termino de execucao das threads
  for(long i = 0; i < NUM_MRG_THREADS + 1; i++) {
      pthread_join(threads[i], NULL);
  }

  // O novo vetor passa a ser o ordenado
  arr = current_arr;
}

// Chamar a funcao merge_sort_total_day, inicializar os semaforos e calcular tempo de execucao
void sorting_days(vector<Row> &arr, int n_test)
{
    // Inicializar semaforos e numero de threads finalizadas
    finished_threads = 0;
    sem_init(&mutex, 0, 1);
    sem_init(&full, 0, 0);

    // Chamar a funcao merge_sort_total_day e calcular o tempo de execucao
    clock_t t_ini, t_fim;
    double time_taken;
    t_ini = clock();
    merge_sort_total_day(arr);
    t_fim = clock();
    time_taken = (t_fim - t_ini) / (double) CLOCKS_PER_SEC;
    result_tests_month.push_back(time_taken); // Salvar resultado
}

// Escrever output com o vetor dos precos de um mes ordenado por data
void write_output_month(vector<Row> &arr, string type)
{
    string aux = arr[0].day;
    aux.resize(7);
    write_csv("output_" + type + "/month/" + aux + ".csv", arr);
}
