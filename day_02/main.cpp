#include <vector>
#include <unordered_map>
#include <algorithm>

#include "../common/utils.hpp"
#include "../common/AoCDay.hpp"

using Draw = std::array<int, 3>; // red - green - blue
using Game = std::vector<Draw>;
using InputData = std::vector<Game>;

class Day02 : public AoCDay<InputData>
{
    InputData ParseInputs(std::ifstream& data) override;
    long PartOne(const InputData& data) const override;
    long PartTwo(const InputData& data) const override;
};

const std::unordered_map<std::string, int> ColorMapping {
	{"red", 0}, {"green", 1}, {"blue", 2}
};

bool operator <= (const Draw& lhs, const Draw& rhs){
	return std::equal(
		lhs.cbegin(), lhs.cend(),
		rhs.cbegin(), rhs.cend(),
		[](const int lval, const int rval){
			return lval <= rval;
		}
	);
}

bool is_impossible_game(const Game& game) {
	static const Draw max_draw{12, 13, 14};
	for (const auto& draw : game) {
		if (!(draw <= max_draw)) { return true; }
	}

	return false;
}

Draw find_minimum_set(const Game& game) {
	Draw minimum_set{0, 0, 0};

	for (const auto& draw : game) {
		minimum_set[0] = std::max(minimum_set[0], draw[0]);
		minimum_set[1] = std::max(minimum_set[1], draw[1]);
		minimum_set[2] = std::max(minimum_set[2], draw[2]);
	}

	return minimum_set;
}

InputData Day02::ParseInputs(std::ifstream& data) {
    // Actual outputs
    std::vector<Game> games {};

    std::string line;
    while(std::getline(data, line)) {
		std::vector<std::string> game_info = string_split(line, ": ");
		std::vector<std::string> game_draws = string_split(game_info[1], "; ");
		Game game{};

		for (const auto draw : game_draws) {
			std::vector<std::string> draw_info = string_split(draw, ", ");
			Draw draw_data{};

			for (const auto cubes : draw_info) {
				std::vector<std::string> color_info = string_split(cubes, " ");
				draw_data[ColorMapping.at(color_info[1])] = std::stoi(color_info[0]);
			}

			game.push_back(draw_data);
		}

		games.push_back(game);
    }

    return games;
}

long Day02::PartOne(const InputData& data) const {
	long result{0};

	for (int i = 0; i < data.size(); ++i) {
		if (is_impossible_game(data[i])) {
			continue;
		}

		result += i + 1;
	}

	return result;
}

long Day02::PartTwo(const InputData& data) const {
	long result{0};

	for (const auto game : data) {
		const auto min_set = find_minimum_set(game);
		result += min_set[0] * min_set[1] * min_set[2];
	}

	return result;
}

int main(int argc, char* argv[]) {
	Day02{}.Run(argc, argv);
	return 0;
}
