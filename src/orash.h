#include <stdio.h>

#ifndef __ORASH_H
#define __ORASH_H

/*
** State information about the database connection is contained in an
** instance of the following structure.
*/
typedef struct {
  unsigned mEqpLines;    /* Mask of veritical lines in the EQP output graph */
  int outCount;          /* Revert to stdout when reaching zero */
  int cnt;               /* Number of records displayed so far */
  int lineno;            /* Line number of last line read from in */
  int openFlags;         /* Additional flags to open.  (SQLITE_OPEN_NOFOLLOW) */
  FILE *ifd;             /* Input file descriptor */
  FILE *ofd;             /* Output file descriptor */
  FILE *tfd;             /* Trace file descriptor (reserved, not yet done) */
  int nErr;              /* Number of errors seen */
  int mode;              /* An output mode setting */
  int modePrior;         /* Saved mode */
  int cMode;             /* temporary output mode for the current query */
  int normalMode;        /* Output mode before ".explain on" */
  int showHeader;        /* True to show column names in List or Column mode */
  int nCheck;            /* Number of ".check" commands run */
  unsigned nProgress;    /* Number of progress callbacks encountered */
  unsigned mxProgress;   /* Maximum progress callbacks before failing */
  unsigned flgProgress;  /* Flags for the progress callback */
  unsigned shellFlgs;    /* Various flags */
  unsigned priorShFlgs;  /* Saved copy of flags */
  char zTestcase[30];    /* Name of current test case */
  char colSeparator[20]; /* Column separator character for several modes */
  char rowSeparator[20]; /* Row separator character for MODE_Ascii */
  char colSepPrior[20];  /* Saved column separator */
  char rowSepPrior[20];  /* Saved row separator */
  int colWidth[100];     /* Requested width of each column when in column mode*/
  int actualWidth[100];  /* Actual width of each column */
  char nullValue[20];    /* The text to print when a NULL comes back from the database */
  char outfile[16384];  /* Filename for *out */
  FILE *pLog;            /* Write log output here */
  int *aiIndent;         /* Array of indents used in MODE_Explain */
  int nIndent;           /* Size of array aiIndent[] */
  int iIndent;           /* Index of current op in aiIndent[] */
} orash_t;

#endif /* __ORASH_H */
