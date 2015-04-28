Compression utils
=================

Simple CLI tools to do simple compression. The programs are written primarily
for simplicity and not performance and absolute robustness.

The code for the lzw compressor and decompressor is inspired by the lzw code on
rosettacode.org.

To build, you need autotools. On Ubuntu that means::

    $ sudo apt-get install autotools-dev automake

The utilities can then be installed via the standard::

    $ ./autogen.sh
    $ ./configure
    $ make -j

To test::

    $ ./test.sh datafile/simple.txt

To clean up the mess after building::

    $ git clean -fdx

