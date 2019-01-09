import threading

class PushableSource:
    def __init__(self):
        self._value = None
        self._finished = True
        self._lock = threading.Lock()
        self._condition = threading.Condition(self._lock)
        
    def source(self):
        self._lock.acquire()
        self._condition.wait()
        while not self.finished:
            yield self._value
            self.condition.wait()
        self._lock.release()

    def sink(self, value):
        self._lock.acquire()
        self._value = value
        self._condition.notify_all()
        self._lock.release()

    def halt(self):
        self._lock.acquire()
        self.finished = True
        self._condition.notify()
        self._lock.release()

