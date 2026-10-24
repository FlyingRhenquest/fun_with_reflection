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

#include <fwr/Serializer.h>
#include <serializer_type.h>
#include <iostream>
#include <fstream>

/**
 * This is a very simple program that outputs a header file to
 * a filename. If no filename is specified, it outputs the
 * header file to stdout.
 */

int main(int argc, char *argv[]) {
  std::ostream* out = &std::cout;
  std::ofstream header;
  if (argc == 2) {
    std::string filename(argv[1]);
    header.open(filename);
    out = &header;
  }
  std::vector<std::string> includes;
  includes.push_back(std::string("#include <c_structs.h>"));
  includes.push_back(std::string("#include <stddef.h>")); // For size_t
  SerializerType serializer;
  // Name paramter (last parameter) must match the name of the pointer
  // defined in the extern "C" in the Demo.cpp library
  // Type can be any arbitrary type you want to use to
  // identify the structure type in your C program.
  serializer.emit_header(*out, includes, "demo_serializer", "demo_serialize");
}
