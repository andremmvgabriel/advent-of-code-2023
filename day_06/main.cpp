#include <string>
#include <vector>
#include <tuple>
#include <cmath>
#include <algorithm>
#include <numeric>

#include "../common/utils.hpp"
#include "../common/AoCDay.hpp"

struct Record {
	long time{0};
	long distance{0};
};

using Records = std::vector<Record>;
using InputData = std::tuple<Records, Record>;

class Day06 : public AoCDay<InputData>
{
    InputData ParseInputs(std::ifstream& data) override;
    long PartOne(const InputData& data) const override;
    long PartTwo(const InputData& data) const override;
};

// Logic behind method
// > Condition to satisfy a record break:
//    record < ( (time total) - (time i) ) * (velocity i)
// > Both (time i) and (velocity i) assume the same values, for all 'i'
//    (time i) = (velocity i)
// > Solving the first equation for (time i)
//    (time threshold initial) < (time i) < (time threshold final)
//    Where:
//       (time threshold initial) = ( (time total) - sqrt( (time total)^2 - 4*record ) ) / 2
//       (time threshold final)   = ( (time total) + sqrt( (time total)^2 - 4*record ) ) / 2
// > We only care about (time threshold initial) since this is mirrored distribution
long beatable_records(const Record& record) {
	// Values that will be used in the equation
	const long time_power = std::pow(record.time, 2);
	const long value_for_sqrt = time_power - 4 * record.distance;

	// If the value for the sqrt is negative => record impossible to break
	if (value_for_sqrt <= 0) { return 0; }

	// Time threshold
	long time_th = std::ceil((record.time - std::sqrt(value_for_sqrt) ) / 2.0);

	// If the time threshold is exactly the same distance record, increase to next tick
	long time_th_distance = (record.time - time_th) * time_th;
	if (time_th_distance == record.distance) { time_th++; }

	return record.time + 1 - time_th * 2;
}

InputData Day06::ParseInputs(std::ifstream& data) {
    // Outputs for Part I
	Records records{};

    std::string line;

	// Read times
	std::string concatenated_time{};
	if (std::getline(data, line)) {
		auto line_numbers = line.substr(9);
		auto times = string_split(line_numbers, " ");
		std::erase_if(times, [](const std::string& text){return text.empty();});

		for (const auto& time : times) {
			Record record{ std::stoi(time), 0 };
			records.push_back(record);
		}

		std::erase_if(line_numbers, [](const char& c){return c == ' ';});
		concatenated_time = line_numbers;
	}

	// Read distances
	std::string concatenated_distance{};
	if (std::getline(data, line)) {
		auto line_distances = line.substr(9);
		auto distances = string_split(line_distances, " ");
		std::erase_if(distances, [](const std::string& text){return text.empty();});

		for (int i{0}; i < distances.size(); ++i) {
			records[i].distance = std::stoi(distances[i]);
		}

		std::erase_if(line_distances, [](const char& c){return c == ' ';});
		concatenated_distance = line_distances;
	}

	// Outputs for Part II
	Record record{
		std::stol(concatenated_time),
		std::stol(concatenated_distance)
	};

    return {records, record};
}

long Day06::PartOne(const InputData& data) const {
	const auto& [records, _] = data;
	return std::accumulate(
		records.cbegin(), records.cend(), 1,
		[](long result, const Record& record){ return result * beatable_records(record); }
	);
}

long Day06::PartTwo(const InputData& data) const {
	const auto& [_, record] = data;
	return beatable_records(record);
}

int main(int argc, char* argv[]) {
	Day06{}.Run(argc, argv);
	return 0;
}
