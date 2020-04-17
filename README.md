# Boost C++ Libraries

The Boost project provides free peer-reviewed portable C++ source libraries.

We emphasize libraries that work well with the C++ Standard Library. Boost
libraries are intended to be widely useful, and usable across a broad spectrum
of applications. The Boost license encourages both commercial and non-commercial use
and does not require attribution for binary use.

The project website is www.boost.org, where you can obtain more information and
[download](https://www.boost.org/users/download/) the current release.

## Cloning

Clone the superproject repository with all submodules and checkout the `master` as the default branch:

```console
git clone --recurse-submodules https://github.com/boostorg/boost.git boost-root
```

or checkout the `develop` branch with the latest development snapshot:

```console
git clone --branch develop --recurse-submodules https://github.com/boostorg/boost.git boost-root
```

Alternatively, clone just the superproject repository, then clone a desired submodule together with its dependencies:

```console
git clone https://github.com/boostorg/boost.git boost-root
cd boost-root
git submodule update --init tools/boostdep
python tools/boostdep/depinst/depinst.py filesystem
```

Alternatively, clone just the superproject repository, then manually pick individual submodules:

```console
git clone https://github.com/boostorg/boost.git boost-root
cd boost-root
git submodule --quiet update --init \
    tools/build \
    tools/boost_install \
    tools/boostdep \
    libs/headers
```
