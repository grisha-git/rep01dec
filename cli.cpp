#include <iostream>

void hi()
{
  std::cout << "< HI! >\n";
}

int main()
{
  constexpr size_t cmds_count = 1;
  void(*cmds[1])() = {hi};
  size_t i = 0;
  while (!(std::cin >> i).eof())
  {
    if (std::cin.fail())
    {
      std::cerr << "invalid\n";
      //сделать так чтобы пользователь дальше вводил числа
      return 1;
    }
    else if (i < cmds_count)
    {
      cmds[i]();
    }
    else
    {
      std::cerr << "< unknown >\n";
    }
  }
}