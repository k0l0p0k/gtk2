
CC = gcc

#CFLAGS = -Wall			 	\
#	-DG_DISABLE_DEPRECATED 	 	\
#	-DGDK_DISABLE_DEPRECATED 	\
#	-DGDK_PIXBUF_DISABLE_DEPRECATED \
#	-DGTK_DISABLE_DEPRECATED

CFLAGS = 
	
clist2: clist2.c 
	$(CC) clist2.c -o clist2 $(CFLAGS) `pkg-config gtk+-2.0 --cflags --libs`	
	
сlist: сlist.c 	
	$(CC) clist.c -o clist `pkg-config gtk+-2.0 --cflags --libs`

clean: 
	rm -f *.o clist*
