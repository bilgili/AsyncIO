# AsyncIO

AsyncIO library provides pool of multi-threaded file readers based on Posix
and AIO. It is based on the futures, which users can wait on a particular
read operation result.

## Features

AsyncIO library provides:

* Posix, AIO based reader pool using futures

## Building from Source

```
  git clone https://github.com/bilgili/AsyncIO.git
  mkdir AsyncIO/build
  cd AsyncIO/build
  cmake ..
  make

```
