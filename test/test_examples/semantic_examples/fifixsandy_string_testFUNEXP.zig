const ifj = @import("ifj24.zig");

pub fn main() void {
    const a = ifj.readstr();
    var a_without_null = ifj.string("");
    if(a) |b|{
        a_without_null = b;
    }
    else{}
    ifj.write(a);
    ifj.write(ifj.string("\n"));
    const str_after = ifj.string("--after");
    const str_before = ifj.string("before--");
    
    const len_after = ifj.length(str_after);
    ifj.write(ifj.concat(ifj.string("Sring and length: "), str_after));
    ifj.write(len_after);
    ifj.write(ifj.string("\n"));

    var cc = ifj.concat(ifj.concat(str_before, a_without_null), str_after);
    ifj.write(cc);
    ifj.write(ifj.string("\n"));

    cc = ifj.concat(ifj.string(""), ifj.string(""));
    ifj.write(cc);
    ifj.write(ifj.length(cc));
    ifj.write(ifj.string("\n"));

    const test: []u8 = ifj.string("abcdef");
    const sub = ifj.substring(test, 1, 3);
    if(sub) |sub_without_null|{
        ifj.write(ifj.concat(sub_without_null, ifj.string("\n")));
    }
    else{}

    ifj.write(ifj.string("abeceda: "));
    
    var i = 65;
    while(90 >= i){
        const j = i;
        ifj.write(ifj.chr(j));
        i = i+      1;
    }
    ifj.write(ifj.string("\n"));

    const a_len = ifj.length(a_without_null);
    var j = 0;
    while(a_len > j){
        ifj.write(ifj.ord(a_without_null, j));
        ifj.write(ifj.string("\n"));
        j = j + 1;
    }
}