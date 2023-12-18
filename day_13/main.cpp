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
	std::vector<long> candidates{}; candidates.resize(terrain[0].size() - 1);
	std::iota(candidates.begin(), candidates.end(), 0);

	for (long y{0}; y < terrain.size(); ++y) {
		if (candidates.empty()) { break; }
		std::vector<long> next_candidates{};

		for (const auto& candidate : candidates) {
			long lhs = candidate;
			long rhs = candidate + 1;

			bool is_mirror {true};

			while (lhs >= 0 && rhs < terrain[0].size()) {
				if (terrain[y][lhs] != terrain[y][rhs]) {
					is_mirror = false;
					break;
				}
				--lhs;
				++rhs;
			}

			if (is_mirror) {
				next_candidates.push_back(candidate);
			}
		}

		candidates = next_candidates;
	}

	return (candidates.size() == 1 ? candidates[0] + 1 : 0);
}

long find_horizontal_mirror(const Terrain& terrain) {
	std::vector<long> candidates{}; candidates.resize(terrain.size() - 1);
	std::iota(candidates.begin(), candidates.end(), 0);

	for (long x{0}; x < terrain[0].size(); ++x) {
		if (candidates.empty()) { break; }
		std::vector<long> next_candidates{};

		for (const auto& candidate : candidates) {
			long lhs = candidate;
			long rhs = candidate + 1;

			bool is_mirror {true};

			while (lhs >= 0 && rhs < terrain.size()) {
				if (terrain[lhs][x] != terrain[rhs][x]) {
					is_mirror = false;
					break;
				}
				--lhs;
				++rhs;
			}

			if (is_mirror) {
				next_candidates.push_back(candidate);
			}
		}

		candidates = next_candidates;
	}

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
