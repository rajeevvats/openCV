// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#include "test_precomp.hpp"
#include <opencv2/dnn/shape_utils.hpp>
#include "npy_blob.hpp"
namespace opencv_test { namespace {

template<typename TString>
static std::string _tf(TString filename, bool required = true)
{
    String rootFolder = "dnn/";
    return findDataFile(rootFolder + filename, required);
}


class Test_Model : public DNNTestLayer
{
public:
    void testDetectModel(const std::string& weights, const std::string& cfg,
                         const std::string& imgPath, const std::vector<int>& refClassIds,
                         const std::vector<float>& refConfidences,
                         const std::vector<Rect2d>& refBoxes,
                         double scoreDiff, double iouDiff,
                         double confThreshold = 0.24, double nmsThreshold = 0.0,
                         const Size& size = {-1, -1}, Scalar mean = Scalar(),
                         double scale = 1.0, bool swapRB = false, bool crop = false,
                         bool nmsAcrossClasses = false)
    {
        checkBackend();

        Mat frame = imread(imgPath);
        DetectionModel model(weights, cfg);

        model.setInputSize(size).setInputMean(mean).setInputScale(scale)
             .setInputSwapRB(swapRB).setInputCrop(crop);

        model.setPreferableBackend(backend);
        model.setPreferableTarget(target);

        model.setNmsAcrossClasses(nmsAcrossClasses);

        std::vector<int> classIds;
        std::vector<float> confidences;
        std::vector<Rect> boxes;

        model.detect(frame, classIds, confidences, boxes, confThreshold, nmsThreshold);

        std::vector<Rect2d> boxesDouble(boxes.size());
        for (int i = 0; i < boxes.size(); i++) {
            boxesDouble[i] = boxes[i];
        }
        normAssertDetections(refClassIds, refConfidences, refBoxes, classIds,
                             confidences, boxesDouble, "",
                             confThreshold, scoreDiff, iouDiff);
    }

    void testClassifyModel(const std::string& weights, const std::string& cfg,
                           const std::string& imgPath, std::pair<int, float> ref, float norm,
                           const Size& size = {-1, -1}, Scalar mean = Scalar(),
                           double scale = 1.0, bool swapRB = false, bool crop = false)
    {
        checkBackend();

        Mat frame = imread(imgPath);
        ClassificationModel model(weights, cfg);
        model.setInputSize(size).setInputMean(mean).setInputScale(scale)
             .setInputSwapRB(swapRB).setInputCrop(crop);

        std::pair<int, float> prediction = model.classify(frame);
        EXPECT_EQ(prediction.first, ref.first);
        ASSERT_NEAR(prediction.second, ref.second, norm);
    }

    void testKeypointsModel(const std::string& weights, const std::string& cfg,
                            const Mat& frame, const Mat& exp, float norm,
                            const Size& size = {-1, -1}, Scalar mean = Scalar(),
                            double scale = 1.0, bool swapRB = false, bool crop = false)
    {
        checkBackend();

        std::vector<Point2f> points;

        KeypointsModel model(weights, cfg);
        model.setInputSize(size).setInputMean(mean).setInputScale(scale)
             .setInputSwapRB(swapRB).setInputCrop(crop);

        model.setPreferableBackend(backend);
        model.setPreferableTarget(target);

        points = model.estimate(frame, 0.5);

        Mat out = Mat(points).reshape(1);
        normAssert(exp, out, "", norm, norm);
    }

    void testSegmentationModel(const std::string& weights_file, const std::string& config_file,
                               const std::string& inImgPath, const std::string& outImgPath,
                               float norm, const Size& size = {-1, -1}, Scalar mean = Scalar(),
                               double scale = 1.0, bool swapRB = false, bool crop = false)
    {
        checkBackend();

        Mat frame = imread(inImgPath);
        Mat mask;
        Mat exp = imread(outImgPath, 0);

        SegmentationModel model(weights_file, config_file);
        model.setInputSize(size).setInputMean(mean).setInputScale(scale)
             .setInputSwapRB(swapRB).setInputCrop(crop);

        model.segment(frame, mask);
        normAssert(mask, exp, "", norm, norm);
    }

    void testTextRecognitionModel(const std::string& weights, const std::string& cfg,
                                  const std::string& imgPath, const std::string& seq,
                                  const std::string& decodeType, const std::vector<std::string>& vocabulary,
                                  const Size& size = {-1, -1}, Scalar mean = Scalar(),
                                  double scale = 1.0, bool swapRB = false, bool crop = false)
    {
        checkBackend();

        Mat frame = imread(imgPath, IMREAD_GRAYSCALE);

        TextRecognitionModel model(weights, cfg);
        model.setDecodeType(decodeType)
             .setVocabulary(vocabulary)
             .setInputSize(size).setInputMean(mean).setInputScale(scale)
             .setInputSwapRB(swapRB).setInputCrop(crop);

        model.setPreferableBackend(backend);
        model.setPreferableTarget(target);

        std::string result = model.recognize(frame);
        EXPECT_EQ(result, seq) << "Full frame: " << imgPath;

        std::vector<Rect> rois;
        rois.push_back(Rect(0, 0, frame.cols, frame.rows));
        rois.push_back(Rect(0, 0, frame.cols, frame.rows));  // twice
        std::vector<std::string> results;
        model.recognize(frame, rois, results);
        EXPECT_EQ((size_t)2u, results.size()) << "ROI: " << imgPath;
        EXPECT_EQ(results[0], seq) << "ROI[0]: " << imgPath;
        EXPECT_EQ(results[1], seq) << "ROI[1]: " << imgPath;
    }

    void testTextDetectionModelByDB(const std::string& weights, const std::string& cfg,
                                    const std::string& imgPath, const std::vector<std::vector<Point>>& gt,
                                    float binThresh, float polyThresh,
                                    uint maxCandidates, double unclipRatio,
                                    const Size& size = {-1, -1}, Scalar mean = Scalar(),
                                    double scale = 1.0, bool swapRB = false, bool crop = false)
    {
        checkBackend();

        Mat frame = imread(imgPath);

        TextDetectionModel_DB model(weights, cfg);
        model.setBinaryThreshold(binThresh)
             .setPolygonThreshold(polyThresh)
             .setUnclipRatio(unclipRatio)
             .setMaxCandidates(maxCandidates)
             .setInputSize(size).setInputMean(mean).setInputScale(scale)
             .setInputSwapRB(swapRB).setInputCrop(crop);

        model.setPreferableBackend(backend);
        model.setPreferableTarget(target);

        // 1. Check common TextDetectionModel API through RotatedRect
        std::vector<cv::RotatedRect> results;
        model.detectTextRectangles(frame, results);

        EXPECT_GT(results.size(), (size_t)0);

        std::vector< std::vector<Point> > contours;
        for (size_t i = 0; i < results.size(); i++)
        {
            const RotatedRect& box = results[i];
            Mat contour;
            boxPoints(box, contour);
            std::vector<Point> contour2i(4);
            for (int i = 0; i < 4; i++)
            {
                contour2i[i].x = cvRound(contour.at<float>(i, 0));
                contour2i[i].y = cvRound(contour.at<float>(i, 1));
            }
            contours.push_back(contour2i);
        }
#if 0 // test debug
        Mat result = frame.clone();
        drawContours(result, contours, -1, Scalar(0, 0, 255), 1);
        imshow("result", result); // imwrite("result.png", result);
        waitKey(0);
#endif
        normAssertTextDetections(gt, contours, "", 0.05f);

        // 2. Check quadrangle-based API
        // std::vector< std::vector<Point> > contours;
        model.detect(frame, contours);

#if 0 // test debug
        Mat result = frame.clone();
        drawContours(result, contours, -1, Scalar(0, 0, 255), 1);
        imshow("result_contours", result); // imwrite("result_contours.png", result);
        waitKey(0);
#endif
        normAssertTextDetections(gt, contours, "", 0.05f);
    }

    void testTextDetectionModelByEAST(
            const std::string& weights, const std::string& cfg,
            const std::string& imgPath, const std::vector<RotatedRect>& gt,
            float confThresh, float nmsThresh,
            const Size& size = {-1, -1}, Scalar mean = Scalar(),
            double scale = 1.0, bool swapRB = false, bool crop = false,
            double eps_center = 5/*pixels*/, double eps_size = 5/*pixels*/, double eps_angle = 1
    )
    {
        checkBackend();

        Mat frame = imread(imgPath);

        TextDetectionModel_EAST model(weights, cfg);
        model.setConfidenceThreshold(confThresh)
             .setNMSThreshold(nmsThresh)
             .setInputSize(size).setInputMean(mean).setInputScale(scale)
             .setInputSwapRB(swapRB).setInputCrop(crop);

        model.setPreferableBackend(backend);
        model.setPreferableTarget(target);

        std::vector<cv::RotatedRect> results;
        model.detectTextRectangles(frame, results);

        EXPECT_EQ(results.size(), (size_t)1);
        for (size_t i = 0; i < results.size(); i++)
        {
            const RotatedRect& box = results[i];
#if 0 // test debug
            Mat contour;
            boxPoints(box, contour);
            std::vector<Point> contour2i(4);
            for (int i = 0; i < 4; i++)
            {
                contour2i[i].x = cvRound(contour.at<float>(i, 0));
                contour2i[i].y = cvRound(contour.at<float>(i, 1));
            }
            std::vector< std::vector<Point> > contours;
            contours.push_back(contour2i);

            Mat result = frame.clone();
            drawContours(result, contours, -1, Scalar(0, 0, 255), 1);
            imshow("result", result); //imwrite("result.png", result);
            waitKey(0);
#endif
            const RotatedRect& gtBox = gt[i];
            EXPECT_NEAR(box.center.x, gtBox.center.x, eps_center);
            EXPECT_NEAR(box.center.y, gtBox.center.y, eps_center);
            EXPECT_NEAR(box.size.width, gtBox.size.width, eps_size);
            EXPECT_NEAR(box.size.height, gtBox.size.height, eps_size);
            EXPECT_NEAR(box.angle, gtBox.angle, eps_angle);
        }
    }

    void testFaceDetectionModelBySSD(
        const std::string& weights,
        const std::string& cfg,
        const std::string& imgage_path,
        const std::vector<Rect>& ref_boxes,
        const std::vector<float>& ref_confidences,
        const double iou_diff,
        const double score_diff,
        const double conf_threshold = 0.2,
        const double nms_threshold = 0.4
    )
    {
        checkBackend();

        const Mat image = imread(imgage_path);

        FaceDetectionModel_SSD face_detector(weights, cfg);
        face_detector.setPreferableBackend(backend);
        face_detector.setPreferableTarget(target);

        std::vector<float> confidences;
        std::vector<Rect> boxes;
        face_detector.detect(image, confidences, boxes, conf_threshold, nms_threshold);

        std::vector<Rect2d> boxes_double(boxes.size());
        for (int i = 0; i < boxes.size(); i++)
        {
            boxes_double[i] = boxes[i];
        }
        std::vector<Rect2d> ref_boxes_double(ref_boxes.size());
        for (int i = 0; i < ref_boxes.size(); i++)
        {
            ref_boxes_double[i] = ref_boxes[i];
        }

        // In the case of face detection, class id don't exist.
        // Therefor, all give 1 to class id and reference.
        std::vector<int> class_ids(boxes.size(), 1);
        std::vector<int> ref_class_ids(ref_boxes.size(), 1);

        normAssertDetections(
            ref_class_ids, ref_confidences, ref_boxes_double,
            class_ids, confidences, boxes_double,
            "", conf_threshold, score_diff, iou_diff);
    }

    void testFaceDetectionModelByYN(
        const std::string& weights,
        const std::string& cfg,
        const std::string& imgage_path,
        const std::vector<Rect>& ref_boxes,
        const std::vector<std::vector<Point>>& ref_landmarks,
        const double iou_diff,
        const double l2d_diff,
        const double conf_threshold = 0.9,
        const double nms_threshold = 0.3
    )
    {
        CV_Assert(ref_boxes.size() == ref_landmarks.size());

        checkBackend();

        const Mat image = imread(imgage_path);

        FaceDetectionModel_YN face_detector(weights, cfg);
        face_detector.setPreferableBackend(backend);
        face_detector.setPreferableTarget(target);

        std::vector<float> confidences;
        std::vector<Rect> boxes;
        face_detector.detect(image, confidences, boxes, conf_threshold, nms_threshold);

        std::vector<Rect2d> boxes_double(boxes.size());
        for (int i = 0; i < boxes.size(); i++)
        {
            boxes_double[i] = boxes[i];
        }
        std::vector<Rect2d> ref_boxes_double(ref_boxes.size());
        for (int i = 0; i < ref_boxes.size(); i++)
        {
            ref_boxes_double[i] = ref_boxes[i];
        }

        // In the case of face detection, class id don't exist.
        // Therefor, all give 1 to class id and reference.
        std::vector<int> class_ids(boxes.size(), 1);
        std::vector<int> ref_class_ids(ref_boxes.size(), 1);

        // Not test for confidences.
        // Therefor, all give 1 to confidences and reference.
        confidences.assign(boxes.size(), 1.0f);
        std::vector<float> ref_confidences(ref_boxes.size(), 1.0f);
        const double score_diff = 1e-5;

        normAssertDetections(
            ref_class_ids, ref_confidences, ref_boxes_double,
            class_ids, confidences, boxes_double,
            "", conf_threshold, score_diff, iou_diff);

        std::vector<std::vector<Point>> landmarks;
        face_detector.getLandmarks(landmarks);

        for (int i = 0; i < landmarks.size(); i++)
        {
            normAssertLandmarkDetections(
                ref_landmarks[i], landmarks[i], "", l2d_diff);
        }
    }

    void testFaceRecognitionModelBySF(
        const std::string& face_detection_weights,
        const std::string& face_detection_cfg,
        const std::string& face_recognition_weights,
        const std::string& face_recognition_cfg,
        const std::vector<std::pair<std::string, std::string>>& image_pairs,
        const std::vector<int>& gt_labels,
        const double cos_threshold = 0.363,
        const double l2norm_threshold = 1.128
    )
    {
        CV_Assert(image_pairs.size() == gt_labels.size());

        checkBackend();

        FaceDetectionModel_YN face_detector(face_detection_weights, face_detection_cfg);
        face_detector.setPreferableBackend(backend);
        face_detector.setPreferableTarget(target);

        FaceRecognitionModel_SF face_recognizer(face_recognition_weights, face_recognition_cfg);
        face_recognizer.setPreferableBackend(backend);
        face_recognizer.setPreferableTarget(target);

        const double conf_threshold = 0.9;
        const double nms_threshold = 0.3;

        const int numPairs = image_pairs.size();
        for (int i = 0; i < numPairs; i++)
        {
            const std::string image_path1 = image_pairs[i].first;
            const std::string image_path2 = image_pairs[i].second;
            const Mat image1 = imread(image_path1);
            const Mat image2 = imread(image_path2);

            std::vector<float> confidences;
            std::vector<Rect> boxes;

            std::vector<std::vector<Point>> landmarks1;
            face_detector.detect(image1, confidences, boxes, conf_threshold, nms_threshold);
            face_detector.getLandmarks(landmarks1);
            CV_Assert(landmarks1.size() == 1);

            std::vector<std::vector<Point>> landmarks2;
            face_detector.detect(image2, confidences, boxes, conf_threshold, nms_threshold);
            face_detector.getLandmarks(landmarks2);
            CV_Assert(landmarks2.size() == 1);

            cv::Mat aligned_face1, aligned_face2;
            face_recognizer.alignCrop(image1, aligned_face1, landmarks1[0]);
            face_recognizer.alignCrop(image2, aligned_face2, landmarks2[0]);

            cv::Mat face_feature1, face_feature2;
            face_recognizer.feature(aligned_face1, face_feature1);
            face_recognizer.feature(aligned_face2, face_feature2);

            const double cos_score = face_recognizer.match(face_feature1, face_feature2, cv::dnn::FaceRecognitionModel::DisType::FR_COSINE);
            const double l2norm_score = face_recognizer.match(face_feature1, face_feature2, cv::dnn::FaceRecognitionModel::DisType::FR_NORM_L2);

            const int cos_label = (cos_score >= cos_threshold) ? 1 : 0;
            const int l2norm_label = (l2norm_score <= l2norm_threshold) ? 1 : 0;

            const int gt_label = gt_labels[i];
            EXPECT_TRUE(gt_label == cos_label);
            EXPECT_TRUE(gt_label == l2norm_label);
        }
    }
};

TEST_P(Test_Model, Classify)
{
    std::pair<int, float> ref(652, 0.641789);

    std::string img_path = _tf("grace_hopper_227.png");
    std::string config_file = _tf("bvlc_alexnet.prototxt");
    std::string weights_file = _tf("bvlc_alexnet.caffemodel", false);

    Size size{227, 227};
    float norm = 1e-4;

    testClassifyModel(weights_file, config_file, img_path, ref, norm, size);
}


TEST_P(Test_Model, DetectRegion)
{
    applyTestTag(
        CV_TEST_TAG_LONG,
        CV_TEST_TAG_MEMORY_2GB
    );

#if defined(INF_ENGINE_RELEASE) && INF_ENGINE_VER_MAJOR_EQ(2021040000)
    // accuracy
    if (backend == DNN_BACKEND_INFERENCE_ENGINE_NGRAPH && target == DNN_TARGET_OPENCL_FP16)
        applyTestTag(CV_TEST_TAG_DNN_SKIP_IE_OPENCL_FP16, CV_TEST_TAG_DNN_SKIP_IE_NGRAPH, CV_TEST_TAG_DNN_SKIP_IE_VERSION);
#endif

#if defined(INF_ENGINE_RELEASE) && INF_ENGINE_VER_MAJOR_EQ(2020040000)  // nGraph compilation failure
    if (backend == DNN_BACKEND_INFERENCE_ENGINE_NGRAPH && target == DNN_TARGET_OPENCL)
        applyTestTag(CV_TEST_TAG_DNN_SKIP_IE_OPENCL, CV_TEST_TAG_DNN_SKIP_IE_VERSION);
    if (backend == DNN_BACKEND_INFERENCE_ENGINE_NGRAPH && target == DNN_TARGET_OPENCL_FP16)
        applyTestTag(CV_TEST_TAG_DNN_SKIP_IE_OPENCL_FP16, CV_TEST_TAG_DNN_SKIP_IE_VERSION);
#endif

#if defined(INF_ENGINE_RELEASE) && INF_ENGINE_VER_MAJOR_GE(2019010000)
    // FIXIT DNN_BACKEND_INFERENCE_ENGINE is misused
    if (backend == DNN_BACKEND_INFERENCE_ENGINE && target == DNN_TARGET_OPENCL_FP16)
        applyTestTag(CV_TEST_TAG_DNN_SKIP_IE_OPENCL_FP16);
#endif

#if defined(INF_ENGINE_RELEASE)
    if (target == DNN_TARGET_MYRIAD
        && getInferenceEngineVPUType() == CV_DNN_INFERENCE_ENGINE_VPU_TYPE_MYRIAD_X)
        applyTestTag(CV_TEST_TAG_DNN_SKIP_IE_MYRIAD_X);
#endif

    std::vector<int> refClassIds = {6, 1, 11};
    std::vector<float> refConfidences = {0.750469f, 0.780879f, 0.901615f};
    std::vector<Rect2d> refBoxes = {Rect2d(240, 53, 135, 72),
                                    Rect2d(112, 109, 192, 200),
                                    Rect2d(58, 141, 117, 249)};

    std::string img_path = _tf("dog416.png");
    std::string weights_file = _tf("yolo-voc.weights", false);
    std::string config_file = _tf("yolo-voc.cfg");

    double scale = 1.0 / 255.0;
    Size size{416, 416};
    bool swapRB = true;

    double confThreshold = 0.24;
    double nmsThreshold = (target == DNN_TARGET_MYRIAD) ? 0.397 : 0.4;
    double scoreDiff = 8e-5, iouDiff = 1e-5;
    if (target == DNN_TARGET_OPENCL_FP16 || target == DNN_TARGET_MYRIAD || target == DNN_TARGET_CUDA_FP16)
    {
        scoreDiff = 1e-2;
        iouDiff = 1.6e-2;
    }

    testDetectModel(weights_file, config_file, img_path, refClassIds, refConfidences,
                    refBoxes, scoreDiff, iouDiff, confThreshold, nmsThreshold, size,
                    Scalar(), scale, swapRB);
}

TEST_P(Test_Model, DetectRegionWithNmsAcrossClasses)
{
    applyTestTag(
        CV_TEST_TAG_LONG,
        CV_TEST_TAG_MEMORY_2GB
    );

#if defined(INF_ENGINE_RELEASE) && INF_ENGINE_VER_MAJOR_EQ(2021040000)
    // accuracy
    if (backend == DNN_BACKEND_INFERENCE_ENGINE_NGRAPH && target == DNN_TARGET_OPENCL_FP16)
        applyTestTag(CV_TEST_TAG_DNN_SKIP_IE_OPENCL_FP16, CV_TEST_TAG_DNN_SKIP_IE_NGRAPH, CV_TEST_TAG_DNN_SKIP_IE_VERSION);
#endif

#if defined(INF_ENGINE_RELEASE) && INF_ENGINE_VER_MAJOR_EQ(2020040000)  // nGraph compilation failure
    if (backend == DNN_BACKEND_INFERENCE_ENGINE_NGRAPH && target == DNN_TARGET_OPENCL)
        applyTestTag(CV_TEST_TAG_DNN_SKIP_IE_OPENCL, CV_TEST_TAG_DNN_SKIP_IE_VERSION);
    if (backend == DNN_BACKEND_INFERENCE_ENGINE_NGRAPH && target == DNN_TARGET_OPENCL_FP16)
        applyTestTag(CV_TEST_TAG_DNN_SKIP_IE_OPENCL_FP16, CV_TEST_TAG_DNN_SKIP_IE_VERSION);
#endif

#if defined(INF_ENGINE_RELEASE) && INF_ENGINE_VER_MAJOR_GE(2019010000)
    if (backend == DNN_BACKEND_INFERENCE_ENGINE && target == DNN_TARGET_OPENCL_FP16)
        applyTestTag(CV_TEST_TAG_DNN_SKIP_IE_OPENCL_FP16);
#endif

#if defined(INF_ENGINE_RELEASE)
    if (target == DNN_TARGET_MYRIAD
        && getInferenceEngineVPUType() == CV_DNN_INFERENCE_ENGINE_VPU_TYPE_MYRIAD_X)
        applyTestTag(CV_TEST_TAG_DNN_SKIP_IE_MYRIAD_X);
#endif

    std::vector<int> refClassIds = { 6, 11 };
    std::vector<float> refConfidences = { 0.750469f, 0.901615f };
    std::vector<Rect2d> refBoxes = { Rect2d(240, 53, 135, 72),
                                    Rect2d(58, 141, 117, 249) };

    std::string img_path = _tf("dog416.png");
    std::string weights_file = _tf("yolo-voc.weights", false);
    std::string config_file = _tf("yolo-voc.cfg");

    double scale = 1.0 / 255.0;
    Size size{ 416, 416 };
    bool swapRB = true;
    bool crop = false;
    bool nmsAcrossClasses = true;

    double confThreshold = 0.24;
    double nmsThreshold = (target == DNN_TARGET_MYRIAD) ? 0.15: 0.15;
    double scoreDiff = 8e-5, iouDiff = 1e-5;
    if (target == DNN_TARGET_OPENCL_FP16 || target == DNN_TARGET_MYRIAD || target == DNN_TARGET_CUDA_FP16)
    {
        scoreDiff = 1e-2;
        iouDiff = 1.6e-2;
    }

    testDetectModel(weights_file, config_file, img_path, refClassIds, refConfidences,
        refBoxes, scoreDiff, iouDiff, confThreshold, nmsThreshold, size,
        Scalar(), scale, swapRB, crop,
        nmsAcrossClasses);
}

TEST_P(Test_Model, DetectionOutput)
{
#if defined(INF_ENGINE_RELEASE) && INF_ENGINE_VER_MAJOR_EQ(2021040000)
    // Exception: Function contains several inputs and outputs with one friendly name! (HETERO bug?)
    if (backend == DNN_BACKEND_INFERENCE_ENGINE_NGRAPH && target != DNN_TARGET_CPU)
        applyTestTag(CV_TEST_TAG_DNN_SKIP_IE_NGRAPH, CV_TEST_TAG_DNN_SKIP_IE_VERSION);
#endif

#if defined(INF_ENGINE_RELEASE)
    // FIXIT DNN_BACKEND_INFERENCE_ENGINE is misused
    if (backend == DNN_BACKEND_INFERENCE_ENGINE && target == DNN_TARGET_OPENCL_FP16)
        applyTestTag(CV_TEST_TAG_DNN_SKIP_IE_OPENCL_FP16);

    if (target == DNN_TARGET_MYRIAD)
        applyTestTag(CV_TEST_TAG_DNN_SKIP_IE_MYRIAD);
#endif

    std::vector<int> refClassIds = {7, 12};
    std::vector<float> refConfidences = {0.991359f, 0.94786f};
    std::vector<Rect2d> refBoxes = {Rect2d(491, 81, 212, 98),
                                    Rect2d(132, 223, 207, 344)};

    std::string img_path = _tf("dog416.png");
    std::string weights_file = _tf("resnet50_rfcn_final.caffemodel", false);
    std::string config_file = _tf("rfcn_pascal_voc_resnet50.prototxt");

    Scalar mean = Scalar(102.9801, 115.9465, 122.7717);
    Size size{800, 600};

    double scoreDiff = default_l1, iouDiff = 1e-5;
    float confThreshold = 0.8;
    double nmsThreshold = 0.0;
    if (target == DNN_TARGET_OPENCL_FP16 || target == DNN_TARGET_CUDA_FP16)
    {
        if (backend == DNN_BACKEND_OPENCV)
            scoreDiff = 4e-3;
        else
            scoreDiff = 2e-2;
        iouDiff = 1.8e-1;
    }

    testDetectModel(weights_file, config_file, img_path, refClassIds, refConfidences, refBoxes,
                    scoreDiff, iouDiff, confThreshold, nmsThreshold, size, mean);
}


TEST_P(Test_Model, DetectionMobilenetSSD)
{
    Mat ref = blobFromNPY(_tf("mobilenet_ssd_caffe_out.npy"));
    ref = ref.reshape(1, ref.size[2]);

    std::string img_path = _tf("street.png");
    Mat frame = imread(img_path);
    int frameWidth  = frame.cols;
    int frameHeight = frame.rows;

    std::vector<int> refClassIds;
    std::vector<float> refConfidences;
    std::vector<Rect2d> refBoxes;
    for (int i = 0; i < ref.rows; i++)
    {
        refClassIds.emplace_back(ref.at<float>(i, 1));
        refConfidences.emplace_back(ref.at<float>(i, 2));
        int left   = ref.at<float>(i, 3) * frameWidth;
        int top    = ref.at<float>(i, 4) * frameHeight;
        int right  = ref.at<float>(i, 5) * frameWidth;
        int bottom = ref.at<float>(i, 6) * frameHeight;
        int width  = right  - left + 1;
        int height = bottom - top + 1;
        refBoxes.emplace_back(left, top, width, height);
    }

    std::string weights_file = _tf("MobileNetSSD_deploy.caffemodel", false);
    std::string config_file = _tf("MobileNetSSD_deploy.prototxt");

    Scalar mean = Scalar(127.5, 127.5, 127.5);
    double scale = 1.0 / 127.5;
    Size size{300, 300};

    double scoreDiff = 1e-5, iouDiff = 1e-5;
    if (target == DNN_TARGET_OPENCL_FP16)
    {
        scoreDiff = 1.7e-2;
        iouDiff = 6.91e-2;
    }
    else if (target == DNN_TARGET_MYRIAD)
    {
        scoreDiff = 0.017;
        if (getInferenceEngineVPUType() == CV_DNN_INFERENCE_ENGINE_VPU_TYPE_MYRIAD_X)
            iouDiff = 0.1;
    }
    else if (target == DNN_TARGET_CUDA_FP16)
    {
        scoreDiff = 0.002;
        iouDiff = 1e-2;
    }
    float confThreshold = FLT_MIN;
    double nmsThreshold = 0.0;

    testDetectModel(weights_file, config_file, img_path, refClassIds, refConfidences, refBoxes,
                    scoreDiff, iouDiff, confThreshold, nmsThreshold, size, mean, scale);
}

TEST_P(Test_Model, Keypoints_pose)
{
    if (target == DNN_TARGET_OPENCL_FP16)
        applyTestTag(CV_TEST_TAG_DNN_SKIP_OPENCL_FP16);
#ifdef HAVE_INF_ENGINE
    if (target == DNN_TARGET_MYRIAD)
        applyTestTag(CV_TEST_TAG_DNN_SKIP_IE_MYRIAD, CV_TEST_TAG_DNN_SKIP_IE_VERSION);
#endif

    Mat inp = imread(_tf("pose.png"));
    std::string weights = _tf("onnx/models/lightweight_pose_estimation_201912.onnx", false);
    float kpdata[] = {
        237.65625f, 78.25f, 237.65625f, 136.9375f,
        190.125f, 136.9375f, 142.59375f, 195.625f, 79.21875f, 176.0625f, 285.1875f, 117.375f,
        348.5625f, 195.625f, 396.09375f, 176.0625f, 205.96875f, 313.0f, 205.96875f, 430.375f,
        205.96875f, 528.1875f, 269.34375f, 293.4375f, 253.5f, 430.375f, 237.65625f, 528.1875f,
        221.8125f, 58.6875f, 253.5f, 58.6875f, 205.96875f, 78.25f, 253.5f, 58.6875f
    };
    Mat exp(18, 2, CV_32FC1, kpdata);

    Size size{256, 256};
    float norm = 1e-4;
    double scale = 1.0/255;
    Scalar mean = Scalar(128, 128, 128);
    bool swapRB = false;

    // Ref. Range: [58.6875, 508.625]
    if (target == DNN_TARGET_CUDA_FP16)
        norm = 20; // l1 = 1.5, lInf = 20

    testKeypointsModel(weights, "", inp, exp, norm, size, mean, scale, swapRB);
}

TEST_P(Test_Model, Keypoints_face)
{
#if defined(INF_ENGINE_RELEASE)
    if (backend == DNN_BACKEND_INFERENCE_ENGINE_NN_BUILDER_2019)
        applyTestTag(CV_TEST_TAG_DNN_SKIP_IE_NN_BUILDER, CV_TEST_TAG_DNN_SKIP_IE_VERSION);
#endif

    Mat inp = imread(_tf("gray_face.png"), 0);
    std::string weights = _tf("onnx/models/facial_keypoints.onnx", false);
    Mat exp = blobFromNPY(_tf("facial_keypoints_exp.npy"));

    Size size{224, 224};
    double scale = 1.0/255;
    Scalar mean = Scalar();
    bool swapRB = false;

    // Ref. Range: [-1.1784188, 1.7758257]
    float norm = 1e-4;
    if (target == DNN_TARGET_OPENCL_FP16)
        norm = 5e-3;
    if (target == DNN_TARGET_MYRIAD)
    {
        // Myriad2: l1 = 0.0004, lInf = 0.002
        // MyriadX: l1 = 0.003, lInf = 0.009
        norm = 0.009;
    }
    if (target == DNN_TARGET_CUDA_FP16)
        norm = 0.004; // l1 = 0.0006, lInf = 0.004

    testKeypointsModel(weights, "", inp, exp, norm, size, mean, scale, swapRB);
}

TEST_P(Test_Model, Detection_normalized)
{
    std::string img_path = _tf("grace_hopper_227.png");
    std::vector<int> refClassIds = {15};
    std::vector<float> refConfidences = {0.999222f};
    std::vector<Rect2d> refBoxes = {Rect2d(0, 4, 227, 222)};

    std::string weights_file = _tf("MobileNetSSD_deploy.caffemodel", false);
    std::string config_file = _tf("MobileNetSSD_deploy.prototxt");

    Scalar mean = Scalar(127.5, 127.5, 127.5);
    double scale = 1.0 / 127.5;
    Size size{300, 300};

    double scoreDiff = 1e-5, iouDiff = 1e-5;
    float confThreshold = FLT_MIN;
    double nmsThreshold = 0.0;
    if (target == DNN_TARGET_CUDA)
    {
        scoreDiff = 3e-4;
        iouDiff = 0.018;
    }
    if (target == DNN_TARGET_OPENCL_FP16 || target == DNN_TARGET_MYRIAD || target == DNN_TARGET_CUDA_FP16)
    {
        scoreDiff = 5e-3;
        iouDiff = 0.09;
    }
#if defined(INF_ENGINE_RELEASE) && INF_ENGINE_VER_MAJOR_GE(2020040000)
    if (backend == DNN_BACKEND_INFERENCE_ENGINE_NGRAPH && target == DNN_TARGET_MYRIAD)
    {
        scoreDiff = 0.02;
        iouDiff = 0.1f;
    }
#endif
    testDetectModel(weights_file, config_file, img_path, refClassIds, refConfidences, refBoxes,
                    scoreDiff, iouDiff, confThreshold, nmsThreshold, size, mean, scale);
}

TEST_P(Test_Model, Segmentation)
{
    applyTestTag(
        CV_TEST_TAG_MEMORY_2GB
    );

    std::string inp = _tf("dog416.png");
    std::string weights_file = _tf("fcn8s-heavy-pascal.prototxt");
    std::string config_file = _tf("fcn8s-heavy-pascal.caffemodel", false);
    std::string exp = _tf("segmentation_exp.png");

    Size size{128, 128};
    float norm = 0;
    double scale = 1.0;
    Scalar mean = Scalar();
    bool swapRB = false;

    testSegmentationModel(weights_file, config_file, inp, exp, norm, size, mean, scale, swapRB);
}

TEST_P(Test_Model, TextRecognition)
{
#if defined(INF_ENGINE_RELEASE) && INF_ENGINE_VER_MAJOR_EQ(2021040000)
    // IE Exception: Ngraph operation Reshape with name 71 has dynamic output shape on 0 port, but CPU plug-in supports only static shape
    if (backend == DNN_BACKEND_INFERENCE_ENGINE_NGRAPH && (target == DNN_TARGET_OPENCL || target == DNN_TARGET_OPENCL_FP16))
        applyTestTag(target == DNN_TARGET_OPENCL ? CV_TEST_TAG_DNN_SKIP_IE_OPENCL : CV_TEST_TAG_DNN_SKIP_IE_OPENCL_FP16,
            CV_TEST_TAG_DNN_SKIP_IE_NGRAPH, CV_TEST_TAG_DNN_SKIP_IE_VERSION
        );
#endif

    std::string imgPath = _tf("text_rec_test.png");
    std::string weightPath = _tf("onnx/models/crnn.onnx", false);
    std::string seq = "welcome";

    Size size{100, 32};
    double scale = 1.0 / 127.5;
    Scalar mean = Scalar(127.5);
    std::string decodeType = "CTC-greedy";
    std::vector<std::string> vocabulary = {"0","1","2","3","4","5","6","7","8","9",
                                           "a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z"};

    testTextRecognitionModel(weightPath, "", imgPath, seq, decodeType, vocabulary, size, mean, scale);
}

TEST_P(Test_Model, TextRecognitionWithCTCPrefixBeamSearch)
{
#if defined(INF_ENGINE_RELEASE) && INF_ENGINE_VER_MAJOR_EQ(2021040000)
    // IE Exception: Ngraph operation Reshape with name 71 has dynamic output shape on 0 port, but CPU plug-in supports only static shape
    if (backend == DNN_BACKEND_INFERENCE_ENGINE_NGRAPH && (target == DNN_TARGET_OPENCL || target == DNN_TARGET_OPENCL_FP16))
        applyTestTag(target == DNN_TARGET_OPENCL ? CV_TEST_TAG_DNN_SKIP_IE_OPENCL : CV_TEST_TAG_DNN_SKIP_IE_OPENCL_FP16,
            CV_TEST_TAG_DNN_SKIP_IE_NGRAPH, CV_TEST_TAG_DNN_SKIP_IE_VERSION
        );
#endif


    std::string imgPath = _tf("text_rec_test.png");
    std::string weightPath = _tf("onnx/models/crnn.onnx", false);
    std::string seq = "welcome";

    Size size{100, 32};
    double scale = 1.0 / 127.5;
    Scalar mean = Scalar(127.5);
    std::string decodeType = "CTC-prefix-beam-search";
    std::vector<std::string> vocabulary = {"0","1","2","3","4","5","6","7","8","9",
                                           "a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z"};

    testTextRecognitionModel(weightPath, "", imgPath, seq, decodeType, vocabulary, size, mean, scale);
}

TEST_P(Test_Model, TextDetectionByDB)
{
    if (target == DNN_TARGET_OPENCL_FP16)
        applyTestTag(CV_TEST_TAG_DNN_SKIP_OPENCL_FP16);

    std::string imgPath = _tf("text_det_test1.png");
    std::string weightPath = _tf("onnx/models/DB_TD500_resnet50.onnx", false);

    // GroundTruth
    std::vector<std::vector<Point>> gt = {
        { Point(142, 193), Point(136, 164), Point(213, 150), Point(219, 178) },
        { Point(136, 165), Point(122, 114), Point(319, 71), Point(330, 122) }
    };

    Size size{736, 736};
    double scale = 1.0 / 255.0;
    Scalar mean = Scalar(122.67891434, 116.66876762, 104.00698793);

    float binThresh = 0.3;
    float polyThresh = 0.5;
    uint maxCandidates = 200;
    double unclipRatio = 2.0;

    testTextDetectionModelByDB(weightPath, "", imgPath, gt, binThresh, polyThresh, maxCandidates, unclipRatio, size, mean, scale);
}

TEST_P(Test_Model, TextDetectionByEAST)
{
    std::string imgPath = _tf("text_det_test2.jpg");
    std::string weightPath = _tf("frozen_east_text_detection.pb", false);

    // GroundTruth
    std::vector<RotatedRect> gt = {
        RotatedRect(Point2f(657.55f, 409.5f), Size2f(316.84f, 62.45f), -4.79)
    };

    // Model parameters
    Size size{320, 320};
    double scale = 1.0;
    Scalar mean = Scalar(123.68, 116.78, 103.94);
    bool swapRB = true;

    // Detection algorithm parameters
    float confThresh = 0.5;
    float nmsThresh = 0.4;

    double eps_center = 5/*pixels*/;
    double eps_size = 5/*pixels*/;
    double eps_angle = 1;

    if (target == DNN_TARGET_OPENCL_FP16 || target == DNN_TARGET_CUDA_FP16 || target == DNN_TARGET_MYRIAD)
    {
        eps_center = 10;
        eps_size = 25;
        eps_angle = 3;
    }

    testTextDetectionModelByEAST(weightPath, "", imgPath, gt, confThresh, nmsThresh, size, mean, scale, swapRB, false/*crop*/,
        eps_center, eps_size, eps_angle
    );
}

TEST_P(Test_Model, FaceDetectionBySSD)
{
    if (target == DNN_TARGET_OPENCL_FP16)
        applyTestTag(CV_TEST_TAG_DNN_SKIP_OPENCL_FP16);

    // Weight
    std::string face_detection_weight_path = _tf("opencv_face_detector.caffemodel", false);
    std::string face_detection_config_path = _tf("opencv_face_detector.prototxt", false);

    // Ground Truth
    std::string image_path = findDataFile("gpu/lbpcascade/er.png");

    std::vector<Rect> gt_boxes;
    gt_boxes.push_back(Rect(408, 84, 34, 49));
    gt_boxes.push_back(Rect(142, 256, 32, 43));
    gt_boxes.push_back(Rect(118, 46, 34, 45));
    gt_boxes.push_back(Rect(340, 34, 29, 42));
    gt_boxes.push_back(Rect(65, 123, 37, 43));
    gt_boxes.push_back(Rect(259, 228, 32, 43));

    std::vector<float> gt_confidences;
    gt_confidences.push_back(0.672344f);
    gt_confidences.push_back(0.665494f);
    gt_confidences.push_back(0.646890f);
    gt_confidences.push_back(0.388749f);
    gt_confidences.push_back(0.283762f);
    gt_confidences.push_back(0.256311f);

    // Threshold
    const double iouDiff = 0.9;
    const double scoreDiff = 2e-4;

    // Run Test
    testFaceDetectionModelBySSD(
        face_detection_weight_path,
        face_detection_config_path,
        image_path,
        gt_boxes,
        gt_confidences,
        iouDiff,
        scoreDiff
    );
}

TEST_P(Test_Model, FaceDetectionByYN)
{
    if (target == DNN_TARGET_OPENCL_FP16)
        applyTestTag(CV_TEST_TAG_DNN_SKIP_OPENCL_FP16);

    // Weight
    std::string face_detection_weight_path = _tf("onnx/models/yunet-202109.onnx", false);

    // Ground Truth
    std::string test_labels = findDataFile("cv/dnn_face/detection/cascades_labels.txt");
    std::ifstream ifs(test_labels.c_str());
    CV_Assert(ifs.is_open());

    std::string image_path;
    {
        std::string line;
        getline(ifs, line);
        std::istringstream iss(line);
        iss >> image_path;
    }
    CV_Assert(!image_path.empty());
    image_path = findDataFile("cv/cascadeandhog/images/" + image_path);

    int num_faces = -1;
    {
        std::string line;
        getline(ifs, line);
        std::istringstream iss(line);
        iss >> num_faces;
    }
    CV_Assert(0 < num_faces);

    std::vector<Rect> gt_boxes;
    std::vector<std::vector<Point>> gt_landmarks;
    {
        for (int i = 0; i < num_faces; i++)
        {
            std::string line, part;
            getline(ifs, line);
            std::stringstream ss{ line };

            int box[4];
            for (int j = 0; j < 4; j++)
            {
                getline(ss, part, ' ');
                std::istringstream iss(part);
                iss >> box[j];
            }
            gt_boxes.push_back(Rect(box[0], box[1], box[2], box[3]));

            std::vector<Point> points;
            for(int j = 4; j < 14; j+=2)
            {
                int x, y;
                getline(ss, part, ' ');
                std::istringstream iss_x(part);
                iss_x >> x;
                getline(ss, part, ' ');
                std::istringstream iss_y(part);
                iss_y >> y;
                points.push_back(Point(x, y));
            }
            gt_landmarks.push_back(points);
        }

        // There are 7 faces in the addams-family.png, but yunet-202109.onnx can't detect the last one.
        // Therefore, it is removed from ground truth data.
        gt_boxes.pop_back();
        gt_landmarks.pop_back();
    }

    // Threshold
    const double iouDiff = 0.9;
    const double l2dDiff = 5.0;

    // Run Test
    testFaceDetectionModelByYN(
        face_detection_weight_path,
        "",
        image_path,
        gt_boxes,
        gt_landmarks,
        iouDiff,
        l2dDiff
    );
}

TEST_P(Test_Model, FaceRecognitionBySF)
{
    if (target == DNN_TARGET_OPENCL_FP16)
        applyTestTag(CV_TEST_TAG_DNN_SKIP_OPENCL_FP16);

    // Weight
    std::string face_detection_weight_path = _tf("onnx/models/yunet-202109.onnx", false);
    std::string face_recognition_weight_path = _tf("onnx/models/face_recognizer_fast.onnx", false);

    // Ground Truth
    std::string test_labels = findDataFile("cv/dnn_face/recognition/cascades_label.txt");
    std::ifstream ifs(test_labels.c_str());
    CV_Assert(ifs.is_open());

    std::vector<std::pair<std::string, std::string>> image_path_pairs; // image1_path, image2_path
    std::vector<int> gt_labels; // labels (1:same identity, 0:different identities)
    {
        for(std::string line, key; getline(ifs, line);)
        {
            std::string image1_path, image2_path;
            int label;
            std::istringstream iss(line);
            iss >> image1_path >> image2_path >> label;
            image1_path = findDataFile("cv/dnn_face/recognition/" + image1_path);
            image2_path = findDataFile("cv/dnn_face/recognition/" + image2_path);
            image_path_pairs.push_back(std::make_pair(image1_path, image2_path));
            gt_labels.push_back(label);
        }
    }

    // Run Test
    testFaceRecognitionModelBySF(
        face_detection_weight_path,
        "",
        face_recognition_weight_path,
        "",
        image_path_pairs,
        gt_labels
    );
}

INSTANTIATE_TEST_CASE_P(/**/, Test_Model, dnnBackendsAndTargets());

}} // namespace
