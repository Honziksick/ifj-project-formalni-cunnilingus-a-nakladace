// Program: Chybejici navratova hodnota
//ERR4

const ifj = @import("ifj24.zig");
pub fn main() void {
    ifj.write("nytnelaV izilb es zu\n");

    const a: i32 = decrement(10, 5);

    ifj.write(a);
}

pub fn decrement(n: i32, m: i32) i32 {
    return;     //Return => Kys
}