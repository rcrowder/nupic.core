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
* Implementation of Metric class used in monitor mixin framework.
*/

#include <string>
#include <iostream>
#include <sstream>
#include <boost/format.hpp>

#include <nupic/types/Types.hpp>
#include "Instance.hpp"
#include "Trace.hpp"
#include "Metric.hpp"

using namespace boost;
using namespace std;
using namespace nupic;

template<typename TraceType>
Metric<TraceType>::Metric(Instance* monitor, string & title, TraceType & data)
{
  //@param monitor(MonitorMixinBase)  Monitor Mixin instance that generated this trace
  //@param title(string)              Title
  //@param data(list)                 List of numbers to compute metric from
  _monitor = monitor;
  _title = title;

  _min =  1000000000.0;
  _max = -1000000000.0;
  _sum = _mean = _standardDeviation = 0.0;

  _data = data;
  _computeStats();
}

template<typename TraceType>
Metric<TraceType> Metric<TraceType>::createFromTrace(Trace<TraceType>& trace)
{
  Metric<TraceType> ret;
  ret._monitor = *trace._monitor;
  ret._title = trace._title;
  ret._data = trace._data;
  ret._computeStats();

  return ret;
}

template<typename TraceType>
Metric<TraceType> Metric<TraceType>::createFromTrace(Trace<TraceType>& trace, Trace<vector<bool>>& resets)
{
  Metric<TraceType> ret;
  ret._monitor = *trace._monitor;
  ret._title = trace._title;
  ret._data = trace._data;
  ret._computeStats();

  return ret;
}

template<typename TraceType>
Metric<TraceType> Metric<TraceType>::createFromTrace(Trace<TraceType>& trace, Trace<vector<Int>>& resets)
{
  Metric<TraceType> ret;
  ret._monitor = *trace._monitor;
  ret._title = trace._title;
  ret._data = trace._data;
  ret._computeStats();

  return ret;
}

template<typename TraceType>
Metric<TraceType> Metric<TraceType>::copy(const Metric<TraceType>& rhs)
{
  Metric<TraceType> metric = Metric<TraceType>(rhs._monitor, rhs._title);

  metric._min = rhs._min;
  metric._max = rhs._max;
  metric._sum = rhs._sum;
  metric._mean = rhs._mean;
  metric._standardDeviation = rhs._standardDeviation;

  return metric;
}
template<typename TraceType>
Metric<TraceType> copy(const Metric<TraceType>& rhs)
{
  Metric<TraceType> metric = Metric<TraceType>(rhs._monitor, rhs._title);

  metric._min = rhs._min;
  metric._max = rhs._max;
  metric._sum = rhs._sum;
  metric._mean = rhs._mean;
  metric._standardDeviation = rhs._standardDeviation;

  return metric;
}

template<typename TraceType>
string Metric<TraceType>::prettyPrintTitle()
{
  string ret;

  if (_monitor && _monitor->_name.size() > 0)
    ret = "[" + _monitor->_name + "]";
  else
    ret = "[" + _title + "]";

  ret += "{" + _title + "}";

  return ret;
}

template<typename TraceType>
void Metric<TraceType>::_computeStats()
{
  if (_data.size() == 0)
    return;

  _min =  1000000000.0;
  _max = -1000000000.0;
  _sum = _mean = _standardDeviation = 0.0;
}

void MetricsVector::_computeStats()
{
  if (_data.size() == 0)
    return;

  int total = 0;
  for (auto item : _data)
  {
    for (int i : item)
    {
      if (i < _min)
        _min = i;

      if (i > _max)
        _max = i;

      _mean += i;

      total++;
    }
  }
  _sum = _mean;
  _mean /= total;

  Real sum_deviation = 0.0;
  for (auto item : _data)
  {
    for (int i : item)
    {
      sum_deviation += (i - _mean) * (i - _mean);
    }
  }
  _standardDeviation = sqrt(sum_deviation / total);

}

MetricsVector MetricsVector::createFromTrace(Trace<vector<Int>>& trace)
{
  MetricsVector ret;
  ret._monitor = trace._monitor;
  ret._title = trace._title;
  ret._data = trace._data;

  ret._min =  1000000000.0;
  ret._max = -1000000000.0;
  ret._sum = ret._mean = ret._standardDeviation = 0.0;

  ret._computeStats();

  return ret;
}

MetricsVector MetricsVector::createFromTrace(Trace<vector<Int>>& trace, Trace<vector<bool>>& excludesResets)
{
  MetricsVector ret;
  ret._monitor = trace._monitor;
  ret._title = trace._title;

  for (vector<Int> v : trace._data)
  {
    vector<Int> n;

    Int i = 0;
    for (bool r : excludesResets._data[0])
    {
      if (!r)
        n.push_back(v[i]);

      i++;
    }

    ret._data.push_back(n);
  }

  ret._min = 1000000000.0;
  ret._max = -1000000000.0;
  ret._sum = ret._mean = ret._standardDeviation = 0.0;

  ret._computeStats();
  return ret;
}

MetricsVector MetricsVector::createFromTrace(Trace<vector<Int>>& trace, Trace<vector<Int>>& excludesResets)
{
  MetricsVector ret;
  ret._monitor = trace._monitor;
  ret._title = trace._title;

  for (vector<Int> v : trace._data)
  {
    vector<Int> n;

    Int i = 0;
    for (Int r : excludesResets._data[0])
    {
      if (!r)
        n.push_back(v[i]);

      i++;
    }

    ret._data.push_back(n);
  }

  ret._min = 1000000000.0;
  ret._max = -1000000000.0;
  ret._sum = ret._mean = ret._standardDeviation = 0.0;

  ret._computeStats();
  return ret;
}

string MetricsTrace::prettyPrintDatum(Metric<Int>& datum)
{
  stringstream ss;

  ss << format("min: %.2fn") % datum._min;
  ss << format(", max: %.2fn") % datum._max;
  ss << format(", sum: %.2fn") % datum._sum;
  ss << format(", mean: %.2fn") % datum._mean;
  ss << format(", std dev: %.2fn") % datum._standardDeviation;
  return ss.str();
}
