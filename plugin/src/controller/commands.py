"""
Module for serial interface commands to be used with the controller.
"""

import struct


class CommandBase:
    """
    Base class for controller commands. Each command
    contains ID byte and 4 data bytes.

    Raises ``InvalidCommandException`` when constructor parameters
    do not match this format.

    :param commandId: ID of the command, integer between 0 and 256 (one byte).
    :param data: 4 bytes (e. g. ``bytes`` or ``bytearray`` or list integers between 0 and 256)
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


class IntegerCommand(CommandBase):
    """
    Command used for sending integers via serial interface. Uses ``struct.pack``
    to create byte representation of given integer (using big endian).

    :param value: should be integer (or castable to integer via ``int()``).
    """

    def __init__(self, commandId, value):
        data = struct.pack(">I", int(value))
        super().__init__(commandId, data)


class SetTC(IntegerCommand):
    """
    Command for setting TC value. Command ID: 0x01.

    :param value: value of the TC to be set, positive integer.
    """

    def __init__(self, value):
        super().__init__(0x01, value)
