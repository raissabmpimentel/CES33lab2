// #include <unordered_map>
// #include <pthread.h>

// #define NUM_MRG_THREADS 2
//
// /* define derived values from the variables */
// int numbers_per_thread;
// int offset;
// int finished_threads;
// vector<Row> current_arr;
//
// sem_t full;
// sem_t empty;
// sem_t mutex;

/* merge function */
void merge_hour(vector<Row> &arr, int l, int m, int r) {
    //cout << "merge_hour \n";
    //printVector(arr);
    int i1 = l, i2 = m + 1, i3 = 0;

    vector<Row> aux(r - l + 1);

    while(i1 <= m and i2 <= r){
        if(arr[i1].hour < arr[i2].hour){
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
    //printVector(arr);
}
/* merge locally sorted sections */
void* merge_sort_union_hour(void * arg) {
    //cout << "merge_sort_union \n";
    sem_wait(&full);
    //printVector(current_arr);
    int length = current_arr.size();
    for(int i = 0; i < NUM_MRG_THREADS; i = i + 2) {
        /*int left = i * (numbers_per_thread * aggregation);
        int right = ((i + 2) * numbers_per_thread * aggregation) - 1;
        int middle = left + (numbers_per_thread * aggregation) - 1;*/
        int left = i * (numbers_per_thread);
        int right = ((i + 2) * numbers_per_thread) - 1;
        int middle = left + (numbers_per_thread) - 1;
        if (right >= length) {
            right = length - 1;
        }
        merge_hour(current_arr, left, middle, right);
    }
    /*if (number / 2 >= 1) {
        merge_sort_union(arr, number / 2, aggregation * 2);
    }*/
    //printVector(current_arr);
}

/* perform merge sort */
void merge_sort_hour(vector<Row> &arr, int left, int right) {
    //cout << "merge_sort \n";
    if (left < right) {
        int middle = left + (right - left) / 2;
        merge_sort_hour(arr, left, middle);
        merge_sort_hour(arr, middle + 1, right);
        merge_hour(arr, left, middle, right);
    }
}

/** assigns work to each thread to perform merge sort */
void *thread_merge_sort_hour(void* arg)
{
    //cout << "thread_merge_sort \n";
    //sem_wait(&empty);
    //printVector(current_arr);
    int thread_id = (long) arg;
    int left = thread_id * (numbers_per_thread);
    int right = (thread_id + 1) * (numbers_per_thread) - 1;
    if (thread_id == NUM_MRG_THREADS - 1) {
        right += offset;
    }
    int middle = left + (right - left) / 2;
    if (left < right) {
        merge_sort_hour(current_arr, left, right);
        merge_sort_hour(current_arr, left + 1, right);
        merge_hour(current_arr, left, middle, right);
    }
    sem_wait(&mutex);
    finished_threads++;
    if(finished_threads == 2)
      sem_post(&full);
    //printVector(current_arr);
    sem_post(&mutex);
    return NULL;
}

void merge_sort_total_hour(vector<Row> &arr)
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
      int rc = pthread_create(&threads[i], NULL, thread_merge_sort_hour, (void *)i);
      if (rc){
          printf("ERROR; return code from pthread_create() is %d\n", rc);
          exit(-1);
      }
  }

  int rc = pthread_create(&threads[NUM_MRG_THREADS], NULL, merge_sort_union_hour, (void *)NULL);
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
void test_array_is_in_order_hour(unordered_map<string, vector<Row>> &mp) {
  for(auto it = mp.begin(); it != mp.end(); it++)
  {
    vector<Row> arr = it->second;
    int length = arr.size();
    string max = "";
    for (int i = 1; i < length; i ++) {
        if (arr[i].hour >= arr[i - 1].hour) {
            max = arr[i].hour;
        } else {
            cout << "Array of day " << it->first << " is not ordered" << endl;
            return;
        }
    }
    cout << "Array of day " << it->first << " are in sorted order" << endl;
  }

}

void sorting_hours_thread(unordered_map<string, vector<Row>> &mp, vector<Row> &arr_days)
{
  clock_t t_ini, t_fim;
  //cout << "sorting days \n";
  double time_taken;
  vector<double> time_in_test;
  for(auto it = mp.begin(); it != mp.end(); it++)
  {
    finished_threads = 0;
    sem_init(&mutex, 0, 1);
    sem_init(&full, 0, 0);
    //sem_init(&empty, 0, NUM_MRG_THREADS);
    t_ini = clock();
    merge_sort_total_hour(it->second);
    t_fim = clock();
    time_taken = (t_fim - t_ini) / (double) CLOCKS_PER_SEC;
    time_in_test.push_back(time_taken);
    arr_days.push_back(it->second.back());
  }
  result_tests_days.push_back(time_in_test);
}

void sorting_hours_seq(unordered_map<string, vector<Row>> &mp, vector<Row> &arr_days)
{
  clock_t t_ini, t_fim;
  double time_taken;
  vector<double> time_in_test;
  //cout << "sorting days \n";
  for(auto it = mp.begin(); it != mp.end(); it++)
  {
    int right = it->second.size() - 1;
    t_ini = clock();
    merge_sort_hour(it->second, 0, right);
    t_fim = clock();
    time_taken = (t_fim - t_ini) / (double) CLOCKS_PER_SEC;
    time_in_test.push_back(time_taken);
    arr_days.push_back(it->second.back());
  }
  result_tests_days.push_back(time_in_test);
}

void write_output_days(unordered_map<string, vector<Row>> &mp, string type)
{
  for(auto it = mp.begin(); it != mp.end(); it++)
    write_csv("output_" + type + "/days/" + it->first + ".csv", it->second);
}
