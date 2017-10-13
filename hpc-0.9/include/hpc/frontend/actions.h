//
//  actions.h
//  HumanPlus
//
//  Created by Lore on 27/01/2017.
//  Copyright © 2017 Lore. All rights reserved.
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
