# Font Syntax

### Preamble
Every font file starts with a preamble:
```
SPACE_WIDTH <value>
TAB_WIDTH <value>
LINE_SPACING <value>
GLYPH_SIZE <value>
```
All values are in pixels (before applying scale). `GLYPH_SIZE` is used to determine automatic line breaks and the size of the box representation of unknown symbols. It should be the height of a standard symbol.

### Add symbol
```
SYMBOL <name> <type> <texture_path>
    <anchor_x> <anchor_y>
    <attach_x> <attach_y>
    <next_x> <next_y>
    [LEFT <int>]
    [RIGHT <int>]
    [TOP <int>]
    [BOTTOM <int>]
    END
```
`name` can be any string, `type` is an integer (`0` is reserved for space (`' '`), `-1` for line break (`'\n'`), `-2` for tab (`'\t'`) and `-3` for unknown symbols), `texture_path` is a relative path to the texture for the symbol. The next six values are in pixels (before scaling) and mean the following:

+ `anchor`: where the texture will be attached to attachment points
+ `attach`: attachment point for diacritics
+ `next`: attachment point for the following symbol

`LEFT`, `RIGHT`, `TOP` and `BOTTOM` are optional. The texture will be cropped to these bounds (inclusive). `anchor`, `attach` and `next` are coordinates on the cropped texture with the point `0, 0` at the bottom left corner.

### Add find-and-replace rule
```
FNR_RULE
    FIND <symbol> <symbol> ...
    REPLACE <symbol> <symbol> ...
    END
```
Find all instances of the sequence of symbols in `FIND` in the input text and replace them with the sequence of symbols in `REPLACE` (In the beginning, every character of the input text is its own symbol, so `hello` would be parsed as `{"h", "e", "l", "l", "o"}`).  
`REPLACE` may be empty (for deletion), `FIND` may not. In `FIND`, instead of any symbol a type reference can be used (`$x` where x is the type). This way, if a symbol has this type, it will be treated as a matching symbol and the name of the symbol will be saved as a variable of this `FNR_RULE`. In `REPLACE`, instead of any symbol a variable reference may be used (`$x` where x is the index of a variable of this `FNR_RULE`, starting at 1). The value of this variable will be used as the replacement.  
For example, the following rule will swap the two symbols of all instances of a symbol of type 4 followed by a symbol of type 15 while placing an `a` between them:
```
FNR_RULE FIND $4 $15 REPLACE $2 a $1 END
```

### Add attach rule
```
ATTACH_RULE <base_symbol> <attach_symbol>
```
Any instance of the base symbol being followed by the attach symbol is replaced by the base symbol with the attach symbol attached to it (using `attach_x` and `attach_y` of the base symbol). You can use type references here as well.  
For example, the two following rules replace all instances of two symbols of type 3 next to each other with the first symbol with `accute_accent` attached to it:
```
FNR_RULE FIND $3 $3 REPLACE $1 accute_accent END
ATTACH_RULE $3 accute_accent
```

### Comments
C-like line and block comments are supported.

### Abbreviations
The following abbreviations are available (note the `_` to avoid ambiguity):
```
SPACE_WIDTH = SW_
TAB_WIDTH = TW_
LINE_SPACING = LS_
GLYPH_SIZE = GS_
SYMBOL = S_
LEFT = L_
RIGHT = R_
TOP = T_
BOTTOM = B_
FNR_RULE = FR_
FIND = F_
REPLACE = R_
ATTACH_RULE = AR_
END = E_
```
