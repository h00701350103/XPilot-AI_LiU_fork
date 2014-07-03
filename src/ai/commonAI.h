#ifndef commonAI_H_INCLUDED
#define commonAI_H_INCLUDED
int getLag(void);
void turnLeft(void);
void turnRight(void);
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

#endif
