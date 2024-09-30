# MyFind

<div style="text-align: center;">
    <img src="https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white" alt="C++ Badge" />

</div>


## Overview

**MyFind** is a command-line tool, written in C++, that mimics the behavior of Unix `find`. It allows you to search for files in a directory hierarchy.

## Features

- Recursive search with `-R`
- Case-insensitive search with `-i`
- Lightweight and efficient

## Project Structure

```bash
myfind/
├── .gitignore
├── Makefile
├── main.cpp
```

## Installation

Clone the repository and build:

```bash
git clone https://github.com/MehmetMertt/myfind.git
cd myfind
make
```

## Usage

Run `myfind` with options:

```bash
./myfind [OPTIONS] [FOLDER] [FILE/S]
```

### Available Options

- `-R`: Enable recursive search through directories.
- `-i`: Perform case-insensitive searches.


### Example Usage

Search for `hello.txt` file recursively:

```bash
./myfind -R ./ hello.txt
```

Perform a case-insensitive search for a `.txt` file:

```bash
./myfind -i ./ hello.txt
```

Or both

```bash
./myfind -i -R ./ hello.txt
```
