class PID_Control:
    def __init__(self, P=0.2, I=0.0, D=0.0, current_time=None):
        self.Kp = P
        self.Ki = I
        self.Kd = D
        self.clear()

    def clear(self):
        self.pTerm = 0.0
        self.iTerm = 0.0
        self.dTerm = 0.0
        self.last_error = 0.0
        self.int_error = 0.0
        self.maxAbsITerm = 1000.0
        self.output = 0.0


    def update(self, error, delta_time=None):
        delta_error = error - self.last_error

        self.pTerm = self.Kp * error
        self.iTerm += error * delta_time

        if (self.iTerm < -self.maxAbsITerm):
            self.iTerm = -self.maxAbsITerm
        elif (self.iTerm > self.maxAbsITerm):
            self.iTerm = self.maxAbsITerm

        self.dTerm = 0.0
        if delta_time > 0:
            self.dTerm = delta_error / delta_time
        self.last_error = error

        print("pTerm:", self.pTerm,  "iTerm:",  self.iTerm, "dTerm : ", self.dTerm)
        # compute output
        self.output = self.pTerm + (self.Ki * self.iTerm) + (self.Kd * self.dTerm)