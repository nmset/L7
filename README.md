# L7
 A library aiming to help writing database applications using wxWidgets, PostgreSQL and SQLite.

It's a generic spin-off of a production library I'm using.

Released under the LGPL 2.1 of the Free software Foundation.

Author and copyright : SET, M.D. , nmset@yandex.com

Developped with NetBeans.

------Build

wxWidgets is expected in /usr/local/wxWidgets, as referenced in the NetBeans project.libpq and libsqlite3 are expected in the system's library directory.

$ cd L7

$ make clobber

$ make CONF=Debug

$ cd ../L7Test

$ make CONF=Debug

Output will be in dist/Debug/GNU-Linux/ and dist/Release/GNU-Linux/ .

------How to consider the code.

I do not have an academic background in computer science, programming is my 
hobby.

------How to consider L7.

Use at your own risks. It's free software.
Use it or trash it. No warranties.
L7 does not claim to be fit for any purpose.
L7 does not expect to be used.

------Documentation

$ cd L7

$ doxygen L7.dox

Output will be in docs/ .
