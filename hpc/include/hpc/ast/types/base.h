// => hpc/ast/types/base.h
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#ifndef __human_plus_compiler_ast_type_base
#define __human_plus_compiler_ast_type_base

#include <hpc/ast/unit.h>
#include <hpc/ast/symbols.h>

#include <llvm/IR/Type.h>

#include <string>

namespace hpc {
    namespace ast {
        
        class FieldDecl;
        class TypeRef;
        class PointerType;
        class TypeAliasDecl;
        class QualifiedType;
        
        class TypeQualifiers {
            
            unsigned constQual : 1;
            unsigned volatileQual : 1;
            
        public:
            TypeQualifiers() : constQual(0), volatileQual(0) {  }
            
            inline bool isDefault() const { return isConstant() && isVolatile(); }
            
            void setConstant(bool newConstQual) {
                constQual = newConstQual;
            }
            
            inline bool isConstant() const { return constQual; }
            
            void setVolatile(bool newVolatileQual) { volatileQual = newVolatileQual; }
            
            inline bool isVolatile() const {
                return volatileQual;
            }
            
        };
        
        /*!
         \brief Value indicating how the data is formatted conforming to the type.
         */
        typedef enum {
            /*!
             \brief Value indicating that the associated type does not use any byte.
             */
            TypeFormatVoid,
            /*!
             \brief Value indicating that the associated type is a boolean type.
             */
            TypeFormatBoolean,
            /*!
             \brief Value indicating that the associated type is a signed integer type.
             */
            TypeFormatSignedInteger,
            /*!
             \brief Value indicating that the associated type is an unsigned integer type.
             */
            TypeFormatUnsignedInteger,
            /*!
             \brief Value indicating that the associated type is either a signed or unsigned integer type.
             */
            TypeFormatInteger,
            /*!
             \brief Value indicating that the associated type is an IEEE 754 floating point standard.
             */
            TypeFormatFloatingPoint,
            /*!
             \brief Value indicating that the associated type is the string type.
             */
            TypeFormatString,
            /*!
             \brief Value indicating that the associated type is not a simple type, and may contain other types (structs, classes).
             */
            TypeFormatCompound,
            /*!
             \brief Value indicating that the associated type is a memory address.
             */
            TypeFormatPointer,
            /*!
             \brief Value indicating that the associated type is a function pointer.
             */
            TypeFormatFunction
        } TypeFormat;
        
        
        /*!
         \brief Values indicating the qualifiers that can be applied to types.
         */
        typedef enum {
            /*!
             \brief \c constant qualifier.
             */
            TypeQualConstant = 1,
            /*!
             \brief \c volatile qualifier.
             */
            TypeQualVolatile = 1 << 1
        } TypeQuals;
        
        
        /*!
         \brief Base class for all the types contained in the type system.
         */
        class Type : public Component {
            
            
        public:
            virtual ~Type() {  }
            
            /*!
             \brief A type object describing the type without any reference or aliasing.
             */
            virtual Type *getCanonicalType() = 0;
            
            /*!
             \brief Returns whether the type is canonical.
             */
            virtual bool isCanonicalType() const = 0;
            
            /*!
             \brief Returns the \c TypeFormat indicating the type of data held by this type.
             \see \c TypeFormat enum for possible values that this method may return.
             */
            virtual TypeFormat getFormat() const = 0;
            
            
            /*!
             \brief Returns a boolean indicating whether this type can be casted to \c type.
             \param type The destination type of the casting
             \param explicitly A boolean indicating whether the cast is explicit or implicit. An explicit cast is specified in the source code by the user, and there may be cases where an explicit cast is permitted while an implicit cast is not.
             */
            virtual bool canCastTo(Type *type, bool explicitly = false);
            /*!
             \brief Returns a boolean indicating whether this type can be assigned to a memory entity of type \c type.
             \param type The destination type of the casting
             \note There may be cases where a simple cast is permitted while a complete assignment is not.
             */
            virtual bool canAssignTo(Type *type);
            
            
            /*!
             \brief Returns the type pointed by this type, if this type is a pointer type. This method should be overridden by classes indicating pointer types.
             */
            virtual Type *getPointedType() const { return nullptr; }
            /*!
             \brief Returns a pointer type to this type.
             */
            PointerType *pointerTo();
            
            /*!
             \brief Returns the member with the given identifier contained in this type, or \c nullptr if this type doesn't contain a member with this identifier.
             */
            virtual FieldDecl *getMember(std::string memberID) { return nullptr; }
            
            
            /*!
             \brief Returns whether this type is the void type.
             */
            inline bool isVoidType() const { return getFormat() == TypeFormatVoid; }
            /*!
             \brief Returns whether this type is the boolean type.
             */
            inline bool isBooleanType() const { return getFormat() == TypeFormatBoolean; }
            /*!
             \brief Returns whether this type is a signed integer type.
             */
            inline bool isSignedIntegerType() const { return getFormat() == TypeFormatSignedInteger; }
            /*!
             \brief Returns whether this type is an unsigned integer type.
             */
            inline bool isUnsignedIntegerType() const { return getFormat() == TypeFormatUnsignedInteger; }
            /*!
             \brief Returns whether this type is an integer type.
             */
            inline bool isIntegerType() const {
                TypeFormat format = getFormat();
                return format == TypeFormatSignedInteger || format == TypeFormatUnsignedInteger;
            }
            /*!
             \brief Returns whether this type is a floating point type.
             */
            inline bool isFloatingPointType() const { return getFormat() == TypeFormatFloatingPoint; }
            /*!
             \brief Returns whether this type is a numeric type, whether integer or real.
             */
            inline bool isNumericType() const { return isIntegerType() || isFloatingPointType(); }
            /*!
             \brief Returns whether this type is a pointer type.
             */
            inline bool isPointerType() const { return getFormat() == TypeFormatPointer; }
            /*!
             \brief Returns whether this type is a null pointer type.
             */
            inline bool isNullPointerType() const { return isPointerType() && !getPointedType(); }
            
            /*!
             \brief Returns an \c ast::Type object which should be used when a type cannot be determined.
             */
            static Type *getPlaceholderType();
            
            /*!
             \brief Returns a source-like representation of the type.
             \param quoted Whether the string should have quotes.
             */
            virtual std::string str(bool quoted = true) = 0;
            
            /*!
             \brief Returns a string describing the type. If this type is not canonical, additional information about the canonical type will be added. Useful for diagnostics and dumpings.
             */
            std::string asString();
            
            /*!
             \brief Returns the type with a \c constant qualifier.
             */
            QualifiedType *withConstantQualifier();
            
            
            /*!
             \brief Returns whether the instances of this type cannot be changed after declaration (\c constant ).
             */
            virtual bool isConstant() const { return false; }
            
            /*!
             \brief Returns a boolean indicating whether the two given types represent the exact same type.
             */
            static bool areEquivalent(Type &type1, Type &type2);
            
            static inline bool areEquivalent(Type *type1, Type *type2) {
                return areEquivalent(*type1, *type2);
            }
            
            inline bool operator==(Type &type2) {
                return areEquivalent(*this, type2);
            }
            
            
            llvm_rtti_impl_superclass(Type);
        };
        
        
        /*!
         \brief Class implementing a type encloser. This is a base class for types which reflect an enclosing type, adding information to it.
         */
        class TypeEncloser : public Type {
            
        protected:
            Type *theType = nullptr;
            
            TypeEncloser(Type *theType = nullptr) : theType(theType) {  }
            
            virtual ~TypeEncloser() {  }
            
        public:
            inline Type *getEnclosingType() const { return theType; }
            
            inline void setEnclosingType(Type *newType) { theType = newType; }
            
            virtual inline Type *getCanonicalType() {
                assert(theType && "No type found.");
                return theType->getCanonicalType();
            }
            
            virtual inline bool isCanonicalType() const {
                assert(theType && "No type found.");
                return theType->isCanonicalType();
            }
            
            inline TypeFormat getFormat() const {
                assert(theType && "No type found.");
                return theType->getFormat();
            }
            
            inline bool canCastTo(Type *type, bool explicitly = false) {
                assert(theType && "No type found.");
                return theType->canCastTo(type, explicitly);
            }
            
            inline bool canAssignTo(Type *type) {
                assert(theType && "No type found.");
                return theType->canAssignTo(type);
            };
            
            inline Type *getPointedType() const {
                assert(theType && "No type found.");
                return theType->getPointedType();
            }
            
            inline FieldDecl *getMember(std::string memberID) {
                assert(theType && "No type found.");
                return theType->getMember(memberID);
            }

            virtual inline bool isConstant() const {
                assert(theType && "No type found.");
                return theType->isConstant();
            }
            
            llvm_rtti_impl_superclass(TypeEncloser);
            
        };
        
        
        /*!
         \brief Class implementing types that needs to be resolved in a second moment.
         */
        class TypeRef : public TypeEncloser {
            
            /*!
             \brief The symbol indicating the path to the type to lookup.
             */
            Symbol pathToType;
            /*!
             \brief The found type, or \c nullptr if the lookup has not been done yet or the entity has not been found.
             */
            //Type *theType = nullptr;
            
        public:
            TypeRef(Symbol pathToType) : pathToType(pathToType) {  }
            
            inline const Symbol &getSymbolPath() const { return pathToType; }
            
            inline Type *getType() const { return getEnclosingType(); }
            
            inline void setType(Type *newType) { setEnclosingType(newType); }
            
            inline void setPlaceholderType() { setType(Type::getPlaceholderType()); }
            
            std::string str(bool quoted = true);


            llvm_rtti_impl(TypeRef);
        };
        
        
        /*!
         \brief Class implementing dummy types for type aliasing.
         */
        class AliasedType : public TypeEncloser {
            
            /*!
             \brief The declaration that created this type.
             */
            TypeAliasDecl *declaration;
            
        public:
            AliasedType(TypeAliasDecl *declaration, Type *originalType) : TypeEncloser(originalType), declaration(declaration) {  }
            
            inline Type *getOriginalType() const { return getEnclosingType(); }
            
            inline bool isCanonicalType() const { return false; }
            
            std::string str(bool quoted = true);
            

            llvm_rtti_impl(AliasedType);
        };
        

        /*!
         \brief Intermediate type which adds qualifiers to the enclosing type.
         */
        class QualifiedType : public TypeEncloser {
            
            unsigned constQual : 1;

        public:
            QualifiedType(Type *theType = nullptr) : TypeEncloser(theType) {  }
            
            void setType(Type *newType) { setEnclosingType(newType); }
            
            void setConstant(bool newConstQual) { constQual = newConstQual; }
            
            inline bool isConstant() const { return constQual; }
            
            Type *getCanonicalType();

            /*!
             \brief Creates a new QualifiedType object with the same qualifiers enclosing the given type.
             */
            QualifiedType *cloneQuals(Type *encType) const;
            
            std::string str(bool quoted = true);
            

            llvm_rtti_impl(QualifiedType);
        };
        
    }
}

#endif
