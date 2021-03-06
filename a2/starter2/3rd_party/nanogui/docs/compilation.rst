Compilation
========================================================================================

NanoGUI uses a CMake build system to ensure portability. All dependencies are
cloned and compiled in one batch, which should generally reduce the amount of
configuration effort to zero. Assuming that NanoGUI was cloned into the current
working directory, the following commands need to be executed:

.. code-block:: bash

   # enter the top-level NanoGUI directory
   $ cd nanogui

   # make a build directory and enter that
   $ mkdir build
   $ cd build

   # generate your Makefile
   $ cmake ..

   # now that you have a Makefile, use that to build
   $ make -j 4

For Windows, the process is nearly the same:

.. code-block:: bash

   # enter the top-level NanoGUI directory
   $ cd nanogui

   # make a build directory and enter that
   $ mkdir build
   $ cd build

   # Specify VS Version AND 64bit, otherwise it defaults to 32.
   # The version number and year may be different for you, Win64
   # can be appended to any of them.
   # 32 bit Windows builds are /not/ supported
   $ cmake -G "Visual Studio 14 2015 Win64" ..

   # Either open the .sln with Visual Studio, or run
   $ cmake --build . --config Release

Default Configurations
----------------------------------------------------------------------------------------

By default, NanoGUI will

+---------------------------------+---------------------------+
| Impact / effect                 | CMake Option              |
+=================================+===========================+
| Build the example programs.     | ``NANOGUI_BUILD_EXAMPLE`` |
+---------------------------------+---------------------------+
| Build as a *shared* library.    | ``NANOGUI_BUILD_SHARED``  |
+---------------------------------+---------------------------+
| Build the Python plugins.       | ``NANOGUI_BUILD_PYTHON``  |
+---------------------------------+---------------------------+
| Use GLAD if on Windows.         | ``NANOGUI_USE_GLAD``      |
+---------------------------------+---------------------------+
| Generate an ``install`` target. | ``NANOGUI_INSTALL``       |
+---------------------------------+---------------------------+

Users developing projects that reference NanoGUI as a ``git submodule`` (this
is **strongly** encouraged) can set up the parent project's CMake configuration
file as follows (this assumes that ``nanogui`` lives in the directory
``ext/nanogui`` relative to the parent project):

.. code-block:: cmake

    # Disable building extras we won't need (pure C++ project)
    set(NANOGUI_BUILD_EXAMPLE OFF CACHE BOOL " " FORCE)
    set(NANOGUI_BUILD_PYTHON  OFF CACHE BOOL " " FORCE)
    set(NANOGUI_INSTALL       OFF CACHE BOOL " " FORCE)

    # Add the configurations from nanogui
    add_subdirectory(ext/nanogui)

    # For reliability of parallel build, make the NanoGUI targets dependencies
    set_property(TARGET glfw glfw_objects bin2c nanogui PROPERTY FOLDER "dependencies")

Required Variables Exposed
----------------------------------------------------------------------------------------

Due to the nature of building an OpenGL application for different platforms, three
variables are populated to allow for easy incorporation with your CMake build.  After
you have executed ``add_subdirectory`` as shown above, you will need to add the
following (assuming the target you are building is called ``myTarget``):

.. code-block:: cmake

   # Various preprocessor definitions have been generated by NanoGUI
   add_definitions(${NANOGUI_EXTRA_DEFS})

   # On top of adding the path to nanogui/include, you may need extras
   include_directories(${NANOGUI_EXTRA_INCS})

   # Compile a target using NanoGUI
   add_executable(myTarget myTarget.cpp)

   # Lastly, additional libraries may have been built for you.  In addition to linking
   # against NanoGUI, we need to link against those as well.
   target_link_libraries(myTarget nanogui ${NANOGUI_EXTRA_LIBS})

.. nanogui_compiling_the_docs:

Compiling the Documentation
----------------------------------------------------------------------------------------

The documentation system relies on 'Doxygen', 'Sphinx', 'Breathe', and
'Exhale'.  It uses the 'Read the Docs' theme for the layout of the generated
html.  So you will need to first

1. Install Doxygen for your operating system.  On Unix based systems, this
   should be available through your package manager (apt-get, brew, dnf, etc).

2. Install Sphinx, Breathe, and the theme:

   .. code-block:: py

      pip install breathe sphinx_rtd_theme

Now that you have the relevant tools, you can build the documentation with

.. code-block:: bash

   # Enter the documentation directory
   $ cd <path/to/nanogui>/docs

   # Build the documentation
   $ make html

The output will be generated in ``_build``, the root html document is located
at ``_build/html/index.html``.

.. note::

   When building the documentation locally, there can be subtle differences in
   the rendered pages than what is hosted online.  You should largely be able
   to ignore this.
