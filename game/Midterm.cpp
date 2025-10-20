idVec3 positions[1] = {
    idVec3(-150.47, -414.45, 161.25)
};
int nextSpawnTime = 0;

void MidtermSpawn(idVec3 pos)
{
    idDict dict;
    dict.Set("classname", "monster_strogg_marine");
    dict.Set("origin", pos.ToString());
    gameLocal.Printf(pos.ToString());

	idEntity *newEnt = NULL;
	gameLocal.SpawnEntityDef( dict, &newEnt );

	if (newEnt)	{
		gameLocal.Printf("spawned entity '%s'\n", newEnt->name.c_str());
	}

}

void MidtermInit()
{
}

void MidtermUpdate()
{
    //gameLocal.Printf("%d %d\n", gameLocal.GetTime(), nextSpawnTime);
    if (gameLocal.GetTime() > nextSpawnTime) {
        MidtermSpawn(positions[0]);
        nextSpawnTime = gameLocal.GetTime() + 1000;
    }
}

