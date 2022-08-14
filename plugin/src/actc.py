import platform, sys, os
import ac
import acsys

# obtain current architecture (64/32 bit)
# we dont actually care about linkage
arch, linkage = platform.architecture()
if arch == "64bit":
    sysdir = os.path.dirname(__file__) + "/third_party/stdlib64"
else:
    sysdir = os.path.dirname(__file__) + "/third_party/stdlib"
sys.path.insert(0, sysdir)

# this has to be here to correctly load _ctypes module
# shamelessly stolen from Sidekick (https://www.racedepartment.com/downloads/sidekick.11007/)
# source code
os.environ["PATH"] = os.environ["PATH"] + ";."

from third_party.sim_info import info

updateTimer = 0.0


def acMain(ac_version):
    """
    This is the main entry point of the plugin.
    """

    appWindow = ac.newApp("ACTC App")
    ac.setSize(appWindow, 200, 200)

    ac.console("AC Version: " + str(ac_version))
    ac.console("Python version: " + sys.version)

    # 0 -> player's car
    carName = ac.getCarName(0)
    ac.console("Car name: " + carName)

    return "ACTC App"


def acUpdate(deltaT):
    global updateTimer

    updateTimer += deltaT

    # Update RPM in console each 1.0 second
    if updateTimer > 1.0:
        updateTimer = 0.0
        rpm = ac.getCarState(0, acsys.CS.RPM)
        fuel = info.physics.fuel
        ac.console("RPM: {}; Fuel: {}".format(rpm, fuel))
