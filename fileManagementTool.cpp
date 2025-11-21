#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<algorithm>
#include<limits>

void writeFile(const std::string& filename, const std::string& content) {
    // std::ofstream will create the file if it doesn't exist,
    // or truncate (overwrite) it if it does.
    std::ofstream outFile(filename);

    if (!outFile.is_open()) {
        std::cerr << "Error: Could not open the file '" << filename << "' for writing.\n";
        return;
    }

    outFile << content;

    std::cout << "Successfully wrote to '" << filename << "'.\n";
}

void appendFile(const std::string& filename, const std::string& content) {
    // Open the file in append mode
    std::ofstream outFile(filename, std::ios::app);

    if (!outFile.is_open()) {
        std::cerr << "Error: Could not open the file '" << filename << "' for appending.\n";
        return;
    }

    outFile << content;

    std::cout << "Successfully appended to '" << filename << "'.\n";
}

void readFile(const std::string& filename) {
    std::ifstream inFile(filename);

    if (!inFile.is_open()) {
        std::cerr << "Error: Could not open the file '" << filename << "' for reading.\n";
        return;
    }

    std::cout << "\n--- Content of '" << filename << "' ---\n";
    std::string line;
    while (std::getline(inFile, line)) {
        std::cout << line << std::endl;
    }
    std::cout << "--- End of File ---\n";

}

void showMenu() {
    std::cout << "=============================\n";
    std::cout << "   File Management Tool\n";
    std::cout << "=============================\n";
    std::cout << "1. Write to File (Overwrite)\n";
    std::cout << "2. Read from File\n";
    std::cout << "3. Append to File\n";
    std::cout << "4. Exit\n";
    std::cout << "-----------------------------\n";
    std::cout << "Enter your choice: ";
}

int getUserChoice() {
    int choice;
    std::cin >> choice;

    // Input validation
    while (std::cin.fail()) {
        std::cout << "Invalid input. Please enter a number: ";
        std::cin.clear(); // clear input buffer to restore cin
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // ignore last input
        std::cin >> choice;
    }
    
    // Consume the rest of the line to handle the newline character after number input
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
       std::string fileName;
       std::string content;

       do{
              showMenu();
              choice = getUserChoice();

              if (choice >= 1 && choice <= 3) {
              std::cout << "Enter filename (e.g., my_document.txt): ";
              fileName = getInput();
              }
              switch (choice) {
              case 1: // Write to file
                std::cout << "Enter content to write (end with a new line):\n";
                content = getInput();
                writeFile(fileName, content);
                break;
              case 2: // Read from file
                readFile(fileName);
                break;
              case 3: // Append to file
                std::cout << "Enter content to append (end with a new line):\n";
                content = getInput();
                appendFile(fileName, content);
                break;
              case 4: // Exit
                std::cout << "Exiting the program. Goodbye!\n";
                break;
              default:
                std::cout << "Invalid choice. Please try again.\n";
                break;
              }std::cout << "\n";
       } while (choice != 4);
       return 0;
}
