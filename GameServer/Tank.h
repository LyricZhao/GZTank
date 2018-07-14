#ifndef TANK_H_
#define TANK_H_

#include <ctime>

struct State {
  int life;
  int cannonDisTime;
  int cannonDisWaitingTime;
  int camDisWaitingTime;
  int camDisTime;
  
  State() {}
};

class Tank {
private:
  State m_state;

  time_t m_time;
public:
  Tank();
  void GetShot();
  void DisturbCam();
  void DisturbCannon();
  void UseDisCam();
  void UseDisCannon();

  State GetState() { return m_state; }

  void Step();

  static const int k_maxCannonDisTime = 10;
  static const int k_maxCamDisTime = 10;
  static const int k_maxCannonDisWaitingTime = 10;
  static const int k_maxCamDisWaitingTime = 10;
};


#endif //TANK_H_
