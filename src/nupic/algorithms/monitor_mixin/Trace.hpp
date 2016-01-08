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
* ----------------------------------------------------------------------
*/

/** @file
* Definitions for A record of the past data the algorithm has seen,
* with an entry for each iteration.
*/

#ifndef NTA_trace_classes_HPP
#define NTA_trace_classes_HPP

#include "Instance.hpp"

using namespace std;

namespace nupic
{
  template<typename TraceType>
  class Trace
  {
  public:
    Instance* _monitor;
    string _title;

    vector<TraceType> _data;

  public:
    Trace()
      : _monitor(NULL), _title("") {};

    Trace(Instance* monitor, string& title)
      : _monitor(monitor), _title(title) {};
  };

  template<typename CountsType>
  class CountsTrace : public Trace<CountsType>
  {
  public:
    // Each entry contains counts (for example # of predicted = > active cells).
    CountsTrace() {};
    CountsTrace(Instance* monitor, string& title)
    {
      this->_monitor = monitor;
      this->_title = title;
    };
  };

  class IndicesTrace : public Trace<vector<Int>>
  {
  public:
    IndicesTrace() {};
    IndicesTrace(Instance* monitor, string& title)
    {
      this->_monitor = monitor;
      this->_title = title;
    };

    virtual CountsTrace<Int> makeCountsTrace();
    virtual CountsTrace<Int> makeCumulativeCountsTrace();
  };

  class BoolsTrace : public Trace<bool>
  {
  public:
    // Each entry contains bools(for example resets).
    BoolsTrace() {};
    BoolsTrace(Instance* monitor, string& title)
    {
      this->_monitor = monitor;
      this->_title = title;
    };
  };

  class StringsTrace : public Trace<string>
  {
  public:
    // Each entry contains strings(for example sequence labels).
    StringsTrace() {};
    StringsTrace(Instance* monitor, string& title)
    {
      this->_monitor = monitor;
      this->_title = title;
    };
  };

}; // of namespace nupic

#endif // NTA_trace_classes_HPP
