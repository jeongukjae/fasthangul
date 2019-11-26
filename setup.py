from setuptools import setup, Extension

fasthangul = Extension(
    "fasthangul",
    sources=["fasthangul/PythonExtension.cc"],
    depends=[],
    extra_compile_args=["-std=c++17"],
    language="c++",
)

setup(
    name="fasthangul",
    version="1.0.0a0",
    ext_modules=[fasthangul],
    url="https://github.com/jeongukjae/fasthangul",
    author="Jeong Ukjae",
    author_email="jeongukjae@gmail.com",
)
