#!/bin/bash

# IFJ24 - Příklady pro studenty pro dokreslení zadání (spuštění konfigurováno pro server Merlin)

COMPILER=""  # zde může být váš překladač IFJ24 (např. ./ifj24)
HELPER="ifj24.zig"
INTERPRETER="./run.sh"  # zde může být interpret IFJcode24 (např. ./ic24int)

for i in ./ifj24_programs/*.zig; do

    if [[ $i == "./ifj24_programs/$HELPER" ]]; then
        continue
    fi

    IN=$(echo "$i" | sed 's#./ifj24_programs/#in_reference/#' | sed 's/\(.*\)\.zig/\1.in/')
    OUT=$(echo "$i" | sed 's#./ifj24_programs/#out_reference/#' | sed 's/\(.*\)\.zig/\1.out/')
    PRG=$(basename "$i")

    # $COMPILER $i $HELPER > $PRG   # nekontrolují se chyby překladu (pro chybové testy nutno rozšířit)
    # RETURNED_COMPILER=$?

    echo -n "DIFF: $INTERPRETER $PRG and $OUT: "
    cat "$IN" | $INTERPRETER "$PRG" 2> >(sed $'s,.*,\e[31m&\e[m,' >&2) | diff - "$OUT" > /dev/null 2> /dev/null
    DIFFOK=$?
    if [ "$DIFFOK" -eq 0 ]; then
        echo "OK"
    else
        echo "DIFFERENCE to $OUT"
        cat "$IN" | $INTERPRETER "$PRG" 2> >(sed $'s,.*,\e[31m&\e[m,' >&2) | diff - "$OUT"
    fi
    echo
done

exit 0