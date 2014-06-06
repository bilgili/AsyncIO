# AsyncIO

AsyncIO library provides pool of multi-threaded file readers based on Posix
and AIO. It is based on the futures, which users can wait on a particular
read operation result.

## Features

AsyncIO library provides:
* Job Pool where user can set the number of jobs in parallel.
* Jobs which user can use his own functions to read and be notified with result.
* Posix, AIO based reader pool
* Future based data operations ( wait, get status, etc )

## Building from Source

```
  git clone https://github.com/bilgili/AsyncIO.git
  mkdir AsyncIO/build
  cd AsyncIO/build
  cmake ..
  make
```

Or using Buildyard:

```
to be decided
```
