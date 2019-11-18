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
#include <deque>
#include <limits>
#include <random>
#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <string_view>

struct Choice {
  std::string text;
  bool value {false};
};

using Choices = std::vector<Choice>;

struct Question {
  std::string text;
  Choices choices;
};

using Questions = std::vector<Question>;
using Question_ptr = Question*;
using Queue = std::deque<Question_ptr>;

std::vector<std::string_view> split(std::string_view str, std::string_view delim,
  std::size_t size = std::numeric_limits<std::size_t>::max());
std::size_t random_range(std::size_t const min, std::size_t const max);
Questions get_questions();
Queue get_queue(Questions& questions);
void ask_questions(Queue& queue);
void ask_question(Question_ptr const question);
std::size_t get_input(Question_ptr const question, std::size_t const choice_max);
bool check_answer(Question_ptr const question, std::size_t const input);

std::vector<std::string_view> split(std::string_view str, std::string_view delim, std::size_t size) {
  std::vector<std::string_view> vtok;
  std::size_t start {0};
  auto end = str.find(delim);
  while ((size-- > 0) && (end != std::string_view::npos)) {
    vtok.emplace_back(str.data() + start, end - start);
    start = end + delim.size();
    end = str.find(delim, start);
  }
  vtok.emplace_back(str.data() + start, str.size() - start);
  return vtok;
}

std::size_t random_range(std::size_t const min, std::size_t const max) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<std::size_t> distr(min, max);
  return distr(gen);
}

template<typename T>
void random_shuffle(T& obj) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::shuffle(obj.begin(), obj.end(), gen);
}

Questions get_questions() {
  Questions questions;
  questions.emplace_back(Question{"The questions text here...", {{"A", true}, {"4"}, {"8"}, {"32"}}});
  questions.emplace_back(Question{"Another questions text here...", {{"2"}, {"A", true}, {"8"}, {"32"}}});
  questions.emplace_back(Question{"A different questions text here...", {{"2"}, {"4"}, {"A", true}, {"32"}}});
  questions.emplace_back(Question{"A tricky questions text here...", {{"2"}, {"4"}, {"8"}, {"A", true}}});
  return questions;
}

Queue get_queue(Questions& questions) {
  Queue queue;
  for (auto& question : questions) {
    queue.emplace_back(&question);
  }
  return queue;
}

void ask_questions(Queue& queue) {
  while (queue.size()) {
    for (auto question_iter = queue.begin(); question_iter != queue.end();) {
      auto const question = *question_iter;
      ask_question(question);
      std::size_t tries {0};
      std::size_t choice {0};
      do {
        ++tries;
        choice = get_input(question, question->choices.size());
        if (!choice) {
          throw std::runtime_error("unexpected EOF");
        }
      } while (!check_answer(question, choice));
      if (tries == 1) {
        question_iter = queue.erase(question_iter);
      }
      else {
        queue.emplace_back(*question_iter);
        question_iter = queue.erase(question_iter);
      }
    }
  }
}

void ask_question(Question_ptr const question) {
  std::cout << question->text << "\n";
  random_shuffle(question->choices);
  std::size_t num {0};
  for (auto const& choice : question->choices) {
    std::cout << ++num << ") " << choice.text << "\n";
  }
  std::cout << "\n";
}

std::size_t get_input(Question_ptr const question, std::size_t const choice_max) {
  std::size_t choice {0};
  std::string const prompt {"> "};
  std::cout << prompt;
  std::string line;
  while (std::getline(std::cin, line)) {
    if (line == "/q") {
      std::cout << question->text << "\n" << prompt;
      continue;
    }
    if (line == "/c") {
      std::size_t num {0};
      for (auto const& choice : question->choices) {
        std::cout << ++num << ") " << choice.text << "\n";
      }
      std::cout << prompt;
      continue;
    }
    try {
      try {choice = std::stoul(std::string(line));}
      catch (...) {throw std::runtime_error("Invalid choice, select from range 1-" + std::to_string(choice_max));}
      if (choice < 1 || choice > choice_max) {
        throw std::runtime_error("Choice '" + std::to_string(choice) + "' is out of range");
      }
      break;
    }
    catch (std::exception const& e) {
      std::cout << "Error: " << e.what() << "\n" << prompt;
      choice = 0;
    }
  }
  return choice;
}

bool check_answer(Question_ptr const question, std::size_t const choice) {
  bool result {question->choices.at(choice - 1).value};
  if (result) {
    std::cout << "Correct! The answer is choice '" << choice << "':\n" << question->choices.at(choice - 1).text << "\n\n";
  }
  else {
    std::cout << "Choice '" << choice << "' is incorrect, please try again.\n";
  }
  return result;
}

int main() {
  try {
    auto questions = get_questions();
    auto queue = get_queue(questions);
    random_shuffle(queue);
    ask_questions(queue);
  }
  catch (std::exception const& e) {
    std::cout << "\nError: " << e.what() << "\n";
  }
  return 0;
}
