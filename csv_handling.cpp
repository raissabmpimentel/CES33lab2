/* Codigo adaptado de https://www.geeksforgeeks.org/csv-file-management-using-c/
   para ler conteudo de um arquivo csv */

vector<Row> read_csv(string filename){

    vector<Row> result;
    Row row;
    fstream fin(filename);

    vector<string> aux; // Para guardar conteudo da linha
    string line, word; // line guarda string da linha e word guarda cada palavra da linha

    // Ler uma linha e salvar em uma string chamada 'line'
    while (getline(fin, line)) {
        aux.clear();
        // Usado para quebrar linhas
        stringstream s(line);
        while (getline(s, word, ',')) { // Ler cada palavra da linha separado por virgulas e salvar em aux
            aux.push_back(word);
        }

        // Salvar linha conforme struct
        row.day = aux[0];
        row.hour = aux[1];
        row.price = stof(aux[2]);

        result.push_back(row); // Colocar linha no vetor de resultado
}
    return result;
}

// Escrever o vetor de structs em arquivo csv
void write_csv(string filename, vector<Row> vec){
  ofstream outfile;
  outfile.open(filename);

  for (auto i = vec.begin(); i != vec.end(); i++)
    outfile << (*i).day << ',' << (*i).hour << ',' << (*i).price << endl;

}

// Ler strings dos dias analisados, que estao na pasta "preprocessed_data/"
vector<string> read_days()
{
  vector<string> days;
  DIR* dirp = opendir("preprocessed_data/");
  struct dirent * dp;
  while ((dp = readdir(dirp)) != NULL) {
    if (dp->d_name[0] != '.') // Ignorar arquivos ocultos do Linux
    {
      string aux = dp->d_name;
      aux.resize(10); // Retirar .csv dos nomes
      days.push_back(aux);
    }
  }
  closedir(dirp);
  return(days);
}

// Escrever resultados dos testes para os vetores de precos de um unico dia em formato csv
void write_results_days(string filename, unordered_map<string, vector<Row>> mp) {
  ofstream outfile;
  outfile.open(filename);

  // Cabecalho com os nomes dos meses dias em ordem que foram ordenados
  outfile << "test number,";
  for(auto it = mp.begin(); it != mp.end(); it++)
  {
    if(it != mp.begin())
      outfile << ',';
    outfile << it->first;
  }
  outfile << endl;

  // Imprimir resultados dos testes para os vetores de precos de um unico dia
  for (int i = 0; i < result_tests_days.size(); i++)
  {
    outfile << i+1 << ',';
    for(auto j = result_tests_days[i].begin(); j != result_tests_days[i].end(); j++)
    {
      outfile << (*j);
      if (j + 1 != result_tests_days[i].end())
        outfile << ',';
    }
    outfile << endl;
  }
}

// Escrever resultados dos testes para os vetores de precos de um mes em formato csv
void write_results_month(string filename, vector<Row> arr) {
  ofstream outfile;
  outfile.open(filename);

  // Imprimir cabecalho
  string aux = arr[0].day;
  aux.resize(7); // Retirar dia da string para deixar so o mes
  outfile << "test number," << aux << endl;

  // Imprimir resultados dos testes para os vetores de precos de um mes
  for (int i = 0; i < result_tests_month.size(); i++)
    outfile << i+1 << ',' << result_tests_month[i] << endl;
}
