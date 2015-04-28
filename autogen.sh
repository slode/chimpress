#!/bin/bash

aclocal
autoheader
automake --include-deps --add-missing --copy
autoconf
