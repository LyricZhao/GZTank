#include "Tank.h"
#include <ctime>
#include <math.h>

Tank::Tank() {
  m_state.life = 10;
  m_state.camDisTime = 0;
  m_state.cannonDisTime = 0;
  m_state.cannonDisWaitingTime = 0;
  m_state.camDisWaitingTime = 0;

  m_time = time(0);
}

void Tank::Step() {
  time_t timeTemp = time(0);
  int dt = timeTemp - m_time;
  m_state.camDisTime = fmax( m_state.camDisTime - dt, 0 );
  m_state.cannonDisTime = fmax( m_state.cannonDisTime - dt, 0 );
  m_state.camDisWaitingTime = fmax(m_state.camDisWaitingTime - dt, 0 );
  m_state.cannonDisWaitingTime = fmax(m_state.cannonDisWaitingTime - dt, 0 );
  m_time = timeTemp;
}

void Tank::GetShot() {
  m_state.life -= 1;
}

void Tank::UseDisCam() {
  m_state.camDisWaitingTime = k_maxCamDisWaitingTime;
}

void Tank::UseDisCannon() {
  m_state.cannonDisWaitingTime = k_maxCannonDisWaitingTime;
}

void Tank::DisturbCam() {
  m_state.camDisTime = k_maxCamDisTime;
}

void Tank::DisturbCannon() {
  m_state.cannonDisTime = k_maxCannonDisTime;
}
