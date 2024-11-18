// Program: Spatny pocet parametru
//ERR4

const ifj = @import("ifj24.zig");
pub fn main() void {
    ifj.write("ejugnuf mitaz ot it dans\n");

    const a: i32 = decrement(10); //Spatny pocet parametru

    ifj.write(a);
}

pub fn decrement(n: i32, m: i32) i32 {
    return n - m;
}

//Odhalil bys chybu, i kdybych tam dal 3 parametry?