import math
import time
from gui.WindowsManager import g_windowsManager

def GetEnemyTanks():
  tankIDs = BigWorld.player().arena._ClientArena__vehicleIndexToId.values()
  tankInfos = map(TankInfo, tankIDs)
  myTank = TankInfo(BigWorld.player().playerVehicleID)
  enemies = filter(lambda t: t.isValid and t.info["isAlive"] and (t.info["team"] != myTank.info["team"]), tankInfos)
  return enemies


class AimNotifier:
  # Consts
  AIM_NONE = 0    
  AIM_NONDIRECT = 1
  AIM_DIRECT = 2
  
  AIM_ANGLE_BOUND = 0.1 # in radians
  NOTIFY_COOLDOWN = 4  # in seconds
  
  # Fields
  notifications = {}
  myTank = None
  logger = None

  def __init__(self):
    self.myTank = TankInfo(BigWorld.player().playerVehicleID)
    self.logger = g_windowsManager.battleWindow.vMsgsPanel 
    self.logger.showMessage("AimNotifier loaded.")

  @staticmethod
  def IsAiming(attacker, target):
    dirn = attacker.gun.position - target.gun.position
    aim_angle = abs(attacker.gun.yaw - dirn.yaw)
    #print "Aim %s -> %s: %f rad" % (attacker.info["name"], target.info["name"], aim_angle)
    aimDir = (aim_angle < AimNotifier.AIM_ANGLE_BOUND) or (abs(aim_angle - math.pi) < AimNotifier.AIM_ANGLE_BOUND)
    if not aimDir:
      return AimNotifier.AIM_NONE
    else:
      testRes = BigWorld.wg_collideSegment(BigWorld.player().spaceID, attacker.gun.position, target.gun.position, 3)
      # print testRes
      if testRes:
        return AimNotifier.AIM_NONDIRECT
      return AimNotifier.AIM_DIRECT 
      
  def ShowNotice(self, enemy, aimCheck):
    if aimCheck == self.AIM_NONE:
      return

    enemyName = enemy.info["name"]
    #old_check = self.notifications.get(enemyName, None)
    #if old_check and (old_check[0] == aimCheck) and (old_check[1] + self.NOTIFY_COOLDOWN > time.time()):
    #  print "skip"
    #  return
      
    # saving && showing updated info 
    print "refresh"
    self.notifications[enemyName] = (aimCheck, time.time()) 
    color = "purple"
    if aimCheck == self.AIM_DIRECT:
      color = "red" 
    msg = "%s [%s] is aiming at you!" % (enemy.info["vehicleType"].name.split(':')[1], enemyName)
    self.logger.showMessage(msg, {"colorAlias": color}) # pMsgsPanel 

  def Run(self, timeLen):
    for i in range(timeLen * 3):
      time.sleep(0.33)
      enemies = GetEnemyTanks()
      # print "checking %i enemies..." % len(enemies)
      for enemy in enemies:
        self.ShowNotice(enemy, AimNotifier.IsAiming(enemy, self.myTank))
