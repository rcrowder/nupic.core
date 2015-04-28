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
* ---------------------------------------------------------------------
*/

#ifndef NTA_MONITOR_MIXIN_HPP
#define NTA_MONITOR_MIXIN_HPP

/** @file
* Definition of MonitorMixinBase class used in
* monitor mixin framework.
*/

#include <map>
#include <set>
#include <vector>
#include <string>

#include <nupic\types\Types.hpp>
#include <nupic\algorithms\TemporalMemory.hpp>

#include "Trace.hpp"
#include "Metric.hpp"
#include "Instance.hpp"

namespace nupic
{
  using namespace algorithms::temporal_memory;

  class MonitorMixinBase : public Instance
  {
  protected:
    string _mmName;

    map<string, Trace<vector<UInt>>> _mmTraces;
    map<string, vector<Cell>> _mmData;

    vector<string> sequenceLabel;

    vector<Cell> predictiveCells;
    vector<UInt> predictedActiveCells;
    vector<UInt> predictedInactiveCells;

    vector<UInt> predictedActiveColumns;
    vector<UInt> predictedInactiveColumns;
    vector<UInt> unpredictedActiveColumns;

  public:
    // Base class for MonitorMixin.
    // Each subclass will be a mixin for a particular algorithm.
    // 
    // All arguments, variables, and methods in monitor mixin classes should be
    // prefixed with "mm" (to avoid collision with the classes they mix in to).

    MonitorMixinBase();
    MonitorMixinBase(string& title);

    virtual void compute(vector<UInt> activeColumns, bool learn);// , string sequenceLabel = "");
    virtual void reset();

    virtual void mmClearHistory();

    virtual vector<Trace<vector<UInt>>> mmGetDefaultTraces(int verbosity = 1);
    virtual vector<Metric<vector<UInt>>> mmGetDefaultMetrics(int verbosity = 1);

    virtual string mmPrettyPrintTraces(vector<Trace<vector<UInt>>>& traces, Trace<vector<UInt>>& breakOnResets);
    virtual string mmPrettyPrintMetrics(vector<Metric<vector<UInt>>>& metrics);

  }; // MonitorMixinBase


  // Mixin for TemporalMemory that stores a detailed history, 
  // for inspection and debugging.

  class TemporalMemoryMonitorMixin : public MonitorMixinBase, public TemporalMemory
  {
  protected:
    bool _mmResetActive;
    bool _mmTransitionTracesStale;

  public:
    TemporalMemoryMonitorMixin() {};
    TemporalMemoryMonitorMixin(string& title);

    Trace<vector<UInt>>& mmGetTraceActiveColumns();
    Trace<vector<UInt>>& mmGetTracePredictiveCells();
    Trace<vector<UInt>>& mmGetTraceNumSegments();
    Trace<vector<UInt>>& mmGetTraceNumSynapses();
    Trace<vector<UInt>>& mmGetTraceSequenceLabels();
    Trace<vector<UInt>>& mmGetTraceResets();
    Trace<vector<UInt>>& mmGetTracePredictedActiveCells();
    Trace<vector<UInt>>& mmGetTracePredictedInactiveCells();
    Trace<vector<UInt>>& mmGetTracePredictedActiveColumns();
    Trace<vector<UInt>>& mmGetTracePredictedInactiveColumns();
    Trace<vector<UInt>>& mmGetTraceUnpredictedActiveColumns();

    Metric<vector<UInt>> mmGetMetricSequencesPredictedActiveCellsPerColumn();
    Metric<vector<UInt>> mmGetMetricSequencesPredictedActiveCellsShared();

    Metric<vector<UInt>> mmGetMetricFromTrace(Trace<vector<UInt>>& trace);

    string mmPrettyPrintConnections();
    string mmPrettyPrintSequenceCellRepresentations(string sortby = "Column");

    // ==============================
    // Helper methods
    // ==============================

    void _mmComputeTransitionTraces();

    //==============================
    // Overrides
    // ==============================

    virtual void compute(UInt activeColumns[], bool learn);
    virtual void reset();

    virtual vector<Trace<vector<UInt>>> mmGetDefaultTraces(int verbosity = 1);
    virtual vector<Metric<vector<UInt>>> mmGetDefaultMetrics(int verbosity = 1);

    virtual void mmClearHistory();

  }; // of TemporalMemoryMonitorMixin

}; // of namespace nupic

#endif // of NTA_MONITOR_MIXIN_HPP
