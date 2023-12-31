// Copyright 2005-2020 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// Wrapper for the context dependent rewriter.  Arguments should be, in order,
// 1.) The rewrite transducer (tau).
// 2.) The unweighted left context acceptor (lambda).
// 3.) The unweighted right context acceptor (rho).
// 4.) The unweighted alphabet closure acceptor (sigma*).
// 5.) The string 'ltr', 'rtl', or 'sim' for the direction of rewrite. [opt]
// 6.) The string 'obl' or 'opt' for the rewrite mode. [opt]
//
// If arguments 5 and 6 are omitted, we'll perform a left-to-right and
// obligatory rewrite by default.

#ifndef THRAX_CDREWRITE_H_
#define THRAX_CDREWRITE_H_

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <fst/compat.h>
#include <thrax/compat/compat.h>
#include <fst/fst.h>
#include <fst/vector-fst.h>
#include <thrax/algo/cdrewrite.h>
#include <thrax/algo/getters.h>
#include <thrax/algo/stringcompile.h>
#include <thrax/datatype.h>
#include <thrax/function.h>

DECLARE_bool(save_symbols);  // From util/flags.cc.

namespace thrax {
namespace function {

template <typename Arc>
class CDRewrite : public Function<Arc> {
 public:
  using Transducer = ::fst::Fst<Arc>;
  using MutableTransducer = ::fst::VectorFst<Arc>;
  using Label = typename Arc::Label;

  CDRewrite() {}
  ~CDRewrite() final {}

  std::unique_ptr<DataType> Execute(
      const std::vector<std::unique_ptr<DataType>>& args) const final {
    if (args.size() != 4 && args.size() != 6) {
      std::cout << "CDRewrite: Expected 4 or 6 arguments but received "
                << args.size() << std::endl;
      return nullptr;
    }
    for (int i = 0; i < 4; ++i) {
      if (!args[i]->is<Transducer*>()) {
        std::cout << "CDRewrite: Expect FST for argument " << i + 1
                  << std::endl;
        return nullptr;
      }
    }
    MutableTransducer tau(**args[0]->get<Transducer*>());
    MutableTransducer lambda(**args[1]->get<Transducer*>());
    MutableTransducer rho(**args[2]->get<Transducer*>());
    MutableTransducer sigma(**args[3]->get<Transducer*>());
    // All symbol tables for all transducers must match, and all input symbols
    // must match all output symbols. It makes no sense if they don't. So we
    // perform all these checks.
    //
    // Then, internal to ContextDependentRewrite, we need symbol-table-free
    // versions, since that algorithm constructs a large number of intermediate
    // filters that need to be composed with the pieces that are passed through
    // here. The choices to deal with that are:
    //
    // 1) Change  to add symbol tables to all of the
    //    intermediate filters. This seems messy.
    // 2) Toggle FST_FLAGS_fst_compat_symbols prior to passing this in. This is not
    //    thread-safe.
    // 3) Null out the symbol tables of all of the pieces, pass them in and then
    //    put the symbol table on the result when we are done. This seems the
    //    simplest and safest.
    const ::fst::SymbolTable* symbols = nullptr;
    if (FST_FLAGS_save_symbols) {
      if (!::fst::CompatSymbols(tau.InputSymbols(), tau.OutputSymbols())) {
        std::cout
            << "CDRewrite: input symbols and output symbols must match for tau"
            << std::endl;
        return nullptr;
      }
      if (!::fst::CompatSymbols(lambda.InputSymbols(),
                                    lambda.OutputSymbols())) {
        std::cout << "CDRewrite: input symbols and output symbols must match "
                     "for lambda" << std::endl;
        return nullptr;
      }
      if (!::fst::CompatSymbols(rho.InputSymbols(), rho.OutputSymbols())) {
        std::cout
            << "CDRewrite: input symbols and output symbols must match for rho"
            << std::endl;
        return nullptr;
      }
      if (!::fst::CompatSymbols(sigma.InputSymbols(),
                                    sigma.OutputSymbols())) {
        std::cout << "CDRewrite: input symbols and output symbols must match "
                     "for sigma" << std::endl;
        return nullptr;
      }
      if (!::fst::CompatSymbols(tau.InputSymbols(),
                                    lambda.InputSymbols())) {
        std::cout << "CDRewrite: symbol tables for tau and lambda must match"
                  << std::endl;
        return nullptr;
      }
      if (!::fst::CompatSymbols(tau.InputSymbols(), rho.InputSymbols())) {
        std::cout << "CDRewrite: symbol tables for tau and rho must match"
                  << std::endl;
        return nullptr;
      }
      if (!::fst::CompatSymbols(tau.InputSymbols(), sigma.InputSymbols())) {
        std::cout << "CDRewrite: symbol tables for tau and sigma must match"
                  << std::endl;
        return nullptr;
      }
      symbols = (*args[0]->get<Transducer*>())->InputSymbols();  // tau
      tau.SetInputSymbols(nullptr);
      tau.SetOutputSymbols(nullptr);
      lambda.SetInputSymbols(nullptr);
      lambda.SetOutputSymbols(nullptr);
      rho.SetInputSymbols(nullptr);
      rho.SetOutputSymbols(nullptr);
      sigma.SetInputSymbols(nullptr);
      sigma.SetOutputSymbols(nullptr);
    }
    ::fst::CDRewriteDirection dir = ::fst::LEFT_TO_RIGHT;
    ::fst::CDRewriteMode mode = ::fst::OBLIGATORY;
    if (args.size() == 6) {
      for (int i = 4; i < 6; ++i) {
        if (!args[i]->is<std::string>()) {
          std::cout << "CDRewrite: Expected string for argument " << i + 1
                    << std::endl;
          return nullptr;
        }
      }
      const auto& direction_str = *args[4]->get<std::string>();
      if (!::fst::script::GetCDRewriteDirection(direction_str, &dir)) {
        std::cout << "CDRewrite: Invalid direction: " << direction_str
                  << std::endl;
        return nullptr;
      }
      const auto& mode_str = *args[5]->get<std::string>();
      if (!::fst::script::GetCDRewriteMode(mode_str, &mode)) {
        std::cout << "CDRewrite: Invalid mode: " << mode_str << std::endl;
        return nullptr;
      }
    }
    auto output = std::make_unique<MutableTransducer>();
    ::fst::CDRewriteCompile(tau, lambda, rho, sigma, output.get(), dir,
                                mode, ::fst::kBosIndex,
                                ::fst::kEosIndex);
    if (FST_FLAGS_save_symbols) {
      output->SetInputSymbols(symbols);
      output->SetOutputSymbols(symbols);
    }
    return std::make_unique<DataType>(std::move(output));
  }

 private:
  CDRewrite<Arc>(const CDRewrite<Arc>&) = delete;
  CDRewrite<Arc>& operator=(const CDRewrite<Arc>&) = delete;
};

}  // namespace function
}  // namespace thrax

#endif  // THRAX_CDREWRITE_H_
