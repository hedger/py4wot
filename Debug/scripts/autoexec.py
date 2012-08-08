import BigWorld
import Math

def GetVeh(vehID):
  return BigWorld.entities.get(vehID, None)

class GunInfo:
  position = (0, 0, 0)
  model = None
  yaw = pitch = roll = 0
  
  def __init__(self, model):
    if not model:
      return
    self.model = model
    mat = Math.Matrix(model.matrix)
    self.position = model.position
    self.yaw, self.pitch, self.roll = mat.yaw, mat.pitch, mat.roll
    
  def __repr__(self):
    return "<Gun %s @ %f %f %f>" % (self.position, self.yaw, self.pitch, self.roll)
    
  @staticmethod
  def GetForVehicle(vehID):
    veh = GetVeh(vehID)
    if veh is None:
      return None
    gun = veh.appearance.modelsDesc["gun"]["model"]
    return GunInfo(gun) 
    
class TankInfo:
  isValid = False
  isEnemy = True
  gun = None
  info = {}
  
  def __init__(self, vehID):
    self.info = BigWorld.player().arena._ClientArena__vehicles.get(vehID, None)
    if self.info is None:
      return
    
    self.gun = GunInfo.GetForVehicle(vehID)
    self.isValid = self.gun is not None
    
  def __repr__(self):
    return "<TankInfo for %s (valid: %s)>" % (self.info.get("name", "Alex"), self.isValid)    

def list(package):
  dct = package.__dict__
  for k in dct.keys():
    if repr(dct.get(k, "")).find("built") == -1:
      print( "%s -> %s" % ( k, dct.get(k, "none") ))

def listAll(package):
  dct = package.__dict__
  for k in dct.keys():
    print( "%s -> %s" % ( k, dct.get(k, "none") ))
  
print "Hello!"