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

    int Ident::init_check(StaticSemantics *ss, std::set<std::string> *vars) {
        std::cout << "Current Vars: " << endl;
        for (std::string var : *vars) {
            std::cout << var << endl;
        }
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
        int const_result = this->constructor_.init_check(ss, vars);
        if (!const_result) {
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
            std::set<std::string>* method_args = new std::set<std::string>(*vars);
            if (!method->init_check(ss, method_args)){
                return 0;
            }
            vars->insert(method->name_.text_);
        }
        // if everything goes well add the class name to the var table
        vars->insert(this->name_.text_);
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
        int r_result = this->right_.init_check(ss, temp_args);
        if (!r_result) {
            std::cout<< "Error in Dot Right Part" <<endl;
            return 0;
        }
        int l_result = this->left_.init_check(ss, temp_args);
        if (!l_result) {
            std::cout<< "Error in Dot Left Part" <<endl;
            return 0;
        }
        vars->insert(temp_args->begin(), temp_args->end());
        return 1;
    }

    std::string Dot::get_text() {
        return this->right_.get_text();
    }

}