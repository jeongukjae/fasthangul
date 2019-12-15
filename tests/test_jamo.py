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
