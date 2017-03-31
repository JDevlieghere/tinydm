# TinyDM

As the name suggests, `tinydm` is a lightweight utility for downloading
multiple files concurrently. It takes a single argument, the number of threads,
and queues downloads until a thread is available.

## Usage

The sample command bellow starts three concurrent downloads. The downloads are
read from standard input by redirecting `urls.txt` which contains a list of
newline-separated URLs.

```bash
./tinydm 3 < urls.txt
Finished downloading sample_download0.bin
Finished downloading sample_download1.bin
Failed to download sample_download2.bin: Couldn't resolve host name
```

The output of `tinydm` is limited to one line per file informing you whether
the download was successful or not.

## Build

TinyDM's only dependency is libcurl.

```
$ git clone https://github.com/JDevlieghere/tinydm.git && cd tinydm
$ mkdir build && cd build
$ cmake ..
$ cmake --build .
```
