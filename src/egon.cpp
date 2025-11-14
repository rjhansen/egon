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

#include "egon.h"
#include <print>
#include <regex>

namespace {
const std::regex email_rx(R"(^[a-zA-Z0-9_\.]+@((protonmail\.com)|(proton\.me))$)");
}

int main(int argc, char* argv[])
{
    int exit_code = EXIT_SUCCESS;
    try {
        for (int i = 1; i < argc; ++i) {
            if (!std::regex_match(argv[i], email_rx))
                continue;
            fetch_key_for(argv[i]);
        }
    } catch (const egon_error& e) {
        std::println(stderr, "{}", e.what());
        exit_code = EXIT_FAILURE;
    }
    return exit_code;
}