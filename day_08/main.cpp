#include <iterator>
#include <numeric>
#include <string>
#include <fstream>
#include <tuple>
#include <unordered_map>
#include <algorithm>

#include "../common/utils.hpp"
#include "../common/AoCDay.hpp"

using Choice = std::array<std::string, 2>;
using Map = std::unordered_map<std::string, Choice>;
using InputData = std::tuple<std::string, Map>;

class Day08 : public AoCDay<InputData>
{
    InputData ParseInputs(std::ifstream& data) override;
    long PartOne(const InputData& data) const override;
    long PartTwo(const InputData& data) const override;
};

template<typename Condition>
long find_steps_to_stop(const Map& map, const std::string& pattern, const std::string& initial_pos, Condition stop_condition) {
	// Check if start position exists
	if (map.find(initial_pos) == map.cend()) { return 0; }

	// Relevant variables for the algorithm
	long steps{0};
	std::string position{initial_pos};

	// Find algorithm
	while (true) {
		// Current pattern step
		const auto& step = pattern[steps++ % pattern.size()];

		// Updates the position
		position = map.at(position)[step == 'L' ? 0 : 1];

		// Check the stop condition
		if (stop_condition(position)) { break; }
	}

	return steps;
}

template<typename Condition>
std::vector<std::string> find_start_positions(const Map& map, Condition found_condition) {
	std::vector<std::string> start_positions{};

	for (const auto& option : map) {
		if (found_condition(option.first)) {
			start_positions.push_back(option.first);
		}
	}

	return start_positions;
}

InputData Day08::ParseInputs(std::ifstream& data) {
    // Actual outputs
	std::string pattern{};
    Map map {};

	// First line is the moving pattern
	std::getline(data, pattern);

	std::string line;
	std::getline(data, line); // Remove empty line
    while(std::getline(data, line)) {
		const auto from = line.substr(0, 3);
		const auto left = line.substr(7, 3);
		const auto right = line.substr(12, 3);

		map[from] = Choice{left, right};
    }

    return {pattern, map};
}

long Day08::PartOne(const InputData& data) const {
	const auto& [pattern, map] = data;

	// Execution conditions
	const std::string start_position = "AAA";
	const auto stop_condition = [](const std::string& position){ return position == "ZZZ"; };

	return find_steps_to_stop(map, pattern, start_position, stop_condition);
}

long Day08::PartTwo(const InputData& data) const {
	const auto& [pattern, map] = data;

	// Execution conditions
	const auto found_condition = [](const std::string& position){ return position[2] == 'A'; };
	const std::vector<std::string> start_positions = find_start_positions(map, found_condition);
	const auto stop_condition = [](const std::string& position){ return position[2] == 'Z'; };

	return std::accumulate(
		start_positions.cbegin(), start_positions.cend(), 1L,
		[&](long result, const std::string& position){
			long steps = find_steps_to_stop(map, pattern, position, stop_condition);
			return std::lcm(result, steps);
		}
	);
}

int main(int argc, char* argv[]) {
	Day08{}.Run(argc, argv);
	return 0;
}
