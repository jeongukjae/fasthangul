from setuptools import setup, Extension

fasthangul_jamo = Extension(
    "fasthangul.jamo",
    sources=["../../Sources/jamo.cc", "fasthangul/jamo.cc",],
    include_dirs=["../../Includes"],
    extra_compile_args=["-std=c++17"],
    language="c++",
)

fasthangul_normalizer = Extension(
    "fasthangul.normalizer",
    sources=["../../Sources/normalizer.cc", "fasthangul/normalizer.cc",],
    include_dirs=["../../Includes"],
    extra_compile_args=["-std=c++17"],
    language="c++",
)

setup(
    name="fasthangul",
    version="1.0.0a1",
    python_requires=">=3.5",
    packages=["fasthangul"],
    ext_modules=[fasthangul_jamo, fasthangul_normalizer],
    url="https://github.com/jeongukjae/fasthangul",
    author="Jeong Ukjae",
    author_email="jeongukjae@gmail.com",
    classifiers=[
        "Development Status :: 2 - Pre-Alpha",
        "License :: OSI Approved :: MIT License",
        "Natural Language :: Korean",
        "Programming Language :: C++",
        "Programming Language :: Python :: 3",
        "Programming Language :: Python :: 3.5",
        "Programming Language :: Python :: 3.6",
        "Programming Language :: Python :: 3.7",
        "Programming Language :: Python :: Implementation :: CPython",
    ],
)
