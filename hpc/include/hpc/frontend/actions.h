// => hpc/frontend/actions.h
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#ifndef __human_plus_compiler_fe_actions
#define __human_plus_compiler_fe_actions


namespace hpc {
    namespace frontend {
        
        /*!
         \brief A generic action done by the frontend.
         */
        class FrontendAction {
            
        public:
            virtual ~FrontendAction() {  }
            
            virtual bool doit() = 0;
            
        };
        
    }
}

#endif
