#include <algorithm>
#include <vector>
#include <string>
#include <array>

#include "../common/utils.hpp"
#include "../common/AoCDay.hpp"

using Platform = std::vector<std::string>;
using InputData = Platform;

class Day14 : public AoCDay<InputData>
{
    InputData ParseInputs(std::ifstream& data) override;
    long PartOne(const InputData& data) const override;
    long PartTwo(const InputData& data) const override;
};

void TiltNorth(Platform& platform) {
	for (long y{1}; y < platform.size(); ++y) {
		for (long x{0}; x < platform[y].size(); ++x) {
			auto& symbol = platform[y][x];

			if (symbol == 'O') {
				long y_s{y-1};
				while(y_s >= 0 && platform[y_s][x] != 'O' && platform[y_s][x] != '#') {
					--y_s;
				}

				symbol = '.';
				platform[y_s+1][x] = 'O';
			}
		}
	}
}

void TiltWest(Platform& platform) {
	for (long x{1}; x < platform[0].size(); ++x) {
		for (long y{0}; y < platform.size(); ++y) {
			auto& symbol = platform[y][x];

			if (symbol == 'O') {
				long x_s{x-1};
				while(x_s >= 0 && platform[y][x_s] != 'O' && platform[y][x_s] != '#') {
					--x_s;
				}

				symbol = '.';
				platform[y][x_s+1] = 'O';
			}
		}
	}
}

void TiltSouth(Platform& platform) {
	for (long y = platform.size() - 2; y >= 0; --y) {
		for (long x{0}; x < platform[y].size(); ++x) {
			auto& symbol = platform[y][x];

			if (symbol == 'O') {
				long y_s{y+1};
				while(y_s < platform.size() && platform[y_s][x] != 'O' && platform[y_s][x] != '#') {
					++y_s;
				}

				symbol = '.';
				platform[y_s-1][x] = 'O';
			}
		}
	}
}

void TiltEast(Platform& platform) {
	for (long x = platform[0].size() - 2; x >= 0; --x) {
		for (long y{0}; y < platform.size(); ++y) {
			auto& symbol = platform[y][x];

			if (symbol == 'O') {
				long x_s{x+1};
				while(x_s < platform[0].size() && platform[y][x_s] != 'O' && platform[y][x_s] != '#') {
					++x_s;
				}

				symbol = '.';
				platform[y][x_s-1] = 'O';
			}
		}
	}
}

bool operator == (const Platform& lhs, const Platform& rhs) {
	if (lhs.size() != rhs.size()) { return false; }
	if (lhs[0].size() != rhs[0].size()) { return false; }

	for (long y{0}; y < lhs.size(); ++y) {
		for (long x{0}; x < lhs[y].size(); ++x) {
			if (lhs[y][x] != rhs[y][x]) {
				return false;
			}
		}
	}

	return true;
}

void spin_cycle_platform(Platform& platform) {
	TiltNorth(platform);
	TiltWest(platform);
	TiltSouth(platform);
	TiltEast(platform);
}

Platform spin_cycle_platform_for(Platform platform, long n_spin_cycles) {
	std::vector<Platform> cached_spin_cycles{};

	for (long n_spin{0}; n_spin < n_spin_cycles; ++n_spin) {
		spin_cycle_platform(platform);

		for (long i{0}; i < cached_spin_cycles.size(); ++i) {
			const auto& previous = cached_spin_cycles[i];
			if (platform == previous) {
				const long pattern_start = i;
				const long pattern_size = cached_spin_cycles.size() - pattern_start;
				const long remaining_cycles = n_spin_cycles - n_spin - 1;
				const long pos_in_pattern = remaining_cycles % pattern_size;
				return cached_spin_cycles[pattern_start + pos_in_pattern];
			}
		}

		cached_spin_cycles.push_back(platform);
	}

	return cached_spin_cycles.back();
}

InputData Day14::ParseInputs(std::ifstream& data) {
	// Actual outputs
	Platform platform{};

	std::string line;
	while(std::getline(data, line)) {
		platform.push_back(line);
	}

	return platform;
}

long Day14::PartOne(const InputData& data) const {
	Platform platform = data;

	TiltNorth(platform);

	long result{0};

	for (long y {0}; y < platform.size(); ++y) {
		const auto& row = platform[y];
		long rocks = std::count(row.cbegin(), row.cend(), 'O');
		result += rocks * (platform.size() - y);
	}

	return result;
}

long Day14::PartTwo(const InputData& data) const {
	const long total_spin_cycles = 1000000000;
	Platform platform = spin_cycle_platform_for(data, total_spin_cycles);

	long result{0};

	for (long y {0}; y < platform.size(); ++y) {
		const auto& row = platform[y];
		long rocks = std::count(row.cbegin(), row.cend(), 'O');
		result += rocks * (platform.size() - y);
	}

	return result;
}

int main(int argc, char* argv[]) {
	Day14{}.Run(argc, argv);
	return 0;
}
