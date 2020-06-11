// #include <unordered_map>
// #include <pthread.h>

/* merge function */
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

/* merge locally sorted sections */
void* merge_sort_union_day(void * arg) {
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
        merge_day(current_arr, left, middle, right);
    }
    /*if (number / 2 >= 1) {
        merge_sort_union(arr, number / 2, aggregation * 2);
    }*/
    //printVector(current_arr);
}

/* perform merge sort */
void merge_sort_day(vector<Row> &arr, int left, int right) {
    //cout << "merge_sort \n";
    if (left < right) {
        int middle = left + (right - left) / 2;
        merge_sort_day(arr, left, middle);
        merge_sort_day(arr, middle + 1, right);
        merge_day(arr, left, middle, right);
    }
}

/** assigns work to each thread to perform merge sort */
void *thread_merge_sort_day(void* arg)
{
    //cout << "thread_merge_sort \n";
    sem_wait(&empty);
    //printVector(current_arr);
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
    //printVector(current_arr);
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

void sorting_days(vector<Row> &arr)
{
  //cout << "sorting days \n";
    finished_threads = 0;
    sem_init(&mutex, 0, 1);
    sem_init(&full, 0, 0);
    sem_init(&empty, 0, NUM_MRG_THREADS);
    merge_sort_total_day(arr);
}


void write_output_month(vector<Row> &arr, string type)
{
    string aux = arr[0].day;
    aux.resize(7);
    write_csv("output_" + type + "/" + aux + ".csv", arr);
}
