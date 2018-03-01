#include <iostream>
#include <vector>

#include "Solver.h"



Input ParseInput(std::istream& in) {
	Input input;

	int ride_count;
	in >> input.rows >> input.cols >> input.car_count >>
		ride_count >> input.bonus_for_starting_on_time >> input.step_count;

	for (int i = 0; i < ride_count; ++i) {
		Ride ride;
		ride.index = i;
		in >> ride.start_row >> ride.start_col >>
			ride.end_row >> ride.end_col >>
			ride.earliest_start >> ride.latest_finish;

		input.rides.push_back(ride);
	}

	return input;
}

void OutputAssigments(std::ostream& os, const CarAssigmentsVec& v) {
	for (auto& e : v) {
		os << e.rides.size() << ' ';
		for (int i : e.rides) {
			os << i << ' ';
		}
		os << '\n';
	}
	os << std::flush;
}

CarAssigmentsVec GetResult(const Input& input, int value) {
	Solver solver{input, value};
	auto result = solver.Solve();
	std::cerr << solver.full_score_ << std::endl;
	return result;
}

int main(int argc, char** argv) {
	int value = 0;
	if (argc > 1) {
		value = std::stoi(argv[1]);
	}

	auto input = ParseInput(std::cin);
	auto result = GetResult(input, value);

	// cerr << score;
	OutputAssigments(std::cout, result);

	return 0;
}
