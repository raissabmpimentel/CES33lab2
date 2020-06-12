#include "csv_handling.cpp"

vector<Row> read_csv(string filename);

void write_csv(string filename, vector<Row> vec);

vector<string> read_days();

void write_results_days(string filename, unordered_map<string, vector<Row>> mp);

void write_results_month(string filename, vector<Row> arr);
