#include <unistd.h>
#include <iostream>
#include <vector>
#include <cassert>
#include <fstream>
#include <string>
#include <vector>
#include <cstdio>
#include <chrono>

//#define CL_USE_DEPRECATED_OPENCL_2_0_APIS
#include <CL/cl.hpp>
#include <immintrin.h>


using utyp32 = unsigned int;
using ityp32 = int;
using utyp64 = unsigned long long;

using cutyp32 = const utyp32;
using cityp32 = const ityp32;
using cutyp64 = const utyp64;

static cl::Program createClProgram(const std::string &pathToSrc, const char *const verbose) {
    std::vector< cl::Platform > platforms;
    cl_int success = cl::Platform::get( &platforms );
    assert( CL_SUCCESS ==  success );
    assert( platforms.size() > 0 );
    if( strcasecmp(verbose,"verbose") == 0 ) {
        for( size_t i=0 ; i<platforms.size() ; i++ ) {
            std::string buf;
            std::cout << "Platform " << (i+1) << "] " << std::endl;
            platforms[i].getInfo( CL_PLATFORM_PROFILE , &buf );
            std::cout << "  --profile: " << buf << std::endl;
            platforms[i].getInfo(  CL_PLATFORM_VERSION , &buf );
            std::cout << "  --version: " << buf << std::endl;
            platforms[i].getInfo(  CL_PLATFORM_NAME , &buf );
            std::cout << "  --name: " << buf << std::endl;
            platforms[i].getInfo(  CL_PLATFORM_VENDOR , &buf );
            std::cout << "  --vendor: " << buf << std::endl;
            platforms[i].getInfo(  CL_PLATFORM_EXTENSIONS , &buf );
            std::cout << "  --extensions: " << buf << std::endl;
        }
    }
    auto platform = platforms.front();

    std::vector<cl::Device> devices;
    success = platform.getDevices(CL_DEVICE_TYPE_ALL,&devices);
    assert( CL_SUCCESS ==  success );
    assert( devices.size() > 0 );

    if( strcasecmp(verbose,"verbose") == 0 ) {
        for( size_t i=0 ; i<devices.size() ; i++ ) {
            std::string sBuf;
            int iBuf;

            std::cout << "Device " << (i+1) << "] " << std::endl;
            devices[i].getInfo( CL_DEVICE_ADDRESS_BITS , &iBuf );
            std::cout << "  --address bits: " << iBuf << std::endl;
            devices[i].getInfo( CL_DEVICE_AVAILABLE , &iBuf );
            std::cout << "  --available: " << iBuf << std::endl;
            devices[i].getInfo( CL_DEVICE_COMPILER_AVAILABLE , &iBuf );
            std::cout << "  --compiler available: " << iBuf << std::endl;
            devices[i].getInfo( CL_DEVICE_ENDIAN_LITTLE , &iBuf );
            std::cout << "  --endian little: " << iBuf << std::endl;
            devices[i].getInfo( CL_DEVICE_ERROR_CORRECTION_SUPPORT , &iBuf );
            std::cout << "  --error correcton support: " << iBuf << std::endl;
            devices[i].getInfo( CL_DEVICE_EXECUTION_CAPABILITIES , &iBuf );
            std::cout << "  --execution capabilities: " << iBuf << std::endl;
            devices[i].getInfo( CL_DEVICE_EXTENSIONS , &sBuf );
            std::cout << "  --extensions: " << sBuf << std::endl;
            devices[i].getInfo( CL_DEVICE_GLOBAL_MEM_CACHE_SIZE , &iBuf );
            std::cout << "  --global mem cache size: " << iBuf << std::endl;
            devices[i].getInfo( CL_DEVICE_GLOBAL_MEM_CACHE_TYPE , &iBuf );
            std::cout << "  --global mem cache type: " << iBuf << std::endl;
            devices[i].getInfo( CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE , &iBuf );
            std::cout << "  --global mem cacheline size: " << iBuf << std::endl;
            devices[i].getInfo( CL_DEVICE_GLOBAL_MEM_SIZE , &iBuf );
            std::cout << "  --global mem size: " << iBuf << std::endl;
            devices[i].getInfo( CL_DEVICE_IMAGE_SUPPORT , &iBuf );
            std::cout << "  --image support: " << iBuf << std::endl;
            devices[i].getInfo( CL_DEVICE_IMAGE2D_MAX_HEIGHT , &iBuf );
            std::cout << "  --image2d max height: " << iBuf << std::endl;
            devices[i].getInfo( CL_DEVICE_IMAGE2D_MAX_WIDTH , &iBuf );
            std::cout << "  --image2d max width: " << iBuf << std::endl;
            devices[i].getInfo( CL_DEVICE_IMAGE3D_MAX_DEPTH , &iBuf );
            std::cout << "  --image3d max depth: " << iBuf << std::endl;
            devices[i].getInfo( CL_DEVICE_IMAGE3D_MAX_HEIGHT , &iBuf );
            std::cout << "  --image3d max height: " << iBuf << std::endl;
            devices[i].getInfo( CL_DEVICE_IMAGE3D_MAX_WIDTH , &iBuf );
            std::cout << "  --image3d max width: " << iBuf << std::endl;
            devices[i].getInfo( CL_DEVICE_LOCAL_MEM_SIZE , &iBuf );
            std::cout << "  --local mem size: " << iBuf << std::endl;
            devices[i].getInfo( CL_DEVICE_LOCAL_MEM_TYPE , &iBuf );
            std::cout << "  --local mem type: " << iBuf << std::endl;
            devices[i].getInfo( CL_DEVICE_MAX_CLOCK_FREQUENCY , &iBuf );
            std::cout << "  --max clock frequency: " << iBuf << std::endl;
            devices[i].getInfo( CL_DEVICE_MAX_COMPUTE_UNITS , &iBuf );
            std::cout << "  --max compute units: " << iBuf << std::endl;
            devices[i].getInfo( CL_DEVICE_MAX_CONSTANT_ARGS , &iBuf );
            std::cout << "  --max constant args: " << iBuf << std::endl;
            devices[i].getInfo( CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE , &iBuf );
            std::cout << "  --max constant buffer size: " << iBuf << std::endl;
            devices[i].getInfo( CL_DEVICE_MAX_MEM_ALLOC_SIZE , &iBuf );
            std::cout << "  --max mem alloc size: " << iBuf << std::endl;
            devices[i].getInfo( CL_DEVICE_MAX_PARAMETER_SIZE , &iBuf );
            std::cout << "  --max parameter size: " << iBuf << std::endl;
            devices[i].getInfo( CL_DEVICE_MAX_READ_IMAGE_ARGS , &iBuf );
            std::cout << "  --max read image args: " << iBuf << std::endl;
            devices[i].getInfo( CL_DEVICE_MAX_SAMPLERS , &iBuf );
            std::cout << "  --max samplers: " << iBuf << std::endl;
            devices[i].getInfo( CL_DEVICE_MAX_WORK_GROUP_SIZE , &iBuf );
            std::cout << "  --CL_DEVICE_MAX_WORK_GROUP_SIZE: " << iBuf << std::endl;
            devices[i].getInfo( CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS , &iBuf );
            std::cout << "  --CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS: " << iBuf << std::endl;
            devices[i].getInfo( CL_DEVICE_MAX_WORK_ITEM_SIZES , &iBuf );
            std::cout << "  --CL_DEVICE_MAX_WORK_ITEM_SIZES: " << iBuf << std::endl;
            devices[i].getInfo( CL_DEVICE_MAX_WRITE_IMAGE_ARGS , &iBuf );
            std::cout << "  --CL_DEVICE_MAX_WRITE_IMAGE_ARGS: " << iBuf << std::endl;
            devices[i].getInfo( CL_DEVICE_MEM_BASE_ADDR_ALIGN , &iBuf );
            std::cout << "  --CL_DEVICE_MEM_BASE_ADDR_ALIGN: " << iBuf << std::endl;
            devices[i].getInfo( CL_DEVICE_MIN_DATA_TYPE_ALIGN_SIZE , &iBuf );
            std::cout << "  --CL_DEVICE_MIN_DATA_TYPE_ALIGN_SIZE: " << iBuf << std::endl;
            devices[i].getInfo( CL_DEVICE_NAME , &iBuf );
            std::cout << "  --CL_DEVICE_NAME: " << iBuf << std::endl;
            devices[i].getInfo( CL_DEVICE_PLATFORM , &iBuf );
            std::cout << "  --CL_DEVICE_PLATFORM: " << iBuf << std::endl;
            devices[i].getInfo( CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR , &iBuf );
            std::cout << "  --CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR: " << iBuf << std::endl;
            devices[i].getInfo( CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE , &iBuf );
            std::cout << "  --CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE: " << iBuf << std::endl;
            devices[i].getInfo( CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT , &iBuf );
            std::cout << "  --CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT: " << iBuf << std::endl;
            devices[i].getInfo( CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT , &iBuf );
            std::cout << "  --CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT: " << iBuf << std::endl;
            devices[i].getInfo( CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG , &iBuf );
            std::cout << "  --CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG: " << iBuf << std::endl;
            devices[i].getInfo( CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT , &iBuf );
            std::cout << "  --CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT: " << iBuf << std::endl;
            devices[i].getInfo( CL_DEVICE_PROFILE , &iBuf );
            std::cout << "  --CL_DEVICE_PROFILE: " << iBuf << std::endl;
            devices[i].getInfo( CL_DEVICE_PROFILING_TIMER_RESOLUTION , &iBuf );
            std::cout << "  --CL_DEVICE_PROFILING_TIMER_RESOLUTION: " << iBuf << std::endl;
            devices[i].getInfo( CL_DEVICE_QUEUE_PROPERTIES , &iBuf );
            std::cout << "  --CL_DEVICE_QUEUE_PROPERTIES: " << iBuf << std::endl;
            devices[i].getInfo( CL_DEVICE_SINGLE_FP_CONFIG , &iBuf );
            std::cout << "  --CL_DEVICE_SINGLE_FP_CONFIG: " << iBuf << std::endl;
            devices[i].getInfo( CL_DEVICE_TYPE , &iBuf );
            std::cout << "  --CL_DEVICE_TYPE: " << iBuf << std::endl;
            devices[i].getInfo( CL_DEVICE_VENDOR_ID , &iBuf );
            std::cout << "  --CL_DEVICE_VENDOR_ID: " << iBuf << std::endl;
            devices[i].getInfo( CL_DEVICE_VENDOR , &sBuf );
            std::cout << "  --CL_DEVICE_VENDOR: " << sBuf << std::endl;
            devices[i].getInfo( CL_DEVICE_VERSION , &sBuf );
            std::cout << "  --CL_DEVICE_VERSION: " << sBuf << std::endl;
            devices[i].getInfo( CL_DRIVER_VERSION , &sBuf );
            std::cout << "  --CL_DRIVER_VERSION: " << sBuf << std::endl;
        }
    }

    auto device = devices.front();

    std::ifstream file(pathToSrc);
    std::string src( std::istreambuf_iterator<char>(file) , (std::istreambuf_iterator<char>()) );
    cl::Program::Sources sources = { std::pair<const char*, ::size_t>(src.c_str(), src.length()+1 ) };
    cl::Context context(device);
    cl::Program program(context,sources);
    success = program.build("-cl-std=CL2.0");
    assert( CL_SUCCESS == success );
    return program;
}

class Rnd4Lin  : public RndBase {
private:
    utyp64 a,b,c,d;
    utyp32 sa,sb,sc,sd;
private:
    static bool test( utyp32 &s , cutyp32 max) {
        if( s++ < max ) {
            return false;
        }
        s = 0;
        return true;
    }
    static void next( utyp64 &v, cutyp64 A, cutyp64 B ) {
        v = v * A + B;
    }
    static void next( utyp64 &v, utyp32 &s, cutyp32 max, cutyp64 A, cutyp64 B ) {
        next( v , A , B );
        if( test( s , max ) ) {
            next( v , A , B );
        }
    }
public:
    Rnd4Lin( cutyp64 __sd) {
        seed( __sd );
    }
    void seed( cutyp64 __sd ) {
        a = __sd ^ 0x055910041214AED9ULL;
        b = __sd ^ 0xAC1144C2DA18253EULL;
        c = __sd ^ 0xD775B26A5E40A18AULL;
        d = __sd ^ 0xC22556BCAAB6EC12ULL;
        sa = sb = sc = sd = 0;
    }
    utyp32 operator ()() {
        next( a , sa , 10 , 119821673ull,  53695357673ull );
        next( b , sb , 12 , 174990143ull,  67869171119ull );
        next( c , sc , 16 , 139917857ull,  18819389437ull );
        next( d , sd , 18 ,  11744023ull,  65463955637ull );
        return (utyp32) (
            (
              ( ( a >> (64-11) ) <<  0 ) |
              ( ( b >> (64-11) ) << 11 ) |
              ( ( c >> (64-10) ) << 22 )
            ) ^ ( d >> 32 )
        );
    }

};

static std::vector<utyp64> mkSeeds(
    const char *const seed,
    const char *const n_tasks
) {
    Rnd4BLin rnd( atoi(seed) );
    std::vector<utyp64> seeds( atoi(n_tasks) );
    for( utyp32 i=0 ; i<seeds.size() ; i++ ) {
        seeds[i] = rnd();
        seeds[i] <<= 32;
        seeds[i] |= rnd();
    }
    return seeds;
}



static std::vector<utyp64> versionOpenCL(
    std::vector<utyp64>       &seeds,
    cutyp64                   n_loops,
    const char *const         verbose,
    std::chrono::duration<double> &elapsedTime
) {
    cl_int success;
    std::vector<utyp64> sums(seeds.size());
    cutyp32 MEMSIZE = seeds.size() * sizeof(utyp64);

    cl::Program program = createClProgram("OpenCLSumRnd.cl",verbose);
    auto context = program.getInfo<CL_PROGRAM_CONTEXT>();
    auto devices = program.getInfo<CL_PROGRAM_DEVICES>();
    auto device = devices.front();

    const auto start = std::chrono::steady_clock::now();

    cl::Buffer inBuf(context, CL_MEM_READ_ONLY | CL_MEM_HOST_NO_ACCESS | CL_MEM_COPY_HOST_PTR, MEMSIZE, seeds.data() , &success );
    assert( success == CL_SUCCESS );

    cl::Buffer outBuf(context,CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY , MEMSIZE , nullptr , &success );
    assert( success == CL_SUCCESS );

    cl::Kernel kernel( program , "OpenCLSumRnd");

    success = kernel.setArg(0,inBuf);
    assert( success == CL_SUCCESS );

    success = kernel.setArg(1,outBuf);
    assert( success == CL_SUCCESS );

    success = kernel.setArg(2,n_loops);
    assert( success == CL_SUCCESS );

    cl::CommandQueue queue( context , device );
    success = queue.enqueueNDRangeKernel( kernel, cl::NullRange, cl::NDRange(seeds.size()) );
    assert( success == CL_SUCCESS );

    success = queue.enqueueReadBuffer( outBuf, CL_TRUE, 0 , MEMSIZE, sums.data() );
    assert( success == CL_SUCCESS );

    cl::finish();

    elapsedTime = std::chrono::steady_clock::now() - start;

    return sums;
}

static std::vector<utyp64> versionOpenMP(
    const std::vector<utyp64> &seeds,
    cutyp64                   n_loops,
    std::chrono::duration<double> &elapsedTime
) {
    std::vector<utyp64> sums(seeds.size());
    const auto start = std::chrono::steady_clock::now();
    {
        #pragma omp parallel for
        for( utyp32 i=0 ; i<seeds.size() ; i++ ) {
            sums[i] = 0;
            Rnd4BLin rnd( seeds[i] );
            for( utyp64 j=0 ; j<n_loops ; j++ ) {
                sums[i] += rnd();
            }
        }
    }
    elapsedTime = std::chrono::steady_clock::now() - start;
    return sums;
}

static std::vector<utyp64> versionOneThread(
    const std::vector<utyp64> &seeds,
    cutyp64                   n_loops,
    std::chrono::duration<double> &elapsedTime
) {
    std::vector<utyp64> sums(seeds.size());
    const auto start = std::chrono::steady_clock::now();
    for( utyp32 i=0 ; i<seeds.size() ; i++ ) {
        sums[i] = 0;
        Rnd4BLin rnd( seeds[i] );
        for( utyp64 j=0 ; j<n_loops ; j++ ) {
            sums[i] += rnd();
        }
    }
    elapsedTime = std::chrono::steady_clock::now() - start;
    return sums;
}

static void testCrc(
    const std::vector<utyp64> &sums
) {
    utyp64 crc = 0;
    for( utyp32 i=0 ; i<sums.size() ; i++ ) {
        crc += ( sums[i] ^ ( crc >> 12 ));
    }
    std::cout << "crc: " << crc << std::endl;
}

int main(int argc, char *argv[]) {
    if( argc != 6 ) {
        std::cout << "Using OpenCLSumRnd verbose|silent OpenCL|OpenMP|OneThread seed tasks loops" << std::endl;
        return 0;
    }

    constexpr cutyp32 ARG_VERB  = (1);
    constexpr cutyp32 ARG_MODEL = (2);
    constexpr cutyp32 ARG_SEED  = (3);
    constexpr cutyp32 ARG_LOOPS = (4);
    constexpr cutyp32 ARG_TASKS = (5);

    cutyp64 n_loops = atoi(argv[ARG_LOOPS]);

    std::vector<utyp64> seeds = mkSeeds( argv[ARG_SEED] , argv[ARG_TASKS] );
    std::vector<utyp64> sums;

    std::chrono::duration<double> elapsedTime;
    if( strcasecmp( argv[ARG_MODEL] , "OpenCl") == 0 ) {        Rnd4BLin rnd(12345);
        sums = versionOpenCL( seeds , n_loops , argv[ARG_VERB] , elapsedTime );
    } else if( strcasecmp( argv[ARG_MODEL] , "OpenMP") == 0 ) {
        sums = versionOpenMP( seeds , n_loops , elapsedTime );
    } else if( strcasecmp( argv[ARG_MODEL] , "OneThread") == 0 ) {
        sums = versionOneThread( seeds , n_loops , elapsedTime );
    } else {
        std::cout << "Invalid compute model '" << argv[ARG_MODEL] << "'" << std::endl;
        abort();
    }

    testCrc( sums );
    std::cout << "Elapsed time: " << elapsedTime.count() << "s " << std::endl;

    return 0;
}
