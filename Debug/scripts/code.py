import math
import time
from gui.WindowsManager import g_windowsManager

def GetEnemyTanks():
  tankIDs = BigWorld.player().arena._ClientArena__vehicleIndexToId.values()
  tankInfos = map(TankInfo, tankIDs)
  myTank = TankInfo(BigWorld.player().playerVehicleID)
  enemies = filter(lambda t: t.isValid and t.info["isAlive"] and (t.info["team"] != myTank.info["team"]), tankInfos)
  return enemies

def IsAiming(attacker, target):
  aim_angle_bound = 0.1
  dirn = attacker.turret.position - target.turret.position
  aim_angle = abs(attacker.turret.yaw - dirn.yaw)
  #print "Aim %s -> %s: %f rad" % (attacker.info["name"], target.info["name"], aim_angle)
  return  (aim_angle < aim_angle_bound) or (abs(aim_angle - 3.1415926) < aim_angle_bound)
  
def RunNotifier(timeLen):
    for i in range(timeLen * 3):
      time.sleep(0.33)
      myTank = TankInfo(BigWorld.player().playerVehicleID)
      enemies = GetEnemyTanks()
      # print "checking %i enemies..." % len(enemies)
      for enemy in enemies:
        if IsAiming(enemy, myTank):
          msg = "ALERT! %s is aiming at you!" % enemy.info["name"]
          #g_windowsManager.battleWindow.pMsgsPanel.showMessage(msg, {"colorAlias": "purple"})
          g_windowsManager.battleWindow.vMsgsPanel.showMessage(msg, {"colorAlias": "purple"})
          #print "ALERT! %s is aiming at you!" % enemy.info["name"]
