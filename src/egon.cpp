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

#include <curl/curl.h>
#include <gpgme.h>
#include <print>
#include <ranges>
#include <regex>
#include <string>
#include <vector>

using std::println;
using std::regex;
using std::regex_match;
using std::string;
using std::vector;
using std::views::filter;

namespace {
const regex email_rx(R"(^[a-zA-Z0-9_\.]+@((protonmail\.com)|(proton\.me))$)");
gpgme_ctx_t context = nullptr;

size_t populate_buffer(const void* contents, size_t size, size_t num, void* user_data)
{
    size_t realsize = size * num;
    auto buffer = static_cast<gpgme_data_t*>(user_data);
    gpgme_data_new_from_mem(buffer, static_cast<const char*>(contents), realsize, 1);
    return realsize;
}

int fetch_key_for(const auto& addr)
{
    int rv { 0 };
    CURLcode result;
    gpgme_data_t buffer;
    gpgme_error_t errcode;
    const string url = "https://api.protonmail.ch/pks/lookup?op=get&search=" + addr;

    CURL *handle = curl_easy_init();
    if (nullptr == handle) {
        println("internal curl error");
        rv = 6;
        goto FETCH_KEY_FOR_BAIL;
    }
    if ((CURLE_OK != (result = curl_easy_setopt(handle, CURLOPT_URL, url.c_str()))) ||
        (CURLE_OK != (result = curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, populate_buffer))) ||
        (CURLE_OK != (result = curl_easy_setopt(handle, CURLOPT_WRITEDATA, static_cast<void*>(&buffer)))) ||
        (CURLE_OK != (result = curl_easy_setopt(handle, CURLOPT_USERAGENT, "libcurl-agent/1.0")))) {
        println("internal curl error: {}", curl_easy_strerror(result));
        rv = 7;
        goto FETCH_KEY_FOR_BAIL;
    }
    if (CURLE_OK != (result = curl_easy_perform(handle))) {
        println("curl error: {}", curl_easy_strerror(result));
        return 8;
    }
    errcode = gpgme_op_import(context, buffer);
    if (GPG_ERR_NO_ERROR != errcode) {
        println("couldn't import key: error {}", gpgme_strerror(errcode));
    }

FETCH_KEY_FOR_BAIL:
    if (buffer != nullptr)
        gpgme_data_release(buffer);
    curl_easy_cleanup(handle);
    return rv;
}

int initialize_gpgme()
{
    if (nullptr == gpgme_check_version("2.0.0")) {
        println("GPGME library is too old: version 2.0 or later is required");
        return 2;
    }
    gpgme_set_locale(nullptr, LC_CTYPE, setlocale(LC_CTYPE, nullptr));
    if (GPG_ERR_NO_ERROR != gpgme_engine_check_version(GPGME_PROTOCOL_OpenPGP)) {
        println("GPGME could not initialize an OpenPGP engine. Aborting…");
        return 3;
    }
    if (GPG_ERR_NO_ERROR != gpgme_new(&context)) {
        println("GPGME could not create a new context. Aborting…");
        return 4;
    }
    if (GPG_ERR_NO_ERROR != gpgme_set_protocol(context, GPGME_PROTOCOL_OpenPGP)) {
        println("GPGME could not be switched to OpenPGP mode. Aborting…");
        return 5;
    }
    return 0;
}
}

int main(int argc, char* argv[])
{
    int rv { 0 };
    vector<string> args(argv + 1, argv + argc);

    if (0 != curl_global_init(CURL_GLOBAL_ALL)) {
        rv = 1;
        goto BAIL;
    };
    rv = initialize_gpgme();
    if (0 != rv)
        goto BAIL;

    for (const auto& s : filter(args, [](const auto& addr) { return regex_match(addr, email_rx); })) {
        rv = fetch_key_for(s);
        if (0 != rv)
            break;
    }

BAIL:
    if (nullptr != context)
        gpgme_release(context);
    curl_global_cleanup();
    return rv;
}
