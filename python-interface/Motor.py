from generated.MotorAutoGenerated import MotorAutoGenerated
from generated.ErrorAutoGenerated import error_map
from CurrentControl import CurrentControl
from Rotor import Rotor


class Motor(MotorAutoGenerated):

    def __init__(self, motornr, odrive):
        self.motornr = motornr
        self.odrive = odrive
        self.current_control = CurrentControl(self)
        self.rotor = Rotor(self)

        self._get = odrive._get
        self._set = odrive._set
        self._monitor = odrive._monitor
    
    def get_error(self):
        errornr = MotorAutoGenerated.get_error(self)
        return error_map[errornr]
