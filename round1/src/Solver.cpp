#include "Solver.h"
#include <queue>
#include <cassert>
#include <tuple>

bool Solver::CanSatisfyRide(const Car& car, const Ride& ride) {
	int sum_ride_length = distance(car, ride) + ride.length();
	return car.available_in_tick + sum_ride_length < ride.latest_finish;
}

bool Solver::CanGetBonus(const Car& car, const Ride& ride) {
	return car.available_in_tick + distance(car, ride) <= ride.earliest_start;
}

int Solver::GetScore(const Car& car, const Ride& ride) {
	return CanGetBonus(car, ride) * input_.bonus_for_starting_on_time +
		ride.length();
}

ScoreType Solver::ScoreCarRide(const Car& car, const Ride& ride) {
	assert(CanSatisfyRide(car, ride));

	int abs_wait = std::abs(
		car.available_in_tick + distance(car, ride) - ride.earliest_start);
	return ScoreType{-ride.length(), -abs_wait, -distance(car, ride)};
	return ScoreType{-abs_wait, ride.length(), -distance(car, ride)};
}

void Solver::AssignRide(Car& car, Ride& ride) {
	assert(CanSatisfyRide(car, ride));

	int sum_ride_length = distance(car, ride) + ride.length();
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
	assert(c.available_in_tick <= /* < ? */ input_.step_count);
	Ride* best_ride = nullptr;
	ScoreType best_score = {};
	for (Ride& ride : input_.rides) {
		if (CanSatisfyRide(c, ride)) {
			auto ride_score = ScoreCarRide(c, ride);
			if (!best_ride || best_score < ride_score) {
				best_ride = &ride;
				best_score = ride_score;
			}
		}
	}
	return best_ride;
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
