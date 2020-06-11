#include "mergesort_hour.cpp"

void merge_hour(vector<Row> &arr, int l, int m, int r);

void* merge_sort_union_hour(void * arg);

void merge_sort_hour(vector<Row> &arr, int left, int right);

void *thread_merge_sort_hour(void* arg);

void merge_sort_total_hour(vector<Row> &arr);

void test_array_is_in_order_hour(unordered_map<string, vector<Row>> &mp);

void sorting_hours_thread(unordered_map<string, vector<Row>> &mp, vector<Row> &arr_days);

void sorting_hours_seq(unordered_map<string, vector<Row>> &mp, vector<Row> &arr_days);

void write_output_days(unordered_map<string, vector<Row>> &mp, string type);
