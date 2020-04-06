typedef struct Rnd4Lin {
    ulong a,b,c,d;
    uint  sa,sb,sc,sd;
} TRnd4Lin;
static bool test( uint *const s , const uint max) {
    if( (*s)++ < max ) {
        return false;
    }
    *s = 0;
    return true;
}
static void nextV( ulong *const v, const ulong A, const ulong B ) {
    *v = *v * A + B;
}
static void next( ulong *const v, uint *const s, const uint max, const ulong A, const ulong B ) {
    nextV( v , A , B );
    if( test( s , max ) ) {
        nextV( v , A , B );
    }
}
static void seed( TRnd4Lin *const r, const ulong __sd ) {
    r->a = __sd ^ (ulong)0x055910041214AED9;
    r->b = __sd ^ (ulong)0xAC1144C2DA18253E;
    r->c = __sd ^ (ulong)0xD775B26A5E40A18A;
    r->d = __sd ^ (ulong)0xC22556BCAAB6EC12;
    r->sa = r->sb = r->sc = r->sd = 0;
}
uint getRnd(TRnd4Lin *const r) {
    next( &r->a , &r->sa , 10 , (ulong)119821673,  (ulong)53695357673 );
    next( &r->b , &r->sb , 12 , (ulong)174990143,  (ulong)67869171119 );
    next( &r->c , &r->sc , 16 , (ulong)139917857,  (ulong)18819389437 );
    next( &r->c , &r->sd , 18 ,  (ulong)11744023,  (ulong)65463955637 );
        return (uint) (
            (
              ( ( r->a >> (64-11) ) <<  0 ) |
              ( ( r->b >> (64-11) ) << 11 ) |
              ( ( r->c >> (64-10) ) << 22 )
            ) ^ ( r->d >> 32 )
        );
}
__kernel void OpenCLSumRnd(
    __global const ulong*  seeds,
    __global ulong*        sums,
    const ulong n_loops
) {
    TRnd4Lin rnd;
    seed( &rnd, seeds[ get_global_id(0) ] );
    ulong sum = 0;
    for( ulong i=0 ; i<n_loops ; i++ ) {
        sum += getRnd( &rnd );
    }
    sums[ get_global_id(0) ] = sum;
}
