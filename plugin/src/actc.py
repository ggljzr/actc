import platform, sys, os
import ac
import acsys

# obtain current architecture (64/32 bit)
# we dont actually care about linkage
arch, linkage = platform.architecture()
if arch == "64bit":
    sysdir = os.path.dirname(__file__) + "/libs/stdlib64"
else:
    sysdir = os.path.dirname(__file__) + "/libs/stdlib"
sys.path.insert(0, sysdir)

# required to properly import pyserial library
sys.path.insert(0, os.path.dirname(__file__) + "/pyserial")

# this has to be here to correctly load _ctypes module
# shamelessly stolen from Sidekick (https://www.racedepartment.com/downloads/sidekick.11007/)
# source code
os.environ["PATH"] = os.environ["PATH"] + ";."

from libs.sim_info import info
from actc_controller import Controller

serialConfig = {"port": "COM28", "baudrate": 9600}
controller = Controller(**serialConfig)

# global variable for storing TC settings in between acUpdate calls
tc = 0.0


def tcRecalculation(val):
    """
    Helper function for transforming TC value from ``info.physics``
    into more readable form.

    Also note that in Assetto Corsa low TC values means stronger TC
    (so e. g. for AMG GT3 1 is strongest, 12 is weakest), except for 0,
    which means that TC is off :-D.
    """
    val = round(val * 100, 0)
    if val == 0.0:
        return val

    # Currently I have no idea why it is offset like this,
    # perhaps it is different for some specific cars?
    return val - 7


def testButtonHandler(a, b):
    """
    Test button OnClick handler. Not sure about the parameters, but
    the caller apparently passes 2 arguments to this function.
    """

    ac.console("Test button pressed")


def acMain(ac_version):
    """
    This is the main entry point of the plugin.
    """
    global tc

    appWindow = ac.newApp("ACTC App")
    ac.setSize(appWindow, 200, 200)

    ac.console("AC Version: " + str(ac_version))
    ac.console("Python version: " + sys.version)

    # 0 -> player's car
    carName = ac.getCarName(0)
    ac.console("Car name: " + carName)

    tc = tcRecalculation(info.physics.tc)
    ac.console("TC: " + str(tc))

    button = ac.addButton(appWindow, "TEST")
    ac.setPosition(button, 3, 30)
    ac.addOnClickedListener(button, testButtonHandler)

    controller.start()

    return "ACTC App"


def acUpdate(deltaT):
    global tc

    newTc = tcRecalculation(info.physics.tc)

    if newTc != tc:
        tc = newTc
        msg = "TC: " + str(tc)
        ac.console(msg)
        controller.add_command(msg.encode("ascii") + b"\n")


def acShutdown():
    controller.stop()
