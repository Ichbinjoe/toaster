import threading
import time

import pushpull

from ..toaster import T

def avg(t1, t2):
    return (t1 + t2) / 2

class ToastController(threading.Thread):
    def __init__(self, sp, pid, sleep=0.5, t=T, reading_strategy=avg):
        self._pid = pid
        self._t = t
        self._read_stgy = reading_strategy
        self._do = True
        self._sleep = sleep
        self._pushpull = pushpull.PushableSource

    def run(self):
        while self._do:
            with self._t:
                t1 = self._t.temp(0)
                t2 = self._t.temp(1)

                temp = self._read_stgy(t1, t2)

                # calculate the PID feedback
                fb = self._pid.feedback(temp)

                # publish to people who ask via generator magic
                self._pushpull.sink((temp, self._pid.reference, fb))

                # enable if the feedback says we need to
                self._t.enable(fb < 0)
            if self._sleep is not None:
                time.sleep(self._sleep)

        self._pushpull.halt()
                
    def states(self):
        return self._pushpull.source()

    def set(self, target):
        self._pid.reference = target

    def stop(self, join=True, timeout=None):
        self._do = False
        if join:
            self.join(timeout)


