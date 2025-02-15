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

#include "endstone/server.h"

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "endstone/logger.h"
#include "endstone/plugin/plugin_manager.h"
#include "endstone_python/endstone_python.h"

namespace py = pybind11;

void def_server(py::module &m)
{
    py::class_<Server>(m, "Server")
        .def_property_readonly("logger", &Server::getLogger, py::return_value_policy::reference)
        .def_property_readonly("plugin_manager", &Server::getPluginManager, py::return_value_policy::reference)
        .def_property_readonly("version", &Server::getVersion)
        .def_property_readonly("minecraft_version", &Server::getMinecraftVersion);
}
