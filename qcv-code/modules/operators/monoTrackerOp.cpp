/*
 * Copyright (C) 2012 Hernan Badino <hernan.badino@gmail.com>
 *
 * This file is part of QCV
 *
 * QCV is under the terms of the GNU Lesser General Public License
 * version 2.1. See the GNU LGPL version 2.1 for details.
 * QCV is distributed "AS IS" without ANY WARRANTY, without even the
 * implied warranty of merchantability or fitness for a particular
 * purpose. 
 *
 * In no event shall the authors or contributors be liable
 * for any direct, indirect, incidental, special, exemplary, or
 * consequential damages arising in any way out of the use of this
 * software.
 *
 * By downloading, copying, installing or using the software you agree
 * to this license. Do not download, install, copy or use the
 * software, if you do not agree to this license.
 */

/**
*******************************************************************************
*
* @file monoTrackerOp.cpp
*
* \class CMonoTrackerOp 
* \author Hernan Badino (hernan.badino@gmail.com)
*
*
*******************************************************************************/

/* INCLUDES */
#include <limits>

#include "monoTrackerOp.h"

#include "paramMacros.h"
#include "drawingList.h"
#include "gfttFreakOp.h"
#include "kltTrackerOp.h"

#include "paramIOXmlFile.h"

#include "feature.h"

using namespace QCV;

/// Constructors.
CMonoTrackerOp::CMonoTrackerOp ( COperator * const f_parent_p,
                     const std::string f_name_str )
    : COperator (             f_parent_p, f_name_str ),
      m_gftt_p (                                NULL ),
      m_kltTracker_p (                          NULL ),
      m_compute_b (                             true ),
      m_origCamera(                                  ),
      m_camera (                                     ),
      m_centralPointOffset (                0.f, 0.f ),
      m_resizeToOrigSize_b (                    true ),
      m_fovScale_f (                             1.f ),
      m_scaleHorOnly_b (                       false ),
      m_grayImage0 (                                 ),
      m_grayImage1 (                                 ),
      m_scaledImage0 (                               ),
      m_scaledImage1 (                               ),
      m_scaledImage2 (                               ),
      m_scaledImage3 (                               ),
      m_unifiedFeatureVector (                       ),
      m_cropTopLeft (                         -1, -1 ),
      m_cropBottomRight (                     -1, -1 )
      
{
    m_gftt_p       = new CGfttFreakOp     ( this, "Harris-Freak Tracker" );
    m_kltTracker_p = new CKltTrackerOp    ( this, "KLT Tracker" );

    addChild ( m_gftt_p );
    addChild ( m_kltTracker_p );

    registerDrawingLists(  );
    registerParameters (  );
}

void
CMonoTrackerOp::registerDrawingLists(  )
{
   registerDrawingList ("Image 0",
                        S2D<int> (0, 0),
                        false);
}

void
CMonoTrackerOp::registerParameters(  )
{

    BEGIN_PARAMETER_GROUP("Computation", false, SRgb(220,0,0));
    ADD_BOOL_PARAMETER ( "Compute",
                         "Perform the scaling?.",
                         m_compute_b,
                         this,
                         Compute,
                         CMonoTrackerOp );

    ADD_FLT2D_PARAMETER ( "Central point offset",
                          "Camera central point offset [px]",
                          m_centralPointOffset,
                          "u", "v",
                          this,
                          CentralPointOffset,
                          CMonoTrackerOp );

    ADD_FLOAT_PARAMETER ( "FOV scale factor",
                          "Scale factor to apply to the fov for the left and right images.",
                          m_fovScale_f,
                          this,
                          FovScale,
                          CMonoTrackerOp );

    ADD_BOOL_PARAMETER ( "Scale Horizontally Only",
                         "Resize image only in horizontal direction",
                         m_scaleHorOnly_b,
                         this,
                         ScaleHorizontallyOnly,
                         CMonoTrackerOp );

    ADD_BOOL_PARAMETER ( "Resize to orig size",
                         "Resize image to original size if fov scale factor <> 1",
                         m_resizeToOrigSize_b,
                         this,
                         ResizeToOrigSize,
                         CMonoTrackerOp );

    ADD_INT2D_PARAMETER ( "Crop Top-left",
                          "Top-left image coordinates to define crop area. [px]",
                          m_cropTopLeft,
                          "x","y",
                          this,
                          CropTopLeft,
                          CMonoTrackerOp );

    ADD_INT2D_PARAMETER ( "Crop Bottom-right",
                          "Bottom-right image coordinates to define crop area. [px]",
                          m_cropBottomRight,
                          "x","y",
                          this,
                          CropBottomRight,
                          CMonoTrackerOp );


    END_PARAMETER_GROUP;

    BEGIN_PARAMETER_GROUP("Display", false, SRgb(220,0,0));
      ADD_BOOL_PARAMETER ( "Register Drawing Lists",
                           "Register the drawing lists as output of the operator so that other operators can reuse the texture information",
                           m_registerDL_b,
                           this,
                           RegisterDrawingLists,
                           CMonoTrackerOp );

      addDrawingListParameter ( "Image 0" );
      addDrawingListParameter ( "Image 1" );      
    END_PARAMETER_GROUP;
}

/// Virtual destructor.
CMonoTrackerOp::~CMonoTrackerOp ()
{
}

/// Cycle event.
bool
CMonoTrackerOp::cycle()
{
    if ( m_compute_b )
    {
       m_camera = m_origCamera;

       cv::Mat img0 =  getInput<cv::Mat>("Image 0", cv::Mat() );
       cv::Mat img1 =  getInput<cv::Mat>("Image 1", cv::Mat() );

       /// Convert to gray scale if in RGB format.
       if ( img0.type() == CV_8UC3 )
       {
          cvtColor(img0, m_scaledImage0, CV_BGR2GRAY);
          registerOutput<cv::Mat>("Image 0", &m_scaledImage0);
       }

       /// Convert to gray scale if in RGB format.
       if ( img1.type() == CV_8UC3 )
       {
          cvtColor(img1, m_scaledImage1, CV_BGR2GRAY);
          registerOutput<cv::Mat>("Image 1", &m_scaledImage1);
       }

       img0 =  getInput<cv::Mat>("Image 0", cv::Mat() );
       img1 =  getInput<cv::Mat>("Image 1", cv::Mat() );
       if (img0.cols > 0 && img1.cols > 0 && m_fovScale_f < 1.f)
       {
          float fov_f       = atan(img0.cols/(2*m_origCamera.getFocalLength()));
          fov_f            *= m_fovScale_f;
          float newWidth_f  = 2 * m_origCamera.getFocalLength() * tan(fov_f);

          if (fabsf(newWidth_f - img0.cols) > 1.f )
          {
             float newHeight_f = m_scaleHorOnly_b?img0.rows:img0.rows * newWidth_f/img0.cols;
             
             cv::Rect roi (img0.cols/2.f-newWidth_f/2.f, std::max(img0.rows/2.f-newHeight_f/2.f, 0.f), 
                           newWidth_f, newHeight_f );
             
             if (m_resizeToOrigSize_b && !m_scaleHorOnly_b)
             {
                cv::resize(img0(roi), m_scaledImage0, img0.size());
                cv::resize(img1(roi), m_scaledImage1, img1.size());
                
                m_camera.setFocalLength( img0.cols / (tan(fov_f)*2.f) );
             }
             else
             {
                float scale_f = newWidth_f/img0.cols;
                img0(roi).copyTo(m_scaledImage0);
                img1(roi).copyTo(m_scaledImage1);
                m_camera.setU0( m_camera.getU0() - roi.x );
                m_camera.setV0( m_camera.getV0() - roi.y );
             }
             
             registerOutput<cv::Mat>("Image 0", &m_scaledImage0);
             registerOutput<cv::Mat>("Image 1", &m_scaledImage1);

          }
       }
 
       if ( m_cropBottomRight.x >= 0 || 
            m_cropBottomRight.y >= 0 || 
            m_cropTopLeft.x >= 0 || 
            m_cropTopLeft.y >= 0 )
       {
          cv::Mat img0 =  getInput<cv::Mat>("Image 0", cv::Mat() );
          cv::Mat img1 =  getInput<cv::Mat>("Image 1", cv::Mat() );

          cv::Point2i topleft  ( m_cropTopLeft.x<0?0:std::min( std::max(m_cropTopLeft.x, 0), img0.cols-1),
                                 m_cropTopLeft.y<0?0:std::min( std::max(m_cropTopLeft.y, 0), img0.rows-1) );
          cv::Point2i botright ( m_cropBottomRight.x<0?img0.cols:std::min( std::max(m_cropBottomRight.x, 0), img0.cols),
                                 m_cropBottomRight.y<0?img0.rows:std::min( std::max(m_cropBottomRight.y, 0), img0.rows) );
          
          if (topleft.x < botright.x && topleft.y < botright.y )
          {
             m_scaledImage2 = img0(cv::Rect(topleft, botright));
             m_scaledImage3 = img1(cv::Rect(topleft, botright));
             
             m_camera.setU0( m_camera.getU0() - topleft.x );
             m_camera.setV0( m_camera.getV0() - topleft.y );

             registerOutput<cv::Mat>("Image 0", &m_scaledImage2);
             registerOutput<cv::Mat>("Image 1", &m_scaledImage3);
          }
       }

       m_camera.setU0( m_camera.getU0() + m_centralPointOffset.x );
       m_camera.setV0( m_camera.getV0() + m_centralPointOffset.y );
       registerOutput<CCamera> ( "Rectified Camera", &m_camera );

       COperator::cycle(m_gftt_p);
       COperator::cycle(m_kltTracker_p);

       /// Join feature vectors
       const CFeatureVector *featVec1 = m_gftt_p      ->getFeatureVector();
       const CFeatureVector *featVec2 = m_kltTracker_p->getFeatureVector();
       
       m_unifiedFeatureVector.clear();
       
       if (featVec1)
          m_unifiedFeatureVector.insert(m_unifiedFeatureVector.begin()+m_unifiedFeatureVector.size(), 
                                        featVec1->begin(), featVec1->end() );
       
       if (featVec2)
          m_unifiedFeatureVector.insert(m_unifiedFeatureVector.begin()+m_unifiedFeatureVector.size(), 
                                        featVec2->begin(), featVec2->end() );
       
       registerOutput <CFeatureVector> ( "Unified Feature Vector", 
                                         &m_unifiedFeatureVector );

       if ( getParentOp() )
       {
          
          getParentOp() -> registerOutput <CFeatureVector> ( m_gftt_p->getFeaturePointVectorId(), 
                                                             m_gftt_p->getFeatureVector() );


          getParentOp() -> registerOutput <CFeatureVector> ( m_kltTracker_p->getFeaturePointVectorId(), 
                                                             m_kltTracker_p->getFeatureVector() );      

          
          std::string str = "KltTrackerOp Previous Image";
          getParentOp() -> registerOutput <cv::Mat> ( str,
                                                      &m_kltTracker_p->getPreviousImage() );      
          
          str = "KltTrackerOp Current Image";
          getParentOp() -> registerOutput <cv::Mat> ( str,
                                                      &m_kltTracker_p->getCurrentImage() );
       }
    }
    
    return true;
}
    
/// Show event.
bool CMonoTrackerOp::show()
{
   cv::Mat img =  getInput<cv::Mat>("Image 0", cv::Mat() );

   CDrawingList * list_p = getDrawingList("Image 0");
   list_p->clear();

   /// Set the screen size if this is the parent operator.
   if ( img.size().width > 0 )
   {
	   if ( list_p->isVisible() || m_registerDL_b )
	   {
    	  list_p->addImage(img);
	      registerOutput<CDrawingList>("Image 0 Drawing List", list_p);
	   }
      
	   if (!getParentOp() )
	   {
	      setScreenSize ( img.size() );
	   }
	}

   return COperator::show();
}

/// Init event.
bool CMonoTrackerOp::initialize()
{
    std::string  filePath0_str = getInput<std::string> ("Image 0 Path", "");

    printf("File path to Image 0 is %s\n",
           filePath0_str.c_str());
    
    int pos_i = filePath0_str.find_last_of ("/\\");

    if ( pos_i != -1 )
        filePath0_str.erase(pos_i);
    else
        filePath0_str = std::string("."); 

    filePath0_str += "/camera.xml";

    bool fileFound_b = false;

    CParamIOXmlFile paramFile;

    printf("Trying to open parameter file from \"%s\"\n",
           filePath0_str.c_str() );

    /// Try to load camera file.
    if (!paramFile.load( filePath0_str ))
    {    
       printf("NOT Found. NOW trying from local directory\n" );
       /// If does not exist, try to open camera.xml in the current directory.
        if (paramFile.load ("./camera.xml"))
            fileFound_b = true;
    }       
    else
        fileFound_b = true;
    
    if (fileFound_b)
    {
        paramFile.setCurrentCategory ( "Rectified Camera" );
        m_origCamera.load ( paramFile );
        printf("Camera parameters successfuly loaded\n");
    }
    else
    {
       printf("%s:%i camera.xml file could not be found\n", __FILE__, __LINE__ );
    }

    registerOutput<CCamera> ( "Rectified Camera", &m_origCamera );

    return COperator::initialize();
}

/// Reset event.
bool CMonoTrackerOp::reset()
{
    return COperator::reset();
}

bool CMonoTrackerOp::exit()
{
    return COperator::exit();
}


