from fasthangul.normalizer import clean_text


def test_clean_text():
    assert clean_text("\tHello\n안녕안녕   mm") == "Hello 안녕안녕 mm"
