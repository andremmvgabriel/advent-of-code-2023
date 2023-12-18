#include <algorithm>
#include <numeric>
#include <vector>
#include <string>

#include "../common/utils.hpp"
#include "../common/AoCDay.hpp"

using Terrain = std::vector<std::string>;
using Terrains = std::vector<Terrain>;
using InputData = Terrains;

class Day12 : public AoCDay<InputData>
{
    InputData ParseInputs(std::ifstream& data) override;
    long PartOne(const InputData& data) const override;
    long PartTwo(const InputData& data) const override;
};

InputData Day12::ParseInputs(std::ifstream& data) {
	// Actual outputs
	Terrains terrains{};

	std::string line;
	Terrain terrain{};
	while(std::getline(data, line)) {
		if (line.empty()) {
			terrains.push_back(terrain);
			terrain.clear();
			continue;
		}
		terrain.push_back(line);
	}
	terrains.push_back(terrain);

	return terrains;
}

long find_vertical_mirror(const Terrain& terrain) {
	std::vector<long> candidates{}; candidates.resize(terrain[0].size() - 2);
	std::iota(candidates.begin(), candidates.end(), 1);

	long y{0};
	while (candidates.size() > 1) {
		std::vector<long> next_candidates{};
		for (const auto& candidate : candidates) {
			const auto dist = std::min(candidate, static_cast<long>(terrain[0].size()) - 1 - candidate);
			std::cout << candidate << " " << static_cast<long>(terrain[0].size()) - 1 - candidate << " -> " << dist << std::endl;

			bool mirror {true};
			for (long x{0}; x < dist; ++x) {
				const auto lhs = candidate - x;
				const auto rhs = candidate + 1 + x;

				if (terrain[y][lhs] != terrain[y][rhs]) {
					mirror = false;
					break;
				}
			}
			if (mirror) {
				next_candidates.push_back(candidate);
			}
		}
		candidates = next_candidates;
		++y;
	}

	std::cout << (candidates.size() == 1 ? candidates[0] + 1 : 0) << " = " << (candidates.size() == 1 ? candidates[0] + 1 : 0) << std::endl;

	return (candidates.size() == 1 ? candidates[0] + 1 : 0);
}

long find_horizontal_mirror(const Terrain& terrain) {
	std::vector<long> candidates{}; candidates.resize(terrain.size() - 2);
	std::iota(candidates.begin(), candidates.end(), 1);

	// long x{0};
	// while (candidates.size() > 1) {
	// 	std::vector<long> next_candidates{};
	// 	for (const auto& candidate : candidates) {
	// 		const auto dist = std::min(candidate, static_cast<long>(terrain.size()) - 2 - candidate);
	// 		std::cout << candidate << " " << static_cast<long>(terrain.size()) - 2 - candidate << " -> " << dist << std::endl;

	// 		bool mirror {true};
	// 		for (long y{0}; y <= dist; ++y) {
	// 			const auto lhs = candidate - y;
	// 			const auto rhs = candidate + 1 + y;
	// 			std::cout << "  > " << lhs << " (" << terrain[lhs][x] << ") " << rhs << " (" << terrain[rhs][x] << ") " << std::endl;

	// 			if (terrain[lhs][x] != terrain[rhs][x]) {
	// 				std::cout << "Not equal" << std::endl;
	// 				mirror = false;
	// 				break;
	// 			}
	// 		}
	// 		if (mirror) {
	// 			next_candidates.push_back(candidate);
	// 		}
	// 	}
	// 	candidates = next_candidates;
	// 	++x;
	// }

	// if (candidates.empty()) {
	// 	return 0;
	// }

	// // TODO: Merge two for cycles. Remove While() and iterate through all the x's. Break if no candidates are available.

	// const auto dist = std::min(candidates[0], static_cast<long>(terrain.size()) - 2 - candidates[0]);
	// for (; x < terrain[0].size(); ++x) {
	// 	bool mirror {true};
	// 	for (long y{0}; y <= dist; ++y) {
	// 		const auto lhs = candidates[0] - y;
	// 		const auto rhs = candidates[0] + 1 + y;
	// 		std::cout << "  > " << lhs << " (" << terrain[lhs][x] << ") " << rhs << " (" << terrain[rhs][x] << ") " << std::endl;

	// 		if (terrain[lhs][x] != terrain[rhs][x]) {
	// 			std::cout << "Not equal" << std::endl;
	// 			mirror = false;
	// 			break;
	// 		}
	// 	}
	// 	if (!mirror) {
	// 		candidates.clear();
	// 	}
	// }

	for (long x{0}; x < terrain[0].size(); ++x) {
		if (candidates.empty()) { break; }

		std::vector<long> next_candidates{};

		for (const auto& candidate : candidates) {
			bool mirror{true};
			for (long y{0}; y < candidates.size(); ++y) {
				const auto lhs = candidate - y;
				const auto rhs = candidate + 1 + y;

				std::cout << "  > " << lhs << " (" << terrain[lhs][x] << ") " << rhs << " (" << terrain[rhs][x] << ") " << std::endl;

				if (lhs < 0) { break; }
				if (rhs >= candidates.size()) { break; }

				if (terrain[lhs][x] != terrain[rhs][x]) {
					std::cout << "Not equal" << std::endl;
					mirror = false;
					break;
				}
			}
			if (mirror) {
				std::cout << "Equal" << std::endl;
				next_candidates.push_back(candidate);
			}
		}

		candidates = next_candidates;
	}

	std::cout << "Candidates: " << candidates.size() << std::endl;

	return (candidates.size() == 1 ? candidates[0] + 1 : 0);
}

long Day12::PartOne(const InputData& data) const {
	return std::accumulate(
		data.cbegin(), data.cend(), 0L,
		[](long result, const Terrain& terrain){
			const auto v_mirror_idx = find_vertical_mirror(terrain);
			const auto h_mirror_idx = find_horizontal_mirror(terrain);
			return result + v_mirror_idx + 100 * h_mirror_idx;
		}
	);
}

long Day12::PartTwo(const InputData& data) const {
	return 2;
}

int main(int argc, char* argv[]) {
	Day12{}.Run(argc, argv);
	return 0;
}
