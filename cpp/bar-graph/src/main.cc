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
#include <fstream>
#include <iostream>

int main()
{
  std::cout << "Daily Sales (each * = $1000)\n";

  // the store number
  std::size_t store {1};

  // string buffer for each line of the input file
  std::string line;

  // init and open the input file
  std::ifstream ifile {"data.txt"};

  // read each line of the input file into the string buffer
  // the while loop will break when the file returns EOF
  while (std::getline(ifile, line))
  {
    // output the store number and then increase the value by one

    // use std::stoul to convert the number contained in the string buffer
    // to an unsigned long int

    // divide the number by 1000 to get the number of stars to display

    // use the std::string constructor that initializes a string that takes
    // two arguments, the first is the size of the string, and the second
    // argument is the fill character

    std::cout
      << "Store " << store++ << ": "
      << std::string(std::stoul(line) / 1000, '*')
      << "\n";
  }

  return 0;
}
