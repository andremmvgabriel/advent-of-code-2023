#pragma once

#include <iostream>
#include <fstream>
#include <string>

#include "utils.hpp"

template<typename InputData>
class AoCDay
{
protected:
    InputData INPUT_DATA{};

    void RunParseInputs(int argc, char* argv[]) {
        // Makes sure a input file is specified
        if (argc <= 1) throw std::invalid_argument("You have to specify an input text file.");

        // Open input file
        std::ifstream input_file{std::string(argv[1]), std::ios::in};

        // Actual parsing
        float elapsed_time = time_block( [&](){
            INPUT_DATA = ParseInputs(input_file);
        });

        // Close input file
        input_file.close();

        printf("\n> Parse Inputs < (%f seconds)\n", elapsed_time);
    }

    void RunPartOne() const {
        long result{0};

        float elapsed_time = time_block( [&](){
            result = PartOne(INPUT_DATA);
        });

        printf("\n> Part One < (%f seconds)\n", elapsed_time);
        printf("   The result is: %ld\n", result);
    }

    void RunPartTwo() const {
        long result{0};

        float elapsed_time = time_block( [&](){
            result = PartTwo(INPUT_DATA);
        });

        printf("\n> Part Two < (%f seconds)\n", elapsed_time);
        printf("   The result is: %ld\n", result);
    }

    virtual InputData ParseInputs(std::ifstream& data) = 0;
    virtual long PartOne(const InputData& data) const = 0;
    virtual long PartTwo(const InputData& data) const = 0;

public:
    void Run(int argc, char* argv[]) {
        RunParseInputs(argc, argv);
        RunPartOne();
        RunPartTwo();
    }
};
