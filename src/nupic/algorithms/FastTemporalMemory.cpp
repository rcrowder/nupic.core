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
  * Implementation of FastTemporalMemory
  */

#include <algorithm>
#include <vector>

#include <boost/tuple/tuple.hpp>

#include <nupic/algorithms/Connections.hpp>
#include <nupic/algorithms/FastTemporalMemory.hpp>

using namespace std;
using namespace nupic;
using namespace nupic::algorithms::connections;
using namespace nupic::algorithms::temporal_memory;
using namespace nupic::algorithms::fast_temporal_memory;

void FastTemporalMemory::initialize()
{
  TemporalMemory::initialize();
  connections_ = new Connections(numberOfCells());
}


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
tuple<set<Cell>, set<Cell>, set<Segment>> FastTemporalMemory::burstColumns(
  set<Int>& activeColumns,
  set<Int>& predictedColumns,
  set<Cell>& prevActiveCells,
  set<Cell>& prevWinnerCells,
  Connections &connections)
{
  set<Cell> activeCells;
  set<Cell> winnerCells;

  set<Segment> learningSegments;

  vector<Int> unpredictedColumns;

  // Resize to the largest size
  unpredictedColumns.resize(activeColumns.size() + predictedColumns.size());

  // Remove the predicted columns from the currently active columns
  vector<Int>::iterator it = set_difference(
    activeColumns.begin(), activeColumns.end(),
    predictedColumns.begin(), predictedColumns.end(),
    unpredictedColumns.begin());

  // Trim remainer of set
  unpredictedColumns.resize(it - unpredictedColumns.begin());

  for (auto column : unpredictedColumns)
  {
    Segment bestSegment;
    Cell bestCell;

    set<Cell> cells = cellsForColumn(column);

    activeCells_.insert(cells.begin(), cells.end());

    if (connections.mostActiveSegmentForCells(
          cells, prevActiveCells, minThreshold_, bestSegment))
    {
      bestCell = leastUsedCell(cells, connections);

      if (prevWinnerCells.size())
        bestSegment = connections.createSegment(bestCell);
    }
    else
    {
      // TODO : For some reason, bestSegment.cell is garbage - collected after
      // this function returns.So we have to use the below hack.Figure out
      // why and clean up.
      bestCell = Cell(bestSegment.cell.idx);
    }

    winnerCells_.insert(bestCell);

    if (bestSegment.idx >= 0)
      learningSegments.insert(bestSegment);
  }

  return make_tuple(activeCells, winnerCells, learningSegments);
}


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

tuple<set<Segment>, set<Cell>> FastTemporalMemory::computePredictiveCells(
  set<Cell>& activeCells,
  Connections& connections)
{
  Activity activity = 
    connections.computeActivity(activeCells, 
                                connectedPermanence_, 
                                activationThreshold_);

  vector<Segment> segments = connections.activeSegments(activity);
  set<Segment> activeSegments;
  activeSegments.insert(segments.begin(), segments.end());

  vector<Cell> cells = connections.activeCells(activity);
  set<Cell> predictiveCells;
  predictiveCells.insert(cells.begin(), cells.end());;

  return make_tuple(activeSegments, predictiveCells);
}