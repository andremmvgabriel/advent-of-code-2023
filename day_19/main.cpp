#include <tuple>
#include <string>
#include <vector>
#include <unordered_map>

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

		std::cout << "Part: x" << part.x << " m" << part.m << " a" << part.a << " s" << part.s << std::endl;

		while (outcome != "R" && outcome != "A") {
			const auto& workflow = workflows.at(curr_workflow); 
			std::cout << "> Workflow: " << workflow.outcome << std::endl;

			bool condition_pass{false};

			for (const auto& condition : workflow.conditions) {
				std::cout << "  > " << condition.packet << condition.operation << condition.threshold << " ? ";
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
					std::cout << "Yes -> " << outcome << std::endl;
					break;
				}

				std::cout << "No" << std::endl;
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
	return 2;
}

int main(int argc, char* argv[]) {
	Day19{}.Run(argc, argv);
	return 0;
}
