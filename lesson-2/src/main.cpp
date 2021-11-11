#include <algorithm>
#include <array>
#include <cctype>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <ostream>
#include <set>
#include <string>
#include <unordered_set>
#include <utility>

constexpr const char *lowercase = "abcdefghijklmnopqrstuvwxyz";

// 保持顺序的情况下，对输入文本去重，并且从文本里把字符j替换成i
std::string my_unique(const std::string &text) {
  std::string ret = text;
  std::replace(ret.begin(), ret.end(), 'j', 'i');
  std::unordered_set<char> s;
  auto last = std::stable_partition(ret.begin(), ret.end(), [&s](int n) {
    bool ret = !s.count(n); // not exists
    s.insert(n);
    return ret;
  });
  ret.erase(last, ret.end());
  return ret;
}

// 从密钥字符串构造出 playfair 密钥矩阵
std::array<std::string, 5> playfair_matrix(const std::string &key) {
  // 初始化密钥，构造的密钥中没有 j，加密时 j 视作 i 处理
  std::string fullkey = my_unique(key + lowercase);
  if (fullkey.length() % 5) {
    std::cerr << "invalid key length" << std::endl;
    exit(1);
  }

  // 构造矩阵
  std::array<std::string, 5> matrix;
  int count = 0;
  for (auto c : fullkey) {
    matrix[count / 5].push_back(c);
    ++count;
  }
  return matrix;
}

// 用迭代器读取一组两个字符（从当前位置开始，*iter 和 *(iter+1) 为一组）。
// 如果后续两个字符重复，则取一个字符加上 x 返回；
// 如果后续仅剩一个字符也加上 x 返回。
std::array<char, 2> next2(std::string::const_iterator &it, std::string::const_iterator end) {
  std::array<char, 2> ret;
  if (it + 1 == end || *it == *(it + 1)) {
    ret[0] = tolower(*(it++));
    ret[1] = 'x';
  } else {
    ret[0] = tolower(*(it++));
    ret[1] = tolower(*(it++));
  }
  return ret;
}

// 获得字符在密钥矩阵中的坐标，返回 (行,列)
std::pair<int, int> get_row_col(std::array<std::string, 5> matrix, char c) {
  for (size_t r = 0; r < matrix.size(); ++r) {
    const auto &row = matrix[r];
    auto col = row.find(c);
    if (col != row.npos) {
      return std::make_pair(r, col);
    }
  }
  return std::make_pair(-1, -1);
}

// playfair 加密函数
std::string playfair_cipher(const std::string &input, const std::string &key) {
  std::string ciphertext;
  auto matrix = playfair_matrix(key);
  auto it = input.cbegin();
  while (it != input.cend()) {
    auto char_pair = next2(it, input.cend());
    auto c1_pos = get_row_col(matrix, char_pair[0] == 'j' ? 'i' : char_pair[0]);
    auto c2_pos = get_row_col(matrix, char_pair[1] == 'j' ? 'i' : char_pair[1]);

    if (c1_pos.first == c2_pos.first) {
      // 同一行，取同行下一个字符
      auto row = c1_pos.first;
      auto c1_col = c1_pos.second + 1 >= 5 ? 0 : c1_pos.second + 1;
      auto c2_col = c2_pos.second + 1 >= 5 ? 0 : c2_pos.second + 1;

      ciphertext.push_back(matrix[row][c1_col]);
      ciphertext.push_back(matrix[row][c2_col]);
    } else if (c1_pos.second == c2_pos.second) {
      // 同一列，取同列下一个字符
      auto col = c1_pos.second;
      auto c1_row = c1_pos.first + 1 >= 5 ? 0 : c1_pos.first + 1;
      auto c2_row = c2_pos.first + 1 >= 5 ? 0 : c2_pos.first + 1;

      ciphertext.push_back(matrix[c1_row][col]);
      ciphertext.push_back(matrix[c2_row][col]);
    } else {
      // 不同行也不同列，取本行，另一字符所在列的字符
      auto row = c1_pos.first;
      auto c1_col = c2_pos.second;
      auto c2_col = c1_pos.second;

      ciphertext.push_back(matrix[row][c1_col]);
      ciphertext.push_back(matrix[row][c2_col]);
    }
  }
  return ciphertext;
}

int main(void) {
  for (const auto &row : playfair_matrix("haoye")) {
    std::cout << row << std::endl;
  }

  std::cout << "ciphertext:" << playfair_cipher("helloworld", "haoye") << std::endl;
  return 0;
}
