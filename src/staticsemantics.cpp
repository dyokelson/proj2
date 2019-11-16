//
// Created by dewimaharani on 11/11/19.
//

//#include "staticsemantics.h"
#include <map>
#include <vector>
#include "ASTNode.h"

using namespace std;

class MethodNode {

public:
    string name;
    string return_type;
    //vector<string> formal_arg_types;
    // table of local variables (anything passed in, defined inside or class level instance vars)
    map<string, string> local_vars;

    MethodNode() {
    }

    MethodNode(AST::Method *method) {
        name = method->name_.str();
        return_type = method->returns_.str();
        local_vars = map<string, string>();

        AST::Formals formals = method->formals_;
        if (formals.elements_.size() != 0) {
            vector<AST::Formal *> formal_list = formals.elements_;
            for (AST::Formal *formal: formal_list) {
                local_vars[formal->var_.str()] = formal->type_.str();
            }
        }

    }
};

// okay to build with minimal information now, as long as do a type inference/check topologically
class ClassNode {
    public:
        string name;
        // keep track of parents and children for the class hierarchy
        string parent;
        // attributes of the class (vars, methods)
        // type check the constructor before the methods
            // needs to initialize the same types as inherited class
        vector<string> instance_vars; // names of the variables,
        vector<MethodNode*> methods;
        // constructor
        MethodNode constructor;

        ClassNode() {

        }

        ClassNode(string name) { // constructor takes a name
            name = name;
            parent = "";
            instance_vars = vector<string>();
            methods = vector<MethodNode*>();
        }

        // TODO do I need all these or should I just access the public fields and push later?
        void add_parent(string parent) {
            parent = parent;
        }

        void add_inst_var(string inst_var) {
            instance_vars.push_back(inst_var);
        }

        void add_method(MethodNode* method) {
            methods.push_back(method);
        }
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
    // hashtable for variables and types - scopes!
    map<string, string> var_types;
    // indicates an error occurred during static semantics check and should return nullptr
    bool error = false;

    public:
        StaticSemantics(AST::ASTNode *root) { // default constructor
            root = root;
            class_hierarchy = map<string, ClassNode>();
        }

        // this method takes the AST as input and returns a struct of pointers to the two tables
        tablepointers *check(AST::ASTNode *root) { // TODO this doesn't need to take the root passed in anymore
            // can stop if I find any error in any of these steps at least for now
            // build class hierarchy
            build_class_hierarchy(root);
            // check vars are initialized - if error stop
            check_init(root);
            // if error
            // type inference
            type_inference(root); // do this to the methods within a class, topologically because of instance variables
            // make sure not doing anything that doesn't fit that type of instance variables - also trying to do something wrong just within the method
            //type inference for a method gets a block of statements, with list of local variables
            // everything starts at bottom - did anything change? pseudo code
            // pass a context that holds variables, and a flag to say if changed or not
            // add to the context as things change (recursive walk of the tree)
            // create a pseudo class (maybe) type inference can be done on a fake class or not, method inference with no context, no instance variables
            // if no errors
            //if (error == true) {
            //    return nullptr;
            // }
            //else {
            return &tp;
            //}

    };

    // update the class_hierarchy table
    void build_class_hierarchy(AST::ASTNode *root) {
        // cast root to a Program node
        AST::Program *root_node = (AST::Program*) root;
        // get the classes
        AST::Classes classes = root_node->classes_;
        vector<AST::Class *> class_list = classes.elements_;
        for (AST::Class *clazz: class_list) {
            // first pass just builds the classes - not check to see if they're already there
            // cycles or anything that "extends" that isn't there (2nd pass)
            // Obj just doesn't have a parent
            string cls_name = (*clazz).name_.text_;
            ClassNode new_class = ClassNode(cls_name);
            // populate all the things!
            new_class.parent = clazz->super_.text_;
            AST::ASTNode *constr = &clazz->constructor_;
            AST::Method *construct = (AST::Method*) constr;
            new_class.constructor = MethodNode(construct);
            vector<AST::Method *> method_list = clazz->methods_.elements_;
            for (AST::Method *method: method_list) {
                MethodNode new_method = MethodNode(method);
                new_class.add_method(&new_method);
            }
            class_hierarchy[cls_name] = new_class;
        }
        // now go through the class_hierarchy again and check for cycles and extends

    }

    void check_init(AST::ASTNode *root) {

    }

    // update the this.var_types table
    void type_inference(AST::ASTNode *root) {

    }

    // return the least common ancestor of the two classes
    string lca(string type1, string type2) {
        // find these two classes on the class hierarchy table and return the lca
        // if error, return Top
        return "Hello";
    }
};