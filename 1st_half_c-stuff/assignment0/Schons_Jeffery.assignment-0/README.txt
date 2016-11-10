Looking at the code to add i decided tha tthe only things that we needed to know was where the "ant" was (i used i
for its location on x-axes and j for y-axes in buffer)and what direction it is facing (using an int 
(up=1, right = 2 down = 3, left = 4)). 
First I preset the location and direction then went into a while loop to see if it was in the range. If the ant was 
not in the range it would finish encoding. Then i would check if buffer[i][j]== (0 or 1) (white=0, black =1).
the direction would be increaced (or decreaced if black) by 1 to represent change in direction. If the direction goes
above 4 (or bellow 1 if black) it is set to 1 (4 if black). then based on what the direction is i eddit j and i to 
show movement
