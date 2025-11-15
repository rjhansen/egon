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

#define CPPHTTPLIB_OPENSSL_SUPPORT

#include "egon.h"
#include "gpgme++/context.h"
#include "gpgme++/data.h"
#include "gpgme++/importresult.h"
#include "httplib.h"
#include <memory>

using GpgME::Context;
using GpgME::Data;
using GpgME::OpenPGP;
using GpgME::Protocol;
using std::format;
using std::string;
using std::unique_ptr;

namespace {
  string get_certificate_for(const char* addr) {
    static httplib::Client client{"https://api.protonmail.ch"};

    auto res = client.Get(format("/pks/lookup?op=get&search={}", addr));
    switch (res.error()) {
      case httplib::Error::Success:
        return res->body;
      case httplib::Error::SSLConnection:
        throw egon_error(format("SSL connection failed: {}", res.ssl_error()));
      case httplib::Error::SSLLoadingCerts:
      case httplib::Error::SSLServerVerification:
      case httplib::Error::SSLServerHostnameVerification:
        throw egon_error(format("SSL general failure, X509 error: {}",
                                res.ssl_openssl_error()));
      default:
        throw egon_error(format("HTTP error: {}", httplib::to_string(res.error())));
    }
  }

  void import_certificate(const std::string& certificate) {
    static bool isGpgMEInitialized = false;

    if (!isGpgMEInitialized) {
      GpgME::initializeLibrary();
      isGpgMEInitialized = true;
    }

    const unique_ptr<Context> ctx{Context::createForProtocol(OpenPGP)};
    if (nullptr == ctx) {
      throw egon_error("Couldn’t acquire context.");
    }

    auto buffer = Data(certificate.c_str(), certificate.size(), true);
    if (buffer.isNull()) {
      throw egon_error("Couldn’t allocate buffer");
    }

    auto errcode = ctx->importKeys(buffer);
    if (errcode.error().isError()) {
      throw egon_error(
          format("Couldn’t import key: {}", errcode.error().asStdString()));
    }
  }
}

void fetch_key_for(const char *addr) {
  import_certificate(get_certificate_for(addr));
}
