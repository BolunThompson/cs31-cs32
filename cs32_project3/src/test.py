import dis
import inspect
from collections import defaultdict

def detect_side_effects(func):
    """
    Analyze bytecode to determine:
      1) Which arguments and globals are mutated (i.e., have attributes or subscripts set).
      2) Once an argument is 'shadowed' by STORE_FAST, no further writes count as argument writes.
      3) We store these mutations in nested dictionaries:
            {
                "arguments": {
                    "arg_name": {
                        "attributes": {attr_names...},
                        "subscripts": {keys_or_indexes...},
                    },
                    ...
                },
                "globals": {
                    "global_name": {
                        "attributes": {attr_names...},
                        "subscripts": {keys_or_indexes...},
                    },
                    ...
                }
            }
    """

    code = func.__code__
    arg_count = code.co_argcount
    arg_names = code.co_varnames[:arg_count]

    # We track what each variable name currently represents: "arg", "global", or "local".
    # Once we see STORE_FAST on an "arg", that variable name becomes "local" (shadowed).
    env = {}
    for name in arg_names:
        env[name] = "arg"

    # For each recognized argument/global, store sets of mutated attributes/subscripts
    arg_mutations = defaultdict(lambda: {"attributes": set(), "subscripts": set()})
    global_mutations = defaultdict(lambda: {"attributes": set(), "subscripts": set()})

    def ensure_arg(name):
        """Make sure 'name' is still recognized as an argument before mutating."""
        return env.get(name) == "arg"

    def ensure_global(name):
        """Make sure 'name' is recognized as a global before mutating."""
        return env.get(name) == "global"

    # We maintain a stack of (kind, name_or_const).
    #   kind can be "arg", "global", "local", "const", "unknown", etc.
    #   name_or_const is either a variable name, a constant, or None.
    stack = []

    def push(kind, val):
        stack.append((kind, val))

    def pop():
        return stack.pop() if stack else ("unknown", None)

    # Disassemble once to iterate
    instructions = list(dis.get_instructions(func))

    for instr in instructions:
        op = instr.opname
        val = instr.argval

        if op == "LOAD_FAST":
            # Push the environment info
            kind = env.get(val, "local")
            push(kind, val)

        elif op == "STORE_FAST":
            # If we're storing to a name that's still "arg", we shadow it => becomes "local"
            if env.get(val) == "arg":
                env[val] = "local"

        elif op == "LOAD_GLOBAL":
            # Mark the name as global in the env
            env[val] = "global"
            push("global", val)

        elif op == "STORE_GLOBAL":
            # Directly storing into a global name => treat it as mutated
            env[val] = "global"
            global_mutations[val]["attributes"].add("<direct_store>")

        elif op == "LOAD_CONST":
            # Just push a constant
            push("const", val)

        elif op == "STORE_ATTR":
            # Bytecode stack: top-of-stack is the value being stored
            value_kind, value_name = pop()
            # Next popped item is the object whose attribute we set
            obj_kind, obj_name = pop()
            attr_name = instr.argval  # the attribute name
            # Mutating the object
            if obj_kind == "arg" and ensure_arg(obj_name):
                arg_mutations[obj_name]["attributes"].add(attr_name)
            elif obj_kind == "global" and ensure_global(obj_name):
                global_mutations[obj_name]["attributes"].add(attr_name)

        elif op == "STORE_SUBSCR":
            # For Python 3.x, this sets: container[index] = value
            # Bytecode stack top: index (TOS), container (TOS1), value (TOS2)? 
            # Actually CPython 3.10 doc says: "Implements TOS1[TOS] = TOS2".
            # So we need to pop in reverse: first the value, then the container, then the index.
            val_kind, val_name = pop()      # value being stored
            cont_kind, cont_name = pop()    # container
            index_kind, index_val = pop()   # subscript key/index
            if cont_kind == "arg" and ensure_arg(cont_name):
                arg_mutations[cont_name]["subscripts"].add(index_val)
            elif cont_kind == "global" and ensure_global(cont_name):
                global_mutations[cont_name]["subscripts"].add(index_val)

        # We'll ignore other ops (calls, closures, etc.) in this simple example.

    return {
        "arguments": dict(arg_mutations),
        "globals": dict(global_mutations),
    }

# Suppose we have a global
GLOBAL_DICT = {}

def example_func(a, b, c):
    a[0] = 100        # subscript write => 'a' mutated
    b.some_attr = 99  # attribute write => 'b' mutated
    b = "shadowed"    # overshadow 'b' => no further writes count as arg writes
    GLOBAL_DICT["test"] = "abc"  # subscript => 'GLOBAL_DICT' mutated
    GLOBAL_DICT = None           # overshadow => no more global tracking for 'GLOBAL_DICT'
    return a, b, c

info = detect_side_effects(example_func)
import pprint
pprint.pprint(info)
