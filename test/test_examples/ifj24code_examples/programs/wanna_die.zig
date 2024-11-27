// Ukazka prace s retezci a vestavenymi funkcemi
const ifj = @import("ifj24.zig");

pub fn main() void {
    // Proměnné
    var a: i32 = 0;
    var b: f64 = 0.0;
    var s: []u8 = ifj.string("s");
    var c: []u8 = ifj.string("s");
    var ord_val: i32 = 0;
    var chr_val: []u8 = ifj.string("s");
    var substr_val: []u8 = ifj.string("s");
    var cmp_res: i32 = 0;
    var len: i32 = 0;
    var j: i32 = 0;

    // Použití readi32
    ifj.write("Zadejte celé číslo: ");
    a = ifj.readi32();

    // Použití readf64
    ifj.write("Zadejte desetinné číslo: ");
    b = ifj.readf64();

    // Použití readstr
    ifj.write("Zadejte řetězec: ");
    s = ifj.readstr();

    // Použití write
    ifj.write("Zadali jste celé číslo: ");
    ifj.write(a);
    ifj.write("\n");

    ifj.write("Zadali jste desetinné číslo: ");
    ifj.write(b);
    ifj.write("\n");

    ifj.write("Zadali jste řetězec: ");
    ifj.write(s);
    ifj.write("\n");

    // Použití i2f
    const a_to_f: f64 = ifj.i2f(a);
    ifj.write("Převod celého čísla na float: ");
    ifj.write(a_to_f);
    ifj.write("\n");

    // Použití f2i
    const b_to_i: i32 = ifj.f2i(b);
    ifj.write("Převod desetinného čísla na int: ");
    ifj.write(b_to_i);
    ifj.write("\n");

    // Použití length
    len = ifj.length(s);
    ifj.write("Délka zadaného řetězce: ");
    ifj.write(len);
    ifj.write("\n");

    // Použití substr
    if (len > 0) {
        substr_val = ifj.substr(s, 0, 1);
        ifj.write("První znak řetězce: ");
        ifj.write(substr_val);
        ifj.write("\n");
    } else {}

    // Použití ord
    if (len > 0) {
        ord_val = ifj.ord(s, 0);
        ifj.write("ASCII hodnota prvního znaku: ");
        ifj.write(ord_val);
        ifj.write("\n");
    } else {}
}
