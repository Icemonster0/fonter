# Font Syntax

### Preamble
Every font file starts with a preamble:
```
SPACE_WIDTH <value>
TAB_WIDTH <value>
LINE_SPACING <value>
GLYPH_SIZE <value>
```

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

### Add find-and-replace rule
```
FNR_RULE
    FIND <symbol> <symbol> ...
    REPLACE <symbol> <symbol> ...
    END
```

### Add attach rule
```
ATTACH_RULE <base_symbol> <attach_symbol>
```

### Comments
C-like line and block comments are supported.

### Abbreviations
The following abbreviations are available (note the `_` to avoid ambiguity):
```
SPACE_WIDTH -> SW_
TAB_WIDTH -> TW_
LINE_SPACING -> LS_
GLYPH_SIZE -> GS_
SYMBOL -> S_
LEFT -> L_
RIGHT -> R_
TOP -> T_
BOTTOM -> B_
FNR_RULE -> FR_
ATTACH_RULE -> AR_
```
