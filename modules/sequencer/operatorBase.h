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

#ifndef __OPERATORBASE_H
#define __OPERATORBASE_H

/**
 *******************************************************************************
 *
 * @file operatorbase.h
 *
 * \class COperatorBase
 * \author Hernan Badino (hernan.badino@gmail.com)
 *
 * \brief Defines the base class for the operators of the framework.
 *
 * The COperatorBase class provides a base for defining operators in the framework.
 * This class provides virtual functions for handling operations input provided by
 * the user or the device controler. The method cycle() is called every time new
 * sequence data is ready for processing. reset() is called when the device
 * controler stops operation (because of user input for example). initialize() is 
 * called when a new sequence data is loaded. exit() is called when the program 
 * exits or the current sequence is closed. Additional functions for handling 
 * mouse and keyboard actions are provided.
 *
 * An operator can have a parent and multiple children. This allows the generation
 * of tree structures. At least one root operator must exist in a functioning 
 * program.
 *
 * Operators also have a parameter set for defining the parameters of the operator.
 * 
 * An operator also might define visual output in the form of drawing lists 
 * (registerDrawingList and getDrawingList methods).
 *
 *******************************************************************************/

/* INCLUDES */
#include <typeinfo>

#include "events.h"

#include "standardTypes.h"
#include "parameterSet.h"
#include "paramMacros.h"
#include "paramBaseConnector.h"
#include "node.h"

#include "drawingListHandler.h"
#include "clockHandler.h"

//#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "events.h"

/* CONSTANTS */

/* PROTOTYPES */
class QWidget;

/* CLASS DEFINITION */

namespace QCV
{
/* PROTOTYPES */        

    class CClockHandler;
    class CClock;
    class CDrawingListHandler;
    class CDrawingList;
    
    class COperatorBase: public CNode
    {
        friend class CMainWindow;

    /// Constructor, Desctructors
    public:    

        /// Constructors.
        COperatorBase ( COperatorBase * const f_parent_p = NULL, 
                        const std::string f_name_str = "Unnamed OperatorBase" );

        /// Virtual destructor.
        virtual ~COperatorBase ();

    /// Sequence Cycle Events
    public:

        /// Show event.
        virtual bool cycle();

        /// Show event.
        virtual bool show();

        /// Init event.
        virtual bool initialize();

        /// Reset event.
        virtual bool reset();

        /// Exit event.
        virtual bool exit();


    /// User Operation Events
    public:

        /// Mouse clicked.
        virtual void mousePressed (   CMouseEvent * f_event_p );

        /// Mouse released.
        virtual void mouseReleased (  CMouseEvent * f_event_p );

        /// Mouse moved.
        virtual void mouseMoved (     CMouseEvent * f_event_p );

        /// Mouse wheel event.
        virtual void wheelTurned (    CWheelEvent * f_event_p );

        /// Key pressed in display.
        virtual void keyPressed (     CKeyEvent * f_event_p );

        /// Region selected in display.
        virtual void regionSelected ( CRegionSelectedEvent * 
                                     f_event_p );

    /// Get/Set methods
    public:

        /// Get list of widgets generated by this operator.
        virtual std::vector<QWidget*> getWidgets( ) const;

        /// Get drawing handler.
        static CDrawingListHandler *  getDrawingListHandler() 
        {
            return &m_drawingListHandler;
        }

        /// Get drawing handler.
        static CClockHandler *        getClockHandler() 
        {
            return &m_clockHandler;
        }

        /// Get the input of this operator.
        virtual COperatorBase*        getParentOp ( ) const { return static_cast<COperatorBase *> (m_parent_p); }
        
        /// Get a child of a given type and name
        template <class _OpType>
        _OpType               getChild ( std::string f_name_str );
        
        /// Get a child of a given type and name
        template <class _OpType>
        _OpType               getChild ( std::string f_name_str ) const;

        /// Get a child based on its index.
        template <class _OpType>
        _OpType               getChild ( int f_idx_i );
        
        /// Get a child based on its index.
        template <class _OpType>
        _OpType               getChild ( int f_idx_i ) const;

    /// Get/Set static methods
    public:
        
    /// I/O registration.
    public:

    /// Parameter handling.
    public:

        /// Get parameter set.
        CParameterSet *   getParameterSet () const;

    /// Protected methods for internal use.
    protected:
        /// Register a drawing list so that is visible and available from the 
        /// beginning.
        void           registerDrawingList ( std::string f_id_str,
                                             S2D<int>    f_position = S2D<int>(0,0), 
                                             bool        f_visibile_b = false,
                                             int         f_overlayLevel_i = 0);

        void           addDrawingListParameter ( std::string f_id_str, 
                                                 std::string f_comment_str = "" );
        
        bool              setScreenSize ( S2D<unsigned int> f_size );
        S2D<unsigned int> getScreenSize (  ) const;

        /// Get a drawing list to draw something.
        CDrawingList * getDrawingList ( std::string f_id_str );

        /// Inform the drawing list handler to update the display.
        void           updateDisplay();

        /// Register a clock so that is visible and available from the 
        /// beginning.
        void           registerClock ( std::string f_name_str );

        /// Get a clock to measure computation time.
        CClock *       getClock ( std::string f_id_str );

        /// Start clock.
        void           startClock ( std::string f_id_str );

        /// Stop clock.
        void           stopClock ( std::string f_id_str );

    /// Support functions for internal use.
    protected:

    /// Private data types
    protected:

    /// Private static members
    private:
        /// Drawing handler
        static CDrawingListHandler        m_drawingListHandler;

        /// Drawing handler
        static CClockHandler              m_clockHandler;

        /// Plots handling.
        /// to be implemented.

    /// Protected static members
    protected:    

        /// OperatorBase's parameter handling.
        CParameterSet *                    m_paramSet_p;

    };


    /// Get a child of a given type and name
    template <typename _OpType>
    _OpType COperatorBase::getChild ( std::string f_name_str )
    {
        for (int i = 0; i < m_children_v.size(); ++i)
        {
            if ( m_children_v[i].ptr_p->getName() == f_name_str )
            {
                _OpType ptr_p = static_cast<_OpType> ( m_children_v[i].ptr_p );
                if (!ptr_p)
                {
                    printf("Operator with name %s is not of type %s\n",
                           f_name_str.c_str(), typeid(ptr_p).name() );
                }
                
                return ptr_p;
            }
        }
        return NULL;
    }

    /// Get a child of a given type and name
    template <typename _OpType>
    _OpType COperatorBase::getChild ( std::string f_name_str ) const
    {
        for (int i = 0; i < m_children_v.size(); ++i)
        {
            if ( m_children_v[i].ptr_p->getName() == f_name_str )
            {
                const _OpType ptr_p = static_cast<_OpType> ( m_children_v[i].ptr_p );
                if (!ptr_p)
                {
                    printf("Operator with name %s is not of type %s\n",
                           f_name_str.c_str(), typeid(ptr_p).name() );
                }
            
                return ptr_p;
            }
        }
        return NULL;
    }

    /// Get a child based on its index.
    template <typename _OpType>
    _OpType COperatorBase::getChild ( int f_idx_i )
    {
        if (f_idx_i < m_children_V[i].size())
        {
            _OpType ptr_p = static_cast<_OpType> ( m_children_v[f_idx_i].ptr_p );
            if (!ptr_p)
            {
                printf("Operator at index %i is not of type %s.\n",
                       f_name_str.c_str(), typeid(ptr_p).name() );
            }
            
            return ptr_p;
        }

        return NULL;
    }

    /// Get a child based on its index.
    template <typename _OpType>
    _OpType COperatorBase::getChild ( int f_idx_i ) const
    {
        if (f_idx_i < m_children_V[i].size())
        {
            const _OpType ptr_p = static_cast<_OpType> ( m_children_v[f_idx_i].ptr_p );
            if (!ptr_p)
            {
                printf("Operator at index %i is not of type %s.\n",
                       f_name_str.c_str(), typeid(ptr_p).name() );
            }
            
            return ptr_p;
        }

        return NULL;
    }

} // Namespace VIC

#endif // __OPERATORBASE_H
