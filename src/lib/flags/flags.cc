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
// List of common command line flags that multiple modules may need to include.

#include <fst/compat.h>
#include <thrax/compat/compat.h>
#include <fst/flags.h>

DEFINE_bool(save_symbols, false,
            "Keep symbol tables associated with generated fsts.");

DEFINE_string(indir, "", "The directory with the source files.");
DEFINE_string(outdir, "", "The directory in which we'll write the output.");
