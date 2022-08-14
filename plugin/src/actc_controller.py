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
        self.command_queue = Queue()

        self.port = port
        self.baudrate = baudrate

        self.__is_running = False
        self.__thread = Thread(target=self.__loop, daemon=True)

    def __loop(self):
        while self.__is_running:
            try:
                with Serial(
                    port=self.port, baudrate=self.baudrate, write_timeout=1.0
                ) as ser:
                    while self.__is_running:

                        try:
                            command = self.command_queue.get(timeout=1.0)
                        except Empty:
                            continue

                        ser.write(command)

            except SerialException as e:
                ac.console("ACTC Serial exception: " + str(e))

            sleep(1.0)

    def add_command(self, command):
        self.command_queue.put(command)

    def start(self):
        ac.console("Starting ACTC controller...")
        if self.__is_running:
            ac.console("ACTC controller is already running...")
            return

        self.__is_running = True
        self.__thread.start()

    def stop(self):
        ac.console("Stopping ACTC controller...")
        self.__is_running = False
