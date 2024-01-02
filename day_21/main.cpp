#include <vector>
#include <string>
#include <set>
#include <map>
#include <numeric>

#include "../common/utils.hpp"
#include "../common/AoCDay.hpp"

struct GardenPlot
{
	long x{0};
	long y{0};
};

bool operator < (const GardenPlot& lhs, const GardenPlot& rhs) {
	return lhs.x < rhs.x || (lhs.x == rhs.x && lhs.y < rhs.y);
}

GardenPlot operator + (const GardenPlot& lhs, const GardenPlot& rhs) {
	return GardenPlot{lhs.x + rhs.x, lhs.y + rhs.y};
}

using InputData = std::vector<std::string>;

class Day21 : public AoCDay<InputData>
{
    InputData ParseInputs(std::ifstream& data) override;
    long PartOne(const InputData& data) const override;
    long PartTwo(const InputData& data) const override;
};

InputData Day21::ParseInputs(std::ifstream& data) {
	// Actual outputs
	InputData garden{};

	// Read inputs
	std::string line;
	while(std::getline(data, line)) {
		garden.push_back(line);
	}

	return garden;
}

std::set<GardenPlot> directions {
	GardenPlot{ 0,  1},
	GardenPlot{ 0, -1},
	GardenPlot{ 1,  0},
	GardenPlot{-1,  0}
};

long Day21::PartOne(const InputData& data) const {
	std::map<GardenPlot, long> visited_plots{};
	std::set<GardenPlot> next_plots{};

	// Find starting point
	for (long y{0}; y < data.size(); ++y) {
		const auto& line = data[y];
		const auto pos = line.find('S');
		if (pos != -1) {
			GardenPlot garden_plot{static_cast<long>(pos), y};
			next_plots.insert(garden_plot);
			++visited_plots[garden_plot];
			break;
		}
	}

	const long max_number_steps{64};
	long steps_counter{0};
	while (steps_counter < max_number_steps) {
		std::set<GardenPlot> next_next{};

		// Flips all the visited garden plots
		for (auto& garden_plot : visited_plots) {
			++garden_plot.second;
		}

		for (const auto& garden_plot : next_plots) {
			for (const auto& dir : directions) {
				const auto neighbour_garden_plot = garden_plot + dir;

				if (neighbour_garden_plot.x < 0 || neighbour_garden_plot.x >= data[0].size()) { continue; }
				if (neighbour_garden_plot.y < 0 || neighbour_garden_plot.y >= data.size())    { continue; }
				if (visited_plots.find(neighbour_garden_plot) != visited_plots.cend())        { continue; }

				const auto& symbol = data[neighbour_garden_plot.y][neighbour_garden_plot.x];

				if (symbol == '#') { continue; }

				++visited_plots[neighbour_garden_plot];
				next_next.insert(neighbour_garden_plot);
			}
		}

		++steps_counter;

		if (next_next.empty()) {
			break;
		}

		next_plots = next_next;
	}

	return std::accumulate(
		visited_plots.cbegin(), visited_plots.cend(), 0L,
		[](long result, const std::pair<GardenPlot, long>& garden_plot){
			return result + garden_plot.second % 2;
		}
	);
}

long Day21::PartTwo(const InputData& data) const {
	return 2;
}

int main(int argc, char* argv[]) {
	Day21{}.Run(argc, argv);
	return 0;
}
