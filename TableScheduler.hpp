#pragma once

#include "Event.hpp"
#include "Logger.hpp"

#include <set>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <map>

namespace YadroTask {
    class TableScheduler {
    private:
        int table_number;
        int open_time;
        int close_time;
        int cost_per_hour;
        std::unordered_map<int, int> booked_tables; //[table_id, start_time]
        std::unordered_map<int, int> table_money; // [table_id, money]
        std::unordered_map<int, int> table_time; // [table_id, time]

        std::set<std::string> clients; // set of clients
        std::unordered_map<std::string, int> client_table; // [client_name, table_id]
        std::queue<std::string> waiting_queue;

        Logger logger;

        void ExecuteEvent1(const Event& event);
        void ExecuteEvent2(const Event& event);
        void ExecuteEvent3(const Event& event);
        void ExecuteEvent4(const Event& event);
        void ClearTableAfterCustomer(const std::string& customer_name, int cur_time);

    public:
        TableScheduler(int number_of_tables, int open_hours, int open_minutes, int close_hours, int close_minutes, int _cost_per_hour);

        void ProcessEvent(const Event& event);
        void CloseClub();

        std::string getReport();
    };
};
