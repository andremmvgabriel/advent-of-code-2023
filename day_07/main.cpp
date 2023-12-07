#include <numeric>
#include <set>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <algorithm>

#include "../common/utils.hpp"
#include "../common/AoCDay.hpp"

enum class HandStrength : int {
	HightHand = 0x00,
	OnePair,
	DoublePair,
	Triple,
	FullHouse,
	FourKind,
	FiveKind
};

struct Hand {
	std::string cards{};
	int bid{0};
};

struct HandAux {
	HandStrength weight{};
	std::string cards_remap{};
	std::string cards_joker_best{};
};

using HandInfo = std::pair<Hand, HandAux>;
using InputData = std::vector<Hand>;

class Day07 : public AoCDay<InputData>
{
    InputData ParseInputs(std::ifstream& data) override;
    long PartOne(const InputData& data) const override;
    long PartTwo(const InputData& data) const override;
};

bool operator < (const HandInfo& lhs, const HandInfo& rhs) {
	return (lhs.second.weight < rhs.second.weight) || ((lhs.second.weight == rhs.second.weight) && (lhs.second.cards_remap < rhs.second.cards_remap));
}

std::string remap_cards(std::string cards, const std::unordered_map<char, char>& map) {
	std::transform(cards.begin(), cards.end(), cards.begin(), [&map](char card){ return map.at(card); });
	return cards;
}

std::string best_scenario_cards(std::string cards, const std::unordered_map<char, char>& map) {
	std::map<char, int> cards_counter{{'1', 0}};

	for (const char& card : cards) {
		++cards_counter[card];
	}

	// No J's
	if (cards_counter['1'] == 0) { return cards; }

	// All J's
	if (cards_counter['1'] == 5) { return remap_cards("AAAAA", map); }

	std::pair<char, int> best_candidate{'2', 0};
	for (auto counter = cards_counter.cbegin(); counter != cards_counter.cend(); ++counter) {
		if (counter->first == '1') { continue; }
		if (counter->second >= best_candidate.second) {
			best_candidate = *counter;
		}
	}

	std::transform(
		cards.begin(), cards.end(),
		cards.begin(),
		[&best_candidate](char card){ return card == '1' ? best_candidate.first : card; }
	);

	return cards;
}

HandStrength find_hand_strength(const std::string& cards) {
	std::unordered_map<char, int> cards_counter{};
	for (const auto& card : cards) {
		++cards_counter[card];
	}

	if (cards_counter.size() == 1) {
		return HandStrength::FiveKind;
	}

	if (cards_counter.size() == 4) {
		return HandStrength::OnePair;
	}

	if (cards_counter.size() == 2) {
		if (cards_counter.begin()->second == 4 || cards_counter.begin()->second == 1) {
			return HandStrength::FourKind;
		}
		return HandStrength::FullHouse;
	}

	for (const auto& count : cards_counter) {
		if (count.second == 2) {
			return HandStrength::DoublePair;
		}
		if (count.second == 3) {
			return HandStrength::Triple;
		}
	}

	return HandStrength::HightHand;
}

InputData Day07::ParseInputs(std::ifstream& data) {
    // Actual outputs
    InputData hands {};

    std::string line;
    while(std::getline(data, line)) {
		std::vector<std::string> hand_info = string_split(line, " ");

		hands.push_back({
			hand_info[0],
			std::stoi(hand_info[1])
		});
    }

    return hands;
}

long Day07::PartOne(const InputData& data) const {
	const std::unordered_map<char, char> card_remap {
		{'2', '2'}, {'3', '3'}, {'4', '4'},
		{'5', '5'}, {'6', '6'}, {'7', '7'},
		{'8', '8'}, {'9', '9'}, {'T', 'A'},
		{'J', 'B'}, {'Q', 'C'}, {'K', 'D'},
		{'A', 'E'}
	};

	std::set<HandInfo> hands_sorted;

	for (auto& hand : data) {
		const std::string remapped_cards = remap_cards(hand.cards, card_remap);
		const HandStrength weight = find_hand_strength(hand.cards);
		const HandAux hand_info{weight, remapped_cards, remapped_cards};
		hands_sorted.insert(std::make_pair(hand, hand_info));
	}

	return std::accumulate(
		hands_sorted.cbegin(), hands_sorted.cend(), 0L,
		[](long result, const HandInfo& hand_info){
			static long counter{0};
			return result + hand_info.first.bid * ++counter;
		}
	);
}

long Day07::PartTwo(const InputData& data) const {
	const std::unordered_map<char, char> card_remap {
		{'2', '2'}, {'3', '3'}, {'4', '4'},
		{'5', '5'}, {'6', '6'}, {'7', '7'},
		{'8', '8'}, {'9', '9'}, {'T', 'A'},
		{'J', '1'}, {'Q', 'C'}, {'K', 'D'},
		{'A', 'E'}
	};

	std::set<HandInfo> hands_sorted;

	for (auto& hand : data) {
		const std::string remapped_cards = remap_cards(hand.cards, card_remap);
		const std::string best_cards = best_scenario_cards(remapped_cards, card_remap);
		const HandStrength weight = find_hand_strength(best_cards);
		const HandAux hand_info{weight, remapped_cards, best_cards};
		hands_sorted.insert(std::make_pair(hand, hand_info));
	}

	return std::accumulate(
		hands_sorted.cbegin(), hands_sorted.cend(), 0L,
		[](long result, const HandInfo& hand_info){
			static long counter{0};
			return result + hand_info.first.bid * ++counter;
		}
	);
}

int main(int argc, char* argv[]) {
	Day07{}.Run(argc, argv);
	return 0;
}
