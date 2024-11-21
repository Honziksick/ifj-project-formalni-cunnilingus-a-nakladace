// Program: Prirazeni hodnoty do nekompatibilniho typu
//ERR7

const ifj = @import("ifj24.zig");
pub fn main() void {
    ifj.write("yag er'ouy\n");
    
    var a: f64 = 12.5;

    a = "kys";  //Prirazeni []u8 do f64

    ifj.write(a);
}