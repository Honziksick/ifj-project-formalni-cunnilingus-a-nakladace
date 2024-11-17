// Program: Chybejici return ve volane funkci, ktera ma neco vratit
//ERR4

const ifj = @import("ifj24.zig");
pub fn main() void {
    ifj.write("ykcezej dar mam\n");

    var a: i32 = decrement(10, 5);

    ifj.write(a);
}

pub fn decrement(n: i32, m: i32) i32 {
    //Return to zabalil
}