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
* Definitions for unit tests for Metric class
*/

#ifndef NTA_TRACE_TEST
#define NTA_TRACE_TEST

#include <cstring>
#include <fstream>
#include <stdio.h>
#include <set>

#include <nupic/types/Types.hpp>
#include <nupic/utils/Log.hpp>
#include <nupic/test/Tester.hpp>
#include <nupic/algorithms/monitor_mixin/Trace.hpp>

using namespace std;

namespace nupic {

  class TraceTest : public Tester
  {
  public:
    TraceTest() {}
    virtual ~TraceTest() {}

    // Run all appropriate tests
    virtual void RunTests() override;

  private:
    IndicesTrace  _trace;

    void setUp();

    virtual void testMakeCountsTrace();
    virtual void testMakeCumCountsTrace();

  }; // end class TraceTest

} // end namespace nupic

#endif // NTA_TRACE_TEST
