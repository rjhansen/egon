/*
Copyright 2025, Robert J. Hansen <rjh@sixdemonbag.org>

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#ifndef EGON_FETCH_H
#define EGON_FETCH_H

#include <stdexcept>
#include <string>

class egon_error : public std::runtime_error {
public:
  egon_error() noexcept : std::runtime_error("") {}
  explicit egon_error(const char *what) noexcept : std::runtime_error(what) {}
  explicit egon_error(const std::string &what) noexcept
      : std::runtime_error(what.c_str()) {}
  ~egon_error() noexcept override {}
};

void fetch_key_for(const char *addr);

#endif // EGON_FETCH_H
