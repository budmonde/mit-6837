import pytest


def test_callbacks():
    from functools import partial
    from pybind11_tests import (test_callback1, test_callback2, test_callback3,
                                test_callback4, test_callback5)

    def func1():
        return "func1"

    def func2(a, b, c, d):
        return "func2", a, b, c, d

    def func3(a):
        return "func3({})".format(a)

    assert test_callback1(func1) == "func1"
    assert test_callback2(func2) == ("func2", "Hello", "x", True, 5)
    assert test_callback1(partial(func2, 1, 2, 3, 4)) == ("func2", 1, 2, 3, 4)
    assert test_callback1(partial(func3, "partial")) == "func3(partial)"
    assert test_callback3(lambda i: i + 1) == "func(43) = 44"

    f = test_callback4()
    assert f(43) == 44
    f = test_callback5()
    assert f(number=43) == 44


def test_lambda_closure_cleanup():
    from pybind11_tests import test_cleanup, payload_cstats

    test_cleanup()
    cstats = payload_cstats()
    assert cstats.alive() == 0
    assert cstats.copy_constructions == 1
    assert cstats.move_constructions >= 1


def test_cpp_function_roundtrip():
    """Test if passing a function pointer from C++ -> Python -> C++ yields the original pointer"""
    from pybind11_tests import dummy_function, dummy_function2, test_dummy_function, roundtrip

    assert test_dummy_function(dummy_function) == "matches dummy_function: eval(1) = 2"
    assert test_dummy_function(roundtrip(dummy_function)) == "matches dummy_function: eval(1) = 2"
    assert roundtrip(None, expect_none=True) is None
    assert test_dummy_function(lambda x: x + 2) == "can't convert to function pointer: eval(1) = 3"

    with pytest.raises(TypeError) as excinfo:
        test_dummy_function(dummy_function2)
    assert "Incompatible function arguments" in str(excinfo.value)

    with pytest.raises(TypeError) as excinfo:
        test_dummy_function(lambda x, y: x + y)
    assert any(s in str(excinfo.value) for s in ("missing 1 required positional argument",
                                                 "takes exactly 2 arguments"))


def test_function_signatures(doc):
    from pybind11_tests import test_callback3, test_callback4

    assert doc(test_callback3) == "test_callback3(arg0: Callable[[int], int]) -> str"
    assert doc(test_callback4) == "test_callback4() -> Callable[[int], int]"
