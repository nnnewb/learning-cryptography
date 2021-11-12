#include <iostream>
#include <ostream>
#include <string>
#include <vector>

enum class CryptMode {
  encrypt,
  decrypt,
};

std::string caesar(int k, const std::string &input, CryptMode mode) {
  std::string output;
  for (auto c : input) {
    if (mode == CryptMode::decrypt) {
      output.push_back((c - 'A' - k) % 26 + 'a');
    } else {
      output.push_back((c - 'a' + k) % 26 + 'A');
    }
  }
  return output;
}

std::vector<std::string> brute_force_caesar(std::string input) {
  std::vector<std::string> result;
  for (int k = 1; k <= 25; k++) {
    result.push_back(caesar(k, input, CryptMode::decrypt));
  }
  return result;
}

int main(void) {
  std::cout << "ciphertext:" << caesar(3, "helloworld", CryptMode::encrypt) << std::endl;
  for (auto p : brute_force_caesar(caesar(3, "helloworld", CryptMode::encrypt))) {
    std::cout << "bruteforce:" << p << std::endl;
  }
  return 0;
}
