#include <algorithm>
#include <iterator>
#include <numeric>
#include <vector>
#include <string>
#include <array>

#include "../common/utils.hpp"
#include "../common/AoCDay.hpp"

class Sequence
{
public:
	std::string data{};
	std::string label{};
	char operation{};
	long value{0};

	Sequence() = default;
	Sequence(const std::string& sequence) : data{sequence} {
		const auto pos = sequence.find_first_of("=-");
		label = data.substr(0, pos);
		operation = data[pos];
		if (operation == '=') {
			value = std::stol(data.substr(pos + 1));
		}
	};
};

using Sequences = std::vector<Sequence>;
using BoxManager = std::array<Sequences, 256>;
using InputData = Sequences;

class Day15 : public AoCDay<InputData>
{
    InputData ParseInputs(std::ifstream& data) override;
    long PartOne(const InputData& data) const override;
    long PartTwo(const InputData& data) const override;
};

long hash_algorithm(const std::string& word) {
	return std::accumulate(
		word.cbegin(), word.cend(), 0L,
		[](long result, const char& c){
			return ((result + c) * 17) % 256;
		}
	);
}

void operation_minus(BoxManager& boxes, const Sequence& sequence) {
	// Safety check
	if (sequence.operation != '-') { return; }

	// Box index
	const auto box = hash_algorithm(sequence.label);

	for (int i = 0; i < boxes[box].size(); ++i) {
		if (boxes[box][i].label == sequence.label) {
			boxes[box].erase(boxes[box].begin() + i);
			break;
		}
	}
}

void operation_equal(BoxManager& boxes, const Sequence& sequence) {
	// Safety check
	if (sequence.operation != '=') { return; }

	// Box index
	const auto box = hash_algorithm(sequence.label);

	bool exists{false};

	for (int i = 0; i < boxes[box].size(); ++i) {
		if (boxes[box][i].label == sequence.label) {
			boxes[box][i] = sequence;
			exists = true;
			break;
		}
	}

	if (!exists) {
		boxes[box].push_back(sequence);
	}
}

long calculate_focusing_power(const BoxManager& boxes) {
	long result {0};

	for (int i{1}; i <= boxes.size(); ++i) {
		for (int j{1}; j <= boxes[i-1].size(); ++j) {
			result += i * j * boxes[i-1][j-1].value;
		}
	}

	return result;
}

InputData Day15::ParseInputs(std::ifstream& data) {
	// Read the only line in the file
	std::string line;
	std::getline(data, line);

	// Parse the line
	InputData sequences{};
	for (const auto& sequence : string_split(line, ",")) {
		sequences.emplace_back(sequence);
	}

	return sequences;
}

long Day15::PartOne(const InputData& data) const {
	return std::accumulate(
		data.cbegin(), data.cend(), 0L,
		[](long result, const Sequence& sequence){
			return result + hash_algorithm(sequence.data);
		}
	);
}

long Day15::PartTwo(const InputData& data) const
{
	BoxManager boxes{};

	for (const auto& sequence : data) {
		if (sequence.operation == '-') {
			operation_minus(boxes, sequence);
		} else {
			operation_equal(boxes, sequence);
		}
	}

	return calculate_focusing_power(boxes);
}

int main(int argc, char* argv[]) {
	Day15{}.Run(argc, argv);
	return 0;
}
