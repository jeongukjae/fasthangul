from distutils.core import setup, Extension

fasthangul = Extension(
    "fasthangul",
    sources=["fasthangul/PythonExtension.cc"],
    extra_compile_args=["-std=c++17"],
    language="c++",
)
fasthangul_jamo = Extension(
    "fasthangul.jamo",
    sources=["fasthangul/jamo.cc"],
    depends=["fasthangul/jamo.hh"],
    extra_compile_args=["-std=c++17"],
    language="c++",
)

setup(
    name="fasthangul",
    version="1.0.0a0",
    ext_modules=[fasthangul, fasthangul_jamo],
    url="https://github.com/jeongukjae/fasthangul",
    author="Jeong Ukjae",
    author_email="jeongukjae@gmail.com",
)
