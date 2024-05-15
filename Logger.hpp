#pragma once

#include "Event.hpp"

#include <string>

namespace YadroTask {
    std::string NumberToTime(int number);

    class Logger {
    private:
        std::string logs;

    public:
        Logger() = default;

        void Log(const std::string& log);

        void Log(const Event& event);

        const std::string& getLogs();
    };
}
