#include "settings.h"
#include "csv_handling.h"
#include "mergesort_hour.h"
#include "mergesort_day.h"

int main() {

    unordered_map<string, vector<Row>> all_prices; // Mapa para precos (string da data que indexa os precos)
    vector<Row> arr_days;

    // Mapear os dias que tem arquivos
    vector<string> days;
    days = read_days();

    // Executar os testes NUM_TESTS vezes
    for(int i = 0; i < NUM_TESTS; i++)
    {
      // Limpar dados anteriores
      arr_days.clear();
      all_prices.clear();

      // Ler dados de todos os dias e inserir no mapa
      for (auto it = days.begin(); it != days.end(); it++)
      {
        vector<Row> aux = read_csv("preprocessed_data/"+ *it + ".csv");
        all_prices.insert(pair<string, vector<Row>>(*it,aux));
      }

      // Ordenar o mapa por horas sequencialmente
      sorting_hours_seq(all_prices, arr_days);

      // Ordenar por dia o vetor de precos durante um mes sequencialmente, calcular o tempo e colocar no vetor de resultdos
      clock_t t_ini, t_fim;
      double time_taken;
      t_ini = clock();
      merge_sort_day(arr_days, 0, arr_days.size() - 1);
      t_fim = clock();
      time_taken = (t_fim - t_ini) / (double) CLOCKS_PER_SEC;
      result_tests_month.push_back(time_taken);
    }

    // Escrever vetores ordenados em um arquivo de output
    write_output_days(all_prices, "seq");
    write_output_month(arr_days, "seq");

    // Escrever resultados dos testes
    write_results_days("tests_seq/results_days.csv", all_prices);
    write_results_month("tests_seq/results_month.csv", arr_days);

    return 0;
}
