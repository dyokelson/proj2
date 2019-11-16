//
// Created by dewimaharani on 11/11/19.
//

#ifndef PROJ2_STATICSEMANTICS_H
#define PROJ2_STATICSEMANTICS_H

#include <map>
using namespace std;


class ClassNode {

public:
    string name;
    // keep track of parents and children
    // attributes of the class

};

//struct for returning from the check() method
struct tablepointers {
    map<string, ClassNode> *class_hierarchy;
    map<string, string> *var_types;
};

class StaticSemantics {

    tablepointers tp;
    // hashtable for class hierarchy
    map<string, ClassNode> class_hierarchy;
    // hashtable for variables and types
    map<string, string> var_types;
    // indicates an error occurred during static semantics check and should return nullptr
    bool error = false;

public:
    // this method takes the AST as input and returns a struct of pointers to the two tables
    tablepointers check(AST::ASTNode *root) {
        // build class hierarchy
        build_class_hierarchy(root);
        // check vars are initialized - do we need to return anything?
        check_init(root);
        // type inference
        type_inference(root);
        // if no errors
        //if (error == true) {
        //    return nullptr;
       // }
        //else {
        return tp;
        //}

    };

    // update the this.class_hierarchy table
    void build_class_hierarchy(AST::ASTNode *root) {

    }

   void check_init(AST::ASTNode *root) {

    };

    // update the this.var_types table
    void type_inference(AST::ASTNode *root) {

    };

    // return the least common ancestor of the two classes
    string lca(string type1, string type2) {
        // find these two classes on the class hierarchy table and return the lca
        // if error, return Top
        return "Hello";
    }
};


#endif //PROJ2_STATICSEMANTICS_H
