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
#include "ASTBuiltIns.h"

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
        name = method->name_.text_;

        return_type = method->returns_.get_text();
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
        string name_;
        // keep track of parents for the class hierarchy
        string parent_;
        // attributes of the class (vars, methods)
        map<string, string> instance_vars; // names of the variables,
        map<string, MethodNode> methods;
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
            methods = map<string, MethodNode>();
        }

        // TODO do I need all these or should I just access the public fields and push later?
        void add_parent(string parent) {
            parent = parent;
        }

        void add_method(MethodNode method) {
            methods[method.name] = method;
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
    bool changed = true;
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
            //std::cout << " Found Resolved Class " <<cn->name_ << endl;
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
            //AST::ASTNode *constr = &(clazz->constructor_);
            AST::Method *constr = &(clazz->constructor_);
            //AST::Method *construct = (AST::Method *) constr;
            MethodNode constructor = MethodNode(constr);
            new_class.constructor_ = constructor;
            new_class.instance_vars = new_class.constructor_.local_vars; //constructors variables are the class level - need this?
            new_class.add_method(constructor);

            vector < AST::Method * > method_list = clazz->methods_.elements_;
            for (AST::Method *method: method_list) {
                MethodNode new_method = MethodNode(method);
                new_class.add_method(new_method);
                std::cout << "Created method node with name: " << new_method.name << endl;
            }

            std::cout << "Class " << cls_name << " created class node with name: " << new_class.name_ << " and parent " << new_class.parent_ <<"\n" << std::flush;
            class_hierarchy[cls_name] = new_class;
        } // end for class in classes

        // Create Built-In Classes
        ClassNode obj_node = ClassNode("Obj", "None");
        obj_node.visited = true;
        obj_node.resolved = true;
        obj_node.constructor_.return_type = "Obj";
        class_hierarchy["Obj"] = obj_node;
        sorted_classes.push_back(obj_node);
        ClassNode string_node = ClassNode("String", "Obj");
        string_node.constructor_.return_type = "String";
        class_hierarchy["String"] = string_node;
        ClassNode bool_node = ClassNode("Boolean", "Obj");
        bool_node.constructor_.return_type = "Boolean";
        class_hierarchy["Boolean"] = bool_node;
        ClassNode int_node = ClassNode("Int", "Obj");
        MethodNode printmn = MethodNode();
        printmn.name = "PRINT";
        (int_node.methods)["PRINT"] = printmn;
        MethodNode plusmn = MethodNode();
        plusmn.name = "PLUS";
        (int_node.methods)["PLUS"] = plusmn;
        int_node.constructor_.return_type = "Int";
        class_hierarchy["Int"] = int_node;
        ClassNode nothing_node = ClassNode("Nothing", "Obj");
        nothing_node.constructor_.return_type = "Nothing";
        class_hierarchy["Nothing"] = nothing_node;
//        ClassNode obj_node = ClassNode(AST_builtins::Obj.name_.text_, AST_builtins::Obj.super_.text_);
//        AST::Method construct = (AST::Method) AST_builtins::Obj.constructor_;
//        MethodNode constructor = MethodNode(&AST_builtins::Obj.constructor_);
//        obj_node.constructor_ = constructor;
//        obj_node.visited = true;
//        obj_node.resolved = true;
//        class_hierarchy["Obj"] = obj_node;
//        sorted_classes.push_back(obj_node);
//
//        ClassNode int_node = ClassNode(AST_builtins::Int.name_.text_, AST_builtins::Int.super_.text_);
//        //AST::Method construct_int = (AST::Method) AST_builtins::Int.constructor_;
//        MethodNode constructor_int = MethodNode(&(AST_builtins::Int.constructor_));
//        int_node.constructor_ = constructor_int;
//        class_hierarchy["Int"] = int_node;

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
        set<string>  initial_vars;
        initial_vars.insert("Nothing");
        initial_vars.insert("none");
        initial_vars.insert("true");
        initial_vars.insert("false");
        initial_vars.insert("Int");
        initial_vars.insert("Boolean");
        initial_vars.insert("String");
        initial_vars.insert("Obj");
        initial_vars.insert("this");
        initial_vars.insert("PLUS");
        initial_vars.insert("MINUS");
        initial_vars.insert("TIMES");
        initial_vars.insert("DIVIDE");
        initial_vars.insert("PRINT");
        initial_vars.insert("EQUALS");
        initial_vars.insert("ATMOST");
        initial_vars.insert("<");
        initial_vars.insert("ATLEAST");
        initial_vars.insert(">");

        int success = root_node->init_check(this, &initial_vars);
        if (success) {
            std::cout << "Success with initialization check!"<<endl;
        } else {
            std::cout << "Error with initialization check!" <<endl;
            error = true; //something went wrong
        }
    }

    // do type inference per method!!! can reuse this method with statements after class definitions
    void type_inference(AST::ASTNode *root) {

        var_types["Obj"] = "Obj";
        var_types["Int"] = "Int";
        var_types["Boolean"] = "Boolean";
        var_types["String"] = "String";
        var_types["Nothing"] = "Nothing";
        var_types["PRINT"] = "Nothing";
        var_types["PLUS"] = "Int";

        AST::Program *root_node = (AST::Program*) root;
        //TODO Insert built in stuff into var types

        while (changed) { // initialized as true
            changed = false;
            std::string result = root_node->type_infer(this, &var_types, "", "");
            if (result == "Ok") {
                std::cout << "Success with type inference!";
            } else {
                error = true; //something went wrong
            }
        }

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
        if ((type1 == "Top") or (type2 == "Top")) {
            return "Top";
        }
        // find these two classes on the class hierarchy table and return the lca
        // if error, return Top
        if (type1 == type2) {
            return type1;
        } else {
            ClassNode cn1 = class_hierarchy[type1];
            ClassNode cn2 = class_hierarchy[type2];
            string t1 = cn1.parent_;
            string t2 = cn2.parent_;
            vector<string> parents1 = vector<string>();
            vector<string> parents2 = vector<string>();
            // get the parents in order and populate the lists
            for (ClassNode cls : sorted_classes) {
                string cls_name = cls.name_;
                if (cls_name == t1) {
                    parents1.push_back(cls_name);
                    t1 = cls_name;
                }
                if (cls_name == t2) {
                    parents2.push_back(cls_name);
                    t2 = cls_name;
                }
            }
            // now just return the first match between the two lists, if no match...?
            for (string p : parents1) {
                for (string pp : parents2) {
                    if (p == pp) {
                        return p;
                    }
                }
            }
        }
        return "Top"; // nothing matched some sort of error
    }
};