#define INTTYPE 0
#define DOUBLETYPE 1
#define STRINGTYPE 2
#define BOOLTYPE 3

typedef struct {
  char name[64]; //maxunshieldedplayerwallbounceangle is longest w/ 35ch
  char stringValue[32];
  int intValue;
  double doubleValue;
  int status;
  int type;
} options_struct;
#define storedOptionCount 261
options_struct storedOptions[storedOptionCount];

double AI_radToDeg(double rad);
double AI_radToXdeg(double rad);
double AI_degToRad(double deg);
double AI_degToXdeg(double deg);
double AI_xdegToRad(double xdeg);
double AI_xdegToDeg(double xdeg);
int getLag(void);
void turnLeft(void);
void turnRight(void);
void turn(double xdeg);
void turnXdeg(double xdeg);
void turnToXdeg(double xdeg);
void turnDeg(double deg);
void turnToDeg(double deg);
void turnRad(double rad);
void turnToRad(double rad);
int setMaxTurn(double xdeg);
int setMaxTurnXdeg(double max);
int setMaxTurnDeg(double max);
int setMaxTurnRad(double max);
double getMaxTurnXdeg(void);
double getMaxTurnDeg(void);
double getMaxTurnRad(void);
void thrust(void);
int setTurnSpeed(double speed);
int setPower(double s);
int setTurnResistance(double s);
void fireShot(void);
void fireMissile(void);
void fireTorpedo(void);
void fireHeat(void);
void dropMine(void);
void detachMine(void);
void detonateMines(void);
void fireLaser(void);
void tankDetach(void);
void cloak(void);
void ecm(void);
void transporter(void);
void tractorBeam(void);
void pressorBeam(void);
void phasing(void);
void shield(void);
void emergencyShield(void);
void hyperjump(void);
void nextTank(void);
void prevTank(void);
void toggleAutopilot(void);
void emergencyThrust(void);
void deflector(void);
void selectItem(void);
void loseItem(void);
void lockNext(void);
void lockPrev(void);
void lockClose(void);
void lockNextClose(void);
int loadLock(int lock);
int saveLock(int lock);
int getLockId(void);
void toggleNuclear(void);
void togglePower(void);
void toggleVelocity(void);
void toggleCluster(void);
void toggleMini(void);
void toggleSpread(void);
void toggleLaser(void);
void toggleImplosion(void);
void toggleUserName(void);
int loadModifier(int i);
int saveModifier(int i);
void clearModifiers(void);
void connector(void);
void dropBall(void);
void refuel(void);
void keyHome(void);
void selfDestruct(void);
void pauseAI(void);
void swapSettings(void);
void quitAI(void);
void talkKey(void);
void toggleShowMessage(void);
void toggleShowItems(void);
void toggleCompass(void);
void repair(void);
void reprogram(void);
int getMaxMsgs(void);
int setMaxMsgs(int var);
void talk(char* talk_str);
int removeTalkMsg(int i);
int removeGameMsg(int i);
int selfX(void);
int selfY(void);
double selfVelX(void);
double selfVelY(void);
double selfSpeed(void);
int lockHeadingXdeg(void);
int lockHeadingDeg(void);
int lockHeadingRad(void);
int selfLockDist(void);
int selfReload(void);
int selfID();
int selfAlive(void);
int selfTeam(void);
int selfLives(void);
double selfTrackingRad();
double selfTrackingDeg();
int selfHeading();
double selfHeadingDeg();
double selfHeadingRad();
int hudTimeLeft(int i);
int getTurnSpeed(void);
int getPower(void);
int getTurnResistance(void);
int selfShield(void);
double selfScore(void);
int selfItem(int i);
int selfFuel(void);
int selfFuelMax(void);
int selfFuelCurrent(void);
double selfMass(void);
int closestRadarId(void);
int radarX(int id);
int radarY(int id);
int radarType(int id);
int radarCount(void);
int radarHeight(void);
int radarWidth(void);
int itemCountScreen(void);
int itemIdCheck(int id);
int itemX(int id);
int itemY(int id);
int itemType(int id);
int itemRandom(int id);
double itemVelX(int id);
double itemVelY(int id);
double itemDist(int id);
double itemSpeed(int id);
double itemTrackingRad(int id);
double itemTrackingDeg(int id);
int shipCountScreen(void);
int shipIdCheck(int id);
int shipId(int idx);
int shipX(int id);
int shipY(int id);
double shipVelX(int id);
double shipVelY(int id);
int shipShield(int id);
double shipDistance(int id);
double shipSpeed(int id);
double shipTrackingRad(int id);
double shipTrackingDeg(int id);
int shipHeadingXdeg(int id);
double shipHeadingDeg(int id);
double shipHeadingRad(int id);
int playerCountServer(void);
int pausedCountServer(void);
int tankCountServer(void);
int playerId(int id);
int playerLives(int id);
int playerTeam(int id);
char* playerName(int id);
int playerScore(int id);
double wallFeelerRad(double dist, double angle);
double wallFeelerDeg(double dist, double angle);
int blockSize(void);
int mapWidthBlocks(void);
int mapHeightBlocks(void);
int mapWidthPixels(void);
int mapHeightPixels(void);
int mapData(int x, int y);
int getOption(char name[64]);
double wallBetween(double x1, double y1, double x2, double y2);
int shotCountScreen(void);
int shotIdCheck(int id);
int shotX(int id);
int shotY(int id);
double shotVelX(int id);
double shotVelY(int id);
double shotDist(int id);
double shotSpeed(int id);
double shotTrackingRad(int id);
double shotTrackingDeg(int id);
int asteroidIdCheck(int id);
int asteroidCountScreen();
int asteroidX(int id);
int asteroidY(int id);
int asteroidType(int id);
int asteroidSize(int id);
int asteroidRotation(int id);
double asteroidVelX(int id);
double asteroidVelY(int id);
double asteroidDist(int id);
double asteroidSpeed(int id);
double asteroidTrackingRad(int id);
double asteroidTrackingDeg(int id);
int phasingTime(void);
int getNextCheckPoint(void);
int checkpointIdCheck(int id);
int checkpointBlockX(int id);
int checkpointBlockY(int id);
int checkpointX(int id);
int checkpointY(int id);
int connectorCountScreen(void);
int connectorIdCheck(int id);
int connectorX0(int id);
int connectorY0(int id);
int connectorX1(int id);
int connectorY1(int id);
int connectorTractor(int id);
int missileCountScreen(void);
int missileIdCheck(int id);
int missileX(int id);
int missileY(int id);
int missileLen(int id);
int missileHeadingXdeg(int id);
int missileHeadingDeg(int id);
int missileHeadingRad(int id);
int laserCountScreen(void);
int laserIdCheck(int id);
int laserX(int id);
int laserY(int id);
int laserLen(int id);
int laserHeadingXdeg(int id);
int laserHeadingDeg(int id);
int laserHeadingRad(int id);
int ballCountScreen(void);
int ballIdCheck(int id);
int ballX(int id);
int ballY(int id);
int ballId(int id);
int mineCountScreen(void);
int mineIdCheck(int id);
int mineX(int id);
int mineY(int id);
int mineFriendly(int id);
int mineId(int id);
int wormholeCountScreen(void);
int wormholeIdCheck(int id);
int wormholeX(int id);
int wormholeY(int id);
int ecmCountScreen(void);
int ecmIdCheck(int id);
int ecmX(int id);
int ecmY(int id);
int timeLeftSec(void);
int fuelstationCount(void);
int fuelstationIdCheck(int id);
int fuelstationBlockX(int id);
int fuelstationBlockY(int id);
int fuelstationX(int id);
int fuelstationY(int id);
int fuelstationFuel(int id);
int fuelstationTeam(int id);
int cannonCountServer(void);
int cannonIdCheck(int id);
int cannonBlockX(int id);
int cannonBlockY(int id);
int cannonX(int id);
int cannonY(int id);
int cannonAlive(int id);
int cannonTeam(int id);
int targetCountServer(void);
int targetIdCheck(int id);
int targetBlockX(int id);
int targetBlockY(int id);
int targetX(int id);
int targetY(int id);
int targetDamage(int id);
int targetAlive(int id);
int targetTeam(int id);
int baseCountServer(void);
int baseIdCheck(int id);
int baseBlockX(int id);
int baseBlockY(int id);
int baseX(int id);
int baseY(int id);
int baseId(int id);
int baseTeam(int id);
void commonInject(void);
void headlessMode();
int commonStart(int argc, char* argv[], void (*injectFnPtr)(void));
