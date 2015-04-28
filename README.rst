Compression utils
=================

Simple CLI tools to do simple compression. Code for lzw inspired by the lzw code on rosettacode.org.

To build, you need autotools::

    $ ./autogen.sh
    $ ./configure
    $ make -j

To test::

    $ ./test.sh datafile/simple.txt

To clean up the mess after building::

    $ git clean -fdx

