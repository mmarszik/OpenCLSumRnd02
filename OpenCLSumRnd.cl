/*
typedef struct Rnd3Lin {
    ulong a,b,c;
    uint  s1,s2,s3;
} TRnd3Lin;
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
static void seed( TRnd3Lin *const r, const ulong __sd ) {
    r->a = __sd ^ (ulong)0x140CA25429E95B21;
    r->b = __sd ^ (ulong)0x4A37D2E9E5D5C6A3;
    r->c = __sd ^ (ulong)0xD76C0A509DACE77D;
    r->s1 = r->s2 = r->s3 = 0;
}
uint getRnd(TRnd3Lin *const r) {
    next( &r->a , &r->s1 ,  6 , (ulong)195366727,  (ulong)3788059271 );
    next( &r->b , &r->s2 , 10 , (ulong)201733549,  (ulong)6004841807 );
    next( &r->c , &r->s3 , 12 ,  (ulong)87604849, (ulong)11409409549 );
    return (uint) ( ( ( r->a >> 48 ) | ( ( r->b >> 48 ) << 16 ) ) ^ ( r->c >> 32 ) );
}

__kernel void OpenCLSumRnd(
    __global const ulong*  seeds,
    __global ulong*        sums,
    const ulong n_loops
) {
    TRnd3Lin rnd;
    seed( &rnd, seeds[ get_global_id(0) ] );
    ulong sum = 0;
    for( ulong i=0 ; i<n_loops ; i++ ) {
        sum += getRnd( &rnd );
    }
    sums[ get_global_id(0) ] = sum;
}
*/

__kernel void OpenCLSumRnd(
    __global const ulong*  seeds,
    __global ulong*        sums,
    const ulong n_loops
) {
    cl_ulong x = {1,1,1,1};
    sums[ get_global_id(0) ] = 1;
}
