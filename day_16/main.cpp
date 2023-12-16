#include <numeric>
#include <unordered_map>
#include <queue>

#include "../common/utils.hpp"
#include "../common/AoCDay.hpp"

const std::unordered_map<char, std::array<long, 2>> direction_increment {
	{'>', {1, 0}}, {'<', {-1, 0}}, {'^', {0, -1}}, {'v', {0, 1}}
};

struct LightRay {
	long x{0};
	long y{0};
	char direction{'>'};
};

struct Tile {
	char symbol{};

	std::unordered_map<char, bool> directions {
		{'>', false}, {'<', false}, {'^', false}, {'v', false}
	};

	bool energized{false};
};

using TileRow = std::vector<Tile>;
using TileMap = std::vector<TileRow>;
using InputData = TileMap;

class Day16 : public AoCDay<InputData>
{
    InputData ParseInputs(std::ifstream& data) override;
    long PartOne(const InputData& data) const override;
    long PartTwo(const InputData& data) const override;
};

InputData Day16::ParseInputs(std::ifstream& data) {
	// Actual outputs
	TileMap tile_map{};

	std::string line;
	while(std::getline(data, line)) {
		TileRow row{};

		for (const auto& c : line) {
			row.push_back( Tile{c} );
		}

		tile_map.push_back(row);
	}

	return tile_map;
}

long energize_tiles_from(TileMap tile_map, const LightRay& start) {
	std::queue<LightRay> tiles_to_search {};
	tiles_to_search.push(start);

	while(!tiles_to_search.empty()) {
		const auto light = tiles_to_search.front();
		tiles_to_search.pop();

		// If the tile already addressed a light ray with the same direction, the same output will occurr
		if(tile_map[light.y][light.x].directions[light.direction]) {
			continue;
		}

		tile_map[light.y][light.x].directions[light.direction] = true;
		tile_map[light.y][light.x].energized = true;

		if (tile_map[light.y][light.x].symbol == '.') {
			const long next_light_ray_x = light.x + direction_increment.at(light.direction)[0];
			const long next_light_ray_y = light.y + direction_increment.at(light.direction)[1];

			if (next_light_ray_x < 0 || next_light_ray_x >= tile_map[0].size()) { continue; }
			if (next_light_ray_y < 0 || next_light_ray_y >= tile_map.size())    { continue; }

			LightRay next_ray {
				next_light_ray_x,
				next_light_ray_y,
				light.direction
			};

			tiles_to_search.push(next_ray);
			continue;
		}

		if (tile_map[light.y][light.x].symbol == '-') {
			if (light.direction == '>' || light.direction == '<') {
				const long next_light_ray_x = light.x + direction_increment.at(light.direction)[0];
				const long next_light_ray_y = light.y + direction_increment.at(light.direction)[1];

				if (next_light_ray_x < 0 || next_light_ray_x >= tile_map[0].size()) { continue; }
				if (next_light_ray_y < 0 || next_light_ray_y >= tile_map.size())    { continue; }

				LightRay next_ray {
					next_light_ray_x,
					next_light_ray_y,
					light.direction
				};

				tiles_to_search.push(next_ray);
			} else {
				const long next_left_light_ray_x = light.x + direction_increment.at('<')[0];
				const long next_left_light_ray_y = light.y + direction_increment.at('<')[1];

				if (next_left_light_ray_x >= 0 && next_left_light_ray_x < tile_map[0].size() && next_left_light_ray_y >= 0 && next_left_light_ray_y < tile_map.size()) {
					LightRay next_left_ray {
						next_left_light_ray_x,
						next_left_light_ray_y,
						'<'
					};

					tiles_to_search.push(next_left_ray);
				}

				const long next_right_light_ray_x = light.x + direction_increment.at('>')[0];
				const long next_right_light_ray_y = light.y + direction_increment.at('>')[1];

				if (next_right_light_ray_x >= 0 && next_right_light_ray_x < tile_map[0].size() && next_right_light_ray_y >= 0 && next_right_light_ray_y < tile_map.size()) {
					LightRay next_right_ray {
						next_right_light_ray_x,
						next_right_light_ray_y,
						'>'
					};

					tiles_to_search.push(next_right_ray);
				}
			}
			continue;
		}

		if (tile_map[light.y][light.x].symbol == '|') {
			if (light.direction == '^' || light.direction == 'v') {
				const long next_light_ray_x = light.x + direction_increment.at(light.direction)[0];
				const long next_light_ray_y = light.y + direction_increment.at(light.direction)[1];

				if (next_light_ray_x < 0 || next_light_ray_x >= tile_map[0].size()) { continue; }
				if (next_light_ray_y < 0 || next_light_ray_y >= tile_map.size())    { continue; }

				LightRay next_ray {
					next_light_ray_x,
					next_light_ray_y,
					light.direction
				};

				tiles_to_search.push(next_ray);
			} else {
				const long next_up_light_ray_x = light.x + direction_increment.at('^')[0];
				const long next_up_light_ray_y = light.y + direction_increment.at('^')[1];

				if (next_up_light_ray_x >= 0 && next_up_light_ray_x < tile_map[0].size() && next_up_light_ray_y >= 0 && next_up_light_ray_y < tile_map.size()) {
					LightRay next_up_ray {
						next_up_light_ray_x,
						next_up_light_ray_y,
						'^'
					};

					tiles_to_search.push(next_up_ray);
				}

				const long next_down_light_ray_x = light.x + direction_increment.at('v')[0];
				const long next_down_light_ray_y = light.y + direction_increment.at('v')[1];

				if (next_down_light_ray_x >= 0 && next_down_light_ray_x < tile_map[0].size() && next_down_light_ray_y >= 0 && next_down_light_ray_y < tile_map.size()) {
					LightRay next_down_ray {
						next_down_light_ray_x,
						next_down_light_ray_y,
						'v'
					};

					tiles_to_search.push(next_down_ray);
				}
			}
			continue;
		}

		if (tile_map[light.y][light.x].symbol == '\\') {
			if (light.direction == '>') {
				const long next_light_ray_x = light.x + direction_increment.at('v')[0];
				const long next_light_ray_y = light.y + direction_increment.at('v')[1];

				if (next_light_ray_x < 0 || next_light_ray_x >= tile_map[0].size()) { continue; }
				if (next_light_ray_y < 0 || next_light_ray_y >= tile_map.size())    { continue; }

				LightRay next_ray {
					next_light_ray_x,
					next_light_ray_y,
					'v'
				};

				tiles_to_search.push(next_ray);
			} else if (light.direction == '<') {
				const long next_light_ray_x = light.x + direction_increment.at('^')[0];
				const long next_light_ray_y = light.y + direction_increment.at('^')[1];

				if (next_light_ray_x < 0 || next_light_ray_x >= tile_map[0].size()) { continue; }
				if (next_light_ray_y < 0 || next_light_ray_y >= tile_map.size())    { continue; }

				LightRay next_ray {
					next_light_ray_x,
					next_light_ray_y,
					'^'
				};

				tiles_to_search.push(next_ray);
			} else if (light.direction == '^') {
				const long next_light_ray_x = light.x + direction_increment.at('<')[0];
				const long next_light_ray_y = light.y + direction_increment.at('<')[1];

				if (next_light_ray_x < 0 || next_light_ray_x >= tile_map[0].size()) { continue; }
				if (next_light_ray_y < 0 || next_light_ray_y >= tile_map.size())    { continue; }

				LightRay next_ray {
					next_light_ray_x,
					next_light_ray_y,
					'<'
				};

				tiles_to_search.push(next_ray);
			} else if (light.direction == 'v') {
				const long next_light_ray_x = light.x + direction_increment.at('>')[0];
				const long next_light_ray_y = light.y + direction_increment.at('>')[1];

				if (next_light_ray_x < 0 || next_light_ray_x >= tile_map[0].size()) { continue; }
				if (next_light_ray_y < 0 || next_light_ray_y >= tile_map.size())    { continue; }

				LightRay next_ray {
					next_light_ray_x,
					next_light_ray_y,
					'>'
				};

				tiles_to_search.push(next_ray);
			}
			continue;
		}

		if (tile_map[light.y][light.x].symbol == '/') {
			if (light.direction == '>') {
				const long next_light_ray_x = light.x + direction_increment.at('^')[0];
				const long next_light_ray_y = light.y + direction_increment.at('^')[1];

				if (next_light_ray_x < 0 || next_light_ray_x >= tile_map[0].size()) { continue; }
				if (next_light_ray_y < 0 || next_light_ray_y >= tile_map.size())    { continue; }

				LightRay next_ray {
					next_light_ray_x,
					next_light_ray_y,
					'^'
				};

				tiles_to_search.push(next_ray);
			} else if (light.direction == '<') {
				const long next_light_ray_x = light.x + direction_increment.at('v')[0];
				const long next_light_ray_y = light.y + direction_increment.at('v')[1];

				if (next_light_ray_x < 0 || next_light_ray_x >= tile_map[0].size()) { continue; }
				if (next_light_ray_y < 0 || next_light_ray_y >= tile_map.size())    { continue; }

				LightRay next_ray {
					next_light_ray_x,
					next_light_ray_y,
					'v'
				};

				tiles_to_search.push(next_ray);
			} else if (light.direction == '^') {
				const long next_light_ray_x = light.x + direction_increment.at('>')[0];
				const long next_light_ray_y = light.y + direction_increment.at('>')[1];

				if (next_light_ray_x < 0 || next_light_ray_x >= tile_map[0].size()) { continue; }
				if (next_light_ray_y < 0 || next_light_ray_y >= tile_map.size())    { continue; }

				LightRay next_ray {
					next_light_ray_x,
					next_light_ray_y,
					'>'
				};

				tiles_to_search.push(next_ray);
			} else if (light.direction == 'v') {
				const long next_light_ray_x = light.x + direction_increment.at('<')[0];
				const long next_light_ray_y = light.y + direction_increment.at('<')[1];

				if (next_light_ray_x < 0 || next_light_ray_x >= tile_map[0].size()) { continue; }
				if (next_light_ray_y < 0 || next_light_ray_y >= tile_map.size())    { continue; }

				LightRay next_ray {
					next_light_ray_x,
					next_light_ray_y,
					'<'
				};

				tiles_to_search.push(next_ray);
			}
			continue;
		}
	}

	return std::accumulate(
		tile_map.cbegin(), tile_map.cend(), 0L,
		[](long result, const TileRow& tile_row){
			return result + std::accumulate(
				tile_row.cbegin(), tile_row.cend(), 0L,
				[](long result, const Tile& tile){
					return result + (tile.energized ? 1 : 0);
				}
			);
		}
	);
}

long Day16::PartOne(const InputData& data) const {
	return energize_tiles_from(data, {0, 0, '>'});
}

long Day16::PartTwo(const InputData& data) const {
	long best{0};

	for (int y{0}; y < data.size(); ++y) {
		long res1 = energize_tiles_from(data, {0, y, '>'});
		if (best < res1) { best = res1; }
		long res2 = energize_tiles_from(data, {static_cast<long>(data[0].size()) - 1, y, '<'});
		if (best < res2) { best = res2; }
	}

	for (int x{0}; x < data[0].size(); ++x) {
		long res1 = energize_tiles_from(data, {x, 0, 'v'});
		if (best < res1) { best = res1; }
		long res2 = energize_tiles_from(data, {x, static_cast<long>(data.size()) - 1, '^'});
		if (best < res2) { best = res2; }
	}

	return best;
}

int main(int argc, char* argv[]) {
	Day16{}.Run(argc, argv);
	return 0;
}
