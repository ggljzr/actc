import ac

from threading import Thread
from queue import Queue, Empty
from serial import Serial, SerialException
from time import sleep


class Controller:
    """
    Class implementing command queue and threaded serial communication.
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

                        ser.write(command)

            except SerialException as e:
                ac.console("ACTC Serial exception: " + str(e))

            sleep(1.0)

    def addCommand(self, command):
        self.__commandQueue.put(command)

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
