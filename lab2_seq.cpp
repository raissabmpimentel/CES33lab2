#include "settings.h"
#include "csv_handling.h"
#include "mergesort_hour.h"
#include "mergesort_day.h"

/*void printVector(vector<Row> v){
  int len = v.size();
  cout << "current_arr: {";
  for(int i = 0; i < len; ++i){
    cout << "(" << v[i].day << " , " << v[i].hour <<  " , " << v[i].price << "), ";
  }
  cout << "}" << endl;
}*/

int main() {

    unordered_map<string, vector<Row>> all_prices; // Mapa para precos (string que indexa)
    vector<Row> arr_days;

    // Mapear os dias que tem arquivos
    vector<string> days;
    days = read_days();

    /*int len = days.size();
    for(int i = 0; i < len; ++i){
      cout << days[i] << " ";
    }
    cout << endl;*/
    for(int i = 0; i < NUM_TESTS; i++)
    {
      // Ler dados de todos os dias
      for (auto it = days.begin(); it != days.end(); it++)
      {
        vector<Row> aux = read_csv("preprocessed_data/"+ *it + ".csv");
        // vector<Row> arr = read_csv("teste.csv");
        // all_prices.insert(pair<string, vector<Row>>("2020-05-04",arr));
        all_prices.insert(pair<string, vector<Row>>(*it,aux));
      }

      //merge_sort_total(arr);

      //current_arr = {Row{"2020-05-04","12:39",50.66}, Row{"2020-05-04","13:58",50.35}, Row{"2020-05-04","12:03",49.94}, Row{"2020-05-04","16:22",51.47}};
      // printVector(current_arr);
      // merge_hour(current_arr, 0, 1, 3);
      // printVector(current_arr);

      sorting_hours_seq(all_prices, arr_days);

      //test_array_is_in_order_hour(all_prices);

      clock_t t_ini, t_fim;
      double time_taken;
      t_ini = clock();
      merge_sort_day(arr_days, 0, arr_days.size() - 1);
      t_fim = clock();
      time_taken = (t_fim - t_ini) / (double) CLOCKS_PER_SEC;
      result_tests_month.push_back(time_taken);

      //test_array_is_in_order_day(arr_days);
    }

    write_output_days(all_prices, "seq");

    write_output_month(arr_days, "seq");

    write_results_days("tests_seq/results_days.csv", all_prices);
    write_results_month("tests_seq/results_month.csv", arr_days);
    //sorting_month(arr_days);

    // Selecionar data para escrever como saida de teste (Apagar depois)
    //write_csv("result.csv", arr);

    return 0;
}
