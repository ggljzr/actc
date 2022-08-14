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

    return "ACTC App"
