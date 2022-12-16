
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>


struct Position {
    int x;
    int y;

    bool operator==(Position const& other) const {
        return (x == other.x) && (y == other.y);
    }
};

struct hashPosition {
    size_t operator()(Position const& pos) const {
        return 31 * pos.x ^ pos.y;
    }
};

struct Sensor : Position {
    int radius;
};

using Beacon = Position;

struct Interval {
    int beg;
    int end;

    bool operator<(Interval const& other) {
        return (beg < other.beg) || (beg == other.beg && end < other.end);
    }
};

void add_sensor_and_beacon(std::vector<Sensor>& sensors, std::vector<Beacon>& beacons, std::string input) {
    Sensor sensor;
    size_t index1 = input.find(',');
    size_t index2 = input.find(':');
    sensor.x = std::stoi(input.substr(12, index1-11));
    sensor.y = std::stoi(input.substr(index1+4, index2-index1-3));

    Beacon beacon;
    input = input.substr(index2+2);
    index1 = input.find(',');
    beacon.x = std::stoi(input.substr(23, index1-23));
    beacon.y = std::stoi(input.substr(index1+4));

    sensor.radius = std::abs(sensor.x - beacon.x) + std::abs(sensor.y - beacon.y);

    sensors.push_back(sensor);
    if (std::find(beacons.begin(), beacons.end(), beacon) == beacons.end())
        beacons.push_back(beacon);
}

void merge_intervals(std::vector<Interval>& intervals, Interval& range) {
    for (auto it=intervals.begin(); it!=intervals.end(); ) {
        // Interval `range` included in one of the existing intervals
        if (it->beg <= range.beg && range.end <= it->end)
            return;

        // Interval `range` outside of interval
        if (range.end < it->beg - 1 || it->end < range.beg - 1) {
            it++;
            continue;
        }

        // Merge intervals
        range.beg = std::min(range.beg, it->beg);
        range.end = std::max(range.end, it->end);
        it = intervals.erase(it);
    }

    intervals.push_back(range);
}

void limit_intervals(std::vector<Interval>& intervals, Interval const& limit) {
    for (auto it=intervals.begin(); it!=intervals.end(); ) {
        if (it->end < limit.beg || limit.end < it->beg) {
            it = intervals.erase(it);
        } else {
            it->beg = std::max(it->beg, limit.beg);
            it->end = std::min(it->end, limit.end);
            it++;
        }
    }
}

void cover_range(std::vector<Interval>& intervals, Sensor sensor, int target_y) {
    int dx = sensor.radius - std::abs(target_y - sensor.y);

    // Sensor does not reach the line y=target_y
    if (dx < 0)
        return;

    Interval range{sensor.x-dx, sensor.x+dx};
    merge_intervals(intervals, range);
}

void cover_range_diagonally(std::vector<Interval>& intervals, Sensor sensor, int target_z) {
    int dz = sensor.radius - std::abs(target_z - sensor.x - sensor.y);

    if (dz < 0)
        return;

    int w = sensor.x - sensor.y;
    Interval range{w - sensor.radius, w + sensor.radius};
    merge_intervals(intervals, range);
}


int main(int argc, char* argv[])
{
    bool first_half = true;
	long ans = 0;

    std::ifstream myfile("input");
    for (int i=1; i<argc; i++) {
        std::string arg(argv[i]);
        if (arg == "-t" || arg == "--test")
            myfile = std::ifstream("test_input");

        else if (arg == "-s" || arg == "--second")
            first_half = false;
    }

    std::vector<Sensor> sensors;
    std::vector<Beacon> beacons;
	if (myfile.is_open()) {
        std::string input;

        while (getline(myfile, input))
            add_sensor_and_beacon(sensors, beacons, input);

		myfile.close();
	} else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    if (first_half) {
        int target_y = 2000000;
        std::vector<Interval> intervals;
        for (Sensor sensor : sensors)
            cover_range(intervals, sensor, target_y);

        for (Interval interval : intervals)
            ans += interval.end - interval.beg + 1;

        for (Beacon beacon : beacons) {
            if (beacon.y == target_y)
                ans--;
        }
    } else {
        // Look at the grid diagonally (z = x+y)
        int max_coord = 4000000;

        // The only lines the beacon can be on are the lines right before the start
        // of a "square" of sensors or right after the end of one
        std::set<int> sensor_limits;
        for (Sensor sensor : sensors) {
            sensor_limits.insert(sensor.x + sensor.y - sensor.radius - 1);
            sensor_limits.insert(sensor.x + sensor.y + sensor.radius + 1);
        }

        for (int z : sensor_limits) {
            if (!(0 <= z && z <= 2*max_coord))
                continue;

            std::vector<Interval> intervals;
            for (Sensor sensor : sensors)
                cover_range_diagonally(intervals, sensor, z);

            Interval range;
            range.beg = (z <= max_coord ? -z : z - 2 * max_coord);
            range.end = -range.beg;
            limit_intervals(intervals, range);

            if (intervals.size() == 1)
                continue;

            std::sort(intervals.begin(), intervals.end());

            for (size_t i=0; i<intervals.size()-1; i++) {
                if (intervals[i].end == intervals[i+1].beg - 2) {
                    long w = intervals[0].end + 1;
                    long x = (z + w) / 2;
                    long y = (z - w) / 2;
                    ans = x * max_coord + y;
                }
            }
        }

    }

    std::cout << "Answer : " << ans << std::endl;
    return 0;
}
