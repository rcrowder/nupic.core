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
* Implementation of unit tests for Metric class
*/

#include <cstring>
#include <fstream>
#include <stdio.h>

#include <nupic/types/Types.hpp>
#include <nupic/utils/Log.hpp>
#include "TraceTest.hpp"

using namespace std;

namespace nupic {

  void TraceTest::setUp()
  {
    string indicesTitle = "active cells";
    _trace = IndicesTrace(NULL, indicesTitle);
    _trace._data.push_back({ 1, 2, 3 });
    _trace._data.push_back({ 4, 5 });
    _trace._data.push_back({ 6 });
    _trace._data.push_back({});
  }

  void TraceTest::RunTests()
  {
    setUp();

    testMakeCountsTrace();
    testMakeCumCountsTrace();
  }

  void TraceTest::testMakeCountsTrace()
  {
    CountsTrace<Int> countsTrace = _trace.makeCountsTrace();
    NTA_CHECK(countsTrace._title == string("# active cells"));
    vector<Int> expectedValues = { 3, 2, 1, 0 };
    NTA_CHECK(countsTrace._data == expectedValues);
  }

  void TraceTest::testMakeCumCountsTrace()
  {
    CountsTrace<Int> countsTrace = _trace.makeCumulativeCountsTrace();
    NTA_CHECK(countsTrace._title == string("# (cumulative) active cells"));
    vector<Int> expectedValues = { 3, 5, 6, 6 };
    NTA_CHECK(countsTrace._data == expectedValues);
  }

}; // of namespace nupic
