/*
 * Copyright (C) 2012 Hernan Badino <hernan.badino@gmail.com>
 *
 * This file is part of QCV
 *
 * QCV is under the terms of the GNU Lesser General Public License
 * version 3. See the GNU LGPL version 3 for details.
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

#ifndef __MAINWINDOW_H
#define __MAINWINDOW_H

/**
 *******************************************************************************
 *
 * @file mainWindow.h
 *
 * \class CMainWindow
 * \author Hernan Badino (hernan.badino@gmail.com)
 *
 * \brief The class implements the main window dialog of the framework.
 *
 * The CMainWindow class instantiates the device controls, parameter editors, 
 * displays dialogs and control dialogs. It also acts as communication interface 
 * between all those components. A pointer to the root operator must be given 
 * in the instantiation of the object.
 *
 *******************************************************************************/

/* INCLUDES */

#include <QObject>

#include "simpleWindow.h"

/* CONSTANTS */

/* PROTOTYPES */

namespace QCV
{
/* NAMESPACE PROTOTYPES */
    class CDeviceOpBinderBase;
    class CSeqDeviceControlBase;
    class COperatorBase;
    class CSeqControler;
    class CDisplayWidget;
    class CWindowListView;
    class CParameterEditorDlg;
    class CClockTreeDlg;
    class CKeyEvent;
    class CMouseEvent;
    class CWheelEvent;
    class CRegionSelectedEvent;
    
    class CMainWindow: public CSimpleWindow
    {
        Q_OBJECT
                
    public:
        
        CMainWindow ( CDeviceOpBinderBase * f_binder_p );
        
        virtual ~CMainWindow();
        
    public slots:
        /// Cycle
        virtual void initialize();

        /// Cycle
        virtual void cycle();

        /// Stop
        virtual void stop();

        /// Key Pressed event.
        virtual void keyPressed     ( CKeyEvent * f_event_p );
        virtual void mousePressed   ( CMouseEvent *  f_event_p );
        virtual void mouseReleased  ( CMouseEvent *  f_event_p );
        virtual void mouseMoved     ( CMouseEvent *  f_event_p );
        virtual void wheelTurned    ( CWheelEvent *  f_event_p );        
        virtual void regionSelected ( CRegionSelectedEvent * 
                                      f_event_p );

    //// Protected signals.
    protected:
        void closeEvent ( QCloseEvent *  f_event_p);
        
    //// Private methods.
    private:

        /// Create Base Widget
        void createBaseWidgets();

    private:

        /// Vector of windows.
        CDeviceOpBinderBase *     m_binder_p;

        /// Input device.
        CSeqDeviceControlBase *   m_device_p;

        /// Root operator
        COperatorBase *           m_rootOp_p;

        /// Control Window.
        CSeqControler *           m_controler_p;

        /// Display widget.
        CDisplayWidget *          m_display_p;

        /// Display widget.
        CParameterEditorDlg *     m_paramEditorDlg_p;

        /// Clock Tree Dialog
        CClockTreeDlg *           m_clockTreeDlg_p;
    };
}

#endif // __MAINWINDOW_H