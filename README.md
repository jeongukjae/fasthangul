# fasthangul

![Test Python Status](https://github.com/jeongukjae/fasthangul/workflows/Test%20Python/badge.svg)
![Test C++ Status](https://github.com/jeongukjae/fasthangul/workflows/Test%20C++/badge.svg)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/54b43aedda274679b37a965ac133dcd3)](https://www.codacy.com/manual/jeongukjae/fasthangul?utm_source=github.com&utm_medium=referral&utm_content=jeongukjae/fasthangul&utm_campaign=Badge_Grade)
[![codecov](https://codecov.io/gh/jeongukjae/fasthangul/branch/master/graph/badge.svg)](https://codecov.io/gh/jeongukjae/fasthangul)

![Py Versions](https://img.shields.io/pypi/pyversions/fasthangul)
![PyPi Versions](https://img.shields.io/pypi/v/fasthangul)
![License](https://img.shields.io/pypi/l/fasthangul)

한국어 처리를 제대로, 빠르게 하기 위해 작성한 라이브러리입니다. 기능을 하나씩 추가해나갈 예정입니다.

- [Python 벤치마크](./Benchmarks/Python)

## Python 사용법

### 설치

```shell
pip install fasthangul
```

### 자모 분리하고 합치기

```python
>>> from fasthangul.jamo import compose, decompose
>>> decompose('안녕하세요')
'ㅇㅏㄴㄴㅕㅇㅎㅏㅅㅔㅇㅛ'
>>> compose('ㅇㅏㄴㄴㅕㅇㅎㅏㅅㅔㅇㅛ')
'안녕하세요'
```
