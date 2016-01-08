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
* Definitions for an abstract computational interface for modules, such as poolers
*/

#ifndef NTA_Instance_HPP
#define NTA_Instance_HPP

#include <string>
#include <vector>
#include <nupic/types/Types.hpp>

class Instance
{
public:
  std::string _name;
  bool _learn;

  Instance() :
    _name(""), _learn(false) {};

  virtual void mmReset() = 0;
  virtual void mmCompute(std::vector<nupic::UInt> pattern, bool learn) = 0;
};

typedef void ComputeFunction(std::vector<nupic::UInt>& pattern, Instance& instance);

#endif // NTA_Instance_HPP
