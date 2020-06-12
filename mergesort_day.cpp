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
    int left = i * (numbers_per_thread);
    int right = ((i + 2) * numbers_per_thread) - 1;
    int middle = left + (numbers_per_thread) - 1;
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

// Relizar
void *thread_merge_sort_day(void* arg)
{
    int thread_id = (long) arg;
    int left = thread_id * (numbers_per_thread);
    int right = (thread_id + 1) * (numbers_per_thread) - 1;
    if (thread_id == NUM_MRG_THREADS - 1) {
        right += offset;
    }
    int middle = left + (right - left) / 2;
    if (left < right) {
        merge_sort_day(current_arr, left, right);
        merge_sort_day(current_arr, left + 1, right);
        merge_day(current_arr, left, middle, right);
    }
    sem_wait(&mutex);
    finished_threads++;
    if(finished_threads == 2)
      sem_post(&full);
    sem_post(&mutex);
    return NULL;
}

void merge_sort_total_day(vector<Row> &arr)
{
  //cout << "merge_sort_total \n";
  current_arr = arr;
  //printVector(current_arr);
  int length = current_arr.size();
  numbers_per_thread = length / NUM_MRG_THREADS;
  offset = length % NUM_MRG_THREADS;

  /* begin timing */
  pthread_t threads[NUM_MRG_THREADS + 1];

  /* create threads */
  for (long i = 0; i < NUM_MRG_THREADS; i ++) {
      int rc = pthread_create(&threads[i], NULL, thread_merge_sort_day, (void *)i);
      if (rc){
          printf("ERROR; return code from pthread_create() is %d\n", rc);
          exit(-1);
      }
  }

  int rc = pthread_create(&threads[NUM_MRG_THREADS], NULL, merge_sort_union_day, (void *)NULL);
  if (rc){
      printf("ERROR; return code from pthread_create() is %d\n", rc);
      exit(-1);
  }

  for(long i = 0; i < NUM_MRG_THREADS + 1; i++) {
      pthread_join(threads[i], NULL);
  }

  //merge_sort_union(current_arr, NUM_MRG_THREADS, 1);
  //printVector(current_arr);
  arr = current_arr;

}

/* test to ensure that the array is in sorted order */
void test_array_is_in_order_day(vector<Row> &arr) {
  int length = arr.size();
  string max = "";
  for (int i = 1; i < length; i ++) {
      if (arr[i].hour >= arr[i - 1].hour) {
          max = arr[i].hour;
      } else {
          cout << "Array of month is not ordered" << endl;
          return;
      }
  }
  cout << "Array of month is in sorted order" << endl;

}

void sorting_days(vector<Row> &arr, int n_test)
{
  //cout << "sorting days \n";
    finished_threads = 0;
    sem_init(&mutex, 0, 1);
    sem_init(&full, 0, 0);
    //sem_init(&empty, 0, NUM_MRG_THREADS);
    clock_t t_ini, t_fim;
    double time_taken;
    t_ini = clock();
    merge_sort_total_day(arr);
    t_fim = clock();
    time_taken = (t_fim - t_ini) / (double) CLOCKS_PER_SEC;
    result_tests_month.push_back(time_taken);

}


void write_output_month(vector<Row> &arr, string type)
{
    string aux = arr[0].day;
    aux.resize(7);
    write_csv("output_" + type + "/month/" + aux + ".csv", arr);
}
