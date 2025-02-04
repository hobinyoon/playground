import logging
import time
import unittest


def GetLogger():
    logger = logging.getLogger("kv-8973")
    logger.setLevel(logging.DEBUG)
    sh = logging.StreamHandler()
    sh.setLevel(logging.DEBUG)
    sh.setFormatter(
        logging.Formatter(
            "%(asctime)s %(levelname)s %(filename)s:%(lineno)d %(funcName)s() %(message)s"
        )
    )
    logger.addHandler(sh)
    logger.propagate = False
    return logger


log = GetLogger()


class Timer:
    def __enter__(self):
        self.start_time = time.perf_counter()
        return self

    def __exit__(self, *args):
        self.end_time = time.perf_counter()
        self.elapsed_time = self.end_time - self.start_time
        log.debug(f"Code block took {self.elapsed_time:.3f} seconds", stacklevel=2)


class MyTest(unittest.TestCase):
    def test_1(self):
        log.debug("I'm here")
        with Timer():
            time.sleep(1)
        log.debug("I'm here")

        with self.subTest("subtest1"):
            # Since unittest doesn't have a "checkTrue" option, we use subTest,
            # because a failure of a subtest doesn't stop the other subTest
            # from being run.
            self.assertTrue(False)
            log.debug("I'm not executed because the above fails")

        with self.subTest("subtest2"):
            self.assertTrue(True)
            log.debug("I'm here")


if __name__ == "__main__":
    unittest.main()
