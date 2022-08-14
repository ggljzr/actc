import sys
import ac
import acsys


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
