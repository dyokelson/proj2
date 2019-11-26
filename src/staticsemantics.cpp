//
// Created by dewimaharani on 11/11/19.
//

//#include "staticsemantics.h"
#include <iostream>
#include <map>
#include <vector>
#include <stack>
#include <list>
#include "string.h"
#include "ASTNode.h"

using namespace std;

class MethodNode {

public:
    string name;
    string return_type;
    //vector<string> formal_arg_types;
    // table of local variables (anything passed in, defined inside or class level instance vars)
    map<string, string> local_vars;
    //vector<AST::Statement> body;

    MethodNode() {
    }

    MethodNode(AST::Method *method) {
        name = method->name_.text_;

        return_type = method->returns_.str();
        local_vars = map<string, string>();
        //body = vector<AST::Statement>();

        AST::Formals formals = method->formals_;
        if (formals.elements_.size() != 0) {
            vector<AST::Formal *> formal_list = formals.elements_;
            for (AST::Formal *formal: formal_list) {
                local_vars[formal->var_.str()] = formal->type_.str();
            }
        }
    }
    //string type_check(AST::Method ast_method, map<string, string> context, ClassNode classNode) {


   // }

};

// okay to build with minimal information now, as long as do a type inference/check topologically
class ClassNode {
    public:
        string name_;
        // keep track of parents and children for the class hierarchy
        string parent_;
        // attributes of the class (vars, methods)
        // type check the constructor before the methods
            // needs to initialize the same types as inherited class
        map<string, string> instance_vars; // names of the variables,
        map<string, MethodNode*> methods;
        // constructor
        MethodNode constructor_;
        bool visited;
        bool resolved;

        ClassNode() {

        }

        //explicit ClassNode(string name, string parent):  name_{string::strdup(name)}, parent_{string::strdup(parent)}{ // constructor takes a name
        ClassNode(string name, string parent) {
            name_ = name;
            parent_ = parent;
            visited = false;
            resolved = false;
            constructor_ = MethodNode();
            instance_vars = map<string, string>();
            methods = map<string, MethodNode*>();
        }

        // TODO do I need all these or should I just access the public fields and push later?
        void add_parent(string parent) {
            parent = parent;
        }

        void add_method(MethodNode* method) {
            methods[method->name] = method;
        }
};

//struct for returning from the check() method
struct tablepointers {
    map<string, ClassNode> class_hierarchy;
    map<string, string> *var_types;
};

class StaticSemantics {

public:
    tablepointers tp;
    // hashtable for class hierarchy
    map <string, ClassNode> class_hierarchy;
    // once sorted
    vector <ClassNode> sorted_classes;
    // hashtable for variables and types - scopes!
    map <string, string> var_types;
    // indicates an error occurred sometime during static semantics check and should return nullptr
    bool error = false;
    AST::ASTNode * root;

    StaticSemantics(AST::ASTNode *root) { // default constructor
        root = root;
        class_hierarchy = map<string, ClassNode>();
        sorted_classes = vector<ClassNode>();
        var_types = map<string, string>();
    }

    // A recursive function used by topologicalSort
    void topologicalSortRec(ClassNode* cn)
    {
        if (cn->resolved) {
            std::cout << " Found Resolved Class " <<cn->name_ << endl;
            return;
        }
        if (cn->visited) {
            std::cout << " Class " <<cn->name_ << " in a cycle with " << cn->parent_ << endl;
            error = true;
            return;
        }
        cn->visited = true;
        string cur_parent = cn->parent_;
        string cur_class = cn->name_;
        //std::cout << " Class " << cur_class << " getting sorted, parent: " << cur_parent<<endl;
        if (class_hierarchy.count(cur_parent)) {
            topologicalSortRec(&(class_hierarchy[cur_parent]));
            cn->resolved = true;
            sorted_classes.push_back(*cn);
        } else {
            std::cout << " Class " << cur_class << " extends undefined class " << cur_parent<<endl;
        }

}

    // this method takes the AST as input and returns a struct of pointers to the two tables
    tablepointers *check(AST::ASTNode *root) { // TODO this doesn't need to take the root passed in anymore

        // build class hierarchy
        build_class_hierarchy(root);

        // check vars are initialized
        if (error) {
            return nullptr;
        } else {
            check_init(root);
        }
        // type inference
        if (error) {
            return nullptr;
        } else {
            type_inference(root);
        }
        if (error) {
            return nullptr;
        }

        return &tp;


    };

    // update the class_hierarchy table
    void build_class_hierarchy(AST::ASTNode *root) {
        std::cout << "Building Class Hierarchy \n" << std::flush;
        // cast root to a Program node
        AST::Program *root_node = (AST::Program*) root;
        // get the classes
        AST::Classes classes = root_node->classes_;
        vector<AST::Class *> class_list = classes.elements_;
        for (AST::Class *clazz: class_list) {
            // first pass just builds the classes as is
            string cls_name = clazz->name_.text_;
            string par_name = clazz->super_.text_;
            //std::cout << " Class and PAR " << cls_name << par_name <<endl;
            ClassNode new_class = ClassNode(cls_name, par_name);

            // populate all the things!
            AST::ASTNode *constr = &(clazz->constructor_);
            AST::Method *construct = (AST::Method *) constr;
            new_class.constructor_ = MethodNode(construct);
            new_class.instance_vars = new_class.constructor_.local_vars; //constructors variables are the class level - need this?

            vector < AST::Method * > method_list = clazz->methods_.elements_;
            for (AST::Method *method: method_list) {
                MethodNode new_method = MethodNode(method);
                new_class.add_method(&new_method);
            }

            std::cout << "Class " << cls_name << " created class node with name: " << new_class.name_ << " and parent " << new_class.parent_ <<"\n" << std::flush;
            class_hierarchy[cls_name] = new_class;
        } // end for class in classes

        // Create Built-In Classes
        ClassNode obj_node = ClassNode("Obj", "None");
        obj_node.visited = true;
        obj_node.resolved = true;
        class_hierarchy["Obj"] = obj_node;
        sorted_classes.push_back(obj_node);
        ClassNode string_node = ClassNode("String", "Obj");
        class_hierarchy["String"] = string_node;
        ClassNode bool_node = ClassNode("Boolean", "Obj");
        class_hierarchy["Boolean"] = bool_node;
        ClassNode int_node = ClassNode("Int", "Obj");
        class_hierarchy["Int"] = int_node;
        ClassNode nothing_node = ClassNode("Nothing", "Obj");
        class_hierarchy["Nothing"] = nothing_node;

        // now go through the class_hierarchy again and check for cycles and nonexistent parents
        std::cout << "SORTING " << class_hierarchy.size() << " CLASSES\n" << std::flush;
        for (std::pair<std::string, ClassNode> element : class_hierarchy) {
            ClassNode *cn = &class_hierarchy[element.first];
            //std::cout << "***** PRINTING OUT THE MAP: " << element.first << " node name " << element.second.name_
            //          << " with parent " << element.second.parent_ << endl;
            topologicalSortRec(cn);
        }
        int size = sorted_classes.size();
        std::cout << size << " TOTAL SORTED CLASSES" <<endl;
        for (int i = 0; i < sorted_classes.size(); i++) {
            std::cout << (sorted_classes.at(i)).name_ << "\n ";
        } //somehow there are still duplicates - can maybe just remove duplicates or ignore later during type inference/codegen
    }

    void check_init(AST::ASTNode *root) {
        AST::Program *root_node = (AST::Program*) root;
        set<string>  empty_vars;
        int success = root_node->init_check(this, empty_vars);
        if (success) {
            std::cout << "Success with initialization check!";
        } else {
            error = true; //something went wrong
        }
    }

    //maybe move this inside class or inside method? a method can infer itself??
    // do type inference per method!!! can reuse this method with statements after class definitions
    void type_inference(AST::ASTNode *root) {
        // do this to the methods within a class, topologically because of instance variables
        // make sure not doing anything that doesn't fit that type of instance variables - also trying to do something wrong just within the method
        //type inference for a method gets a block of statements, with list of local variables
        // everything starts at bottom - did anything change? pseudo code
        // pass a context that holds variables, and a flag to say if changed or not
        // add to the context as things change (recursive walk of the tree)
        // create a pseudo class (maybe) type inference can be done on a fake class or not, method inference with no context, no instance variables

    }

    // return the least common ancestor of the two classes
    string lca(string type1, string type2) {
        // find these two classes on the class hierarchy table and return the lca
        // if error, return Top
        return "Hello";
    }
};