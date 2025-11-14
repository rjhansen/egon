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

void fetch_key_for(const char* addr)
{
    static bool isGpgMEInitialized = false;
    static httplib::Client client { "https://api.protonmail.ch" };

    // All networking code lives here.
    auto res = client.Get(format("/pks/lookup?op=get&search={}", addr));
    switch (res.error()) {
    case httplib::Error::Success:
        break;
    case httplib::Error::SSLConnection:
        throw egon_error(format("SSL connection failed: {}", res.ssl_error()));
    case httplib::Error::SSLLoadingCerts:
    case httplib::Error::SSLServerVerification:
    case httplib::Error::SSLServerHostnameVerification:
        throw egon_error(format("SSL hostname verification failed, X509 error: {}",
            res.ssl_openssl_error()));
    default:
        throw egon_error(format("HTTP error: {}", httplib::to_string(res.error())));
    }

    // All GnuPG code lives here.
    if (!isGpgMEInitialized) {
        GpgME::initializeLibrary();
        isGpgMEInitialized = true;
    }
    const unique_ptr<Context> ctx { Context::createForProtocol(OpenPGP) };
    if (nullptr == ctx) {
        throw egon_error("Couldn’t acquire context.");
    }
    auto buffer = Data(res->body.c_str(), res->body.size(), true);
    if (buffer.isNull()) {
        throw egon_error("Couldn’t allocate buffer");
    };
    auto errcode = ctx->importKeys(buffer);
    if (errcode.error().isError()) {
        throw egon_error(format("Couldn’t import key: {}",
            errcode.error().asStdString()));
    }
}