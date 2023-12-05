#include <iterator>
#include <limits>
#include <vector>
#include <array>
#include <tuple>
#include <set>
#include <algorithm>

#include "../common/utils.hpp"
#include "../common/AoCDay.hpp"

struct Range {
	long min{0};
	long max{0};
};

struct MapRegion {
	Range src{};
	Range dst{};
};

using Map = std::set<MapRegion>;
using Maps = std::array<Map, 7>;
using Seeds = std::vector<long>;
using InputData = std::tuple<Seeds, Maps>;

class Day05 : public AoCDay<InputData>
{
    InputData ParseInputs(std::ifstream& data) override;
    long PartOne(const InputData& data) const override;
    long PartTwo(const InputData& data) const override;
};

bool operator < (const MapRegion& lhs, const MapRegion& rhs) {
	return (lhs.src.min < rhs.src.min);
}

InputData Day05::ParseInputs(std::ifstream& data) {
    // Actual outputs
    Seeds seeds{};
	Maps maps{};

    std::string line;

	// Read seeds
	if (std::getline(data, line)) {
		const auto& seeds_text = string_split(line.substr(7), " ");
		std::transform(
			seeds_text.cbegin(), seeds_text.cend(),
			std::back_inserter(seeds),
			[](const std::string& num_text){ return std::stol(num_text);}
		);
	}

	// Empty line
	std::getline(data, line);

	// Read the maps
	for (int i{0}; i < 7; ++i) {
		Map current_map{};

		// Map name
		std::getline(data, line);

		// While not EOF and Empty line
		while(std::getline(data, line) && !line.empty()) {
			const auto& values = string_split(line, " ");

			// Converts the read values
			const long dst_min = std::stol(values[0]);
			const long src_min = std::stol(values[1]);
			const long range = std::stol(values[2]);

			// Creates a new map region
			const MapRegion region{
				Range{src_min, src_min + range - 1},
				Range{dst_min, dst_min + range - 1}
			};
			current_map.insert(region);
		}

		// Adds the created map into the list
		maps[i] = current_map;
	}

    return {seeds, maps};
}

long Day05::PartOne(const InputData& data) const {
	const auto& [seeds, maps] = data;

	Seeds locations = seeds;

	for (auto& seed : locations) {
		for (const auto& map : maps) {
			for (const auto& region : map) {
				// Upcoming regions will not contain the seed
				if (seed < region.src.min) { break; }

				// Pass to the next region
				if (seed > region.src.max) { continue; }

				// Convert seed value
				// > Pass to next seed
				seed += region.dst.min - region.src.min;
				break;
			}
		}
	}

	return *std::min_element(locations.cbegin(), locations.cend());
}

long Day05::PartTwo(const InputData& data) const {
	const auto& [seeds, maps] = data;

	// Re-Arrange seeds to fulfill new conditions
	std::vector<Range> locations;
	for (int i{0}; i < seeds.size(); i+=2) {
		locations.push_back(Range{
			seeds[i], seeds[i] + seeds[i+1] - 1
		});
	}

	for (const auto& map : maps) {
		// Vector that will gather the generated locations for next map
		std::vector<Range> next_locations;

		for (auto& location : locations) {
			for (auto region = map.cbegin(); region != map.cend(); region++) {
				// Location range ahead map region
				// > Go to next region
				// > If last region, save location for next map
				if (location.min > region->src.max) {
					if (std::next(region) == map.cend()) {
						next_locations.push_back(location);
					}
					continue;
				}

				// Location range behind map region
				// > No intersection will occurr with the upcoming regions
				// > Location remains unchanged for next map
				if (location.max < region->src.min) {
					next_locations.push_back(location);
					break;
				}

				// If reached here, the location is intersecting with map region
				const Range intersection {
					std::max(location.min, region->src.min),
					std::min(location.max, region->src.max)
				};

				// Check if intersection does not contain all the initial location points
				// > Created region remains unchanged for next map
				if (intersection.min > location.min) {
					next_locations.push_back(Range{
						location.min,
						intersection.min - 1
					});
				}

				// Apply the map convertion to the intersection zone
				next_locations.push_back(Range{
					intersection.min - region->src.min + region->dst.min,
					intersection.max - region->src.min + region->dst.min
				});

				// Check if intersection does not contain all the final location points
				// > Updates the current location for next region check
				// > If last region, save updated location for next map
				if (intersection.max < location.max) {
					location.min = intersection.max + 1;
					if (std::next(region) == map.cend()) {
						next_locations.push_back(location);
					}
				}
				// If the intersection does indeed contaill all the final location points
				// > No need to check the next region
				else { break; }
			}
		}

		// Updates the location for next map
		locations = next_locations;
	}

	return (*std::min_element(
		locations.cbegin(), locations.cend(),
		[](const Range& lhs, const Range& rhs){
			return lhs.min < rhs.min;
		}
	)).min;
}

int main(int argc, char* argv[]) {
	Day05{}.Run(argc, argv);
	return 0;
}
