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

/** @file
 * Implementation of Sequence Machine test
 */

#include <string>
#include <sstream>
#include <exception>
#include <vector>
#include <iterator>
#include <functional>

#include "SequenceMachineTest.hpp"

using namespace nupic;

bool SequenceMachineTest::check_pattern_eq(Pattern& p1, Pattern& p2)
{
  for (UInt i = 0; i < p1.size(); i++) {
    if (p1[i] != p2[i]) {
      return false;
    }
  }
  return true;
}

Pattern SequenceMachineTest::get_pattern_diffs(Pattern& p1, Pattern& p2)
{
  vector<int> s1 = p1, s2 = p2;
  vector<int> diffs;

  // For the set_symmetric_difference algorithm to work, the source ranges must be ordered!    
  sort(s1.begin(), s1.end());
  sort(s2.begin(), s2.end());

  // Now that we have sorted ranges (i.e., containers), find the differences    
  set_symmetric_difference(s1.begin(), s1.end(), s2.begin(), s2.end(), back_inserter(diffs));

  return diffs;
}

Pattern SequenceMachineTest::get_pattern_intersections(Pattern& p1, Pattern& p2)
{
  vector<int> s1 = p1, s2 = p2;
  vector<int> diffs;

  // For the set_symmetric_difference algorithm to work, the source ranges must be ordered!    
  sort(s1.begin(), s1.end());
  sort(s2.begin(), s2.end());

  // Now that we have sorted ranges (i.e., containers), find the differences    
  set_intersection(s1.begin(), s1.end(), s2.begin(), s2.end(), back_inserter(diffs));

  return diffs;
}

Pattern SequenceMachineTest::get_pattern_union(Pattern& p1, Pattern& p2)
{
  vector<int> s1(p1), s2(p2);
  vector<int> combined;
  std::vector<int>::iterator it;

  sort(s1.begin(), s1.end());
  sort(s2.begin(), s2.end());

  combined.resize(s1.size()+s2.size());
  it = std::set_union(s1.begin(), s1.end(), s2.begin(), s2.end(), combined.begin());
  combined.resize(it - combined.begin());

  return combined;
}

SequenceMachineTest::SequenceMachineTest()
{
  _patternMachine = ConsecutivePatternMachine();
  _patternMachine.initialize(100, vector<int>{ 5 });

  _sequenceMachine = SequenceMachine(_patternMachine);

}

void SequenceMachineTest::RunTests()
{
  testGenerateFromNumbers();
  testAddSpatialNoise();
  testGenerateNumbers();
  testGenerateNumbersMultipleSequences();
  testGenerateNumbersWithShared();

}

void SequenceMachineTest::testGenerateFromNumbers()
{
  vector<int> numbers, range0, range1, range2;
  range0 = range(0, 10);
  range1 = vector<int>{ -1 };
  range2 = range(10, 19);
  
  numbers.insert(numbers.end(), range0.begin(), range0.end());
  numbers.insert(numbers.end(), range1.begin(), range1.end());
  numbers.insert(numbers.end(), range2.begin(), range2.end());

  Sequence sequence = _sequenceMachine.generateFromNumbers(numbers);

  NTA_CHECK(sequence.size() == 20);
  NTA_CHECK(check_pattern_eq(sequence[0], _patternMachine.get(0)));
  NTA_CHECK(check_pattern_eq(sequence[10], Pattern{ -1 }));
  NTA_CHECK(check_pattern_eq(sequence[11], _patternMachine.get(10)));

}

void SequenceMachineTest::testAddSpatialNoise()
{
  PatternMachine patternMachine;
  patternMachine.initialize(10000, vector<int>{ 1000 });

  SequenceMachine sequenceMachine = SequenceMachine(patternMachine);

  Pattern overlap, numbers = range(0, 100);

  Sequence sequence = sequenceMachine.generateFromNumbers(numbers);
  Sequence noisy = sequenceMachine.addSpatialNoise(sequence, 0.5);

  overlap = get_pattern_intersections(noisy[0], patternMachine.get(0));
  NTA_CHECK((400 < overlap.size()) && (overlap.size() < 600));

  sequence = sequenceMachine.generateFromNumbers(numbers);
  noisy = sequenceMachine.addSpatialNoise(sequence, 0.0);

  overlap = get_pattern_intersections(noisy[0], patternMachine.get(0));
  NTA_CHECK((overlap.size() == 1000));

}

void SequenceMachineTest::testGenerateNumbers()
{
  Sequence numbers = _sequenceMachine.generateNumbers(1, 100);
  Pattern p = range(0, 100);
  NTA_CHECK(check_pattern_eq(numbers[0], p) == false);
  sort(numbers[0].begin(), numbers[0].end());
  NTA_CHECK(check_pattern_eq(numbers[0], p));
}

void SequenceMachineTest::testGenerateNumbersMultipleSequences()
{
  Sequence numbers = _sequenceMachine.generateNumbers(3, 100);
  Pattern p;

  p = range(0, 100);
  sort(numbers[0].begin(), numbers[0].end());
  NTA_CHECK(check_pattern_eq(numbers[0], p));

  p = range(100, 200);
  sort(numbers[1].begin(), numbers[1].end());
  NTA_CHECK(check_pattern_eq(numbers[1], p));

  p = range(200, 300);
  sort(numbers[2].begin(), numbers[2].end());
  NTA_CHECK(check_pattern_eq(numbers[2], p));
}

void SequenceMachineTest::testGenerateNumbersWithShared()
{
  Sequence numbers = _sequenceMachine.generateNumbers(3, 100, { 20, 35 });
  Pattern shared = range(300, 315);
  NTA_CHECK(check_pattern_eq(Pattern(numbers[0].begin() + 20, numbers[0].begin() + 35), shared));
  NTA_CHECK(check_pattern_eq(Pattern(numbers[1].begin() + 20, numbers[1].begin() + 35), shared));
  NTA_CHECK(check_pattern_eq(Pattern(numbers[2].begin() + 20, numbers[2].begin() + 35), shared));
}
