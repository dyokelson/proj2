//
// Created by Michal Young on 9/12/18.
//

#include "ASTNode.h"
#include "staticsemantics.cpp"


namespace AST {
    // Abstract syntax tree.  ASTNode is abstract base class for all other nodes.


    // JSON representation of all the concrete node types.
    // This might be particularly useful if I want to do some
    // tree manipulation in Python or another language.  We'll
    // do this by emitting into a stream.

    // --- Utility functions used by node-specific json output methods

    /* Indent to a given level */
    void ASTNode::json_indent(std::ostream& out, AST_print_context& ctx) {
        if (ctx.indent_ > 0) {
            out << std::endl;
        }
        for (int i=0; i < ctx.indent_; ++i) {
            out << "    ";
        }
    }

    /* The head element looks like { "kind" : "block", */
    void ASTNode::json_head(std::string node_kind, std::ostream& out, AST_print_context& ctx) {
        json_indent(out, ctx);
        out << "{ \"kind\" : \"" << node_kind << "\"," ;
        ctx.indent();  // one level more for children
        return;
    }

    void ASTNode::json_close(std::ostream& out, AST_print_context& ctx) {
        // json_indent(out, ctx);
        out << "}";
        ctx.dedent();
    }

    void ASTNode::json_child(std::string field, ASTNode& child, std::ostream& out, AST_print_context& ctx, char sep) {
        json_indent(out, ctx);
        out << "\"" << field << "\" : ";
        child.json(out, ctx);
        out << sep;
    }

    void Stub::json(std::ostream& out, AST_print_context& ctx) {
        json_head("Stub", out, ctx);
        json_indent(out, ctx);
        out  << "\"rule\": \"" <<  name_ << "\"";
        json_close(out, ctx);
    }


    void Program::json(std::ostream &out, AST::AST_print_context &ctx) {
        json_head("Program", out, ctx);
        json_child("classes_", classes_, out, ctx);
        json_child("statements_", statements_, out, ctx, ' ');
        json_close(out, ctx);
    }

    void Formal::json(std::ostream &out, AST::AST_print_context &ctx) {
        json_head("Formal", out, ctx);
        json_child("var_", var_, out, ctx);
        json_child("type_", type_, out, ctx, ' ');
        json_close(out, ctx);
    }

    void Method::json(std::ostream &out, AST::AST_print_context &ctx) {
        json_head("Method", out, ctx);
        json_child("name_", name_, out, ctx);
        json_child("formals_", formals_, out, ctx);
        json_child("returns_", returns_, out, ctx);
        json_child("statements_", statements_, out, ctx, ' ');
        json_close(out, ctx);
    }

    void Assign::json(std::ostream& out, AST_print_context& ctx) {
        json_head("Assign", out, ctx);
        json_child("lexpr_", lexpr_, out, ctx);
        json_child("rexpr_", rexpr_, out, ctx, ' ');
        json_close(out, ctx);
     }

    void AssignDeclare::json(std::ostream& out, AST_print_context& ctx) {
        json_head("Assign", out, ctx);
        json_child("lexpr_", lexpr_, out, ctx);
        json_child("rexpr_", rexpr_, out, ctx);
        json_child("static_type_", static_type_, out, ctx, ' ');
        json_close(out, ctx);
    }

    void Return::json(std::ostream &out, AST::AST_print_context &ctx) {
        json_head("Return", out, ctx);
        json_child("expr_", expr_, out, ctx, ' ');
        json_close(out, ctx);
    }

    void If::json(std::ostream& out, AST_print_context& ctx) {
        json_head("If", out, ctx);
        json_child("cond_", cond_, out, ctx);
        json_child("truepart_", truepart_, out, ctx);
        json_child("falsepart_", falsepart_, out, ctx, ' ');
        json_close(out, ctx);
    }

    void While::json(std::ostream& out, AST_print_context& ctx) {
        json_head("While", out, ctx);
        json_child("cond_", cond_, out, ctx);
        json_child("body_", body_, out, ctx, ' ');
        json_close(out, ctx);
    }


    void Typecase::json(std::ostream& out, AST_print_context& ctx) {
        json_head("Typecase", out, ctx);
        json_child("expr_", expr_, out, ctx);
        json_child("cases_", cases_, out, ctx, ' ');
        json_close(out, ctx);
    }

    void Type_Alternative::json(std::ostream& out, AST_print_context& ctx) {
        json_head("Type_Alternative", out, ctx);
        json_child("ident_", ident_, out, ctx);
        json_child("classname_", classname_, out, ctx);
        json_child("block_", block_, out, ctx, ' ');
        json_close(out, ctx);
    }

    void Load::json(std::ostream &out, AST::AST_print_context &ctx) {
        json_head("Load", out, ctx);
        json_child("loc_", loc_, out, ctx, ' ');
        json_close(out, ctx);
    }

    void Ident::json(std::ostream& out, AST_print_context& ctx) {
        json_head("Ident", out, ctx);
        out << "\"text_\" : \"" << text_ << "\"";
        json_close(out, ctx);
    }

    void Class::json(std::ostream &out, AST::AST_print_context &ctx) {
        json_head("Class", out, ctx);
        json_child("name_", name_, out, ctx);
        json_child("super_", super_, out, ctx);
        json_child("constructor_", constructor_, out, ctx);
        json_child("methods_", methods_, out, ctx, ' ');
        json_close(out, ctx);
    }

    void Call::json(std::ostream &out, AST::AST_print_context &ctx) {
        json_head("Call", out, ctx);
        json_child("obj_", receiver_, out, ctx);
        json_child("method_", method_, out, ctx);
        json_child("actuals_", actuals_, out, ctx, ' ');
        json_close(out, ctx);
    }

    void Construct::json(std::ostream &out, AST::AST_print_context &ctx) {
        json_head("Construct", out, ctx);
        json_child("method_", method_, out, ctx);
        json_child("actuals_", actuals_, out, ctx, ' ');
        json_close(out, ctx);
    }

    void IntConst::json(std::ostream& out, AST_print_context& ctx) {
        json_head("IntConst", out, ctx);
        out << "\"value_\" : " << value_ ;
        json_close(out, ctx);
    }

    void StrConst::json(std::ostream& out, AST_print_context& ctx) {
        json_head("StrConst", out, ctx);
        out << "\"value_\" : \"" << value_  << "\"";
        json_close(out, ctx);
    }


    void BinOp::json(std::ostream& out, AST_print_context& ctx) {
        json_head(opsym, out, ctx);
        json_child("left_", left_, out, ctx);
        json_child("right_", right_, out, ctx, ' ');
        json_close(out, ctx);
    }


    void Not::json(std::ostream& out, AST_print_context& ctx) {
        json_head("Not", out, ctx);
        json_child("left_", left_, out, ctx, ' ');
        json_close(out, ctx);
    }

    void Dot::json(std::ostream& out, AST_print_context& ctx) {
        json_head("Dot", out, ctx);
        json_child("left_", left_, out, ctx);
        json_child("right_", right_, out, ctx, ' ');
        json_close(out, ctx);
    }


    /* Convenience factory for operations like +, -, *, / */
    Call* Call::binop(std::string opname, Expr& receiver, Expr& arg) {
        Ident* method = new Ident(opname);
        Actuals* actuals = new Actuals();
        actuals->append(&arg);
        return new Call(receiver, *method, *actuals);
    }

    /* IMPLEMENT ALL THE TYPE INITIALIZATION METHODS */
    int Stub::init_check(StaticSemantics *ss, std::set<std::string> *vars) {
        return 0;
    }

    //template<>
    //int Seq<ASTNode>::init_check(StaticSemantics *ss, std::set<std::string> *vars) {}

    int Program::init_check(StaticSemantics *ss, std::set<std::string> *vars) {
        // check initialization for each class in classes
        AST::Classes classes = this->classes_;
        vector < AST::Class * > class_list = classes.elements_;
        for (AST::Class *clazz: class_list) {
            std::cout << "Init Checking Class: " << clazz->name_.str() << endl;
            int class_result = clazz->init_check(ss, vars);
            if (!class_result) {
                return 0;
            }
        }
        // bus error happening sometime above this
        //exit(1);
        // check each statement in the statement block that comes after the classes
        AST::Block statements = this->statements_;
        vector < AST::ASTNode * > statement_list =  statements.elements_;
        for (AST::ASTNode *stmt: statement_list) {
            std::cout << "Init Checking Statement: " << stmt->str() << endl;
            int stmt_result = stmt->init_check(ss, vars);
            if (!stmt_result) {
                return 0;
            }
        }
        return 1;
    }

    int Formal::init_check(StaticSemantics *ss, std::set<std::string> *vars) {
        // TODO don't add here? only add when I know it was a fully successful method?
        Ident formal_arg = this->var_;
        string arg = formal_arg.text_;
        std::cout << "Inserting formal arg: " << arg << endl;
        vars->insert(arg);
        return 1;
    }

    int Method::init_check(StaticSemantics *ss, std::set<std::string> *vars) {
        // init check formal args - add
        //std::set<std::string>* method_args = new std::set<std::string>(*vars);
        for (Formal* fml : this->formals_.elements_) {
            int fml_result = fml->init_check(ss, vars);
            if (!fml_result) {
                std::cout << "Method Checking Results: Formal Arg Result: " << fml_result <<endl;
                return 0;
            }
        }
        // init check method body
        AST::Block statements = this->statements_;
        vector < AST::ASTNode * > statement_list =  statements.elements_;
        for (AST::ASTNode *stmt: statement_list) {
            int stmt_result = stmt->init_check(ss, vars);
            if (!stmt_result) {
                std::cout << "Method Checking Results: Statement Result: " << stmt_result<<endl;
                return 0;
            }
        }
        // init check return statement
        int return_result = this->returns_.init_check(ss, vars);
        if (!return_result) {
            std::cout << "Error in Method Checking Results: Return Result: " << return_result<<endl;
            return 0;
        }
        // add method name to the table if all was successful
        vars->insert(this->name_.text_);
        return 1;

    }
    /*  statement: l_expr '=' expr ';'
    { $$ = new AST::Assign(*$1, *$3); };*/
    int Assign::init_check(StaticSemantics *ss, std::set<std::string> *vars) {
        //std::set<std::string>* temp_args = new std::set<std::string>(*vars);
        int r_result = this->rexpr_.init_check(ss, vars);
        if (!r_result) {
            std::cout<< "Error in Assign Right Part" <<endl;
            return 0;
        }
        std::string var_name = this->lexpr_.get_text();
        vars->insert(var_name);
        return 1;
    }
    /*statement: l_expr ':' ident '=' expr ';'
    {$$ = new AST::AssignDeclare(*$1, *$5, *$3);};*/
    int AssignDeclare::init_check(StaticSemantics *ss, std::set<std::string> *vars) {
        //std::set<std::string>* temp_args = new std::set<std::string>(*vars);
        int r_result = this->rexpr_.init_check(ss, vars);
        if (!r_result) {
            std::cout<< "Error in AssignDeclare Right Part" <<endl;
            return 0;
        }
        std::string var_name = this->lexpr_.get_text();
        vars->insert(var_name);
        return 1;
    }

    int Return::init_check(StaticSemantics *ss, std::set<std::string> *vars) {
        std::cout<< "Return Statement Expr: " <<expr_.str()<<endl;
        int result = this->expr_.init_check(ss, vars);
        return result;
    }

    int If::init_check(StaticSemantics *ss, std::set<std::string> *vars) {
        if (!(this->cond_.init_check(ss, vars))){
            std::cout << "Error in If Statement Conditional Part: " << cond_.str()<<endl;
            return 0;
        }
        std::set<std::string>* true_args = new std::set<std::string>(*vars);
        // true and false are seqs of ASTNodes
        //AST::Block statements = this->statements_;
        vector < AST::ASTNode * > true_list =  this->truepart_.elements_;
        for (AST::ASTNode *true_item: true_list) {
            int result = true_item->init_check(ss, true_args);
            if (!result) {
                std::cout << "Error in If Statement True Part: " << true_item->str()<<endl;
                return 0;
            }
        }
        std::set<std::string>* false_args = new std::set<std::string>(*vars);
        vector < AST::ASTNode * > false_list =  this->falsepart_.elements_;
        for (AST::ASTNode *false_item: false_list) {
            int result = false_item->init_check(ss, false_args);
            if (!result) {
                std::cout << "Error in If Statement False Part: " << false_item->str()<<endl;
                return 0;
            }
        }
        //exit(1);
        // add the intersecting variables of true and false parts if every section type init OK
        for (std::string var : *true_args) {
            if ((false_args->find(var)) != false_args->end()) {
                vars->insert(var);
            }
        }
        return 1;
    }

    int While::init_check(StaticSemantics *ss, std::set<std::string> *vars) {
        // type check the condition and the statements, don't update the vars table
        if (!(this->cond_.init_check(ss, vars))){
            return 0;
        }
        std::set<std::string>* temp_vars = new std::set<std::string>(*vars);
        vector < AST::ASTNode * > body_list =  this->body_.elements_;
        for (AST::ASTNode *body_item: body_list) {
            int result = body_item->init_check(ss, temp_vars);
            if (!result) {
                std::cout << "Error in While Statement Body: " << result<<endl;
                return 0;
            }
        }
        return 1;
    }

    int Typecase::init_check(StaticSemantics *ss, std::set<std::string> *vars) {
        //TODO something here?
        return 1;
    }

    int Type_Alternative::init_check(StaticSemantics *ss, std::set<std::string> *vars) {
        //TODO something here?
        return 1;
    }

    int Load::init_check(StaticSemantics *ss, std::set<std::string> *vars) {
        //TODO add variable to the table
        std::cout << "Load loc: " << this->loc_.str() <<endl;
        return this->loc_.init_check(ss, vars);
    }

    std::string Load::get_text() {
        return this->loc_.get_text();
    }

    int Ident::init_check(StaticSemantics *ss, std::set<std::string> *vars) {
        if ((vars->find(this->text_)) != vars->end()) {
            return 1;
        } else {
            std::cout << "ERROR Using Uninitialized Variable: " << this->text_ << endl;
            return 0;
        }
    }

    std::string Ident::get_text() {
        return this->text_;
    }

    int Class::init_check(StaticSemantics *ss, std::set<std::string> *vars) {
        // check that the constructor initializes all class level vars (like any normal method)
        //std::set<std::string>* class_args = new std::set<std::string>(*vars);
        vars->insert(this->name_.text_);
        int const_result = this->constructor_.init_check(ss, vars);
        if (!const_result) {
            std::cout << "Error Init Checking Class Constructor: " << this->name_.text_<<endl;
            return 0;
        }
        // anything inherited from the superclass needs to have been initialized
        ClassNode superclass = ss->class_hierarchy[this->super_.str()];
        std::map<string, string> super_vars = superclass.instance_vars;
        for (std::pair<std::string, string> element : super_vars) {
            if ((vars->find(element.first)) == vars->end()){
                return 0; // something wasn't initialized
            }
        }
        // now check all the methods
        vector < AST::Method * > method_list = this->methods_.elements_;
        for (AST::Method *method: method_list) {
            std::cout << "Init Checking Method: " << method->name_.text_;
            std::set<std::string>* method_args = new std::set<std::string>(*vars);
            if (!method->init_check(ss, method_args)){
                return 0;
            }
            vars->insert(method->name_.text_);
        }
        // if everything goes well add the class name to the var table
//        vars->insert(this->name_.text_);
        return 1;
    }

    int Call::init_check(StaticSemantics *ss, std::set<std::string> *vars) {
        if ((vars->find(this->method_.text_)) != vars->end()) {
            // method was initialized! now check the args
            Actuals actual_args = this->actuals_;
            vector < Expr * > arg_list = actual_args.elements_;
            for (Expr* arg : arg_list) {
                if (!arg->init_check(ss, vars)) {
                    std::cout << "Actual argument not found: " << arg->str()<<endl;
                    return 0;
                }
            }
            if (!this->receiver_.init_check(ss, vars)){
                std::cout << "Error init checking the receiver object: " << this->receiver_.str()<<endl;
                return 0;
            }
            return 1;
        } else {
            return 0;
        }
    }

    int Construct::init_check(StaticSemantics *ss, std::set<std::string> *vars) {
        // make sure that class type has been added to the scope
        if ((vars->find(this->method_.text_) != vars->end())) {
            // then make sure the passed args were initialized
            Actuals actual_args = this->actuals_;
            vector < Expr * > arg_list = actual_args.elements_;
            for (Expr* arg : arg_list) {
                if (!arg->init_check(ss, vars)) {
                    return 0;
                }
            }
            return 1;
        }
        return 0;
    }

    int IntConst::init_check(StaticSemantics *ss, std::set<std::string> *vars) {
        return 1;
    }

    int StrConst::init_check(StaticSemantics *ss, std::set<std::string> *vars) {
        return 1;
    }

    int And::init_check(StaticSemantics *ss, std::set<std::string> *vars) {
        int right_result = this->right_.init_check(ss, vars);
        int left_result = this->left_.init_check(ss, vars);

        if (right_result and left_result) {
            return 1;
        } else {
            return 0;
        }
    }

    int Or::init_check(StaticSemantics *ss, std::set<std::string> *vars) {
        int right_result = this->right_.init_check(ss, vars);
        int left_result = this->left_.init_check(ss, vars);

        if (right_result and left_result) {
            return 1;
        } else {
            return 0;
        }

    }

    int Not::init_check(StaticSemantics *ss, std::set<std::string> *vars) {
        int result = this->left_.init_check(ss, vars);
        return result;
    }

    int Dot::init_check(StaticSemantics *ss, std::set<std::string> *vars) {
        std::set<std::string>* temp_args = new std::set<std::string>(*vars);
//        int r_result = this->right_.init_check(ss, temp_args);
//        if (!r_result) {
//            std::cout<< "Error in Dot Right Part" <<endl;
//            return 0;
//        }
        int l_result = this->left_.init_check(ss, temp_args);
        if (!l_result) {
            std::cout<< "Error in Dot Left Part" <<endl;
            return 0;
        }
        vars->insert(temp_args->begin(), temp_args->end());
        return 1;
    }

    std::string Dot::get_text() {
        std::string text = (this->left_).get_text() + '.' + this->right_.get_text();
        return text;
        //return this->right_.get_text();
    }


    /* IMPLEMENT ALL THE TYPE INFERENCE METHODS */

    std::string Program::type_infer(StaticSemantics *ss, map<std::string, std::string>* context, string cur_class, string cur_method) {
        // type inference for each class in classes
        AST::Classes classes = this->classes_;
        vector < AST::Class * > class_list = classes.elements_;
        for (AST::Class *clazz: class_list) {
            std::string cls_name = clazz->name_.text_;
            std::cout << "Type Inferring Class: " << cls_name << endl;
            ClassNode* cn = &((ss->class_hierarchy)[cls_name]);
            std::map<std::string, std::string>* class_args = &(cn->instance_vars);
            std::string class_result = clazz->type_infer(ss, class_args, cls_name, cur_method);
            if (class_result=="Top") {
                return "Top";
            }
        }
        // type infer each statement in the statement block that comes after the classes
        AST::Block statements = this->statements_;
        vector < AST::ASTNode * > statement_list =  statements.elements_;
        for (AST::ASTNode *stmt: statement_list) {
            std::cout << "Type Inferring Statement: " << stmt->str() << endl;
            std::string stmt_result = stmt->type_infer(ss, context, "$main", "$meth");
            if (stmt_result=="Top") {
                return "Top";
            }
        }
        return "Ok";
    }

    std::string Formal::type_infer(StaticSemantics *ss, map<std::string, std::string>* context, string cur_class, string cur_method) {
        Ident formal_arg = this->var_;
        string arg = formal_arg.text_;
        string type = this->type_.text_;
        std::cout << "Adding formal arg: " << arg << " with type: "<< type << endl;
        (*context)[arg] = type;
        return "Ok";
    }

    std::string Method::type_infer(StaticSemantics *ss, map<std::string, std::string>* context, string cur_class, string cur_method) {
        // init check formal args - add
        std::string dis_method = this->name_.text_;
        //TODO later check if its an inherited method and make sure it gets all those types
        for (Formal* fml : this->formals_.elements_) {
            std::string fml_result = fml->type_infer(ss, context, cur_class, dis_method);
            if (fml_result=="Top") {
                std::cout << "Type Infer Method Checking Results: Formal Arg Result: " << fml_result <<endl;
                return "Top";
            }
        }
        // type infer method body
        AST::Block statements = this->statements_;
        vector < AST::ASTNode * > statement_list =  statements.elements_;
        for (AST::ASTNode *stmt: statement_list) {
            std::string stmt_result = stmt->type_infer(ss, context, cur_class, dis_method);
            std::cout<<stmt->str()<<endl;
            if (stmt_result == "Top") {
                std::cout << "Type Infer Method Checking Results in class: "<<cur_class<<" and Method: " << dis_method<<endl;
                return "Top";
            }
        }
        return "Ok";

    }
    /*  statement: l_expr '=' expr ';'
    { $$ = new AST::Assign(*$1, *$3); };*/
    std::string Assign::type_infer(StaticSemantics *ss, map<std::string, std::string>* context, string cur_class, string cur_method) {
        //std::set<std::string>* temp_args = new std::set<std::string>(*vars);
        std::string r_result = this->rexpr_.type_infer(ss, context, cur_class, cur_method);
        std::cout<< "VARIABLE TYPE "<< r_result <<endl;
        if ((r_result== "Top") or (r_result =="")) { //TODO why am I getting a blank result?
            std::cout<< "Error in Assign Right Part" <<endl;
            return "Top";
        }
        std::string var_name = this->lexpr_.get_text();
        std::cout<< "VARIABLE NAME "<< var_name <<endl;
        // if variable is already in the table we have to get the lca
        std::string new_type;
        if (context->count(var_name)) {
            //get the current type in the table
            std::string cur_type = (*context)[var_name];
            std::cout<< "CURRENT TYPE "<< cur_type <<endl;
            if (r_result == "") {
                return cur_type;
            }
            //get the lca
            new_type = ss->lca(cur_type, r_result);
            std::cout<< "NEW TYPE "<< new_type <<endl;
            // if they are not the same set the changed flag! (if they are, don't say anything changed)
            if (cur_type != new_type) {
                ss->changed = true;
                // update the current context and also the class hierarchy table
                (*context)[var_name] = new_type;
//                ClassNode cn = (ss->class_hierarchy)[cur_class];
//                MethodNode mn = (cn.methods)[cur_method];
//                (mn.local_vars)[var_name] = new_type;
            }
        } else { // if not in the table, we are adding something, set the changed flag!
            std::cout<< "R RESULT: "<<r_result<<endl;
            new_type = r_result;
            (*context)[var_name] = new_type;
            //ss->changed = true;
        }
        return new_type;
    }
    /*statement: l_expr ':' ident '=' expr ';'
    {$$ = new AST::AssignDeclare(*$1, *$5, *$3);};*/
    std::string AssignDeclare::type_infer(StaticSemantics *ss, map<std::string, std::string>* context, string cur_class, string cur_method) {
        //std::set<std::string>* temp_args = new std::set<std::string>(*vars);
        std::string r_result = this->rexpr_.type_infer(ss, context, cur_class, cur_method);
        std::string type = this->static_type_.text_;
        if (r_result == type) {
            std::string var_name = this->lexpr_.get_text();
            (*context)[var_name] = type;
            ClassNode cn = (ss->class_hierarchy)[cur_class];
            MethodNode mn = (cn.methods)[cur_method];
            (mn.local_vars)[var_name] = type;
            return r_result;
        }
        std::cout<< "Error in AssignDeclare Type Inference " <<endl;
        return "Top";
    }

    std::string Return::type_infer(StaticSemantics *ss, map<std::string, std::string>* context, string cur_class, string cur_method) {
        std::string result = this->expr_.type_infer(ss, context, cur_class, cur_method);
        ClassNode cn = ss->class_hierarchy[cur_class];
        std::cout << "Class Node: "<<cn.name_<<endl;
//        std::map<string, MethodNode> methods = cn->methods;
//        MethodNode* mn = &(methods[cur_method]);
        MethodNode mn = (cn.methods)[cur_method];
        std::cout << "Method Node: "<<mn.name<<endl;
        std::string should_return = mn.return_type;
//        if (result != should_return) {
//            std::cout << "Error in Method Checking Results Return Result: " << result << " should return "<< should_return<<endl;
//            return "Top";
//        }
        return result;
    }

    std::string If::type_infer(StaticSemantics *ss, map<std::string, std::string>* context, string cur_class, string cur_method) {
        std::string cond_result = this->cond_.type_infer(ss, context, cur_class, cur_method);
        std::cout << "Evaluating If Statement " <<endl;
        if (cond_result == "Boolean"){
            std::map<std::string, std::string>* true_args = new std::map<std::string, std::string>(*context);
            // true and false are seqs of ASTNodes
            //AST::Block statements = this->statements_;
            vector < AST::ASTNode * > true_list =  this->truepart_.elements_;
            for (AST::ASTNode *true_item: true_list) {
                std::string result = true_item->type_infer(ss, true_args, cur_class, cur_method);
                if (result=="Top") {
                    std::cout << "Error in If Statement True Part: " << true_item->str()<<endl;
                    return result;
                }
            }
            std::map<std::string, std::string>* false_args = new std::map<std::string, std::string>(*context);
            vector < AST::ASTNode * > false_list =  this->falsepart_.elements_;
            for (AST::ASTNode *false_item: false_list) {
                std::string result = false_item->type_infer(ss, true_args, cur_class, cur_method);
                if (result=="Top") {
                    std::cout << "Error in If Statement False Part: " << false_item->str()<<endl;
                    return result;
                }
            }
            // add the intersecting variables of true and false parts if every section type init OK
            for (std::pair<std::string,std::string> element : *true_args) {
                std::string var_name = element.first;
                if ((false_args->count(var_name))) {
                    (*context)[var_name] = element.second;
                }
            }
            return "Ok";
        }
        return "Top";
    }

    std::string While::type_infer(StaticSemantics *ss, map<std::string, std::string>* context, string cur_class, string cur_method) {
        // type check the condition and the statements, don't update the vars table
        std::string cond_result = this->cond_.type_infer(ss, context, cur_class, cur_method);
        if (cond_result == "Boolean"){
            std::map<std::string, std::string>* temp_vars = new std::map<std::string, std::string>(*context);
            vector < AST::ASTNode * > body_list =  this->body_.elements_;
            for (AST::ASTNode *body_item: body_list) {
                std::string result = body_item->type_infer(ss, temp_vars, cur_class, cur_method);
                if (result == "Top") {
                    std::cout << "Error in While Statement Body: " << result<<endl;
                    return "Top";
                }
            }
            return "Ok";
        }
        return "Top";
    }

    std::string Typecase::type_infer(StaticSemantics *ss, map<std::string, std::string>* context, string cur_class, string cur_method) {
        //TODO something here?
        return "Ok";
    }

    std::string Type_Alternative::type_infer(StaticSemantics *ss, map<std::string, std::string>* context, string cur_class, string cur_method) {
        //TODO something here?
        return "Ok";
    }

    std::string Load::type_infer(StaticSemantics *ss, map<std::string, std::string>* context, string cur_class, string cur_method) {
        //std::cout << "Load loc: " << this->loc_.str() <<endl;
        return this->loc_.type_infer(ss, context, cur_class, cur_method);
    }

    std::string Ident::type_infer(StaticSemantics *ss, map<std::string, std::string>* context, string cur_class, string cur_method) {
//        std::cout << "Current Vars: " << endl;
//        for (std::pair<std::string, string> element : *context) {
//            std::cout << element.first << " with type "<<element.second<< endl;
//        }
        if ((context->count(this->text_))) {
            return (*context)[this->text_];
        } else {
            std::cout << "ERROR Can't find Type of Variable: " << this->text_ << endl;
            return "Top";
        }
    }

    std::string Class::type_infer(StaticSemantics *ss, map<std::string, std::string>* context, string cur_class, string cur_method) {
        // check that the constructor initializes all class level vars (like any normal method)
        std::string dis_class = this->name_.text_;
        ClassNode cn = ss->class_hierarchy[dis_class];
        MethodNode mn = cn.constructor_;
        std::string should_return = mn.return_type;
        std::cout<< "Class should return type "<< should_return << endl;
        (*context)["this"] = should_return;

        std::string const_result = this->constructor_.type_infer(ss, context, dis_class, dis_class);
        if (const_result== "Top") {
            std::cout<< "Type Error in Constructor" << endl;
            return const_result;
        }

        // now check all the methods
        vector < AST::Method * > method_list = this->methods_.elements_;
        for (AST::Method *method: method_list) {
            //std::map<std::string, std::string>* method_args = new std::map<std::string, std::string>(*context);
            std::string dis_method = method->name_.text_;
            MethodNode mn = (cn.methods)[dis_method];
            std::map<std::string, std::string>* method_args = &(mn.local_vars);
            method_args->insert(context->begin(), context->end());
            std::cout<< "CURRENT VARS: "<<endl;
            for (std::pair<std::string, string> element : *method_args) {
                std::cout << element.first << " with type "<<element.second<< endl;
            }
            std::cout<< "About to check method: "<<dis_method<<endl;
            std::string meth_return = method->type_infer(ss, method_args, dis_class, dis_method);
            if (meth_return=="Top") {
                return "Top";
            }
        }
        // TODO anything inherited from the superclass needs to be consistent

        return "Ok";
    }

    std::string Call::type_infer(StaticSemantics *ss, map<std::string, std::string>* context, string cur_class, string cur_method){
        //type infer the receiver
        std::string receiver = this->receiver_.type_infer(ss, context, cur_class, cur_method);
        std::string method_call = this->method_.get_text();

        //look up the method name in the receiver class
        ClassNode* cn = &(ss->class_hierarchy[receiver]);
        std::cout<< "Got Class Node: "<<cn->name_<< " for receiver "<<this->receiver_.get_text()<<" and type "<<receiver<<endl;
        //TODO in case the method isn't there check count
        MethodNode mn = cn->methods[method_call];
        std::cout<< "Got Method Node: "<<mn.name<< " for method " << method_call<<endl;
        std::string should_return = mn.return_type;
        std::cout<< "Method Call: "<<method_call<<" Should Return: "<< should_return<<endl;

        // TODO can check if actual arguments are correct, skipping for now
        // returns the return type of the method
        return should_return;

    }

    std::string Construct::type_infer(StaticSemantics *ss, map<std::string, std::string>* context, string cur_class, string cur_method) {
        // we are creating a new object
        cur_class = this->method_.get_text();
        cur_method = this->method_.get_text();
        //  type infer the passed args
        Actuals actual_args = this->actuals_;
        vector < Expr * > arg_list = actual_args.elements_;
        for (Expr* arg : arg_list) {
            if (arg->type_infer(ss, context, cur_class, cur_method) == "Top") {
                std::cout << "Error checking args of constructor"<<endl;
                return "Top";
            }
        }
        // all is good, look up the return type in the class hierarchy and return it
        ClassNode cn = ss->class_hierarchy[cur_class];
        //std::cout<< "Got Class Node: "<<cn.name_<< " for construct type "<<cur_method<<endl;
        MethodNode mn = cn.constructor_;
        //std::cout<< "Got Method Node: "<<mn.name<<endl;
        std::string should_return = mn.return_type;

        return should_return;

    }

    std::string IntConst::type_infer(StaticSemantics *ss, map<std::string, std::string>* context, string cur_class, string cur_method) {
        return "Int";
    }

    std::string StrConst::type_infer(StaticSemantics *ss, map<std::string, std::string>* context, string cur_class, string cur_method) {
        return "String";
    }

    std::string And::type_infer(StaticSemantics *ss, map<std::string, std::string>* context, string cur_class, string cur_method) {
        std::string right_result = this->right_.type_infer(ss, context, cur_class, cur_method);
        std::string left_result = this->left_.type_infer(ss, context, cur_class, cur_method);

        if (right_result=="Boolean" and left_result=="Boolean") {
            return "Boolean";
        } else {
            return "Top";
        }
    }

    std::string Or::type_infer(StaticSemantics *ss, map<std::string, std::string>* context, string cur_class, string cur_method) {
        std::string right_result = this->right_.type_infer(ss, context, cur_class, cur_method);
        std::string left_result = this->left_.type_infer(ss, context,cur_class, cur_method);

        if (right_result=="Boolean" and left_result=="Boolean") {
            return "Boolean";
        } else {
            return "Top";
        }

    }

    std::string Not::type_infer(StaticSemantics *ss, map<std::string, std::string>* context, string cur_class, string cur_method) {
        std::string result = this->left_.type_infer(ss, context, cur_class, cur_method);
        if (result == "Boolean") {
            return "Boolean";
        } else {
            return "Top";
        }
    }

    std::string Dot::type_infer(StaticSemantics *ss, map<std::string, std::string>* context, string cur_class, string cur_method) {
        //std::cout<< "IS THIS A THIS "<< this->left_.get_text() <<endl;
        //get the type of the left (this or other, etc.) then dig that type out of the class hierarchy
        std::string l_result = this->left_.type_infer(ss, context, cur_class, cur_method);
        ClassNode* cn = &((ss->class_hierarchy)[l_result]);
        std::map<std::string, std::string>* cls_table = &(cn->instance_vars);
        std::cout<< "DOT CURRENT VARS: "<<endl;
        for (std::pair<std::string, string> element : *cls_table) {
            std::cout << element.first << " with type "<<element.second<< endl;
        }
        std::string r_result = this->right_.type_infer(ss, cls_table, l_result, l_result);
        if (r_result == "Top") {
            std::cout<< "Error while Type Inferring Dot" <<endl;
        }
        return r_result;
    }
    void Program::gen_rvalue(CodegenContext& ctx, std::string target_reg) {
        this->statements_.gen_rvalue(ctx, target_reg);
    }

    void Block::gen_rvalue(CodegenContext& ctx, std::string target_reg) {
        vector < AST::ASTNode * > statement_list =  this->elements_;
        for (AST::ASTNode *stmt: statement_list) {
            //std::cout << "STATEMENT" << stmt->str() <<endl;
            stmt->gen_rvalue(ctx, target_reg);
        }
    }
    void Call::gen_rvalue(CodegenContext &ctx, std::string target_reg) {
        this->receiver_.gen_rvalue(ctx, target_reg);
        ctx.emit(target_reg +" -> clazz."+ this->method_.text_ + "("+target_reg+");");
    }

    void IntConst::gen_rvalue(CodegenContext &ctx, std::string target_reg) {
        std::string value = to_string(this->value_);
        ctx.emit(target_reg + " = (Obj_obj) int_literal(" + value + "); // LOAD constant value");
    }

}