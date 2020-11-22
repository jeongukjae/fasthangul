import random
import string

from fasthangul.jamo import compose_jamo, decompose_jamo


def test_compose_jamo():
    assert compose_jamo("ㅇㅏㄴㄴㅕㅇ") == "안녕"
    assert compose_jamo("ㅇㅏㄴㄴㅕㅇ  ") == "안녕  "
    assert compose_jamo("abcdㅇㅏㄴㄴㅕㅇ  ") == "abcd안녕  "
    assert compose_jamo("ㄴㅓ ㅁㅝㅎㅐ?") == "너 뭐해?"
    assert compose_jamo("ㄴㅓ ㅎㅁㅝㅎㅐ?") == "너 ㅎ뭐해?"
    assert compose_jamo("ㅉㅡㅎㅂㅛㅎ") == "쯯뵿"


def test_decompose_jamo():
    assert decompose_jamo("안녕") == "ㅇㅏㄴㄴㅕㅇ"
    assert decompose_jamo("안녕  ") == "ㅇㅏㄴㄴㅕㅇ  "
    assert decompose_jamo("abcd안녕  ") == "abcdㅇㅏㄴㄴㅕㅇ  "
    assert decompose_jamo("너 뭐해?") == "ㄴㅓ ㅁㅝㅎㅐ?"


def test_large_text():
    letters = (
        string.ascii_letters
        + "".join(map(chr, range(ord("가"), ord("힣") + 1)))
        + "          "
    )

    original_sentences = "".join(random.sample(letters, random.randint(7000, 10000)))
    decomposed = decompose_jamo(original_sentences)
    composed = compose_jamo(decomposed)

    assert composed == original_sentences
