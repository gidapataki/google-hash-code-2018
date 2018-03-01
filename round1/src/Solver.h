#pragma once

#include <cmath>
#include <vector>
#include <iostream>

inline
int distance(int ar, int ac, int br, int bc) {
	return std::abs(ar - br) + std::abs(ac - bc);
}

struct Ride {
	int index;
	int start_row;
	int start_col;
	int end_row;
	int end_col;

	int earliest_start;
	int latest_finish;

	int length() const {
		return distance(start_row, start_col, end_row, end_col);
	}
};

struct Input {
	int rows;
	int cols;
	int car_count;
	std::vector<Ride> rides;

	int bonus_for_starting_on_time;
	int step_count;
};

struct CarAssignments {
	std::vector<int> rides;
};
using CarAssigmentsVec = std::vector<CarAssignments>;

struct Car {
	int available_in_tick = 0;
	int target_pos_row = 0;
	int target_pos_col = 0;

	std::vector<int> rides;
};

using ScoreType = std::tuple<int, int>;

struct Solver {
	Solver(Input input) : input_(std::move(input)) {}

	bool CanSatisfyRide(const Car& car, const Ride& ride);

	ScoreType ScoreCarRide(const Car& car, const Ride& ride);

	Ride* SelectRide(const Car& c);
	void AssignRide(Car& car, Ride& ride);

	CarAssigmentsVec Solve();

	Input input_;
};

inline
int distance(const Car& c, const Ride& r) {
	return distance(c.target_pos_row, c.target_pos_col, r.start_row, r.start_col);
}
