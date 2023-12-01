#include <vector>
#include <array>
#include <string>
#include <algorithm>
#include <map>

#include "../common/utils.hpp"
#include "../common/AoCDay.hpp"

using InputData = std::vector<std::string>;

class Day01 : public AoCDay<InputData>
{
    InputData ParseInputs(std::ifstream& data) override;
    long PartOne(const InputData& data) const override;
    long PartTwo(const InputData& data) const override;
};

InputData Day01::ParseInputs(std::ifstream& data) {
    // Actual outputs
    std::vector<std::string> lines {};

    std::string line;
    while(std::getline(data, line)) {
        lines.push_back(line);
    }

    return lines;
}

long Day01::PartOne(const InputData& data) const {
	long result{0};

	for (const auto line : data) {
		// Find the first and last occurence of a number
		const auto pos1 = line.find_first_of("0123456789");
		const auto pos2 = line.find_last_of("0123456789");

		// Both must be found
		if (pos1 == -1 || pos2 == -1) { continue; }

		// Creates a string from the two characters
		std::string value{line[pos1], line[pos2]};

		// Add result to the total
		result += std::stoi(value);
	}

	return result;
}

long Day01::PartTwo(const InputData& data) const {
	std::map<std::string, int> text_to_digit {
		{"zero", 0},
		{"one", 1},
		{"two", 2},
		{"three", 3},
		{"four", 4},
		{"five", 5},
		{"six", 6},
		{"seven", 7},
		{"eight", 8},
		{"nine", 9}
	};

	long result{0};

	for (const auto line : data) {
		// Arrays containing the left and right digit candidates, respectively
		std::array<int, 10> l_digit_idxs{}; l_digit_idxs.fill(9999);
		std::array<int, 10> r_digit_idxs{}; r_digit_idxs.fill(-1);

		// First step: find all the spelled numbers
		for (const auto [text, idx] : text_to_digit) {
			const auto pos1_text = static_cast<long>(line.find(text)); // Find first occurence
			const auto pos2_text = static_cast<long>(line.rfind(text)); // Find last occurence

			if (pos1_text != -1) {
				l_digit_idxs[idx] = pos1_text;
			}

			if (pos2_text != -1) {
				r_digit_idxs[idx] = pos2_text;
			}
		}

		// Second step: find the first and last occurence of a number
		const auto pos1_digit = static_cast<long>(line.find_first_of("0123456789"));
		const auto pos2_digit = static_cast<long>(line.find_last_of("0123456789"));

		if (pos1_digit != -1) {
			const auto num_idx = line[pos1_digit] - '0';
			// Only updates the index if the occurence is located before in the line
			if (l_digit_idxs[num_idx] > pos1_digit) {
				l_digit_idxs[num_idx] = pos1_digit;
			}
		}

		if (pos2_digit != -1) {
			const auto num_idx = line[pos2_digit] - '0';
			// Only updates the index if the occurence is located after in the line
			if (r_digit_idxs[num_idx] < pos2_digit) {
				r_digit_idxs[num_idx] = pos2_digit;
			}
		}

		// Get the lowest number occurence position in the line
		const auto l_pos = std::min_element(l_digit_idxs.cbegin(), l_digit_idxs.cend());
		const auto r_pos = std::max_element(r_digit_idxs.cbegin(), r_digit_idxs.cend());

		// From the occurence position, get the actual number it is assigned to
		const auto l_value = std::distance(l_digit_idxs.cbegin(), l_pos);
		const auto r_value = std::distance(r_digit_idxs.cbegin(), r_pos);

		// Add result to the total
		result += l_value * 10 + r_value;
	}

	return result;
}

int main(int argc, char* argv[]) {
	Day01{}.Run(argc, argv);
	return 0;
}
