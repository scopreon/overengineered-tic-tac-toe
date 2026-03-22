from glob import glob
from setuptools import setup
from pybind11.setup_helpers import Pybind11Extension

ext_modules = [
    Pybind11Extension(
        "tic_tac_toe._bindings",
        sorted(glob("lib/*.cpp")),  # Sort source files for reproducibility
        cxx_std=17,
    ),
]

setup(
    name="my-project",
    ext_modules=ext_modules,
    entry_points={"console_scripts": ["tic-tac-toe=tic_tac_toe.main:main"]},
)
