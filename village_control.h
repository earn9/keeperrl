#ifndef _VILLAGE_CONTROL_H
#define _VILLAGE_CONTROL_H

class VillageControl : public EventListener {
  public:
  virtual ~VillageControl();
  void addCreature(Creature* c, int attackTime);

  virtual MoveInfo getMove(Creature* c) = 0;
  bool startedAttack(Creature* c);

  virtual void onKillEvent(const Creature* victim, const Creature* killer) override;

  static VillageControl* humanVillage(const Collective* villain, const Location* villageLocation, 
      StairDirection dir, StairKey key);
  static VillageControl* dwarfVillage(const Collective* villain, const Level*, 
      StairDirection dir, StairKey key);
  protected:
  VillageControl(const Collective* villain, const Level*, StairDirection, StairKey);
  map<const Creature*, int> attackTimes;
  const Collective* villain;
  const Level* level;
  StairDirection direction;
  StairKey stairKey;
};

#endif
