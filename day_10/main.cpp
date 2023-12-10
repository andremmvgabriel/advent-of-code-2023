#include <algorithm>
#include <tuple>
#include <unordered_map>
#include <vector>
#include <string>
#include <map>

#include "../common/utils.hpp"
#include "../common/AoCDay.hpp"

struct Pipe {
	long x{0};
	long y{0};
};

const std::map<char, Pipe> DIRECTIONS {
	{'E', { 1, 0}},
	{'W', {-1, 0}},
	{'S', {0 , 1}},
	{'N', {0 ,-1}}
};

const std::unordered_map<char, char> EDGE_PIPES_CLOSING_MATCH {
	{'F', 'J'},
	{'L', '7'}
};

using PipeMap = std::vector<std::string>;
using PipeSystem = std::vector<Pipe>;
using InputData = std::tuple<PipeMap, Pipe>;

class Day10 : public AoCDay<InputData>
{
    InputData ParseInputs(std::ifstream& data) override;
    long PartOne(const InputData& data) const override;
    long PartTwo(const InputData& data) const override;
};

InputData Day10::ParseInputs(std::ifstream& data) {
	// Actual outputs
	PipeMap pipe_map{};
	Pipe start_position{};

	long y_counter{0};
	std::string line;
    while(std::getline(data, line)) {
		pipe_map.push_back(line);
		const auto start_pos = line.find('S');
		if (start_pos != -1) {
			start_position.x = static_cast<long>(start_pos);
			start_position.y = y_counter;
		}
		++y_counter;
    }

	return {pipe_map, start_position};
}

// Utility function to check if two pipes are the same
bool operator==(const Pipe& lhs, const Pipe& rhs) {
	return (lhs.x == rhs.x) && (lhs.y == rhs.y);
}

bool is_inside_map(const PipeMap& map, const Pipe& pipe) {
	// Map grid limits
	const auto map_x_range = map[0].size();
	const auto map_y_range = map.size();

	// Check
	if (pipe.x < 0 || pipe.x >= map_x_range || pipe.y < 0 || pipe.y >= map_y_range) {
		return false;
	}

	return true;
}

char find_start_position_pipe(const PipeMap& map, const Pipe& starting_pipe) {
	// Possible candidates for the starting pipe will be stored here
	std::map<char, int> candidates{};

	// Checks the pipe surroudings in all directions
	for (const auto& [dir, vec] : DIRECTIONS) {
		// Calculates the current adjacent pipe position
		const Pipe next_pipe {
			starting_pipe.x + vec.x,
			starting_pipe.y + vec.y
		};

		// Do not proceed if outside the pipe map grid
		if (!is_inside_map(map, next_pipe)) { continue; }

		// Gets the symbol in the pipe position
		const char& symbol = map[next_pipe.y][next_pipe.x];

		// Deciphers the possible candidates based on the current adjacent pipe
		if (dir == 'N') {
			if (symbol == '|') { ++candidates['J']; ++candidates['L']; ++candidates['|']; }
			if (symbol == 'F') { ++candidates['J']; ++candidates['L']; ++candidates['|']; }
			if (symbol == '7') { ++candidates['J']; ++candidates['L']; ++candidates['|']; }
		} else if (dir == 'S') {
			if (symbol == '|') { ++candidates['7']; ++candidates['F']; ++candidates['|']; }
			if (symbol == 'L') { ++candidates['7']; ++candidates['F']; ++candidates['|']; }
			if (symbol == 'J') { ++candidates['7']; ++candidates['F']; ++candidates['|']; }
		} else if (dir == 'E') {
			if (symbol == '-') { ++candidates['F']; ++candidates['L']; ++candidates['-']; }
			if (symbol == 'J') { ++candidates['F']; ++candidates['L']; ++candidates['-']; }
			if (symbol == '7') { ++candidates['F']; ++candidates['L']; ++candidates['-']; }
		} else if (dir == 'W') {
			if (symbol == '-') { ++candidates['7']; ++candidates['J']; ++candidates['-']; }
			if (symbol == 'L') { ++candidates['7']; ++candidates['J']; ++candidates['-']; }
			if (symbol == 'F') { ++candidates['7']; ++candidates['J']; ++candidates['-']; }
		}
	}

	// Returns the best candidate (should be the only one with 2 as counter)
	return std::max_element(
		candidates.cbegin(), candidates.cend(),
		[](const std::pair<char, int>& lhs, const std::pair<char, int>& rhs){
			return lhs.second < rhs.second;
		}
	)->first;
}

char find_start_search_flow(const PipeMap& map, const Pipe& starting_pipe) {
	// Checks the pipe surroudings in all directions
	for (const auto& [dir, vec] : DIRECTIONS) {
		// Calculates the current adjacent pipe position
		const Pipe next_pipe {
			starting_pipe.x + vec.x,
			starting_pipe.y + vec.y
		};

		// Do not proceed if outside the pipe map grid
		if (!is_inside_map(map, next_pipe)) { continue; }

		// Gets the symbol in the pipe position
		const char& symbol = map[next_pipe.y][next_pipe.x];

		// Checks if for the current direction the adjacent pipe allows to flow with the starting pipe
		if (dir == 'N') {
			if (symbol == '|') { return 'N'; }
			if (symbol == 'F') { return 'N'; }
			if (symbol == '7') { return 'N'; }
		} else if (dir == 'S') {
			if (symbol == '|') { return 'S'; }
			if (symbol == 'L') { return 'S'; }
			if (symbol == 'J') { return 'S'; }
		} else if (dir == 'E') {
			if (symbol == '-') { return 'E'; }
			if (symbol == 'J') { return 'E'; }
			if (symbol == '7') { return 'E'; }
		} else if (dir == 'W') {
			if (symbol == '-') { return 'W'; }
			if (symbol == 'L') { return 'W'; }
			if (symbol == 'F') { return 'W'; }
		}
	}

	// If we ever reach here we have a problem :D
	return {};
}

std::tuple<Pipe, char> find_next_flow_pipe(const PipeMap& map, const Pipe& pipe, char dir) {
	// Gets the vector for the current search direction
	const auto& vec = DIRECTIONS.at(dir);

	// Calculates the adjacent pipe position
	const Pipe next_pipe {
		pipe.x + vec.x,
		pipe.y + vec.y
	};

	// Do not proceed if outside the pipe map grid
	if (!is_inside_map(map, next_pipe)) {
		return { Pipe{-1, -1}, {} }; // Dummy output
	}

	// Gets the symbol in the pipe position
	const char& symbol = map[next_pipe.y][next_pipe.x];

	// If by any change we reach the starting position
	if (symbol == 'S') { return {next_pipe, {}}; }

	// Find the next search direction based on the symbol of the adjacent pipe
	if (dir == 'N') {
		if (symbol == '|') { return {next_pipe, 'N'}; }
		if (symbol == 'F') { return {next_pipe, 'E'}; }
		if (symbol == '7') { return {next_pipe, 'W'}; }
	} else if (dir == 'S') {
		if (symbol == '|') { return {next_pipe, 'S'}; }
		if (symbol == 'L') { return {next_pipe, 'E'}; }
		if (symbol == 'J') { return {next_pipe, 'W'}; }
	} else if (dir == 'E') {
		if (symbol == '-') { return {next_pipe, 'E'}; }
		if (symbol == 'J') { return {next_pipe, 'N'}; }
		if (symbol == '7') { return {next_pipe, 'S'}; }
	} else if (dir == 'W') {
		if (symbol == '-') { return {next_pipe, 'W'}; }
		if (symbol == 'L') { return {next_pipe, 'N'}; }
		if (symbol == 'F') { return {next_pipe, 'S'}; }
	}

	return { Pipe{-1, -1}, {} }; // Dummy output
}

PipeSystem find_pipe_system(const PipeMap& map, const Pipe& starting_pipe) {
	PipeSystem pipe_system{starting_pipe};

	// Get a starting search direction
	char direction = find_start_search_flow(map, starting_pipe);

	while(true) {
		// Always analyse the last inserted pipe until we find the loop
		const auto& pipe = pipe_system.back();

		// Find the next flow pipe and search direction
		const auto [next_pipe, next_dir] = find_next_flow_pipe(map, pipe, direction);

		// Check if the loop is unreachable
		if (!is_inside_map(map, next_pipe)) { break; }

		// Check if the loop was reached
		if (next_pipe == starting_pipe) { break; }

		// Updates the search variables
		direction = next_dir;
		pipe_system.push_back(next_pipe);
	}

	return pipe_system;
}

long Day10::PartOne(const InputData& data) const {
	const auto& [pipe_map, start_position] = data;

	const auto pipe_system = find_pipe_system(pipe_map, start_position);
	return pipe_system.size() / 2;
}

long Day10::PartTwo(const InputData& data) const {
	const auto& [pipe_map, start_position] = data;

	const auto pipe_system = find_pipe_system(pipe_map, start_position);

	// Creates a clean pipe map full of empty spaces
	PipeMap clean_pipe_map{};
	clean_pipe_map.resize(pipe_map.size(), std::string(pipe_map[0].size(), '.'));

	// Fills the clean pipe map with only the pipe system
	for (const auto& pipe : pipe_system) {
		clean_pipe_map[pipe.y][pipe.x] = pipe_map[pipe.y][pipe.x];
	}

	// Replaces the S symbol with its corresponding pipe symbol
	clean_pipe_map[start_position.y][start_position.x] = find_start_position_pipe(pipe_map, start_position);

	// Actual search algorithm
	// > Raycasting in all the map rows
	// > - Everytime a delimiter pipe symbol is found (lets call it frontier), there is a transition
	//    between outside and inside regime;
	//     - The transition causes the raycast to increase its current value by 1
	// > - In edge cases (L, J, 7, F) is found, another edge symbol will be found at the end of the
	//    pipe segment. It is necessary to find if it is a closing edge creating a frontier, or not
	//     - Closing edges    : FJ and L7
	//     - Not closing edges: F7 and LJ
	//     - Note: Best way to imagine this is to project its shadow. FJ or L7 projected allow the
	//            creation of the symbol '|' as their shadow, while the others only half of it.
	// > - In not closing edges scenarios, and additional increment of the raycast must be done
	//    to restore its previous status before encoutering the starting pipe of the segment (F or L)
	long inside_points{0};
	for (int y{0}; y < clean_pipe_map.size(); ++y) {
		// Variable that will control the raycast status of the current row
		int raycast_status{0};

		for (int x{0}; x < clean_pipe_map[y].size(); ++x) {
			// Current symbol under analysis
			auto symbol = clean_pipe_map[y][x];

			// If it is a pipe symbol
			if (symbol != '.') {
				// Update raycast status
				++raycast_status;

				// If it is a edge case
				if (std::string("FL").find(symbol) != -1) {
					// Get the expected symbol to be a closing segment
					const auto& expected_closing_match = EDGE_PIPES_CLOSING_MATCH.at(symbol);

					// Search for the end edge in the pipe segment
					do {
						++x;
						symbol = clean_pipe_map[y][x];
					}
					while (std::string("7J").find(symbol) == -1);

					// Update the raycast status if the end edge does not make a closing match
					if (symbol != expected_closing_match) {
						++raycast_status;
					}
				}
				continue;
			}

			// If reached here, no pipe symbol was found. Increate inner point if raycast status says it is inside
			if (raycast_status % 2 == 1) {
				++inside_points;
			}
		}
	}

	return inside_points;
}

int main(int argc, char* argv[]) {
	Day10{}.Run(argc, argv);
	return 0;
}
