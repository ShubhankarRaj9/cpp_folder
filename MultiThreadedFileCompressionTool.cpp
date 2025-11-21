#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>
#include <algorithm>
#include <limits>

std::mutex mtx;

// Simple Run-Length Encoding (RLE) Compression
std::string compressRLE(const std::string& data) {
    if (data.empty()) return "";
    std::string compressed;
    char current = data[0];
    int count = 1;
    for (size_t i = 1; i < data.size(); ++i) {
        if (data[i] == current) {
            ++count;
        } else {
            compressed += current + std::to_string(count);
            current = data[i];
            count = 1;
        }
    }
    compressed += current + std::to_string(count);
    return compressed;
}

// Simple RLE Decompression
std::string decompressRLE(const std::string& data) {
    std::string decompressed;
    for (size_t i = 0; i < data.size(); ) {
        char ch = data[i++];
        std::string countStr;
        while (i < data.size() && std::isdigit(data[i])) {
            countStr += data[i++];
        }
        int count = std::stoi(countStr);
        decompressed += std::string(count, ch);
    }
    return decompressed;
}

// Single-threaded compression
void compressFileSingleThreaded(const std::string& inputFile, const std::string& outputFile) {
    std::ifstream inFile(inputFile, std::ios::binary);
    if (!inFile.is_open()) {
        std::cerr << "Error: Could not open input file '" << inputFile << "' for reading.\n";
        return;
    }
    std::string data((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
    inFile.close();

    std::string compressed = compressRLE(data);

    std::ofstream outFile(outputFile, std::ios::binary);
    if (!outFile.is_open()) {
        std::cerr << "Error: Could not open output file '" << outputFile << "' for writing.\n";
        return;
    }
    outFile << compressed;
    outFile.close();

    std::cout << "File compressed single-threaded: '" << inputFile << "' -> '" << outputFile << "'\n";
}

// Function to compress a chunk (for multithreading)
void compressChunk(const std::string& chunk, std::string& result, std::mutex& mtx) {
    std::string compressed = compressRLE(chunk);
    std::lock_guard<std::mutex> lock(mtx);
    result += compressed;
}

// Multi-threaded compression
void compressFileMultiThreaded(const std::string& inputFile, const std::string& outputFile, int numThreads) {
    std::ifstream inFile(inputFile, std::ios::binary);
    if (!inFile.is_open()) {
        std::cerr << "Error: Could not open input file '" << inputFile << "' for reading.\n";
        return;
    }
    std::string data((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
    inFile.close();

    size_t dataSize = data.size();
    size_t chunkSize = dataSize / numThreads;
    if (chunkSize == 0) chunkSize = 1; // Minimum chunk size

    std::vector<std::thread> threads;
    std::string compressedResult;
    std::mutex resultMutex;

    for (int i = 0; i < numThreads; ++i) {
        size_t start = i * chunkSize;
        size_t end = (i == numThreads - 1) ? dataSize : (i + 1) * chunkSize;
        std::string chunk = data.substr(start, end - start);
        threads.emplace_back(compressChunk, chunk, std::ref(compressedResult), std::ref(resultMutex));
    }

    for (auto& t : threads) {
        t.join();
    }

    std::ofstream outFile(outputFile, std::ios::binary);
    if (!outFile.is_open()) {
        std::cerr << "Error: Could not open output file '" << outputFile << "' for writing.\n";
        return;
    }
    outFile << compressedResult;
    outFile.close();

    std::cout << "File compressed multi-threaded (" << numThreads << " threads): '" << inputFile << "' -> '" << outputFile << "'\n";
}

// Decompression
void decompressFile(const std::string& inputFile, const std::string& outputFile) {
    std::ifstream inFile(inputFile, std::ios::binary);
    if (!inFile.is_open()) {
        std::cerr << "Error: Could not open input file '" << inputFile << "' for reading.\n";
        return;
    }
    std::string data((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
    inFile.close();

    std::string decompressed = decompressRLE(data);

    std::ofstream outFile(outputFile, std::ios::binary);
    if (!outFile.is_open()) {
        std::cerr << "Error: Could not open output file '" << outputFile << "' for writing.\n";
        return;
    }
    outFile << decompressed;
    outFile.close();

    std::cout << "File decompressed: '" << inputFile << "' -> '" << outputFile << "'\n";
}

// Performance comparison
void comparePerformance(const std::string& inputFile, int numThreads) {
    std::string singleOutput = "single_compressed.bin";
    std::string multiOutput = "multi_compressed.bin";

    // Single-threaded timing
    auto start = std::chrono::high_resolution_clock::now();
    compressFileSingleThreaded(inputFile, singleOutput);
    auto end = std::chrono::high_resolution_clock::now();
    auto singleTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    // Multi-threaded timing
    start = std::chrono::high_resolution_clock::now();
    compressFileMultiThreaded(inputFile, multiOutput, numThreads);
    end = std::chrono::high_resolution_clock::now();
    auto multiTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "Performance Comparison:\n";
    std::cout << "Single-threaded time: " << singleTime << " ms\n";
    std::cout << "Multi-threaded time (" << numThreads << " threads): " << multiTime << " ms\n";
    if (multiTime < singleTime) {
        std::cout << "Multi-threaded is faster by " << (singleTime - multiTime) << " ms\n";
    } else {
        std::cout << "Single-threaded is faster or equal.\n";
    }
}

void showMenu() {
    std::cout << "=====================================\n";
    std::cout << "   Multithreaded File Compression Tool\n";
    std::cout << "=====================================\n";
    std::cout << "1. Compress file (Single-threaded)\n";
    std::cout << "2. Compress file (Multi-threaded)\n";
    std::cout << "3. Decompress file\n";
    std::cout << "4. Compare performance\n";
    std::cout << "5. Exit\n";
    std::cout << "-------------------------------------\n";
    std::cout << "Enter your choice: ";
}

int getUserChoice() {
    int choice;
    std::cin >> choice;

    while (std::cin.fail()) {
        std::cout << "Invalid input. Please enter a number: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> choice;
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return choice;
}

std::string getInput() {
    std::string input;
    std::getline(std::cin, input);
    return input;
}

int main() {
    int choice;
    std::string inputFile, outputFile;
    int numThreads = 4; // Default number of threads

    do {
        showMenu();
        choice = getUserChoice();

        switch (choice) {
        case 1: // Compress single-threaded
            std::cout << "Enter input filename: ";
            inputFile = getInput();
            std::cout << "Enter output filename: ";
            outputFile = getInput();
            compressFileSingleThreaded(inputFile, outputFile);
            break;
        case 2: // Compress multi-threaded
            std::cout << "Enter input filename: ";
            inputFile = getInput();
            std::cout << "Enter output filename: ";
            outputFile = getInput();
            std::cout << "Enter number of threads: ";
            std::cin >> numThreads;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            compressFileMultiThreaded(inputFile, outputFile, numThreads);
            break;
        case 3: // Decompress
            std::cout << "Enter input filename: ";
            inputFile = getInput();
            std::cout << "Enter output filename: ";
            outputFile = getInput();
            decompressFile(inputFile, outputFile);
            break;
        case 4: // Compare performance
            std::cout << "Enter input filename: ";
            inputFile = getInput();
            std::cout << "Enter number of threads: ";
            std::cin >> numThreads;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            comparePerformance(inputFile, numThreads);
            break;
        case 5: // Exit
            std::cout << "Exiting the program. Goodbye!\n";
            break;
        default:
            std::cout << "Invalid choice. Please try again.\n";
            break;
        }
        std::cout << "\n";
    } while (choice != 5);

    return 0;
}
