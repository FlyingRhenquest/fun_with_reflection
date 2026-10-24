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
#include <meta>
#include <ranges>
#include <fwr/templated_function.h>
#include <vector>

namespace fr::fwr {

  /**
   * Accept any number of struct reflections and generate serializers
   * for them
   */
  template <std::meta::info... Types>
  struct Serializer {
    struct Serialize;

    consteval {
      // We'll stash our function specs here
      std::vector<std::meta::info> specs;
      // We can iterate through all of our types using iota
      template for(constexpr size_t i : std::views::iota(0u, sizeof...(Types))) {
        // We can substitute our type into our serialize function
        constexpr auto fnType = std::meta::substitute(^^serialize, {Types...[i]});
        // But we *really* need a pointer to that function
        constexpr auto fnPtr = std::meta::add_pointer(std::meta::type_of(fnType));
        // We'll just use the name of the struct for our method name
        constexpr auto fnName = std::meta::identifier_of(Types...[i]);
        // Add this to our specs
        specs.push_back(std::meta::data_member_spec(fnPtr, { .name = fnName}));
      }
      // And then define the whole schebang into serialize
      std::meta::define_aggregate(^^Serialize, specs);
    }
    // Now that Serialize represents a structure of pointers to functions
    // let's allocate some storage...
    Serialize s;

    // Let's also set up a function to initialize s (I could do this
    // in the constructor but I prefer this)
    void init_s() {
      constexpr static auto ctx = std::meta::access_context::unchecked();
      // Oh yeah, we can reflect on our Serialize structure now too
      constexpr static auto members = std::define_static_array(std::meta::nonstatic_data_members_of(^^Serialize, ctx));
      // The members will conveniently be in the same order as the list of types
      template for(constexpr size_t i : std::views::iota(0u, sizeof...(Types))) {
        // We need the function type again
        constexpr auto fnType = std::meta::substitute(^^serialize, {Types...[i]});
        // And then a little splicing to set the member to the pointer to the
        // function for that member
        s.[: members[i] :] = &[: fnType :];
      }
      // Boom! Initialized!
    }

    Serializer() { init_s(); }

    // There IS one more thing we need to do. We need to emit a header
    // that C will understand.
    void emit_header(std::ostream& out, std::vector<std::string>& includes, std::string serializerType,
                     std::string serializerName) {
      // Isn't this FUN?!
      out << "#ifndef _H_" << serializerType << std::endl;
      out << "#define _H_" << serializerType << std::endl;
      // Emit any includes you include
      for (auto& include : includes) {
        out << include << std::endl;
      }
      out << std::endl;
      out << "typedef struct " << serializerType << " {" << std::endl;

      constexpr static auto ctx = std::meta::access_context::unchecked();
      constexpr static auto members = std::define_static_array(std::meta::nonstatic_data_members_of(^^Serialize, ctx));
      template for (constexpr size_t i : std::views::iota(0u, sizeof...(Types))) {
        constexpr auto memberName = std::meta::identifier_of(members[i]);
        constexpr auto sTypeId = std::meta::identifier_of(Types...[i]);
        // That's actually all we need to construct the function pointer for C
        out << "  int (*" << memberName << ")(" << sTypeId << "*, char*, size_t);" << std::endl;
      }

      out << "} " << serializerType << ";" << std::endl;
      // We'll also set up an extern serializer name for it
      out << "extern " << serializerType << "* " << serializerName << ";" << std::endl;
      out << "#endif" << std::endl;
    }

  };

}
