#include <iostream>
#include <vector>

struct Ride {
	int index;
	int start_row;
	int start_col;
	int end_row;
	int end_col;

	int earliest_start;
	int latest_finish;
};

struct Input {
	int rows;
	int cols;
	int car_count;
	std::vector<Ride> rides;

	int bonus_for_starting_on_time;
	int step_count;
};

Input ParseInput(std::istream& in) {
	Input input;

	int ride_count;
	in >> input.rows >> input.cols >> input.car_count >>
		ride_count >> input.bonus_for_starting_on_time >> input.step_count;

	for (int i = 0; i < ride_count; ++i) {
		Ride ride;
		ride.index = i;
		in >> ride.start_row >> ride.start_col >>
			ride.end_col >> ride.end_col >>
			ride.earliest_start >> ride.latest_finish;

		input.rides.push_back(ride);
	}

	return input;
}

int main() {
	auto input = ParseInput(std::cin);

	return 0;
}
