/*
                                    88888888
                                  888888888888
                                 88888888888888
                                8888888888888888
                               888888888888888888
                              888888  8888  888888
                              88888    88    88888
                              888888  8888  888888
                              88888888888888888888
                              88888888888888888888
                             8888888888888888888888
                          8888888888888888888888888888
                        88888888888888888888888888888888
                              88888888888888888888
                            888888888888888888888888
                           888888  8888888888  888888
                           888     8888  8888     888
                                   888    888

                                   OCTOBANANA

Licensed under the MIT License

Copyright (c) 2019 Brett Robinson <https://octobanana.com/>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <cstddef>
#include <string>
#include <iostream>

std::string make_diamond(std::size_t const size) {
  std::string diamond;

  std::string const s1 {"/"};
  std::string const s2 {"\\"};
  std::string const fill {"."};

  std::size_t const midpoint {size / 2};
  std::size_t p1 {midpoint - 1};
  std::size_t p2 {0};

  // Top half
  // .../  \...  Q1  Q2
  // ../.  .\..  Q1  Q2
  // ./..  ..\.  Q1  Q2
  // /...  ...\  Q1  Q2

  // Bottom half
  // \...  .../  Q3  Q4
  // .\..  ../.  Q3  Q4
  // ..\.  ./..  Q3  Q4
  // ...\  /...  Q3  Q4

  // Top half
  for (std::size_t i = 0; i < midpoint; ++i) {
    // Q1
    for (std::size_t j = 0; j < midpoint; ++j) {
      diamond += (j == p1 ? s1 : fill);
    }
    --p1;

    // Q2
    for (std::size_t j = 0; j < midpoint; ++j) {
      diamond += (j == p2 ? s2 : fill);
    }
    ++p2;

    diamond += "\n";
  }

  // Bottom half
  for (std::size_t i = 0; i < midpoint; ++i) {
    // Q3
    ++p1;
    for (std::size_t j = 0; j < midpoint; ++j) {
      diamond += (j == p1 ? s2 : fill);
    }

    // Q4
    --p2;
    for (std::size_t j = 0; j < midpoint; ++j) {
      diamond += (j == p2 ? s1 : fill);
    }

    diamond += "\n";
  }

  return diamond;
}

int main(int argc, char** argv) {
  std::size_t size {0};
  std::cout << "size: ";
  std::cin >> size;

  std::size_t repeat {0};
  std::cout << "repeat: ";
  std::cin >> repeat;

  auto diamond = make_diamond(size);

  for (std::size_t i = 0; i < repeat; ++i) {
    std::cout << diamond;
  }

  return 0;
}
