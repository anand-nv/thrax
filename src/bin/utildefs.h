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
// Definitions needed by various utilities here.

#ifndef NLP_GRM_LANGUAGE_UTIL_UTILDEFS_H_
#define NLP_GRM_LANGUAGE_UTIL_UTILDEFS_H_

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <fst/compat.h>
#include <thrax/compat/compat.h>
#include <fst/arc.h>
#include <fst/fst.h>
#include <fst/symbol-table.h>
#include <fst/vector-fst.h>
#include <thrax/grm-manager.h>

namespace thrax {

// Computes the n-shortest paths and returns a vector of strings, each string
// corresponding to each path. The mapping of labels to strings is controlled by
// the type and the symtab. Elements that are in the generated label set from
// the grammar are output as "[name]" where "name" is the name of the generated
// label. Paths are sorted in ascending order of weights.
bool FstToStrings(const ::fst::VectorFst<::fst::StdArc> &fst,
                  std::vector<std::pair<std::string, float>> *strings,
                  const ::fst::SymbolTable *generated_symtab,
                  ::fst::TokenType type = ::fst::TokenType::BYTE,
                  ::fst::SymbolTable *symtab = nullptr, size_t n = 1);

// Returns a string for each path of `fst`, in an arbitrary order.
//
// The mapping of labels to strings is controlled by the type and the symtab.
// Elements that are in the generated label set from the grammar are output as
// "[name]" where "name" is the name of the generated label.
//
// `fst` must not be cyclic -- if it is, this function will return false
// and produce no results. For cyclic FSTs, use `FstToStrings`.
bool FstToAllStrings(const ::fst::VectorFst<::fst::StdArc> &fst,
                     std::vector<std::pair<std::string, float>> *strings,
                     const ::fst::SymbolTable *generated_symtab,
                     ::fst::TokenType type = ::fst::TokenType::BYTE,
                     ::fst::SymbolTable *symtab = nullptr);

// Find the generated labels from the grammar.
std::unique_ptr<::fst::SymbolTable> GetGeneratedSymbolTable(
    GrmManagerSpec<::fst::StdArc> *grm);

}  // namespace thrax

#endif  // NLP_GRM_LANGUAGE_UTIL_UTILDEFS_H_
