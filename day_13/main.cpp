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

#include <set>

struct Point {
	long x{-1};
	long y{-1};
};

bool is_point_valid(const Point& point) {
	return (point.x >= 0 && point.y >=0);
}

bool operator<(const Point& lhs, const Point& rhs){
	return (lhs.x < rhs.x) || (lhs.x == rhs.x && lhs.y < rhs.y);
}

Point find_vertical_smudge_point(const Terrain& terrain) {
	std::vector<std::set<Point>> candidates{}; candidates.resize(terrain[0].size() - 1, {});

	for (long y{0}; y < terrain.size(); ++y) {
		for (long x{0}; x < terrain[0].size() - 1; ++x) {
			long lhs = x;
			long rhs = x + 1;

			while (lhs >= 0 && rhs < terrain.size()) {
				if (terrain[y][lhs] != terrain[y][rhs]) {
					candidates[x].insert({terrain[lhs][x] == '.' ? lhs : rhs, y});
				}
				--lhs;
				++rhs;
			}
		}
	}

	for (int y{0}; y < candidates.size(); ++y) {
		std::cout << "[y] " << y << ": " << candidates[y].size() << std::endl;
	}

	for (const auto candidate : candidates) {
		if (candidate.size() == 1) {
			return *candidate.begin();
		}
	}

	return {};
}

Point find_horizontal_smudge_point(const Terrain& terrain) {
	std::vector<std::set<Point>> candidates{}; candidates.resize(terrain.size() - 1, {});

	for (long x{0}; x < terrain[0].size(); ++x) {
		for (long y{0}; y < terrain.size() - 1; ++y) {
			long lhs = y;
			long rhs = y + 1;

			while (lhs >= 0 && rhs < terrain.size()) {
				if (terrain[lhs][x] != terrain[rhs][x]) {
					candidates[y].insert({x, terrain[lhs][x] == '.' ? lhs : rhs});
				}
				--lhs;
				++rhs;
			}
		}
	}

	for (int x{0}; x < candidates.size(); ++x) {
		std::cout << "[x] " << x << ": " << candidates[x].size() << std::endl;
		// for (auto c:candidates[x]) {
			// std::cout << "  > " << c.x << " " << c.y << std::endl;
		// }
	}

	for (const auto candidate : candidates) {
		if (candidate.size() == 1) {
			return *candidate.begin();
		}
	}

	return {};
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
	Terrains terrains = data;

	std::vector<std::array<long, 2>> tmp{};

	for (auto& terrain : terrains) {
		const auto v_mirror_idx = find_vertical_mirror(terrain);
		const auto h_mirror_idx = find_horizontal_mirror(terrain);
		tmp.push_back({v_mirror_idx, h_mirror_idx});
	}

	for (auto& terrain : terrains) {
		auto tmp1 = find_vertical_smudge_point(terrain);
		if (is_point_valid(tmp1)) {
			terrain[tmp1.y][tmp1.x] = '#';
			std::cout << "> Smudge found vertically" << std::endl;
		} std::cout << std::endl;
		auto tmp2 = find_horizontal_smudge_point(terrain);
		if (is_point_valid(tmp2)) {
			terrain[tmp2.y][tmp2.x] = '#';
			std::cout << "> Smudge found horizontally" << std::endl;
		}
		std::cout << "-------------------------------------" << std::endl;
	}

	for (int t{0}; t < terrains.size(); ++t) {
		const auto& terrain = terrains[t];
		const auto v_mirror_idx = find_vertical_mirror(terrain);
		const auto h_mirror_idx = find_horizontal_mirror(terrain);
		// if (tmp[t][0] == v_mirror_idx && tmp[t][1] == h_mirror_idx) { continue; }
		// std::cout << tmp[t][0] << " " << tmp[t][1] << " -> ";
		if (tmp[t][0] == v_mirror_idx) {
			tmp[t][0] = 0;
			// std::cout << "No new Vertical"
		} else {
			tmp[t][0] = v_mirror_idx;
		}
		if (tmp[t][1] == h_mirror_idx) {
			tmp[t][1] = 0;
		} else {
			tmp[t][1] = h_mirror_idx;
		}
		// std::cout << tmp[t][0] << " " << tmp[t][1] << std::endl;
	}
	
	
	return std::accumulate(
		tmp.cbegin(), tmp.cend(), 0L,
		[](long result, const std::array<long, 2>& mirrors){
			return result + mirrors[0] + 100 * mirrors[1];
		}
	);
}

int main(int argc, char* argv[]) {
	Day12{}.Run(argc, argv);
	return 0;
}
