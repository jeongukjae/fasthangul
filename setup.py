from setuptools import setup, Extension

fasthangul_jamo = Extension(
    "fasthangul.jamo",
    sources=["fasthangul/jamo.cc"],
    extra_compile_args=["-std=c++17"],
    language="c++",
)

setup(
    name="fasthangul",
    version="1.0.0a0",
    packages=['fasthangul'],
    ext_modules=[fasthangul_jamo],
    url="https://github.com/jeongukjae/fasthangul",
    author="Jeong Ukjae",
    author_email="jeongukjae@gmail.com",
)
