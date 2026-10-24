/**
 * Copyright 2026 Bruce Ide
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#pragma once
#include <fwr/c_strbuf_adapter.h>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <iostream>
#include <string.h>

namespace fr::fwr {
  /**
   * This is a cool almost-C-like function to reflect a structure
   * into a buffer. Returns a 0 if it works OK and a 1 if it does not.
   */
  template <typename T>
  int serialize(T* thing, char* buffer, size_t bufsize) {
    int ret = 0;
    // Let's clear out our buffer each time
    memset(buffer, '\0', bufsize);
    /**
     * This is crazy! You would need some sort of streambuf_adapter...
     */
    c_strbuf_adapter adapter(buffer, bufsize);
    std::ostream out(&adapter); // Uhm... ok...
    try {
      rfl::json::write(thing, out);
    } catch (std::exception &e) {
      int ret = 1;
      std::cout << "Exception caught serializing data: " << e.what() << std::endl;
    }
    return ret;
  }

}
