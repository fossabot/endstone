// Copyright (c) 2023, The Endstone Project. (https://endstone.dev) All Rights Reserved.
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

#pragma once

#include "endstone/permission/permission_default.h"

class PermissibleBase {
public:
    // Delete copy constructor and copy assignment operator
    PermissibleBase(const PermissibleBase &) = delete;
    PermissibleBase &operator=(const PermissibleBase &) = delete;

    // Delete move constructor and move assignment operator
    PermissibleBase(PermissibleBase &&) = delete;
    PermissibleBase &operator=(PermissibleBase &&) = delete;
};
