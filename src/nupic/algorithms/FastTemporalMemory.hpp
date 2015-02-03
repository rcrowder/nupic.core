/* ---------------------------------------------------------------------
 * Numenta Platform for Intelligent Computing (NuPIC)
 * Copyright (C) 2013, Numenta, Inc.  Unless you have an agreement
 * with Numenta, Inc., for a separate license for this software code, the
 * following terms and conditions apply:
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see http://www.gnu.org/licenses.
 *
 * http://numenta.org/licenses/
 * ----------------------------------------------------------------------
 */

 /** @file
  * Definitions for the Temporal Memory in C++
  */

#ifndef NTA_fast_temporal_memory_HPP
#define NTA_fast_temporal_memory_HPP

#include <nupic/algorithms/TemporalMemory.hpp>

using namespace std;
using namespace nupic;
using namespace nupic::algorithms::temporal_memory;
using namespace nupic::algorithms::connections;

namespace nupic {
  namespace algorithms {
    namespace fast_temporal_memory {

      /*
       * Class implementing the Temporal Memory algorithm.
       *
       * Uses C++ Connections data structure for optimization.
       */
      class FastTemporalMemory : public TemporalMemory
      {
      public:
        virtual ~FastTemporalMemory() {};

        void initialize();

        /*
         Phase 2 : Burst unpredicted columns.

         Pseudocode :

         - for each unpredicted active column
           - mark all cells as active
           - mark the best matching cell as winner cell
             - (learning)
               - if it has no matching segment
                 - (optimization) if there are prev winner cells
                   - add a segment to it
               - mark the segment as learning

         @param activeColumns    (set)          Indices of active columns in `t`
         @param predictedColumns (set)          Indices of predicted columns in `t`
         @param prevActiveCells  (set)          Indices of active cells in `t-1`
         @param prevWinnerCells  (set)          Indices of winner cells in `t-1`
         @param connections      (Connections)  Connectivity of layer

         @return (tuple)Contains:
           `activeCells`      (set),
           `winnerCells`      (set),
           `learningSegments` (set)
        */
        //tuple<vector<Cell>, vector<Cell>, vector<Segment>> 
        void burstColumns(vector<Int>& activeColumns, 
                          vector<Int>& predictedColumns,
                          vector<Cell>& prevActiveCells, 
                          vector<Cell>& prevWinnerCells, 
                          Connections &connections);

        /*
         Phase 4 : Compute predictive cells due to lateral input
         on distal dendrites.

         Pseudocode :

         - for each distal dendrite segment with activity >= activationThreshold
           - mark the segment as active
           - mark the cell as predictive

         Forward propagates activity from active cells to the synapses that touch
         them, to determine which synapses are active.

         @param activeCells(set)         Indices of active cells in `t`
         @param connections(Connections) Connectivity of layer

         @return (tuple)Contains:
           `activeSegments`  (set),
           `predictiveCells` (set)
        */
        //tuple<vector<Segment>, vector<Cell>>
        void computePredictiveCells(vector<Cell>& activeCells, 
                                    Connections& connections);

      };

    } // end namespace fast_temporal_memory
  } // end namespace algorithms
} // end namespace nta

#endif // NTA_fast_temporal_memory_HPP
