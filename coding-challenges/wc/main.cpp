#include <fstream>
#include <ios>
#include <iterator>
#include <string>
#include <iostream>

int main(int argc, char** argv) {
  int flag_mask = 0;
  for(int i = 0; i < argc; i++) {
    std::string str_cmp = argv[i];
    if (str_cmp == "-c") flag_mask |= 1 << 0;
    if (str_cmp == "-l") flag_mask |= 1 << 1;
    if (str_cmp == "-w") flag_mask |= 1 << 2;
    if (str_cmp == "-m") flag_mask |= 1 << 3;
  }

  // default args
  if (flag_mask == 0) flag_mask |= (1 << 3) - 1;

  std::string filename = argv[argc - 1];
  std::ifstream infile(filename, std::ios::binary);
  
  if ((flag_mask >> 0) & 1) {
    infile.seekg(0);
    int res = 0;
    std::string line;
    while (std::getline(infile, line)) {
      res++;
    }
    std::cout << res << " ";
  }

  if ((flag_mask >> 1) & 1) {
    infile.clear();
    infile.seekg(0, std::ios::beg);

    int res = 0;
    std::string word;
    while (infile >> word) {
      res++;
    }
    std::cout << res << " ";
  }

  if ((flag_mask >> 2) & 1) {
    infile.clear();
    infile.seekg(0, std::ios::end);
    int res = infile.tellg();
    std::cout << res << " ";
  }

  if ((flag_mask >> 3) & 1) {
    std::istreambuf_iterator<char> it(infile), end;
    int res = 0;

    for(; it != end; it++) {
      unsigned char c = static_cast<unsigned char>(*it);
      if ((c & 0xC0) != 0x80) res++;
    }

    std::cout << res << " ";
  }

  std::cout << filename << std::endl;
  return 0;
};
