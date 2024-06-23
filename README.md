# LS in C implementation

## Overview

This is a custom implementation of the Unix `ls` command in C. It provides functionality to list directory contents with various options such as showing hidden files, sorting by modification time, recursive listing, and displaying detailed file information.

## Features

- **-a, --all**: Show all files including hidden files (files starting with `.`).
- **-l, --long**: Use a long listing format to display detailed file information.
- **-R, --recursive**: List subdirectories recursively.
- **-t, --time**: Sort files by modification time, newest first.
- **--help**: Display help information.

## Usage

Compile the program using the following command:
```sh
gcc -o mojls mojls.c
```

Run the program with various options:
```sh
./mojls [DIRECTORY]... [OPTION]...
```

### Examples

1. **List all files in the current directory:**
    ```sh
    ./mojls
    ```

2. **List all files, including hidden files:**
    ```sh
    ./mojls -a
    ```

3. **List files with detailed information:**
    ```sh
    ./mojls -l
    ```

4. **List files recursively:**
    ```sh
    ./mojls -R
    ```

5. **List files sorted by modification time:**
    ```sh
    ./mojls -t
    ```

6. **List files with multiple options:**
    ```sh
    ./mojls -alRt
    ```

7. **Display help information:**
    ```sh
    ./mojls --help
    ```

## Dependencies

- Requires standard C libraries: `stdio.h`, `sys/types.h`, `dirent.h`, `errno.h`, `string.h`, `stdlib.h`, `sys/stat.h`, `unistd.h`, `time.h`, `stdbool.h`, `getopt.h`.
- `help.txt`: A file containing help information to be displayed when the `--help` option is used.


## Help

For help or more information on the usage of this program, you can use the `--help` option or refer to the source code comments.

```sh
./mojls --help
```

## Notes

- Ensure `help.txt` is in the same directory as the compiled program to use the help feature.
- This implementation uses ANSI escape codes for coloring directory and executable file names.
- The program handles directory recursion and detailed listing, providing a comprehensive view of the file system structure.

## License

This project is licensed under the MIT License. Feel free to use, modify, and distribute as per the terms of the license.
