#include "../include/raylib.h"

#include "Gstructs.h"
#include "Timers.h"
#include "Options.h"

void DamageActor(struct Actor *actor)
{
  if (!actor->in) {
    actor->hp--;
    if (!mute) PlaySoundMulti(actor->fxhit);
    actor->in = true;
  }

  actor->currentframe = 1;
}

void UpdateiFrameTimer(struct Actor *actor)
{
  // here we use pointers to avoid duplicating code
  if (actor->in) {
    actor->iframetimer += GetFrameTime();
    actor->currentframe = 1;
    if ((int)globalTimer % 2 == 0) actor->color = GRAY;
    else actor->color = RAYWHITE;
    if (actor->iframetimer > 2) {
      actor->in = false;
      actor->iframetimer = 0;
    }
  } else { actor->color = RAYWHITE; actor->currentframe = 0; }
}

bool CheckAttackActivity(struct Attack attack[], int val, int max)
{
  int matches = 0;
  for (int i = 0; i < max; i++) {
    if (attack[i].active == val) matches++;
  }

  if (matches == max) return true;
  else return false;
}