#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>
#include <unistd.h>
#include <err.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <libutil.h>            /* has to be included after sys/types.h */

enum {
        LINELEN = 1024
};

void            usage(void);
int             getsize(char *path, int *size);

int
main(int argc, char *argv[])
{
        char           *line;
        int64_t         sumsize = 0, maxsize = -1;
        int             debug = 0;
        char            delim = '\n';
        char            opt;

        while ((opt = getopt(argc, argv, "d0hs:")) != -1) {
                switch (opt) {
                case 'd':
                        debug = 1;
                        break;
                case '0':
                        delim = '\0';
                        break;
                case 'h':
                        usage();
                        return (EX_OK);
                case 's':
                        if (expand_number(optarg, &maxsize) == -1)
                                err(EX_DATAERR, "expand_numper(\"%s\", ...)", optarg);
                        break;
                default:
                        usage();
                        return (EX_USAGE);
                }
        }
        if (maxsize == -1) {
                usage();
                return (EX_USAGE);
        }
        if ((line = malloc(LINELEN)) == NULL)
                err(EX_SOFTWARE, "malloc(%d)", LINELEN);

        while (fgets(line, LINELEN, stdin) != NULL) {
                char           *p;
                int             s;

                if ((p = strchr(line, (int)'\n')) != NULL)
                        *p = '\0';

                if (getsize(line, &s) == -1)
                        continue;

                if (debug)
                        fprintf(stderr, "%s: %s: %d total: %jd max: %jd\n",
                              getprogname(), line, s, sumsize + s, maxsize);

                if ((sumsize += s) > maxsize)
                        break;

                fputs(line, stdout);
                putchar(delim);
        }
        if (ferror(stdin))
                errx(EX_SOFTWARE, "gets");

        free(line);

        return (EX_OK);
}

int
getsize(char *path, int *size)
{
        struct stat     s;

        assert(path != NULL);
        assert(size != NULL);

        if (stat(path, &s) == -1) {
                warn("stat(\"%s\", ...)", path);
                return (-1);
        }
        *size = s.st_size;

        return (0);
}

void
usage(void)
{
        fputs("usage:\n", stderr);
        fprintf(stderr, "\t%s -h\n", getprogname());
        fprintf(stderr, "\t%s [-d0] -s <bytes>\n", getprogname());
}
