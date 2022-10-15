# L7

A custom library aiming to help writing simple database applications using [wxWidgets](https://wxwidgets.org/), PostgreSQL and SQLite. It requires [wxJSON](https://wxcode.sourceforge.net/docs/wxjson/) that should be first downloaded and built with the provided template file.


### Build

$ cd L7

$ mkdir build; cd $_

$ cmake -DwxJson_DIR:PATH=/usr/local -DWITH_LIBPQ:BOOL=ON -DWITH_LIBSQ:BOOL=ON ../

$ make L7 -j4 && make L7Test


##### Documentation

$ cd L7

$ doxygen Resources/L7.dox

Output will be in docs/ .

## Disclaimer

Use at your own risks. L7 does not claim to be fit for any purpose. L7 does not expect to be used.