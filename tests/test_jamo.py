import random
import string

from fasthangul.jamo import compose, decompose


def test_compose():
    assert compose("ㅇㅏㄴㄴㅕㅇ") == "안녕"
    assert compose("ㅇㅏㄴㄴㅕㅇ  ") == "안녕  "
    assert compose("abcdㅇㅏㄴㄴㅕㅇ  ") == "abcd안녕  "
    assert compose("ㄴㅓ ㅁㅝㅎㅐ?") == "너 뭐해?"
    assert compose("ㄴㅓ ㅎㅁㅝㅎㅐ?") == "너 ㅎ뭐해?"
    assert compose("ㅉㅡㅎㅂㅛㅎ") == "쯯뵿"


def test_decompose():
    assert decompose("안녕") == "ㅇㅏㄴㄴㅕㅇ"
    assert decompose("안녕  ") == "ㅇㅏㄴㄴㅕㅇ  "
    assert decompose("abcd안녕  ") == "abcdㅇㅏㄴㄴㅕㅇ  "
    assert decompose("너 뭐해?") == "ㄴㅓ ㅁㅝㅎㅐ?"


def test_large_text():
    letters = (
        string.ascii_letters
        + "".join(map(chr, range(ord("가"), ord("힣") + 1)))
        + "          "
    )

    original_sentences = "".join(random.sample(letters, random.randint(7000, 10000)))
    decomposed = decompose(original_sentences)
    composed = compose(decomposed)

    assert composed == original_sentences
