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

#ifndef MOZC_REQUEST_CONVERSION_REQUEST_UTIL_H_
#define MOZC_REQUEST_CONVERSION_REQUEST_UTIL_H_

#include "composer/composer.h"
#include "protocol/commands.pb.h"
#include "request/conversion_request.h"

namespace mozc {

class ConversionRequestUtil {
 public:
  ConversionRequestUtil() = delete;
  ConversionRequestUtil(const ConversionRequestUtil &) = delete;
  ConversionRequestUtil &operator=(const ConversionRequestUtil &) = delete;

  static bool IsHandwriting(const ConversionRequest &request) {
    return request.has_composer() &&
           !request.composer().GetHandwritingCompositions().empty();
  }

  static bool IsAutoPartialSuggestionEnabled(const ConversionRequest &request) {
    return request.request().auto_partial_suggestion();
  }
};

}  // namespace mozc

#endif  // MOZC_REQUEST_CONVERSION_REQUEST_UTIL_H_
