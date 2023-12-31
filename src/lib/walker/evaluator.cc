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
#include <string>

#include <fst/flags.h>
#include <fst/fst-decl.h>
#include <thrax/evaluator.h>
#include <thrax/function.h>
#include <thrax/compat/utils.h>
#include <fst/compat.h>

DEFINE_bool(optimize_all_fsts, false,
            "If true, we'll run Optimize[] on all FSTs.");
DEFINE_bool(print_rules, true,
            "If true, we'll print out the rules as we evaluate them.");

namespace thrax {

}  // namespace thrax
