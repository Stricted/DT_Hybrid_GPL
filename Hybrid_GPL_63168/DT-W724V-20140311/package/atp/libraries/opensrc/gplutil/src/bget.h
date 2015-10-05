/*

    Interface definitions for bget.c, the memory management package.

*/

#ifndef __BGET_H__
#define __BGET_H__

typedef long bufsize;
void	bpool(void *pvBgetHandle, void *buffer, bufsize len);
void   *bget(void *pvBgetHandle, bufsize size);
void   *bgetz	    (void *pvBgetHandle, bufsize size);
void   *bgetr	    (void *pvBgetHandle, void *buffer, bufsize newsize);
void	brel	    (void *pvBgetHandle, void *buf);
void	bectl	   (int (*compact)(bufsize sizereq, int sequence),
		       void *(*acquire)(bufsize size),
		       void (*release)(void *buf), bufsize pool_incr);
void	bstats	    (void *pvBgetHandle, bufsize *curalloc, bufsize *totfree, bufsize *maxfree,
		       long *nget, long *nrel);
void	bstatse     (bufsize *pool_incr, long *npool, long *npget,
		       long *nprel, long *ndget, long *ndrel);
void	bufdump     (void *buf);
void	bpoold	    (void *pool, int dumpalloc, int dumpfree);
int	bpoolv	    (void *pool);

/* Queue links */

struct qlinks {
    struct bfhead *flink;	      /* Forward link */
    struct bfhead *blink;	      /* Backward link */
};

/* Header in allocated and free buffers */

struct bhead {
    bufsize prevfree;		      /* Relative link back to previous
					 free buffer in memory or 0 if
					 previous buffer is allocated.	*/
    bufsize bsize;		      /* Buffer size: positive if free,
					 negative if allocated. */
};
#define BH(p)	((struct bhead *) (p))

/*  Header in directly allocated buffers (by acqfcn) */

struct bdhead {
    bufsize tsize;		      /* Total size, including overhead */
    struct bhead bh;		      /* Common header */
};
#define BDH(p)	((struct bdhead *) (p))

/* Header in free buffers */

struct bfhead {
    struct bhead bh;		      /* Common allocated/free header */
    struct qlinks ql;		      /* Links on free list */
};
#define BFH(p)	((struct bfhead *) (p))

#define BufStats 1
/* Bget 全局结构 */
typedef struct tagATP_UTIL_BGET_GLOBAL_ST
{
    struct bfhead freelist; // bget memory block list
#ifdef BufStats
    bufsize totalloc;
    long numget;
    long numrel;
#endif
}ATP_UTIL_BGET_GLOBAL_ST;

#endif

