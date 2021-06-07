#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

#define FIFO_FILE "/tmp/g13-0"
#define PROMPT "G13>"
#define MAX_COMMAND_LENGTH 32

int main() {
    std::ofstream fifo_file;
    fifo_file.open("/tmp/g13-0", std::fstream::out);
    if (!fifo_file.is_open()) {
        std::cerr << "Error opening the FIFO file " << FIFO_FILE << std::endl;
        return -1;
    }
    std::cerr << "Successfully connected to the FIFO file, I can now send commands!" << std::endl;

    while (true) {
        std::string command;
        std::cout << PROMPT;
        getline(std::cin, command);

        std::stringstream command_ss;
        command_ss.str(command);
        std::string firstword;
        command_ss >> firstword;

        if (firstword == "help") {
            std::cout   << "g13d_cli: A command line interface to g13d to control a Logitech G13 Keyboard\n\n"
                        << "help\t\t\tPrint this help message\n" 
                        << "exit\t\t\tExit the program\n"
                        << "rgb r g b\t\tChange the backlight color to RGB(r,g,b)\n"
                        << "mod n\t\t\tSets the background light of the mod-keys. n is the sum of 1 (M1), 2 (M2), 4 (M3) and 8 (MR)\n"
                        << "\n\n";
        }
        else if (firstword == "rgb") {
            std::string r;
            std::string g;
            std::string b;
            command_ss >> r >> g >> b;

            if (r.empty() || g.empty() || b.empty()) {
                std::cout << "Error: use rgb r g b where the desired backlight color is RGB(r,g,b)\n";
            }
            else {
                fifo_file << "rgb " << r << " " << g << " " << b;
            }
        }
        else if (firstword == "mod") {
            std::string n;
            command_ss >> n;

            if (n.empty()) {
                std::cout << "Error: use mod n where n is the sum of 1 (M1), 2 (M2), 4 (M3) and 8 (MR)";
            }
            else {
                fifo_file << "mod " << n;
            }
        }
        else if (firstword == "exit") {
            break;
        }
        fifo_file.flush();
    }

    fifo_file.close();

}