// main.cpp
//
// ICS 46 Spring 2017
// Project #4: Rock and Roll Stops the Traffic
//
// This is the program's main() function, which is the entry point for your
// console user interface.
#include "TripReader.hpp"
#include "RoadMapReader.hpp"
#include "RoadMapWriter.hpp"
#include <iostream>

int main()
{
    InputReader inputReader = InputReader(std::cin);
    RoadMapReader roadMapReader;
    TripReader tripReader;
    RoadMap roadMap = roadMapReader.readRoadMap(inputReader);
    std::vector<Trip> trips = tripReader.readTrips(inputReader);

    std::function<double(const RoadSegment&)> shortestPath = [](RoadSegment roadSegment)
        {
        return roadSegment.miles;
        };
    std::function<double(const RoadSegment&)> shortestTime = [](RoadSegment roadSegment)
        {
        return roadSegment.milesPerHour/roadSegment.miles;
        };
    return 0;
}

