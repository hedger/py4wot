import BigWorld
import Math

def GetVeh(vehID):
  return BigWorld.entities.get(vehID, None)

class TurretInfo:
  position = ()
  model = None
  yaw = pitch = roll = 0
  
  def __init__(self, model):
    self.model = model
    mat = Math.Matrix(model.matrix)
    self.position = model.position
    self.yaw, self.pitch, self.roll = mat.yaw, mat.pitch, mat.roll
    
  def __repr__(self):
    return "<Turret %s @ %f %f %f>" % (self.position, self.yaw, self.pitch, self.roll)
    
  @staticmethod
  def GetForVehicle(vehID):
    veh = GetVeh(vehID)
    if veh is None:
      return None
    turret = veh.appearance.modelsDesc["turret"]["model"]
    return TurretInfo(turret) 
    
class TankInfo:
  isValid = False
  isEnemy = True
  turret = None
  info = {}
  
  def __init__(self, vehID):
    self.info = BigWorld.player().arena._ClientArena__vehicles.get(vehID, None)
    if self.info is None:
      return
    
    self.turret = TurretInfo.GetForVehicle(vehID)
    self.isValid = self.turret is not None
    
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