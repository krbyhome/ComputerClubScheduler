#include "Logger.hpp"

namespace YadroTask {
    std::string NumberToTime(int number) {
        std::string time;
        if (number / 60 < 10) {
            time.push_back('0');
        }
        time.append(std::to_string(number / 60));
        time.push_back(':');
        if (number % 60 < 10) {
            time.push_back('0');
        }
        time.append(std::to_string(number % 60));

        return time;
    }


    void Logger::Log(const Event &event) {
        std::string text;
        text.append(NumberToTime(event.time));
        text.push_back(' ');
        text.append(std::to_string(event.id));
        text.push_back(' ');
        text.append(event.name);

        if (event.id == 2 || event.id == 12) {
            text.push_back(' ');
            text.append(std::to_string(event.table_id));
        }
        logs.append(text);
        logs.push_back('\n');
    }

    void Logger::Log(const std::string &log) {
        logs.append(log);
        logs.push_back('\n');
    }

    const std::string &Logger::getLogs() {
        return logs;
    }
}