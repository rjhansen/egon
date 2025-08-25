# Egon

## What's Egon?
[Proton Mail](https://protonmail.com) is an email provider that creates
an OpenPGP keypair for each account. They provide a simple API to fetch
public keys for their accounts, but it's not quite as easy as I'd like.
What I want to do is simple: type `egon rob.hansen@protonmail.com` and
have it fetch the key and add it to my GnuPG keyring automagically.

## What does Egon do?
Exactly that, and nothing more.

## How do I build it?

```sh
$ cd egon
$ mkdir build
$ cd build
$ cmake -S .. -B . -DCMAKE_BUILD_TYPE=Release
$ make
$ make install
```

## What are its dependencies?
You'll need libcurl and GPGME both installed.

## Bugs
If you give it a nonexistent Proton user account, Proton will cheerfully
give you back a dummy public key, and Egon will then import it because
it has no way of knowing it's fake.