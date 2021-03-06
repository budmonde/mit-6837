/*
    tests/test_eval.cpp -- Usage of eval() and eval_file()

    Copyright (c) 2016 Klemens D. Morgenstern

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE file.
*/


#include <pybind11/eval.h>
#include "pybind11_tests.h"

void init_ex_eval(py::module & m) {
    auto global = py::dict(py::module::import("__main__").attr("__dict__"));

    m.def("test_eval_statements", [global]() {
        auto local = py::dict();
        local["call_test"] = py::cpp_function([&]() -> int {
            return 42;
        });

        auto result = py::eval<py::eval_statements>(
            "print('Hello World!');\n"
            "x = call_test();",
            global, local
        );
        auto x = local["x"].cast<int>();

        return result == py::none() && x == 42;
    });

    m.def("test_eval", [global]() {
        auto local = py::dict();
        local["x"] = py::int_(42);
        auto x = py::eval("x", global, local);
        return x.cast<int>() == 42;
    });

    m.def("test_eval_single_statement", []() {
        auto local = py::dict();
        local["call_test"] = py::cpp_function([&]() -> int {
            return 42;
        });

        auto result = py::eval<py::eval_single_statement>("x = call_test()", py::dict(), local);
        auto x = local["x"].cast<int>();
        return result == py::none() && x == 42;
    });

    m.def("test_eval_file", [global](py::str filename) {
        auto local = py::dict();
        local["y"] = py::int_(43);

        int val_out;
        local["call_test2"] = py::cpp_function([&](int value) { val_out = value; });

        auto result = py::eval_file(filename, global, local);
        return val_out == 43 && result == py::none();
    });

    m.def("test_eval_failure", []() {
        try {
            py::eval("nonsense code ...");
        } catch (py::error_already_set &) {
            PyErr_Clear();
            return true;
        }
        return false;
    });

    m.def("test_eval_file_failure", []() {
        try {
            py::eval_file("non-existing file");
        } catch (std::exception &) {
            return true;
        }
        return false;
    });
}
