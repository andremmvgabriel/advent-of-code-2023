#include <tuple>
#include <string>
#include <vector>
#include <unordered_map>
#include <queue>

#include "../common/utils.hpp"
#include "../common/AoCDay.hpp"

struct Condition
{
	char packet{};
	char operation{};
	long threshold{};
	std::string outcome{};
};

struct Workflow
{
	std::vector<Condition> conditions{};
	std::string outcome{};
};

struct XMAS_Part
{
	long x{0};
	long m{0};
	long a{0};
	long s{0};
};

using Workflows = std::unordered_map<std::string, Workflow>;
using XMAS_Parts = std::vector<XMAS_Part>;
using InputData = std::tuple<Workflows, XMAS_Parts>;

class Day19 : public AoCDay<InputData>
{
    InputData ParseInputs(std::ifstream& data) override;
    long PartOne(const InputData& data) const override;
    long PartTwo(const InputData& data) const override;
};

InputData Day19::ParseInputs(std::ifstream& data) {
	// Actual outputs
	Workflows workflows{};
	XMAS_Parts parts{};

	std::string line;

	// Read workflows
	while(std::getline(data, line) && !line.empty()) {
		const auto workflow_split_1 = string_split(line, "{");
		const auto workflow_name = workflow_split_1[0];
		const auto workflow_split_2 = string_split(workflow_split_1[1], ",");

		Workflow workflow {};

		for (int i{0}; i < workflow_split_2.size() - 1; ++i) {
			const auto& condition_str = workflow_split_2[i];
			const auto condition_split = string_split(condition_str, ":");
			Condition condition{
				condition_str[0],
				condition_str[1],
				std::stol(condition_split[0].substr(2)),
				condition_split[1]
			};
			workflow.conditions.push_back(condition);
		}

		workflow.outcome = workflow_split_2.back().substr(0, workflow_split_2.back().size() - 1);

		workflows[workflow_name] = workflow;
	}

	// Read parts
	while(std::getline(data, line)) {
		const auto parts_pkg = string_split(line, ",");
		parts.push_back({
			std::stol(parts_pkg[0].substr(3)), // Ignores "{x="
			std::stol(parts_pkg[1].substr(2)), // Ignores "m="
			std::stol(parts_pkg[2].substr(2)), // Ignores "a="
			std::stol(parts_pkg[3].substr(2, parts_pkg[3].size() - 3)) // Ignores "s=" and "}"
		});
	}

	return {workflows, parts};
}

long Day19::PartOne(const InputData& data) const {
	const auto& [workflows, parts] = data;

	long result{0};

	for (const auto& part : parts) {
		std::string curr_workflow = "in";
		std::string outcome = "";

		while (outcome != "R" && outcome != "A") {
			const auto& workflow = workflows.at(curr_workflow); 

			bool condition_pass{false};

			for (const auto& condition : workflow.conditions) {
				long value{0};
				if (condition.packet == 'x') { value = part.x; }
				if (condition.packet == 'm') { value = part.m; }
				if (condition.packet == 'a') { value = part.a; }
				if (condition.packet == 's') { value = part.s; }

				if (condition.operation == '<') {
					condition_pass = (value < condition.threshold);
				}
				if (condition.operation == '>') {
					condition_pass = (value > condition.threshold);
				}

				if (condition_pass) {
					outcome = condition.outcome;
					curr_workflow = condition.outcome;
					break;
				}
			}

			if (!condition_pass) {
				outcome = workflow.outcome;
				curr_workflow = workflow.outcome;
			}
		}

		if (outcome == "A") {
			result += part.x + part.m + part.a + part.s;
		}
	}

	return result;
}

long Day19::PartTwo(const InputData& data) const {
	const auto& [workflows, _] = data;

	Workflows backwards_workflows{};

	long x_min {1}, x_max {4000};
	long m_min {1}, m_max {4000};
	long a_min {1}, a_max {4000};
	long s_min {1}, s_max {4000};

	std::string curr_workflow = "in";

	struct Candidate
	{
		std::string next_search{};
		std::string seq{};
		long x_min {1}, x_max {4000};
		long m_min {1}, m_max {4000};
		long a_min {1}, a_max {4000};
		long s_min {1}, s_max {4000};
	};

	std::vector<Candidate> candidates{};
	std::queue<Candidate> search; search.push(Candidate{"in"});

	while(!search.empty()) {
		const auto cur_candidate = search.back(); search.pop();
		std::cout << "> Current candidate: " << cur_candidate.next_search << std::endl;

		const auto& workflow = workflows.at(cur_candidate.next_search);

		for (const auto& condition : workflow.conditions) {
			std::cout << "  > Condition: " << condition.packet << condition.operation << condition.threshold << " -> " << condition.outcome << std::endl;
			if (condition.outcome == "R") {
				std::cout << "    > The outcome of this condition is not desired." << std::endl;
				continue;
			}

			Candidate new_candidate = cur_candidate;
			new_candidate.next_search = condition.outcome;

			if (condition.packet == 'x') {
				std::cout << "    > Current range: " << cur_candidate.x_min << " " << cur_candidate.x_max << std::endl;
				if (condition.operation == '<') {
					if (new_candidate.x_min >= condition.threshold) {
						std::cout << "      > Impossible to reach condition." << std::endl;
						continue;
					}

					if (new_candidate.x_max >= condition.threshold) {
						new_candidate.x_max = condition.threshold - 1;
					}
				} else {
					if (new_candidate.x_max <= condition.threshold) {
						std::cout << "      > Impossible to reach condition." << std::endl;
						continue;
					}

					if (new_candidate.x_min <= condition.threshold) {
						new_candidate.x_min = condition.threshold + 1;
					}
				}
				std::cout << "    > Updated range: " << new_candidate.x_min << " " << new_candidate.x_max << std::endl;
			} else if (condition.packet == 'm') {
				std::cout << "    > Current range: " << cur_candidate.m_min << " " << cur_candidate.m_max << std::endl;
				if (condition.operation == '<') {
					if (new_candidate.m_min >= condition.threshold) {
						std::cout << "      > Impossible to reach condition." << std::endl;
						continue;
					}

					if (new_candidate.m_max >= condition.threshold) {
						new_candidate.m_max = condition.threshold - 1;
					}
				} else {
					if (new_candidate.m_max <= condition.threshold) {
						std::cout << "      > Impossible to reach condition." << std::endl;
						continue;
					}

					if (new_candidate.m_min <= condition.threshold) {
						new_candidate.m_min = condition.threshold + 1;
					}
				}
				std::cout << "    > Updated range: " << new_candidate.m_min << " " << new_candidate.m_max << std::endl;
			} else if (condition.packet == 'a') {
				std::cout << "    > Current range: " << cur_candidate.a_min << " " << cur_candidate.a_max << std::endl;
				if (condition.operation == '<') {
					if (new_candidate.a_min >= condition.threshold) {
						std::cout << "      > Impossible to reach condition." << std::endl;
						continue;
					}

					if (new_candidate.a_max >= condition.threshold) {
						new_candidate.a_max = condition.threshold - 1;
					}
				} else {
					if (new_candidate.a_max <= condition.threshold) {
						std::cout << "      > Impossible to reach condition." << std::endl;
						continue;
					}

					if (new_candidate.a_min <= condition.threshold) {
						new_candidate.a_min = condition.threshold + 1;
					}
				}
				std::cout << "    > Updated range: " << new_candidate.a_min << " " << new_candidate.a_max << std::endl;
			} else if (condition.packet == 's') {
				std::cout << "    > Current range: " << cur_candidate.s_min << " " << cur_candidate.s_max << std::endl;
				if (condition.operation == '<') {
					if (new_candidate.s_min >= condition.threshold) {
						std::cout << "      > Impossible to reach condition." << std::endl;
						continue;
					}

					if (new_candidate.s_max >= condition.threshold) {
						new_candidate.s_max = condition.threshold - 1;
					}
				} else {
					if (new_candidate.s_max <= condition.threshold) {
						std::cout << "      > Impossible to reach condition." << std::endl;
						continue;
					}

					if (new_candidate.s_min <= condition.threshold) {
						new_candidate.s_min = condition.threshold + 1;
					}
				}
				std::cout << "    > Updated range: " << new_candidate.s_min << " " << new_candidate.s_max << std::endl;
			}

			if (new_candidate.next_search == "A") {
				std::cout << "    > A Candidate found!" << std::endl;
				candidates.push_back(new_candidate);
			} else {
				std::cout << "    > Next search: " << new_candidate.next_search << std::endl;
				search.push(new_candidate);
			}
		}

		if (workflow.outcome == "A") {
			candidates.push_back(cur_candidate);
		} else if (workflow.outcome != "R") {
			Candidate new_candidate = cur_candidate;
			new_candidate.next_search = workflow.outcome;
			search.push(new_candidate);
		}
	}

	Candidate final_candidate{};
	for (const auto& candidate : candidates) {
		final_candidate.x_min = std::max(final_candidate.x_min, candidate.x_min);
		final_candidate.m_min = std::max(final_candidate.m_min, candidate.m_min);
		final_candidate.a_min = std::max(final_candidate.a_min, candidate.a_min);
		final_candidate.s_min = std::max(final_candidate.s_min, candidate.s_min);

		final_candidate.x_max = std::min(final_candidate.x_max, candidate.x_max);
		final_candidate.m_max = std::min(final_candidate.m_max, candidate.m_max);
		final_candidate.a_max = std::min(final_candidate.a_max, candidate.a_max);
		final_candidate.s_max = std::min(final_candidate.s_max, candidate.s_max);
	}

	std::cout << "x " << final_candidate.x_min << " " << final_candidate.x_max << std::endl;
	std::cout << "m " << final_candidate.m_min << " " << final_candidate.m_max << std::endl;
	std::cout << "a " << final_candidate.a_min << " " << final_candidate.a_max << std::endl;
	std::cout << "s " << final_candidate.s_min << " " << final_candidate.s_max << std::endl;

	long result = (final_candidate.x_max - final_candidate.x_min) * 
				  (final_candidate.m_max - final_candidate.m_min) *
				  (final_candidate.a_max - final_candidate.a_min) *
				  (final_candidate.s_max - final_candidate.s_min);
	
	return result;
}

int main(int argc, char* argv[]) {
	Day19{}.Run(argc, argv);
	return 0;
}
