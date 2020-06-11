// #include <string>
// #include <fstream>
// #include <iostream>
// #include <vector>
// #include <sstream> // stringstream
// #include <sys/types.h> // Para leitura de arquivos
// #include <dirent.h> // Para leitura de arquivos

// using namespace std;
//
// typedef struct{
// string day;
// string hour;
// float price;
// } Row;

/* Codigo adaptado de https://www.geeksforgeeks.org/csv-file-management-using-c/ */

vector<Row> read_csv(string filename){

    vector<Row> result;
    Row row;
    // File pointer
    fstream fin(filename);

    // Read the Data from the file
    // as String Vector
    vector<string> aux;
    string line, word, temp;

    // read an entire row and
    // store it in a string variable 'line'
    while (getline(fin, line)) {

        aux.clear();

        // used for breaking words
        stringstream s(line);

        // read every column data of a row and
        // store it in a string variable, 'word'
        while (getline(s, word, ',')) {
            // add all the column data
            // of a row to a vector
            aux.push_back(word);
        }

        row.day = aux[0];
        row.hour = aux[1];
        row.price = stof(aux[2]);

        result.push_back(row);
}
    return result;
}

void write_csv(string filename, vector<Row> vec){
  ofstream outfile;
  outfile.open(filename);

  for (vector<Row>::const_iterator i = vec.begin(); i != vec.end(); i++)
    outfile << (*i).day << ',' << (*i).hour << ',' << (*i).price << endl;

}

vector<string> read_days()
{
  vector<string> days;
  DIR* dirp = opendir("preprocessed_data/");
  struct dirent * dp;
  while ((dp = readdir(dirp)) != NULL) {
    if (dp->d_name[0] != '.')
    {
      string aux = dp->d_name;
      aux.resize(10); // Retirar .csv dos nomes
      days.push_back(aux);
    }
  }
  closedir(dirp);
  return(days);
}
