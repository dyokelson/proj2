//
// Created by Michal Young on 10/23/18.
//

#include "ASTBuiltIns.h"

static AST::Class Nothing = AST::Class(
        *new AST::Ident("Nothing"), // Name
        *new AST::Ident("Obj"),     // Superclass
        *new AST::Method(*new AST::Ident("Construct"), *new AST::Formals(),
                *new AST::Ident("Nothing"), *new AST::Block()),  // Constructor
        *new AST::Methods() // Zero methods
        );

static AST::Class Obj = AST::Class(
        *new AST::Ident("Obj"), // Name
        *new AST::Ident("None"),     // Superclass
        *new AST::Method(*new AST::Ident("Construct"), *new AST::Formals(),
                         *new AST::Ident("Obj"), *new AST::Block()),  // Constructor
        *new AST::Methods() // Zero methods
        // Method(Ident& name, Formals& formals, ASTNode& returns, Block& statements)
//        *new AST::Methods(*new AST::Method(*new AST::Ident("PRINT"), *new AST::Formals(), *new AST::Ident("Nothing"),
//                *new AST::Block()),
//                          *new AST::Method(*new AST::Ident("STR"), *new AST::Formals(), *new AST::Ident("String"),
//                                           *new AST::Block())) // TODO add print and string methods
);

static AST::Class Boolean = AST::Class(
        *new AST::Ident("Boolean"), // Name
        *new AST::Ident("Obj"),     // Superclass
        *new AST::Method(*new AST::Ident("Construct"), *new AST::Formals(),
                         *new AST::Ident("Boolean"), *new AST::Block()),  // Constructor
        *new AST::Methods()
);

static AST::Class Int = AST::Class(
        *new AST::Ident("Int"), // Name
        *new AST::Ident("Obj"),     // Superclass
        *new AST::Method(*new AST::Ident("Construct"), *new AST::Formals(),
                         *new AST::Ident("Int"), *new AST::Block()),  // Constructor
        *new AST::Methods()
);

static AST::Class String = AST::Class(
        *new AST::Ident("String"), // Name
        *new AST::Ident("Obj"),     // Superclass
        *new AST::Method(*new AST::Ident("Construct"), *new AST::Formals(),
                         *new AST::Ident("String"), *new AST::Block()),  // Constructor
        *new AST::Methods()
);

//static AST::Ident AST_builtins::bi_true = AST::Ident("true");
//static AST::Ident AST_builtins::bi_false = AST::Ident("false");
//static AST::Ident AST_builtins::bi_none = AST::Ident("None");
/*
namespace AST_builtins {

    // The built-in classes
    extern AST::Class Obj;
    extern AST::Class Nothing;
    extern AST::Class String;
    extern AST::Class Boolean;
    extern AST::Class Int;

    // The built-in identifiers
    extern AST::Ident bi_none;
    extern AST::Ident bi_true;
    extern AST::Ident bi_false;

}*/
