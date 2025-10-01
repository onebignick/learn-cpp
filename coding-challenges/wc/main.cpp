#include <fstream>
#include <ios>
#include <iterator>
#include <string>
#include <iostream>

int main(int argc, char** argv) {
  bool find_byte = false;
  bool find_lines = false;
  bool find_words = false;
  bool find_chars = false;
  for(int i = 0; i < argc; i++) {
    if (std::string(argv[i]) == "-c") find_byte = true;
    if (std::string(argv[i]) == "-l") find_lines = true;
    if (std::string(argv[i]) == "-w") find_words = true;
    if (std::string(argv[i]) == "-m") find_chars = true;
  }

  // default args
  if (!(find_byte | find_lines | find_words | find_chars)) {
    find_byte = true;
    find_lines = true;
    find_words = true;
  }

  std::string filename = argv[argc - 1];
  std::ifstream infile(filename, std::ios::binary);
  
  if (find_lines) {
    infile.seekg(0);
    int res = 0;
    std::string line;
    while (std::getline(infile, line)) {
      res++;
    }
    std::cout << res << " ";
  }

  if (find_words) {
    infile.clear();
    infile.seekg(0, std::ios::beg);

    int res = 0;
    std::string word;
    while (infile >> word) {
      res++;
    }
    std::cout << res << " ";
  }

  if (find_byte) {
    infile.clear();
    infile.seekg(0, std::ios::end);
    int res = infile.tellg();
    std::cout << res << " ";
  }

  if (find_chars) {
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
