// Program: Var definovan dvakrat
//ERR5

const ifj = @import("ifj24.zig");
pub fn main() void {
    ifj.write("mau <=> uam\n");
    var a: i32 = 12;

    ifj.write(a);

    var a: f64 = 5; //Podruhe promenna 'a'

    ifj.write(a);
}