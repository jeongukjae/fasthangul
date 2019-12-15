from fasthangul import compose, decompose
import random
import string
import time

N_SENTENCES = 10000
MIN_WORD_LENGTH = 500
MAX_WORD_LENGTH = 1000

letters = (
    string.ascii_letters
    + "".join(map(chr, range(ord("가"), ord("힣") + 1)))
    + "          "
)


def get_random_string():
    return "".join(
        random.sample(letters, random.randint(MIN_WORD_LENGTH, MAX_WORD_LENGTH))
    )


def main():
    sentences = set(get_random_string() for _ in range(N_SENTENCES))
    while len(sentences) != N_SENTENCES:
        sentences.add(get_random_string())

    sentences = list(sentences)
    print(
        f"average length: {len(''.join(sentences)) / N_SENTENCES}, "
        f"n sentences: {N_SENTENCES}"
    )
    sentences = "".join(sentences)

    s = time.time()
    decomposed = decompose(sentences)
    print(f"decomposition time: {time.time() - s}")

    s = time.time()
    composed = compose(decomposed)
    print(f"composition time: {time.time() - s}")

    assert composed == sentences


if __name__ == "__main__":
    main()
