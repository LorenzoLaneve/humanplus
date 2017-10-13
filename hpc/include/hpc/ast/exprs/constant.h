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
        class CharLiteral : public Literal<runtime::utf7_char_ty> {
        public:
            CharLiteral(const runtime::utf7_char_ty val) : Literal(val) {  };
            
            virtual Type *evalType();
            
            
            llvm_rtti_impl(CharLiteral);
        };
        
        /*!
         \brief An object describing an \c integer literal.
         */
        class IntegerLiteral : public Literal<runtime::int32_ty> {
        public:
            IntegerLiteral(const runtime::int32_ty val) : Literal(val) {  };
            
            virtual bool isNullPointer() const { return !val; }
            
            virtual Type *evalType();
            
            
            llvm_rtti_impl(IntegerLiteral);
        };
        
        /*!
         \brief An object describing an \c unsigned &nbsp;\c integer literal.
         */
        class UIntegerLiteral : public Literal<runtime::uint32_ty> {
        public:
            UIntegerLiteral(const runtime::uint32_ty val) : Literal(val) {  };
            
            virtual Type *evalType();
            
            
            llvm_rtti_impl(UIntegerLiteral);
        };
        
        /*!
         \brief An object describing a \c long literal.
         */
        class LongLiteral : public Literal<runtime::int64_ty> {
        public:
            LongLiteral(const runtime::int64_ty val) : Literal(val) {  };
            
            virtual Type *evalType();
            
            
            llvm_rtti_impl(LongLiteral);
        };
        
        /*!
         \brief An object describing an \c unsigned &nbsp;\c long literal.
         */
        class ULongLiteral : public Literal<runtime::uint64_ty> {
        public:
            ULongLiteral(const runtime::uint64_ty val) : Literal(val) {  };
            
            virtual Type *evalType();
            
            
            llvm_rtti_impl(ULongLiteral);
        };
        
        /*!
         \brief An object describing a \c float literal.
         */
        class FloatLiteral : public Literal<runtime::fp_single_ty> {
        public:
            FloatLiteral(const runtime::fp_single_ty val) : Literal(val) {  };
            
            virtual Type *evalType();
            
            
            llvm_rtti_impl(FloatLiteral);
        };
        
        /*!
         \brief An object describing a \c double literal.
         */
        class DoubleLiteral : public Literal<runtime::fp_double_ty> {
        public:
            DoubleLiteral(const runtime::fp_double_ty val) : Literal(val) {  };
            
            virtual Type *evalType();
            
            
            llvm_rtti_impl(DoubleLiteral);
        };
        
        /*!
         \brief An object describing a \c boolean literal.
         */
        class BoolLiteral : public Literal<runtime::boolean_ty> {
        public:
            BoolLiteral(const runtime::boolean_ty val) : Literal(val) {  };
            
            virtual Type *evalType();
            
            
            llvm_rtti_impl(BoolLiteral);
        };
        
        /*!
         \brief An object describing a \c string literal.
         */
        class StringLiteral : public Literal<runtime::string_ty> {
        public:
            StringLiteral(const runtime::string_ty val) : Literal(val) {  };
            
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
