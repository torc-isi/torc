// Torc - Copyright 2011 University of Southern California.  All Rights Reserved.
// $HeadURL$
// $Id$

// This program is free software: you can redistribute it and/or modify it under the terms of the 
// GNU General Public License as published by the Free Software Foundation, either version 3 of the 
// License, or (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See 
// the GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License along with this program.  If 
// not, see <http://www.gnu.org/licenses/>.

/// \file
/// \brief Header for the Placer class.

#ifndef TORC_PLACER_PLACER_HPP
#define TORC_PLACER_PLACER_HPP

#include "torc/architecture/DDB.hpp"
#include "torc/placer/Placement.hpp"
#include "torc/placer/PlacerHeuristicBase.hpp"
#include <boost/timer.hpp>

namespace torc {
namespace placer {

	/// \brief Simulated annealing algorithm class.
	class Placer {
	protected:
	//types
		typedef architecture::DDB DDB;
		typedef boost::uint32_t uint32;
		
		DDB& mDB;
		uint32 mMovesPerTemperature;

	public:
		// the heuristic may be the site mapping structure...
		// heuristic should contain most of the numeric constants here
		// probably functions for cooling schedule and such as well
		// in fact, the heuristic may wrap the placement entirely... as we do in routing
		Placer(DDB& inDB, PlacerHeuristicBase& inHeuristic) : mDB(inDB) {}
		
		~Placer() {}
		
		void generatePlacement(Placement& placement) {
			double initialtemperature = 10000;
			int currentCost = placement.getCost();
			int newCost = 9999999;
			int goodMoves = 0;
			int movesUndone = 0;
			int acceptedBad = 0;
			int zeroCostMoves = 0;
			int bestCost = 999999;
			bool done = false;
			double temperature = initialtemperature;
			double acceptrate = 0;
			int doneCount = 0;
			
			boost::timer epochTimer;
			boost::timer totalTimer;
			
			mMovesPerTemperature = 50000;
			//mMovesPerTemperature = 10 * sqrt(pow(placement.getNumInstances(), 3));
			//mMovesPerTemperature = 100000;
			std::cout << "Moves per temperature: " << mMovesPerTemperature << std::endl;
			
			while (!done) {
				epochTimer.restart();
				placement.updateCostFull(false);
				goodMoves = 0;
				movesUndone = 0;
				acceptedBad = 0;
				zeroCostMoves = 0;
				std::cout << "currentCost for epoch: " << currentCost;
				
				for (uint32 movei = 0; movei < mMovesPerTemperature; movei++) {
					placement.randomMove(false);
					newCost = placement.getCost();
					if (newCost < bestCost) {
						bestCost = newCost;
						//fp.savePlacement();
					}
					if (currentCost < newCost) {
						//if (mRandom.nextDouble() >= 
						//	Math.exp(((double)currentCost - (double)newCost / temperature))
						double irand = (double)rand() / (double)(std::numeric_limits<int>::max());
						if (irand >= exp(((double)currentCost - (double)newCost) / temperature)) {
							movesUndone++;
							placement.undoMove(false);
							//std::cout << " UNDO" << std::endl;
						} else {
							acceptedBad++;
							currentCost = newCost;
							std::cout << " ACCEPT BAD" << std::endl;
						}
					} else if (currentCost == newCost) {
						zeroCostMoves++;
					} else {
						goodMoves++;
						currentCost = newCost;
					}
				}
				
				double epochTime = epochTimer.elapsed();
				//currentCost = newCost;
				acceptrate = (double)acceptedBad / ((double)acceptedBad + (double)movesUndone);
				
				// temperature adjustment
				double tempadjust = 0;
				if (acceptrate > 0.96) {
					temperature = 0.5 * temperature;
					tempadjust = (double)0.5;
				} else if (acceptrate <= 0.96 && acceptrate > 0.8) {
					temperature = 0.9 * temperature;
					tempadjust = (double)0.9;
				} else if (acceptrate <= 0.8 && acceptrate > 0.15) {
					temperature = 0.95 * temperature;
					tempadjust = (double)0.95;
				} else {
					temperature = 0.8 * temperature;
					tempadjust = (double)0.8;
				}
				
				// termination condition
				//if (acceptedBad == 0)
				if (goodMoves == 0) {
					doneCount++;
				} else {
					doneCount = 0;
				}
				if (doneCount == 10) {
					done = true;
				}
				
				//%.3f%n
				std::cout << " Good moves: " << goodMoves;
				std::cout << " Accepted bad: " << acceptedBad;
				std::cout << " Undone: " << movesUndone;
				std::cout << " ZeroCostMoves: " << zeroCostMoves;
				std::cout << " Best Cost: " << bestCost;
				std::cout << " Acceptance rate: " << acceptrate;
				std::cout << " new temp: " << temperature;
				std::cout << " tempadjust: " << tempadjust;
				//std::cout << " time: " << epochTimer.elapsed();
				std::cout << " time: " << epochTime;
				//std::cout << " | ";
				std::cout << std::endl;
				//fp.getCost(temperature, initialtemperature, true);
			}
			std::cout << "Total time: " << totalTimer.elapsed();
			std::cout << std::endl;
			//fp.displayPlacementWithCost();
			//std::cout << "Here's where we ended:";
			//fp.displayPlacement();
			//((AmorphousFloorPlan)fp).getCostAllPairsDistance(true);
			//fp.restorePlacement();
			//fp.validatePlacement();
			//fp.displayPlacement();
			placement.updateCostFull(false);
			std::cout << "Actual end cost: " << placement.getCost() << std::endl;
		}
		
	}; // class Placer
} // namespace placer
} // namespace torc

#endif // TORC_PLACER_PLACER_HPP
