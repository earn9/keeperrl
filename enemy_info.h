#pragma once

#include "settlement_info.h"
#include "villain_type.h"
#include "collective_config.h"
#include "village_behaviour.h"
#include "attack_trigger.h"

struct EnemyInfo;
class EnemyFactory;

struct LevelConnection {
  enum Type {
    CRYPT,
    GNOMISH_MINES,
    TOWER,
    MAZE,
    SOKOBAN,
  };
  Type type;
  HeapAllocated<EnemyInfo> otherEnemy;
  bool deadInhabitants;
};

struct EnemyInfo {
  EnemyInfo(SettlementInfo s, CollectiveConfig c, optional<VillageBehaviour> v = none,
      optional<LevelConnection> = none);
  EnemyInfo& setVillainType(VillainType type);
  EnemyInfo& setId(EnemyId);
  EnemyInfo& setNonDiscoverable();
  EnemyInfo& setCreateOnBones(EnemyFactory&, double prob, vector<EnemyId>);
  SettlementInfo settlement;
  CollectiveConfig config;
  optional<VillageBehaviour> villain;
  optional<VillainType> villainType;
  optional<LevelConnection> levelConnection;
  optional<EnemyId> id;
  bool discoverable = true;
};
