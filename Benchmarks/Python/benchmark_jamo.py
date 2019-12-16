import random
import string
import time

from fasthangul.jamo import compose, decompose
from han2jamo import Han2Jamo
from hgtk import text as hgtk

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
    print("n sen | avg len | action | fasthangul | hangul toolkit | han2jamo")
    print("-------------------------------------------------------------------")
    for n_sentences in range(1, 10001, 100):
        sentences = set(get_random_string() for _ in range(n_sentences))
        while len(sentences) != n_sentences:
            sentences.add(get_random_string())

        sentences = list(sentences)
        sentences = "".join(sentences)

        avg_len = len(''.join(sentences)) / n_sentences

        # fasthangul
        fh_decomposition = time.time()
        decomposed = decompose(sentences)
        fh_decomposition = time.time() - fh_decomposition

        fh_composition = time.time()
        composed = compose(decomposed)
        fh_composition = time.time() - fh_composition

        assert sentences == composed, "fasthangul error"

        # hangul toolkit
        ht_decomposition = time.time()
        decomposed = hgtk.decompose(sentences)
        ht_decomposition = time.time() - ht_decomposition

        ht_composition = time.time()
        composed = hgtk.compose(decomposed)
        ht_composition = time.time() - ht_composition

        assert sentences == composed, "hangul toolkit error"

        # han2jamo toolkit
        han2jamo = Han2Jamo()

        hj_decomposition = time.time()
        decomposed = han2jamo.str_to_jamo(sentences)
        hj_decomposition = time.time() - hj_decomposition

        hj_composition = time.time()
        composed = han2jamo.jamo_to_str(decomposed)
        hj_composition = time.time() - hj_composition

        assert sentences == composed, "han2jamo error"

        print(f"{n_sentences:5} | {int(avg_len):7} | comp   | {fh_composition:.8f} |     {ht_composition:.8f} | {hj_composition:.8f}")
        print(f"{n_sentences:5} | {int(avg_len):7} | decomp | {fh_decomposition:.8f} |     {ht_decomposition:.8f} | {hj_decomposition:.8f}")


if __name__ == "__main__":
    main()
