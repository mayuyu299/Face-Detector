// -*- C++ -*-
/*!
 * @file  Face_judge2.cpp
 * @brief ModuleDescription
 * @date $Date$
 *
 * $Id$
 */

#include "Face_judge2.h"
#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"
#include <vector>

using namespace cv;
using namespace std;

// Module specification
// <rtc-template block="module_spec">
static const char* face_judge2_spec[] =
  {
    "implementation_id", "Face_judge2",
    "type_name",         "Face_judge2",
    "description",       "ModuleDescription",
    "version",           "1.0.0",
    "vendor",            "MayuWatanabe",
    "category",          "example",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
Face_judge2::Face_judge2(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_CameraIn("Camera", m_Camera),
    m_FaceOut("Face", m_Face)

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
Face_judge2::~Face_judge2()
{
}



RTC::ReturnCode_t Face_judge2::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  addInPort("Camera", m_CameraIn);

  // Set OutPort buffer
  addOutPort("Face", m_FaceOut);

  // Set service provider to Ports

  // Set service consumers to Ports

  // Set CORBA Service Ports

  // </rtc-template>

  // <rtc-template block="bind_config">
  // </rtc-template>

  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t Face_judge2::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Face_judge2::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Face_judge2::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Face_judge2::onActivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Face_judge2::onDeactivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t Face_judge2::onExecute(RTC::UniqueId ec_id)
{
	//Mat frame; //USBカメラから得た１フレームを格納する場所
	CascadeClassifier cascade; //カスケード分類器格納場所
	cascade.load("C:/opencv/sources/data/haarcascades/haarcascade_frontalface_alt.xml"); //正面顔情報が入っているカスケード
	vector<Rect> faces; //輪郭情報を格納場所

	Mat detection_frame;//顔の検出範囲
	Rect roi;
	int detection_flag = 0;//直前に顔を検出したか(0:してない　1:した)

	int x = 0;//顔座標の左上のx座標
	int y = 0;//顔座標の左上のy座標
	int x_end = 0;//顔座標の右下のx座標
	int y_end = 0;//顔座標の右下のy座標

	int basic_flag = 0;//連続で顔を検知しているかフラグ(0:いいえ(初めての検知)　1:はい(2連続以上の検知))
	int x_basic = 0;//基準点のX座標
	int y_basic = 0;//基準点のY座標

	int not_found_flag = 1;//連続顔を見つけられなかったフラグ(0:いいえ(見つかった)　1:はい(見つからなかった))


	while (1)//無限ループ
	{
		m_CameraIn.read();
		cv::Mat frame(cv::Size(m_Camera.width, m_Camera.height), CV_8UC3, (void*)&(m_Camera.pixels[0]));//CameraImage型をMat型に変換
		

		//直前のフレームで顔が検出されていない場合
		if (m_Face.data == 0) {

			//検出範囲はカメラ映像全体とする
			detection_frame = frame;

			//基準点をリセット
			basic_flag = 0;
			x_basic = 0;
			y_basic = 0;

		}
		else {//直前のフレームで顔が検出された場合

			//検出範囲がキャプチャフレーム内に収まるように変換する
			if (x - 50 < 1) {
				x = 51;
			}
			if (y - 50 < 1) {
				y = 51;
			}
			if (x_end + 50 > frame.cols - 1) {
				x_end = frame.cols - 51;
			}
			if (y_end + 50 > frame.rows - 1) {
				y_end = frame.rows - 51;
			}

			//検出範囲として、直前のフレームの顔検出の範囲より一回り(上下左右50pixel)大きい範囲とする
			Rect roi(Point(x - 50, y - 50), Point(x_end + 50, y_end + 50));
			detection_frame = frame(roi);

			//検出範囲をピンク枠で囲う
			rectangle(frame, Point(x - 50, y - 50), Point(x_end + 50, y_end + 50), Scalar(200, 0, 255), 3);

			//連続検索フラグを1(2連続以上の)
			basic_flag = 1;

		}

		detection_flag = 0;
		
		
		
		//格納されたフレームに対してカスケードファイルに基づいて顔を検知
		cascade.detectMultiScale(detection_frame, faces, 1.2, 5, 0, Size(20, 20));
		

		//連続顔検出フラグが0のとき顔を斜めにする
		//(直前に顔を検出していた時だけ斜めの検出を行う)
		if (not_found_flag == 0) {
			not_found_flag = 1;
			if (faces.size() == 0) {
				//右に15度傾けるアフィン行列を求める
				Mat trans = getRotationMatrix2D(Point(detection_frame.cols / 2, detection_frame.rows / 2), 15, 1);
				//求めたアフィン行列を使って、ピンク枠内画像を回転する
				warpAffine(detection_frame, detection_frame, trans, detection_frame.size());
				//傾けた画像で顔を検出
				cascade.detectMultiScale(detection_frame, faces, 1.2, 5, 0, Size(20, 20));
			}
			if (faces.size() == 0) {
				//左に15度傾けるアフィン行列を求める(右に15度傾けていたので-30度右に傾けることで実質左に15度傾く)
				Mat trans = getRotationMatrix2D(Point(detection_frame.cols / 2, detection_frame.rows / 2), -30, 1);
				//求めたアフィン行列を使って、ピンク枠内画像を回転する
				warpAffine(detection_frame, detection_frame, trans, detection_frame.size());
				//傾けた画像で顔を検出
				cascade.detectMultiScale(detection_frame, faces, 1.2, 5, 0, Size(20, 20));
			}
		}



		//顔を検出した場合
		if (faces.size() > 0 ) {
			//顔の検出フラグを1(発見)にする
			detection_flag = 1;
			m_Face.data = 1;
			m_FaceOut.write();

			//連続顔を見つけられなかったフラグを0
			not_found_flag = 0;

			//顔座標の左上の座標を求める
			if (basic_flag == 0) {//初検知の場合

				//初検知の場合は検出された値をそのまま使う
				x = faces[0].x;
				y = faces[0].y;

			}
			else if (basic_flag == 1) {//連続検知の場合

				//連続検知の場合は、検出座標と直前の基準点を使って顔座標を検出する
				//(x_basic - 50)：カメラキャプチャ全体の座標から見た検出範囲の左上の座標(ピンク枠の左上)
				//faces[0].x：切り出したフレーム(ピンク枠内)から見た顔の左上の座標(赤枠の左上)

				x = (x_basic - 50) + faces[0].x;
				y = (y_basic - 50) + faces[0].y;

			}

			//顔座標の右下の座標を求める
			x_end = x + faces[0].width;
			y_end = y + faces[0].height;

			//基準点を今算出した顔座標に更新する
			x_basic = x;
			y_basic = y;

			rectangle(frame, Point(x, y), Point(x_end, y_end), Scalar(0, 0, 255), 3);
			//printf("(%d,%d) (%d,%d)\n", x, y, x_end, y_end);
		}
		else {//顔検出をしなかった場合
			m_Face.data = 0;
			m_FaceOut.write();
		}

		imshow("window", frame);//画像を表示．

		int key = waitKey(1);
		if (key == 113)//qボタンが押されたとき
		{
			break;//whileループから抜ける（終了）
		}
	}
	destroyAllWindows();
	return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t Face_judge2::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Face_judge2::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Face_judge2::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Face_judge2::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Face_judge2::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{

  void Face_judge2Init(RTC::Manager* manager)
  {
    coil::Properties profile(face_judge2_spec);
    manager->registerFactory(profile,
                             RTC::Create<Face_judge2>,
                             RTC::Delete<Face_judge2>);
  }

};


