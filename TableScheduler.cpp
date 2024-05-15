#include "TableScheduler.hpp"

YadroTask::TableScheduler::TableScheduler(int number_of_tables, int open_hours, int open_minutes, int close_hours,
                                          int close_minutes, int _cost_per_hour)
                                          : logger() {
    table_number = number_of_tables;
    cost_per_hour = _cost_per_hour;
    open_time = open_hours * 60 + open_minutes;
    close_time = close_hours * 60 + close_minutes;
    logger.Log(NumberToTime(open_time));
}

void YadroTask::TableScheduler::ProcessEvent(const YadroTask::Event &event) {
    logger.Log(event);

    switch(event.id) {
        case 1:
            ExecuteEvent1(event);
            break;
        case 2:
            ExecuteEvent2(event);
            break;
        case 3:
            ExecuteEvent3(event);
            break;
        case 4:
            ExecuteEvent4(event);
            break;
    }
}

std::string YadroTask::TableScheduler::getReport() {
    return logger.getLogs();
}

void YadroTask::TableScheduler::ExecuteEvent1(const YadroTask::Event &event) {
    if (clients.contains(event.name)) {
        Event error(event.time, 13, "YouShallNotPass");
        logger.Log(error);
        return;
    }

    if (event.time < open_time || close_time < event.time) {
        Event error(event.time, 13, "NotOpenYet");
        logger.Log(error);
        return;
    }

    clients.insert(event.name);
}

void YadroTask::TableScheduler::ExecuteEvent2(const YadroTask::Event &event) {
    if (booked_tables.contains(event.table_id)) {
        Event error(event.time, 13, "PlaceIsBusy");
        logger.Log(error);
        return;
    }

    if (!clients.contains(event.name)) {
        Event error(event.time, 13, "ClientUnknown");
        logger.Log(error);
        return;
    }

    if (client_table.contains(event.name)) {
        ClearTableAfterCustomer(event.name, event.time);
    }

    client_table[event.name] = event.table_id;
    booked_tables[event.table_id] = event.time;
}

void YadroTask::TableScheduler::ExecuteEvent3(const YadroTask::Event &event) {
    if (booked_tables.size() != table_number) {
        Event error(event.time, 13, "ICanWaitNoLonger!");
        logger.Log(error);
        return;
    }

    if (waiting_queue.size() + 1 > table_number) {
        Event Quit(event.time, 11, event.name);
        logger.Log(Quit);
        clients.erase(event.name);
        return;
    }

    clients.insert(event.name);
    waiting_queue.push(event.name);
}

void YadroTask::TableScheduler::ExecuteEvent4(const YadroTask::Event &event) {
    if (!clients.contains(event.name)) {
        Event error(event.time, 13, "ClientUnknown");
        logger.Log(error);
        return;
    }

    if (client_table.contains(event.name)) {
        int table_id = client_table[event.name];
        ClearTableAfterCustomer(event.name, event.time);
        clients.erase(event.name);

        if (!waiting_queue.empty()) {
            Event Seat(event.time, 12, waiting_queue.front(), table_id);
            logger.Log(Seat);
            client_table[waiting_queue.front()] = table_id;
            booked_tables[table_id] = event.time;
            waiting_queue.pop();
        }
    }
}

void YadroTask::TableScheduler::ClearTableAfterCustomer(const std::string &customer_name, int cur_time) {
    int table_id = client_table[customer_name];
    int start_time = booked_tables[table_id];
    table_money[table_id] += (cur_time - start_time) / 60 * cost_per_hour;
    if ((cur_time - start_time) % 60 != 0) {
        table_money[table_id] += cost_per_hour;
    }
    table_time[table_id] += (cur_time - start_time);
    booked_tables.erase(table_id);
    client_table.erase(customer_name);
}

void YadroTask::TableScheduler::CloseClub() {
    for (auto& client : clients) {
        if (client_table.contains(client)) {
            Event Quit(close_time, 11, client);
            logger.Log(Quit);
            ClearTableAfterCustomer(client, close_time);
        }
    }
    logger.Log(NumberToTime(close_time));

    for (int i = 1; i <= table_number; ++i) {
        logger.Log(std::to_string(i) + " " + std::to_string(table_money[i]) + " " + NumberToTime(table_time[i]));
    }
}
