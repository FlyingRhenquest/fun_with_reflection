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

/* Here's your antique... */
#include <demo_header.h>
#include <c_structs.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv) {
  Hello hello;
  /* Reset memory so we don't get random crap past the end of our strings */
  memset(&hello, '\0', sizeof(Hello));
  strcpy(hello.greeting, "Hello");
  strcpy(hello.name, "world!");
  /* kind of overkill for this application... */
  char buffer[4096];
  demo_serialize->Hello(&hello, buffer, 4096);
  printf("%s\n", buffer);
}
