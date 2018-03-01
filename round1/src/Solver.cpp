#include "Solver.h"
#include <queue>
#include <cassert>
#include <tuple>
#include <algorithm>
#include <iostream>

bool CompareScores(const ScoreType& a, const ScoreType& b) {
	return std::get<0>(a) * std::get<1>(b) < std::get<1>(a) * std::get<0>(b);
}

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

    int sum_ride_length = distance(car, ride) + ride.length();
    int end_tick = std::max(car.available_in_tick + sum_ride_length,
                ride.earliest_start + ride.length());

    return ScoreType{GetScore(car, ride), end_tick - car.available_in_tick};
}

ScoreType Solver::ScoreCarRide(const Car& car, const Ride& ride, int lookahead,
	std::vector<const Ride*> continuations)
{
	assert(CanSatisfyRide(car, ride));

	auto scoreOfThisLeg = ScoreCarRide(car, ride);

	Ride* nextContinuation = nullptr;
	ScoreType nextScore = {};
	int distanceOfLegs = 0;
	if (lookahead > 0) {
		int sum_ride_length = distance(car, ride) + ride.length();
		Car carAfterRide{0, ride.end_row, ride.end_col};
		carAfterRide.available_in_tick =
			std::max(car.available_in_tick + sum_ride_length,
					ride.earliest_start + ride.length());
		std::tie(nextContinuation, nextScore) =
			SelectRide(carAfterRide, lookahead-1, continuations);

		if (!nextContinuation) {
			nextScore = {};
			distanceOfLegs = 0;
		}
	} else {
		static int counter = 0;
		if (++counter % 10000 == 0) {
		}
	}

	return ScoreType{std::get<0>(nextScore) + std::get<0>(scoreOfThisLeg),
		std::get<1>(nextScore) + std::get<1>(scoreOfThisLeg)};
}

void Solver::AssignRide(Car& car, Ride& ride) {
	assert(CanSatisfyRide(car, ride));

	full_score_ += GetScore(car, ride);

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

std::pair<Ride*, ScoreType> Solver::SelectRide(const Car& c, int lookahead,
	std::vector<const Ride*> continuations)
{
	assert(c.available_in_tick <= /* < ? */ input_.step_count);
	Ride* best_ride = nullptr;
	ScoreType best_score = {};
	for (Ride& ride : input_.rides) {
		if (std::find(continuations.begin(), continuations.end(), &ride) !=
			continuations.end())
		{
			continue;
		}
		if (CanSatisfyRide(c, ride)) {
			auto ride_score = ScoreCarRide(c, ride, lookahead, continuations);
			if (!best_ride || CompareScores(best_score, ride_score)) {
				best_ride = &ride;
				best_score = ride_score;
			}
		}
	}
	return std::make_pair(best_ride, best_score);
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

		std::cerr << "Currently at tick: " << top.available_in_tick << std::endl;
		const auto& currentTick = top.available_in_tick;
		for (Ride& ride: input_.rides) {
			if (ride.latest_finish <= currentTick) {
				std::swap(ride, input_.rides.back());
				input_.rides.pop_back();
			}
		}
		auto ride = SelectRide(top, 1);
		if (!ride.first) {
			result.push_back({top.rides});
			// car removed from queue
			continue;
		}
		AssignRide(top, *ride.first);

		car_queue.push(top);
	}


	return result;
}
