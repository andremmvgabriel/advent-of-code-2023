#include <algorithm>
#include <iterator>
#include <string>
#include <vector>

#include "../common/utils.hpp"
#include "../common/AoCDay.hpp"

using Sequence = std::vector<long>;
using Sequences = std::vector<Sequence>;
using InputData = Sequences;

class Day09 : public AoCDay<InputData>
{
    InputData ParseInputs(std::ifstream& data) override;
    long PartOne(const InputData& data) const override;
    long PartTwo(const InputData& data) const override;
};

Sequence adjacent_difference_sequence(const Sequence& sequence) {
	Sequence diff_sequence{};
	std::transform(
		sequence.cbegin() + 1, sequence.cend(),
		sequence.cbegin(),
		std::back_inserter(diff_sequence),
		[](long last, long first){ return last - first; }
	);
	return diff_sequence;
}

Sequences recursive_adjacent_difference_sequences(const Sequence& sequence) {
	Sequences diff_sequences{sequence};

	while (std::any_of(
		diff_sequences.back().cbegin(), diff_sequences.back().cend(),
		[](const long val){ return val != 0; })
	) {
		Sequence next_sequence = adjacent_difference_sequence(diff_sequences.back());
		diff_sequences.push_back(next_sequence);
	}

	return diff_sequences;
}

long extrapolate_right(const Sequence& sequence, long value) {
	return sequence.back() + value;
}

long extrapolate_left(const Sequence& sequence, long value) {
	return sequence.back() - value;
}

long recursive_extrapolate_right(const Sequences& sequences) {
	long output{0};

	for (std::size_t i {sequences.size()}; i >= 2; --i) {
		output = sequences[i-2].back() + output;
	}

	return output;
}

long recursive_extrapolate_left(const Sequences& sequences) {
	long output{0};

	for (std::size_t i {sequences.size()}; i >= 2; --i) {
		output = sequences[i-2].front() - output;
	}

	return output;
}

InputData Day09::ParseInputs(std::ifstream& data) {
	// Actual outputs
	InputData sequences{};

	std::string line;
    while(std::getline(data, line)) {
		const auto numbers_txt = string_split(line, " ");

		Sequence sequence{};
		std::transform(
			numbers_txt.cbegin(), numbers_txt.cend(),
			std::back_inserter(sequence),
			[](const std::string& num_txt){ return std::stol(num_txt); }
		);

		sequences.push_back(sequence);
    }

    return sequences;
}

long Day09::PartOne(const InputData& data) const {
	long result{0};

	for (const auto& sequence : data) {
		InputData diff_sequences = recursive_adjacent_difference_sequences(sequence);
		long extrapolated_value = recursive_extrapolate_right(diff_sequences);
		result += extrapolated_value;
	}

	return result;
}

long Day09::PartTwo(const InputData& data) const {
	long result{0};

	for (const auto& sequence : data) {
		InputData diff_sequences = recursive_adjacent_difference_sequences(sequence);
		long extrapolated_value = recursive_extrapolate_left(diff_sequences);
		result += extrapolated_value;
	}

	return result;
}

int main(int argc, char* argv[]) {
	Day09{}.Run(argc, argv);
	return 0;
}
