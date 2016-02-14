#include <hl.h>

vbytes *hl_balloc( int size ) {
	return (vbytes*)hl_gc_alloc_noptr(size);
}

vbytes *hl_bcopy( vbytes *ptr, int size ) {
	vbytes *b = hl_balloc(size);
	memcpy(b,ptr,size);
	return b;
}

HL_PRIM void hl_bblit( char *dst, int dpos, char *src, int spos, int len ) {
	memcpy(dst + dpos,src+spos,len);
}

HL_PRIM int hl_bytes_compare( vbytes *a, int apos, vbytes *b, int bpos, int len ) {
	return memcmp(a+apos,b+bpos,len);
}

typedef unsigned char byte;
static void *
memfind_rb (const void  *in_block,      /*  Block containing data            */
            size_t       block_size,    /*  Size of block in bytes           */
            const void  *in_pattern,    /*  Pattern to search for            */
            size_t       pattern_size,  /*  Size of pattern block            */
            size_t      *shift,         /*  Shift table (search buffer)      */
            bool        *repeat_find)   /*  TRUE: search buffer already init */
{
    size_t
        byte_nbr,                       /*  Distance through block           */
        match_size;                     /*  Size of matched part             */
    const byte
        *match_base = NULL,             /*  Base of match of pattern         */
        *match_ptr  = NULL,             /*  Point within current match       */
        *limit      = NULL;             /*  Last potiental match point       */
    const byte
        *block   = (byte *) in_block,   /*  Concrete pointer to block data   */
        *pattern = (byte *) in_pattern; /*  Concrete pointer to search value */

    if (block == NULL || pattern == NULL || shift == NULL)
        return (NULL);

    /*  Pattern must be smaller or equal in size to string                   */
    if (block_size < pattern_size)
        return (NULL);                  /*  Otherwise it's not found         */

    if (pattern_size == 0)              /*  Empty patterns match at start    */
        return ((void *)block);

    /*  Build the shift table unless we're continuing a previous search      */

    /*  The shift table determines how far to shift before trying to match   */
    /*  again, if a match at this point fails.  If the byte after where the  */
    /*  end of our pattern falls is not in our pattern, then we start to     */
    /*  match again after that byte; otherwise we line up the last occurence */
    /*  of that byte in our pattern under that byte, and try match again.    */

    if (!repeat_find || !*repeat_find)
      {
        for (byte_nbr = 0; byte_nbr < 256; byte_nbr++)
            shift [byte_nbr] = pattern_size + 1;
        for (byte_nbr = 0; byte_nbr < pattern_size; byte_nbr++)
            shift [(byte) pattern [byte_nbr]] = pattern_size - byte_nbr;

        if (repeat_find)
            *repeat_find = true;
      }

    /*  Search for the block, each time jumping up by the amount             */
    /*  computed in the shift table                                          */

    limit = block + (block_size - pattern_size + 1);

    for (match_base = block;
         match_base < limit;
         match_base += shift [*(match_base + pattern_size)])
      {
        match_ptr  = match_base;
        match_size = 0;

        /*  Compare pattern until it all matches, or we find a difference    */
        while (*match_ptr++ == pattern [match_size++])
          {
            /*  If we found a match, return the start address                */
            if (match_size >= pattern_size)
              return ((void*)(match_base));

          }
      }
    return NULL;
}

int hl_bytes_find( vbytes *where, int pos, int len, vbytes *which, int wpos, int wlen ) {
	size_t searchbuf [256];
	bool repeat_find = false;
	vbytes *found = (vbytes*)memfind_rb(where + pos,len,which+wpos,wlen,searchbuf,&repeat_find);
	if( found == NULL ) return -1;
	return (int)(size_t)(found - (where + pos));
}

void hl_bytes_fill( vbytes *bytes, int pos, int len, int value ) {
	memset(bytes+pos,value,len);
}

void hl_bsort_i32( vbytes *bytes, int pos, int len, vclosure *cmp ) {
	hl_fatal("TODO");
}

void hl_bsort_f64( vbytes *bytes, int pos, int len, vclosure *cmp ) {
	hl_fatal("TODO");
}

DEFINE_PRIM(_BYTES,hl_balloc,_I32);
DEFINE_PRIM(_VOID,hl_bblit,_BYTES _I32 _BYTES _I32 _I32);
DEFINE_PRIM(_I32,hl_bytes_compare,_BYTES _I32 _BYTES _I32 _I32);
DEFINE_PRIM(_I32,hl_bytes_find,_BYTES _I32 _I32 _BYTES _I32 _I32);
DEFINE_PRIM(_VOID,hl_bytes_fill,_BYTES _I32 _I32 _I32);
DEFINE_PRIM(_VOID,hl_bsort_i32,_BYTES _I32 _I32 _FUN(_I32,_I32 _I32));
DEFINE_PRIM(_VOID,hl_bsort_f64,_BYTES _I32 _I32 _FUN(_I32,_F64 _F64));
