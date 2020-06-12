#include "csv_handling.cpp"

vector<Row> read_csv(string filename);

void write_csv(string filename, vector<Row> vec);

vector<string> read_days();

void write_results(string filename, vector<vector<double>> vec, unordered_map<string, vector<Row>> mp);
