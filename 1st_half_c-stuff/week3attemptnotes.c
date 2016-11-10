//new part to dungeon object
//distance_to_pc
//path to player through walls //not using at moment

int pathfinding(dungeon_t *d, int16_t y, int16_t x)
{
	//using dykstasas
	//not through walls
	//the x and y are where player is
	if(d->hardness[y][x] !=0)
	{
		//location for this placment is unvallid please use the path through walls
		//to player (not writtent yet)
		return 1;
	}
	
	//preset the path 
	for(i=0; i<y_size; i++)
	{
		for(j=0; j<x_size; j++)
		{
			d->distance_to_pc[i][j]=="."
		}
	}
	
	int i;
	int j;
	
	//checks if noo change has been made in iteration
	int nochange=1;
	d->distance_to_pc[y][x]=0
	while(nochange!=0)
	{
		nochange=0;
		for(i=0; i<y_size; i++)
		{
			for(j=0; j<x_size; j++)
			{
				if(d->distance_to_pc[i][j] != '.') //has been changed
				{
					//change its upper
					if(d->hardness[i+1][j]==0)
					{
						d->distance_to_pc[i+1][j]=d->distance_to_pc[i][j]+1;
						nochange=1;
					}
					//change its lower
					if(d->hardness[i-1][j]==0)
					{
						d->distance_to_pc[i-1][j]=d->distance_to_pc[i][j]+1;
						nochange=1;
					}
					//change its left
					if(d->hardness[i][j+1]==0)
					{
						d->distance_to_pc[i][j+1]=d->distance_to_pc[i][j]+1;
						nochange=1;
					}
					//change its right
					if(d->hardness[i][j-1]==0)
					{
						d->distance_to_pc[i][j-1]=d->distance_to_pc[i][j]+1;
						nochange=1;
					}
				}
			}
		}
	}
	
	return 0;
	//d->hardness[DUNGEON_Y][DUNGEON_X];
}
