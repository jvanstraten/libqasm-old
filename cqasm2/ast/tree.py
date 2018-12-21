
import sys
import textwrap
from collections import namedtuple

Member = namedtuple("Member", ["attrib", "typ", "name", "doc"])

def comment(indent, lines):
    indent = "    " * indent
    start = indent + "/**"
    mid   = indent + " * "
    end   = indent + " */"
    remain = 80 - len(mid)
    s = [start]
    for line in lines:
        s.extend([mid + wrapped for wrapped in textwrap.wrap(line, remain)])
    s.append(end)
    return "\n".join(s)

class Member(object):
    def __init__(self, attrib, typ, name, doc):
        super().__init__()
        self._attrib = set(attrib)
        self._typ = typ
        self._name = name
        self._doc = doc

    def is_vector(self):
        return "v" in self._attrib

    def is_shared_ptr(self):
        return "&" in self._attrib

    def make_shared(self, name):
        typ = self._typ
        if self.is_vector():
            typ = "std::vector<%s>" % typ
        return "std::shared_ptr<%s>(%s)" % (typ, name)

    def is_string(self):
        return self._typ == "std::string"

    def has_vec_push(self):
        return "V" in self._attrib

    def construct_default(self):
        return not self.construct_optional() and not self.construct_always()

    def construct_optional(self):
        return "o" in self._attrib

    def construct_always(self):
        return "c" in self._attrib

    def should_pointer_init_free(self):
        return self.is_string()

    def member_type(self):
        typ = self._typ
        if self.is_shared_ptr():
            typ = "std::shared_ptr<%s>" % typ
        if self.is_vector():
            typ = "std::vector<%s>" % typ
        return typ

    def member_name(self):
        name = self._name
        if self.is_vector():
            name = "%ss" % name
        return name

    def member_doc(self):
        return self._doc

    def normal_init_type(self):
        if self.is_vector():
            raise ValueError("Cannot initialize vector type in constructor")
        if self.is_shared_ptr():
            return "std::shared_ptr<%s>" % self._typ
        return self._typ

    def pointer_init_type(self):
        if self.is_vector():
            raise ValueError("Cannot initialize vector type in constructor")
        if self.is_shared_ptr():
            return "%s *" % self._typ
        if self.is_string():
            return "char *"
        return self._typ

    def init_name(self):
        return self._name

    def normal_init_doc(self):
        return self._doc

    def pointer_init_doc(self):
        doc = self._doc
        if self.is_string():
            doc += " The passed pointer will be free()d by this constructor!"
        return doc

    def normal_init(self, name):
        return name

    def pointer_init(self, name):
        if self.is_shared_ptr():
            return self.make_shared(name)
        if self.is_string():
            return "std::string(%s)" % name
        return name

    def push_type(self):
        if not self.is_vector():
            raise ValueError("Cannot add vector push op for non-vector")
        typ = self._typ
        if self.is_shared_ptr():
            typ = "std::shared_ptr<%s>" % typ
        return typ

    def push_type_ptr(self):
        if not self.is_shared_ptr():
            raise ValueError("Cannot add pointer push op for non-shared")
        return "%s *" % self._typ

    def push_make_shared(self, name):
        if not self.is_shared_ptr():
            raise ValueError("Cannot add pointer push op for non-shared")
        return "std::shared_ptr<%s>(%s)" % (self._typ, name)

    def push_name(self):
        return self._name

    def vec_push_type(self):
        typ = self._typ
        if self.is_shared_ptr():
            typ = "std::shared_ptr<%s>" % typ
        if not self.is_vector():
            raise ValueError("Cannot add vector push op for non-vector")
        typ = "std::vector<%s> &" % typ
        return typ

    def vec_push_name(self):
        name = self._name
        if self.is_vector():
            name = "%ss" % name
        return name


class Class(object):
    def __init__(self, name, parent):
        super().__init__()
        self.name = name
        self.parent = parent
        self.doc = []
        self.children = []
        self.members = []

    def classlist(self):
        s = []
        s.append("    class %s;" % self.name)
        for c in self.children:
            s.append(c.classlist())
        return "\n".join(s)

    def generate(self, properties):
        hdr = []
        src = []

        # Class header.
        if self.doc:
            hdr.append(comment(1, [" ".join(map(str.strip, self.doc))]))
        if self.parent:
            hdr.append("    class %s : public %s {" % (self.name, self.parent.name))
        elif properties["derive"]:
            hdr.append("    class %s : public %s {" % (self.name, properties["derive"]))
        else:
            hdr.append("    class %s {" % (self.name))
        hdr.append("    public:")

        # Class members declarations.
        for member in self.members:
            hdr.append("")
            hdr.append(comment(2, [member.member_doc()]))
            hdr.append("        %s %s;" % (member.member_type(), member.member_name()))

        # Constructor(s).
        need_more_cons = True
        num_opt_total = 0
        while need_more_cons:
            need_more_cons = False
            num_opt = num_opt_total
            num_opt_total += 1

            normal_args = []
            normal_doc = ["Constructor for %s." % self.name]
            normal_inits = []
            pointer_args = []
            pointer_doc = [
                "Constructor for %s, intended to be used from YACC only. This version:" % self.name,
                " - uses char* for strings and bare pointers instead of std::shared_ptr<> encapsulations for inputs;",
                " - calls free() on strings passed to it after constructing std::string instances."
            ]
            pointer_inits = []
            pointer_frees = []
            for member in self.members:
                use_default = True
                if member.construct_always():
                    use_default = False
                elif member.construct_optional():
                    if num_opt > 0:
                        use_default = False
                        num_opt -= 1
                    else:
                        need_more_cons = True

                if not use_default:
                    name = member.init_name()
                    normal_args.append("%s %s" % (member.normal_init_type(), name))
                    pointer_args.append("%s %s" % (member.pointer_init_type(), name))
                    normal_doc.append("@param %s %s" % (name, member.normal_init_doc()))
                    pointer_doc.append("@param %s %s" % (name, member.pointer_init_doc()))
                    normal_inits.append("        %s(%s)" % (name, member.normal_init(name)))
                    pointer_inits.append("        %s(%s)" % (name, member.pointer_init(name)))
                    if member.should_pointer_init_free():
                        pointer_frees.append("        free(%s);" % name)

            # Regular constructor; header.
            hdr.append("")
            hdr.append(comment(2, normal_doc))
            hdr.append("        %s(%s);" % (self.name, ", ".join(normal_args)))

            # Regular constructor; source.
            src.append("")
            src.append(comment(1, normal_doc))
            if normal_inits:
                src.append("    %s::%s(%s):" % (self.name, self.name, ", ".join(normal_args)))
                src.append(",\n".join(normal_inits))
                src.append("    {}")
            else:
                src.append("    %s::%s(%s) {" % (self.name, self.name, ", ".join(normal_args)))
                src.append("    }")

            if normal_args != pointer_args:
                # Pointer constructor; header.
                hdr.append("")
                hdr.append(comment(2, pointer_doc))
                hdr.append("        %s(%s);" % (self.name, ", ".join(pointer_args)))

                # Pointer constructor; source.
                src.append("")
                src.append(comment(1, pointer_doc))
                if pointer_inits:
                    src.append("    %s::%s(%s):" % (self.name, self.name, ", ".join(pointer_args)))
                    src.append(",\n".join(pointer_inits))
                    src.append("    {")
                    src.extend(pointer_frees)
                    src.append("    }")
                else:
                    src.append("    %s::%s(%s)" % (self.name, self.name, ", ".join(pointer_args)))
                    src.append("    {}")

        # Destructor.
        hdr.append("")
        hdr.append(comment(2, ["Default destructor for %s." % self.name]))
        hdr.append("        virtual ~%s() = default;" % (self.name))

        # Push functions for vectors.
        for member in self.members:
            if member.is_vector():
                doc = [
                    "Appends to %s vector. Returns reference to this to allow chaining." % member.push_name(),
                    "@param %s Value to push." % member.push_name(),
                    "@return this, to allow chaining."
                ]

                hdr.append("")
                hdr.append(comment(2, doc))
                hdr.append("        %s *push_%s(%s %s);" % (self.name, member.push_name(), member.push_type(), member.push_name()))

                src.append("")
                src.append(comment(1, doc))
                src.append("    %s *%s::push_%s(%s %s) {" % (self.name, self.name, member.push_name(), member.push_type(), member.push_name()))
                src.append("        this->%s.push_back(%s);" % (member.member_name(), member.push_name()));
                src.append("        return this;")
                src.append("    }")

                if member.is_shared_ptr():
                    hdr.append("")
                    hdr.append(comment(2, doc))
                    hdr.append("        %s *push_%s(%s %s);" % (self.name, member.push_name(), member.push_type_ptr(), member.push_name()))

                    src.append("")
                    src.append(comment(1, doc))
                    src.append("    %s *%s::push_%s(%s %s) {" % (self.name, self.name, member.push_name(), member.push_type_ptr(), member.push_name()))
                    src.append("        this->%s.push_back(%s);" % (member.member_name(), member.push_make_shared(member.push_name())));
                    src.append("        return this;")
                    src.append("    }")

                if member.has_vec_push():
                    doc = [
                        "Appends %s vector by another vector. Returns reference to this to allow chaining." % member.vec_push_name(),
                        "@param %s Vector to push." % member.vec_push_name(),
                        "@return this, to allow chaining."
                    ]

                    hdr.append("")
                    hdr.append(comment(2, doc))
                    hdr.append("        %s *push_%s(%s %s);" % (self.name, member.vec_push_name(), member.vec_push_type(), member.vec_push_name()))

                    src.append("")
                    src.append(comment(1, doc))
                    src.append("    %s *%s::push_%s(%s %s) {" % (self.name, self.name, member.vec_push_name(), member.vec_push_type(), member.vec_push_name()))
                    src.append("        this->%s.insert(this->%s.end(), %s.begin(), %s.end());" % (member.member_name(), member.member_name(), member.vec_push_name(), member.vec_push_name()))
                    src.append("        return this;")
                    src.append("    }")

        hdr.append("")
        hdr.append("    };")
        hdr.append("")
        for c in self.children:
            h, s = c.generate(properties)
            hdr.append(h)
            src.append(s)
        return "\n".join(hdr), "\n".join(src)



if len(sys.argv) != 2:
    print("Usage: %s <fname.tree>" % sys.argv[0])
    sys.exit(1)

with open(sys.argv[1], "r") as f:
    data = f.read().replace("\\\n", "")

data = list(enumerate(map(lambda line: line.split("###")[0].rstrip(), data.split("\n"))))

mode = None
properties = {}
header_head = []
header_foot = []
source_head = []
source_foot = []
toplevel = []
for i, line in data:
    try:
        line_nr = i + 1
        if line.startswith("@"):
            mode = line[1:]
            continue
        if mode == "properties":
            if not line.strip():
                continue
            x = line.split(":", maxsplit=1)
            if len(x) != 2:
                raise ValueError("Invalid property, missing :")
            key, value = x
            properties[key.strip()] = value.strip()
        elif mode == "header_head":
            header_head.append(line)
        elif mode == "header_foot":
            header_foot.append(line)
        elif mode == "source_head":
            source_head.append(line)
        elif mode == "source_foot":
            source_foot.append(line)
        elif mode == "classes":
            if not line.strip():
                continue
            indent = len(line) - len(line.lstrip())
            if indent % 4:
                raise ValueError("Indent not multiple of four spaces")
            indent //= 4
            class_list = toplevel
            parent = None
            for _ in range(indent):
                if not class_list:
                    raise ValueError("Wrong indent, missing parent class")
                parent = class_list[-1]
                class_list = parent.children

            line = line.strip()
            if line.startswith("/"):
                parent.doc.append(line[1:])
                continue

            x = line.split(":", maxsplit=1)
            if len(x) != 2:
                raise ValueError("Invalid property, missing :")
            name, members = x
            name = name.strip()
            members = [tuple((y.strip() for y in x.split(maxsplit=3))) for x in members.split("|") if x.strip()]
            cls = Class(name, parent)
            for member in members:
                if len(member) != 4:
                    raise ValueError("Invalid member")
                cls.members.append(Member(*member))
            class_list.append(cls)
        elif mode:
            raise ValueError("Unknown mode")
    except:
        raise ValueError("On line %d: %s" % (line_nr, line))


hdr = []
src = []
for cls in toplevel:
    h, s = cls.generate(properties)
    hdr.append(h)
    src.append(s)


s = []
s.extend(header_head)
s.append("")
s.append("    /*")
s.append("     * Forward declarations for all classes defined here.")
s.append("     */")
for cls in toplevel:
    s.append(cls.classlist())
s.append("")
s.extend(hdr)
s.append("")
s.extend(header_foot)
s.append("")
s = "\n".join(s)
with open(properties["header"], "w") as f:
    f.write(s)

s = []
s.extend(source_head)
s.append("")
s.extend(src)
s.append("")
s.extend(source_foot)
s.append("")
s = "\n".join(s)
with open(properties["source"], "w") as f:
    f.write(s)
