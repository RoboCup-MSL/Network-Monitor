# msl-monitor-roboc0p

## Dependencies
msl-monitor-roboc0p depends on qt5, aircrack-ng, and iwlist and nmcli

## aircrack-ng
* Get sources with:

`git clone https://github.com/aircrack-ng/aircrack-ng`

* Install packages needed:

`sudo apt-get install build-essential autoconf automake libtool pkg-config libnl-3-dev libnl-genl-3-dev libssl-dev  libsqlite3-dev libpcre3-dev ethtool shtool rfkill`

* Compile with the following commands:

`autoreconf -i`

`./configure`

`make`


* Run unit tests with:

`make check`

* Install with:

`sudo make install`

## msl-monitor-roboc0p

* Install packages needed:

`sudo apt-get install qt5-default wireless-tools`

* Compile application:

`qmake policia.pro`

`make`

* Run application as root with:

`sudo ./policia`