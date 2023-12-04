#include <numeric>
#include <set>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <cmath>
#include <unordered_map>

#include "../common/utils.hpp"
#include "../common/AoCDay.hpp"

struct Card
{
	std::set<int> numbers{};
	std::set<int> winning_numbers{};
};

using InputData = std::vector<Card>;

class Day04 : public AoCDay<InputData>
{
    InputData ParseInputs(std::ifstream& data) override;
    long PartOne(const InputData& data) const override;
    long PartTwo(const InputData& data) const override;
};

InputData Day04::ParseInputs(std::ifstream& data) {
    // Actual outputs
    std::vector<Card> cards{};

    std::string line;
    while(std::getline(data, line)) {
        // [0] -> Card number
        // [1] -> Scratches info
        const auto card_info = string_split(line, ": ");

        // [0] -> Numbers
        // [1] -> Winning Numbers
        const auto scratches = string_split(card_info[1], " | ");

        // Calculate how many numbers and winning numbers the card involves
        const auto n_numbers = (scratches[0].size() + 1) / 3;
        const auto n_winning_numbers = (scratches[1].size() + 1) / 3;

        Card card{};

        for (int i{0}; i < n_numbers; ++i)
        {
            std::string num = scratches[0].substr(0 + i*3, 2);
            std::replace(num.begin(), num.end(), ' ', '0');
            card.numbers.insert(std::stoi(num));
        }

        for (int i{0}; i < n_winning_numbers; ++i)
        {
            std::string num = scratches[1].substr(0 + i*3, 2);
            std::replace(num.begin(), num.end(), ' ', '0');
            card.winning_numbers.insert(std::stoi(num));
        }

        cards.push_back(card);
    }

    return cards;
}

long Day04::PartOne(const InputData& data) const {
    long result{0};

    for (const auto& card : data) {
        std::vector<int> common_numbers;

        std::set_intersection(
            card.numbers.cbegin(), card.numbers.cend(),
            card.winning_numbers.cbegin(), card.winning_numbers.cend(),
            std::back_inserter(common_numbers)
        );

        if (!common_numbers.empty()) {
            result += std::pow(2, common_numbers.size() - 1);
        }
    }

	return result;
}

long Day04::PartTwo(const InputData& data) const {
    std::unordered_map<int, int> cards_instances;

    for (int i{0}; i < data.size(); ++i) {
        const auto& card = data[i];

        std::vector<int> common_numbers;

        std::set_intersection(
            card.numbers.cbegin(), card.numbers.cend(),
            card.winning_numbers.cbegin(), card.winning_numbers.cend(),
            std::back_inserter(common_numbers)
        );

        ++cards_instances[i]; // Increase the number of card instances of the current card
        for (int j{1}; j <= common_numbers.size() && i + j < data.size(); ++j) {
            cards_instances[i + j] += cards_instances[i];
        }
    }

	return std::accumulate(cards_instances.cbegin(), cards_instances.cend(), 0,
        [](int result, const auto& card_result){
            return result + card_result.second;
        }
    );
}

int main(int argc, char* argv[]) {
	Day04{}.Run(argc, argv);
	return 0;
}
