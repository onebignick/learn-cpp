#include <fstream>
#include <string>
#include <iostream>

int main(int argc, char** argv) {

  std::string filename = argv[argc-1];

  std::ifstream infile(filename);
  std::string line;

  long long rs = 0;
  while (std::getline(infile, line)) {
    rs += line.size();
    rs++;
  }
  std::cout << rs << std::endl;

  infile.close();
};
