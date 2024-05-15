#include "InputParser.hpp"
#include "InvalidFormatException.hpp"

namespace YadroTask {
    InputParser::InputParser()
            : number(0), hours(0), minutes(0), prev_event_time(-1) {
    }

    bool InputParser::ReadNumber(std::stringstream &input) {
        std::string number_of_tables_line;
        input >> number_of_tables_line;
        try {
            number = std::stoi(number_of_tables_line);
        } catch (std::exception &e) {
            return false;
        };

        if (number <= 0) {
            return false;
        }

        return true;
    }

    bool InputParser::ReadDayTime(std::stringstream &input) {
        std::string day_time;
        input >> day_time;

        if (day_time.size() != 5 || day_time[2] != ':') {
            return false;
        }

        try {
            hours = std::stoi(day_time.substr(0, 2));
        } catch (std::exception &e) {
            return false;
        };

        try {
            minutes = std::stoi(day_time.substr(3));
        } catch (std::exception &e) {
            return false;
        };

        if (0 <= hours && hours < 24 && 0 <= minutes && minutes < 60) {
            return true;
        }

        return false;
    }

    bool InputParser::ReadEvent(std::stringstream &input, int number_of_tables) {
        if (!ReadDayTime(input)) {
            return false;
        }
        event.time = hours * 60 + minutes;
        if (event.time < prev_event_time) {
            return false;
        }
        prev_event_time = event.time;

        if (!ReadNumber(input)) {
            return false;
        }
        event.id = number;
        if (!events_set.contains(event.id)) {
            return false;
        }

        input >> event.name;
        if (!ValidateEventText(event.name)) {
            return false;
        }

        if (event.id == 2) {
            if (!ReadNumber(input)) {
                return false;
            }

            event.table_id = number;
            return 0 < event.table_id && event.table_id <= number_of_tables;
        }

        return true;
    }

    bool InputParser::ValidateEventText(const std::string &text) {
        return  !text.empty() && std::ranges::all_of(text.begin(), text.end(), [](char c) {
            return isalnum(c) || c == '-' || c == '_';
        });
    }

    int InputParser::DirectReadNumber(std::string &number_line) {
        number = 0;

        std::stringstream ss(number_line);
        if (!ReadNumber(ss)) {
            throw InvalidFormatException(number_line);
        }

        return number;
    }

    std::vector<int> InputParser::DirectReadDayTime(std::string &day_time_line) {
        hours = 0;
        minutes = 0;

        std::stringstream ss(day_time_line);
        if (!ReadDayTime(ss)) {
            throw InvalidFormatException(day_time_line);
        }
        std::vector<int> res;
        res.push_back(hours);
        res.push_back(minutes);

        if (!ReadDayTime(ss)) {
            throw InvalidFormatException(day_time_line);
        }
        res.push_back(hours);
        res.push_back(minutes);

        return res;
    }

    const Event& InputParser::DirectReadEvent(std::string &event_line, int number_of_tables) {
        event = Event();

        std::stringstream ss(event_line);
        if (!ReadEvent(ss, number_of_tables)) {
            throw InvalidFormatException(event_line);
        }

        return event;
    }
};