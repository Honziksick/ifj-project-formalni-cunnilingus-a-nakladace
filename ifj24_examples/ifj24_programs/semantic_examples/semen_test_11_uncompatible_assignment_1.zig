// Program: Prirazeni hodnoty do nekompatibilniho typu
//ERR7

const ifj = @import("ifj24.zig");
pub fn main() void {
    ifj.write("yag m'i\n");
    
    var a: i32 = 12;

    a = 30;

    a = 15.5;   //Prirazeni f64 do i32

    ifj.write(a);
}