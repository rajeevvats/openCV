#include <opencv2/imgproc.hpp>
#include <opencv2/gapi/render/render.hpp> // Kernel API's

#include "api/render_ocv.hpp"

namespace cv
{
namespace gapi
{
namespace wip
{
namespace draw
{

inline void mosaic(cv::Mat& mat, const cv::Rect &rect, int cellSz)
{
    cv::Mat msc_roi = mat(rect);
    int crop_x = msc_roi.cols - msc_roi.cols % cellSz;
    int crop_y = msc_roi.rows - msc_roi.rows % cellSz;

    for(int i = 0; i < crop_y; i += cellSz ) {
        for(int j = 0; j < crop_x; j += cellSz) {
            auto cell_roi = msc_roi(cv::Rect(j, i, cellSz, cellSz));
            cell_roi = cv::mean(cell_roi);
        }
    }
};

inline void poly(cv::Mat& mat,
                 const std::vector<cv::Point>& points,
                 const cv::Scalar& color)
{
    std::vector<std::vector<cv::Point>> pp{points};
    cv::fillPoly(mat, pp, color);
};

inline void blendImage(const cv::Mat& img,
                       const cv::Mat& alpha,
                       const cv::Point& org,
                       cv::Mat background)
{
    GAPI_Assert(alpha.type() == CV_32FC1);
    GAPI_Assert(background.channels() == 3u);

    cv::Mat roi = background(cv::Rect(org, img.size()));
    cv::Mat img32f_w;
    cv::merge(std::vector<cv::Mat>(3, alpha), img32f_w);

    cv::Mat roi32f_w(roi.size(), CV_32FC3, cv::Scalar::all(1.0));
    roi32f_w -= img32f_w;

    cv::Mat img32f, roi32f;
    if (img.type() == CV_32FC3) {
        img.copyTo(img32f);
    } else {
        img.convertTo(img32f, CV_32F, 1.0/255);
    }

    roi.convertTo(roi32f, CV_32F, 1.0/255);

    cv::multiply(img32f, img32f_w, img32f);
    cv::multiply(roi32f, roi32f_w, roi32f);
    roi32f += img32f;

    roi32f.convertTo(roi, CV_8U, 255.0);
}

struct BGR2YUVConverter
{
    cv::Scalar cvtColor(const cv::Scalar& bgr) const
    {
        double y = bgr[2] *  0.299000 + bgr[1] *  0.587000 + bgr[0] *  0.114000;
        double u = bgr[2] * -0.168736 + bgr[1] * -0.331264 + bgr[0] *  0.500000 + 128;
        double v = bgr[2] *  0.500000 + bgr[1] * -0.418688 + bgr[0] * -0.081312 + 128;

        return {y, u, v};
    }

    void cvtImg(const cv::Mat& in, cv::Mat& out) { cv::cvtColor(in, out, cv::COLOR_BGR2YUV); };
};

struct EmptyConverter
{
    cv::Scalar cvtColor(const cv::Scalar& bgr)   const { return bgr; };
    void cvtImg(const cv::Mat& in, cv::Mat& out) const { out = in;   };
};

// FIXME util::visitor ?
template <typename ColorConverter>
void drawPrimitivesOCV(cv::Mat& in, const Prims& prims, cv::gapi::wip::draw::IBitmaskCreator* mc)
{
    ColorConverter converter;
    for (const auto &p : prims)
    {
        switch (p.index())
        {
            case Prim::index_of<Rect>():
            {
                const auto& rp = cv::util::get<Rect>(p);
                const auto color = converter.cvtColor(rp.color);
                cv::rectangle(in, rp.rect, color , rp.thick);
                break;
            }

            case Prim::index_of<Text>():
            {
                auto t_p = cv::util::get<Text>(p);
                t_p.color = converter.cvtColor(t_p.color);

                mc->setMaskParams(t_p);
                auto size = mc->computeMaskSize();

                // Allocate mask outside
                cv::Mat mask(size, CV_8UC1);
                int baseline = mc->createMask(mask);

                mask.convertTo(mask, CV_32FC1, 1 / 255.0);
                cv::Mat color_mask;

                cv::merge(std::vector<cv::Mat>(3, mask), color_mask);
                cv::Scalar color32f = t_p.color / 255.0;
                cv::multiply(color_mask, color32f, color_mask);

                // Org is bottom left point, trasform it to top left point for blendImage
                cv::Point tl(t_p.org.x, t_p.org.y - color_mask.size().height + baseline);

                blendImage(color_mask, mask, tl, in);

                break;
            }

            case Prim::index_of<Circle>():
            {
                const auto& cp = cv::util::get<Circle>(p);
                const auto color = converter.cvtColor(cp.color);
                cv::circle(in, cp.center, cp.radius, color, cp.thick);
                break;
            }

            case Prim::index_of<Line>():
            {
                const auto& lp = cv::util::get<Line>(p);
                const auto color = converter.cvtColor(lp.color);
                cv::line(in, lp.pt1, lp.pt2, color, lp.thick);
                break;
            }

            case Prim::index_of<Mosaic>():
            {
                const auto& mp = cv::util::get<Mosaic>(p);
                GAPI_Assert(mp.decim == 0 && "Only decim = 0 supported now");
                mosaic(in, mp.mos, mp.cellSz);
                break;
            }

            case Prim::index_of<Image>():
            {
                const auto& ip = cv::util::get<Image>(p);

                cv::Mat img;
                converter.cvtImg(ip.img, img);

                img.convertTo(img, CV_32FC1, 1.0 / 255);
                blendImage(img, ip.alpha, ip.org, in);
                break;
            }

            case Prim::index_of<Poly>():
            {
                const auto& pp = cv::util::get<Poly>(p);
                const auto color = converter.cvtColor(pp.color);
                poly(in, pp.points, color);
                break;
            }

            default: cv::util::throw_error(std::logic_error("Unsupported draw operation"));
        }
    }
}

void drawPrimitivesOCVBGR(cv::Mat &in, const Prims &prims, cv::gapi::wip::draw::IBitmaskCreator* mc)
{
    drawPrimitivesOCV<EmptyConverter>(in, prims, mc);
}

void drawPrimitivesOCVYUV(cv::Mat &in, const Prims &prims, cv::gapi::wip::draw::IBitmaskCreator* mc)
{
    drawPrimitivesOCV<BGR2YUVConverter>(in, prims, mc);
}

} // namespace draw
} // namespace wip
} // namespace gapi
} // namespace cv
