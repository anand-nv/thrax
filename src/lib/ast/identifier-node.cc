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
#include <thrax/identifier-node.h>

#include <ctype.h>

#include <string>
#include <vector>

#include <thrax/node.h>
#include <thrax/walker.h>

namespace thrax {

static bool ComponentIsValid(const std::string& s) {
  if (s.empty()) return false;
  bool found_underscore = false;
  bool found_number = false;
  bool found_alpha = false;
  for (char c : s) {
    if (isalpha(c)) {
      found_alpha = true;
    } else if (c == '_') {
      found_underscore = true;
    } else if (isdigit(c)) {
      found_number = true;
    } else {
      return false;
    }
  }
  return !isdigit(s[0]) && (found_alpha || (found_underscore && found_number));
}

IdentifierNode::IdentifierNode(const std::string& name)
    : IdentifierNode(name, -1) {}

IdentifierNode::IdentifierNode(const std::string& name, int begin_pos)
    : Node(), full_name_(name), begin_pos_(begin_pos) {
  namespaces_ = ::fst::StrSplit(full_name_, '.');
  identifier_ = namespaces_.back();
  namespaces_.pop_back();
  valid_ = CalculateValidity();
}

IdentifierNode::const_iterator IdentifierNode::begin() const {
  return namespaces_.begin();
}

IdentifierNode::const_iterator IdentifierNode::end() const {
  return namespaces_.end();
}

bool IdentifierNode::HasNamespaces() const { return !namespaces_.empty(); }

const std::string& IdentifierNode::GetIdentifier() const { return identifier_; }

const std::string& IdentifierNode::Get() const { return full_name_; }

int IdentifierNode::GetBeginPos() const { return begin_pos_; }

bool IdentifierNode::IsValid() const { return valid_; }

void IdentifierNode::Accept(AstWalker* walker) { walker->Visit(this); }

bool IdentifierNode::CalculateValidity() {
  for (const auto& ns : namespaces_) {
    if (!ComponentIsValid(ns)) return false;
  }
  return ComponentIsValid(identifier_);
}

}  // namespace thrax
