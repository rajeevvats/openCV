#include "perf_precomp.hpp"

using namespace std;
using namespace cv;
using namespace perf;

typedef perf::TestBaseWithParam<Size> SizePrm;

PERF_TEST_P( SizePrm, LUT,
             testing::Values(szQVGA, szVGA, sz1080p)
           )
{
    Size sz = GetParam();

    int maxValue = 255;

    Mat src(sz, CV_8UC1);
    randu(src, 0, maxValue);
    Mat lut(1, 256, CV_8UC1);
    randu(lut, 0, maxValue);
    Mat dst(sz, CV_8UC1);

    TEST_CYCLE() LUT(src, lut, dst);

    SANITY_CHECK_NOTHING();
}
