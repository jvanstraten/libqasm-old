
import sys
import textwrap
from collections import namedtuple

Member = namedtuple("Member", ["attrib", "typ", "name", "doc"])

class Class(object):
    def __init__(self, name, parent):
        super().__init__()
        self.name = name
        self.parent = parent
        self.doc = []
        self.children = []
        self.members = []

    def comment(self, indent, lines):
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

    def classlist(self):
        s = []
        s.append("    class %s;" % self.name)
        for c in self.children:
            s.append(c.classlist())
        return "\n".join(s)

    def header(self, properties):
        s = []

        # Class header.
        if self.doc:
            s.append(self.comment(1, [" ".join(map(str.strip, self.doc))]))
        if self.parent:
            s.append("    class %s : public %s {" % (self.name, self.parent.name))
        elif properties["derive"]:
            s.append("    class %s : public %s {" % (self.name, properties["derive"]))
        else:
            s.append("    class %s {" % (self.name))
        s.append("    public:")

        # Members.
        for member in self.members:
            s.append("")
            s.append(self.comment(2, [member.doc]))
            typ = member.typ
            if "v" in member.attrib:
                typ = "std::vector<%s>" % typ
            s.append("        %s %s;" % (typ, member.name))

        # Constructor(s).
        need_more_cons = True
        num_opt_total = 0
        while need_more_cons:
            need_more_cons = False
            num_opt = num_opt_total
            num_opt_total += 1
            s.append("")
            cons_args = []
            cons_doc = ["Constructor for %s." % self.name]
            for member in self.members:
                include = False
                if "c" in member.attrib:
                    include = True
                elif "o" in member.attrib:
                    if num_opt > 0:
                        include = True
                        num_opt -= 1
                    else:
                        need_more_cons = True
                if include and "v" in member.attrib:
                    raise ValueError("Cannot add vector to initializer list; use generated push() function.")
                if include:
                    cons_args.append("%s %s" % (member.typ, member.name))
                    cons_doc.append("@param %s %s" % (member.name, member.doc))
            s.append(self.comment(2, cons_doc))
            s.append("        %s(%s);" % (self.name, ", ".join(cons_args)))

        # Destructor.
        s.append("")
        s.append(self.comment(2, ["Default destructor for %s." % self.name]))
        s.append("        virtual ~%s() = default;" % (self.name))

        # Push function for vector.
        for member in self.members:
            if "v" in member.attrib:
                s.append("")
                s.append(self.comment(2, [
                    "Appends to %s vector. Returns reference to this to allow chaining." % member.name[:-1],
                    "@param %s Value to push." % member.name[:-1],
                    "@return this, to allow chaining."
                ]))
                s.append("        %s &push_%s(%s %s);" % (self.name, member.name[:-1], member.typ, member.name[:-1]))
            if "V" in member.attrib:
                s.append("")
                s.append(self.comment(2, [
                    "Appends %s vector by another vector. Returns reference to this to allow chaining." % member.name[:-1],
                    "@param %s Vector to push." % member.name,
                    "@return this, to allow chaining."
                ]))
                s.append("        %s &push_%s(std::vector<%s> &%s);" % (self.name, member.name, member.typ, member.name))


        s.append("")
        s.append("    };")
        s.append("")
        for c in self.children:
            s.append(c.header(properties))
        return "\n".join(s)

    def source(self, properties):
        s = []

        # Constructor(s).
        need_more_cons = True
        num_opt_total = 0
        while need_more_cons:
            need_more_cons = False
            num_opt = num_opt_total
            num_opt_total += 1
            cons_args = []
            cons_doc = ["Constructor for %s." % self.name]
            inits = []
            for member in self.members:
                include = False
                if "c" in member.attrib:
                    include = True
                elif "o" in member.attrib:
                    if num_opt > 0:
                        include = True
                        num_opt -= 1
                    else:
                        need_more_cons = True
                if include and "v" in member.attrib:
                    raise ValueError("Cannot add vector to initializer list; use generated push() function.")
                if include:
                    cons_args.append("%s %s" % (member.typ, member.name))
                    cons_doc.append("@param %s %s" % (member.name, member.doc))
                    inits.append("        %s(%s)" % (member.name, member.name))
            s.append("")
            s.append(self.comment(1, cons_doc))
            if inits:
                s.append("    %s::%s(%s):" % (self.name, self.name, ", ".join(cons_args)))
                s.append(",\n".join(inits))
                s.append("    {}")
            else:
                s.append("    %s::%s(%s) {" % (self.name, self.name, ", ".join(cons_args)))
                s.append("    }")

        # Push function for vector.
        for member in self.members:
            if "v" in member.attrib:
                s.append("")
                s.append(self.comment(1, [
                    "Appends to %s vector. Returns reference to this to allow chaining." % member.name[:-1],
                    "@param %s Value to push." % member.name[:-1],
                    "@return this, to allow chaining."
                ]))
                s.append("    %s &%s::push_%s(%s %s) {" % (self.name, self.name, member.name[:-1], member.typ, member.name[:-1]))
                s.append("        return *this;")
                s.append("    }")
            if "V" in member.attrib:
                s.append("")
                s.append(self.comment(1, [
                    "Appends %s vector by another vector. Returns reference to this to allow chaining." % member.name[:-1],
                    "@param %s Vector to push." % member.name,
                    "@return this, to allow chaining."
                ]))
                s.append("    %s &%s::push_%s(std::vector<%s> &%s) {" % (self.name, self.name, member.name, member.typ, member.name))
                s.append("        return *this;")
                s.append("    }")


        for c in self.children:
            s.append(c.source(properties))
        return "\n".join(s)


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
                attrib, typ, name, doc = member
                attrib = set(attrib)
                if "&" in attrib:
                    typ = "std::shared_ptr<%s>" % typ
                    attrib.remove("&")
                if "v" in attrib:
                    name += "s"
                cls.members.append(Member(attrib, typ, name, doc))
            class_list.append(cls)
        elif mode:
            raise ValueError("Unknown mode")
    except:
        raise ValueError("On line %d: %s" % (line_nr, line))


s = []
s.extend(header_head)
s.append("")
s.append("    /*")
s.append("     * Forward declarations for all classes defined here.")
s.append("     */")
for cls in toplevel:
    s.append(cls.classlist())
s.append("")
for cls in toplevel:
    s.append(cls.header(properties))
s.append("")
s.extend(header_foot)
s.append("")
s = "\n".join(s)
with open(properties["header"], "w") as f:
    f.write(s)

s = []
s.extend(source_head)
s.append("")
for cls in toplevel:
    s.append(cls.source(properties))
s.append("")
s.extend(source_foot)
s.append("")
s = "\n".join(s)
with open(properties["source"], "w") as f:
    f.write(s)
