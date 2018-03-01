#include "Solver.h"
#include <queue>
#include <cassert>


void Solver::AssignRide(Car& car, Ride& ride) {
	int sum_ride_length = distance(car, ride) + ride.length();
	assert(car.available_in_tick + sum_ride_length < ride.latest_finish);

	car.available_in_tick =
		std::max(car.available_in_tick + sum_ride_length,
				ride.earliest_start + ride.length());

	car.target_pos_row = ride.end_row;
	car.target_pos_col = ride.end_col;

	car.rides.push_back(ride.index);

	// !!! XXX !!!
	std::swap(ride, input_.rides.back());
	input_.rides.pop_back();
}

Ride* Solver::SelectRide(const Car& c) {
	return &input_.rides.back();
}

CarAssigmentsVec Solver::Solve() {
	CarAssigmentsVec result;
	result.reserve(input_.car_count);

	auto car_compare = [](const Car& c, const Car& d) {
		return c.available_in_tick > d.available_in_tick;
	};
	std::priority_queue<Car, std::vector<Car>, decltype(car_compare)> car_queue(car_compare);

	for (int i = 0; i < input_.car_count; ++i) {
		car_queue.emplace();
	}

	while (!car_queue.empty()) {
		auto top = car_queue.top();
		car_queue.pop();

		auto* ride = SelectRide(top);
		if (!ride) {
			result.push_back({top.rides});
			// car removed from queue
			continue;
		}
		AssignRide(top, *ride);

		car_queue.push(top);
	}


	return result;
}
