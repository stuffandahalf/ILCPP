/* Copyright (C) 2020 Gregory Norton <gregory.norton@me.com */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#ifdef HAVE_PTHREADS
#include <pthread.h>
#endif
#include "dynarray.h"

#define BUFFER_SIZE	512
#define FILE_LANGUAGE_UNSET		0
#define FILE_LANGUAGE_GLSL		1
#define FILE_LANGUAGE_OPENCL	2

static int configure(int argc, char **argv);

static int language;
static char buffer[BUFFER_SIZE];
DYNARRAY(const char *) infiles;

static void
release_mem(void)
{
	int i;
	
	for (i = 0; i < infiles.len; i++) {
		free((void *)infiles.array[i]);
	}
	dynarray_release(infiles);
}

int
main(int argc, char **argv)
{
	int i;
	
	atexit(release_mem);
	
	dynarray_init(char *, infiles);
	language = FILE_LANGUAGE_UNSET;
	
	if (!configure(argc, argv)) {
		return 1;
	}
	
	for (i = 0; i < infiles.len; i++) {
		printf("%s\n", infiles.array[i]);
	}
	
	return 0;
}

static int
print_usage(FILE *stream, const char *command)
{
#ifdef HAVE_PTHREADS
	return fprintf(stream, "Usage: %s [-h] [-t n] -l (GLSL|OpenCL) "
		"file...\n", command);
#else
	return fprintf(stream, "Usage: %s [-h] -l (GLSL|OpenCL) file...\n",
		command);
#endif
}

static int
configure(int argc, char **argv)
{
	int c, i;
	char *inpath;
	size_t sz;

#ifdef HAVE_PTHREADS
	while ((c = getopt(argc, argv, "l:ht:")) != -1) {
#else
	while ((c = getopt(argc, argv, "l:h")) != -1) {
#endif
		switch (c) {
#ifdef HAVE_PTHREADS
		case 't':
			/* threads */
			break;
#endif
		case 'l':
			if (!strcmp("GLSL", optarg)) {
				language = FILE_LANGUAGE_GLSL;
			} else if (!strcmp("OpenCL", optarg)) {
				language = FILE_LANGUAGE_OPENCL;
			} else {
				fprintf(stderr, "Unrecognized language specified \"%s\"\n",
					optarg);
				return 0;
			}
			break;
		case '?':
		case 'h':
		default:
			print_usage(stderr, argv[0]);
			return 0;
		}
	}
	if (!language) {
		fprintf(stderr, "ERROR Input file language not set\n");
		print_usage(stderr, argv[0]);
		return 0;
	}
	
	for (i = optind; i < argc; i++) {
		sz = strlen(argv[i]);
		inpath = malloc(sizeof(char) * (sz + 1));
		if (!inpath) {
			fprintf(stderr, "Failed to allocate buffer for input file\n");
			return 1;
		}
		strcpy(inpath, argv[i]);
		dynarray_append(infiles, inpath);
	}
	
	return 1;
}
