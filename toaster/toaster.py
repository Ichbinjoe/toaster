import ctypes
from threading import Lock

_THERMO_TYPES = {
    'B': 0x0,
    'E': 0x1,
    'J': 0x2,
    'K': 0x3,
    'N': 0x4,
    'R': 0x5,
    'S': 0x6,
    'T': 0x7,
    '8': 0x8,
    '32': 0xB
}

def get_thermo_type(t):
    if t is None:
        raise Exception("Not a thermocouple type")

    if type(t) == ctypes.c_uint:
        return t

    if type(t) == int:
        return ctypes.c_uint(t)

    if t not in _THERMO_TYPES:
        raise Exception("Not a thermocouple type")

    return ctypes.c_uint(_THERMO_TYPES[t])

class ToasterStatus(ctypes.Structure):
    _fields_ = [(x, ctypes.c_int, 1) for x in ["cjrange", 
        "tcrange", "cjhigh", "cjlow", "tchigh", "tclow", "ovuv", "open"]]

class Toaster:
    def __init__(self, lock):
        self._lock = lock
        self.lt = ctypes.CDLL("libtoaster.so")
        self.lt.toaster_thermo_readtemp.restype = ToasterStatus
    
    def __enter__(self):
        self._lock.acquire()

    def __exit__(self, type, value, traceback):
        self._lock.release()
        return False

    def initialize(self, thermo1_type='K', thermo2_type='K'):
        """
        Initializes libtoaster, which in turn initializes the toaster-reflow
        board.
        :param thermo1_type: Type of the thermocouple attached to 1
        :param thermo2_type: Type of the thermocouple attached to 2
        """
        self.lt.toaster_init(get_thermo_type(thermo1_type), get_thermo_type(thermo2_type))


    def deinit(self):
        self.lt.toaster_close()

    def enable(self, yes=True):
        v = ctypes.c_uint(1) if yes else ctypes.c_uint(0)

        self.lt.toaster_setActivate(v)
    
    def stop_buzzer(self):
        self.lt.toaster_buzzer_off()

    def buzzer(self, hz):
        self.lt.toaster_buzzer_hz(ctypes.c_int(hz))

    def servo(self, pos):
        self.lt.toaster_servo_set(ctypes.c_float(pos))
    
    def t1_has_fault(self):
        return self.lt.toaster_t1_hasfault()

    def t2_has_fault(self)->bool:
        return self.lt.toaster_t2_hasfault()

    def t1_has_data(self):
        return self.lt.toaster_t1_datardy()

    def t2_has_data(self):
        return self.lt.toaster_t2_datardy()

    def temp(self, thermo):
        val = ctypes.c_float()
        
        sr = self.lt.toaster_read_temp(ctypes.c_int(thermo), ctypes.byref(val))

        return val, sr

T = Toaster(Lock())
