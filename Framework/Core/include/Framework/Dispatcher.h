// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See http://alice-o2.web.cern.ch/license for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.

/// \file Dispatcher.h
/// \brief Declaration of Dispatcher for O2 Data Sampling
///
/// \author Piotr Konopka, piotr.jan.konopka@cern.ch

#ifndef ALICEO2_DISPATCHER_H
#define ALICEO2_DISPATCHER_H

#include "Framework/DataProcessorSpec.h"
#include "Framework/DeviceSpec.h"
#include "Framework/DataSamplingPolicy.h"
#include "Framework/DataSamplingHeader.h"
#include "Framework/Task.h"

namespace o2
{
namespace framework
{

class Dispatcher : public Task
{
 public:
  /// \brief Constructor
  Dispatcher(const std::string name, const std::string reconfigurationSource);
  /// \brief Destructor
  ~Dispatcher() override;

  /// \brief Dispatcher init callback
  void init(InitContext& ctx) override;
  /// \brief Dispatcher process callback
  void run(ProcessingContext& ctx) override;

  /// \brief Create appropriate inputSpecs and outputSpecs for sampled data during the workflow declaration phase.
  void registerPath(const std::pair<InputSpec, OutputSpec>&);

  const std::string& getName();
  Inputs getInputSpecs();
  Outputs getOutputSpecs();

 private:
  DataSamplingHeader prepareDataSamplingHeader(const DataSamplingPolicy& policy, const DeviceSpec& spec);
  void send(DataAllocator& dataAllocator, const DataRef& inputData, Output&& output) const;
  void sendFairMQ(FairMQDevice* device, const DataRef& inputData, const std::string& fairMQChannel, DataSamplingHeader&& dsHeader) const;

  std::string mName;
  std::string mReconfigurationSource;
  Inputs inputs;
  Outputs outputs;
  // policies should be shared between all pipeline threads
  std::vector<std::shared_ptr<DataSamplingPolicy>> mPolicies;
};

} // namespace framework
} // namespace o2

#endif //ALICEO2_DISPATCHER_H
