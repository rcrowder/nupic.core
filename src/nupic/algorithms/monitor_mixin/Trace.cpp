/* ---------------------------------------------------------------------
* Numenta Platform for Intelligent Computing (NuPIC)
* Copyright (C) 2016, Numenta, Inc.  Unless you have an agreement
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
* ---------------------------------------------------------------------
*/

/** @file
* Implementation of TM mixin that enables detailed monitoring of history.
*/

#include <map>
#include <string>
#include <sstream>
#include <iostream>
#include <algorithm>

#include "Trace.hpp"

using namespace nupic;

CountsTrace<Int> IndicesTrace::makeCountsTrace()
{
  //@return (CountsTrace)A new Trace made up of counts of this trace's indices.
  string title = "# " + _title;
  CountsTrace<Int> trace(_monitor, title);

  for (vector<Int> indicies : _data)
  {
    trace._data.push_back({ (Int)indicies.size() });
  }

  return trace;
}

CountsTrace<Int> IndicesTrace::makeCumulativeCountsTrace()
{
  //@return (CountsTrace)A new Trace made up of cumulative counts of this trace's indices.
  string title = "# (cumulative) " + _title;
  CountsTrace<Int> trace(_monitor, title);

  CountsTrace<Int> countsTrace = makeCountsTrace();
  Int runningTotal = 0;
  for (vector<Int> indicies : _data)
  {
    runningTotal += (Int)indicies.size();
    trace._data.push_back({ runningTotal });
  }
  
  return trace;
}
