"""
Module for serial interface commands to be used with the controller.
"""


class CommandBase:
    """
    Base class for controller commands. Each command
    contains ID byte and 4 data bytes.

    Raises ``InvalidCommandException`` when constructor parameters
    do not match this format.
    """

    def __init__(self, commandId, data):
        if commandId not in range(0, 256):
            raise ValueError("Invalid command ID {}".format(commandId))

        self.commandId = commandId

        if len(data) != 4:
            raise ValueError("Invalid data length {}".format(len(data)))

        self.data = bytes(data)

    def __bytes__(self):
        return bytes([self.commandId]) + self.data
