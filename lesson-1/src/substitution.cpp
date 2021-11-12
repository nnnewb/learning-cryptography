#include <cctype>
#include <corecrt.h>
#include <iostream>
#include <map>
#include <ostream>
#include <stdexcept>
#include <string>
#include <vector>

enum class CryptMode {
  encrypt,
  decrypt,
};

std::string substitution_cipher(const std::string &input, const std::map<char, char> &chart) {
  std::string output;
  for (char c : input) {
    try {
      output.push_back(chart.at(tolower(c)));
    } catch (std::out_of_range err) {
      return "invalid input";
    }
  }
  return output;
}

constexpr auto plaintext = "It can solve simple substitution ciphers often found in newspapers, "
                           "including puzzles like cryptoquips";

int main(void) {
  // 替代密码表
  const std::map<char, char> chart = {
      {'a', 'o'}, {'b', 'a'}, {'c', 'f'}, {'d', 'u'}, {'e', 'g'}, {'f', 'y'},   {'g', 'n'}, {'h', 'k'},
      {'i', 'e'}, {'j', 'z'}, {'k', 't'}, {'l', 'b'}, {'m', 'd'}, {'n', 'p'},   {'o', 'l'}, {'p', 'm'},
      {'q', 'j'}, {'r', 'q'}, {'s', 'c'}, {'t', 'i'}, {'u', 'w'}, {'v', 'x'},   {'w', 's'}, {'x', 'v'},
      {'y', 'r'}, {'z', 'h'}, {' ', ' '}, {',', ','}, {'.', '.'}, {'\'', '\''},
  };
  auto ciphertext = substitution_cipher(plaintext, chart);
  std::cout << "substitution cipher: " << ciphertext << std::endl;
  return 0;
}
