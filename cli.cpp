#include <cctype>
#include <iostream>

void hi()
{
  std::cout << "< HI! >\n";
}
bool is_space(char c)
{
  return std::isspace(c);
}
std::istream& getword(std::istream& os, char* word, size_t k, bool(*c)(char));
size_t match(const char* word, const char* const* words, size_t k);
int main()
{
  constexpr size_t cmds_count = 1;
  void(*cmds[1])() = {hi};
  const char* const cmds_text[] = {"hi"};

  constexpr size_t bsize = 255;
  char word[bsize + 1] = {};

  while (!(getword(std::cin, word, bsize, is_space).eof()))
  {
    if (std::cin.fail())
    {
      std::cerr << "invalid\n";
      //сделать так чтобы пользователь дальше вводил числа
      return 1;
    }
    else if (size_t i = match(word, cmds_text, cmds_count); i < cmds_count)
    {
      cmds[i]();
    }
    else
    {
      std::cerr << "< unknown >\n";
    }
  }
}