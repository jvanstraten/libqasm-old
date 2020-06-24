#include <unordered_set>
#include "cqasm-analyzer.hpp"
#include "cqasm-parse-helper.hpp"
#include "cqasm-utils.hpp"
#include "cqasm-functions-gen.hpp"

namespace cqasm {
namespace analyzer {

/**
 * Creates a new semantic analyzer.
 */
Analyzer::Analyzer() : resolve_instructions(false), resolve_error_model(false) {
}

/**
 * Registers an initial mapping from the given name to the given value.
 */
void Analyzer::register_mapping(const std::string &name, const values::Value &value) {
    mappings.add(name, value);
}

/**
 * Registers a function, usable within expressions.
 */
void Analyzer::register_function(
    const std::string &name,
    const types::Types &param_types,
    const resolver::FunctionImpl &impl
) {
    functions.add(name, param_types, impl);
}

/**
 * Convenience method for registering a function. The param_types are
 * specified as a string, converted to types::Types for the other overload
 * using types::from_spec.
 */
void Analyzer::register_function(
    const std::string &name,
    const std::string &param_types,
    const resolver::FunctionImpl &impl
) {
    functions.add(name, types::from_spec(param_types), impl);
}

/**
 * Registers a number of default functions and mappings, such as the
 * operator functions, the usual trigonometric functions, mappings for pi,
 * eu (aka e, 2.718...), im (imaginary unit) and so on.
 */
void Analyzer::register_default_functions_and_mappings() {
    register_mapping("x", tree::make<values::ConstAxis>(primitives::Axis::X));
    register_mapping("y", tree::make<values::ConstAxis>(primitives::Axis::Y));
    register_mapping("z", tree::make<values::ConstAxis>(primitives::Axis::Z));
    register_mapping("true", tree::make<values::ConstBool>(true));
    register_mapping("false", tree::make<values::ConstBool>(false));
    register_mapping("pi", tree::make<values::ConstReal>(M_PI));
    register_mapping("eu", tree::make<values::ConstReal>(M_E));
    register_mapping("im", tree::make<values::ConstComplex>(primitives::Complex(0.0, 1.0)));
    functions::register_into(functions);
}

/**
 * Registers an instruction type. If you never call this, instructions are
 * not resolved (i.e. anything goes name- and operand type-wise). Once you
 * do, only instructions with signatures as added are legal, so anything
 * that doesn't match returns an error.
 */
void Analyzer::register_instruction(const instruction::Instruction &instruction) {
    resolve_instructions = true;
    instruction_set.add(instruction);
}

/**
 * Convenience method for registering an instruction type. The arguments
 * are passed straight to instruction::Instruction's constructor.
 */
void Analyzer::register_instruction(
    const std::string &name,
    const std::string &param_types,
    bool allow_conditional,
    bool allow_parallel,
    bool allow_reused_qubits
) {
    register_instruction(instruction::Instruction(
        name, param_types, allow_conditional, allow_parallel, allow_reused_qubits));
}

/**
 * Registers an error model. If you never call this, error models are not
 * resolved (i.e. anything goes name- and operand type-wise). Once you
 * do, only error models with signatures as added are legal, so anything
 * that doesn't match returns an error.
 */
void Analyzer::register_error_model(const error_model::ErrorModel &error_model) {
    resolve_error_model = true;
    error_models.add(error_model);
}

/**
 * Convenience method for registering an error model. The arguments
 * are passed straight to error_model::ErrorModel's constructor.
 */
void Analyzer::register_error_model(
    const std::string &name,
    const std::string &param_types
) {
    register_error_model(error_model::ErrorModel(name, param_types));
}

/**
 * Scope information.
 */
class Scope {
public:
    resolver::MappingTable mappings;
    resolver::FunctionTable functions;
    resolver::InstructionTable instruction_set;

    Scope(
        const resolver::MappingTable &mappings,
        const resolver::FunctionTable &functions,
        const resolver::InstructionTable &instruction_set
    ) :
        mappings(mappings),
        functions(functions),
        instruction_set(instruction_set)
    {}

};

/**
 * Helper class for analyzing a single AST. This contains the stateful
 * information that Analyzer can't have (to allow Analyzer to be reused).
 */
class AnalyzerHelper {
public:
    const Analyzer &analyzer;
    AnalysisResult result;
    Scope scope;

    /**
     * Analyzes the given AST using the given analyzer.
     */
    AnalyzerHelper(const Analyzer &analyzer, const ast::Program &ast);

    /**
     * Parses the version tag. Any semantic errors encountered are pushed into
     * the result error vector.
     */
    void analyze_version(const ast::Version &ast);

    /**
     * Checks the qubits statement and updates the scope accordingly. Any
     * semantic errors encountered are pushed into the result error vector.
     */
    void analyze_qubits(const ast::Expression &count);

    /**
     * Analyzes the given bundle and, if valid, adds it to the current
     * subcircuit. If an error occurs, the message is added to the result
     * error vector, and nothing is added to the subcircuit.
     */
    void analyze_bundle(const ast::Bundle &bundle);

    /**
     * Analyzes the given instruction. If an error occurs, the message is added to
     * the result error vector, and an empty Maybe is returned.
     */
    tree::Maybe<semantic::Instruction> analyze_instruction(const ast::Instruction &insn);

    /**
     * Analyzes the error model meta-instruction and, if valid, adds it to the
     * analysis result. If an error occurs, the message is added to the result
     * error vector, and nothing is added.
     */
    void analyze_error_model(const ast::Instruction &insn);

    /**
     * Analyzes the given mapping and, if valid, adds it to the current
     * scope. If an error occurs, the message is added to the result
     * error vector, and nothing is added to the scope.
     */
    void analyze_mapping(const ast::Mapping &mapping);

    /**
     * Analyzes the given subcircuit header and, if valid, adds it to the
     * subcircuit list. If an error occurs, the message is added to the result
     * error vector, and nothing is added to the result.
     */
    void analyze_subcircuit(const ast::Subcircuit &subcircuit);

    /**
     * Analyzes the given list of annotations. Any errors found result in the
     * annotation being skipped and an error being appended to the result error
     * vector.
     */
    tree::Any<semantic::AnnotationData> analyze_annotations(
        const tree::Any<ast::AnnotationData> &annotations
    );

    /**
     * Parses any kind of expression. Always returns a filled value or throws
     * an exception.
     */
    values::Value analyze_expression(const ast::Expression &expression);

    /**
     * Shorthand for parsing an expression and promoting it to the given type,
     * constructed in-place with the type_args parameter pack. Returns empty
     * when the cast fails.
     */
    template<class Type, class... TypeArgs>
    values::Value analyze_as(
        const ast::Expression &expression,
        TypeArgs... type_args
    );

    /**
     * Shorthand for parsing an expression to a constant integer.
     */
    primitives::Int analyze_as_const_int(const ast::Expression &expression);

    /**
     * Parses a matrix. Always returns a filled value or throws an exception.
     */
    values::Value analyze_matrix(const ast::MatrixLiteral &matrix_lit);

    /**
     * Helper for parsing a matrix. Highly templated to avoid repeating the
     * same code for different kinds of matrices, but bear in mind that the
     * template parameters are codependent. Returns empty on failure.
     */
    template<class ElType, class ElVal, class MatLit, class MatVal>
    values::Value analyze_matrix_helper(
        size_t nrows, size_t ncols,
        const std::vector<values::Value> &vals
    );

    /**
     * Parses an index operator. Always returns a filled value or throws an
     * error.
     */
    values::Value analyze_index(const ast::Index &index);

    /**
     * Parses an index list.
     */
    tree::Many<values::ConstInt> analyze_index_list(
        const ast::IndexList &index_list, size_t size
    );

    /**
     * Parses a function. Always returns a filled value or throws an exception.
     */
    values::Value analyze_function(
        const ast::Identifier &name,
        const ast::ExpressionList &args
    );

    /**
     * Parses an operator. Always returns a filled value or throws an exception.
     */
    values::Value analyze_operator(
        const std::string &name,
        const tree::One<ast::Expression> &a,
        const tree::One<ast::Expression> &b = tree::One<ast::Expression>()
    );

};

/**
 * Analyzes the given AST.
 */
AnalysisResult Analyzer::analyze(const ast::Program &ast) const {
    auto result = AnalyzerHelper(*this, ast).result;
    if (result.errors.empty() && !result.root.is_complete()) {
        std::cerr << *result.root;
        throw std::runtime_error("internal error: no semantic errors returned, but semantic tree is incomplete. Tree was dumped.");
    }
    return result;
}

/**
 * Analyzes the given AST using the given analyzer.
 */
AnalyzerHelper::AnalyzerHelper(
    const Analyzer &analyzer,
    const ast::Program &ast
) :
    analyzer(analyzer),
    result(),
    scope(analyzer.mappings, analyzer.functions, analyzer.instruction_set)
{
    try {

        // Construct the program node.
        result.root.set(tree::make<semantic::Program>());
        result.root->copy_annotation<parser::SourceLocation>(ast);

        // Check and set the version.
        analyze_version(*ast.version);

        // Handle the qubits statement.
        analyze_qubits(*ast.num_qubits);

        // Read the statements.
        for (auto stmt : ast.statements->items) {
            try {
                if (auto bundle = stmt->as_bundle()) {
                    analyze_bundle(*bundle);
                } else if (auto mapping = stmt->as_mapping()) {
                    analyze_mapping(*mapping);
                } else if (auto subcircuit = stmt->as_subcircuit()) {
                    analyze_subcircuit(*subcircuit);
                } else {
                    throw std::runtime_error("unexpected expression node");
                }
            } catch (error::AnalysisError &e) {
                e.context(*stmt);
                result.errors.push_back(e.get_message());
            }
        }

        // Save the list of final mappings.
        for (auto it : scope.mappings.get_table()) {
            const auto &name = it.first;
            const auto &value = it.second.first;
            const auto &ast_node = it.second.second;

            // Ignore predefined and implicit mappings.
            if (ast_node.empty()) {
                continue;
            }

            // Analyze any annotations attached to the mapping.
            auto annotations = analyze_annotations(it.second.second->annotations);

            // Construct the mapping object and copy the source location.
            auto mapping = tree::make<semantic::Mapping>(
                name, value,
                analyze_annotations(it.second.second->annotations)
            );
            result.root->copy_annotation<parser::SourceLocation>(*ast_node);
            result.root->mappings.add(mapping);

        }

    } catch (error::AnalysisError &e) {
        result.errors.push_back(e.get_message());
    }
}

/**
 * Checks the AST version node and puts it into the semantic tree.
 */
void AnalyzerHelper::analyze_version(const ast::Version &ast) {
    try {
        result.root->version = tree::make<semantic::Version>();
        for (auto item : ast.items) {
            if (item < 0) {
                throw error::AnalysisError("invalid version component");
            }
        }
        result.root->version->items = ast.items;
    } catch (error::AnalysisError &e) {
        e.context(ast);
        result.errors.push_back(e.get_message());
    }
    result.root->version->copy_annotation<parser::SourceLocation>(ast);
}

/**
 * Checks the qubits statement and updates the scope accordingly. Any
 * semantic errors encountered are pushed into the result error vector.
 */
void AnalyzerHelper::analyze_qubits(const ast::Expression &count) {
    try {
        // Default to 0 qubits in case we get an exception.
        result.root->num_qubits = 0;

        // Try to load the number of qubits from the expression.
        result.root->num_qubits = analyze_as_const_int(count);
        if (result.root->num_qubits < 1) {
            // Number of qubits must be positive.
            throw error::AnalysisError("invalid number of qubits");
        }

        // Construct the special q and b mappings, that map to the whole qubit
        // and measurement register respectively.
        tree::Many<values::ConstInt> all_qubits;
        for (primitives::Int i = 0; i < result.root->num_qubits; i++) {
            auto vi = tree::make<values::ConstInt>(i);
            vi->copy_annotation<parser::SourceLocation>(count);
            all_qubits.add(vi);
        }
        scope.mappings.add("q", tree::make<values::QubitRefs>(all_qubits));
        scope.mappings.add("b", tree::make<values::BitRefs>(all_qubits));

    } catch (error::AnalysisError &e) {
        e.context(count);
        result.errors.push_back(e.get_message());
    }
}

/**
 * Analyzes the given bundle and, if valid, adds it to the current
 * subcircuit. If an error occurs, the message is added to the result
 * error vector, and nothing is added to the subcircuit.
 */
void AnalyzerHelper::analyze_bundle(const ast::Bundle &bundle) {
    try {

        // The error model statement from the original cQASM grammar is a bit
        // of a pain, because it conflicts with gates/instructions, so we have
        // to special-case it here. Technically we could also have made it a
        // keyword, but the less random keywords there are, the better.
        if (bundle.items.size() == 1) {
            if (utils::case_insensitive_equals(bundle.items[0]->name->name, "error_model")) {
                analyze_error_model(*bundle.items[0]);
                return;
            }
        }

        // Analyze and add the instructions.
        auto node = tree::make<semantic::Bundle>();
        for (const auto &insn : bundle.items) {
            node->items.add(analyze_instruction(*insn));
        }

        // If we have more than two instructions, ensure that all instructions
        // are parallelizable.
        if (node->items.size() > 1) {
            for (const auto &insn : node->items) {
                try {
                    if (!insn->instruction.empty()) {
                        if (!insn->instruction->allow_parallel) {
                            std::ostringstream ss;
                            ss << "instruction ";
                            ss << insn->instruction->name;
                            ss << " with parameter pack ";
                            ss << insn->instruction->param_types;
                            ss << " is not parallelizable, but is bundled with ";
                            ss << (node->items.size() - 1);
                            ss << " other instructions";
                            throw error::AnalysisError(ss.str());
                        }
                    }
                } catch (error::AnalysisError &e) {
                    e.context(*insn);
                    result.errors.push_back(e.get_message());
                }
            }
        }

        // It's possible that no instructions end up being added, due to all
        // condition codes resolving to constant false. In that case the entire
        // bundle is removed.
        if (node->items.empty()) {
            return;
        }

        // Copy annotation data.
        node->annotations = analyze_annotations(bundle.annotations);
        node->copy_annotation<parser::SourceLocation>(bundle);

        // If we don't have a subcircuit yet, add a default one. Note that the
        // original libqasm always had this default subcircuit (even if it was
        // empty) and used the name "default" vs. the otherwise invalid empty
        // string.
        if (result.root->subcircuits.empty()) {
            auto subcircuit_node = tree::make<semantic::Subcircuit>("", 1);
            subcircuit_node->copy_annotation<parser::SourceLocation>(bundle);
            result.root->subcircuits.add(subcircuit_node);
        }

        // Add the node to the last subcircuit.
        result.root->subcircuits.back()->bundles.add(node);

    } catch (error::AnalysisError &e) {
        e.context(bundle);
        result.errors.push_back(e.get_message());
    }
}

/**
 * Analyzes the given instruction. If an error occurs, the message is added to
 * the result error vector, and an empty Maybe is returned. It's also possible
 * that an empty Maybe is returned without an error, when the condition code
 * statically resolves to false.
 */
tree::Maybe<semantic::Instruction> AnalyzerHelper::analyze_instruction(const ast::Instruction &insn) {
    try {

        // Figure out the operand list.
        auto operands = values::Values();
        for (auto operand_expr : insn.operands->items) {
            operands.add(analyze_expression(*operand_expr));
        }

        // Resolve the instruction and/or make the instruction node.
        tree::Maybe<semantic::Instruction> node;
        if (analyzer.resolve_instructions) {
            node.set(scope.instruction_set.resolve(insn.name->name, operands));
        } else {
            node.set(tree::make<semantic::Instruction>(
                tree::Maybe<instruction::Instruction>(),
                insn.name->name, values::Value(), operands,
                tree::Any<semantic::AnnotationData>()));
        }

        // Resolve the condition code.
        if (!insn.condition.empty()) {
            if (!node->instruction.empty() && !node->instruction->allow_conditional) {
                throw error::AnalysisError(
                    "conditional execution is not supported for this instruction");
            }
            auto condition_val = analyze_expression(*insn.condition);
            node->condition = values::promote(condition_val, tree::make<types::Bool>());

            // If the condition is constant false, optimize the instruction
            // away.
            if (auto x = node->condition->as_const_bool()) {
                if (!x->value) {
                    return tree::Maybe<semantic::Instruction>();
                }
            }

        } else {
            node->condition.set(tree::make<values::ConstBool>(true));
        }

        // Enforce qubit uniqueness if the instruction requires us to.
        if (!node->instruction.empty() && !node->instruction->allow_reused_qubits) {
            std::unordered_set<primitives::Int> qubits_used;
            for (const auto &operand : operands) {
                if (auto x = operand->as_qubit_refs()) {
                    for (auto index : x->index) {
                        if (!qubits_used.insert(index->value).second) {
                            throw error::AnalysisError(
                                "qubit with index " + std::to_string(index->value)
                                + " is used more than once");
                        }
                    }
                }
            }
        }

        // Copy annotation data.
        node->annotations = analyze_annotations(insn.annotations);
        node->copy_annotation<parser::SourceLocation>(insn);

        return node;
    } catch (error::AnalysisError &e) {
        e.context(insn);
        result.errors.push_back(e.get_message());
    }
    return tree::Maybe<semantic::Instruction>();
}

/**
 * Analyzes the error model meta-instruction and, if valid, adds it to the
 * analysis result. If an error occurs, the message is added to the result
 * error vector, and nothing is added.
 */
void AnalyzerHelper::analyze_error_model(const ast::Instruction &insn) {
    try {

        // Only one error model should be specified, so throw an error
        // if we already have one.
        if (!result.root->error_model.empty()) {
            auto ss = std::ostringstream();
            ss << "error model can only be specified once";
            if (auto loc = result.root->error_model->get_annotation_ptr<parser::SourceLocation>()) {
                ss << ", previous specification was at " << *loc;
            }
            throw error::AnalysisError(ss.str());
        }

        // Figure out the name of the error model.
        const auto &arg_exprs = insn.operands->items;
        if (arg_exprs.empty()) {
            throw error::AnalysisError("missing error model name");
        }
        std::string name;
        if (auto name_ident = arg_exprs[0]->as_identifier()) {
            name = name_ident->name;
        } else {
            throw error::AnalysisError(
                "first argument of an error model must be its name as an identifier");
        }

        // Figure out the argument list.
        auto arg_values = values::Values();
        for (auto arg_expr_it = arg_exprs.begin() + 1; arg_expr_it < arg_exprs.end(); arg_expr_it++) {
            arg_values.add(analyze_expression(**arg_expr_it));
        }

        // Resolve the error model to one of the known models if
        // requested. If resolving is disabled, just make a node with
        // the name and values directly (without promotion/implicit
        // casts).
        if (analyzer.resolve_error_model) {
            result.root->error_model.set(
                analyzer.error_models.resolve(
                    name, arg_values));
        } else {
            result.root->error_model.set(
                tree::make<semantic::ErrorModel>(
                    tree::Maybe<error_model::ErrorModel>(),
                    name, arg_values,
                    tree::Any<semantic::AnnotationData>()));
        }

        // Copy annotation data.
        result.root->error_model->annotations = analyze_annotations(insn.annotations);
        result.root->error_model->copy_annotation<parser::SourceLocation>(insn);

    } catch (error::AnalysisError &e) {
        e.context(insn);
        result.errors.push_back(e.get_message());
    }
}

/**
 * Analyzes the given mapping and, if valid, adds it to the current
 * scope. If an error occurs, the message is added to the result
 * error vector, and nothing is added to the scope.
 */
void AnalyzerHelper::analyze_mapping(const ast::Mapping &mapping) {
    try {
        scope.mappings.add(
            mapping.alias->name,
            analyze_expression(*mapping.expr),
            tree::make<ast::Mapping>(mapping)
        );
    } catch (error::AnalysisError &e) {
        e.context(mapping);
        result.errors.push_back(e.get_message());
    }
}

/**
 * Analyzes the given subcircuit header and, if valid, adds it to the
 * subcircuit list. If an error occurs, the message is added to the result
 * error vector, and nothing is added to the result.
 */
void AnalyzerHelper::analyze_subcircuit(const ast::Subcircuit &subcircuit) {
    try {
        primitives::Int iterations = 1;
        if (!subcircuit.iterations.empty()) {
            iterations = analyze_as_const_int(*subcircuit.iterations);
            if (iterations < 1) {
                throw error::AnalysisError(
                    "subcircuit iteration count must be positive, but is "
                    + std::to_string(iterations), &*subcircuit.iterations);
            }
        }
        auto node = tree::make<semantic::Subcircuit>(
            subcircuit.name->name,
            iterations,
            tree::Any<semantic::Bundle>(),
            analyze_annotations(subcircuit.annotations));
        node->copy_annotation<parser::SourceLocation>(subcircuit);
        result.root->subcircuits.add(node);
    } catch (error::AnalysisError &e) {
        e.context(subcircuit);
        result.errors.push_back(e.get_message());
    }
}

/**
 * Analyzes the given list of annotations. Any errors found result in the
 * annotation being skipped and an error being appended to the result error
 * vector.
 */
tree::Any<semantic::AnnotationData> AnalyzerHelper::analyze_annotations(
    const tree::Any<ast::AnnotationData> &annotations
) {
    auto retval = tree::Any<semantic::AnnotationData>();
    for (auto annotation_ast : annotations) {
        try {
            auto annotation = tree::make<semantic::AnnotationData>();
            annotation->interface = annotation_ast->interface->name;
            annotation->operation = annotation_ast->operation->name;
            for (auto expression_ast : annotation_ast->operands->items) {
                try {
                    annotation->operands.add(analyze_expression(*expression_ast));
                } catch (error::AnalysisError &e) {
                    e.context(*annotation_ast);
                    result.errors.push_back(e.get_message());
                }
            }
            annotation->copy_annotation<parser::SourceLocation>(*annotation_ast);
            retval.add(annotation);
        } catch (error::AnalysisError &e) {
            e.context(*annotation_ast);
            result.errors.push_back(e.get_message());
        }
    }
    return retval;
}

/**
 * Parses any kind of expression. Always returns a filled value or throws
 * an exception.
 */
values::Value AnalyzerHelper::analyze_expression(const ast::Expression &expression) {
    values::Value retval;
    try {
        if (auto int_lit = expression.as_integer_literal()) {
            retval.set(tree::make<values::ConstInt>(int_lit->value));
        } else if (auto float_lit = expression.as_float_literal()) {
            retval.set(tree::make<values::ConstReal>(float_lit->value));
        } else if (auto string_lit = expression.as_string_literal()) {
            retval.set(tree::make<values::ConstString>(string_lit->value));
        } else if (auto json_lit = expression.as_json_literal()) {
            retval.set(tree::make<values::ConstJson>(json_lit->value));
        } else if (auto matrix_lit = expression.as_matrix_literal()) {
            retval.set(analyze_matrix(*matrix_lit));
        } else if (auto ident = expression.as_identifier()) {
            retval.set(scope.mappings.resolve(ident->name));
        } else if (auto index = expression.as_index()) {
            retval.set(analyze_index(*index));
        } else if (auto func = expression.as_function_call()) {
            retval.set(analyze_function(func->name->name, *func->arguments));
        } else if (auto negate = expression.as_negate()) {
            retval.set(analyze_operator("-", negate->expr));
        } else if (auto power = expression.as_power()) {
            retval.set(analyze_operator("**", power->lhs, power->rhs));
        } else if (auto mult = expression.as_multiply()) {
            retval.set(analyze_operator("*", mult->lhs, mult->rhs));
        } else if (auto div = expression.as_divide()) {
            retval.set(analyze_operator("/", div->lhs, div->rhs));
        } else if (auto add = expression.as_add()) {
            retval.set(analyze_operator("+", add->lhs, add->rhs));
        } else if (auto sub = expression.as_subtract()) {
            retval.set(analyze_operator("-", sub->lhs, sub->rhs));
        } else {
            throw std::runtime_error("unexpected expression node");
        }
    } catch (error::AnalysisError &e) {
        e.context(expression);
        throw;
    }
    if (retval.empty()) {
        throw std::runtime_error(
            "analyze_expression returned nonsense, this should never happen");
    }
    retval->copy_annotation<parser::SourceLocation>(expression);
    return retval;
}

/**
 * Shorthand for parsing an expression and promoting it to the given type,
 * constructed in-place with the type_args parameter pack. Returns empty
 * when the cast fails.
 */
template <class Type, class... TypeArgs>
values::Value AnalyzerHelper::analyze_as(const ast::Expression &expression, TypeArgs... type_args) {
    return values::promote(analyze_expression(expression), tree::make<Type>(type_args...));
}

/**
 * Shorthand for parsing an expression to a constant integer.
 */
primitives::Int AnalyzerHelper::analyze_as_const_int(const ast::Expression &expression) {
    auto value = analyze_as<types::Int>(expression);
    if (auto int_value = value->as_const_int()) {
        return int_value->value;
    } else {
        throw error::AnalysisError("constant integer expected");
    }
}

/**
 * Parses a matrix. Always returns a filled value or throws an exception.
 */
values::Value AnalyzerHelper::analyze_matrix(const ast::MatrixLiteral &matrix_lit) {

    // Figure out the size of the matrix and parse the subexpressions.
    // Note that the number of rows is always at least 1 (Many vs Any) so
    // the ncols line is well-behaved.
    size_t nrows = matrix_lit.rows.size();
    size_t ncols = matrix_lit.rows[0]->items.size();
    for (auto row : matrix_lit.rows) {
        if (row->items.size() != ncols) {
            throw error::AnalysisError("matrix is not rectangular");
        }
    }
    std::vector<values::Value> vals;
    for (size_t row = 0; row < nrows; row++) {
        for (size_t col = 0; col < ncols; col++) {
            vals.push_back(analyze_expression(*matrix_lit.rows[row]->items[col]));
        }
    }

    // Try building a matrix of constant real numbers.
    auto value = analyze_matrix_helper<
        types::Real, values::ConstReal,
        primitives::RMatrix, values::ConstRealMatrix
    >(nrows, ncols, vals);
    if (!value.empty()) {
        return value;
    }

    // Try building a matrix of constant complex numbers.
    value = analyze_matrix_helper<
        types::Complex, values::ConstComplex,
        primitives::CMatrix, values::ConstComplexMatrix
    >(nrows, ncols, vals);
    if (!value.empty()) {
        return value;
    }

    // Only real and complex are supported right now. If more is to be
    // added in the future, this should probably be written a little
    // neater.
    throw error::AnalysisError("only matrices of constant real or complex numbers are currently supported");

}

/**
 * Helper for parsing a matrix. Highly templated to avoid repeating the
 * same code for different kinds of matrices, but bear in mind that the
 * template parameters are codependent. Returns empty on failure.
 */
template <class ElType, class ElVal, class MatLit, class MatVal>
values::Value AnalyzerHelper::analyze_matrix_helper(
    size_t nrows, size_t ncols,
    const std::vector<values::Value> &vals
) {
    auto matrix = MatLit(nrows, ncols);
    for (size_t row = 0; row < nrows; row++) {
        for (size_t col = 0; col < ncols; col++) {
            auto val = values::promote(vals[row * ncols + col], tree::make<ElType>());
            if (val.empty()) {
                return values::Value();
            } else {
                auto val_real = val.template as<ElVal>();
                if (val_real.empty()) {
                    return values::Value();
                } else {
                    matrix.at(row + 1, col + 1) = val_real->value;
                }
            }
        }
    }
    return tree::make<MatVal>(matrix);
}

/**
 * Parses an index operator. Always returns a filled value or throws an error.
 */
values::Value AnalyzerHelper::analyze_index(const ast::Index &index) {
    auto expr = analyze_expression(*index.expr);
    if (auto qubit_refs = expr->as_qubit_refs()) {

        // Qubit refs.
        auto indices = analyze_index_list(*index.indices,
                                          qubit_refs->index.size());
        for (auto idx : indices) {
            idx->value = qubit_refs->index[idx->value]->value;
        }
        return tree::make<values::QubitRefs>(indices);

    } else if (auto bit_refs = expr->as_bit_refs()) {

        // Measurement bit refs.
        auto indices = analyze_index_list(*index.indices,
                                          bit_refs->index.size());
        for (auto idx : indices) {
            idx->value = bit_refs->index[idx->value]->value;
        }
        return tree::make<values::BitRefs>(indices);

    } else {

        // While matrices could conceivably be indexed, this is not supported
        // right now.
        std::ostringstream ss;
        ss << "indexation is not supported for value of type " << values::type_of(expr);
        throw error::AnalysisError(ss.str());

    }
}

/**
 * Parses an index list.
 */
tree::Many<values::ConstInt> AnalyzerHelper::analyze_index_list(const ast::IndexList &index_list, size_t size) {
    tree::Many<values::ConstInt> retval;
    for (auto entry : index_list.items) {
        if (auto item = entry->as_index_item()) {

            // Single index.
            auto index = analyze_as_const_int(*item->index);
            if (index < 0 || (unsigned long)index >= size) {
                throw error::AnalysisError(
                    "index " + std::to_string(index)
                    + " out of range (size " + std::to_string(size) + ")",
                    item);
            }
            auto index_val = tree::make<values::ConstInt>(index);
            index_val->copy_annotation<parser::SourceLocation>(*item);
            retval.add(index_val);

        } else if (auto range = entry->as_index_range()) {

            // Range notation.
            auto first = analyze_as_const_int(*range->first);
            if (first < 0 || (unsigned long)first >= size) {
                throw error::AnalysisError(
                    "index " + std::to_string(first)
                    + " out of range (size " + std::to_string(size) + ")",
                    &*range->first);
            }
            auto last = analyze_as_const_int(*range->last);
            if (last < 0 || (unsigned long)last >= size) {
                throw error::AnalysisError(
                    "index " + std::to_string(last)
                    + " out of range (size " + std::to_string(size) + ")",
                    &*range->first);
            }
            if (first > last) {
                throw error::AnalysisError("last index is lower than first index", range);
            }
            for (auto index = (size_t)first; index <= (size_t)last; index++) {
                auto index_val = tree::make<values::ConstInt>(index);
                index_val->copy_annotation<parser::SourceLocation>(*range);
                retval.add(index_val);
            }

        } else {
            throw std::runtime_error("unknown IndexEntry AST node");
        }
    }
    return retval;
}

/**
 * Parses a function. Always returns a filled value or throws an exception.
 */
values::Value AnalyzerHelper::analyze_function(const ast::Identifier &name, const ast::ExpressionList &args) {
    auto arg_values = values::Values();
    for (auto arg : args.items) {
        arg_values.add(analyze_expression(*arg));
    }
    auto retval = scope.functions.call(name.name, arg_values);
    if (retval.empty()) {
        throw std::runtime_error("function implementation returned empty value");
    }
    return retval;
}

/**
 * Parses an operator. Always returns a filled value or throws an exception.
 */
values::Value AnalyzerHelper::analyze_operator(
    const std::string &name,
    const tree::One<ast::Expression> &a,
    const tree::One<ast::Expression> &b
) {
    auto identifier = ast::Identifier("operator" + name);
    auto args = ast::ExpressionList();
    args.items.add(a);
    args.items.add(b);
    return analyze_function(identifier, args);
}

} // namespace analyzer
} // namespace cqasm
