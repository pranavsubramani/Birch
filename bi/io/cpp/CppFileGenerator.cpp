/**
 * @file
 */
#include "bi/io/cpp/CppFileGenerator.hpp"

#include "bi/io/cpp/CppModelGenerator.hpp"
#include "bi/io/cpp/CppDispatcherGenerator.hpp"
#include "bi/io/cpp/CppParameterGenerator.hpp"
#include "bi/io/cpp/CppOutputGenerator.hpp"
#include "bi/io/cpp/CppReturnGenerator.hpp"
#include "bi/io/cpp/CppTemplateParameterGenerator.hpp"
#include "bi/program/all.hpp"
#include "bi/exception/all.hpp"
#include "bi/primitive/encode.hpp"
#include "bi/visitor/Gatherer.hpp"

#include "boost/filesystem.hpp"
#include "boost/algorithm/string.hpp"

#include <sstream>
#include <iomanip>
#include <typeinfo>

bi::CppFileGenerator::CppFileGenerator(std::ostream& base, const int level,
    const bool header) :
    CppBaseGenerator(base, level, header) {
  //
}

void bi::CppFileGenerator::visit(const File* o) {
  line("/**");
  line(" * @file");
  line(" *");
  line(" * Automatically generated by Birch.");
  line(" */");

  if (header) {
    /* include guard */
    line("#pragma once\n");

    /* standard headers */
    line("#include \"bi/bi.hpp\"");
  } else {
    /* include main header file */
    boost::filesystem::path file(o->path);
    file.replace_extension(".hpp");
    line("#include \"" << file.filename().string() << "\"\n");

    /* dispatcher header code */
    CppDispatcherGenerator auxDispatcher(base, level, true);
    auxDispatcher << o;

    line("");
  }

  /* main code */
  *this << o->root;

  /* dispatcher source code */
  if (!header) {
    CppDispatcherGenerator auxDispatcher(base, level, false);
    auxDispatcher << o;
  }
}

void bi::CppFileGenerator::visit(const Import* o) {
  if (header) {
    line("#include \"" << hpp(o->path->file()) << "\"");
  }
}

void bi::CppFileGenerator::visit(const VarDeclaration* o) {
  if (header) {
    line("namespace bi {");
    line("extern " << o->param->type << ' ' << o->param->name << ';');
    line("}\n");
  } else {
    start(o->param->type << " bi::" << o->param->name);
    if (!o->param->value->isEmpty()) {
      middle(" = " << o->param->value);
    }
    finish(';');
  }
}

void bi::CppFileGenerator::visit(const FuncParameter* o) {
  if (!o->braces->isEmpty() && (header || !o->parens->hasAssignable())) {
    if (header) {
      line("namespace bi {");
    }

    /* template parameters */
    CppTemplateParameterGenerator auxTemplateParameter(base, level, header);
    auxTemplateParameter << o;

    /* return type */
    start(o->type << ' ');

    /* name */
    if (!header) {
      middle("bi::");
    }
    if ((o->isBinary() || o->isUnary()) && isTranslatable(o->name->str())) {
      middle("operator" << o->name);
    } else {
      middle(o->name);
    }

    /* parameters */
    CppParameterGenerator auxParameter(base, level, header);
    auxParameter << o;

    if (header && !o->parens->hasAssignable()) {
      finish(';');
    } else {
      finish(" {");
      in();

      /* output parameters */
      CppOutputGenerator auxOutput(base, level, false);
      auxOutput << o;

      /* body */
      CppBaseGenerator aux(base, level, false);
      aux << o->braces;

      /* return statement */
      if (!o->result->isEmpty()) {
        CppReturnGenerator auxReturn(base, level, false);
        auxReturn << o;
      }

      out();
      finish("}\n");
    }
    if (header) {
      line("}\n");
    }
  }
}

void bi::CppFileGenerator::visit(const ModelParameter* o) {
  if (o->isEqual()) {
    if (header) {
      ModelReference* base = dynamic_cast<ModelReference*>(o->base.get());
      assert(base);
      if (!base->isBuiltin()) {
        line("namespace bi {");
        in();
        line("namespace model {");
        out();
        line("template<class Group = MemoryGroup>");
        start("using " << o->name << " = ");
        //if (base->isBuiltin()) {
        //  middle("PrimitiveValue<" << base << ",Group>");
        //} else {
          middle(base->name << "<Group>");
        //}
        finish(';');
        in();
        line("}");
        out();
        line("}\n");
      }
    }
  } else if (!o->isBuiltin()) {
    if (header) {
      line("namespace bi {");
      in();
      line("namespace model {");
      out();
    }
    CppModelGenerator auxModel(base, level, header);
    auxModel << o;
    if (header) {
      in();
      line("}");
      out();
      line("}\n");
    }
  }
}

void bi::CppFileGenerator::visit(const ProgParameter* o) {
  if (header) {
    line("namespace bi {");
    in();
    line("namespace program {");
    out();
    line("extern \"C\" void " << o->name << "(int argc, char** argv);");
    in();
    line("}");
    out();
    line("}\n");
  } else {
    line("void bi::program::" << o->name << "(int argc, char** argv) {");
    in();
    if (o->parens->tupleSize() > 0) {
      /* option variables */
      for (auto iter = o->parens->begin(); iter != o->parens->end(); ++iter) {
        line(*iter << ';');
      }
      line("");

      /* option flags */
      line("enum {");
      in();
      for (auto iter = o->parens->begin(); iter != o->parens->end(); ++iter) {
        std::string flag = dynamic_cast<const VarParameter*>(*iter)->name->str() + "_ARG";
        boost::to_upper(flag);
        start(flag);
        if (iter == o->parens->begin()) {
          middle(" = 256");
        }
        finish(',');
      }
      out();
      line("};");

      /* long options */
      line("int c, option_index;");
      line("option long_options[] = {");
      in();
      for (auto iter = o->parens->begin(); iter != o->parens->end(); ++iter) {
        const std::string& name = dynamic_cast<const VarParameter*>(*iter)->name->str();
        if (name.length() > 1) {
          std::string flag = name + "_ARG";
          boost::to_upper(flag);
          std::string option = name;
          boost::replace_all(option, "_", "-");

          line(
              "{\"" << option << "\", required_argument, 0, " << flag << " },");
        }
      }
      line("{0, 0, 0, 0}");
      out();
      line("};");

      /* short options */
      start("const char* short_options = \"");
      for (auto iter = o->parens->begin(); iter != o->parens->end(); ++iter) {
        const std::string& name = dynamic_cast<const VarParameter*>(*iter)->name->str();
        if (name.length() == 1) {
          middle(name << ':');
        }
      }
      finish("\";");

      /* read in options with getopt_long */
      line("opterr = 0; // handle error reporting ourselves");
      line(
          "c = getopt_long(argc, argv, short_options, long_options, &option_index);");
      line("while (c != -1) {");
      in();
      line("switch (c) {");
      in();

      for (auto iter = o->parens->begin(); iter != o->parens->end(); ++iter) {
        const std::string& name = dynamic_cast<const VarParameter*>(*iter)->name->str();
        std::string flag = name + "_ARG";
        boost::to_upper(flag);

        start("case ");
        if (name.length() > 1) {
          middle(flag);
        } else {
          middle('\'' << name << '\'');
        }
        finish(':');
        in();
        start(name << " = ");
        Type* type = (*iter)->type->strip();
        const ModelReference* ref = dynamic_cast<const ModelReference*>(type);
        if (!ref) {
          const RandomType* random = dynamic_cast<const RandomType*>(type);
          if (random) {
            ref = dynamic_cast<const ModelReference*>(random->left.get());
          }
        }

        if (ref) {
          std::string typeName = ref->name->str();
          if (typeName == "Boolean") {
            middle("atoi(optarg)");
          } else if (typeName == "Integer") {
            middle("atoi(optarg)");
          } else if (typeName == "Real") {
            middle("atof(optarg)");
          } else if (typeName == "String") {
            middle("optarg");
          } else {
            throw UnsupportedOptionTypeException(type);
          }
        } else {
          throw UnsupportedOptionTypeException(type);
        }
        finish(';');
        line("break;");
        out();
      }
      line("default:");
      in();
      line("throw UnknownOptionException(argv[optind - 1]);");
      line("break;");
      out();
      out();
      line('}');
      line(
          "c = getopt_long(argc, argv, short_options, long_options, &option_index);");
      out();
      line("}\n");
    }

    /* initialise */
    line("method = new Stack();");

    /* body of program */
    if (!o->braces->isEmpty()) {
      CppBaseGenerator aux(base, level, header);
      aux << o->braces;
    }

    /* terminate */
    line("delete method;");

    out();
    line("}\n");
  }
}

std::string bi::CppFileGenerator::hpp(const std::string& path) {
  boost::filesystem::path file = path;
  file.replace_extension(".hpp");
  return file.string();
}
