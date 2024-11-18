// Program: Prirazeni hodnoty do konstanty
//ERR5

const ifj = @import("ifj24.zig");
pub fn main() void {
    ifj.write("seygex.inc\n");
    const a = ifj.readi32();

    a = 12; //Prirazeni '12' do const

    ifj.write(a);
}