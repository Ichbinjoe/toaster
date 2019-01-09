import time

def sign(number):
    return number > 0

def classic_zn(ku, tu):
    return 0.6 * ku, 0.5 * tu, 0.125 * tu

def pessen_integral_rule(ku, tu):
    return 0.7 * ku, 0.4 * tu, 0.15 * tu

def some_overshoot(ku, tu):
    return 0.33 * ku, 0.5 * tu, 0.33 * tu

def no_overshoot(ku, tu):
    return 0.2 * ku, 0.5 * tu, 0.33 * tu

class PIDAutotune:
    def __init__(self, pid):
        self.pid = pid

    def tune(self, error_source, model, kp_min=0.5, kp_max=10, kp_step=0.5, n_samples=1000):
        self.pid.I = 0
        self.pid.D = 0

        for kp in range(kp_min, kp_max + kp_step, kp_step):
            self.pid.P = kp
            results = self.tune_with_kp(kp, error_source, model, n_samples)
            if results is not None:
                return results

        return None

    def tune_with_kp(self, kp, error_source, model, n_samples):
        osc_times = []
        initial_error = next(error_source)

        i = 0
        for new_error in error_source:
            if sign(initial_error) is not sign(new_error):
                # We got an oscillation
                self.osc_times.append((time.time(), i))

                initial_error = new_error

                if len(self.osc_times) > 2:
                    first = osc_times[0]
                    full = osc_times[2]

                    if full[1] - first[1] > 3:
                        tu = full[0] - first[0]
                        return model(kp, tu)

            i += 1
            if i > n_samples:
                return None
        return None

