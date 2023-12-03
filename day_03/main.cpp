#include <vector>
#include <string>
#include <numeric>
#include <algorithm>
#include <tuple>

#include "../common/utils.hpp"
#include "../common/AoCDay.hpp"

struct SchemePos {
	std::size_t x{0};
	std::size_t y{0};
};

struct Symbol : SchemePos
{
	std::string text{};
};

struct Number : Symbol
{
	int value{0};
};

using SchemeDims = SchemePos;
using Symbols = std::vector<Symbol>;
using Numbers = std::vector<Number>;
using InputData = std::tuple<SchemeDims, Symbols, Numbers>;

class Day03 : public AoCDay<InputData>
{
    InputData ParseInputs(std::ifstream& data) override;
    long PartOne(const InputData& data) const override;
    long PartTwo(const InputData& data) const override;
};

bool is_digit(char character) {
	return (character >= '0') && (character <= '9');
}

bool is_symbol(char character) {
	return (character != '.') && !is_digit(character);
}

std::tuple<int, int, int, int> create_roi_xy_range(const Symbol& s, const SchemePos& max_dims) {
	const auto x_min = s.x == 0 ? s.x : (s.x - 1);
	const auto x_max = (s.x + s.text.size() - 1) == (max_dims.x - 1) ? (s.x + s.text.size() - 1) : (s.x + s.text.size());

	const auto y_min = s.y == 0 ? s.y : (s.y - 1);
	const auto y_max = s.y == (max_dims.y - 1) ? s.y : (s.y + 1);

	return {x_min, x_max, y_min, y_max};
}

InputData Day03::ParseInputs(std::ifstream& data) {
    // Actual outputs
	SchemeDims dims{};
	Symbols symbols{};
	Numbers numbers{};

    std::string line;
    while(std::getline(data, line)) {
		for (dims.x = 0; dims.x < line.size(); ++dims.x) {
			auto cha = line[dims.x];
			if (is_digit(cha)) {
				Number number{};
				number.text += cha;
				number.x = dims.x;
				number.y = dims.y;
				while(++dims.x < line.size()) {
					cha = line[dims.x];
					if (!is_digit(cha)) { break; }
					number.text += cha;
				}
				number.value = std::stoi(number.text);
				numbers.push_back(number);
			}
			if (is_symbol(cha)) {
				Symbol symbol{};
				symbol.text = cha;
				symbol.x = dims.x;
				symbol.y = dims.y;
				symbols.push_back(symbol);
			}
		}
		++dims.y;
	}

    return {dims, symbols, numbers};
}

long Day03::PartOne(const InputData& data) const {
	const auto& [dims, symbols, numbers] = data;

	Numbers engine_numbers{};
	for (const auto& num : numbers) {
		const auto [x_min, x_max, y_min, y_max] = create_roi_xy_range(num, dims);

		for (const auto& sym : symbols) {
			if (sym.y > y_max) { break; }
			if (sym.y < y_min) { continue; }
			if (sym.x < x_min || sym.x > x_max) { continue; }

			engine_numbers.push_back(num);
			break;
		}
	}

	return std::accumulate(engine_numbers.cbegin(), engine_numbers.cend(), 0, [](int result, const Number& num){
		return result + num.value;
	});
}

long Day03::PartTwo(const InputData& data) const {
	const auto& [dims, symbols, numbers] = data;

	std::vector<Numbers> gears_candidates{};
	for (const auto& sym : symbols) {
		if (sym.text != "*") { continue; }

		const auto [x_min, x_max, y_min, y_max] = create_roi_xy_range(sym, dims);

		Numbers candidates{};

		for(const auto& num : numbers) {
			if (num.y > y_max) { break; }
			if (num.y < y_min) { continue; }
			if (num.x + num.text.size() - 1 < x_min || num.x > x_max) { continue; }

			candidates.push_back(num);
		}

		gears_candidates.push_back(candidates);
	}

	std::vector<Numbers> gears{};
	std::copy_if(
		gears_candidates.cbegin(),
		gears_candidates.cend(),
		std::back_inserter(gears),
		[](const Numbers& numbers){ return numbers.size() > 1; }
	);

	return std::accumulate(gears.cbegin(), gears.cend(), 0, [](int result, const Numbers& nums){
		return result + std::accumulate(nums.cbegin(), nums.cend(), 1, [](int result, const Number& num){return result * num.value;});
	});
}

int main(int argc, char* argv[]) {
	Day03{}.Run(argc, argv);
	return 0;
}
