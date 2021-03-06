/*
    tests/test_constants_and_functions.cpp -- global constants and functions, enumerations, raw byte strings

    Copyright (c) 2016 Wenzel Jakob <wenzel.jakob@epfl.ch>

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE file.
*/

#include "pybind11_tests.h"

enum MyEnum { EFirstEntry = 1, ESecondEntry };

std::string test_function1() {
    return "test_function()";
}

std::string test_function2(MyEnum k) {
    return "test_function(enum=" + std::to_string(k) + ")";
}

std::string test_function3(int i) {
    return "test_function(" + std::to_string(i) + ")";
}

py::bytes return_bytes() {
    const char *data = "\x01\x00\x02\x00";
    return std::string(data, 4);
}

std::string print_bytes(py::bytes bytes) {
    std::string ret = "bytes[";
    const auto value = static_cast<std::string>(bytes);
    for (size_t i = 0; i < value.length(); ++i) {
        ret += std::to_string(static_cast<int>(value[i])) + " ";
    }
    ret.back() = ']';
    return ret;
}

void init_ex_constants_and_functions(py::module &m) {
    m.attr("some_constant") = py::int_(14);

    m.def("test_function", &test_function1);
    m.def("test_function", &test_function2);
    m.def("test_function", &test_function3);

    py::enum_<MyEnum>(m, "MyEnum")
        .value("EFirstEntry", EFirstEntry)
        .value("ESecondEntry", ESecondEntry)
        .export_values();

    m.def("return_bytes", &return_bytes);
    m.def("print_bytes", &print_bytes);
}
