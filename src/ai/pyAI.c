#include <math.h> // for isnan
#include <Python.h>
#include "commonAI.h"
#include "const.h" // to be able to include ../common/setup.h
#include "../common/setup.h" // Access to Setup for checking if connected
#define commonAI_H_INCLUDED

#define only_available_if_connected \
if (!Setup) { \
  PyErr_SetString(PyExc_RuntimeError, "unavailable prior to connecting to server"); \
  return NULL; \
}

static PyObject* py_loop = NULL;
static PyObject* py_getLag(PyObject* pySelf, PyObject* args)
{
  only_available_if_connected
  return Py_BuildValue("i", getLag());
}
static PyObject* py_turnLeft(PyObject* pySelf, PyObject* args) {  //turns left as if the 'a' key was pressed -JNE
  only_available_if_connected
  turnLeft();
  Py_RETURN_NONE;
}
static PyObject* py_turnRight(PyObject* pySelf, PyObject*args) { //turns right as if the 's' key was pressed -JNE
  only_available_if_connected
  turnRight();
  Py_RETURN_NONE;
}
void turnResistanceCheck(void) {
  only_available_if_connected
  if (getTurnResistance() != 0) {
    printf("WARNING: turnResistance != 0\n");
  }
}
static PyObject* py_turnXdeg(PyObject* pySelf, PyObject* args)
{
  only_available_if_connected
  double xdeg;
  if (!PyArg_ParseTuple(args, "d", &xdeg))
  {
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  turnResistanceCheck();
  turnXdeg(xdeg);
  Py_RETURN_NONE;
}
static PyObject* py_turnToXdeg(PyObject* pySelf, PyObject* args)
{
  only_available_if_connected
  double xdeg;
  if (!PyArg_ParseTuple(args, "d", &xdeg))
  {
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  turnResistanceCheck();
  turnToXdeg(xdeg);
  Py_RETURN_NONE;
}
static PyObject* py_turnDeg(PyObject* pySelf, PyObject* args) { //turns based on the speed, 'deg', that is passed in -JNE
  only_available_if_connected
  double deg;
  if (!PyArg_ParseTuple(args, "d", &deg)){
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  turnResistanceCheck();
  turnDeg(deg);
  Py_RETURN_NONE;
}
static PyObject* py_turnToDeg(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  //sets the ship's heading to a fixed degree -JNE
  //rewritten to be instant
  double deg;
  if (!PyArg_ParseTuple(args, "d", &deg)){
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  turnResistanceCheck();
  turnToDeg(deg);
  Py_RETURN_NONE;
}
static PyObject* py_turnRad(PyObject* pySelf, PyObject* args)
{
  only_available_if_connected
  double rad;
  if (!PyArg_ParseTuple(args, "d", &rad)){
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  turnResistanceCheck();
  turnRad(rad);
  Py_RETURN_NONE;
}
static PyObject* py_turnToRad(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  //sets the ship's heading to a fixed degree -JNE
  //rewritten to be instant
  double rad;
  if (!PyArg_ParseTuple(args, "d", &rad)){
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  turnResistanceCheck();
  turnToRad(rad);
  Py_RETURN_NONE;
}
static PyObject* py_setMaxTurnXdeg(PyObject* pySelf, PyObject* args)
{
  double max;
  if (!PyArg_ParseTuple(args, "d", &max))
  {
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
  }
  if (setMaxTurnXdeg(max))
  {
    PyErr_SetString(PyExc_TypeError, "invalid value");
    return NULL;
  }
  Py_RETURN_NONE;
}
static PyObject* py_setMaxTurnDeg(PyObject* pySelf, PyObject* args)
{
  double max;
  if (!PyArg_ParseTuple(args, "d", &max))
  {
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
  }
  if (setMaxTurnDeg(max))
  {
    PyErr_SetString(PyExc_TypeError, "invalid value");
    return NULL;
  }
  Py_RETURN_NONE;
}
static PyObject* py_setMaxTurnRad(PyObject* pySelf, PyObject* args)
{
  double max;
  if (!PyArg_ParseTuple(args, "d", &max))
  {
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
  }
  if (setMaxTurnRad(max))
  {
    PyErr_SetString(PyExc_TypeError, "invalid value");
    return NULL;
  }
  Py_RETURN_NONE;
}
static PyObject* py_getMaxTurnXdeg(PyObject* pySelf, PyObject* args)
{
  return Py_BuildValue("d", getMaxTurnXdeg());
}
static PyObject* py_getMaxTurnDeg(PyObject* pySelf, PyObject* args)
{
  return Py_BuildValue("d", getMaxTurnDeg());
}
static PyObject* py_getMaxTurnRad(PyObject* pySelf, PyObject* args)
{
  return Py_BuildValue("d", getMaxTurnRad());
}
static PyObject* py_thrust(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  thrust();
  Py_RETURN_NONE;
}
static PyObject* py_setTurnSpeed(PyObject* pySelf, PyObject* args) {
  double s;
  if (!PyArg_ParseTuple(args, "d", &s)){
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (setTurnSpeed(s)) {
    PyErr_SetString(PyExc_ValueError, "valid values are 4.0-64.0");
    return NULL;
  }
  Py_RETURN_NONE;
}
static PyObject* py_setPower(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  double s;
  if (!PyArg_ParseTuple(args, "d", &s)){
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (setPower(s)) {
    PyErr_SetString(PyExc_ValueError, "valid values are 4-64");
    return NULL;
  }
  Py_RETURN_NONE;
}
static PyObject* py_setTurnResistance(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  double s;
  if (!PyArg_ParseTuple(args, "d", &s)){
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (setTurnResistance(s)) {
    PyErr_SetString(PyExc_ValueError, "valid values are 0-1");
    return NULL;
  }
  Py_RETURN_NONE;
}
static PyObject* py_fireShot(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  fireShot();
  Py_RETURN_NONE;
}
static PyObject* py_fireMissile(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  fireMissile();
  Py_RETURN_NONE;
}
static PyObject* py_fireTorpedo(PyObject* pySelf, PyObject* args) { 
  only_available_if_connected
  fireTorpedo();
  Py_RETURN_NONE;
}
static PyObject* py_fireHeat(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  fireHeat();
  Py_RETURN_NONE;
}
static PyObject* py_dropMine(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  dropMine();
  Py_RETURN_NONE;
}
static PyObject* py_detachMine(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  detachMine();
  Py_RETURN_NONE;
}
static PyObject* py_detonateMines(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  detonateMines();
  Py_RETURN_NONE;
}
static PyObject* py_fireLaser(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  fireLaser();
  Py_RETURN_NONE;
}
static PyObject* py_tankDetach(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  tankDetach();
  Py_RETURN_NONE;
}
static PyObject* py_cloak(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  cloak();
  Py_RETURN_NONE;
}
static PyObject* py_ecm(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  ecm();
  Py_RETURN_NONE;
}
static PyObject* py_transporter(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  transporter();
  Py_RETURN_NONE;
}
static PyObject* py_tractorBeam(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  tractorBeam();
  Py_RETURN_NONE;
}
static PyObject* py_pressorBeam(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  pressorBeam();
  Py_RETURN_NONE;
}
static PyObject* py_phasing(PyObject* pySelf, PyObject* args){
  only_available_if_connected
  phasing();
  Py_RETURN_NONE;
}
static PyObject* py_shield(PyObject* pySelf, PyObject* args)
{
  only_available_if_connected
  shield();
  Py_RETURN_NONE;
}
static PyObject* py_emergencyShield(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  emergencyShield();
  Py_RETURN_NONE;
}
static PyObject* py_hyperjump(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  hyperjump();
  Py_RETURN_NONE;
}
static PyObject* py_nextTank(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  nextTank();
  Py_RETURN_NONE;
}
static PyObject* py_prevTank(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  prevTank();
  Py_RETURN_NONE;
}
static PyObject* py_toggleAutopilot(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  toggleAutopilot();
  Py_RETURN_NONE;
}
static PyObject* py_emergencyThrust(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  emergencyThrust();
  Py_RETURN_NONE;
}
static PyObject* py_deflector(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  deflector();
  Py_RETURN_NONE;
}
static PyObject* py_selectItem(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  selectItem();
  Py_RETURN_NONE;
}
static PyObject* py_loseItem(PyObject* pySelf, PyObject* args) { 
  only_available_if_connected
  loseItem();
  Py_RETURN_NONE;
}
static PyObject* py_lockNext(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  lockNext();
  Py_RETURN_NONE;
}
static PyObject* py_lockPrev(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  lockPrev();
  Py_RETURN_NONE;
}
static PyObject* py_lockClose(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  lockClose();
  Py_RETURN_NONE;
}
static PyObject* py_lockNextClose(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  lockNextClose();
  Py_RETURN_NONE;
}
static PyObject* py_loadLock(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int lock;
  if (!PyArg_ParseTuple(args, "i", &lock)) {
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (loadLock(lock)) {
      PyErr_SetString(PyExc_IndexError, "Index must be 1-4");
      return NULL;
  }
  Py_RETURN_NONE;
}
static PyObject* py_saveLock(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int lock;
  if (!PyArg_ParseTuple(args, "i", &lock)) {
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (saveLock(lock)) {
      PyErr_SetString(PyExc_IndexError, "Index must be 1-4");
      return NULL;
  }
  Py_RETURN_NONE;
}
static PyObject* py_getLockId(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  return Py_BuildValue("i", getLockId());
}
static PyObject* py_toggleNuclear(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  toggleNuclear();
  Py_RETURN_NONE;
}
static PyObject* py_togglePower(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  togglePower();
  Py_RETURN_NONE;
}
static PyObject* py_toggleVelocity(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  toggleVelocity();
  Py_RETURN_NONE;
}
static PyObject* py_toggleCluster(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  toggleCluster();
  Py_RETURN_NONE;
}
static PyObject* py_toggleMini(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  toggleMini();
  Py_RETURN_NONE;
}
static PyObject* py_toggleSpread(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  toggleSpread();
  Py_RETURN_NONE;
}
static PyObject* py_toggleLaser(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  toggleLaser();
  Py_RETURN_NONE;
}
static PyObject* py_toggleImplosion(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  toggleImplosion();
  Py_RETURN_NONE;
}
static PyObject* py_toggleUserName(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  toggleUserName();
  Py_RETURN_NONE;
}
static PyObject* py_clearModifiers(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  clearModifiers();
  Py_RETURN_NONE;
}
static PyObject* py_connector(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  connector();
  Py_RETURN_NONE;
}
static PyObject* py_dropBall(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  dropBall();
  Py_RETURN_NONE;
}
static PyObject* py_refuel(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  refuel();
  Py_RETURN_NONE;
}
static PyObject* py_keyHome(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  keyHome();
  Py_RETURN_NONE;
}
static PyObject* py_selfDestruct(PyObject* pySelf, PyObject* args) { //TODO: See comment further down
  only_available_if_connected
  selfDestruct();
  Py_RETURN_NONE;
}
static PyObject* py_pauseAI(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  pauseAI();
  Py_RETURN_NONE;
}
static PyObject* py_swapSettings(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  swapSettings();
  Py_RETURN_NONE;
}
static PyObject* py_talkKey(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  talkKey();
  Py_RETURN_NONE;    
}
static PyObject* py_toggleShowMessage(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  toggleShowMessage();
  Py_RETURN_NONE;    
}
static PyObject* py_toggleShowItems(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  toggleShowItems();
  Py_RETURN_NONE;    
}
static PyObject* py_toggleCompass(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  toggleCompass();
  Py_RETURN_NONE;
}
static PyObject* py_repair(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  repair();
  Py_RETURN_NONE;
}
static PyObject* py_reprogram(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  reprogram();
  Py_RETURN_NONE;
}
static PyObject* py_loadModifiers(PyObject* pySelf, PyObject* args) {//TODO: Use Send_modifier_bank
  only_available_if_connected
  int index;
  if (!PyArg_ParseTuple(args, "i", &index)) {
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (loadModifier(index)) {
    PyErr_SetString(PyExc_IndexError, "Index must be 1-4");
    return NULL;
  }
  Py_RETURN_NONE;
}
static PyObject* py_saveModifiers(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int index;
  if (!PyArg_ParseTuple(args, "i", &index)) {
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (saveModifier(index)) {
    PyErr_SetString(PyExc_IndexError, "Index must be 1-4");
    return NULL;
  }
  Py_RETURN_NONE;
}
static PyObject* py_quitAI(PyObject* pySelf, PyObject* args) {
  quitAI();
  Py_RETURN_NONE;
}
static PyObject* py_getMaxMsgs(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  return Py_BuildValue("i", getMaxMsgs());
}
static PyObject* py_setMaxMsgs(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int var;
  if (!PyArg_ParseTuple(args, "i", &var)) {
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (setMaxMsgs(var)) {
    PyErr_SetString(PyExc_ValueError, "valid values are 1-15");
  }
  Py_RETURN_NONE;
}
static PyObject* py_talk(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  char* talk_str;
  if (!PyArg_ParseTuple(args, "s", &talk_str)) {
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  talk(talk_str);
  Py_RETURN_NONE;
}
static PyObject* py_scanTalkMsg(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int id;
  if (!PyArg_ParseTuple(args, "i", &id)){
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  return Py_BuildValue("s",scanTalkMsg(id));
}
static PyObject* py_removeTalkMsg(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int id;
  if (!PyArg_ParseTuple(args, "i", &id)){
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (removeTalkMsg(id)) {
    Py_RETURN_NONE;
  }
  PyErr_SetString(PyExc_IndexError, "No message with that index.");
  return NULL;
}
static PyObject* py_scanGameMsg(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int id;
  if (!PyArg_ParseTuple(args, "i", &id)){
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  return Py_BuildValue("s",scanTalkMsg(id));
}
static PyObject* py_removeGameMsg(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int id;
  if (!PyArg_ParseTuple(args, "i", &id)){
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (removeTalkMsg(id)) {
    Py_RETURN_NONE;
  }
  PyErr_SetString(PyExc_IndexError, "No message with that index.");
  return NULL;
}
static PyObject* py_selfX(PyObject* pySelf, PyObject* args) {       //returns the player's x position
  only_available_if_connected
  return Py_BuildValue("i",selfX());
}
static PyObject* py_selfY(PyObject* pySelf, PyObject* args) {       //returns the player's y position
  only_available_if_connected
  return Py_BuildValue("i",selfY());
}
static PyObject* py_selfVelX(PyObject* pySelf, PyObject* args) {    //returns the player's x velocity
  only_available_if_connected
  return Py_BuildValue("d",selfVelX());
}
static PyObject* py_selfVelY(PyObject* pySelf, PyObject* args) {    //returns the player's y velocity
  only_available_if_connected
  return Py_BuildValue("d",selfVelY());
}
static PyObject* py_selfSpeed(PyObject* pySelf, PyObject* args) {   //returns speed of the player's ship
  only_available_if_connected
  return Py_BuildValue("d",selfSpeed());
}
static PyObject* py_lockHeadingXdeg(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  return Py_BuildValue("i",lockHeadingXdeg());
}
static PyObject* py_lockHeadingDeg(PyObject* pySelf, PyObject* args) {   //returns the angle at which the player's lock is in relation to the player's ship -JNE
  only_available_if_connected
  return Py_BuildValue("d",lockHeadingDeg());
}
static PyObject* py_lockHeadingRad(PyObject* pySelf, PyObject* args) {   //returns the angle at which the player's lock is in relation to the player's ship -JNE
  only_available_if_connected
  return Py_BuildValue("d",lockHeadingRad());
}
static PyObject* py_selfLockDist(PyObject* pySelf, PyObject* args) {      //returns the distance of the ship the player is locked onto -JNE
  only_available_if_connected
  return Py_BuildValue("i",selfLockDist()); 
}
static PyObject* py_selfReload(PyObject* pySelf, PyObject* args) {    //returns the player's reload time remaining
  only_available_if_connected
  return Py_BuildValue("i",selfReload());
}
static PyObject* py_selfId(PyObject* pySelf, PyObject* args) { 
  only_available_if_connected
  return Py_BuildValue("i",selfId());
}
static PyObject* py_selfAlive(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  return Py_BuildValue("i",selfAlive());
}
static PyObject* py_selfTeam(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  return Py_BuildValue("i",selfTeam());
}
static PyObject* py_selfLives(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  return Py_BuildValue("i",selfLives());
}
static PyObject* py_selfTrackingRad(PyObject* pySelf, PyObject* args) {  //returns the player's tracking in radians -JNE
  only_available_if_connected
  return Py_BuildValue("d",selfTrackingRad());
}
static PyObject* py_selfTrackingDeg(PyObject* pySelf, PyObject* args) {  //returns the player's tracking in degrees -JNE
  only_available_if_connected
  //if (vel.y == 0 && vel.x == 0) return Py_BuildValue("d",(double)selfHeadingDeg()); //fix for NaN -EGG -CJG   
  return Py_BuildValue("d",selfTrackingDeg());
}
static PyObject* py_selfHeadingDeg(PyObject* pySelf, PyObject* args) {   //returns the player's heading in degrees  -JNE
  only_available_if_connected
  return Py_BuildValue("d",selfHeadingDeg());
}
static PyObject* py_selfHeadingRad(PyObject* pySelf, PyObject* args) {   //returns the player's heading in radians  -JNE
  only_available_if_connected
  return Py_BuildValue("d",selfHeadingRad());
}
static PyObject* py_selfHeadingXdeg(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  return Py_BuildValue("i",selfHeading());
}
static PyObject* py_hud(PyObject* pySelf, PyObject* args) {         //if the HUD is displaying a name, return it  -JNE
  only_available_if_connected
  int i; 
  if (!PyArg_ParseTuple(args, "i", &i)){
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  return Py_BuildValue("s",hud(i));
}
static PyObject* py_hudScore(PyObject* pySelf, PyObject* args) {        //if the HUD is displaying a score, return it -JNE
  only_available_if_connected
  int i;
  if (!PyArg_ParseTuple(args, "i", &i)){
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  return Py_BuildValue("s",hudScore(i));
}
static PyObject* py_hudTimeLeft(PyObject* pySelf, PyObject* args) {      //returns how much time the HUD will keep displaying a score for, in seconds -JNE
  only_available_if_connected
  int i;
  if (!PyArg_ParseTuple(args, "i", &i)){
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  return Py_BuildValue("i",hudTimeLeft(i));
}
static PyObject* py_getTurnSpeed(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  return Py_BuildValue("d",getTurnSpeed());
}
static PyObject* py_getPower(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  return Py_BuildValue("d",getPower());
}
static PyObject* py_getTurnResistance(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  return Py_BuildValue("d",getTurnResistance());
}
//Returns 1 if the player's shield is on, 0 if it is not, -1 if player is not alive. -EGG
static PyObject* py_selfShield(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  return Py_BuildValue("i",selfShield());
}
//Returns the player's username (string). -EGG
static PyObject* py_selfName(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  return Py_BuildValue("s",selfName());
}
//Returns the player's score (double). -EGG
static PyObject* py_selfScore(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  return Py_BuildValue("d",selfScore());
}
static PyObject* py_selfItem(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int i, result;
  if (!PyArg_ParseTuple(args, "i", &i)){
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  result = selfItem(i);
  if (result == -1) {
    PyErr_SetString(PyExc_TypeError, "invalid parameter, must be between 0 and 20");
    return NULL;
  }
  else {
    return Py_BuildValue("i",result);
  }
}
static PyObject* py_selfFuel(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  return Py_BuildValue("i",selfFuel());
}
static PyObject* py_selfFuelMax(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  return Py_BuildValue("i",selfFuelMax());
}
static PyObject* py_selfFuelCurrent(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  return Py_BuildValue("i",selfFuelCurrent());
}
static PyObject* py_selfMass(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  double result = selfMass();
  if (isnan(result)) {
      PyErr_SetString(PyExc_RuntimeError,
                      "client is unaware of its mass immediately after starting");
      return NULL;
  }
  return Py_BuildValue("d", result);
}
static PyObject* py_closestRadarId(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  return Py_BuildValue("i", closestRadarId());
}
static PyObject* py_selfRadarX(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  return Py_BuildValue("i", selfRadarX());
}
static PyObject* py_selfRadarY(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  return Py_BuildValue("i", selfRadarY());
}
static PyObject* py_radarX(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int id;
  if (!PyArg_ParseTuple(args, "i", &id)) {
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (radarIdCheck(id) == 1) {
    PyErr_SetString(PyExc_IndexError, "No radar object with that id");
    return NULL;
  }
  return Py_BuildValue("i", radarX(id));
}
static PyObject* py_radarY(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int id;
  if (!PyArg_ParseTuple(args, "i", &id)) {
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (radarIdCheck(id) == 1) {
    PyErr_SetString(PyExc_IndexError, "No radar object with that id");
    return NULL;
  }
  return Py_BuildValue("i", radarY(id));
}
static PyObject* py_radarVelX(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int id;
  if (!PyArg_ParseTuple(args, "i", &id)) {
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (radarIdCheck(id) == 1) {
    PyErr_SetString(PyExc_IndexError, "No radar object with that id");
    return NULL;
  }
  return Py_BuildValue("d", radarVelX(id));
}
static PyObject* py_radarVelY(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int id;
  if (!PyArg_ParseTuple(args, "i", &id)) {
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (radarIdCheck(id) == 1) {
    PyErr_SetString(PyExc_IndexError, "No radar object with that id");
    return NULL;
  }
  return Py_BuildValue("d", radarVelY(id));
}
static PyObject* py_radarType(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int id;
  if (!PyArg_ParseTuple(args, "i", &id)) {
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (radarIdCheck(id) == 1) {
    PyErr_SetString(PyExc_IndexError, "No radar object with that id");
    return NULL;
  }
  return Py_BuildValue("i", radarType(id));
}
static PyObject* py_radarDist(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int id;
  if (!PyArg_ParseTuple(args, "i", &id)) {
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (radarIdCheck(id) == 1) {
    PyErr_SetString(PyExc_IndexError, "No radar object with that id");
    return NULL;
  }
  return Py_BuildValue("d", radarDist(id));
}
static PyObject* py_radarCount(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  return Py_BuildValue("i",radarCount());
}
static PyObject* py_radarHeight(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  return Py_BuildValue("i",radarHeight());
}
static PyObject* py_radarWidth(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  return Py_BuildValue("i",radarWidth());
}
static PyObject* py_playerCountServer(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  return Py_BuildValue("i",playerCountServer());
}
static PyObject* py_pausedCountServer(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  return Py_BuildValue("i",pausedCountServer());
}
static PyObject* py_tankCountServer(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  return Py_BuildValue("i",tankCountServer());
}
static PyObject* py_shipCountScreen(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  return Py_BuildValue("i", shipCountScreen());
}
static PyObject* py_shipId(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int idx;
  if (!PyArg_ParseTuple(args, "i", &idx)) {
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (shipIdCheck(idx) != 0) {     
    PyErr_SetString(PyExc_TypeError, "No ship with that id");     
    return NULL;   
  }
  return Py_BuildValue("i", shipId(idx));
}
static PyObject* py_shipX(PyObject* pySelf, PyObject* args) {    //returns x coordinate of ship at an index -JNE
  only_available_if_connected
  int idx;
  if (!PyArg_ParseTuple(args, "i", &idx)){
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (shipIdCheck(idx) != 0) {
    PyErr_SetString(PyExc_TypeError, "No ship with that id");
    return NULL;
  }
  return Py_BuildValue("i",shipX(idx));
}
static PyObject* py_shipY(PyObject* pySelf, PyObject* args) {    //returns y coordinate of ship at an index -JNE
  only_available_if_connected
  int idx;
  if (!PyArg_ParseTuple(args, "i", &idx)){
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (shipIdCheck(idx) != 0) {
    PyErr_SetString(PyExc_TypeError, "No ship with that id");
    return NULL;
  }
  return Py_BuildValue("i",shipY(idx));
}
static PyObject* py_shipDistance(PyObject* pySelf, PyObject* args) { //returns the distance of a ship with a particular index -JNE
  only_available_if_connected
  int idx;
  if (!PyArg_ParseTuple(args, "i", &idx)){
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (shipIdCheck(idx) != 0) {
    PyErr_SetString(PyExc_TypeError, "No ship with that id");
    return NULL;
  }
  return Py_BuildValue("d",shipDistance(idx));
}
static PyObject* py_shipVelX(PyObject* pySelf, PyObject* args) { //returns velocity of a ship with a particular index -JNE
  only_available_if_connected
  int idx;
  if (!PyArg_ParseTuple(args, "i", &idx)){
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (shipIdCheck(idx) != 0) {
    PyErr_SetString(PyExc_TypeError, "No ship with that id");
    return NULL;
  }
  return Py_BuildValue("d",shipVelX(idx));
}
static PyObject* py_shipVelY(PyObject* pySelf, PyObject* args) { //returns velocity of a ship with a particular index -JNE
  only_available_if_connected
  int idx;
  if (!PyArg_ParseTuple(args, "i", &idx)){
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (shipIdCheck(idx) != 0) {
    PyErr_SetString(PyExc_TypeError, "No ship with that id");
    return NULL;
  }
  return Py_BuildValue("d",shipVelY(idx));
}
static PyObject* py_shipSpeed(PyObject* pySelf, PyObject* args) {  //returns velocity of a ship with a particular index -JNE
  only_available_if_connected
  int idx;
  if (!PyArg_ParseTuple(args, "i", &idx)){
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (shipIdCheck(idx) != 0) {
    PyErr_SetString(PyExc_TypeError, "No ship with that id");
    return NULL;
  }
  return Py_BuildValue("d",shipSpeed(idx));
}
static PyObject* py_shipTrackingRad(PyObject* pySelf, PyObject* args) {  //returns tracking based on index -JNE
  only_available_if_connected
  int idx;
  if (!PyArg_ParseTuple(args, "i", &idx)){
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (shipIdCheck(idx) != 0) {
    PyErr_SetString(PyExc_TypeError, "No ship with that id");
    return NULL;
  }
  return Py_BuildValue("d",shipTrackingRad(idx));
}
static PyObject* py_shipTrackingDeg(PyObject* pySelf, PyObject* args) {  //returns tracking based on index -JNE
  only_available_if_connected
  int idx;
  if (!PyArg_ParseTuple(args, "i", &idx)){
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (shipIdCheck(idx) != 0) {
    PyErr_SetString(PyExc_TypeError, "No ship with that id");
    return NULL;
  }
  return Py_BuildValue("d",shipTrackingDeg(idx));
}
static PyObject* py_shipHeadingXdeg(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int idx;
  if (!PyArg_ParseTuple(args, "i", &idx)) {
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (shipIdCheck(idx) != 0) {
    PyErr_SetString(PyExc_TypeError, "No ship with that id");
    return NULL;
  }
  return Py_BuildValue("i",shipHeadingXdeg(idx));
}
static PyObject* py_shipHeadingDeg(PyObject* pySelf, PyObject* args) {   //returns heading in degrees of ship at an index -JNE
  only_available_if_connected
  int idx;
  if (!PyArg_ParseTuple(args, "i", &idx)){
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (shipIdCheck(idx) != 0) {
    PyErr_SetString(PyExc_TypeError, "No ship with that id");
    return NULL;
  }
  return Py_BuildValue("d",shipHeadingDeg(idx));
}
static PyObject* py_shipHeadingRad(PyObject* pySelf, PyObject* args) {   //returns heading in radians of ship at an index -JNE
  only_available_if_connected
  int idx;
  if (!PyArg_ParseTuple(args, "i", &idx)){
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (shipIdCheck(idx) != 0) {
    PyErr_SetString(PyExc_TypeError, "No ship with that id");
    return NULL;
  }
  return Py_BuildValue("d",shipHeadingRad(idx));
}
static PyObject* py_shipShield(PyObject* pySelf, PyObject* args) {   //returns shield status of ship at an index -JNE
  only_available_if_connected
  int idx;
  if (!PyArg_ParseTuple(args, "i", &idx)){
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (shipIdCheck(idx) != 0) {
    PyErr_SetString(PyExc_TypeError, "No ship with that id");
    return NULL;
  }
  return Py_BuildValue("i",shipShield(idx));
}
static PyObject* py_playerId(PyObject* pySelf, PyObject* args) {    //returns lives of player at an index -JNE
  only_available_if_connected
  int idx;
  if (!PyArg_ParseTuple(args, "i", &idx)){
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (playerIdCheck(idx) != 0) {
    PyErr_SetString(PyExc_TypeError, "No player with that id");
    return NULL;
  }
  return Py_BuildValue("i",playerId(idx));
}
static PyObject* py_playerLives(PyObject* pySelf, PyObject* args) {    //returns lives of player at an index -JNE
  only_available_if_connected
  int idx;
  if (!PyArg_ParseTuple(args, "i", &idx)){
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (playerIdCheck(idx) != 0) {
    PyErr_SetString(PyExc_TypeError, "No player with that id");
    return NULL;
  }
  return Py_BuildValue("i",playerLives(idx));
}
static PyObject* py_playerTeam(PyObject* pySelf, PyObject* args) { //returns team of player at an index -JNE
  only_available_if_connected
  int idx;
  if (!PyArg_ParseTuple(args, "i", &idx)){
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (playerIdCheck(idx) != 0) {
    PyErr_SetString(PyExc_TypeError, "No player with that id");
    return NULL;
  }
  return Py_BuildValue("i",playerTeam(idx));
}
static PyObject* py_playerName(PyObject* pySelf, PyObject* args) {   //returns name of player at an index -JNE
  only_available_if_connected
  int idx;
  if (!PyArg_ParseTuple(args, "i", &idx)){
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (playerIdCheck(idx) != 0) {
    PyErr_SetString(PyExc_TypeError, "No player with that id");
    return NULL;
  }
  return Py_BuildValue("s",playerName(idx));
}
static PyObject* py_playerScore(PyObject* pySelf, PyObject* args) {    //returns score of player at an index -JNE
  only_available_if_connected
  int idx;
  if (!PyArg_ParseTuple(args, "i", &idx)){
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (playerIdCheck(idx) != 0) {
    PyErr_SetString(PyExc_TypeError, "No player with that id");
    return NULL;
  }
  return Py_BuildValue("d",playerScore(idx));
}
static PyObject* py_xdegToDeg(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  double xdeg;
  if (!PyArg_ParseTuple(args, "d", &xdeg)){
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  return Py_BuildValue("d",AI_xdegToDeg(xdeg));
}
static PyObject* py_xdegToRad(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  double xdeg;
  if (!PyArg_ParseTuple(args, "d", &xdeg)){
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  return Py_BuildValue("d",AI_xdegToRad(xdeg));
}
static PyObject* py_degToXdeg(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  double deg;
  if (!PyArg_ParseTuple(args, "d", &deg)){
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  return Py_BuildValue("d",AI_degToXdeg(deg));
}
static PyObject* py_degToRad(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  double deg;
  if (!PyArg_ParseTuple(args, "d", &deg)){
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  return Py_BuildValue("d",AI_degToRad(deg));
}
static PyObject* py_radToXdeg(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  double rad;
  if (!PyArg_ParseTuple(args, "d", &rad)){
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  return Py_BuildValue("d",AI_radToXdeg(rad));
}
//Converts radians (double) to degrees (double). -EGG -hatten
static PyObject* py_radToDeg(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  double rad;
  if (!PyArg_ParseTuple(args, "d", &rad)){
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  return Py_BuildValue("d",AI_radToDeg(rad));
}
static PyObject* py_wallFeelerDeg(PyObject* pySelf, PyObject* args) { //removed flags -CJG
  only_available_if_connected
  double dist, angle;
  if (!PyArg_ParseTuple(args, "dd", &dist, &angle)){
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  return Py_BuildValue("d",wallFeelerDeg(dist, angle));
}
static PyObject* py_wallFeelerRad(PyObject* pySelf, PyObject* args) { //removed flags -CJG
  only_available_if_connected
  double dist, angle;
  if (!PyArg_ParseTuple(args, "dd", &dist, &angle)){
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  return Py_BuildValue("d",wallFeelerRad(dist, angle));
}
static PyObject* py_blockSize(PyObject* pySelf, PyObject* args) {
  return Py_BuildValue("i",blockSize());
}
static PyObject* py_mapWidthBlocks(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  return Py_BuildValue("i",mapWidthBlocks());
}
static PyObject* py_mapHeightBlocks(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  return Py_BuildValue("i",mapHeightBlocks());
}
static PyObject* py_mapWidthPixels(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  return Py_BuildValue("i",mapWidthPixels());
}
static PyObject* py_mapHeightPixels(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  return Py_BuildValue("i",mapHeightPixels());
}
static PyObject* py_mapData(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int x,y;
  if (!PyArg_ParseTuple(args, "ii", &x, &y)){
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  return Py_BuildValue("i",mapData(x, y));
}
static PyObject* py_getOption(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  char *name[64];
  int result, i;
  if (!PyArg_ParseTuple(args, "s", &name)){
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  //convert to lowercase -hatten
  for (i=0; name[0][i]; i++) {
    name[0][i] = tolower(name[0][i]);
  }
  result = getOption(*name);
  switch(result) {
    case -5:
      return Py_BuildValue("s", "queued");
    case -4:
      PyErr_SetString(PyExc_TypeError, "Option unavailable");
      return NULL;
    case -3:
      PyErr_SetString(PyExc_TypeError, "unknown status");
      return NULL;
    case -2:
      PyErr_SetString(PyExc_TypeError, "option doesn't exist, or not supported");
      return NULL;
    case -1:
      PyErr_SetString(PyExc_TypeError, "generic error");
      return NULL;
    default:
      //TODO: remake so storedOptions doesn't have to be global
      switch (storedOptions[result].type) {
        case INTTYPE:
        case BOOLTYPE:
          return Py_BuildValue("i", storedOptions[result].intValue);
        case DOUBLETYPE:
          return Py_BuildValue("d", storedOptions[result].doubleValue);
        case STRINGTYPE:
          return Py_BuildValue("s", storedOptions[result].stringValue);
        default:
          PyErr_SetString(PyExc_TypeError, "unknown type");
          return NULL;
      }
  }
}
static PyObject* py_wallBetween(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  double x1, y1, x2, y2;
  if (!PyArg_ParseTuple(args, "dddd", &x1, &y1, &x2, &y2)){
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  double result = wallBetween(x1, y1, x2, y2);
  if (result == -1)
    return Py_BuildValue("i",-1);
  else
    return Py_BuildValue("d", result);
}
static PyObject* py_shotCountScreen(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  return Py_BuildValue("i",shotCountScreen());
}
static PyObject* py_shotX(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int idx;
  if (!PyArg_ParseTuple(args, "i", &idx)){
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (shotIdCheck(idx) == 1) {
    PyErr_SetString(PyExc_IndexError, "No shot with that id");
    return NULL;
  }
  return Py_BuildValue("i",shotX(idx));
}
static PyObject* py_shotY(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int idx;
  if (!PyArg_ParseTuple(args, "i", &idx)){
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (shotIdCheck(idx) == 1) {
    PyErr_SetString(PyExc_IndexError, "No shot with that id");
    return NULL;
  }
  return Py_BuildValue("i",shotY(idx));
}
static PyObject* py_shotVelX(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int idx;
  if (!PyArg_ParseTuple(args, "i", &idx)){
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (shotIdCheck(idx) == 1) {
    PyErr_SetString(PyExc_IndexError, "No shot with that id");
    return NULL;
  }
  return Py_BuildValue("d",shotVelX(idx));
}
static PyObject* py_shotVelY(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int idx;
  if (!PyArg_ParseTuple(args, "i", &idx)){
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (shotIdCheck(idx) == 1) {
    PyErr_SetString(PyExc_IndexError, "No shot with that id");
    return NULL;
  }
  return Py_BuildValue("d",shotVelY(idx));
}
static PyObject* py_shotFriendly(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int idx;
  if (!PyArg_ParseTuple(args, "i", &idx)){
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (shotIdCheck(idx) == 1) {
    PyErr_SetString(PyExc_IndexError, "No shot with that id");
    return NULL;
  }
  return Py_BuildValue("i",shotFriendly(idx));
}
static PyObject* py_shotDist(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int idx;
  if (!PyArg_ParseTuple(args, "i", &idx)){
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (shotIdCheck(idx) == 1) {
    PyErr_SetString(PyExc_IndexError, "No shot with that id");
    return NULL;
  }
  return Py_BuildValue("d",shotDist(idx));
}
static PyObject* py_shotSpeed(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int idx;
  if (!PyArg_ParseTuple(args, "i", &idx)){
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (shotIdCheck(idx) == 1) {
    PyErr_SetString(PyExc_IndexError, "No shot with that id");
    return NULL;
  }
  return Py_BuildValue("d",shotSpeed(idx));
}
static PyObject* py_shotTrackingRad(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int idx;
  if (!PyArg_ParseTuple(args, "i", &idx)){
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (shotIdCheck(idx) == 1) {
    PyErr_SetString(PyExc_IndexError, "No shot with that id");
    return NULL;
  }
  return Py_BuildValue("d",shotTrackingRad(idx));
}
static PyObject* py_shotTrackingDeg(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int idx;
  if (!PyArg_ParseTuple(args, "i", &idx)){
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (shotIdCheck(idx) == 1) {
    PyErr_SetString(PyExc_IndexError, "No shot with that id");
    return NULL;
  }
  return Py_BuildValue("d",shotTrackingDeg(idx));
}
static PyObject* py_asteroidCountScreen(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  return Py_BuildValue("i",asteroidCountScreen());
}
static PyObject* py_asteroidX(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int idx;
  if (!PyArg_ParseTuple(args, "i", &idx)){
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (asteroidIdCheck(idx) == 1) {
    PyErr_SetString(PyExc_IndexError, "No shot with that id");
    return NULL;
  }
  return Py_BuildValue("i",asteroidX(idx));
}
static PyObject* py_asteroidY(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int idx;
  if (!PyArg_ParseTuple(args, "i", &idx)){
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (asteroidIdCheck(idx) == 1) {
    PyErr_SetString(PyExc_IndexError, "No asteroid with that id");
    return NULL;
  }
  return Py_BuildValue("i",asteroidY(idx));
}
static PyObject* py_asteroidVelX(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int idx;
  if (!PyArg_ParseTuple(args, "i", &idx)){
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (asteroidIdCheck(idx) == 1) {
    PyErr_SetString(PyExc_IndexError, "No asteroid with that id");
    return NULL;
  }
  return Py_BuildValue("d",asteroidVelX(idx));
}
static PyObject* py_asteroidVelY(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int idx;
  if (!PyArg_ParseTuple(args, "i", &idx)){
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (asteroidIdCheck(idx) == 1) {
    PyErr_SetString(PyExc_IndexError, "No asteroid with that id");
    return NULL;
  }
  return Py_BuildValue("d",asteroidVelY(idx));
}
static PyObject* py_asteroidDist(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int idx;
  if (!PyArg_ParseTuple(args, "i", &idx)){
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (asteroidIdCheck(idx) == 1) {
    PyErr_SetString(PyExc_IndexError, "No asteroid with that id");
    return NULL;
  }
  return Py_BuildValue("d",asteroidDist(idx));
}
static PyObject* py_asteroidSpeed(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int idx;
  if (!PyArg_ParseTuple(args, "i", &idx)){
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (asteroidIdCheck(idx) == 1) {
    PyErr_SetString(PyExc_IndexError, "No asteroid with that id");
    return NULL;
  }
  return Py_BuildValue("d",asteroidSpeed(idx));
}
static PyObject* py_asteroidType(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int idx;
  if (!PyArg_ParseTuple(args, "i", &idx)){
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (asteroidIdCheck(idx) == 1) {
    PyErr_SetString(PyExc_IndexError, "No asteroid with that id");
    return NULL;
  }
  return Py_BuildValue("i",asteroidType(idx));
}
static PyObject* py_asteroidSize(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int idx;
  if (!PyArg_ParseTuple(args, "i", &idx)){
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (asteroidIdCheck(idx) == 1) {
    PyErr_SetString(PyExc_IndexError, "No asteroid with that id");
    return NULL;
  }
  return Py_BuildValue("i",asteroidSize(idx));
}
static PyObject* py_asteroidTrackingRad(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int idx;
  if (!PyArg_ParseTuple(args, "i", &idx)){
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (asteroidIdCheck(idx) == 1) {
    PyErr_SetString(PyExc_IndexError, "No asteroid with that id");
    return NULL;
  }
  return Py_BuildValue("d",asteroidTrackingRad(idx));
}
static PyObject* py_asteroidTrackingDeg(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int idx;
  if (!PyArg_ParseTuple(args, "i", &idx)){
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (asteroidIdCheck(idx) == 1) {
    PyErr_SetString(PyExc_IndexError, "No asteroid with that id");
    return NULL;
  }
  return Py_BuildValue("d",asteroidTrackingDeg(idx));
}
static PyObject* py_asteroidRotation(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int idx;
  if (!PyArg_ParseTuple(args, "i", &idx)){
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (asteroidIdCheck(idx) == 1) {
    PyErr_SetString(PyExc_IndexError, "No asteroid with that id");
    return NULL;
  }
  return Py_BuildValue("i",asteroidRotation(idx));
}

static PyObject* py_phasingTime(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  return Py_BuildValue("i",phasingTime());
}
static PyObject* py_nextCheckpoint(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  return Py_BuildValue("i",getNextCheckpoint());
}
static PyObject* py_checkpointX(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int id;
  if (!PyArg_ParseTuple(args, "i", &id)) {
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (checkpointIdCheck(id) == 1) {
    PyErr_SetString(PyExc_IndexError, "invalid value for index");
    return NULL;
  }
  return Py_BuildValue("i", checkpointX(id));
}
static PyObject* py_checkpointY(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int id;
  if (!PyArg_ParseTuple(args, "i", &id)) {
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (checkpointIdCheck(id) == 1) {
    PyErr_SetString(PyExc_IndexError, "invalid value for index");
    return NULL;
  }
  return Py_BuildValue("i", checkpointY(id));
}
static PyObject* py_checkpointBlockX(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int id;
  if (!PyArg_ParseTuple(args, "i", &id)) {
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (checkpointIdCheck(id) == 1) {
    PyErr_SetString(PyExc_IndexError, "invalid value for index");
    return NULL;
  }
  return Py_BuildValue("i", checkpointBlockX(id));
}
static PyObject* py_checkpointBlockY(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int id;
  if (!PyArg_ParseTuple(args, "i", &id)) {
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (checkpointIdCheck(id) == 1) {
    PyErr_SetString(PyExc_IndexError, "invalid value for index");
    return NULL;
  }
  return Py_BuildValue("i", checkpointBlockY(id));
}
static PyObject* py_connectorCountScreen(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  return Py_BuildValue("i",connectorCountScreen());
}
static PyObject* py_connectorX0(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int id;
  if (!PyArg_ParseTuple(args, "i", &id)) {
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (connectorIdCheck(id) == 1) {
    PyErr_SetString(PyExc_IndexError, "invalid value for index");
    return NULL;
  }
  return Py_BuildValue("i",connectorX0(id));
}
static PyObject* py_connectorY0(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int id;
  if (!PyArg_ParseTuple(args, "i", &id)) {
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (connectorIdCheck(id) == 1) {
    PyErr_SetString(PyExc_IndexError, "invalid value for index");
    return NULL;
  }
  return Py_BuildValue("i",connectorY0(id));
}
static PyObject* py_connectorX1(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int id;
  if (!PyArg_ParseTuple(args, "i", &id)) {
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (connectorIdCheck(id) == 1) {
    PyErr_SetString(PyExc_IndexError, "invalid value for index");
    return NULL;
  }
  return Py_BuildValue("i",connectorX1(id));
}
static PyObject* py_connectorY1(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int id;
  if (!PyArg_ParseTuple(args, "i", &id)) {
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (connectorIdCheck(id) == 1) {
    PyErr_SetString(PyExc_IndexError, "invalid value for index");
    return NULL;
  }
  return Py_BuildValue("i",connectorY1(id));
}
static PyObject* py_connectorTractor(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int id;
  if (!PyArg_ParseTuple(args, "i", &id)) {
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (connectorIdCheck(id) == 1) {
    PyErr_SetString(PyExc_IndexError, "invalid value for index");
    return NULL;
  }
  return Py_BuildValue("i",connectorTractor(id));
}
static PyObject* py_missileCountScreen(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  return Py_BuildValue("i",missileCountScreen());
}
static PyObject* py_missileX(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int id;
  if (!PyArg_ParseTuple(args, "i", &id)) {
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (missileIdCheck(id) == 1) {
    PyErr_SetString(PyExc_IndexError, "invalid value for index");
    return NULL;
  }
  return Py_BuildValue("i",missileX(id));
}
static PyObject* py_missileY(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int id;
  if (!PyArg_ParseTuple(args, "i", &id)) {
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (missileIdCheck(id) == 1) {
    PyErr_SetString(PyExc_IndexError, "invalid value for index");
    return NULL;
  }
  return Py_BuildValue("i",missileY(id));
}
static PyObject* py_missileLen(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int id;
  if (!PyArg_ParseTuple(args, "i", &id)) {
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (missileIdCheck(id) == 1) {
    PyErr_SetString(PyExc_IndexError, "invalid value for index");
    return NULL;
  }
  return Py_BuildValue("i",missileLen(id));
}
static PyObject* py_missileHeadingXdeg(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int id;
  if (!PyArg_ParseTuple(args, "i", &id)) {
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (missileIdCheck(id) == 1) {
    PyErr_SetString(PyExc_IndexError, "invalid value for index");
    return NULL;
  }
  return Py_BuildValue("i",missileHeadingXdeg(id));
}
static PyObject* py_missileHeadingDeg(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int id;
  if (!PyArg_ParseTuple(args, "i", &id)) {
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (missileIdCheck(id) == 1) {
    PyErr_SetString(PyExc_IndexError, "invalid value for index");
    return NULL;
  }
  return Py_BuildValue("i",missileHeadingDeg(id));
}
static PyObject* py_missileHeadingRad(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int id;
  if (!PyArg_ParseTuple(args, "i", &id)) {
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (missileIdCheck(id) == 1) {
    PyErr_SetString(PyExc_IndexError, "invalid value for index");
    return NULL;
  }
  return Py_BuildValue("i",missileHeadingRad(id));
}
static PyObject* py_laserCountScreen(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  return Py_BuildValue("i",laserCountScreen());
}
static PyObject* py_laserX(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int id;
  if (!PyArg_ParseTuple(args, "i", &id)) {
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (laserIdCheck(id) == 1) {
    PyErr_SetString(PyExc_IndexError, "invalid value for index");
    return NULL;
  }
  return Py_BuildValue("i",laserX(id));
}
static PyObject* py_laserY(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int id;
  if (!PyArg_ParseTuple(args, "i", &id)) {
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (laserIdCheck(id) == 1) {
    PyErr_SetString(PyExc_IndexError, "invalid value for index");
    return NULL;
  }
  return Py_BuildValue("i",laserY(id));
}
static PyObject* py_laserLen(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int id;
  if (!PyArg_ParseTuple(args, "i", &id)) {
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (laserIdCheck(id) == 1) {
    PyErr_SetString(PyExc_IndexError, "invalid value for index");
    return NULL;
  }
  return Py_BuildValue("i",laserLen(id));
}
static PyObject* py_laserHeadingXdeg(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int id;
  if (!PyArg_ParseTuple(args, "i", &id)) {
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (laserIdCheck(id) == 1) {
    PyErr_SetString(PyExc_IndexError, "invalid value for index");
    return NULL;
  }
  return Py_BuildValue("i",laserHeadingXdeg(id));
}
static PyObject* py_laserHeadingDeg(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int id;
  if (!PyArg_ParseTuple(args, "i", &id)) {
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (laserIdCheck(id) == 1) {
    PyErr_SetString(PyExc_IndexError, "invalid value for index");
    return NULL;
  }
  return Py_BuildValue("i",laserHeadingDeg(id));
}
static PyObject* py_laserHeadingRad(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int id;
  if (!PyArg_ParseTuple(args, "i", &id)) {
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (laserIdCheck(id) == 1) {
    PyErr_SetString(PyExc_IndexError, "invalid value for index");
    return NULL;
  }
  return Py_BuildValue("d", laserHeadingRad(id));
}
static PyObject* py_ballCountScreen(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  return Py_BuildValue("i",ballCountScreen());
}
static PyObject* py_ballX(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int id;
  if (!PyArg_ParseTuple(args, "i", &id)) {
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (ballIdCheck(id) == 1) {
    PyErr_SetString(PyExc_IndexError, "invalid value for index");
    return NULL;
  }
  return Py_BuildValue("i",ballX(id));
}
static PyObject* py_ballY(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int id;
  if (!PyArg_ParseTuple(args, "i", &id)) {
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (ballIdCheck(id) == 1) {
    PyErr_SetString(PyExc_IndexError, "invalid value for index");
    return NULL;
  }
  return Py_BuildValue("i",ballY(id));
}
static PyObject* py_ballId(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int id;
  if (!PyArg_ParseTuple(args, "i", &id)) {
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (ballIdCheck(id) == 1) {
    PyErr_SetString(PyExc_IndexError, "invalid value for index");
    return NULL;
  }
  return Py_BuildValue("i",ballId(id));
}
static PyObject* py_mineCountScreen(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  return Py_BuildValue("i",mineCountScreen());
}
static PyObject* py_mineX(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int id;
  if (!PyArg_ParseTuple(args, "i", &id)) {
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (mineIdCheck(id) == 1) {
    PyErr_SetString(PyExc_IndexError, "invalid value for index");
    return NULL;
  }
  return Py_BuildValue("i",mineX(id));
}
static PyObject* py_mineY(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int id;
  if (!PyArg_ParseTuple(args, "i", &id)) {
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (mineIdCheck(id) == 1) {
    PyErr_SetString(PyExc_IndexError, "invalid value for index");
    return NULL;
  }
  return Py_BuildValue("i",mineY(id));
}
static PyObject* py_mineFriendly(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int id;
  if (!PyArg_ParseTuple(args, "i", &id)) {
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (mineIdCheck(id) == 1) {
    PyErr_SetString(PyExc_IndexError, "invalid value for index");
    return NULL;
  }
  return Py_BuildValue("i",mineFriendly(id));
}
static PyObject* py_mineId(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int id;
  if (!PyArg_ParseTuple(args, "i", &id)) {
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (mineIdCheck(id) == 1) {
    PyErr_SetString(PyExc_IndexError, "invalid value for index");
    return NULL;
  }
  return Py_BuildValue("i",mineId(id));
}
static PyObject* py_wormholeCountScreen(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  return Py_BuildValue("i",wormholeCountScreen());
}
static PyObject* py_wormholeX(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int id;
  if (!PyArg_ParseTuple(args, "i", &id)) {
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (wormholeIdCheck(id) == 1) {
    PyErr_SetString(PyExc_IndexError, "invalid value for index");
    return NULL;
  }
  return Py_BuildValue("i",wormholeX(id));
}
static PyObject* py_wormholeY(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int id;
  if (!PyArg_ParseTuple(args, "i", &id)) {
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (wormholeIdCheck(id) == 1) {
    PyErr_SetString(PyExc_IndexError, "invalid value for index");
    return NULL;
  }
  return Py_BuildValue("i",wormholeY(id));
}
static PyObject* py_ecmCountScreen(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  return Py_BuildValue("i",ecmCountScreen());
}
static PyObject* py_ecmX(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int id;
  if (!PyArg_ParseTuple(args, "i", &id)) {
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (ecmIdCheck(id) == 1) {
    PyErr_SetString(PyExc_IndexError, "invalid value for index");
    return NULL;
  }
  return Py_BuildValue("i",ecmX(id));
}
static PyObject* py_ecmY(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int id;
  if (!PyArg_ParseTuple(args, "i", &id)) {
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (ecmIdCheck(id) == 1) {
    PyErr_SetString(PyExc_IndexError, "invalid value for index");
    return NULL;
  }
  return Py_BuildValue("i",ecmY(id));
}
static PyObject* py_timeLeftSec(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  return Py_BuildValue("i",timeLeftSec());
}
static PyObject* py_fuelstationCount(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  return Py_BuildValue("i",fuelstationCount());
}
static PyObject* py_fuelstationX(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int id;
  if (!PyArg_ParseTuple(args, "i", &id)) {
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (fuelstationIdCheck(id) == 1) {
    PyErr_SetString(PyExc_IndexError, "invalid value for index");
    return NULL;
  }
  return Py_BuildValue("i",fuelstationX(id));
}
static PyObject* py_fuelstationY(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int id;
  if (!PyArg_ParseTuple(args, "i", &id)) {
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (fuelstationIdCheck(id) == 1) {
    PyErr_SetString(PyExc_IndexError, "invalid value for index");
    return NULL;
  }
  return Py_BuildValue("i",fuelstationY(id));
}
static PyObject* py_fuelstationBlockX(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int id;
  if (!PyArg_ParseTuple(args, "i", &id)) {
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (fuelstationIdCheck(id) == 1) {
    PyErr_SetString(PyExc_IndexError, "invalid value for index");
    return NULL;
  }
  return Py_BuildValue("i",fuelstationBlockX(id));
}
static PyObject* py_fuelstationBlockY(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int id;
  if (!PyArg_ParseTuple(args, "i", &id)) {
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (fuelstationIdCheck(id) == 1) {
    PyErr_SetString(PyExc_IndexError, "invalid value for index");
    return NULL;
  }
  return Py_BuildValue("i",fuelstationBlockY(id));
}
static PyObject* py_fuelstationFuel(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int id;
  if (!PyArg_ParseTuple(args, "i", &id)) {
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (fuelstationIdCheck(id) == 1) {
    PyErr_SetString(PyExc_IndexError, "invalid value for index");
    return NULL;
  }
  return Py_BuildValue("i",fuelstationFuel(id));
}
static PyObject* py_fuelstationTeam(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int id;
  if (!PyArg_ParseTuple(args, "i", &id)) {
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (fuelstationIdCheck(id) == 1) {
    PyErr_SetString(PyExc_IndexError, "invalid value for index");
    return NULL;
  }
  if (fuelstationIdCheck(id) == 2) {
    PyErr_SetString(PyExc_IndexError, "team play not enabled");
    return NULL;
  }
  return Py_BuildValue("i",fuelstationTeam(id));
}
static PyObject* py_cannonCountServer(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  return Py_BuildValue("i",cannonCountServer());
}
static PyObject* py_cannonX(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int id;
  if (!PyArg_ParseTuple(args, "i", &id)) {
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (cannonIdCheck(id) == 1) {
    PyErr_SetString(PyExc_IndexError, "invalid value for index");
    return NULL;
  }
  return Py_BuildValue("i",cannonX(id));
}
static PyObject* py_cannonY(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int id;
  if (!PyArg_ParseTuple(args, "i", &id)) {
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (cannonIdCheck(id) == 1) {
    PyErr_SetString(PyExc_IndexError, "invalid value for index");
    return NULL;
  }
  return Py_BuildValue("i",cannonY(id));
}
static PyObject* py_cannonBlockX(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int id;
  if (!PyArg_ParseTuple(args, "i", &id)) {
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (cannonIdCheck(id) == 1) {
    PyErr_SetString(PyExc_IndexError, "invalid value for index");
    return NULL;
  }
  return Py_BuildValue("i",cannonBlockX(id));
}
static PyObject* py_cannonBlockY(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int id;
  if (!PyArg_ParseTuple(args, "i", &id)) {
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (cannonIdCheck(id) == 1) {
    PyErr_SetString(PyExc_IndexError, "invalid value for index");
    return NULL;
  }
  return Py_BuildValue("i",cannonBlockY(id));
}
static PyObject* py_cannonAlive(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int id;
  if (!PyArg_ParseTuple(args, "i", &id)) {
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (cannonIdCheck(id) == 1) {
    PyErr_SetString(PyExc_IndexError, "invalid value for index");
    return NULL;
  }
  return Py_BuildValue("i",cannonAlive(id));
}
static PyObject* py_cannonTeam(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int id;
  if (!PyArg_ParseTuple(args, "i", &id)) {
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (cannonIdCheck(id) == 1) {
    PyErr_SetString(PyExc_IndexError, "invalid value for index");
    return NULL;
  }
  if (cannonIdCheck(id) == 2) {
    PyErr_SetString(PyExc_IndexError, "team play not enabled");
    return NULL;
  }
  return Py_BuildValue("i",cannonTeam(id));
}
static PyObject* py_targetCountServer(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  return Py_BuildValue("i",targetCountServer());
}
static PyObject* py_targetX(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int id;
  if (!PyArg_ParseTuple(args, "i", &id)) {
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (targetIdCheck(id) == 1) {
    PyErr_SetString(PyExc_IndexError, "invalid value for index");
    return NULL;
  }
  return Py_BuildValue("i",targetX(id));
}
static PyObject* py_targetY(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int id;
  if (!PyArg_ParseTuple(args, "i", &id)) {
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (targetIdCheck(id) == 1) {
    PyErr_SetString(PyExc_IndexError, "invalid value for index");
    return NULL;
  }
  return Py_BuildValue("i",targetY(id));
}
static PyObject* py_targetBlockX(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int id;
  if (!PyArg_ParseTuple(args, "i", &id)) {
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (targetIdCheck(id) == 1) {
    PyErr_SetString(PyExc_IndexError, "invalid value for index");
    return NULL;
  }
  return Py_BuildValue("i",targetBlockX(id));
}
static PyObject* py_targetBlockY(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int id;
  if (!PyArg_ParseTuple(args, "i", &id)) {
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (targetIdCheck(id) == 1) {
    PyErr_SetString(PyExc_IndexError, "invalid value for index");
    return NULL;
  }
  return Py_BuildValue("i",targetBlockY(id));
}
static PyObject* py_targetDamage(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int id;
  if (!PyArg_ParseTuple(args, "i", &id)) {
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (targetIdCheck(id) == 1) {
    PyErr_SetString(PyExc_IndexError, "invalid value for index");
    return NULL;
  }
  return Py_BuildValue("i",targetDamage(id));
}
static PyObject* py_targetAlive(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int id;
  if (!PyArg_ParseTuple(args, "i", &id)) {
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (targetIdCheck(id) == 1) {
    PyErr_SetString(PyExc_IndexError, "invalid value for index");
    return NULL;
  }
  return Py_BuildValue("i",targetAlive(id));
}
static PyObject* py_targetTeam(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int id;
  if (!PyArg_ParseTuple(args, "i", &id)) {
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (targetIdCheck(id) == 1) {
    PyErr_SetString(PyExc_IndexError, "invalid value for index");
    return NULL;
  }
  if (targetIdCheck(id) == 2) {
    PyErr_SetString(PyExc_IndexError, "team play not enabled");
    return NULL;
  }
  return Py_BuildValue("i",targetTeam(id));
}
static PyObject* py_baseCountServer(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  return Py_BuildValue("i",baseCountServer());
}
static PyObject* py_baseX(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int id;
  if (!PyArg_ParseTuple(args, "i", &id)) {
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (baseIdCheck(id) == 1) {
    PyErr_SetString(PyExc_IndexError, "invalid value for index");
    return NULL;
  }
  return Py_BuildValue("i",baseX(id));
}
static PyObject* py_baseY(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int id;
  if (!PyArg_ParseTuple(args, "i", &id)) {
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (baseIdCheck(id) == 1) {
    PyErr_SetString(PyExc_IndexError, "invalid value for index");
    return NULL;
  }
  return Py_BuildValue("i",baseY(id));
}
static PyObject* py_baseBlockX(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int id;
  if (!PyArg_ParseTuple(args, "i", &id)) {
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (baseIdCheck(id) == 1) {
    PyErr_SetString(PyExc_IndexError, "invalid value for index");
    return NULL;
  }
  return Py_BuildValue("i",baseBlockX(id));
}
static PyObject* py_baseBlockY(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int id;
  if (!PyArg_ParseTuple(args, "i", &id)) {
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (baseIdCheck(id) == 1) {
    PyErr_SetString(PyExc_IndexError, "invalid value for index");
    return NULL;
  }
  return Py_BuildValue("i",baseBlockY(id));
}
static PyObject* py_baseId(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int id;
  if (!PyArg_ParseTuple(args, "i", &id)) {
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (baseIdCheck(id) == 1) {
    PyErr_SetString(PyExc_IndexError, "invalid value for index");
    return NULL;
  }
  return Py_BuildValue("i",baseId(id));
}
static PyObject* py_baseTeam(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int id;
  if (!PyArg_ParseTuple(args, "i", &id)) {
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  if (baseIdCheck(id) == 1) {
    PyErr_SetString(PyExc_IndexError, "invalid value for index");
    return NULL;
  }
  if (baseIdCheck(id) == 2) {
    PyErr_SetString(PyExc_IndexError, "team play not enabled");
    return NULL;
  }
  return Py_BuildValue("i",baseTeam(id));
}
static PyObject* py_itemCountScreen(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  return Py_BuildValue("i",itemCountScreen());
}
static PyObject* py_itemX(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int idx, check;
  if (!PyArg_ParseTuple(args, "i", &idx)){
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  check = itemIdCheck(idx);
  if (check == 1) {
    PyErr_SetString(PyExc_IndexError, "No item with that id");
    return NULL;
  }
  return Py_BuildValue("i",itemX(idx));
}
static PyObject* py_itemY(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int idx, check;
  if (!PyArg_ParseTuple(args, "i", &idx)){
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  check = itemIdCheck(idx);
  if (check == 1) {
    PyErr_SetString(PyExc_IndexError, "No item with that id");
    return NULL;
  }
  return Py_BuildValue("i",itemY(idx));
}
static PyObject* py_itemType(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int idx, check;
  if (!PyArg_ParseTuple(args, "i", &idx)){
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  check = itemIdCheck(idx);
  if (check == 1) {
    PyErr_SetString(PyExc_IndexError, "No item with that id");
    return NULL;
  }
  return Py_BuildValue("i",itemType(idx));
}
static PyObject* py_itemRandom(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int idx, check;
  if (!PyArg_ParseTuple(args, "i", &idx)){
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  check = itemIdCheck(idx);
  if (check == 1) {
    PyErr_SetString(PyExc_IndexError, "No item with that id");
    return NULL;
  }
  return Py_BuildValue("i",itemRandom(idx));
}
static PyObject* py_itemVelX(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int idx, check;
  if (!PyArg_ParseTuple(args, "i", &idx)){
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  check = itemIdCheck(idx);
  if (check == 1) {
    PyErr_SetString(PyExc_IndexError, "No item with that id");
    return NULL;
  }
  return Py_BuildValue("d",itemVelX(idx));
}
static PyObject* py_itemVelY(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int idx, check;
  if (!PyArg_ParseTuple(args, "i", &idx)){
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  check = itemIdCheck(idx);
  if (check == 1) {
    PyErr_SetString(PyExc_IndexError, "No item with that id");
    return NULL;
  }
  return Py_BuildValue("d",itemVelY(idx));
}
static PyObject* py_itemDist(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int idx, check;
  if (!PyArg_ParseTuple(args, "i", &idx)){
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  check = itemIdCheck(idx);
  if (check == 1) {
    PyErr_SetString(PyExc_IndexError, "No item with that id");
    return NULL;
  }
  return Py_BuildValue("d",itemDist(idx));
}
static PyObject* py_itemSpeed(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int idx, check;
  if (!PyArg_ParseTuple(args, "i", &idx)){
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  check = itemIdCheck(idx);
  if (check == 1) {
    PyErr_SetString(PyExc_IndexError, "No item with that id");
    return NULL;
  }
  return Py_BuildValue("d",itemSpeed(idx));
}
static PyObject* py_itemTrackingRad(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int idx, check;
  if (!PyArg_ParseTuple(args, "i", &idx)){
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  check = itemIdCheck(idx);
  if (check == 1) {
    PyErr_SetString(PyExc_IndexError, "No item with that id");
    return NULL;
  }
  return Py_BuildValue("d",itemTrackingRad(idx));
}
static PyObject* py_itemTrackingDeg(PyObject* pySelf, PyObject* args) {
  only_available_if_connected
  int idx, check;
  if (!PyArg_ParseTuple(args, "i", &idx)){
    PyErr_SetString(PyExc_TypeError, "invalid parameter");
    return NULL;
  }
  check = itemIdCheck(idx);
  if (check == 1) {
    PyErr_SetString(PyExc_IndexError, "No item with that id");
    return NULL;
  }
  return Py_BuildValue("d",itemTrackingDeg(idx));
}
static PyObject* py_version(PyObject* pySelf, PyObject* args) {
  return Py_BuildValue("s",getAiVersion());
}
//THE L00PZ -EGG
static PyObject* py_switchLoop(PyObject* pySelf, PyObject* args) {
  PyObject *temp;
  if (PyArg_ParseTuple(args, "O:set_callback", &temp)) {
    if (!PyCallable_Check(temp)) {
      PyErr_SetString(PyExc_TypeError, "parameter must be callable");
      return NULL;
    }
    Py_XINCREF(temp);         /* Add a reference to new callback */
    Py_XDECREF(py_loop);  /* Dispose of previous callback */
    py_loop = temp;       /* Remember new callback */
  }
  Py_RETURN_NONE;
}
void AI_loop() {
  if (PyCallable_Check(py_loop)) PyObject_CallObject(py_loop,NULL);
  if(PyErr_Occurred() != NULL) {
    printf("An exception occurred in your Python call-back function.\n");
    printf("You may wish to catch and print exceptions for more information.\n");
    printf("Exiting pyAI.\n");
    exit(1);
  }
}

//END L00PZ -EGG
static PyObject* py_headlessMode(PyObject* pySelf, PyObject* args) {
  headlessMode();
  Py_RETURN_NONE;
}
//Inject our loop -EGG
void injectAI() { 
  commonInject();
  AI_loop();
}
//END inject -EGG
//Oh glorious py_main(), with the regular main(), you just start the Python shell. -.-; -EGG
static PyObject* py_start(PyObject* pySelf, PyObject* args) {
  //Parse python arguments
  int i;
  PyObject* listObj = PyList_New(0);
  PyObject* strObj;
  PyObject* temp;
  if (!PyArg_ParseTuple(args, "O|O", &temp, &listObj))
    return Py_BuildValue("s","INVALID ARGUMENTS!\n");
  int argc = PyList_Size(listObj)+1;
  char* argv[argc];
  argv[0] = "xpilot-ng-x11";
  for (i=0; i<argc-1; i++) {
    strObj = PyList_GetItem(listObj, i);
    strObj = PyUnicode_AsEncodedString(strObj,"utf-8","strict");
    argv[i+1] = PyBytes_AS_STRING(strObj);
  }
  //Set AI loop
  args = Py_BuildValue("(O)",temp);
  py_switchLoop(pySelf, args);
  return Py_BuildValue("i",commonStart(argc,argv, &injectAI));
}
//Python/C method definitions -EGG
static PyMethodDef libpyAI_methods[] = {
    {"start",py_start,METH_VARARGS,"Initialize AI interface and start XPilot"},
    {"switchLoop",py_switchLoop,METH_VARARGS,"Switch the method that AI_loop calls"},
    {"headlessMode", py_headlessMode,METH_NOARGS, "Allows XPilot client to run without opening a window"}, //-CJG
    {"version", py_version,METH_NOARGS, "returns the version of the xpilot-AI"},

    {"getLag",py_getLag,METH_NOARGS,""},
    //Movement methods -JRA
    {"turnLeft",py_turnLeft,METH_NOARGS,"Turns left"},
    {"turnRight",py_turnRight,METH_NOARGS,"Turns right"},
    {"turnXdeg",py_turnXdeg,METH_VARARGS,"Turns the specified number of xpilot-degrees"}, //-hatten
    {"turnToXdeg",py_turnToXdeg,METH_VARARGS,"Turns the ship to the specified xpilot-degree"}, //-hatten
    {"turnDeg",py_turnDeg,METH_VARARGS,"Turns the specified number of degrees"}, //-hatten
    {"turnToDeg",py_turnToDeg,METH_VARARGS,"Turns the ship to the specified Degree"}, //-hatten
    {"turnRad",py_turnRad,METH_VARARGS,"Turns the specified number of radians"}, //-hatten
    {"turnToRad",py_turnToRad,METH_VARARGS,"Turns the ship to the specified radian"}, //-hatten

    {"setMaxTurnXdeg",py_setMaxTurnXdeg,METH_VARARGS,"Sets the maximum amount of xpilot degrees the ship is allowed to turn"},
    {"setMaxTurnDeg",py_setMaxTurnDeg,METH_VARARGS,"Sets the maximum amount of degrees the ship is allowed to turn"},
    {"setMaxTurnRad",py_setMaxTurnRad,METH_VARARGS,"Sets the maximum amount of radians the ship is allowed to turn"},
    {"getMaxTurnXdeg",py_getMaxTurnXdeg,METH_NOARGS,"Returns the maximum amount of xpilot degrees the ship is allowed to turn"},
    {"getMaxTurnDeg",py_getMaxTurnDeg,METH_NOARGS,"Returns the maximum amount of degrees the ship is allowed to turn"},
    {"getMaxTurnRad",py_getMaxTurnRad,METH_NOARGS,"Returns the maximum amount of radians the ship is allowed to turn"},

    {"setTurnSpeed",py_setTurnSpeed,METH_VARARGS,"Sets the speed the ship will turn by, the minimum power level is 4.0 and the maximum power is 64.0"},
    {"getTurnSpeed",py_getTurnSpeed,METH_NOARGS,"Returns the ship's Turn Speed"}, //-hatten
    {"setTurnResistance",py_setTurnResistance,METH_VARARGS,"Sets the ship's turn resistance. Minimum is 0 and max 1."}, //-hatten
    {"getTurnResistance",py_getTurnResistance,METH_NOARGS,"Returns the ship's turn resistance"}, //-hatten

    {"thrust",py_thrust,METH_NOARGS,"Thrust the ship"}, //No longer needs a flag -hatten
    {"setPower",py_setPower,METH_VARARGS,"Sets the speed the ship will thrust by, the minimum power level is 5.0 and the maximum power is 55.0"},
    {"getPower",py_getPower,METH_NOARGS,"Returns the ship's power"}, //-hatten
    //Shooting methods -JRA
    //Following commands have not been tested, due to lack of items on map -JRA Most of them tested -hatten
    {"fireShot",py_fireShot,METH_NOARGS,"Fires a Shot"},
    {"fireMissile",py_fireMissile,METH_NOARGS,"Fires a Missile"},
    {"fireTorpedo",py_fireTorpedo,METH_NOARGS,"Fires a Torpedo"},
    {"fireHeat",py_fireHeat,METH_NOARGS,"Fires a Heat Seeking Missile"},
    {"dropMine",py_dropMine,METH_NOARGS,"Drops a Stationary Mine from the ship"},
    {"detachMine",py_detachMine,METH_NOARGS,"Releases a Mine from the ship"},
    {"detonateMines",py_detonateMines,METH_NOARGS,"Detonates released Mines"},
    {"fireLaser",py_fireLaser,METH_NOARGS,"Fires a Laser"},
    //Item usage methods -JRA
    {"tankDetach",py_tankDetach,METH_NOARGS,"Detaches a fuel tank from the ship"},
    {"cloak",py_cloak,METH_NOARGS,"Cloaks the ship"},
    {"ecm",py_ecm,METH_NOARGS,"Launches an ECM to temporarily blind opponents"},
    {"transporter",py_transporter,METH_NOARGS,"Uses the transporter item to steal an opponent's item or fuel"},
    {"tractorBeam",py_tractorBeam,METH_NOARGS,"Uses the ship's Tractor Beam to pull in enemy ships"},
    {"pressorBeam",py_pressorBeam,METH_NOARGS,"Uses the ship's Pressor Beam to push away enemy ships"},
    {"phasing",py_phasing,METH_NOARGS,"Uses the Phasing item to allow the ship to pass through walls"},
    {"shield",py_shield,METH_NOARGS,"Turns on or off the ship's Shield"},
    {"emergencyShield",py_emergencyShield,METH_NOARGS,"Uses the Emergency Shield item to protect your ship from damage for a period of time"},
    {"hyperjump",py_hyperjump,METH_NOARGS,"Uses the Hyper Jump item to warp the ship to a random location"},
    {"nextTank",py_nextTank,METH_NOARGS,"Switches to the ship's next fuel tank"},
    {"prevTank",py_prevTank,METH_NOARGS,"Switches to the ship's previous fuel tank"},
    {"toggleAutopilot",py_toggleAutopilot,METH_NOARGS,"Uses the Autopilot item to stop the ship's movement"},
    {"emergencyThrust",py_emergencyThrust,METH_NOARGS,"Uses the Emergency Thrust item to increase the ship's movement speed for a period of time"},
    {"deflector",py_deflector,METH_NOARGS,"Uses the deflector item to push everything away from the ship"},
    {"selectItem",py_selectItem,METH_NOARGS,"Selects the ships item to be dropped"},
    {"loseItem",py_loseItem,METH_NOARGS,"Drops the ships selected item"},
    //Lock methods -JRA
    {"lockNext",py_lockNext,METH_NOARGS,"Locks onto the next ship in the ship buffer"},
    {"lockPrev",py_lockPrev,METH_NOARGS,"Locks onto the prev ship in the ship buffer"},
    {"lockClose",py_lockClose,METH_NOARGS,"Locks onto the closest ship"},
    {"lockNextClose",py_lockNextClose,METH_NOARGS,"Locks-on to the next closest ship"},
    {"loadLock",py_loadLock,METH_VARARGS,"Load a saved lock-on enemy ship"},
    {"saveLock",py_saveLock,METH_VARARGS,"save the current lock-on at the specified index"},
    {"getLockId",py_getLockId,METH_NOARGS,"Returns the ID of the locked-on ship"},
    //Modifier methods -JRA
    {"toggleNuclear",py_toggleNuclear,METH_NOARGS,"Toggles the option to have the ship fire Nuclear weapons instead of regualar weapons, takes up five mines or seven missile"},
    {"togglePower",py_togglePower,METH_NOARGS,"Toggles the Power of the weapon"},
    {"toggleVelocity",py_toggleVelocity,METH_NOARGS,"Modifies explosion velocity of mines and missiles"},
    {"toggleCluster",py_toggleCluster,METH_NOARGS,"Toggles the option to have the ship fire Cluster weapons instead of regular weapons"},
    {"toggleMini",py_toggleMini,METH_NOARGS,"Modifies explosion velocity of mines and missiles"},
    {"toggleSpread",py_toggleSpread,METH_NOARGS,"Toggles the option to have the ship fire Spread weapons instead of regular weapons"},
    {"toggleLaser",py_toggleLaser,METH_NOARGS,"Toggles between the LS stun laser and the LB blinding laser"},
    {"toggleImplosion",py_toggleImplosion,METH_NOARGS,"Toggle the option to have mines and missiles implode instead of exlode, the explosion will draw in players instead of blowing them away"},
    {"toggleUserName",py_toggleUserName,METH_NOARGS,"Toggle the option to have mines and missiles implode instead of exlode, the explosion will draw in players instead of blowing them away"},
    {"loadModifiers",py_loadModifiers,METH_VARARGS,"Loads Modifiers at the specified index"},
    {"saveModifiers",py_saveModifiers,METH_VARARGS,"Saves Modifiers at the specified index"},
    {"clearModifiers",py_clearModifiers,METH_NOARGS,"Clears Modifiers"},
    //map features -JRA
    {"connector",py_connector,METH_NOARGS,"Connects the ship to the ball in Capture the Flag Mode"},
    {"dropBall",py_dropBall,METH_NOARGS,"Drops the ball in Capture the Flag Mode"},
    {"refuel",py_refuel,METH_NOARGS,"Refuels the ship"},
    //other options -JRA
    {"keyHome",py_keyHome,METH_NOARGS,"Changes the ship's Home Base or respawn location"},
    {"selfDestruct",py_selfDestruct,METH_NOARGS,"Triggers the ship's Self Destruct mechanism"}, //Do not repeatedly press or the ship will not self destruct, it works as a toggle and has a timer -JRA
    {"pauseAI",py_pauseAI,METH_NOARGS,"Pauses the game for the ship, does not affect other ships"},
    {"swapSettings",py_swapSettings,METH_NOARGS,"Swaps between ship Settings for turn rate and thrusting power"},
    {"quitAI",py_quitAI,METH_NOARGS,"Quits the game"}, //Do not have toggleNuclear in the same code segment or else it will not quit -JRA TODO? -hatten
    {"talkKey",py_talkKey,METH_NOARGS,"Opens up the chat window"},
    {"toggleShowMessage",py_toggleShowMessage,METH_NOARGS,"Toggles Messages on the HUD on the left side of the screen"}, 
    {"toggleShowItems",py_toggleShowItems,METH_NOARGS,"Toggles Items on the HUD on the left side of the screen"}, 
    {"toggleCompass",py_toggleCompass,METH_NOARGS,"Toggles the ship's Compass"}, 
    {"repair",py_repair,METH_NOARGS,"Repairs a target"},
    {"reprogram",py_reprogram,METH_NOARGS,"Reprogram a modifier or lock bank"},
    {"talk",py_talk,METH_VARARGS,"Sends a message"},
    {"setMaxMsgs",py_setMaxMsgs,METH_VARARGS,"Sets the maximum messages to be displayed"}, //-hatten
    {"getMaxMsgs",py_getMaxMsgs,METH_NOARGS,"Returns the maximum messages to be displayed"}, //-hatten
    {"scanTalkMsg",py_scanTalkMsg,METH_VARARGS,"Returns the specified message"}, //-EGG //renamed -hatten
    {"removeTalkMsg",py_removeTalkMsg,METH_VARARGS,"Removes the specified message"}, //-hatten
    {"scanGameMsg",py_scanGameMsg,METH_VARARGS,"Returns message at specified index from HUD game feed."}, //-CJG //renamed -hatten
    {"removeGameMsg",py_removeGameMsg,METH_VARARGS,"Removes the specified message"}, //-hatten
    //self properties -JRA
    {"selfX",py_selfX,METH_NOARGS,"Returns the ship's X Position"},
    {"selfY",py_selfY,METH_NOARGS,"Returns the ship's Y Position"},
    {"selfVelX",py_selfVelX,METH_NOARGS,"Returns the ship's X Velocity"},
    {"selfVelY",py_selfVelY,METH_NOARGS,"Returns the ship's Y Velocity"},
    {"selfSpeed",py_selfSpeed,METH_NOARGS,"Returns the ship's speed"},
    {"lockHeadingXdeg",py_lockHeadingXdeg,METH_NOARGS,"Returns in xpilot degrees the direction of the ship's Lock-on of an enemy"}, //-hatten
    {"lockHeadingDeg",py_lockHeadingDeg,METH_NOARGS,"Returns in degrees the direction of the ship's Lock-on of an enemy"},
    {"lockHeadingRad",py_lockHeadingRad,METH_NOARGS,"Returns in radians the direction of the ship's Lock-on of an enemy"},
    {"selfLockDist",py_selfLockDist,METH_NOARGS,"Returns the Distance of the enemy that the ship has Locked-on to"},
    {"selfReload",py_selfReload,METH_NOARGS,"Returns the player's Reload time remaining, based on a call to fireShot()"},
    {"selfId",py_selfId,METH_NOARGS,"Returns the ID of the ship"},
    {"selfAlive",py_selfAlive,METH_NOARGS,"Returns if the ship is Dead or Alive"},
    {"selfTeam",py_selfTeam,METH_NOARGS,"Returns the ship's Team"},
    {"selfLives",py_selfLives,METH_NOARGS,"Returns how many Lives are left for the ship"},
    {"selfTrackingRad",py_selfTrackingRad,METH_NOARGS,"Returns the ship's Tracking in Radians"},
    {"selfTrackingDeg",py_selfTrackingDeg,METH_NOARGS,"Returns the ship's Tracking in Degrees"},
    {"selfHeadingDeg",py_selfHeadingDeg,METH_NOARGS,"Returns the heading of the ship in Degrees"},
    {"selfHeadingRad",py_selfHeadingRad,METH_NOARGS,"Returns the heading of the ship in Radians"},
    {"selfHeadingXdeg",py_selfHeadingXdeg,METH_NOARGS,"Returns the heading of the ship in xpilot-degrees"}, //-hatten
    {"hud",py_hud,METH_VARARGS,"Returns the Name on the HUD"}, // -CJG
    {"hudScore",py_hudScore,METH_VARARGS,"Returns the Score on the HUD"},// -CJG
    {"hudTimeLeft",py_hudTimeLeft,METH_VARARGS,"Returns the Time Left on the HUD"},// -CJG
    {"selfShield",py_selfShield,METH_NOARGS,"Returns the ship's Shield status"},
    {"selfName",py_selfName,METH_NOARGS,"Returns the ship's Name"},
    {"selfScore",py_selfScore,METH_NOARGS,"Returns the ship's Score"},
    {"selfItem",py_selfItem,METH_VARARGS,"Returns the number of items of the specified type"}, //-hatten
    {"selfFuel",py_selfFuel,METH_NOARGS,"Returns the sum of fuel in all tanks"},
    {"selfFuelMax",py_selfFuelMax,METH_NOARGS,"Returns the max amount of fuel the ship can hold (depends on number of tanks"},
    {"selfFuelCurrent",py_selfFuelCurrent,METH_NOARGS,"Returns the currently selected tank"},
    {"selfMass",py_selfMass,METH_NOARGS,"Returns the ship's mass (depends on mass and fuel)"},

    //radar functions -hatten
    {"selfRadarX", py_selfRadarX,METH_NOARGS,"Returns the X radar coordinate of the ship"},
    {"selfRadarY", py_selfRadarY,METH_NOARGS,"Returns the Y radar coordinate of the ship"},
    {"radarX", py_radarX,METH_VARARGS,"Returns the X radar coordinate of the specified ship"},
    {"radarY", py_radarY,METH_VARARGS,"Returns the Y radar coordinate of the specified ship"},
    {"radarVelX", py_radarVelX,METH_VARARGS,"Returns the X velocity of the specified ship"},
    {"radarVelY", py_radarVelY,METH_VARARGS,"Returns the Y velocity of the specified ship"},
    {"radarType", py_radarType,METH_VARARGS,"Returns the type of the specified radar object"},
    {"radarDist", py_radarDist,METH_VARARGS,"Returns the distance to the speciied radar object"},
    {"closestRadarId",py_closestRadarId,METH_NOARGS,"Returns the closest ship's radarId"},
    {"radarCount",py_radarCount,METH_NOARGS,"Returns the number of ships on the radar"},
    {"radarHeight",py_radarHeight,METH_NOARGS,"Returns the height of the radar"},
    {"radarWidth",py_radarWidth,METH_NOARGS,"Returns 256, the width of the radar"},

    //Closest functions -JRA

    {"shipCountScreen",py_shipCountScreen,METH_NOARGS,"Returns the number of ships on the screen"},
    //{"closestShipIdx",py_closestShipIdx,METH_NOARGS,"Returns the Closest ship's IDX"}, //worthless as they are sorted according to distance, though i see no reason for that
    {"shipId",py_shipId,METH_VARARGS,"Returns the Specified Enemy's Id"},
    //idx functions -JRA
    {"shipDistance",py_shipDistance,METH_VARARGS,"Returns the Distance between the ship and the Specified Enemy"},
    {"shipSpeed",py_shipSpeed,METH_VARARGS,"Returns the Speed of the Specified Enemy"},
    {"shipVelX",py_shipVelX,METH_VARARGS,"Returns the x velocity of the Specified Enemy"},
    {"shipVelY",py_shipVelY,METH_VARARGS,"Returns the y velocity of the Specified Enemy"},
    //{"shipReload",py_enemyReload,METH_VARARGS,"Returns the Specified Enemy's Reload time remaining"},
    {"shipTrackingRad",py_shipTrackingRad,METH_VARARGS,"Returns the Specified Enemy's Tracking in Radians"},
    {"shipTrackingDeg",py_shipTrackingDeg,METH_VARARGS,"Returns the Specified Enemy's Tracking in Degrees"},
    {"shipX",py_shipX,METH_VARARGS,"Returns the Specified Enemy's X Coordinate"},
    {"shipY",py_shipY,METH_VARARGS,"Returns the Specified Enemy's Y Coordinate"},
    {"shipHeadingXdeg",py_shipHeadingXdeg,METH_VARARGS,"Returns the Heading of the Specified Enemy from the ship in xdegrees"},
    {"shipHeadingDeg",py_shipHeadingDeg,METH_VARARGS,"Returns the Heading of the Specified Enemy from the ship in Degrees"},
    {"shipHeadingRad",py_shipHeadingRad,METH_VARARGS,"Returns the Heading of the Specified Enemy from the ship in Radians"},
    {"shipShield",py_shipShield,METH_VARARGS,"Returns the Specified Enemy's Shield Status"},
    {"playerCountServer",py_playerCountServer,METH_NOARGS,"Returns number of ships on the server, includes paused players and tanks."},
    {"pausedCountServer",py_pausedCountServer,METH_NOARGS,"Returns number of paused players on the server."},
    {"tankCountServer",py_tankCountServer,METH_NOARGS,"Returns number of tanks on the server."},
    {"playerId",py_playerId,METH_VARARGS,"Returns the Specified Enemy's Id"},
    {"playerLives",py_playerLives,METH_VARARGS,"Returns the Specified Enemy's Remaining Lives"},
    {"playerTeam",py_playerTeam,METH_VARARGS,"Returns the Specified Enemy's Team"},
    {"playerName",py_playerName,METH_VARARGS,"Returns the Specified Enemy's Name"},
    {"playerScore",py_playerScore,METH_VARARGS,"Returns the Specified Enemy's Score"},

    {"xdegToDeg",py_xdegToDeg,METH_VARARGS,"Converts xpilot degrees to degrees"},
    {"xdegToRad",py_xdegToRad,METH_VARARGS,"Converts xpilot degrees to radians"},
    {"degToXdeg",py_degToXdeg,METH_VARARGS,"Converts degrees to xpilot degrees"},
    {"degToRad",py_degToRad,METH_VARARGS,"Converts degrees to radians"},
    {"radToXdeg",py_radToXdeg,METH_VARARGS,"Converts radians to xpilot degrees"},
    {"radToDeg",py_radToDeg,METH_VARARGS,"Converts radians to degrees"},

    {"wallFeelerDeg",py_wallFeelerDeg,METH_VARARGS,"Returns if there is a wall or not at the Specified Angle (degrees) within the Specified Distance of the ship"},
    {"wallFeelerRad",py_wallFeelerRad,METH_VARARGS,"Returns if there is a wall or not at the Specified Angle (radians) within the Specified Distance of the ship"},
    {"wallBetween",py_wallBetween,METH_VARARGS,"Returns if there is a wall or not between two Specified Points"},
    {"blockSize",py_blockSize,METH_NOARGS,"Returns the size (in pixels) of a block"},
    {"mapWidthBlocks",py_mapWidthBlocks,METH_NOARGS,"Returns the width of the map, in blocks"},
    {"mapHeightBlocks",py_mapHeightBlocks,METH_NOARGS,"Returns the height of the map, in blocks"},
    {"mapWidthPixels",py_mapWidthPixels,METH_NOARGS,"Returns the width of the map, in pixels"},
    {"mapHeightPixels",py_mapHeightPixels,METH_NOARGS,"Returns the height of the map, in pixels"},
    {"mapData",py_mapData,METH_VARARGS,"Returns what type of block a specified coordinate is"},
    {"getOption",py_getOption,METH_VARARGS,"Returns the value of a specified option"},
    //Shot functions -JNE
    {"shotCountScreen",py_shotCountScreen,METH_NOARGS,"Returns the number of shots on the screen"},
    {"shotX",py_shotX,METH_VARARGS,"Returns the X coordinate of a shot"},
    {"shotY",py_shotY,METH_VARARGS,"Returns the Y coordinate of a shot"},
    {"shotDist",py_shotDist,METH_VARARGS,"Returns the Distance of a shot from the ship"},
    {"shotVelX",py_shotVelX,METH_VARARGS,"Returns the X velocity of a shot"},
    {"shotVelY",py_shotVelY,METH_VARARGS,"Returns the Y velocity of a shot"},
    {"shotFriendly",py_shotFriendly,METH_VARARGS,"Returns whether a velocity is friendly"},
    {"shotSpeed",py_shotSpeed,METH_VARARGS,"Returns the Velocity of a shot"},
    {"shotTrackingDeg",py_shotTrackingDeg,METH_VARARGS,"Returns the direction (degrees) of the Velocity of a shot"},
    {"shotTrackingRad",py_shotTrackingRad,METH_VARARGS,"Returns the direction (radians) of the Velocity of a shot"},
    //Asteroid tracking functions -hatten
    {"asteroidCountScreen",py_asteroidCountScreen,METH_NOARGS,"Returns the number of asteroids on the screen"},
    {"asteroidX",py_asteroidX,METH_VARARGS,"Returns the x coordinate of the specified asteroid"},
    {"asteroidY",py_asteroidY,METH_VARARGS,"Returns the y coordinate of the specified asteroid"},
    {"asteroidType",py_asteroidType,METH_VARARGS,"Returns the type of the specified asteroid"},
    {"asteroidSize",py_asteroidSize,METH_VARARGS,"Returns the size of the specified asteroid"},
    {"asteroidRotation",py_asteroidRotation,METH_VARARGS,"Returns the rotation of the specified asteroid"},
    {"asteroidDist",py_asteroidDist,METH_VARARGS,"Returns the distance of an asteroid from the ship"},
    {"asteroidVelX",py_asteroidVelX,METH_VARARGS,"Returns the X velocity of a asteroid"},
    {"asteroidVelY",py_asteroidVelY,METH_VARARGS,"Returns the Y velocity of a asteroid"},
    {"asteroidSpeed",py_asteroidSpeed,METH_VARARGS,"Returns the Velocity of a asteroid"},
    {"asteroidTrackingDeg",py_asteroidTrackingDeg,METH_VARARGS,"Returns the direction (degrees) of the Velocity of a asteroid"},
    {"asteroidTrackingRad",py_asteroidTrackingRad,METH_VARARGS,"Returns the direction (radians) of the Velocity of a asteroid"},
    //Item tracking functions -hatten
    {"itemCountScreen",py_itemCountScreen,METH_NOARGS,"Returns the number of items on the screen"},
    {"itemX",py_itemX,METH_VARARGS,"Returns the X screen coordinate of the specified item"},
    {"itemY",py_itemY,METH_VARARGS,"Returns the Y screen coordinate of the specified item"},
    {"itemType",py_itemType,METH_VARARGS,"Returns the type of the specified item"},
    {"itemRandom",py_itemRandom,METH_VARARGS,"Returns whether the item is 'random'"},
    {"itemDist",py_itemDist,METH_VARARGS,"Returns the distance of an item from the ship"},
    {"itemVelX",py_itemVelX,METH_VARARGS,"Returns the X velocity of an item"},
    {"itemVelY",py_itemVelY,METH_VARARGS,"Returns the Y velocity of an item"},
    {"itemSpeed",py_itemSpeed,METH_VARARGS,"Returns the Velocity of an item"},
    {"itemTrackingDeg",py_itemTrackingDeg,METH_VARARGS,"Returns the direction (degrees) of the Velocity of an item"},
    {"itemTrackingRad",py_itemTrackingRad,METH_VARARGS,"Returns the direction (radians) of the Velocity of an item"},
    //beta functions~~~
    {"phasingTime",py_phasingTime,METH_NOARGS,"Returns the time left on the current phasing item."}, //TODO
    {"nextCheckpoint",py_nextCheckpoint,METH_NOARGS,"Returns the index of the next checkpoint"},
    {"checkpointX",py_checkpointX,METH_VARARGS,"Returns the x coordinate of the specified checkpoint"}, //TODO: Use mapdata to return -1 if the checkpoint doesn't exist.
    {"checkpointY",py_checkpointY,METH_VARARGS,"Returns the y coordinate of the specified checkpoint"},
    {"checkpointBlockX",py_checkpointBlockX,METH_VARARGS,"Returns the x block of the specified checkpoint"},
    {"checkpointBlockY",py_checkpointBlockY,METH_VARARGS,"Returns the y block of the specified checkpoint"},
    {"ballCountScreen",py_ballCountScreen,METH_NOARGS,"Returns the number of balls on the screen"},
    {"ballX",py_ballX,METH_VARARGS,"Returns the x coordinate of the specified ball"},
    {"ballY",py_ballY,METH_VARARGS,"Returns the x coordinate of the specified ball"},
    {"ballId",py_ballId,METH_VARARGS,"Returns the id of the specified ball"},
    {"connectorCountScreen",py_connectorCountScreen,METH_NOARGS,"Returns the number of connectors on the screen"},
    {"connectorX0",py_connectorX0,METH_VARARGS,"Returns the connector's X0 Position"},
    {"connectorX1",py_connectorX1,METH_VARARGS,"Returns the connector's X1 Position"},
    {"connectorY0",py_connectorY0,METH_VARARGS,"Returns the connector's Y0 Position"},
    {"connectorY1",py_connectorY1,METH_VARARGS,"Returns the connector's Y1 Position"},
    {"connectorTractor",py_connectorTractor,METH_VARARGS,"Returns the connector's tractor"},

    {"missileCountScreen",py_missileCountScreen,METH_NOARGS,"Returns the number of missiles on the screen"},
    {"missileX",py_missileX,METH_VARARGS,"Returns the x coordinate of the specified missile"},
    {"missileY",py_missileY,METH_VARARGS,"Returns the y coordinate of the specified missile"},
    {"missileHeadingXdeg",py_missileHeadingXdeg,METH_VARARGS,"Returns the direction of the specified missile"},
    {"missileHeadingRad",py_missileHeadingRad,METH_VARARGS,"Returns the direction of the specified missile"},
    {"missileHeadingDeg",py_missileHeadingDeg,METH_VARARGS,"Returns the direction of the specified missile"},
    {"missileLen",py_missileLen,METH_VARARGS,"Returns the length of the specified missile"},

    {"laserCountScreen",py_laserCountScreen,METH_NOARGS,"Returns the number of lasers on the screen"},
    {"laserX",py_laserX,METH_VARARGS,"Returns the x coordinate of the specified laser"},
    {"laserY",py_laserY,METH_VARARGS,"Returns the y coordinate of the specified laser"},
    {"laserHeadingXdeg",py_laserHeadingXdeg,METH_VARARGS,"Returns the direction of the specified laser"},
    {"laserHeadingDeg",py_laserHeadingDeg,METH_VARARGS,"Returns the direction of the specified laser"},
    {"laserHeadingRad",py_laserHeadingRad,METH_VARARGS,"Returns the direction of the specified laser"},
    {"laserLen",py_laserLen,METH_VARARGS,"Returns the length of the specified laser"}, //TODO: Interesting?

    {"mineCountScreen",py_mineCountScreen,METH_NOARGS,"Returns the number of mines on the screen"},
    {"mineX",py_mineX,METH_VARARGS,"Returns the x coordinate of the specified mine."},
    {"mineY",py_mineY,METH_VARARGS,"Returns the y coordinate of the specified mine."},
    {"mineFriendly",py_mineFriendly,METH_VARARGS,"Returns whether the mine is friendly."},
    {"mineId",py_mineId,METH_VARARGS,"Returns the ID of the player that dropped the mine if close enough."},
    {"wormholeCountScreen",py_wormholeCountScreen,METH_NOARGS,"Returns the number of wormholes on screen"},
    {"wormholeX",py_wormholeX,METH_VARARGS,"Returns the x coordinate of the specified wormhole"},
    {"wormholeY",py_wormholeY,METH_VARARGS,"Returns the y coordinate of the specified wormhole"},
    {"ecmCountScreen",py_ecmCountScreen,METH_NOARGS,"Returns the number of ECM's on the screen"},
    {"ecmX",py_ecmX,METH_VARARGS,"Returns the x coordinate of the specified ECM"},
    {"ecmY",py_ecmY,METH_VARARGS,"Returns the y coordinate of the specified ECM"},
    {"timeLeftSec",py_timeLeftSec,METH_NOARGS,"Returns the time left in seconds, if gameduration is set."},
    {"fuelstationCount",py_fuelstationCount,METH_NOARGS,"Returns the number of fuelstations on the server"}, //TODO: Write wrapper so we only handle the stations on screen -hatten
    {"fuelstationX",py_fuelstationX,METH_VARARGS,"Returns the x coordinate of the specified fuelstation"},
    {"fuelstationY",py_fuelstationY,METH_VARARGS,"Returns the y coordinate of the specified fuelstation"},
    {"fuelstationBlockX",py_fuelstationBlockX,METH_VARARGS,"Returns the x coordinate of the specified fuelstation"},
    {"fuelstationBlockY",py_fuelstationBlockY,METH_VARARGS,"Returns the y coordinate of the specified fuelstation"},
    {"fuelstationFuel",py_fuelstationFuel,METH_VARARGS,"Returns the amount of fuel of the specified fuelstation"}, //updated when on screen, IIRC
    {"fuelstationTeam",py_fuelstationTeam,METH_VARARGS,"Returns which team the fuelstation belongs to"},
    {"cannonCountServer",py_cannonCountServer,METH_NOARGS,"Returns the number of cannons on the server"},
    {"cannonX",py_cannonX,METH_VARARGS,"Returns the x coordinate of the specified cannon"},
    {"cannonY",py_cannonY,METH_VARARGS,"Returns the y coordinate of the specified cannon"},
    {"cannonBlockX",py_cannonBlockX,METH_VARARGS,"Returns the x coordinate of the specified cannon"},
    {"cannonBlockY",py_cannonBlockY,METH_VARARGS,"Returns the y coordinate of the specified cannon"},
    {"cannonAlive",py_cannonAlive,METH_VARARGS,"Returns whether the cannon is dead"},
    {"cannonTeam",py_cannonTeam,METH_VARARGS,"Returns which team the cannon belongs to"},
    {"targetCountServer",py_targetCountServer,METH_NOARGS,"Returns the number of targets on the server"},
    {"targetX",py_targetX,METH_VARARGS,"Returns the x screen coordinate of the center of the specified target"},
    {"targetY",py_targetY,METH_VARARGS,"Returns the y screen coordinate of the center of the specified target"},
    {"targetBlockX",py_targetBlockX,METH_VARARGS,"Returns the x block of the specified target"},
    {"targetBlockY",py_targetBlockY,METH_VARARGS,"Returns the y block of the specified target"},
    {"targetAlive",py_targetAlive,METH_VARARGS,"Returns whether the specified target is dead."},
    {"targetDamage",py_targetDamage,METH_VARARGS,"Returns how much damage the specified target have taken"},
    {"targetTeam",py_targetTeam,METH_VARARGS,"Returns which team the specified target belongs to"},
    {"baseCountServer",py_baseCountServer,METH_NOARGS,"Returns the number of bases"},
    {"baseX",py_baseX,METH_VARARGS,"Returns the x coordinate of the specified base."},
    {"baseY",py_baseY,METH_VARARGS,"Returns the y coordinate of the specified base."},
    {"baseBlockX",py_baseBlockX,METH_VARARGS,"Returns the x coordinate of the specified base."},
    {"baseBlockY",py_baseBlockY,METH_VARARGS,"Returns the y coordinate of the specified base."},
    {"baseTeam",py_baseTeam,METH_VARARGS,"Returns the team of the specified base."},
    {"baseId",py_baseId,METH_VARARGS,"Returns the owner of the specified base"},

    //Server options

    {NULL, NULL,0,NULL}
};
//Python/C module definition -EGG
static struct PyModuleDef libpyAI_module = {
    PyModuleDef_HEAD_INIT,
    "libpyAI",
    NULL,
    -1,
    libpyAI_methods
};
//Python initializer -EGG
PyMODINIT_FUNC PyInit_libpyAI(void) {
    return PyModule_Create(&libpyAI_module);
}
