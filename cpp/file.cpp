#include <fstream>

#ifdef __EMSCRIPTEN__
EM_JS(void, persistWebResults, (), {
  try { localStorage.setItem('tetris-results', FS.readFile('wyniki.txt', { encoding: 'utf8' })); }
  catch (error) { console.warn('Could not save results:', error); }
});
#endif

using namespace std;

namespace File {
  void saveResultFile(char resultNames[6][64], int resultPoints[6], char name[64], int points) {
    fstream file;
    file.open("wyniki.txt", ios::out);
    if(file.good()) {
      int addedResult = false;
      int counter = 1;
      for (int slot = 0; slot < 5; ++slot) {
        if(resultPoints[counter] < points && addedResult == false) {
          addedResult = true;
          file<<name<<endl;
          file<<points<<endl;
        } else {
          file<<resultNames[counter]<<endl;
          file<<resultPoints[counter]<<endl;
          counter++;
        }
      }
      file.close();
#ifdef __EMSCRIPTEN__
      persistWebResults();
#endif
    }
  }

  void saveResultFile(char resultNames[6][64], int resultPoints[6]) {
    fstream file;
    file.open("wyniki.txt", ios::out);
    if(file.good()) {
      int counter = 0;
      while(++counter < 6) {
        file<<resultNames[counter]<<endl;
        file<<resultPoints[counter]<<endl;
      }
      file.close();
    }
  }

  void readResultsFile(char resultNames[6][64], int resultPoints[6], char resultCPoint[6][64]) {
    fstream file;
    file.open("wyniki.txt", ios::in);
    if(file.good()) {
      string line;
      int counter = 0;
      while(!file.eof() && ++counter < 6) {
        getline(file, line);
        snprintf(resultNames[counter], 12, "%s", line.c_str());
        if(resultNames[counter][0] == 0) {
          sprintf(resultNames[counter], "Anonim");
        }
        getline(file, line);
        snprintf(resultCPoint[counter], 12, "%s", line.c_str());
        resultPoints[counter] = atoi(resultCPoint[counter]);
      }
      file.close();
    } else {
      saveResultFile(resultNames, resultPoints);
      readResultsFile(resultNames, resultPoints, resultCPoint);
    }
  }
}
