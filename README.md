# Egon

## What's Egon?
[Proton Mail](https://protonmail.com) is an email provider that creates
an OpenPGP keypair for each account. They provide a simple API to fetch
public keys for their accounts, but it's not quite as easy as I'd like.
What I want to do is simple: type `egon rob.hansen@protonmail.com` and
have it fetch the key and add it to my GnuPG keyring automagically.

## What does Egon do?
Exactly that, and nothing more.

## Couldn’t you do this another way?
Sure.

## But my other way is easier! And better!
I’m happy for you. 😀

## How is it licensed?
Egon uses the excellent [cpp-httplib](https://github.com/yhirose/cpp-httplib)
HTTP transaction library, which is released under the [MIT License](https://github.com/yhirose/cpp-httplib?tab=MIT-1-ov-file).

Egon also uses [GnuPG](https//gnupg.org)’s GnuPG Made Easy library, which is
released under the [GNU Lesser General Public License](https://www.gnupg.org/documentation/manuals/gpgme/Library-Copying.html),
version 2.1 (or, at your discretion, any lesser version).

All my code (which isn't much!) is under the Apache 2.0 license.

## How do I build it?

```sh
$ cd egon
$ mkdir build
$ meson setup builddir
$ meson compile -C builddir
```

## What are its dependencies?
You'll need GPGMEPP, OpenSSL, and libcrypto.

## Bugs
If you give it a nonexistent Proton user account, Proton will cheerfully
give you back a dummy public key, and Egon will then import it because
it has no way of knowing it's fake.
