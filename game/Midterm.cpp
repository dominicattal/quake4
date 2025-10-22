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
int waveTime = 60000;
int initialWaveTime = 10000;
int stroggHearts = 0;
bool pause = false;
idVec3 bossPosition = idVec3(7.67, 54.04, 608.45);

void MidtermSpawn(idVec3 pos)
{
    idDict dict;
    if (rand() % 100 < rareProb)
        dict.Set("classname", rare_enemies[rand() % NUM_RARE_ENEMIES]);
    else
        dict.Set("classname", common_enemies[rand() % NUM_COMMON_ENEMIES]);
    dict.Set("origin", pos.ToString());
    gameLocal.Printf(pos.ToString());

	idEntity *newEnt = NULL;
	gameLocal.SpawnEntityDef( dict, &newEnt );

	if (newEnt)	{
		gameLocal.Printf("spawned entity '%s'\n", newEnt->name.c_str());
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
}

void MidtermSpawnWave()
{
    if (wave % 10 == 0 && wave != 0) {
        MidtermSpawnBoss();
        waveTime = 90000;
    }
    for (int i = 0; i < wave + 10; i++) {
        MidtermSpawn(positions[rand() % NUM_POSITIONS]);
        waveTime = 60000;
    }
}

void MidtermUpdate()
{
    if (pause)
        return;
    if (nextSpawnTime == 0) {
        nextSpawnTime = gameLocal.GetTime() + initialWaveTime;
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
}

void MidtermEnemyKilled(idEntity* attacker)
{
    const char* name = attacker->name.c_str();
    gameLocal.Printf("AAAA\n");
}

void MidtermPause(const idCmdArgs &args)
{
    pause = !pause;
}

void MidtermSkipWave(const idCmdArgs &args)
{
    nextSpawnTime = gameLocal.GetTime()-1;
}
