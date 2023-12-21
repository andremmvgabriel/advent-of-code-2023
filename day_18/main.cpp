#include <vector>
#include <string>
#include <unordered_map>
#include <set>

#include "../common/utils.hpp"
#include "../common/AoCDay.hpp"

struct Limits
{
	long min{0};
	long max{0};

	long size() const {
		return max - min + 1;
	}
};

struct Trench
{
	long x{0};
	long y{0};
};

struct TrenchMap // Migrate to this
{
	std::set<Trench> data{};
	std::array<Limits, 2> limits{};

	void add_trench(const Trench& trench) {
		data.insert(trench);

		// Updates the limits
		if (trench.x < limits[0].min) { limits[0].min = trench.x; }
		if (trench.x > limits[0].max) { limits[0].max = trench.x; }
		if (trench.y < limits[1].min) { limits[1].min = trench.y; }
		if (trench.y > limits[1].max) { limits[1].max = trench.y; }
	}

	void visual_representation() const {
		std::vector<std::string> visual_map(limits[1].size(), std::string(limits[0].size(), '.'));

		for (const auto& trench : data) {
			const auto y = trench.y - limits[1].min;
			const auto x = trench.x - limits[0].min;
			visual_map[y][x] = '#';
		}

		for (const auto& line : visual_map) {
			std::cout << line << std::endl;
		}
	}
};

bool operator< (const Trench& lhs, const Trench& rhs) {
	return (lhs.y < rhs.y) || (lhs.y == rhs.y && lhs.x < rhs.x);
}

struct Instruction
{
	char direction{'U'};
	long amount{0};
	std::string color{"#000000"};
};

using Instructions = std::vector<Instruction>;
using InputData = Instructions;

class Day18 : public AoCDay<InputData>
{
    InputData ParseInputs(std::ifstream& data) override;
    long PartOne(const InputData& data) const override;
    long PartTwo(const InputData& data) const override;
};

InputData Day18::ParseInputs(std::ifstream& data) {
	// Actual outputs
	InputData instructions{};

	std::string line;
	while(std::getline(data, line)) {
		std::cout << line << std::endl;
		const auto instruction_parts = string_split(line, " ");
		instructions.push_back(Instruction{
			instruction_parts[0][0],
			std::stol(instruction_parts[1]),
			instruction_parts[2].substr(1, 7)
		});
	}

	return instructions;
}

std::unordered_map<char, std::array<int, 2>> direction_increments {
	{ 'U', {0 , -1} },
	{ 'D', {0 ,  1} },
	{ 'L', {-1,  0} },
	{ 'R', { 1,  0} }
};

long Day18::PartOne(const InputData& data) const {
	Trench trench{0, 0};
	std::set<Trench> trench_map{trench};
	std::array<Limits, 2> map_limits{};

	for (const auto& instruction : data) {
		const auto& increment = direction_increments[instruction.direction];
		for (int i{0}; i < instruction.amount; ++i) {
			trench.x += increment[0];
			trench.y += increment[1];
			trench_map.insert(trench);
		}

		if (trench.x < map_limits[0].min) { map_limits[0].min = trench.x; }
		if (trench.x > map_limits[0].max) { map_limits[0].max = trench.x; }
		if (trench.y < map_limits[1].min) { map_limits[1].min = trench.y; }
		if (trench.y > map_limits[1].max) { map_limits[1].max = trench.y; }
	}

	std::cout << "Map limits: x[" << map_limits[0].min << ", " << map_limits[0].max << "]" << std::endl;
	std::cout << "Map limits: y[" << map_limits[1].min << ", " << map_limits[1].max << "]" << std::endl;

	return 1;
}

long Day18::PartTwo(const InputData& data) const {
	return 2;
}

int main(int argc, char* argv[]) {
	Day18{}.Run(argc, argv);
	return 0;
}
