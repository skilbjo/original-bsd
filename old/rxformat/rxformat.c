/*	rxformat.c	4.3	83/05/08	*/

#include <stdio.h>
#include <sys/file.h>
#include <errno.h>
#include "/sys/vaxuba/rxreg.h"

char devname[] = "/dev/rrx?a";

/*
 * format floppy disks on RX02
 */

main(argc, argv)
	int argc;
	char *argv[];
{
	int fd, idens = 0, filarg = 1;

	if (argc < 2)
		usage();
	if (argc == 3) { 
		if (strncmp(argv[1],"-d",2) != 0)
			usage();
		idens++;
		filarg++;
	}
	devname[8] = argv[filarg][7];
	if ((fd = open(devname, FRDWR, 0600)) < NULL) {
		perror(devname);
		exit (0);
	}
	printf("Format %s to", argv[filarg]);
	if (idens)
		printf(" double density (y/n) ?");
	else
		printf(" single density (y/n) ?");
	if (getchar() != 'y')
		exit (0);
	/* 
	 * change the ioctl command when dkio.h has
	 * been finished
	 */
	if (ioctl(fd, RXIOC_FORMAT, &idens) != NULL)
		perror(devname);
	close (fd);
}

usage()
{
	fprintf(stderr, "usage: rxformat [-d] /dev/rx?\n");
	exit (0);
}
