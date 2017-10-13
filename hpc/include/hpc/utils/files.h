// => hpc/utils/files.h
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#ifndef __human_plus_compiler_files
#define __human_plus_compiler_files

#include <llvm/Support/FileSystem.h>
#include <llvm/Support/raw_ostream.h>

#include <string>

namespace hpc {
    
    namespace fsys {
        /*!
         \brief Values indicating the file types handled by the compiler.
         */
        typedef enum {
            Nothing             = 0,  ///< No files
            SourceFile          = 1,  ///< Human Plus source file (.hmn)
            TextualIR           = 2,  ///< LLVM IR textual representation (.ll)
            BitcodeIR           = 3,  ///< LLVM IR bitcode representation (.bc)
            TextualAssembly     = 4,  ///< Assembly textual representation (.s)
            ObjectCode          = 5,  ///< Object file (.o/.obj)
            Archive             = 7,  ///< Archive file (.a)
            HumanPlusKit        = 8,  ///< Human Plus kit (.hmk)
            Executable          = 9,  ///< Native executable (./.exe)
            Unknown             = 10, ///< Files not recognized by the compiler
        } FileType;
        
        
        class File {
        protected:
            /*!
             \brief The file name, either relative or absolute.
             */
            std::string fname;
            /*!
             \brief The type of the file.
             */
            FileType ftype;
            
        public:
            File(std::string fname, FileType ftype) : fname(fname), ftype(ftype) {  }
            
            /*!
             \brief Returns the filename used to locate this file.
             */
            inline const std::string &getFileName() const {
                return fname;
            }
            /*!
             \brief Returns the type of the input file.
             */
            inline FileType getType() const {
                return ftype;
            }
            /*!
             \brief Returns the llvm file status for this file, or \c nullptr if there was an error getting it.
             \param errc The error code for any occurred error will be passed through this param.
             */
            llvm::sys::fs::file_status *getFileStatus(std::error_code *errc = nullptr);
            /*!
             \brief Returns whether the file exists.
             */
            bool exists();
            
            inline bool operator ==(File &that) { ///< They point to the same file.
                return llvm::sys::fs::equivalent(fname, that.fname);
            }
            
        };
        
        /*!
         \brief Object describing an input file taken by the compiler.
         */
        class InputFile : public File {
            
        public:
            InputFile(std::string fname, FileType ftype) : File(fname, ftype) {  }
            
            /*!
             \brief Creates an \c InputFile object for the file with the given filename. The object will be created and returned no matter whether the file does not exists or the file type is unknown.
             \note Platform-dependent file types will be returned with \c Unknown .
             */
            static InputFile *fromFile(std::string fname);
            
        };
        
        /*!
         \brief Object describing an output file emitted by the compiler.
         */
        class OutputFile : public File {
            /*!
             \brief The output stream that will write to the output file.
             */
            llvm::raw_pwrite_stream *stream = nullptr;
            /*!
             \brief A boolean indicating whether the file should be kept after the compiler process is terminated.
             */
            bool shouldKeep;
            
            /*!
             \brief Initializes the output file with the file name (either absolute or relative to the process current dir), the output stream that will write to it, the file type, and whether this file should be deleted at the end of the compiler process.
             */
            OutputFile(std::string fname, llvm::raw_pwrite_stream *stream, FileType ftype, bool shouldKeep = true);
            
        public:
            ~OutputFile();
            
            /*!
             \brief Declares that this file should be definitely be kept in the file system even after the process.
             */
            void keep(bool sKeep = true);
            /*!
             \brief Returns the output stream that can write to the output file.
             */
            inline llvm::raw_pwrite_stream &os() const {
                return *stream;
            }
            
            /*!
             \brief Creates a new output file.
             \param filename The file pathname, either absolute or relative to the current dir
             \param ftype A \c FileType value indicating the extension of the new file
             \param errcode If there is an error, the function returns \c nullptr and an explaining error code will be put in this parameter
             \param makeTemp If \c true, the OutputFile will be declared as temporary and be put in the system temp dir, and the file will be deleted when the object destructor is called
             
             \note Remember to keep the object in memory as long as you need to keep the file on the file system if you pass \c makeTemp as \c true, or call \c keep() on the returned object.
             */
            static OutputFile *create(std::string filename, FileType ftype, std::error_code &errcode, bool makeTemp = false);
            /*!
             \brief Creates a new temporary output file that will be put in the system temp directorys.
             \param name A simple name for the file, which will be followed
             \param ftype A \c FileType value indicating the extension of the new file
             \param errcode If there is an error, the function returns \c nullptr and an explaining error code will be put in this parameter
             
             \note Remember to keep the object in memory as long as you need to keep the file on the file system, or call \c keep() on the returned object.
             */
            static OutputFile *createTemp(std::string name, FileType ftype, std::error_code &errcode);
            
        };

    }
    
}

#endif
