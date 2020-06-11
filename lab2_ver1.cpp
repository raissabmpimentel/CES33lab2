#include "csv_handling.h"
#include <unordered_map>
#include <pthread.h>
#include <sys/time.h>
#include <semaphore.h>

#define NUM_THREADS 2

/* define derived values from the variables */
int numbers_per_thread;
int offset;
vector<Row> current_arr;

//sem_t mutex;

void printVector(vector<Row> v){
  int len = v.size();
  cout << "current_arr: {";
  for(int i = 0; i < len; ++i){
    cout << "(" << v[i].hour <<  " , " << v[i].price << "), ";
  }
  cout << "}" << endl;
}


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
void merge_sort_union(vector<Row> &arr, int number, int aggregation) {
    //cout << "merge_sort_union \n";
    int length = arr.size();
    for(int i = 0; i < number; i = i + 2) {
        int left = i * (numbers_per_thread * aggregation);
        int right = ((i + 2) * numbers_per_thread * aggregation) - 1;
        int middle = left + (numbers_per_thread * aggregation) - 1;
        if (right >= length) {
            right = length - 1;
        }
        merge_hour(arr, left, middle, right);
    }
    if (number / 2 >= 1) {
        merge_sort_union(arr, number / 2, aggregation * 2);
    }
}

/* perform merge sort */
void merge_sort(vector<Row> &arr, int left, int right) {
    //cout << "merge_sort \n";
    if (left < right) {
        int middle = left + (right - left) / 2;
        merge_sort(arr, left, middle);
        merge_sort(arr, middle + 1, right);
        merge_hour(arr, left, middle, right);
    }
}

/** assigns work to each thread to perform merge sort */
void *thread_merge_sort(void* arg)
{
    //cout << "thread_merge_sort \n";
    int thread_id = (long) arg;
    int left = thread_id * (numbers_per_thread);
    int right = (thread_id + 1) * (numbers_per_thread) - 1;
    if (thread_id == NUM_THREADS - 1) {
        right += offset;
    }
    int middle = left + (right - left) / 2;
    if (left < right) {
        merge_sort(current_arr, left, right);
        merge_sort(current_arr, left + 1, right);
        merge_hour(current_arr, left, middle, right);
    }
    return NULL;
}

void merge_sort_total(vector<Row> &arr)
{
  //cout << "merge_sort_total \n";
  current_arr = arr;
  int length = current_arr.size();
  numbers_per_thread = length / NUM_THREADS;


  /* begin timing */
  pthread_t threads[NUM_THREADS];

  /* create threads */
  for (long i = 0; i < NUM_THREADS; i ++) {
      int rc = pthread_create(&threads[i], NULL, thread_merge_sort, (void *)i);
      if (rc){
          printf("ERROR; return code from pthread_create() is %d\n", rc);
          exit(-1);
      }
  }

  for(long i = 0; i < NUM_THREADS; i++) {
      pthread_join(threads[i], NULL);
  }

  merge_sort_union(current_arr, NUM_THREADS, 1);
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

void sorting_days(unordered_map<string, vector<Row>> &mp)
{
  //cout << "sorting days \n";
  for(auto it = mp.begin(); it != mp.end(); it++)
  {
    merge_sort_total(it->second);
  }
}


int main() {

    unordered_map<string, vector<Row>> all_prices; // Mapa para precos (string que indexa)

    //sem_init(&mutex, 0, 1);

    // Mapear os dias que tem arquivos
    vector<string> days;
    days = read_days();

    int len = days.size();
    for(int i = 0; i < len; ++i){
      cout << days[i] << " ";
    }
    cout << endl;

    // Ler dados de todos os dias
    for (auto i = days.begin(); i != days.end(); i++)
    {
      vector<Row> aux = read_csv("preprocessed_data/"+ *i + ".csv");
      //vector<Row> arr = read_csv("preprocessed_data/2020-05-04.csv");
      all_prices.insert(pair<string, vector<Row>>(*i,aux));
    }

    //merge_sort_total(arr);

    //current_arr = {Row{"2020-05-04","12:39",50.66}, Row{"2020-05-04","13:58",50.35}, Row{"2020-05-04","12:03",49.94}, Row{"2020-05-04","16:22",51.47}};
    // printVector(current_arr);
    // merge_hour(current_arr, 0, 1, 3);
    // printVector(current_arr);

    sorting_days(all_prices);

    test_array_is_in_order_hour(all_prices);

    // Selecionar data para escrever como saida de teste (Apagar depois)
    //write_csv("result.csv", arr);

    return 0;
}
