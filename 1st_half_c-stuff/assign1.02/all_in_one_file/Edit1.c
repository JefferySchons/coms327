int main(int argc, char *argv[])
{

	int NX=160;
	int NY=96;
	char dungeonMap[NX][NY];
	int numOfRooms=12;
	//void makeDungionMap(int NX, int NY, char map[NX][NY], int numbOfRooms, Rooms roomList [])
	Rooms listOfRoomInfo [numOfRooms];
	
	int i;
	int j;
	
	//the args in main allow reading from omand line
	//read in command line
	if (strcmp("--save", argv[1]) == 0)
	{
		//make dungion
		printf("omg Save This Thing");
		printf("\n");
		int saveDungion;
		saveDungion=save(NX, NY, dungeonMap, numOfRooms, listOfRoomInfo);
		if(saveDungion!=0)
		{
			printf("save failed");
			printf("\n");
		}
	}
	else if(strcmp("--load", argv[1]) == 0)
	{
		//printf("you load now");
		printf("\n");
		int loadDungion;
		loadDungion=load(NX, NY, dungeonMap, numOfRooms, listOfRoomInfo);
		if(loadDungion != 0)
		{
			printf("load failed");
			printf("\n");
		}
	}
	else
	{
		printf("not right command plese enter --load or --save");
	}
	
	
	//to test movement
	int upperLX=0; //cant have open at 0,0
	int upperLY=0;
	
	int lowerRX;
	int lowerRY;
	
	//to find uperleft //and lower right
    for (i = 0; i < NX; i++)
    {
        for (j = 0; j < NY; j++)
		{
			if(dungeonMap[i][j]== '.')
			{
				if(upperLX=0)
				{
					upperLX=i;
					upperLY=j;
				}
				lowerRX=i;
				lowerRY=j;
			}
			//at dungeonMap[i][j]
		}
    }
	
	Dijkstra(NX, NY, dungeonMap, upperLX,upperLY, lowerRX, lowerRY); //should eddit map
	
	
	
	//to test if dungion map loaded correctly/ final print of dungion
	printf("\n"); 
    for (i = 0; i < NX; i++)
    {
        for (j = 0; j < NY; j++)
		{
			printf("%c ", dungeonMap[i][j]);
		}
        printf("\n"); 
    }
	
	//makeDungionMap(NX, NY, dungeonMap, numOfRooms, listOfRoomInfo);
	
	/*
	//to print argument sent in for bug checking
    for (i = 0; i < argc; ++i)
    {
		printf("\n");
       printf("argv[%d]: %s\n", i, argv[i]);
    }
	*/
	
//FILE *ptr;

//ptr = fopen("test.bin","rb");  // r for read, b for binary

//fread(buffer,sizeof(buffer),1,ptr); // read 10 bytes to our buffer

}