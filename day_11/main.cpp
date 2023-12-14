#include <numeric>
#include <tuple>
#include <vector>

#include "../common/utils.hpp"
#include "../common/AoCDay.hpp"

struct Position {
	long x{0};
	long y{0};
};

using GraphLine = std::vector<long>;
using Graph = std::vector<GraphLine>;
using EmptySpace = std::vector<bool>;
using Galaxies = std::vector<Position>;
using InputData = std::tuple<Galaxies, EmptySpace, EmptySpace>;

class Day11 : public AoCDay<InputData>
{
    InputData ParseInputs(std::ifstream& data) override;
    long PartOne(const InputData& data) const override;
    long PartTwo(const InputData& data) const override;
};

InputData Day11::ParseInputs(std::ifstream& data) {
	// Actual outputs
	Galaxies galaxies{};
	EmptySpace h_empty_space{};
	EmptySpace v_empty_space{};

	long y{0};
	std::string line;
	std::getline(data, line);
	v_empty_space.resize(line.size(), true);

	do {
		h_empty_space.push_back(true);
		for (int x{0}; x < line.size(); ++x) {
			const auto& symbol = line[x];

			v_empty_space[x] = v_empty_space[x] && (symbol == '.');
			h_empty_space[y] = h_empty_space[y] && (symbol == '.');

			if (symbol == '#') {
				galaxies.push_back({x, y});
			}
		}
		++y;
	} while(std::getline(data, line));

	return {galaxies, h_empty_space, v_empty_space};
}

Graph compute_graph(const Galaxies& galaxies, const EmptySpace& h_empty_space, const EmptySpace& v_empty_space, int empty_space_amount) {
	Graph graph( galaxies.size(), GraphLine(galaxies.size(), 0) );

	for (int i{0}; i < galaxies.size(); ++i) {
		const auto& v1 = galaxies[i];
		for (int j{i+1}; j < galaxies.size(); ++j) {
			const auto& v2 = galaxies[j];

			const long x_min = std::min(v1.x, v2.x);
			const long x_max = std::max(v1.x, v2.x);
			const long y_min = std::min(v1.y, v2.y);
			const long y_max = std::max(v1.y, v2.y);

			const long x_spaces_expansion = std::accumulate(
				h_empty_space.cbegin() + y_min, h_empty_space.cbegin() + y_max, 0L,
				[&](long result, bool space){
					return result + (space ? empty_space_amount - 1 : 0);
				}
			);

			const long y_spaces_expansion = std::accumulate(
				v_empty_space.cbegin() + x_min, v_empty_space.cbegin() + x_max, 0L,
				[&](long result, bool space){
					return result + (space ? empty_space_amount - 1 : 0);
				}
			);

			long x_distance = x_max - x_min + x_spaces_expansion;
			long y_distance = y_max - y_min + y_spaces_expansion;
			long total_distance = x_distance + y_distance;

			graph[i][j] = total_distance;
			graph[j][i] = total_distance;
		}
	}

	return graph;
}

long Day11::PartOne(const InputData& data) const {
	const auto& [galaxies, h_empty_space, v_empty_space] = data;

	const auto graph = compute_graph(galaxies, h_empty_space, v_empty_space, 2);

	long result{0};
	for (int i{0}; i < graph.size(); ++i) {
		for (int j{i+1}; j < graph.size(); ++j) {
			result += graph[i][j];
		}
	}

	return result;
}

long Day11::PartTwo(const InputData& data) const {
	const auto& [galaxies, h_empty_space, v_empty_space] = data;

	const auto graph = compute_graph(galaxies, h_empty_space, v_empty_space, 1000000);

	long result{0};
	for (int i{0}; i < graph.size(); ++i) {
		for (int j{i+1}; j < graph.size(); ++j) {
			result += graph[i][j];
		}
	}

	return result;
}

int main(int argc, char* argv[]) {
	Day11{}.Run(argc, argv);
	return 0;
}
