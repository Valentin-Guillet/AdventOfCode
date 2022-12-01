
#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <vector>
#include <algorithm>
#include <unordered_map>


struct Date {
    int month, day;
    int hour, minute;

    Date() : month(0), day(0), hour(0), minute(0) { }
    Date(int month, int day, int hour, int minute) :
        month(month), day(day), hour(hour), minute(minute) { }

    bool operator<(const Date& other) {
        return (month < other.month || (month == other.month && day < other.day)
                                    || (month == other.month && day == other.day && hour < other.hour)
                                    || (month == other.month && day == other.day && hour == other.hour && minute < other.minute));
    }
};

struct Event {

    Date date;
    std::string action;

    Event(std::string input) {
        int month = std::stoi(input.substr(6, 2));
        int day = std::stoi(input.substr(9, 2));

        int hour = std::stoi(input.substr(12, 2));
        int minute = std::stoi(input.substr(15, 2));

        date = Date(month, day, hour, minute);
        action = input.substr(19);
    }

    bool operator<(const Event& other) {
        return date < other.date;
    }
};

struct Guard {

    int total_asleep;
    std::array<int, 60> minutes_asleep;

    void add_sleep(const Date& start, const Date& end) {
        total_asleep += end.minute - start.minute;

        for (int i=start.minute; i<end.minute; ++i)
            minutes_asleep[i]++;
    }

    std::pair<int, int> max_minute() {
        int max = 0, id_max;
        for (int i=0; i<60; ++i) {
            if (max < minutes_asleep[i]) {
                max = minutes_asleep[i];
                id_max = i;
            }
        }
        return {id_max, max};
    }
};

int main(void)
{
    bool first_half = false;
	int ans = 0;
	std::vector<Event> events;

	std::ifstream myfile ("input");
	if (myfile.is_open()) {
        std::string input;

		while (getline(myfile, input))
            events.push_back(Event(input));

		myfile.close();
	} else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    std::sort(events.begin(), events.end());

    std::unordered_map<int, Guard> guards;
    int id_guard;
    Date start;

    for (Event event : events) {
        if (event.action.substr(0, 5) == "Guard") {
            int ind_begins = event.action.find(" begins");
            id_guard = std::stoi(event.action.substr(7, ind_begins-7));

        } else if (event.action == "falls asleep") {
            start = event.date;

        } else if (event.action == "wakes up") {
            guards[id_guard].add_sleep(start, event.date);

        } else {
            std::cout << "Invalid event, something went wrong !" << std::endl;
        }
    }

    int id_max;

    if (first_half) {
        int time_max = 0;
        for (auto [id, guard] : guards) {
            if (time_max < guard.total_asleep) {
                time_max = guard.total_asleep;
                id_max = id;
            }
        }

    } else {
        int max_minute = 0;
        for (auto [id, guard] : guards) {
            if (max_minute < guard.max_minute().second) {
                max_minute = guard.max_minute().second;
                id_max = id;
            }
        }
    }

    ans = id_max * guards[id_max].max_minute().first;

	std::cout << "Answer : " << ans << std::endl;
	return 0;
}
