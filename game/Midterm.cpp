idVec3 positions[] = {
    idVec3(-150.47, -414.45, 161.25),
    idVec3(586.24, 573.51, 260.25),
    idVec3(406.05 , -480.75 , 68.25),
    idVec3(-410.56, 405.62, 35.89),
    idVec3(-281.8, -1134.21, 68.25),
    idVec3(-173.95, -2135.69, 196.25),
    idVec3(-140.94, -3010.88, 260.25),
    idVec3(514.96, -2649.52, 260.25),
    idVec3(1182.8, -2707.5, 260.25),
    idVec3(1223.01, -1819.38, 324.25),
    idVec3(489.13, -1834.33, 388.25),
    idVec3(339.5, -2721.05, 452.25),
    idVec3(680.48, -2349.9, 196.25),
    idVec3(1068.74, -1995.5, 196.25),
    idVec3(766.63, -1105.31, 388.25),
    idVec3(454.77, -1618.98, 388.25),
    idVec3(1560.23, -1674, 324.25),
    idVec3(1494.13, -2754.36, 452.25),
    idVec3(-1068.18, -283.63, 68.25),
    idVec3(-1780.4, -73.05, 132.25),
    idVec3(-2938.66, -182.32, 260.25),
    idVec3(-2711.1, 476.77, 260.25),
    idVec3(-2675.3, 1207.55, 260.25),
    idVec3(-1807.8, 1214.87, 324.25),
    idVec3(-1773.91, 523.73, 388.25),
    idVec3(-2685.05, 309.67, 452.25),
    idVec3(-2026.02, 639.21, 196.25),
    idVec3(-2428.29, 1045.79, 196.25),
    idVec3(-858.44, 592.98, 388.25),
    idVec3(-1504.52, 302, 388.25),
    idVec3(-1612.43, 1025.26, 388.25),
    idVec3(-1696.56, 1611.08, 324.25),
    idVec3(-327.67, 1407.56, 262.32),
    idVec3(-2862.07, 1526.34, 452.25)
};
const char* common_enemies[] = {
    "monster_strogg_marine",
    "monster_scientist",
    "monster_gladiator",
    "monster_grunt",
    "monster_gunner", // turn this guy's damage down
    "monster_sentry",
    "monster_slimy_transfer"
};
const char* rare_enemies[] = {
    "monster_stream_protector",
    "monster_berserker",
    "monster_boss_buddy",
    "monster_iron_maiden",
    "monster_stream_protector",
    "monster_teleport_dropper"
};
const char* boss_enemies[] = {
    "monster_strogg_hover",
    "monster_makron",
    "monster_harvester_combat",
    "monster_network_guardian"
};

#define NUM_POSITIONS (sizeof(positions) / sizeof(idVec3))
#define NUM_COMMON_ENEMIES (sizeof(common_enemies) / sizeof(const char*))
#define NUM_RARE_ENEMIES (sizeof(rare_enemies) / sizeof(const char*))
#define NUM_BOSS_ENEMIES (sizeof(boss_enemies) / sizeof(const char*))

int wave = 0;
int rareProb = 5;
int nextSpawnTime = 0;
int nextHealthRegenTime = 0;
int waveTime = 60000;
int initialWaveTime = 10000;
int stroggHearts = 0;
int chosenClass = 0;
bool pause = false;
bool playerStatsShowing = false;
bool playerStatsShowingHelper = false;
idVec3 bossPosition = idVec3(7.67, 54.04, 608.45);

typedef enum {
    MID_STATS_DAMAGE=0,     
    MID_STATS_SPEED,      
    MID_STATS_MAX_HEALTH, 
    MID_STATS_HEALTH_REGEN,
    MID_STATS_CLIP_SIZE,
    MID_NUM_PLAYER_STATS
};

typedef enum {
    MID_CLASS_GUNNER,
    MID_CLASS_SNIPER,
    MID_CLASS_SCOUT,
    MID_CLASS_ASSASSIN,
    MID_CLASS_TANK,
    MID_NUM_CLASSES
};

bool playerStatsLoaded;
double* playerStats[5];
const char* playerStatsStr[5] = {
    "midterm_damage",
    "midterm_speed",
    "midterm_max_health",
    "midterm_health_regen",
    "midterm_clip_size"
};
const char* playerWeaponStr[5] = {
    "weapon_machinegun",
    "weapon_railgun",
    "weapon_shotgun",
    "weapon_blaster",
    "weapon_nailgun"
};

// damage - percent increase
// speed - percent increase
// max health - fixed
// health regen - hp/s
// mag size - percent increase
double classStats[MID_NUM_CLASSES][MID_NUM_PLAYER_STATS] = {
    {1, 1.5, 200, 2, 0.5},   //Gunner
    {2, 1.2, 125, 1, 0.5},   //Sniper
    {1, 2.5, 150, 2, 0.5},   //Scout
    {100, 2, 50, 1, 1},      //Assassin
    {0.8, 0.7, 500, 5, 3},    //Tank
};

void MidtermSpawn(idVec3 pos)
{
    idDict dict;
    bool rare = rand() % 100 < rareProb;
    if (rare)
        dict.Set("classname", rare_enemies[rand() % NUM_RARE_ENEMIES]);
    else
        dict.Set("classname", common_enemies[rand() % NUM_COMMON_ENEMIES]);
    dict.Set("origin", pos.ToString());
    gameLocal.Printf(pos.ToString());

	idEntity *newEnt = NULL;
	gameLocal.SpawnEntityDef( dict, &newEnt );

	if (newEnt)	{
		gameLocal.Printf("spawned entity '%s'\n", newEnt->name.c_str());
        if (rare)
            newEnt->stroggHearts = 5;
        else
            newEnt->stroggHearts = 1;
	} else {
        gameLocal.Printf("could not spawn '%s'\n", "monster");
    }
}

void MidtermSpawnBoss()
{
    idDict dict;
    dict.Set("classname", boss_enemies[rand() % NUM_BOSS_ENEMIES]);
    dict.Set("origin", bossPosition.ToString());

	idEntity *newEnt = NULL;
	gameLocal.SpawnEntityDef( dict, &newEnt );

	if (newEnt)	{
        newEnt->stroggHearts = 50;
		//gameLocal.Printf("spawned entity '%s'\n", newEnt->name.c_str());
	} else {
        //gameLocal.Printf("could not spawn '%s'\n", "monster");
    }
}

void MidtermInit()
{
    srand(time(NULL));
    wave = 0;
    nextSpawnTime = 0;
    stroggHearts = 0;
    playerStatsShowing = true;
    playerStatsLoaded = false;
}

void MidtermSpawnWave()
{
    if (wave % 10 == 9 && wave != 0) {
        MidtermSpawnBoss();
        waveTime = 90000;
    }
    for (int i = 0; i < wave + 10; i++) {
        MidtermSpawn(positions[rand() % NUM_POSITIONS]);
        waveTime = 60000;
    }
}

void MidtermLoadPlayerStats(idPlayer* player)
{
    player->midtermPlayerClass  =   chosenClass;
    player->midtermDamage       =   classStats[chosenClass][MID_STATS_DAMAGE];
    player->midtermSpeed        =   classStats[chosenClass][MID_STATS_SPEED];
    player->midtermMaxHealth    =   classStats[chosenClass][MID_STATS_MAX_HEALTH];
    player->midtermHealthRegen  =   classStats[chosenClass][MID_STATS_HEALTH_REGEN];
    player->midtermClipSize     =   classStats[chosenClass][MID_STATS_CLIP_SIZE];
    playerStats[MID_STATS_DAMAGE]       = &player->midtermDamage;
    playerStats[MID_STATS_SPEED]        = &player->midtermSpeed;
    playerStats[MID_STATS_MAX_HEALTH]   = &player->midtermMaxHealth;
    playerStats[MID_STATS_HEALTH_REGEN] = &player->midtermHealthRegen;
    playerStats[MID_STATS_CLIP_SIZE]    = &player->midtermClipSize;

    player->inventory.maxHealth = player->midtermMaxHealth;
    player->health = player->inventory.maxHealth;
}

void MidtermPlayerUpdate(idPlayer* player)
{
    for (int i = 0; i < MAX_AMMOTYPES; i++)
        player->inventory.ammo[i] = 2000000000;
    static double carry;
    double a;
    gameLocal.Printf("%d\n", player->inventory.maxHealth);
    if (gameLocal.GetTime() > nextHealthRegenTime) {
        player->health += player->midtermHealthRegen;
        carry += modf(player->midtermHealthRegen, &a);
        if (carry > 1) {
            player->health += 1;
            carry -= 1;
        }
        player->inventory.maxHealth = player->midtermMaxHealth;
        if (player->health > player->inventory.maxHealth)
            player->health = player->inventory.maxHealth;
        nextHealthRegenTime = gameLocal.GetTime() + 1000;
    }
}

void MidtermGivePlayerWeapon(idPlayer* player)
{
    player->GiveItem(playerWeaponStr[chosenClass]);
}

void MidtermUpdate()
{
    if (pause)
        return;

    idPlayer* player = gameLocal.GetLocalPlayer();
    if (player != NULL) {
        if (!playerStatsLoaded) {
            MidtermGivePlayerWeapon(player);
            MidtermLoadPlayerStats(player);
            playerStatsLoaded = true;
        }
        MidtermPlayerUpdate(player);
    }

    if (nextSpawnTime == 0) {
        nextSpawnTime = gameLocal.GetTime() + initialWaveTime;
        nextHealthRegenTime = gameLocal.GetTime();
    }
    if (gameLocal.GetTime() > nextSpawnTime) {
        MidtermSpawnWave();
        nextSpawnTime = gameLocal.GetTime() + waveTime;
        wave++;
    }
}

void MidtermUpdateHUD(idUserInterface* hud)
{
    hud->SetStateInt("current_wave", wave);
    hud->SetStateInt("strogg_hearts", stroggHearts);
    if (playerStatsLoaded)
        for (int i = 0; i < MID_NUM_PLAYER_STATS; i++)
            hud->SetStateFloat(playerStatsStr[i], *playerStats[i]);
    if (playerStatsShowingHelper) {
        if (playerStatsShowing)
            hud->HandleNamedEvent("midtermShow");
        else
            hud->HandleNamedEvent("midtermHide");
        playerStatsShowingHelper = false;
    }
}

void MidtermEnemyKilled(idEntity* enemy)
{
    const char* name = enemy->name.c_str();
    stroggHearts += enemy->stroggHearts;
}

void MidtermPause(const idCmdArgs &args)
{
    pause = !pause;
}

void MidtermSkipWave(const idCmdArgs &args)
{
    nextSpawnTime = gameLocal.GetTime()-1;
}

void MidtermSetClipSize(double clipSize)
{
    *playerStats[MID_STATS_CLIP_SIZE] = clipSize;
}
void MidtermSetMaxHealth(double maxHealth)
{
    *playerStats[MID_STATS_MAX_HEALTH] = maxHealth;
}
void MidtermSetHealthRegen(double healthRegen)
{
    *playerStats[MID_STATS_HEALTH_REGEN] = healthRegen;
}
void MidtermSetDamage(double damage)
{
    *playerStats[MID_STATS_DAMAGE] = damage;
}
void MidtermSetSpeed(double speed)
{
    *playerStats[MID_STATS_SPEED] = speed;
}

void MidtermSetClassCmd(const idCmdArgs &args)
{
    chosenClass = atoi(args.Argv(1));
}
void MidtermSetDamageCmd(const idCmdArgs &args)
{
    if (args.Argc() == 1) 
        return;
    MidtermSetDamage(atof(args.Argv(1)));
}
void MidtermSetSpeedCmd(const idCmdArgs &args)
{
    if (args.Argc() == 1) 
        return;
    MidtermSetSpeed(atof(args.Argv(1)));
}
void MidtermSetMaxHealthCmd(const idCmdArgs &args)
{
    if (args.Argc() == 1) 
        return;
    MidtermSetMaxHealth(atof(args.Argv(1)));
}
void MidtermSetHealthRegenCmd(const idCmdArgs &args)
{
    if (args.Argc() == 1) 
        return;
    MidtermSetHealthRegen(atof(args.Argv(1)));
}
void MidtermSetClipSizeCmd(const idCmdArgs &args)
{
    if (args.Argc() == 1) 
        return;
    MidtermSetClipSize(atof(args.Argv(1)));
}
void MidtermTogglePlayerStats()
{
    playerStatsShowing = !playerStatsShowing;
    playerStatsShowingHelper = true;
}
void MidtermIncDamage()
{
    if (stroggHearts <= 0) return;
    stroggHearts--;
    *playerStats[MID_STATS_DAMAGE] += 0.1;
}
void MidtermIncSpeed()
{
    if (stroggHearts <= 0) return;
    stroggHearts--;
    *playerStats[MID_STATS_SPEED] += 0.1;
}
void MidtermIncHealthRegen()
{
    if (stroggHearts <= 0) return;
    stroggHearts--;
    *playerStats[MID_STATS_HEALTH_REGEN] += 0.2;
}
void MidtermIncMaxHealth()
{
    if (stroggHearts <= 0) return;
    stroggHearts--;
    *playerStats[MID_STATS_MAX_HEALTH] += 10;
}
void MidtermIncClipSize()
{
    if (stroggHearts <= 0) return;
    stroggHearts--;
    *playerStats[MID_STATS_CLIP_SIZE] += 0.25;
}
