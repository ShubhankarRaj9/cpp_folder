# cpp_folder

This repository contains several C++ tools, including:

- **File Management Tool:** Create, read, and append to text files interactively.
- **MultiThreaded File Compression Tool:** Compress and decompress files using a simple Run-Length Encoding algorithm, supporting both single-threaded and multi-threaded execution.
- **Simple Compiler:** Parse and evaluate basic arithmetic expressions.

## Files Overview

- `fileManagementTool.cpp`: Interactive CLI tool to write, read, and append files.
- `MultiThreadedFileCompressionTool.cpp`: Compress/decompress files using RLE. Supports performance comparison between single and multi-threaded runs.
- `simpleCompiler.cpp`: Simple arithmetic parser supporting `+`, `-`, `*`, `/`, parentheses.
- `my_file.txt`, `test.txt`: Example text files.

## Usage

### Build

You can build the tools using g++:

```bash
g++ -o fileTool fileManagementTool.cpp
g++ -o compressionTool MultiThreadedFileCompressionTool.cpp -pthread
g++ -o simpleCompiler simpleCompiler.cpp
```

### Run

#### File Management Tool

```bash
./fileTool
```
Interactive menu will appear for writing, reading, and appending to files.

#### File Compression Tool

```bash
./compressionTool
```
Offered menu options for compression (single/multi-threaded), decompression, and performance comparison.

#### Simple Compiler

```bash
./simpleCompiler
```
Enter an arithmetic expression when prompted.

## Example

- Use the file tool to create or append to `test.txt`.
- Compress files (e.g., `test.txt`) and evaluate compression speed.
- Try arithmetic like `2 + 3 * (4 - 1)` using the compiler.

## Requirements

- C++11 or higher
- pthreads (for multithreading, on Linux/macOS)

## License

MIT
