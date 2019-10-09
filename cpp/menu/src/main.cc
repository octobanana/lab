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

#include "ob/term.hh"

#include <cstddef>
#include <cstdint>

#include <chrono>
#include <thread>
#include <vector>
#include <string>
#include <iostream>

namespace aec = OB::Term::ANSI_Escape_Codes;

std::size_t menu(std::vector<std::string> const& values);

std::size_t menu(std::vector<std::string> const& values)
{
  if (values.empty())
  {
    return std::numeric_limits<std::size_t>::max();
  }

  bool quit {false};
  bool select {false};
  char32_t ch {0};
  std::size_t idx {0};
  std::size_t width {0};
  std::size_t height {0};
  OB::Term::Mode term_mode;

  auto const render = [&]()
  {
    std::cout
    << aec::screen_clear
    << aec::cursor_home;

    for (std::size_t i = 0; i < values.size(); ++i)
    {
      if (i == idx)
      {
        std::cout
        << aec::reverse
        << values.at(i).substr(0, width)
        << aec::clear;
      }
      else
      {
        std::cout
        << values.at(i).substr(0, width);
      }

      if (i + 1 != values.size())
      {
        std::cout
        << "\n";
      }
    }

    std::cout
    << std::flush;
  };

  std::cout
  << aec::cursor_hide
  << aec::screen_push
  << aec::cursor_hide
  << aec::screen_clear
  << aec::cursor_home
  << aec::mouse_enable
  << std::flush;

  OB::Term::size(width, height);
  term_mode.set_raw();
  render();

  for (;;)
  {
    while ((ch = OB::Term::get_key()) > 0)
    {
      switch (ch)
      {
        case 'q':
        case OB::Term::Key::escape:
        case OB::Term::ctrl_key('c'):
        {
          quit = true;
          break;
        }

        case OB::Term::Key::newline:
        {
          quit = true;
          select = true;
          break;
        }

        case OB::Term::Key::up:
        case OB::Term::Mouse::scroll_up:
        {
          if (idx > 0)
          {
            --idx;
          }
          else
          {
            idx = values.size() - 1;
          }
          render();
          break;
        }

        case OB::Term::Key::down:
        case OB::Term::Mouse::scroll_down:
        {
          if (idx + 1 < values.size())
          {
            ++idx;
          }
          else
          {
            idx = 0;
          }
          render();
          break;
        }

        default:
        {
          break;
        }
      }
    }

    if (quit)
    {
      break;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(20));
  }

  term_mode.set_cooked();

  std::cout
  << aec::mouse_disable
  << aec::nl
  << aec::screen_pop
  << aec::cursor_show
  << std::flush;

  return select ? idx : std::numeric_limits<std::size_t>::max();
}

int main(int argc, char** argv)
{
  std::vector<std::string> values {
    "one",
    "two",
    "three",
    "four",
    "five",
    "six",
    "seven",
    "eight",
    "nine",
    "ten",
  };

  auto const selection = menu(values);

  if (selection != std::numeric_limits<std::size_t>::max())
  {
    std::cout
    << "selected: "
    << values.at(selection)
    << "\n"
    << std::flush;
  }

  return 0;
}
