//
//  bufferer.hpp
//  HumanPlus
//
//  Created by Lore on 10/05/16.
//  Copyright © 2016 Lore. All rights reserved.
//

#ifndef __human_plus_compiler_lexer_bufferer
#define __human_plus_compiler_lexer_bufferer

#include <hpc/analyzers/sources.h>

namespace hpc {
    namespace lexer {
        /*!
         \brief Reads the next character from the bound source file stream. Assertion will occur if no source file was bound to the lexer.
         */
        //source::sourcechar fetch();
        /*!
         \brief Returns the number of characters fetched from the bound source file by the lexer since the beginning or since \c lexer::resetFetchCount() was called.
         */
        //unsigned long getFetchCount();
        /*!
         \brief Resets the number of fetches made by the lexer on the bound source file.
         \param alreadyStarted A boolean saying whether the lexer should assume the current character should be included in the count.
         */
        //void resetFetchCount(bool alreadyStarted = true);
        
    }
}

#endif
