#pragma once
#include <string>
#include <utility>

namespace YadroTask {

    struct Event {
        int time;
        int id;
        int table_id;
        std::string name;

        Event()
        : time(0), id(0), table_id(0), name() {

        }

        Event(int _time, int _id, std::string _text, int _table_id = 0)
        : time(_time), id(_id), name(std::move(_text)), table_id(_table_id) {
        }

    };
}