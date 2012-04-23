"""
Define RPC functions needed to generate
"""

# [ <ret-type>, [<arg_types>] ]
func_table = [
    [ "int", [] ],
    [ "int", ["int"] ],
    [ "int", ["int", "string", "int", "int"] ],    
    [ "int", ["string"] ],
    [ "int", ["string", "int"] ],
    [ "int", ["string", "string"] ],
    [ "int", ["string", "string", "string"] ],
    [ "int", ["string", "string", "int", "int"] ],
    [ "int", ["string", "string", "string", "string"]],
    [ "int64", [] ],
    [ "int64", ["string"] ],
    [ "string", [] ],
    [ "string", ["string"] ],
    [ "string", ["string", "int"] ],
    [ "string", ["string", "string"] ],
    [ "string", ["string", "string", "string"] ],
    [ "string", ["string", "string", "string", "string"] ],
    [ "string", ["string", "string", "string", "string", "int"] ],
    [ "string", ["string", "string", "string", "string", "string"] ],
    [ "objlist", [] ],
    [ "objlist", ["string"] ],
    [ "objlist", ["int", "int"] ],
    [ "objlist", ["string", "int"] ],
    [ "objlist", ["string", "int", "int"] ],
    [ "objlist", ["string", "string", "int"] ],
    [ "object", [] ],
    [ "object", ["int"] ],
    [ "object", ["string"] ],
]
