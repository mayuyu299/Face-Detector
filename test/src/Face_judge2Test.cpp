// -*- C++ -*-
/*!
 * @file  Face_judge2Test.cpp
 * @brief ModuleDescription
 * @date $Date$
 *
 * $Id$
 */

#include "Face_judge2Test.h"

// Module specification
// <rtc-template block="module_spec">
static const char* face_judge2_spec[] =
  {
    "implementation_id", "Face_judge2Test",
    "type_name",         "Face_judge2Test",
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
Face_judge2Test::Face_judge2Test(RTC::Manager* manager)
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
Face_judge2Test::~Face_judge2Test()
{
}



RTC::ReturnCode_t Face_judge2Test::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  addInPort("Face", m_FaceIn);

  // Set OutPort buffer
  addOutPort("Camera", m_CameraOut);

  // Set service provider to Ports

  // Set service consumers to Ports

  // Set CORBA Service Ports

  // </rtc-template>

  // <rtc-template block="bind_config">
  // </rtc-template>

  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t Face_judge2Test::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Face_judge2Test::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Face_judge2Test::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Face_judge2Test::onActivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Face_judge2Test::onDeactivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t Face_judge2Test::onExecute(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t Face_judge2Test::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Face_judge2Test::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Face_judge2Test::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Face_judge2Test::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Face_judge2Test::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{

  void Face_judge2TestInit(RTC::Manager* manager)
  {
    coil::Properties profile(face_judge2_spec);
    manager->registerFactory(profile,
                             RTC::Create<Face_judge2Test>,
                             RTC::Delete<Face_judge2Test>);
  }

};


