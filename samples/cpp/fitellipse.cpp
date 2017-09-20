/********************************************************************************
 *
 *
 *  This program is demonstration for ellipse fitting. Program finds
 *  contours and approximate it by ellipses using three methods.
 *  1: OpenCV's original method fitEllipse which implements Fitzgibbon 1995 method.
 *  2: The Approximate Mean Square (AMS) method fitEllipseAMS  proposed by Taubin 1991
 *  3: The Direct least square (Direct) method fitEllipseDirect proposed by Fitzgibbon1999.
 *
 *  Trackbar specify threshold parameter.
 *
 *  White lines is contours/input points and the true ellipse used to generate the data.
 *  1: Blue lines is fitting ellipses using openCV's original method.
 *  2: Green lines is fitting ellipses using the AMS method.
 *  3: Red lines is fitting ellipses using the Direct method.
 *
 *
 *  Original Autor:  Denis Burenkov
 *  AMS and Direct Methods Autor:  Jasper Shemilt
 *
 *
 ********************************************************************************/
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>

using namespace cv;
using namespace std;

static void help()
{
    cout <<
    "\nThis program is demonstration for ellipse fitting. The program finds\n"
    "contours and approximate it by ellipses. Three methods are used to find the \n"
    "elliptical fits: fitEllipse, fitEllipseAMS and fitEllipseDirect.\n"
    "Call:\n"
    "./fitellipse [image_name -- Default ../data/stuff.jpg]\n" << endl;
}

int sliderPos = 70;

Mat image;

bool fitEllipseQ, fitEllipseAMSQ, fitEllipseDirectQ;
cv::Scalar fitEllipseColor       = Scalar(255,  0,  0);
cv::Scalar fitEllipseAMSColor    = Scalar(  0,255,  0);
cv::Scalar fitEllipseDirectColor = Scalar(  0,  0,255);
cv::Scalar fitEllipseTrueColor   = Scalar(255,255,255);

void drawEllipseWithBox(cv::Mat cimage, cv::RotatedRect box, cv::Scalar color, int lineThickness);

cv::Mat canvasFromPoints(vector<Point2f> pts);
void drawPoints(cv::Mat canvas, vector<Point2f> pts, cv::Scalar color);
cv::Mat drawPoints(vector<Point2f> pts, cv::Scalar color);
cv::Mat drawLabels(cv::Mat canvas);

void processImage(int, void*);

void test1();
void test2();
void test3();
void test4();
void test5();
void test6();
void test7();
void test_Issue_6544();

int main( int argc, char** argv )
{
    test1();
    test2();
    test3();
    test4();
    test5();
    test6();
    test7();
    test_Issue_6544();

    fitEllipseQ       = true;
    fitEllipseAMSQ    = true;
    fitEllipseDirectQ = true;

    cv::CommandLineParser parser(argc, argv,
                                 "{help h||}{@image|../data/stuff.jpg|}"
                                 );
    if (parser.has("help"))
    {
        help();
        return 0;
    }
    string filename = parser.get<string>("@image");
    image = imread(filename, 0);
    if( image.empty() )
    {
        cout << "Couldn't open image " << filename << "\n";
        return 0;
    }

    imshow("source", image);
    namedWindow("result", CV_WINDOW_NORMAL );

    // Create toolbars. HighGUI use.
    createTrackbar( "threshold", "result", &sliderPos, 255, processImage );

    processImage(0, 0);

    // Wait for a key stroke; the same function arranges events processing
    waitKey();
    return 0;
}

void drawEllipseWithBox(cv::Mat cimage, cv::RotatedRect box, cv::Scalar color, int lineThickness)
{
    ellipse(cimage, box, color, lineThickness, LINE_AA);
    //       ellipse(cimage, box.center, box.size*0.5f, box.angle, 0, 360, color, 1, LINE_AA);

    Point2f vtx[4];
    box.points(vtx);
    for( int j = 0; j < 4; j++ ){
        line(cimage, vtx[j], vtx[(j+1)%4], color, lineThickness, LINE_AA);
    }
}

// Define trackbar callback functon. This function find contours,
// draw it and approximate it by ellipses.
void processImage(int /*h*/, void*)
{

    RotatedRect box, boxAMS, boxDirect;
    vector<vector<Point> > contours;
    Mat bimage = image >= sliderPos;

    findContours(bimage, contours, RETR_LIST, CHAIN_APPROX_NONE);

    Mat cimage = Mat::zeros(bimage.size(), CV_8UC3);

    Size textsize1 = getTextSize("OpenCV", FONT_HERSHEY_COMPLEX, 1, 1, 0);
    Size textsize2 = getTextSize("AMS", FONT_HERSHEY_COMPLEX, 1, 1, 0);
    Size textsize3 = getTextSize("Direct", FONT_HERSHEY_COMPLEX, 1, 1, 0);
    Point org1((cimage.cols - textsize1.width), (int)(1.3 * textsize1.height));
    Point org2((cimage.cols - textsize2.width), (int)(1.3 * textsize1.height + 1.3 * textsize2.height));
    Point org3((cimage.cols - textsize3.width), (int)(1.3 * textsize1.height + 1.3 * textsize2.height + 1.3 * textsize3.height));

    putText(cimage, "OpenCV", org1, FONT_HERSHEY_COMPLEX, 1, fitEllipseColor, 1, LINE_8);
    putText(cimage, "AMS",    org2, FONT_HERSHEY_COMPLEX, 1, fitEllipseAMSColor, 1, LINE_8);
    putText(cimage, "Direct", org3, FONT_HERSHEY_COMPLEX, 1, fitEllipseDirectColor, 1, LINE_8);

    for(size_t i = 0; i < contours.size(); i++)
    {
        size_t count = contours[i].size();
        if( count < 6 )
            continue;

        Mat pointsf;
        Mat(contours[i]).convertTo(pointsf, CV_32F);
        if (fitEllipseQ) {
            box = fitEllipse(pointsf);
            if( MAX(box.size.width, box.size.height) > MIN(box.size.width, box.size.height)*30 ){continue;};
        }
        if (fitEllipseAMSQ) {
            boxAMS = fitEllipseAMS(pointsf);
            if( MAX(boxAMS.size.width, boxAMS.size.height) > MIN(boxAMS.size.width, boxAMS.size.height)*30 ){continue;};
        }
        if (fitEllipseDirectQ) {
            boxDirect = fitEllipseDirect(pointsf);
            if( MAX(boxDirect.size.width, boxDirect.size.height) > MIN(boxDirect.size.width, boxDirect.size.height)*30 ){continue;};
        }

        drawContours(cimage, contours, (int)i, Scalar::all(255), 1, 8);

        if (fitEllipseQ) {
            drawEllipseWithBox(cimage, box, fitEllipseColor, 1);
        }
        if (fitEllipseAMSQ) {
            drawEllipseWithBox(cimage, boxAMS, fitEllipseAMSColor, 1);
        }
        if (fitEllipseDirectQ) {
            drawEllipseWithBox(cimage, boxDirect, fitEllipseDirectColor, 1);
        }

    }

    imshow("result", cimage);
}

cv::Mat canvasFromPoints(vector<Point2f> pts){
    int rows=0,cols=0;
    for(size_t i=0; i < pts.size(); i++){
        Point2f pnt = pts[i];
        if(cols<pnt.x){cols = int(pnt.x);};
        if(rows<pnt.y){rows = int(pnt.y);};
    };
    cols = int(1.3 * cols);
    rows = int(1.3 * rows);

    cv::Mat canvas = cv::Mat::zeros(rows,cols,CV_8UC3);
    return canvas;
}

void drawPoints(cv::Mat canvas, vector<Point2f> pts, cv::Scalar color){

    for(size_t i=0; i < pts.size(); i++){
        Point2f pnt = pts[i];
        canvas.at<cv::Vec3b>(int(pnt.y), int(pnt.x))[0] = (uchar)color[0];
        canvas.at<cv::Vec3b>(int(pnt.y), int(pnt.x))[1] = (uchar)color[1];
        canvas.at<cv::Vec3b>(int(pnt.y), int(pnt.x))[2] = (uchar)color[2];
    };
}

cv::Mat drawPoints(vector<Point2f> pts, cv::Scalar color){

    cv::Mat canvas = canvasFromPoints(pts);
    drawPoints(canvas, pts, color);
    return canvas;
}

cv::Mat drawLabels(cv::Mat canvas){

    Size textsize1 = getTextSize("Old openCV", FONT_HERSHEY_COMPLEX, 1, 1, 0);
    Size textsize2 = getTextSize("AMS", FONT_HERSHEY_COMPLEX, 1, 1, 0);
    Size textsize3 = getTextSize("Direct", FONT_HERSHEY_COMPLEX, 1, 1, 0);
    Size textsize4 = getTextSize("True", FONT_HERSHEY_COMPLEX, 1, 1, 0);
    Point org1((canvas.cols - textsize1.width), (int)(1.3 * textsize1.height));
    Point org2((canvas.cols - textsize2.width), (int)(1.3 * textsize1.height + 1.3 * textsize2.height));
    Point org3((canvas.cols - textsize3.width), (int)(1.3 * textsize1.height + 1.3 * textsize2.height + 1.3 * textsize3.height));
    Point org4((canvas.cols - textsize4.width), (int)(1.3 * textsize1.height + 1.3 * textsize2.height + 1.3 * textsize3.height + 1.3 * textsize4.height));

    putText(canvas, "Old openCV", org1, FONT_HERSHEY_COMPLEX, 1, fitEllipseColor, 1, LINE_8);
    putText(canvas, "AMS",    org2, FONT_HERSHEY_COMPLEX, 1, fitEllipseAMSColor, 1, LINE_8);
    putText(canvas, "Direct", org3, FONT_HERSHEY_COMPLEX, 1, fitEllipseDirectColor, 1, LINE_8);
    putText(canvas, "True",   org4, FONT_HERSHEY_COMPLEX, 1, fitEllipseTrueColor, 1, LINE_8);
    return canvas;
}

void test1() {
    vector<Point2f>pts;
    pts.push_back(Point2f(173.41854895999165f, 125.84473135880411f));
    pts.push_back(Point2f(180.63769498640912f, 130.960006577589f));
    pts.push_back(Point2f(174.99173759130173f, 137.34265632926764f));
    pts.push_back(Point2f(170.9044645313217f, 141.68017556480243f));
    pts.push_back(Point2f(163.48965388499656f, 141.9404438924043f));
    pts.push_back(Point2f(159.37687818401147f, 148.60835331594876f));
    pts.push_back(Point2f(150.38917629356735f, 155.68825577720446f));
    pts.push_back(Point2f(147.16319653316862f, 157.06039984963923f));
    pts.push_back(Point2f(141.73118707843207f, 157.2570155198414f));
    pts.push_back(Point2f(130.61569602948597f, 159.40742182929364f));
    pts.push_back(Point2f(127.00573042229027f, 161.34430232187867f));
    pts.push_back(Point2f(120.49383815053747f, 163.72610883128334f));
    pts.push_back(Point2f(114.62383760040998f, 162.6788666385239f));
    pts.push_back(Point2f(108.84871269183333f, 161.90597054388132f));
    pts.push_back(Point2f(103.04574087829076f, 167.44352944383985f));
    pts.push_back(Point2f(96.31623870161255f, 163.71641295746116f));
    pts.push_back(Point2f(89.86174417295126f, 157.2967811253635f));
    pts.push_back(Point2f(84.27940674801192f, 168.6331304010667f));
    pts.push_back(Point2f(76.61995117937661f, 159.4445412678832f));
    pts.push_back(Point2f(72.22526316142418f, 154.60770776728293f));
    pts.push_back(Point2f(64.97742405067658f, 152.3687174339018f));
    pts.push_back(Point2f(58.34612797237003f, 155.61116802371583f));
    pts.push_back(Point2f(55.59089117268539f, 148.56245696566418f));
    pts.push_back(Point2f(45.22711195983706f, 145.6713241271927f));
    pts.push_back(Point2f(40.090542298840234f, 142.36141304004002f));
    pts.push_back(Point2f(31.788996807277414f, 136.26164877915585f));
    pts.push_back(Point2f(27.27613006088805f, 137.46860042141503f));
    pts.push_back(Point2f(23.972392188502226f, 129.17993872328594f));
    pts.push_back(Point2f(20.688046711616977f, 121.52750840733087f));
    pts.push_back(Point2f(14.635115184257643f, 115.36942800110485f));
    pts.push_back(Point2f(14.850919318756809f, 109.43609786936987f));
    pts.push_back(Point2f(7.476847697758103f, 102.67657265589285f));
    pts.push_back(Point2f(1.8896944088091914f, 95.78878215565676f));
    pts.push_back(Point2f(1.731997022935417f, 88.17674033990495f));
    pts.push_back(Point2f(1.6780841363402033f, 80.65581939883002f));
    pts.push_back(Point2f(0.035330281415411946f, 73.1088693846768f));
    pts.push_back(Point2f(0.14652518786238033f, 65.42769523404296f));
    pts.push_back(Point2f(6.99914645302843f, 58.436451064804245f));
    pts.push_back(Point2f(6.719616410428614f, 50.15263031354927f));
    pts.push_back(Point2f(5.122267598477748f, 46.03603214691343f));
    // Input Ellipses
    cv::RotatedRect inputRect = cv::RotatedRect(Point2f(95.242f, 82.1639f), Size2f(196.189f, 157.473f), 23.6645f);
    cv::RotatedRect ellipseTest = fitEllipse(pts);
    cv::RotatedRect ellipseAMSTest = fitEllipseAMS(pts);
    cv::RotatedRect ellipseDirectTest = fitEllipseDirect(pts);
    // Display the results.
    cv::Mat canvas = drawPoints(pts, cv::Scalar(255,255,255));
    drawEllipseWithBox(canvas,inputRect,fitEllipseTrueColor,1);
    drawLabels(canvas);
    drawEllipseWithBox(canvas, ellipseAMSTest,    fitEllipseAMSColor,    1);
    drawEllipseWithBox(canvas, ellipseDirectTest, fitEllipseDirectColor, 1);
    drawEllipseWithBox(canvas, ellipseTest,       fitEllipseColor,       1);
    drawPoints(canvas, pts, cv::Scalar(255,255,255));
    imshow("test1",canvas);
}

void test2() {
    vector<Point2f>pts;
    pts.push_back(Point2f(436.59985753246326f, 99.52113368023126f));
    pts.push_back(Point2f(454.40214161915856f, 160.47565296546912f));
    pts.push_back(Point2f(406.01996690372687f, 215.41999534561575f));
    pts.push_back(Point2f(362.8738685722881f, 262.1842668997318f));
    pts.push_back(Point2f(300.72864073265407f, 290.8182699272777f));
    pts.push_back(Point2f(247.62963883830972f, 311.383137106776f));
    pts.push_back(Point2f(194.15394659099445f, 313.30260991427565f));
    pts.push_back(Point2f(138.934393338296f, 310.50203123324223f));
    pts.push_back(Point2f(91.66999301197541f, 300.57303988670515f));
    pts.push_back(Point2f(28.286233855826133f, 268.0670159317756f));
    // Input Ellipses
    cv::RotatedRect inputRect = cv::RotatedRect(Point2f(225.756f, 156.664f), Size2f(460.469f, 300.007f), -14.9957f);
    cv::RotatedRect ellipseTest = fitEllipse(pts);
    cv::RotatedRect ellipseAMSTest = fitEllipseAMS(pts);
    cv::RotatedRect ellipseDirectTest = fitEllipseDirect(pts);
    // Display the results.
    cv::Mat canvas = drawPoints(pts, cv::Scalar(255,255,255));
    drawEllipseWithBox(canvas,inputRect,fitEllipseTrueColor,1);
    drawLabels(canvas);
    drawEllipseWithBox(canvas, ellipseAMSTest,    fitEllipseAMSColor,    1);
    drawEllipseWithBox(canvas, ellipseDirectTest, fitEllipseDirectColor, 1);
    drawEllipseWithBox(canvas, ellipseTest,       fitEllipseColor,       1);
    drawPoints(canvas, pts, cv::Scalar(255,255,255));
    imshow("test2",canvas);
}


void test3() {
    vector<Point2f>pts;
    pts.push_back(Point2f(459.59217920219083f, 480.1054989283611f));
    pts.push_back(Point2f(427.2759071813645f, 501.82653857689616f));
    pts.push_back(Point2f(388.35145730295574f, 520.9488690267101f));
    pts.push_back(Point2f(349.53248668650656f, 522.9153107979839f));
    pts.push_back(Point2f(309.56018996762094f, 527.449631776843f));
    pts.push_back(Point2f(272.07480726768665f, 508.12367135706165f));
    pts.push_back(Point2f(234.69230939247115f, 519.8943877180591f));
    pts.push_back(Point2f(201.65185545142472f, 509.47870288702813f));
    pts.push_back(Point2f(169.37222144138462f, 498.2681549419808f));
    pts.push_back(Point2f(147.96233740677815f, 467.0923094529034f));
    pts.push_back(Point2f(109.68331701139209f, 433.39069422941986f));
    pts.push_back(Point2f(81.95454413977822f, 397.34325168750087f));
    pts.push_back(Point2f(63.74923800767195f, 371.939105294963f));
    pts.push_back(Point2f(39.966434417279885f, 329.9581349942296f));
    pts.push_back(Point2f(21.581668415402532f, 292.6692716276865f));
    pts.push_back(Point2f(13.687334926511767f, 248.91164234903772f));
    pts.push_back(Point2f(0.0f, 201.25693715845716f));
    pts.push_back(Point2f(3.90259455356599f, 155.68155247210575f));
    pts.push_back(Point2f(39.683930802331844f, 110.26290871953987f));
    pts.push_back(Point2f(47.85826684019932f, 70.82454140948524f));
    // Input Ellipses
    cv::RotatedRect inputRect = cv::RotatedRect(Point2f(277.183f, 262.297f), Size2f(580.724f, 476.204f), 41.4611f);
    cv::RotatedRect ellipseTest = fitEllipse(pts);
    cv::RotatedRect ellipseAMSTest = fitEllipseAMS(pts);
    cv::RotatedRect ellipseDirectTest = fitEllipseDirect(pts);
    // Display the results.
    cv::Mat canvas = drawPoints(pts, cv::Scalar(255,255,255));
    drawEllipseWithBox(canvas,inputRect,fitEllipseTrueColor,1);
    drawLabels(canvas);
    drawEllipseWithBox(canvas, ellipseAMSTest,    fitEllipseAMSColor,    1);
    drawEllipseWithBox(canvas, ellipseDirectTest, fitEllipseDirectColor, 1);
    drawEllipseWithBox(canvas, ellipseTest,       fitEllipseColor,       1);
    drawPoints(canvas, pts, cv::Scalar(255,255,255));
    imshow("test3",canvas);
}

void test4() {
    vector<Point2f>pts;
    pts.push_back(Point2f(461.1761758124861f, 79.55196261616746f));
    pts.push_back(Point2f(470.5034888757249f, 100.56760245239015f));
    pts.push_back(Point2f(470.7814479849749f, 127.45783922150272f));
    pts.push_back(Point2f(465.214384653262f, 157.51792078285405f));
    pts.push_back(Point2f(465.3739691861813f, 185.89204350118942f));
    pts.push_back(Point2f(443.36043162278366f, 214.43399982709002f));
    pts.push_back(Point2f(435.04682693174095f, 239.2657073987589f));
    pts.push_back(Point2f(444.48553588292697f, 262.0816619678671f));
    pts.push_back(Point2f(407.1290185495328f, 285.07828783776347f));
    pts.push_back(Point2f(397.71436554935804f, 304.782713567108f));
    pts.push_back(Point2f(391.65678619785854f, 323.6809382153118f));
    pts.push_back(Point2f(366.3904205781036f, 328.09416679736563f));
    pts.push_back(Point2f(341.7656517790918f, 346.9672607008338f));
    pts.push_back(Point2f(335.8021864809171f, 358.22416661090296f));
    pts.push_back(Point2f(313.29224574204227f, 373.3267160317279f));
    pts.push_back(Point2f(291.121216115417f, 377.3339312050791f));
    pts.push_back(Point2f(284.20367595990547f, 389.5930108233698f));
    pts.push_back(Point2f(270.9682061106809f, 388.4352006517971f));
    pts.push_back(Point2f(253.10188273008825f, 392.35120876055373f));
    pts.push_back(Point2f(234.2306946938868f, 407.0773705761117f));
    pts.push_back(Point2f(217.0544384092144f, 407.54850609237235f));
    pts.push_back(Point2f(198.40910966657933f, 423.7008860314684f));
    pts.push_back(Point2f(175.47011114845057f, 420.4223434173364f));
    pts.push_back(Point2f(154.92083551695902f, 418.5288198459268f));
    pts.push_back(Point2f(136.52988517939698f, 417.8311217226818f));
    pts.push_back(Point2f(114.74657291069317f, 410.1534699388714f));
    pts.push_back(Point2f(78.9220388330042f, 397.6266608135022f));
    pts.push_back(Point2f(76.82658673144391f, 404.27399269891055f));
    pts.push_back(Point2f(50.953595435605116f, 386.3824077178053f));
    pts.push_back(Point2f(43.603489077456985f, 368.7894972436907f));
    pts.push_back(Point2f(19.37402592752713f, 343.3511017547511f));
    pts.push_back(Point2f(8.714663367287343f, 322.2148323327599f));
    pts.push_back(Point2f(0., 288.7836318007535f));
    pts.push_back(Point2f(3.98686689837605f, 263.1748167870333f));
    pts.push_back(Point2f(9.536389714519785f, 233.02995195684738f));
    pts.push_back(Point2f(17.83246556512455f, 205.6536519851621f));
    pts.push_back(Point2f(33.00593702846919f, 180.52628138608327f));
    pts.push_back(Point2f(41.572400996463394f, 153.95185568689314f));
    pts.push_back(Point2f(54.55733659450332f, 136.54322891729444f));
    pts.push_back(Point2f(78.60990563833005f, 112.76538180538182f));
    // Input Ellipses
    cv::RotatedRect inputRect = cv::RotatedRect(Point2f(236.034f, 209.86f), Size2f(515.97f, 353.283f), -37.0581f);
    cv::RotatedRect ellipseTest = fitEllipse(pts);
    cv::RotatedRect ellipseAMSTest = fitEllipseAMS(pts);
    cv::RotatedRect ellipseDirectTest = fitEllipseDirect(pts);
    // Display the results.
    cv::Mat canvas = drawPoints(pts, cv::Scalar(255,255,255));
    drawEllipseWithBox(canvas,inputRect,fitEllipseTrueColor,1);
    drawLabels(canvas);
    drawEllipseWithBox(canvas, ellipseAMSTest,    fitEllipseAMSColor,    1);
    drawEllipseWithBox(canvas, ellipseDirectTest, fitEllipseDirectColor, 1);
    drawEllipseWithBox(canvas, ellipseTest,       fitEllipseColor,       1);
    drawPoints(canvas, pts, cv::Scalar(255,255,255));
    imshow("test4",canvas);
}

void test5() {
    vector<Point2f>pts;
    pts.push_back(Point2f(509.60609444351917f, 484.8233016998119f));
    pts.push_back(Point2f(508.55357451809846f, 498.61004779125176f));
    pts.push_back(Point2f(495.59325478416525f, 507.9238702677585f));
    pts.push_back(Point2f(455.32905012177747f, 517.7518674113691f));
    pts.push_back(Point2f(461.24821761238667f, 524.2115477440211f));
    pts.push_back(Point2f(438.8983455906825f, 528.424911702069f));
    pts.push_back(Point2f(425.9259699875303f, 532.5700430134499f));
    pts.push_back(Point2f(405.77496728300616f, 535.7295008444993f));
    pts.push_back(Point2f(384.31968113982475f, 536.3076260371831f));
    pts.push_back(Point2f(381.5356536818977f, 540.183355729414f));
    pts.push_back(Point2f(378.2530503455792f, 540.2871855284832f));
    pts.push_back(Point2f(357.7242088314752f, 543.473075733281f));
    pts.push_back(Point2f(339.27871831324853f, 541.2099003613087f));
    pts.push_back(Point2f(339.22481874867435f, 541.1105421426018f));
    pts.push_back(Point2f(331.50337377509396f, 539.7296050163102f));
    pts.push_back(Point2f(317.8306501537862f, 540.9077275195326f));
    pts.push_back(Point2f(304.9192648323086f, 541.3434792768918f));
    pts.push_back(Point2f(297.33855427908617f, 543.0590309600501f));
    pts.push_back(Point2f(288.95330515997694f, 543.8756702506837f));
    pts.push_back(Point2f(278.5850913122515f, 538.1343888329859f));
    pts.push_back(Point2f(266.05355938101724f, 538.4115695907074f));
    pts.push_back(Point2f(255.30186994366096f, 534.2459272411796f));
    pts.push_back(Point2f(238.52054973466758f, 537.5007401480628f));
    pts.push_back(Point2f(228.444463024996f, 533.8992361116678f));
    pts.push_back(Point2f(217.8111623149833f, 538.2269193558991f));
    pts.push_back(Point2f(209.43502138981037f, 532.8057062984569f));
    pts.push_back(Point2f(193.33570716763276f, 527.2038128630041f));
    pts.push_back(Point2f(172.66725340039625f, 526.4020881005537f));
    pts.push_back(Point2f(158.33654199771337f, 525.2093856704676f));
    pts.push_back(Point2f(148.65905485249067f, 521.0146762179431f));
    pts.push_back(Point2f(147.6615365176719f, 517.4315201992808f));
    pts.push_back(Point2f(122.43568509949394f, 514.2089723387337f));
    pts.push_back(Point2f(110.88482982039073f, 509.14004840857046f));
    pts.push_back(Point2f(107.10516681523065f, 502.49943180234266f));
    pts.push_back(Point2f(82.66611013934804f, 494.0581153893113f));
    pts.push_back(Point2f(63.573319848965966f, 485.6772487054385f));
    pts.push_back(Point2f(47.65729058071245f, 475.4468806518075f));
    pts.push_back(Point2f(19.96819458379347f, 463.98285210241943f));
    pts.push_back(Point2f(27.855803175234342f, 450.2298664426336f));
    pts.push_back(Point2f(12.832198085636549f, 435.6317753810441f));
    // Input Ellipses
    cv::RotatedRect inputRect = cv::RotatedRect(Point2f(268.357f, 454.105f), Size2f(500.135f, 169.082f), 5.32011f);
    cv::RotatedRect ellipseTest = fitEllipse(pts);
    cv::RotatedRect ellipseAMSTest = fitEllipseAMS(pts);
    cv::RotatedRect ellipseDirectTest = fitEllipseDirect(pts);
    // Display the results.
    cv::Mat canvas = drawPoints(pts, cv::Scalar(255,255,255));
    drawEllipseWithBox(canvas,inputRect,fitEllipseTrueColor,1);
    drawLabels(canvas);
    drawEllipseWithBox(canvas, ellipseAMSTest,    fitEllipseAMSColor,    1);
    drawEllipseWithBox(canvas, ellipseDirectTest, fitEllipseDirectColor, 1);
    drawEllipseWithBox(canvas, ellipseTest,       fitEllipseColor,       1);
    drawPoints(canvas, pts, cv::Scalar(255,255,255));
    imshow("test5",canvas);
}

void test6() {
    vector<Point2f>pts;
    pts.push_back(Point2f(414.90156479295905f, 29.063453659930833f));
    pts.push_back(Point2f(393.79576036337977f, 58.59512774879134f));
    pts.push_back(Point2f(387.9100725249931f, 94.65067695657254f));
    pts.push_back(Point2f(351.6987114318621f, 124.6049267560123f));
    pts.push_back(Point2f(335.3270519942532f, 154.52182750730412f));
    pts.push_back(Point2f(329.2955843262556f, 179.38031343427303f));
    pts.push_back(Point2f(322.7316812937696f, 201.88774427737036f));
    pts.push_back(Point2f(301.48326350826585f, 217.63331351026562f));
    pts.push_back(Point2f(287.4603938315088f, 228.68790184154113f));
    pts.push_back(Point2f(273.36617750656023f, 234.48397257849905f));
    pts.push_back(Point2f(270.7787206270782f, 242.85279436204632f));
    pts.push_back(Point2f(268.6973828073692f, 246.10891460870312f));
    pts.push_back(Point2f(261.60715070464255f, 252.65744793902192f));
    pts.push_back(Point2f(262.9041824871923f, 257.1813047575656f));
    pts.push_back(Point2f(263.3210079177046f, 260.0532193246593f));
    pts.push_back(Point2f(248.49568488533242f, 264.56723557175013f));
    pts.push_back(Point2f(245.4134174127509f, 264.87259401292f));
    pts.push_back(Point2f(244.73208618171216f, 272.32307359830884f));
    pts.push_back(Point2f(232.82093196087555f, 272.0239734764616f));
    pts.push_back(Point2f(235.28539413113458f, 276.8668447478244f));
    pts.push_back(Point2f(231.9766571511147f, 277.71179872893083f));
    pts.push_back(Point2f(227.23880706209866f, 284.5588878789101f));
    pts.push_back(Point2f(222.53202223537826f, 282.2293154479012f));
    pts.push_back(Point2f(217.27525654729595f, 297.42961148365725f));
    pts.push_back(Point2f(212.19490057230672f, 294.5344078014253f));
    pts.push_back(Point2f(207.47417472945446f, 301.72230412668307f));
    pts.push_back(Point2f(202.11143229969164f, 298.8588627545512f));
    pts.push_back(Point2f(196.62967096845824f, 309.39738607353223f));
    pts.push_back(Point2f(190.37809841992106f, 318.3250479151242f));
    pts.push_back(Point2f(183.1296129732803f, 322.35242231955453f));
    pts.push_back(Point2f(171.58530535265993f, 330.4981441404153f));
    pts.push_back(Point2f(160.40092880652247f, 337.47275990208226f));
    pts.push_back(Point2f(149.44888762618092f, 343.42296086656717f));
    pts.push_back(Point2f(139.7923528305302f, 353.4821948045352f));
    pts.push_back(Point2f(121.08414969113318f, 359.7010225709457f));
    pts.push_back(Point2f(100.10629739219641f, 375.3155744055458f));
    pts.push_back(Point2f(78.15715630786733f, 389.0311284319413f));
    pts.push_back(Point2f(51.22820988075294f, 396.98646504159547f));
    pts.push_back(Point2f(30.71132492338431f, 402.85098740402844f));
    pts.push_back(Point2f(10.994737323179852f, 394.6764602972333f));
    // Input Ellipses
    cv::RotatedRect inputRect = cv::RotatedRect(Point2f(203.566f, 200.563f), Size2f(550.809f, 152.538f), -44.5035f);
    cv::RotatedRect ellipseTest = fitEllipse(pts);
    cv::RotatedRect ellipseAMSTest = fitEllipseAMS(pts);
    cv::RotatedRect ellipseDirectTest = fitEllipseDirect(pts);
    // Display the results.
    cv::Mat canvas = drawPoints(pts, cv::Scalar(255,255,255));
    drawEllipseWithBox(canvas,inputRect,fitEllipseTrueColor,1);
    drawLabels(canvas);
    drawEllipseWithBox(canvas, ellipseAMSTest,    fitEllipseAMSColor,    1);
    drawEllipseWithBox(canvas, ellipseDirectTest, fitEllipseDirectColor, 1);
    drawEllipseWithBox(canvas, ellipseTest,       fitEllipseColor,       1);
    drawPoints(canvas, pts, cv::Scalar(255,255,255));
    imshow("test6",canvas);
}

void test7() {
    vector<Point2f>pts;
    pts.push_back(Point2f(386.7497806918209f, 119.55623710363142f));
    pts.push_back(Point2f(399.0712613744503f, 132.61095972401034f));
    pts.push_back(Point2f(400.3582576852657f, 146.71942033652573f));
    pts.push_back(Point2f(383.31046706707906f, 160.13631428164982f));
    pts.push_back(Point2f(387.1626582455823f, 173.82700569763574f));
    pts.push_back(Point2f(378.88843308401425f, 186.10333319745317f));
    pts.push_back(Point2f(367.55061701208f, 201.41492900400164f));
    pts.push_back(Point2f(360.3254967185148f, 209.03834085076022f));
    pts.push_back(Point2f(346.2645164278429f, 222.03214282040395f));
    pts.push_back(Point2f(342.3483403634167f, 230.58290419787073f));
    pts.push_back(Point2f(326.2900969991908f, 240.23679566682756f));
    pts.push_back(Point2f(324.5622396580625f, 249.56961396707823f));
    pts.push_back(Point2f(304.23417130914095f, 259.6693711280021f));
    pts.push_back(Point2f(295.54035697534675f, 270.82284542557704f));
    pts.push_back(Point2f(291.7403057147348f, 276.1536825048371f));
    pts.push_back(Point2f(269.19344116558665f, 287.1705579044651f));
    pts.push_back(Point2f(256.5350613899267f, 274.91264707500943f));
    pts.push_back(Point2f(245.93644351417183f, 286.12398028743064f));
    pts.push_back(Point2f(232.40892420943732f, 282.73986583867065f));
    pts.push_back(Point2f(216.17957969101082f, 293.22229708237705f));
    pts.push_back(Point2f(205.66843722622573f, 295.7032575625158f));
    pts.push_back(Point2f(192.219969335765f, 302.6968969534755f));
    pts.push_back(Point2f(178.37758801730416f, 295.56656776633287f));
    pts.push_back(Point2f(167.60089103756644f, 301.4629292267722f));
    pts.push_back(Point2f(157.44802813915317f, 298.90830855734504f));
    pts.push_back(Point2f(138.44311818820313f, 293.951927187897f));
    pts.push_back(Point2f(128.92747660038592f, 291.4122695492978f));
    pts.push_back(Point2f(119.75160909865994f, 282.5809454721714f));
    pts.push_back(Point2f(98.48443737042328f, 290.39938776333247f));
    pts.push_back(Point2f(88.05275635126131f, 280.11156058895745f));
    pts.push_back(Point2f(82.45799026448167f, 271.46668468419773f));
    pts.push_back(Point2f(68.04031962064084f, 267.8136468580707f));
    pts.push_back(Point2f(58.99967170878713f, 263.8859310392943f));
    pts.push_back(Point2f(41.256097220823484f, 260.6041605773932f));
    pts.push_back(Point2f(40.66198797608645f, 246.64973068177196f));
    pts.push_back(Point2f(31.085484380646008f, 239.28615601336074f));
    pts.push_back(Point2f(24.069417111444253f, 225.2228746297288f));
    pts.push_back(Point2f(22.10122953275156f, 212.75509683149195f));
    pts.push_back(Point2f(9.929991244497518f, 203.20662088477752f));
    pts.push_back(Point2f(0.0f, 190.04891498441148f));
    // Input Ellipses
    cv::RotatedRect inputRect = cv::RotatedRect(Point2f(197.181f, 147.648f), Size2f(395.369f, 289.903f), -12.0625f);
    cv::RotatedRect ellipseTest = fitEllipse(pts);
    cv::RotatedRect ellipseAMSTest = fitEllipseAMS(pts);
    cv::RotatedRect ellipseDirectTest = fitEllipseDirect(pts);
    // Display the results.
    cv::Mat canvas = drawPoints(pts, cv::Scalar(255,255,255));
    drawEllipseWithBox(canvas,inputRect,fitEllipseTrueColor,1);
    drawLabels(canvas);
    drawEllipseWithBox(canvas, ellipseAMSTest,    fitEllipseAMSColor,    1);
    drawEllipseWithBox(canvas, ellipseDirectTest, fitEllipseDirectColor, 1);
    drawEllipseWithBox(canvas, ellipseTest,       fitEllipseColor,       1);
    drawPoints(canvas, pts, cv::Scalar(255,255,255));
    imshow("test7",canvas);
}

void test_Issue_6544() {vector<Point2f> pts;
    pts.push_back(Point2f(924.784f, 764.160f));
    pts.push_back(Point2f(928.388f, 615.903f));
    pts.push_back(Point2f(847.4f,   888.014f));
    pts.push_back(Point2f(929.406f, 741.675f));
    pts.push_back(Point2f(904.564f, 825.605f));
    pts.push_back(Point2f(926.742f, 760.746f));
    pts.push_back(Point2f(863.479f, 873.406f));
    pts.push_back(Point2f(910.987f, 808.863f));
    pts.push_back(Point2f(929.145f, 744.976f));
    pts.push_back(Point2f(917.474f, 791.823f));
    // Input Ellipses
//    cv::RotatedRect inputRect = cv::RotatedRect(Point2f(197.181f, 147.648f), Size2f(395.369f, 289.903f), -12.0625f);
    cv::RotatedRect ellipseTest = fitEllipse(pts);
    cv::RotatedRect ellipseAMSTest = fitEllipseAMS(pts);
    cv::RotatedRect ellipseDirectTest = fitEllipseDirect(pts);
    // Display the results.
    cv::Mat canvas = drawPoints(pts, cv::Scalar(255,255,255));
//    drawEllipseWithBox(canvas,inputRect,fitEllipseTrueColor,1);
    drawLabels(canvas);
    drawEllipseWithBox(canvas, ellipseAMSTest,    fitEllipseAMSColor,    2);
    drawEllipseWithBox(canvas, ellipseDirectTest, fitEllipseDirectColor, 1);
    drawEllipseWithBox(canvas, ellipseTest,       fitEllipseColor,       1);
    drawPoints(canvas, pts, cv::Scalar(255,255,255));
    imshow("test_Issue_6544",canvas);
}


