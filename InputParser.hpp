#pragma once
#include "Event.hpp"

#include <fstream>
#include <vector>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <unordered_set>

//Read methods return false on invalid input, true otherwise
//Thus, read methods contains validation logic
namespace YadroTask {
    class InputParser {
    private:
        bool ValidateEventText(const std::string& text);
        std::unordered_set<int> events_set = {1, 2, 3, 4};

        int number;
        int hours;
        int minutes;
        Event event;
        int prev_event_time;

        bool ReadNumber(std::stringstream &input);
        bool ReadDayTime(std::stringstream &input);
        bool ReadEvent(std::stringstream &input, int number_of_tables);

    public:
        InputParser();

        int DirectReadNumber(std::string& number_line);
        std::vector<int> DirectReadDayTime(std::string& day_time_line);
        const Event& DirectReadEvent(std::string& event_line, int number_of_tables = 0);
    };
};