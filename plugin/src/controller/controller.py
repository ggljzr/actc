import ac

from functools import partial
from threading import Thread
from queue import Queue, Empty
from time import sleep
from configparser import ConfigParser

from serial import Serial, SerialException

from .commands import SetTC, SetABS, SetLapCount, SetBestLap, SetLastLap


def recalculation(val, offset):
    """
    Helper function for transforming TC and ABS value from ``info.physics``
    into more readable form.

    Also note that in Assetto Corsa low TC (or ABS) values means stronger TC
    (so e. g. for AMG GT3 1 is strongest, 12 is weakest), except for 0,
    which means that TC is off :-D.
    """
    val = round(val * 100, 0)
    if val == 0.0:
        return val

    # Currently I have no idea why it is offset like this,
    # perhaps it is different for some specific cars?
    return val + offset


tcRecalculation = partial(recalculation, offset=-7)
absRecalculation = partial(recalculation, offset=-5)


class Controller:
    """
    Class implementing command queue and threaded serial communication. It contains methods
    to set given parameters (e. g. TC) by sending commands to the ACTC device via serial interface.
    """

    def __init__(self, port, baudrate):
        self.__commandQueue = Queue()

        self.port = port
        self.baudrate = baudrate

        self.__isRunning = False
        self.__thread = Thread(target=self.__loop, daemon=True)

    def __loop(self):
        while self.__isRunning:
            try:
                with Serial(
                    port=self.port, baudrate=self.baudrate, write_timeout=1.0
                ) as ser:
                    while self.__isRunning:

                        try:
                            command = self.__commandQueue.get(timeout=1.0)
                        except Empty:
                            continue

                        ser.write(bytes(command))

            except SerialException as e:
                ac.console("ACTC Serial exception: " + str(e))

            sleep(1.0)

    def __addCommand(self, command):
        self.__commandQueue.put(command)

    def setTC(self, value):
        """
        Sends command to set TC to given value. Value should
        come directly from ``info.physics.tc``
        and is recalculated for the controller.
        """

        value = tcRecalculation(value)
        self.__addCommand(SetTC(value))

    def setABS(self, value):
        """
        Sends command to set ABS to given value. Value should
        come directly from ``info.physics.abs``
        and is recalculated for the controller.
        """

        value = absRecalculation(value)
        self.__addCommand(SetABS(value))

    def setLastLap(self, value):
        """
        Sends command for setting last lap time. Value in milliseconds.
        """
        self.__addCommand(SetLastLap(value))

    def setBestLap(self, value):
        """
        Sends command for setting best lap time. Value in milliseconds.
        """
        self.__addCommand(SetBestLap(value))

    def setLapCount(self, value):
        """
        Sends command for setting lap count.
        """
        self.__addCommand(SetLapCount(value))

    def start(self):
        """
        Starts command sending loop in separate thread.
        """

        ac.console("Starting ACTC controller...")
        if self.__isRunning:
            ac.console("ACTC controller is already running...")
            return

        self.__isRunning = True
        self.__thread.start()

    def stop(self):
        """
        Stops command sending loop.
        """

        ac.console("Stopping ACTC controller...")
        self.__isRunning = False

    @staticmethod
    def fromConfig(path="config.ini"):
        config = ConfigParser()
        config.read(path)

        port = config["serial"]["port"]
        baudrate = config["serial"].get("baudrate", 9600)

        return Controller(port, baudrate)
