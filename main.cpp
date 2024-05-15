#include "InputParser.hpp"
#include "TableScheduler.hpp"
#include "InvalidFormatException.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <exception>

using namespace YadroTask;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Program call must be: program.exe test_file.txt";
        return 1;
    }

    std::ifstream input(argv[1]);
    if (!input.is_open()) {
        std::cerr << "No such file " << argv[1] << "\n";
        return 2;
    }

    try {
        InputParser parser;
        std::string current_input_line;

        std::getline(input, current_input_line);
        int number_of_tables = parser.DirectReadNumber(current_input_line);

        std::getline(input, current_input_line);
        auto time = parser.DirectReadDayTime(current_input_line);
        int open_hours = time[0];
        int open_minutes = time[1];
        int close_hours = time[2];
        int close_minutes = time[3];

        std::getline(input, current_input_line);
        int cost_per_hour = parser.DirectReadNumber(current_input_line);

        TableScheduler computerClub(number_of_tables,
                                    open_hours,
                                    open_minutes,
                                    close_hours,
                                    close_minutes,
                                    cost_per_hour);

        while (std::getline(input, current_input_line)) {
            computerClub.ProcessEvent(parser.DirectReadEvent(current_input_line, number_of_tables));
        }
        computerClub.CloseClub();
        std::cout << computerClub.getReport();
    } catch (std::exception& e) {
        std::cerr << e.what();
        return 3;
    }

    return 0;
}