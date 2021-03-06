/*
    pybind11/exec.h: Support for evaluating Python expressions and statements
    from strings and files

    Copyright (c) 2016 Klemens Morgenstern <klemens.morgenstern@ed-chemnitz.de> and
                       Wenzel Jakob <wenzel.jakob@epfl.ch>

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE file.
*/

#pragma once

#pragma once

#include "pybind11.h"

NAMESPACE_BEGIN(pybind11)

enum eval_mode {
    /// Evaluate a string containing an isolated expression
    eval_expr,

    /// Evaluate a string containing a single statement. Returns \c none
    eval_single_statement,

    /// Evaluate a string containing a sequence of statement. Returns \c none
    eval_statements
};

template <eval_mode mode = eval_expr>
object eval(str expr, object global = object(), object local = object()) {
    if (!global) {
        global = object(PyEval_GetGlobals(), true);
        if (!global)
            global = dict();
    }
    if (!local)
        local = global;

    /* PyRun_String does not accept a PyObject / encoding specifier,
       this seems to be the only alternative */
    std::string buffer = "# -*- coding: utf-8 -*-\n" + (std::string) expr;

    int start;
    switch (mode) {
        case eval_expr:             start = Py_eval_input;   break;
        case eval_single_statement: start = Py_single_input; break;
        case eval_statements:       start = Py_file_input;   break;
        default: pybind11_fail("invalid evaluation mode");
    }

    object result(PyRun_String(buffer.c_str(), start, global.ptr(), local.ptr()), false);

    if (!result)
        throw error_already_set();
    return result;
}

template <eval_mode mode = eval_statements>
object eval_file(str fname, object global = object(), object local = object()) {
    if (!global) {
        global = object(PyEval_GetGlobals(), true);
        if (!global)
            global = dict();
    }
    if (!local)
        local = global;

    int start;
    switch (mode) {
        case eval_expr:             start = Py_eval_input;   break;
        case eval_single_statement: start = Py_single_input; break;
        case eval_statements:       start = Py_file_input;   break;
        default: pybind11_fail("invalid evaluation mode");
    }

    int closeFile = 1;
    std::string fname_str = (std::string) fname;
#if PY_VERSION_HEX >= 0x03040000
    FILE *f = _Py_fopen_obj(fname.ptr(), "r");
#elif PY_VERSION_HEX >= 0x03000000
    FILE *f = _Py_fopen(fname.ptr(), "r");
#else
    /* No unicode support in open() :( */
    object fobj(PyFile_FromString(
        const_cast<char *>(fname_str.c_str()),
        const_cast<char*>("r")), false);
    FILE *f = nullptr;
    if (fobj)
        f = PyFile_AsFile(fobj.ptr());
    closeFile = 0;
#endif
    if (!f) {
        PyErr_Clear();
        pybind11_fail("File \"" + fname_str + "\" could not be opened!");
    }

    object result(PyRun_FileEx(f, fname_str.c_str(), start, global.ptr(),
                               local.ptr(), closeFile),
                  false);

    if (!result)
        throw error_already_set();

    return result;
}

NAMESPACE_END(pybind11)
