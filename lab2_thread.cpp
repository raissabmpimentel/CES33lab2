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
      // Ler dados de todos os dias e inserir no mapa
      for (auto it = days.begin(); it != days.end(); it++)
      {
        vector<Row> aux = read_csv("preprocessed_data/"+ *it + ".csv");
        all_prices.insert(pair<string, vector<Row>>(*it,aux));
      }
      // Ordenar o mapa por horas utilizando threads
      sorting_hours_thread(all_prices, arr_days);
      // Ordenar o vetor de precos diarios utilizando threads
      sorting_days(arr_days, i);

    }

    // Escrever vetores ordenados em um arquivo de output
    write_output_days(all_prices, "thread");
    write_output_month(arr_days, "thread");

    // Escrever resultados dos testes
    write_results_days("tests_thread/results_days.csv", all_prices);
    write_results_month("tests_thread/results_month.csv", arr_days);

    return 0;
}
