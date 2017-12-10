// => hpc/ast/exprs/constant.h
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#ifndef __human_plus_compiler_ast_constant
#define __human_plus_compiler_ast_constant

#include <hpc/ast/exprs/expression.h>
#include <hpc/runtime/runtime.h>

namespace hpc {
    namespace ast {
        
        /*!
         \brief Base class for all the constant values.
         */
        class Constant : public Expr {
            
        public:
            virtual ~Constant() {  }
            
            
            llvm_rtti_impl_superclass(Constant);
        };
        
        /*!
         \brief Abstract base class for all the constant literal classes.
         */
        template <typename T> class Literal : public Constant {
        protected:
            /*!
             \brief The value containing the parsed literal constant.
             */
            T val;
            
        public:
            /*!
             \brief Initializes the literal with the constant value.
             */
            Literal(const T val) : val(val) {  };
            ~Literal() {  }
            
            virtual Type *evalType() = 0;
            
            /*!
             \brief Returns the value containing the parsed literal constant.
             */
            inline T getValue() const { return val; };
        };
        
        /*!
         \brief An object describing a \c char literal.
         */
        class CharLiteral : public Literal<rt::utf7_char_ty> {
        public:
            CharLiteral(const rt::utf7_char_ty val) : Literal(val) {  };
            
            virtual Type *evalType();
            
            
            llvm_rtti_impl(CharLiteral);
        };
        
        /*!
         \brief An object describing an \c integer literal.
         */
        class IntegerLiteral : public Literal<rt::int32_ty> {
        public:
            IntegerLiteral(const rt::int32_ty val) : Literal(val) {  };
            
            virtual bool isNullPointer() const { return !val; }
            
            virtual Type *evalType();
            
            
            llvm_rtti_impl(IntegerLiteral);
        };
        
        /*!
         \brief An object describing an \c unsigned &nbsp;\c integer literal.
         */
        class UIntegerLiteral : public Literal<rt::uint32_ty> {
        public:
            UIntegerLiteral(const rt::uint32_ty val) : Literal(val) {  };
            
            virtual Type *evalType();
            
            
            llvm_rtti_impl(UIntegerLiteral);
        };
        
        /*!
         \brief An object describing a \c long literal.
         */
        class LongLiteral : public Literal<rt::int64_ty> {
        public:
            LongLiteral(const rt::int64_ty val) : Literal(val) {  };
            
            virtual Type *evalType();
            
            
            llvm_rtti_impl(LongLiteral);
        };
        
        /*!
         \brief An object describing an \c unsigned &nbsp;\c long literal.
         */
        class ULongLiteral : public Literal<rt::uint64_ty> {
        public:
            ULongLiteral(const rt::uint64_ty val) : Literal(val) {  };
            
            virtual Type *evalType();
            
            
            llvm_rtti_impl(ULongLiteral);
        };
        
        /*!
         \brief An object describing a \c float literal.
         */
        class FloatLiteral : public Literal<rt::fp_single_ty> {
        public:
            FloatLiteral(const rt::fp_single_ty val) : Literal(val) {  };
            
            virtual Type *evalType();
            
            
            llvm_rtti_impl(FloatLiteral);
        };
        
        /*!
         \brief An object describing a \c double literal.
         */
        class DoubleLiteral : public Literal<rt::fp_double_ty> {
        public:
            DoubleLiteral(const rt::fp_double_ty val) : Literal(val) {  };
            
            virtual Type *evalType();
            
            
            llvm_rtti_impl(DoubleLiteral);
        };
        
        /*!
         \brief An object describing a \c boolean literal.
         */
        class BoolLiteral : public Literal<rt::boolean_ty> {
        public:
            BoolLiteral(const rt::boolean_ty val) : Literal(val) {  };
            
            virtual Type *evalType();
            
            
            llvm_rtti_impl(BoolLiteral);
        };
        
        /*!
         \brief An object describing a \c string literal.
         */
        class StringLiteral : public Literal<rt::string_ty> {
        public:
            StringLiteral(const rt::string_ty val) : Literal(val) {  };
            
            virtual Type *evalType();
            
            
            llvm_rtti_impl(StringLiteral);
        };
        
        /*!
         \brief An object describing a \c nothing literal.
         */
        class NullPointer : public Constant {
            /*!
             \brief The type of the null pointer.
             */
            Type *pointerType;
            
        public:
            /*!
             \brief Initializes the null pointer with the type this null pointer should be casted to.
             */
            NullPointer(Type *pointerType) : pointerType(pointerType) {  }
            virtual ~NullPointer() {  }
            
            inline Type *getPointerType() const { return pointerType; }
            
            virtual bool isNullPointer() const { return true; }
            
            virtual Type *evalType();
            
            
            llvm_rtti_impl(NullPointer);
        };
        
    }
}

#endif
