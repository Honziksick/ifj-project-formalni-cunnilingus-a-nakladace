// Program: Redefinovana funkce
//ERR5

const ifj = @import("ifj24.zig");
pub fn main() void {
    ifj.write("ypitv izahcod im zu\n");

    const a: i32 = decrement(10, 5);

    ifj.write(a);
}

pub fn decrement(n: i32, m: i32) i32 {
    return n - m;
}

pub fn decrement(o: f64, p: f64) f64 {  //Funkce 'decrement' je definavana dvakrat
    return o - p;
}