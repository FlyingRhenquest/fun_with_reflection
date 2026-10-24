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
#include "serializer_type.h"

// Create a global Serializer object (constructor inits the
// serialziation structure). If you want to roll multiple
// serializers for various libraries of C++ structs you want
// to serialize, you'll need to be sure you pick unique
// names for all your serializers.

SerializerType demoSerializer;

// Now expose a pointer to our serializer for C.

extern "C" {
  SerializerType::Serialize* demo_serialize = &demoSerializer.s;
}

// All tha remains is to provide a header file that C can understand
