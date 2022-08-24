import platform, sys, os

import ac
import acsys

# obtain current architecture (64/32 bit)
# we dont actually care about linkage
arch, linkage = platform.architecture()
rootDir = os.path.dirname(__file__)

if arch == "64bit":
    sysdir = rootDir + "/libs/stdlib64"
else:
    sysdir = rootDir + "/libs/stdlib"
sys.path.insert(0, sysdir)

# required to properly import pyserial library
sys.path.insert(0, rootDir + "/pyserial")

# this has to be here to correctly load _ctypes module
# shamelessly stolen from Sidekick (https://www.racedepartment.com/downloads/sidekick.11007/)
# source code
os.environ["PATH"] = os.environ["PATH"] + ";."

from libs.sim_info import info
from controller import Controller

controller = Controller.fromConfig(rootDir + "/config.ini")

# global variables for storing settings in between acUpdate calls
tc = 0
abs = 0

lapCount = 0
bestLapTime = 0
lastLapTime = 0


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

    controller.start()

    tc = info.physics.tc
    controller.setTC(tc)

    abs = info.physics.abs
    controller.setABS(abs)

    controller.setLapCount(0)
    controller.setBestLap(0)
    controller.setLastLap(0)

    return "ACTC App"


def acUpdate(deltaT):
    global tc, abs, lapCount, bestLapTime, lastLapTime

    newTc = info.physics.tc
    newAbs = info.physics.abs

    newLapCount = ac.getCarState(0, acsys.CS.LapCount)
    newBestLapTime = ac.getCarState(0, acsys.CS.BestLap)
    newLastLapTime = ac.getCarState(0, acsys.CS.LastLap)

    if newTc != tc:
        tc = newTc
        controller.setTC(tc)

    if newAbs != abs:
        abs = newAbs
        controller.setABS(abs)

    if newLapCount != lapCount:
        lapCount = newLapCount
        controller.setLapCount(lapCount)

    if newBestLapTime != bestLapTime:
        bestLapTime = newBestLapTime
        controller.setBestLap(bestLapTime)

    if newLastLapTime != lastLapTime:
        lastLapTime = newLastLapTime
        controller.setLastLap(lastLapTime)


def acShutdown():
    controller.stop()
