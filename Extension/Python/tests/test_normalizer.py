from fasthangul.normalizer import clean_text


def test_clean_text():
    assert clean_text("\tHello\n안녕안녕   mm") == "Hello 안녕안녕 mm"
    assert clean_text("가나 \t \r \n다라\u0001") == "가나 다라"
