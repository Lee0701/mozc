// Copyright 2010-2021, Google Inc.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef MOZC_ENGINE_ENGINE_H_
#define MOZC_ENGINE_ENGINE_H_

#include <memory>
#include <string>
#include <vector>

#include "absl/status/status.h"
#include "absl/status/statusor.h"
#include "absl/strings/string_view.h"
#include "converter/converter.h"
#include "converter/immutable_converter_interface.h"
#include "data_manager/data_manager_interface.h"
#include "dictionary/suppression_dictionary.h"
#include "engine/engine_interface.h"
#include "engine/modules.h"
#include "engine/spellchecker_interface.h"
#include "engine/user_data_manager_interface.h"
#include "prediction/predictor_interface.h"
#include "rewriter/rewriter_interface.h"

namespace mozc {

// Builds and manages a set of modules that are necessary for conversion engine.
class Engine : public EngineInterface {
 public:
  // There are two types of engine: desktop and mobile.  The differences are the
  // underlying prediction engine (DesktopPredictor or MobilePredictor) and
  // learning preference (to learn content word or not).  See Init() for the
  // details of implementation.

  // Creates an instance with desktop configuration from a data manager.  The
  // ownership of data manager is passed to the engine instance.
  static absl::StatusOr<std::unique_ptr<Engine>> CreateDesktopEngine(
      std::unique_ptr<const DataManagerInterface> data_manager);

  // Helper function for the above factory, where data manager is instantiated
  // by a default constructor.  Intended to be used for OssDataManager etc.
  template <typename DataManagerType>
  static absl::StatusOr<std::unique_ptr<Engine>> CreateDesktopEngineHelper() {
    return CreateDesktopEngine(std::make_unique<const DataManagerType>());
  }

  // Creates an instance with mobile configuration from a data manager.  The
  // ownership of data manager is passed to the engine instance.
  static absl::StatusOr<std::unique_ptr<Engine>> CreateMobileEngine(
      std::unique_ptr<const DataManagerInterface> data_manager);

  // Helper function for the above factory, where data manager is instantiated
  // by a default constructor.  Intended to be used for OssDataManager etc.
  template <typename DataManagerType>
  static absl::StatusOr<std::unique_ptr<Engine>> CreateMobileEngineHelper() {
    return CreateMobileEngine(std::make_unique<const DataManagerType>());
  }

  Engine() = default;
  Engine(const Engine &) = delete;
  Engine &operator=(const Engine &) = delete;

  ConverterImpl *GetConverter() const override { return converter_.get(); }
  prediction::PredictorInterface *GetPredictor() const override {
    return predictor_;
  }
  dictionary::SuppressionDictionary *GetSuppressionDictionary() override {
    return modules_.GetMutableSuppressionDictionary();
  }

  bool Reload() override;

  bool ReloadAndWait() override;

  UserDataManagerInterface *GetUserDataManager() override {
    return user_data_manager_.get();
  }

  absl::string_view GetDataVersion() const override {
    return data_manager_->GetDataVersion();
  }

  const DataManagerInterface *GetDataManager() const override {
    return data_manager_.get();
  }

  std::vector<std::string> GetPosList() const override {
    return modules_.GetUserDictionary()->GetPosList();
  }

  void SetSpellchecker(
      const engine::SpellcheckerInterface *spellchecker) override {
    modules_.SetSpellchecker(spellchecker);
  }

 private:
  // Initializes the object by the given data manager and is_mobile flag.
  // The is_mobile flag is used to select DefaultPredictor and MobilePredictor.
  absl::Status Init(std::unique_ptr<const DataManagerInterface> data_manager,
                    bool is_mobile);

  std::unique_ptr<const DataManagerInterface> data_manager_;
  engine::Modules modules_;
  std::unique_ptr<ImmutableConverterInterface> immutable_converter_;

  // TODO(noriyukit): Currently predictor and rewriter are created by this class
  // but owned by converter_. Since this class creates these two, it'd be better
  // if Engine class owns these two instances.
  prediction::PredictorInterface *predictor_ = nullptr;
  RewriterInterface *rewriter_ = nullptr;

  std::unique_ptr<ConverterImpl> converter_;
  std::unique_ptr<UserDataManagerInterface> user_data_manager_;
};

}  // namespace mozc

#endif  // MOZC_ENGINE_ENGINE_H_
